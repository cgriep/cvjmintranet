<?php
/**
*cvjmKuechenzettel
*
*AWF-Name: cvjmkuechenzettel
*Parameter:
*docinput[Buchung_Nr] - Buchungsnummer zum Anzeigen
*docinput[Art] - Artikelnummer zum Anzeigen
*
*docinput[Datum] - Datum der Aenderung
*docinput[UmwMA]
*docinput[UmwAM] - Umwandlung Mittag/Abend in Lunch/Warm Abend u.U.
*
*SaveAnzahl - Kommando zum Sichern
*docinput[Buchungsnr] - Buchungsnummer
*docinput[NeueAnzahl] - Neue Anzahl
*docinput[Tag] - Tag der Aenderung
*docinput[AlleTage] - fuer alle weiteren Tage ebenfalls aendern
*/
if ( $profile["editor"] != "1" )
{
  include(INC_PATH."misc/CVJM.inc");
  include(INC_PATH."misc/cvjmBuchungstools.inc");
  include(INC_PATH."misc/Kalenderjs.inc");
  require_once(INC_PATH.'cvjm/Buchung.class.php');
include(INC_PATH.'Smarty/Smarty.class.php');
echo '<script src="/javascript/overlib.js" type="text/javascript"></script>';
  echo '<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>';
  echo '<h2>'.transform($document['title'], $document['datatype']['title'])."</h2>\n";
  if ( isset($docinput["Tag"]) && is_numeric($docinput["Tag"]) )
  {
     if ( isset($docinput["UmwMA"]) && is_numeric($docinput["UmwMA"]) )
     {
       // Umwandlung: Mittag (345) -> Lunch (344)
       sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE." SET F_Artikel_Nr=344 WHERE Datum=".
         $docinput["Tag"]." AND F_Artikel_Nr=345 AND F_Buchung_Nr=".$docinput["UmwMA"]);
       // Umwandlung: Abendessen (347) -> Warmes Abendessen (684)
       sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE." SET F_Artikel_Nr=684 WHERE Datum=".
         $docinput["Tag"]." AND F_Artikel_Nr=347 AND F_Buchung_Nr=".$docinput["UmwMA"]);
       sql_query("UPDATE ".TABLE_BUCHUNGEN." SET ");
       sql_query("UPDATE ".TABLE_BUCHUNGEN." SET Logtext=CONCAT('".date("d.m.Y H:i").
         " Mittag in Lunch geäert ".get_user_nickname()."\n',Logtext) ".
         " WHERE Buchung_Nr=".$docinput["UmwMA"]);
     }
     if ( isset($docinput["UmwAM"]) && is_numeric($docinput["UmwAM"]) )
     {
       // Umwandlung: Lunch (344) -> Mittag (345)
       sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE." SET F_Artikel_Nr=345 WHERE Datum=".
         $docinput["Tag"]." AND F_Artikel_Nr=344 AND F_Buchung_Nr=".$docinput["UmwAM"]);
       // Umwandlung: Warmes Abendessen (684) -> Abendessen (347)
       sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE." SET F_Artikel_Nr=347 WHERE Datum=".
         $docinput["Tag"]." AND F_Artikel_Nr=684 AND F_Buchung_Nr=".$docinput["UmwAM"]);
       sql_query("UPDATE ".TABLE_BUCHUNGEN." SET Logtext=CONCAT('".date("d.m.Y H:i").
         " Lunch in Mittag ".get_user_nickname()."\n',Logtext) ".
         " WHERE Buchung_Nr=".$docinput["UmwAM"]);
     }
  }
  if ( isset($_REQUEST['SaveAnzahl']) && is_numeric($docinput['Buchungsnr']) )
  {
    if (!is_numeric($docinput['Vegetarier'])) $docinput['Vegetarier'] = 0;
    if (!is_numeric($docinput['Schweinelos'])) $docinput['Schweinelos'] = 0;
    if (!is_numeric($docinput['NeueAnzahl'])) $docinput['NeueAnzahl'] = 0;
  	$essen = getOrtsname($docinput['Art']);
    // Neue Anzahl als Bemerkung speichern
    $bemerkung = $essen.' am '.date('d.m.Y',$docinput['Tag']);
    $bemerkung .= ' neue Anzahl: '.$docinput['NeueAnzahl']." (";
    $bemerkung .= 'Vegetarier: '.$docinput['Vegetarier'].", ";
    $bemerkung .= 'Schweinefleischfrei: '.$docinput['Schweinelos'].")";
    sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Vegetarier='.$docinput['Vegetarier'].
      ', Schweinelos='.$docinput['Schweinelos'].' WHERE Buchung_Nr='.$docinput['Buchungsnr']);
    if ( isset($docinput['AlleTage']) )
    {
      $bemerkung .= ' auch füe weiteren Essen';
      // füe weiteren Tage der Buchung #-# Einträ erstellen
      $sql = 'SELECT id, Datum FROM '.
        TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.
        TABLE_ARTIKEL.' ON id=F_Artikel_Nr WHERE F_Art_id='.CVJMART_VERPFLEGUNG.
        ' AND Datum >= '.$docinput['Tag'].' AND F_Buchung_Nr='.$docinput['Buchungsnr'].
        ' ORDER BY Datum, Position';
      $query = sql_query($sql);
      $anz = 0;
      while ( $eintrag = sql_fetch_array($query) )
      {
        $sql = 'UPDATE '.TABLE_BUCHUNGSEINTRAEGE.' SET Menge='.$docinput['NeueAnzahl'].
          ' WHERE F_Buchung_Nr='.$docinput['Buchungsnr'].' AND Datum='.$eintrag['Datum'].
          ' AND F_Artikel_Nr='.$eintrag['id'];
        if ( ! sql_query($sql)) echo 'Fehler '.$sql.': '.sql_error();
        $anz ++;
      }
      sql_free_result($query);
      $bemerkung .= " ($anz Einträ veräert)";
    }
    else
    {
      $bemerkung .= ' nur füses Essen';
      $sql = 'UPDATE '.TABLE_BUCHUNGSEINTRAEGE.' SET Menge='.$docinput['NeueAnzahl'].
        ' WHERE F_Buchung_Nr='.$docinput['Buchungsnr'].' AND Datum='.$docinput['Tag'].
        ' AND F_Artikel_Nr='.$docinput['Art'];
      if ( ! sql_query($sql)) echo "Fehler $sql: ".sql_error();
    }
    $bemerkung .= "\n".addslashes($docinput['Hinweise']);
    sql_query('INSERT INTO '.TABLE_BUCHUNGSBEMERKUNGEN.' (F_Buchung_Nr,Bemerkung, Logtext) VALUES ('.
        $docinput['Buchungsnr'].",'".addslashes($bemerkung)."','".
        date('d.m.Y H:i').' automatisiert erstellt '.get_user_nickname()."')");
    echo "<strong>&gt;&gt;&gt; Äderung gespeichert.</strong><br />\n";
  }
  if ( isset($docinput['Buchung_Nr']) && is_numeric($docinput['Buchung_Nr']) )
  {
    $query = sql_query('SELECT * FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_ADRESSEN.
        ' ON Adressen_id= F_Adressen_id WHERE Buchung_Nr = '.$docinput['Buchung_Nr']);
    if ( $Buchung = sql_fetch_array($query) )
    {
      echo '<h1>Buchung '.$Buchung['Buchung_Nr'].' am '.date('d.m.Y',$docinput['Datum']).'</h1>';
      ZeigeBuchungskopf($Buchung);
      echo "\n";
      echo '<br />';
      if ( $Buchung['Essenbesonderheit'] != '' )
      {
      	echo '<strong>'.$Buchung['Essenbesonderheit'].'</strong><br />';
      }
      $essen = getOrtsname($docinput['Art']);
      // Anzahl Eingeben und als Bemerkung speichern
      echo '<form action="'.get_url($id).'" method="post" class="noprint">';
      echo 'Neue Anzahl <strong>'.$essen.'</strong> fü '.$Wochentage[date("w",$docinput["Datum"])]." ".
        date("d.m.Y",$docinput["Datum"]);
      // bisher-Wert anzeigen
	  $sql = 'SELECT Menge FROM '.TABLE_BUCHUNGSEINTRAEGE.' WHERE F_Artikel_Nr='.$docinput['Art'].' AND F_Buchung_Nr='.
	  $docinput['Buchung_Nr'].' AND Datum='.$docinput['Datum'];
      if ( ! $mengequery = sql_query($sql) ) echo "Fehler $sql: ".sql_error();
      $row = sql_fetch_array($mengequery);
      echo ' (bisher <b>'.$row['Menge'].'</b>)';
      sql_free_result($mengequery);
      echo '<input type="hidden" name="docinput[Buchungsnr]" value="'.$docinput["Buchung_Nr"].
        '" />';
      echo '<input type="hidden" name="docinput[Art]" value="'.$docinput["Art"].'" />';
      echo '<input type="hidden" name="docinput[Tag]" value="'.$docinput["Datum"].'" />';
      echo '  <input type="Text" name="docinput[NeueAnzahl]" value="';
      echo Personenanzahl($Buchung);
      echo '" size="3" maxlength="3" />';
      echo "<br />\n";
      echo 'Neue Anzahl <strong>Vegetarier</strong> ';
      echo '<input type="text" name="docinput[Vegetarier]" value="'.$Buchung['Vegetarier'].
        '" size="3"/><br />';
      echo 'Neue Anzahl <strong>ohne Schweinefleisch</strong> ';
      echo '<input type="text" name="docinput[Schweinelos]" value="'.$Buchung['Schweinelos'].
        '" size="3" /><br />';      
      echo 'Neue Anzahl gilt auch füe weiteren Tage ';
      echo '<input type="Checkbox" name="docinput[AlleTage]" value="v" />';
      echo "<br />\n";
      if ( $docinput["Art"] == 345 || $docinput["Art"] == 347 ) // Mittagessen / Abendessen
      {
        echo '<strong>Umwandlung</strong>: Mittag + Abendessen &rarr; <a href="';
        echo get_url($id,"docinput[UmwMA]=".$docinput["Buchung_Nr"].
          "&docinput[Tag]=".$docinput["Datum"]);
        echo '">Lunch + Warmes Abendessen</a>';
        echo '<br />';
      }
      if ( $docinput["Art"] == 344 || $docinput["Art"] == 684 ) // Lunch bzw. warm. Abendessen
      {
         echo '<strong>Umwandlung</strong>: Lunch + warmes Abendessen &rarr; <a href="';
         echo get_url($id,"docinput[UmwAM]=".$docinput["Buchung_Nr"].
                     "&docinput[Tag]=".$docinput["Datum"]);
         echo '">Mittagessen + Abendessen</a>';
         echo '<br />';
      }
      echo 'Zusäliche Hinweise<br />';
      echo '<textarea name="docinput[Hinweise]" cols="60" rows="5"></textarea><br />';
      echo '<input type="Submit" name="SaveAnzahl" value="Speichern" />';
      echo "</form>\n";
      // Buchungsbemerkungen zeigen
	  $Buchung = new Buchung($docinput['Buchung_Nr']);
	  if (Count($Buchung->getBemerkungen()) > 0)
	  {
	  	echo '<div class="ueberschriftzeile">';
	  	echo Count($Buchung->getBemerkungen()).' Bemerkungen';
		echo '</div>';
		$zeile = 'helleZeile';
		foreach ( $Buchung->getBemerkungen() as $bemerkung)
		{
			echo '<div class="'.$zeile.' ganzeZeile">';
			echo nl2br($bemerkung['Bemerkung']);
			if ($bemerkung['Hinweis'] != '') echo '<br />'.nl2br($bemerkung['Hinweis']);
			echo '</div>';
			if ( $zeile == 'helleZeile')
				$zeile = 'dunkleZeile';
			else
				$zeile = 'helleZeile';
		}
	  }      
    }
    sql_free_result($query);
    echo '<br />[ <a href="'.get_url($id).'">Zurür Üersicht</a> ]';
  }
  else
  {
    // Gesamtübersicht der Küchenbelegung
    KalenderScriptEinfuegen();
    $Datum = mktime(0,0,0,date("m",time()),date("d",time()),date("Y",time()));
    if ( isset($docinput["Datum"]) )
    {
      $Datum = convertDatumToTimestamp($docinput["Datum"]);
    }
    if ( isset($docinput["Tage"]) && is_numeric($docinput["Tage"]))
    {
      $Tage = $docinput["Tage"];
    }
    else 
      $Tage = 10;
    $Where = "(Datum BETWEEN $Datum AND ".strtotime("+".$Tage." day",$Datum).")";
    $sql = "SELECT Buchung_Nr, F_Artikel_Nr, Datum, Menge, Bezeichnung, ".
      "Altersgruppe, Kuechenhilfe, Position, Vegetarier, Schweinelos, ".
      "BetreueranzahlM, BetreueranzahlW, Essenbesonderheit, F_Speiseraum_id FROM (".
      TABLE_BUCHUNGSEINTRAEGE." INNER JOIN ".
      TABLE_ARTIKEL." ON id=F_Artikel_Nr) INNER JOIN ".TABLE_BUCHUNGEN.
      " ON F_Buchung_Nr=Buchung_Nr WHERE F_Art_id=".CVJMART_VERPFLEGUNG.
        " AND $Where ORDER BY Datum, F_Speiseraum_id, F_Adressen_id";
    if ( ! $Eintraege = sql_query($sql) ) echo "Fehler $sql: ".sql_error();
    $Artikel = array();
    $Essen = array();
    $Buchungen = array();
    while ( $Eintrag = sql_fetch_array($Eintraege) )
    {
      $Artikel[$Eintrag["F_Artikel_Nr"]] = $Eintrag["Bezeichnung"];
      $ArtikelPos[$Eintrag["F_Artikel_Nr"]] = $Eintrag["Position"];
      $ArtikelNr[$Eintrag["F_Artikel_Nr"]] = $Eintrag["F_Artikel_Nr"];
      if ( ! isset($Essen[$Eintrag["Datum"]][$Eintrag["F_Artikel_Nr"]]))
      {
      	$Essen[$Eintrag["Datum"]][$Eintrag["F_Artikel_Nr"]] = array();
      	$Buchungen[$Eintrag["Datum"]][$Eintrag["F_Artikel_Nr"]] = array();
      }
      $Buchungen[$Eintrag["Datum"]][$Eintrag["F_Artikel_Nr"]][] = new Buchung($Eintrag["Buchung_Nr"]);
      $Essen[$Eintrag["Datum"]][$Eintrag["F_Artikel_Nr"]][] = $Eintrag['Menge'];
    
    }
    sql_free_result($Eintraege);
    array_multisort($ArtikelPos, SORT_ASC, SORT_NUMERIC, $Artikel, $ArtikelNr);
    $art = array();
    foreach ( $Artikel as $k=>$a )
    {
    	$art[$ArtikelNr[$k]] = $a;
    }
    $Artikel = $art;
    $Smarty = new Smarty();
    $Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
    $Smarty->assign('id', $id);
    $Smarty->assign('Artikel', $Artikel);
    $Smarty->assign('Datum', $Datum);
    $Smarty->assign('Buchungen', $Buchungen);
    $Smarty->assign('Essen', $Essen);
    setlocale(LC_ALL, "de_DE@euro", "de_DE", "de", "ge");
    $Smarty->display('Kuechenuebersicht.tpl');
  }
}

?>
