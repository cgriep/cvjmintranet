<?php

/**
 * Holt einen Buchungsdatensatz mit der angegebenen Buchungsnummer
 * Der Datensatz enth�lt die gesamte Buchung plus den Adressdatensatz des Kunden 
 * @param int $Buchung_Nr die buchungsnummer
 * @return array Buchungs- und Adressdatensatz 
 */
function holeBuchung($Buchung_Nr)
{
  if ( ! is_numeric($Buchung_Nr) )
  {
    die('<span class="SchwererFehler">&gt;&gt;&gt; Fehler in holeBuchung - Buchung_Nr nicht numerisch!</span>');
  }
  $query = sql_query("SELECT * FROM ".TABLE_BUCHUNGEN." INNER JOIN ".TABLE_ADRESSEN.
    " ON F_Adressen_id = Adressen_id WHERE Buchung_Nr = ".$Buchung_Nr);
  $buchung = sql_fetch_array($query);
  sql_free_result($query);
  return $buchung;
}

/**
  * Bucht einen Artikel zur Buchung und erg�nzt alle zugeh�rigen Artikel der Gruppe
 * Wenn die Menge des zuzubuchenden Artikels 0 betr�gt wird er nur ein einziges Mal
 * zugebucht. 
 * Um mehrere Tage auf einmal zu buchen, Datum in Form von Anfangsdatum-Enddatum angeben
 * @param int $buchung_nr Die Buchungsnummer
 * @param int $artikel_id die ID des Artikels
 * @param int/String $datum das Datum als Timestamp oder Timestamp-Timestamp als von-bis
 * @param double $menge die Menge
 * @param double $dauer die Dauer
 * @param int $ebene die Ebene des Artikels f�r rekursives Einbuchen. Maximal 10 Ebenen werden gebucht. 
 */
function bucheArtikel($buchung_nr, $artikel_id, $datum, $menge=1, $dauer=0, $ebene=0)
{
  // Absicherung: Keine Endlosschleifen bei rekursiver Einbuchung
  if ( $ebene > 10) return;
  if ( ! is_numeric($datum) )
  {
    $Datum = explode("-",$datum);
    if ( is_array($Datum) )
      for ( $i = $Datum[0]; $i <= $Datum[1]; $i = strtotime("+1 day",$i))
      {
        bucheArtikel($buchung_nr, $artikel_id, $i, $menge, $dauer);
      }
  }
  else
  {
    $menge = str_replace(",",".",$menge);
    $dauer = str_replace(",",".",$dauer);
    if ( ! is_numeric($dauer) ) $dauer = 0;
    if ( ! is_numeric($menge) ) $menge = 1;
    pruefeArtikelVorhanden($buchung_nr, $artikel_id, $datum, $dauer);
    if ( ! sql_query("INSERT INTO ".TABLE_BUCHUNGSEINTRAEGE.
            " (F_Buchung_Nr, F_Artikel_Nr, Menge,Dauer,Datum) VALUES ".
            "($buchung_nr,$artikel_id,$menge,$dauer,$datum)") )
             sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE.
            " SET Menge=Menge+$menge, Dauer=$dauer WHERE F_Buchung_Nr=$buchung_nr AND ".
            "F_Artikel_Nr=$artikel_id AND Datum=$datum");
    // Artikelgruppen zubuchen
    $query = sql_query("SELECT F_Unterartikel_id, Menge, Dauer, Beginn FROM ".TABLE_ARTIKELGRUPPEN.
      " WHERE F_Artikel_id=$artikel_id");
    while ( $art = sql_fetch_array($query) )
    {
       // Wenn die vorangegangene Abfrage scheitert, ist vermutlich bereits ein
       // Datensatz vorhanden. Dann muss die Menge angepasst werden.
       if ( $art["Menge"] == 0 )
       {
         // Pr�fen, ob schon vorhanden
         if ( ! $q = sql_query("SELECT Count(F_Artikel_Nr) FROM ".TABLE_BUCHUNGSEINTRAEGE.
           " WHERE F_Buchung_Nr=".$buchung_nr." AND F_Artikel_Nr=".$artikel_id) )
             echo sql_error();
         $meineMenge = 0;
         if ( $row = sql_fetch_row($q) )
         {
           if ( $row[0] == 1 )
           {
             $meineMenge = 1;
             // Derartige Pauschalen sollen am n�chsten Tag gebucht werden, wenn nicht
             // anders angegeben (weil sonst im "0"-Bereich der frei-Pauschalen)
             if ( $art["Beginn"] == 0 )
               $art["Beginn"] = 86400;
           }
         }
         sql_free_result($q);
       }
       else
         $meineMenge = $menge * $art["Menge"];
       if ( $meineMenge != 0 )
       {
         if ( $art["Beginn"] != 0 )
           $ndatum = $datum + $art["Beginn"];
         else
           $ndatum = $datum;
         if ( $art["Dauer"] != 0 )
           $ndauer = $art["Dauer"];
         else
           $ndauer = $dauer;
         pruefeArtikelVorhanden($buchung_nr, $art["F_Unterartikel_id"], $ndatum, $ndauer, $ebene+1);
         // Rekursives Einbuchen der Artikel. Ge�ndert 07.12.06
         bucheArtikel($buchung_nr, $art['F_Unterartikel_id'], $ndatum, $meineMenge, $ndauer);
         /**
         if ( ! sql_query("INSERT INTO ".TABLE_BUCHUNGSEINTRAEGE.
              " (F_Buchung_Nr, F_Artikel_Nr, Menge,Dauer,Datum) VALUES ".
              "($buchung_nr,".$art["F_Unterartikel_id"].",".$meineMenge.",$ndauer,$ndatum)"))
         {
           if ( ! sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE." SET Menge=Menge+".
           $meineMenge.", Dauer=$ndauer WHERE F_Artikel_Nr=".$art["F_Unterartikel_id"].
           " AND F_Buchung_Nr=$buchung_nr AND Datum=$ndatum"))
            echo sql_error();
         }
         */
       }
    }
    sql_free_result($query);
    // Eintr�ge mit Menge 0 l�schen
    sql_query("DELETE FROM ".TABLE_BUCHUNGSEINTRAEGE.
      " WHERE F_Buchung_Nr=$buchung_nr AND Menge=0");
  } // Datum nicht numerisch
}

