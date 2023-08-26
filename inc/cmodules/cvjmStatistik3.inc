<?php

/*
   AWF-Name: cvjmstatistik
   cvjmStatistik.inc
*/

define("CVJM_ZELTBEREICH", "ZB");

if ( $profile["editor"] != 1 )
{
  include(INC_PATH."misc/CVJM.inc");
  include(INC_PATH."misc/cvjmArtikeltools.inc");
  include(INC_PATH."misc/cvjmBuchungstools.inc");
  include(INC_PATH.'cvjm/Artikel.class.php');
  $Artikelarten = Artikel::getArtikelArten();
  
  if ( $document['title'] != '')
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';

  echo '<form action="'.get_url($id).'" method="post">';
  echo 'Daten aus Monat/Jahr ';
  echo '<input type="Text" name="docinput[Monat]" value="'.date("m").'" size="2" maxlength="2"/>/';
  echo '<input type="Text" name="docinput[Jahr]" value="'.date("Y").'" size="4" maxlength="4"/>';
  echo '<input type="Submit" value="Anzeigen"/>';
  echo '</form>';
  if ( is_numeric($docinput["Monat"]) && is_numeric($docinput["Jahr"]) )
  {
    $anfang = mktime(0,0,0,$docinput["Monat"],1,$docinput["Jahr"]);
    $ende = strtotime("+1month",$anfang);
    $query = sql_query("SELECT ".TABLE_BUCHUNGEN.".*, F_Artikel_Nr, Datum, F_Art_id, Position,Einruecken FROM ((".
        TABLE_RECHNUNGSEINTRAEGE." INNER JOIN ".
        TABLE_RECHNUNGEN." ON F_Rechnung_id=Rechnung_id) INNER JOIN ".TABLE_ARTIKEL.
        " ON id=F_Artikel_Nr) INNER JOIN ".TABLE_BUCHUNGEN.
        " ON Buchung_Nr=F_Buchung_Nr WHERE Rechnung=1 AND BStatus<=".BUCHUNG_FERTIG.
        " AND BStatus <> ".BUCHUNG_STORNIERT." AND Datum > Von ".  // Vorhut wird nicht berücksichtigt
        " AND F_Art_id=".CVJMART_ORT.
        " AND Datum BETWEEN $anfang AND $ende".
        " AND Menge > 0 AND NOT Storno ORDER BY Datum");
    $uebernachtungen = array();
    $plaetze = array();
    while ( $eintrag = sql_fetch_array($query) )
    {
      $a = new Artikel($eintrag['F_Artikel_Nr']);
    	$schlafplaetze = $a->BerechneSchlafplaetze();
      if ( $schlafplaetze >= 0 )
      {
        if ( $schlafplaetze > 0 )
        {
          // H�tten-Übernachtung
          $bereich = getOrtsname(holeArtikelMasterParent($eintrag["F_Art_id"],
            $eintrag["Position"],$eintrag["Einruecken"]));
          $eintrag["Datum"] = strtotime(date("Y-m-d", $eintrag["Datum"]));
          if ( ! isset($plaetze[$eintrag["Buchung_Nr"]][$eintrag["Datum"]][$bereich]) )
          {
            $plaetze[$eintrag["Buchung_Nr"]][$eintrag["Datum"]][$bereich] = 0;
          }
          $plaetze[$eintrag["Buchung_Nr"]][$eintrag["Datum"]][$bereich] += $schlafplaetze;
          $leute[$eintrag["Buchung_Nr"]] = Personenanzahl($eintrag);
        }
      }
    }
    sql_free_result($query);
    // Nun die Auslastung der Bereiche berechnen
    foreach ( $leute as $buchung_nr => $anzahl )
    {
      // Datum durchgehen
      foreach ( $plaetze[$buchung_nr] as $datum => $bereiche )
      {
        $anz = $anzahl;
        foreach ( $bereiche as $bereich => $schlafplaetze )
        {
          // Initialisierung
          if ( ! isset($uebernachtungen[$bereich]) )
          {
            //echo date("d.m.Y",$datum)."/";
            $uebernachtungen[$bereich] = array();
            $uebernachtungen[$bereich]["Anzahl"] = 0;
          }
          if ( $schlafplaetze > 0 )
          {
            if ( $anz <= $schlafplaetze )
            {
              $personen = $anz;
              $anz = 0;
            }
            elseif ( $anz > $schlafplaetze )
            {
              $personen = $schlafplaetze;
              $anz -= $schlafplaetze;
            }
            $uebernachtungen[$bereich]["Anzahl"] += $personen;
          }
          if ( ! in_array($buchung_nr, $uebernachtungen[$bereich]["Buchungen"]) )
            $uebernachtungen[$bereich]["Buchungen"][] = $buchung_nr;
        }
      }
    }
    echo "<h2>In ".$docinput["Monat"]."/".$docinput["Jahr"]." vorhandene Übernachtungen:</h2>";
    foreach ( $uebernachtungen as $bereich => $jahre )
    {
      echo 'Übernachtungen in '.$bereich.':'.$jahre["Anzahl"].'<br />';
    }
    echo '<br />';
    echo "In ".$docinput["Monat"]."/".$docinput["Jahr"]." neu hinzugekommene Personen:<br />";
    $query = sql_query("SELECT DISTINCT ".TABLE_BUCHUNGEN.".* FROM ((".
      TABLE_RECHNUNGSEINTRAEGE." INNER JOIN ".
      TABLE_RECHNUNGEN." ON F_Rechnung_id=Rechnung_id) INNER JOIN ".TABLE_ARTIKEL.
      " ON id=F_Artikel_Nr) INNER JOIN ".TABLE_BUCHUNGEN.
     " ON Buchung_Nr=F_Buchung_Nr WHERE Rechnung=1 AND BStatus<=".BUCHUNG_FERTIG.
      " AND BStatus <> ".BUCHUNG_STORNIERT." AND Datum > Von ".  // Vorhut wird nicht berücksichtigt
      " AND Von BETWEEN $anfang AND $ende".
      " AND Menge > 0 AND NOT Storno ORDER BY Datum");
    $anz = 0;
    while ( $row = sql_fetch_array($query) )
    {
       if ( isset($plaetze[$row["Buchung_Nr"]]))
       {
         $erstesDatum = reset($plaetze[$row["Buchung_Nr"]]);
         foreach ( $erstesDatum as $bereich => $wert )
           $anz += $wert;
       }
       // Rausrechnen: Übernachtungen
    }
    echo $anz." Personen";
  }

}
?>