// Bucht einen Artikel zur Buchung und erg�nzt alle zugeh�rigen Artikel der Gruppe
// Wenn die Menge des zuzubuchenden Artikels 0 betr�gt wird er komplett gel�scht
function entbucheArtikel($buchung_nr, $artikel_id, $datum, $menge=1, $ebene=0)
{
  // Absicherung: Keine Endlosschleifen bei rekursiver Einbuchung
  if ( $ebene > 10 ) return;
  if ( ! is_numeric($datum) )
  {
    $Datum = explode("-",$datum);
    if ( is_array($Datum) )
      $query = sql_query("SELECT Datum FROM ".TABLE_BUCHUNGSEINTRAEGE.
        " WHERE Datum BETWEEN ".$Datum[0]." AND ".strtotime("+23 hours 59 minutes",$Datum[1])." AND F_Artikel_Nr=".$artikel_id.
          " AND F_Buchung_Nr=".$buchung_nr);
      while ( $b = sql_fetch_row($query) )
      {
        entbucheArtikel($buchung_nr, $artikel_id, $b[0], $menge);
      }
      sql_free_result($query);
  }
  else
  {
    if ( !is_numeric($buchung_nr) || ! is_numeric($artikel_id) ||! is_numeric($datum) ||
         !is_numeric($menge) )
      die("Fehler: Keine Zahl in entbucheArtikel: $buchung_nr/$artikel_id/$datum/$menge!");
    if ( $menge == 0)
    {
      $query = sql_query("SELECT Datum, Menge FROM ".TABLE_BUCHUNGSEINTRAEGE.
        " WHERE F_Artikel_Nr=$artikel_id AND F_Buchung_Nr=$buchung_nr AND Datum=$datum");
      if ($menge = sql_fetch_row($query) )
      {
        $menge = $menge[1];
      }
      else
        $menge = 0;
      sql_free_result($query);
    }
    sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE.
            " SET Menge=Menge-$menge ".
            "WHERE F_Buchung_Nr=$buchung_nr AND F_Artikel_Nr=$artikel_id AND Datum=$datum");
    // Artikelgruppen entbuchen
    $query = sql_query("SELECT F_Unterartikel_id, Menge, Dauer, Beginn FROM ".TABLE_ARTIKELGRUPPEN.
      " WHERE F_Artikel_id=$artikel_id");
    while ( $art = sql_fetch_array($query) )
    {
       if ( $art['Menge'] == 0 )
       {
         // Pr�fen, ob schon vorhanden
         if ( ! $q = sql_query("SELECT Count(F_Artikel_Nr) FROM ".TABLE_BUCHUNGSEINTRAEGE.
           " WHERE F_Buchung_Nr=".$buchung_nr." AND F_Artikel_Nr=".$artikel_id.
           " AND Menge>0") )
             echo sql_error();
         $meineMenge = 0;
         if ( $row = sql_fetch_row($q) )
         {
           if ( $row[0] < 1 )
           {
             // Nachschauen, an welchem Datum entfernt werden muss
             $lquery = sql_query("SELECT Datum FROM ".TABLE_BUCHUNGSEINTRAEGE.
               " WHERE F_Artikel_Nr=".$art["F_Unterartikel_id"]." AND F_Buchung_Nr=$buchung_nr");
             if ($artikel = sql_fetch_row($lquery) )
             {
               $loeschDatum  = $artikel[0];
             }
             else
               $loeschDatum = $datum;
             sql_free_result($lquery);
             $meineMenge = 1;
           }
         }
         else
           $loeschDatum = 0;
         sql_free_result($q);
       }
       else
       {
         $meineMenge = $menge * $art['Menge'];
         $loeschDatum = $datum;
       }
       if ( $art['Beginn'] != 0 )
         $loeschDatum = $loeschDatum + $art['Beginn'];
       if ( $meineMenge != 0 )
       {
         // Rekursiv ausbuchen 
         entbucheArtikel($buchung_nr, $art['F_Unterartikel_id'], $loeschDatum, $meineMenge, $ebene+1);
         /**
         $sql = 'UPDATE '.TABLE_BUCHUNGSEINTRAEGE.
              " SET Menge=Menge-$meineMenge WHERE F_Buchung_Nr=$buchung_nr AND F_Artikel_Nr=".
              $art['F_Unterartikel_id']." AND Datum=$loeschDatum";
         if ( ! sql_query($sql) )
            echo "Fehler $sql: ".sql_error();
         */
       }
    } // f�r alle Unterartikel
    sql_free_result($query);
    // Eintr�ge mit Menge 0 l�schen
    sql_query('DELETE FROM '.TABLE_BUCHUNGSEINTRAEGE.
      " WHERE F_Buchung_Nr=$buchung_nr AND Menge=0");
  } // Datum nicht numerisch
}

function ZeigeBuchungskopf($buchung)
{
  if ( ! is_array($buchung) ) return;
  
  // Zeigt die wichtigsten Daten einer Buchung in einem Paragraphen an
  // Links aufdie Buchung und die Adresse
	require_once(INC_PATH.'cvjm/Buchung.class.php');
	require_once (INC_PATH.'Smarty/Smarty.class.php');
  $Smarty = new Smarty();
	$Smarty->template_dir = TEMPLATEPATH; // '/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';	
  $Buchung = new Buchung($buchung['Buchung_Nr']);
  $Smarty->assign_by_ref('Buchung',$Buchung);
  $Smarty->assign('Adressenurl', get_url(get_cmodule_id('cvjmadressen')));
  $Smarty->assign('Buchungurl', get_url(get_cmodule_id('cvjmbuchung')));
  setlocale(LC_ALL, "de_DE@euro", "de_DE", "de", "ge");
  $Smarty->display('Buchung_Kopf.tpl');
}

function pruefeArtikelVorhanden($buchung_nr, $artikel_id, $datum, $dauer=0)
{
  $sql = 'SELECT Menge, Bezeichnung, F_Buchung_Nr, F_Art_id FROM '.
                 TABLE_BUCHUNGSEINTRAEGE.
                 ' INNER JOIN '.TABLE_ARTIKEL.' ON F_Artikel_Nr=id '.
                 'WHERE (F_Art_id='.CVJMART_ORT.' OR F_Art_id='.CVJMART_PROGRAMM.
                 ' OR F_Art_id='.CVJMART_PROGRAMMPAKET.') '.
                 ' AND F_Buchung_Nr<>'.$buchung_nr.' AND F_Artikel_Nr='.$artikel_id.
                 ' AND (Datum BETWEEN '.$datum.' AND '.($datum+$dauer).
                 ' OR (Datum+Dauer) BETWEEN '.$datum.' AND '.($datum+$dauer).')';
  if ( ! $query = sql_query($sql)) echo sql_error();
  while ( $artikel = sql_fetch_array($query) )
  {
    echo '<div class="Fehler">Artikel '.
      stripslashes($artikel['Bezeichnung']).
      ' doppelt gebucht am '.date('d.m.Y',$datum);
    if ( date('H:i',$datum) != '00:00' )
    {
      echo ' '.date('H:i',$datum);
    }
    echo ' von Buchung <a href="'.
      get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$artikel['F_Buchung_Nr']).
      '">'.$artikel['F_Buchung_Nr'].
      '</a> ('.$artikel['Menge'].' mal)';
    echo '</div>';
  }
  sql_free_result($query);
}

// Zeigt die oberste Kategorie einer Art / Buchung
function ZeigeBereiche($Art, $Buchung_Nr)
{
  $Bereiche = array();
  $sql = 'SELECT Position, Einruecken FROM '.TABLE_BUCHUNGSEINTRAEGE.
    ' INNER JOIN '.TABLE_ARTIKEL.
    ' ON F_Artikel_Nr = id WHERE F_Buchung_Nr = '.$Buchung_Nr.
    ' AND F_Art_id ='.$Art;
  if ( ! $query = sql_query($sql)) echo "Fehler Zeigebereiche: ".sql_error();
  while ( $artikel = sql_fetch_array($query) )
  {
    $id = holeArtikelMasterParent($Art, $artikel["Position"], $artikel["Einruecken"]);
    $Bereiche[] = $id;
  }
  sql_free_result($query);
  $Bereich = array();
  foreach ( $Bereiche as $key => $value)
  {
    $Bereich[$value] = getOrtsname($value);
  }
  return implode(",",$Bereich);
}

function Personenanzahl($Buchung)
{
  $anz = 0;
  for ( $i=1; $i < 7; $i++)
  {
    if ( isset($Buchung['Personen'.$i]))
      $anz += $Buchung['Personen'.$i];
    if ( isset($Buchung['Personen'.$i]))
      $anz += $Buchung['Personen'.$i.'w'];
  }
  return $anz;
}

function berechneUebernachtungen($buchung, $von = "Von", $bis="Bis")
{
  return (($buchung[$bis]-$buchung[$von]) / 86400);
}

?>
