<?php

/**
*  Modul Buchungsbemerkungen
*  Bemerkungen zu einer Buchung
*  z.B. Zerbruch, Ausleihen etc.
*
*  AWF-Name: cvjmbuchungsbemerkungen
*  Parameter: docinput[Buchung_Nr] mit der Buchungsnummer für die eingegeben werden soll
*/
function VerantwortlicheAnzeigen($buchung, $Name)
{
  global $id;
  global $design;
  echo '<br />';
  if ( substr($buchung[$Name],0,2) == 'OK' )
    echo '<span class="Erledigt">'.substr($buchung[$Name],2).'</span>';
  else
  {
    if ( is_numeric($buchung[$Name]) )
      echo get_user_nickname($buchung[$Name]);
    else
      echo $buchung[$Name];
    if ( $buchung[$Name] != '' ) echo '<br />';
    echo ' <a href="'.get_url($id,"docinput[Erledigt][$Name]=".$buchung["Buchung_Nr"]).
      '" ';
    echo 'onClick="javascript:return window.confirm(\'Eintrag erfolgreich erledigt?\');">';
    echo 'Klick wenn ok</a>';
  }
}

include(INC_PATH.'misc/CVJM.inc');
include(INC_PATH.'misc/cvjmBuchungstools.inc');
include(INC_PATH.'misc/cvjmArtikeltools.inc');
include(INC_PATH.'misc/cvjmAuftragstools.inc');
require_once(INC_PATH.'cvjm/Buchung.class.php');
include(INC_PATH.'misc/Kalenderjs.inc');
KalenderScriptEinfuegen();
if ( $profile['editor'] != 1 )
{
	// Kalender 
  echo '
  <style type="text/css">
@import url(css/popcalendar.css);
</style>';
  echo '<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>';
  echo '<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>';
	
  if ( isset($docinput['NewBem']) )
  {
    if ( trim($docinput['NewBem']) != '' )
    {
      // Neue Bemerkung anfügen
      if ( sql_query('INSERT INTO '.TABLE_BUCHUNGSBEMERKUNGEN.
        ' (F_Buchung_Nr,Bemerkung, Logtext) VALUES ('.
        $docinput['Buchung_Nr'].",'".
        trim(sql_real_escape_string($docinput["NewBem"]))."','".date("d.m.Y H:i").
        " erstellt ".get_user_nickname()."')"))
        echo '<strong>&gt;&gt;&gt; Bemerkung erzeugt.</strong><br />';
      else
        echo '<strong>&gt;&gt;&gt; Fehler beim Erzeugen der Bemerkung.</strong><br />';
      // Mail an Verwaltung schreiben (Gruppe 20)
      $qresult = sql_query("SELECT user_id FROM ".TABLE_USERDATA." WHERE name='".
        CVJM_BUCHUNGSGRUPPE."' AND value != '0'");
      if(sql_num_rows($qresult) > 0) {
        while($row = sql_fetch_row($qresult)) {
          send_message ($row[0], $docinput["NewBem"], SESSION_DBID,
            get_user_nickname(),
            '[Buchung '.$docinput["Buchung_Nr"].'] Neue Bemerkung', 'email');
        }
        sql_free_result($qresult);
      }      
      // Wenn Reparaturauftrag, dann einen Auftrag einrichten
	  $Betroffen = '';
	  $title = 'Auftrag';
      if ( isset($docinput["Reparatur"]) )
      {      	
        $Betroffen = ",g".substr(CVJM_AUSSENTEAM, strpos(CVJM_AUSSENTEAM,"_")+1);
        $title = 'Reparaturauftrag';
      }
      /* Gruppen fehlen noch 
      if ( isset($docinput["Paedagogik"]) )
      {      	
        $Betroffen = "g".substr(CVJM_AUSSENTEAM, strpos(CVJM_AUSSENTEAM,"_")+1);
      }
      if ( isset($docinput["Kueche"]) )
      {      	
        $Betroffen = "g".substr(CVJM_AUSSENTEAM, strpos(CVJM_AUSSENTEAM,"_")+1);
      }
	  */
      if ( isset($docinput["Verwaltung"]) )
      {      	
        $Betroffen .= ",g".substr(CVJM_VERWALTUNGSGRUPPE, strpos(CVJM_VERWALTUNGSGRUPPE,"_")+1);
      }
      if ( $Betroffen != '' )
      {      
        // Auftrag einrichten
        insertAuftrag($title.' zu Bu '.$docinput["Buchung_Nr"],
          $docinput["NewBem"], 2, $Betroffen.',', -1, -1, 1); // ,8 user_id 8 = Poddel
      }
    }
    $query = sql_query("SELECT * FROM ".TABLE_BUCHUNGSBEMERKUNGEN." WHERE F_Buchung_Nr = ".$docinput["Buchung_Nr"]);
    while ($buchung = sql_fetch_array($query) )
    {
      if ( isset($docinput["H".$buchung["Bemerkung_id"]]) )
      {
        if ( stripslashes($buchung["Hinweis"]) != $docinput["H".$buchung["Bemerkung_id"]] )
        {
          // Text wurde verändert
          sql_query("UPDATE ".TABLE_BUCHUNGSBEMERKUNGEN." SET Hinweis = '".trim(addslashes(
            $docinput["H".$buchung["Bemerkung_id"]]))."', Logtext=CONCAT('".date("d.m.Y H:i")." geändert ".
            get_user_nickname()."\n',Logtext) WHERE Bemerkung_id = ".$buchung["Bemerkung_id"]);
        }
      }
    }
    sql_free_result($query);
  }
  if ( $document['title'] != '')
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  if ( isset($docinput["Erledigt"]) )
  {
    include(INC_PATH."misc/cvjmPersonalplanungtools.inc");
    $Arten = array("Anreisename", "Abreisename", "Rechnungname", "Abnahmename" );
    $Was = "";
    foreach ( $Arten as $key => $value) 
        if ( is_numeric($docinput["Erledigt"][$value]) )
          $Was = $value;
    if ( isset($docinput["Buchung_Nr"]) && is_numeric($docinput["Buchung_Nr"]))
    {
      // Erledigte Arbeiten
      //if ( $Was != "" )
          sql_query("UPDATE ".TABLE_BUCHUNGEN." SET Logtext=CONCAT('".
            date("d.m.Y H:i")." $Was gesetzt ".get_user_nickname()."\n',Logtext)".
            " WHERE Buchung_Nr = ".$docinput["Erledigt"][$Was]);
      //$docinput[$Was] = "OK ".get_user_nickname();

      speicherePersonalplanung($docinput);
      speichereSchluessel($docinput);
      // erledigt - Buchung muss nicht mehr angezeigt werden
      unset($docinput["Buchung_Nr"]);
      unset($docinput["Erledigt"]);
    }
    elseif ( $Was != "" )
    {
      // Schlüsselliste / Personalplanung zeigen
      $query = sql_query("SELECT * FROM ".TABLE_BUCHUNGEN." INNER JOIN ".TABLE_ADRESSEN.
      " ON Adressen_id = F_Adressen_id WHERE Buchung_Nr = ".$docinput["Erledigt"][$Was]);
      $Buchung = sql_fetch_array($query);
      sql_free_result($query);
      $Buchung[$Was] = "OK ".get_user_nickname();
      ZeigeBuchungskopf($Buchung);
      echo $design["table_begin"];
      echo '<form action="'.get_url($id).'" method="post">';
      echo '<input type="hidden" name="docinput[Erledigt]['.$Was;
      echo ']" value="'.$docinput[Erledigt][$Was].'" />';
      // Schlüsselliste
      ZeigeSchluesselliste($Buchung);
      // Personalplanung
      ZeigePersonalplanung($Buchung);
      echo '<tr><td colspan="2" class="zentriert">';
      echo '<input type="Submit" value="Erledigt, Schlüsselliste und Abnahmedaten speichern" /></td></tr>';
      echo '</form>';
      echo $design["table_end"];
    }
  }
  if ( isset($docinput["Buchung_Nr"]) && is_numeric($docinput["Buchung_Nr"]))
  {    
  	// Buchung mit Bemerkungen versehen
    $sql = "SELECT * FROM ".TABLE_BUCHUNGSBEMERKUNGEN.
     " WHERE F_Buchung_Nr = ".$docinput['Buchung_Nr']." ORDER BY Bemerkung_id DESC";
    $query = sql_query($sql);
    $Bemerkungen = array();
    while ($bemerkung = sql_fetch_array($query))
    {
    	$Bemerkungen[] = $bemerkung;
    }
    sql_free_result($query);
    
    require_once(INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('id', $id);
    $Smarty->assignbyref('Bemerkungen', $Bemerkungen);    	
	
    $Buchung = new Buchung($docinput['Buchung_Nr']);
	$Smarty->assignbyref('Buchung',$Buchung);
	$Smarty->assign('Adressenurl',get_url(get_cmodule_id('cvjmadressen')));
	$Smarty->assign('Auftragurl',get_url(get_cmodule_id('auftrag')));
	$Smarty->assign('Buchungurl', get_url(get_cmodule_id('cvjmbuchung')));
	setlocale(LC_ALL, "de_DE@euro", "de_DE", "de", "ge");
	$Smarty->display('Buchung_Bemerkung_Edit.tpl'); 
  }
  elseif ( ! isset($docinput["Erledigt"]) )
  {
    // Übersicht über die aktuellen Belegungen
    echo $design["table_begin"];
    echo '<tr class="ueberschrift"><th></th><th>Ankunft</th><th>Häuser-<br />abnahme<br />Abfahrt</th><th>Abrechnung</th>'.
      '<th>Smileycard</th><th>Name</th><th>Belegung/<br />Personen</th></tr>';
      /* Alt:
    $query = sql_query("SELECT * FROM ".TABLE_BUCHUNGEN." INNER JOIN ".TABLE_ADRESSEN.
      " ON F_Adressen_id = Adressen_id WHERE Von <= ".strtotime("+".$document["TageLZ"]." day")." AND Bis >= ".
      strtotime("-".$document["TageLZ"]." day")." AND BStatus < ".BUCHUNG_FERTIG.
      " AND BStatus <> ".
      BUCHUNG_STORNIERT." ORDER BY Von");
      */      
    $sql = 'SELECT DISTINCT '.TABLE_BUCHUNGEN.'.*, '.TABLE_ADRESSEN.
      '.*, F_Buchung_Nr FROM '.TABLE_BUCHUNGEN.
      ' INNER JOIN '.TABLE_ADRESSEN.' ON F_Adressen_id = Adressen_id '.
      ' LEFT JOIN '.TABLE_SCHLUESSEL.' ON F_Buchung_Nr=Buchung_Nr WHERE '. 
		'((Von <= '.time().' AND (SUBSTRING(Anreisename,1,2) <> "OK" OR '.
		'SUBSTRING(Abnahmename,1,2) <> "OK" OR '.
		'SUBSTRING(Rechnungname,1,2) <> "OK" OR '.
    //	'SUBSTRING(Smilycardname,1,2) <> "OK" OR '.
		' F_Buchung_Nr IS NOT NULL))'.
		' OR Von BETWEEN '.time().' AND '.strtotime('+'.$document['TageLZ'].' day').')'.
        ' AND BStatus <> '.BUCHUNG_STORNIERT.' AND BStatus<>'.BUCHUNG_GELOESCHT.' ORDER BY Von';
    $query = sql_query($sql);
/*      
    if ( sql_num_rows($query) < 5 )
    {
      sql_free_result($query);
      $query = sql_query('SELECT DISTINCT '.TABLE_BUCHUNGEN.'.*, '.TABLE_ADRESSEN.
      '.*, F_Buchung_Nr FROM '.TABLE_BUCHUNGEN.
      ' INNER JOIN '.TABLE_ADRESSEN.' ON F_Adressen_id = Adressen_id '.
      ' LEFT JOIN '.TABLE_SCHLUESSEL.' ON F_Buchung_Nr=Buchung_Nr WHERE '. 
//Bis >= '.strtotime('-'.$document['TageLZ'].' day').' AND BStatus < '.BUCHUNG_FERTIG.
        // solange Schlüssel vorhanden oder eine der Abnahmen nicht da
		'(SUBSTRING(Anreisename,1,2) <> "OK" OR '.
		'(SUBSTRING(Abreisename,1,2) <> "OK" OR '.
		'(SUBSTRING(Rechnungname,1,2) <> "OK" OR '.
		' F_Buchung_Nr IS NOT NULL)'.
        ' AND BStatus <> '.BUCHUNG_STORNIERT.' ORDER BY Von '.sql_limit(5));
    }
*/
    $Color = 'helleZeile';
    while ( $buchung = sql_fetch_array($query) )
    {
      echo '<tr class="'.$Color.'">';
      echo '<td>';
      echo '<a href="'.get_url($id,"docinput[Buchung_Nr]=".$buchung["Buchung_Nr"]).'">';
      echo $buchung["Buchung_Nr"];
      echo '</a><br />';
      echo '<a href="'.get_url($id,"docinput[Buchung_Nr]=".$buchung["Buchung_Nr"]).
        '"><img src="img/small_edit/edit_small.gif" title="Bemerkung eingeben" />';
      echo "<br />\n";
      // Farbige Markierung von Schlüssel ausgegeben sind
      echo '<a href="'.get_url(get_cmodule_id("cvjmpersonalplanung"),
        "docinput[Buchung_Nr]=".$buchung["Buchung_Nr"]).
        '"';
      $query2 = sql_query("SELECT SUM(Anzahl) FROM ".TABLE_SCHLUESSEL." WHERE F_Buchung_Nr=".$buchung["Buchung_Nr"]);
      $anz = sql_fetch_row($query2);
      sql_free_result($query2);
      $schluessel = $anz[0];
      if ( $schluessel > 0 ) 
      {
      	echo ' style="background-color: #FF8080;"';
      }
      echo '><img src="img/icon_key.png" title="Schlüsselliste ansehen" />';
      echo '<br />';
      switch ( $buchung["BStatus"] )
      {
        case BUCHUNG_GELOESCHT: // gelöscht
          echo '<img src="img/CVJM/geloescht.gif" title="Vorreservierung gelöscht">';
          break;
        case 0:
          echo '<img src="img/CVJM/vorres.gif" title="Vorreservierung">';
          break;
        case 1:
          echo '<img src="img/CVJM/1nachfrage.gif" title="1. Nachfrage">';
          break;
        case BUCHUNG_VORRESERVIERUNG: // Vorreservierung
         echo '<img src="img/CVJM/2nachfrage.gif" title="2. Nachfrage">';
          break;
        case BUCHUNG_RESERVIERUNG:  // Reserviert
          echo '<img src="img/CVJM/reserviert.gif" title="Reserviert">';
          break;
        case BUCHUNG_FERTIG: // abgerechnet
          echo '<img src="img/CVJM/abgerechnet.gif" title="Abgerechnet">';
          break;
        case BUCHUNG_STORNIERT: // Storniert
          echo '<img src="img/CVJM/storno.gif" title="Storniert">';
      }
      echo '</a></td>';
      echo '<td class="zentriert">'.$Wochentage[date("w",$buchung["Von"])]."<br />";
      echo date("d.m.Y",$buchung["Von"]);
      if ( $buchung["Ankunftszeit"] != 0 )
        echo " ".date("H:i",$buchung["Ankunftszeit"]);
      VerantwortlicheAnzeigen($buchung, "Anreisename");
      echo '</td><td>';
      echo $Wochentage[date("w",$buchung["Bis"])]."<br />";
      if ( $buchung["Abnahme"] != 0 )
        echo date("d.m.Y H:i",$buchung["Abnahme"]);
      else
        echo "-nicht festgelegt-";
      VerantwortlicheAnzeigen($buchung, "Abnahmename");
      // Abfahrtszeit anzeigen
      echo '<br />';
      echo date("d.m.Y",$buchung["Bis"]);
      if ( $buchung["Abfahrtszeit"] != 0 )
        echo " ".date("H:i",$buchung["Abfahrtszeit"]);
      echo '</td><td>';
      if ( $buchung["Rechnungsstellung"] != 0 )
        echo date("d.m.Y H:i",$buchung["Rechnungsstellung"]);
      else
        echo "-nicht festgelegt-";
      VerantwortlicheAnzeigen($buchung, "Rechnungname");
      echo '</td>';
      echo '<td>';
      $squery = sql_query("SELECT Historie FROM cvjm_Smilycard WHERE Buchung_Nr = ".$buchung["Buchung_Nr"]);
      if ( $row = sql_fetch_array($squery) )
      {
          echo $row['Historie'];
      }
      else
      {
          echo '<a href="?id=5754&Buchung_Nr='.$buchung['Buchung_Nr'].'">Smileycard fehlt!</a>';
      }
      sql_free_result($squery);
      //VerantwortlicheAnzeigen($buchung, "Smilycardname");
      echo '</td>';
      
      echo '<td>'.stripslashes($buchung["Name"]." ".$buchung["Vorname"])." (".$buchung["Ort"].")";
      if ( trim($buchung["Zusatz"]) != "" )
        echo '<br />'.stripslashes($buchung["Zusatz"]);
      echo '</td>';
      echo '<td>'.ZeigeArtikel(CVJMART_ORT, $buchung["Buchung_Nr"]);
      $wo = ZeigeBereiche(CVJMART_VERPFLEGUNG, $buchung["Buchung_Nr"]);
      if ($wo != "" )
      {
        // Selbstverpflegung?
        $essenquery = sql_query("SELECT DISTINCT F_Artikel_Nr FROM ".
          TABLE_BUCHUNGSEINTRAEGE." WHERE F_Buchung_Nr = ".
          $buchung["Buchung_Nr"]." AND F_Artikel_Nr = ".CVJM_SELBSTVERPFLEGUNG);
        if ( sql_num_rows($essenquery) > 0 )
        {
          $wo = '<span class="Selbstverpflegung">Selbstverpflegung</span>';
        }
        else
        {
          $wo = "Verpflegung";
        }
        sql_free_result($essenquery);
      }
      echo '<br />'.$wo;
      $wo = ZeigeBereiche(CVJMART_PROGRAMMPAKET, $buchung["Buchung_Nr"]);
      $wo .= ZeigeBereiche(CVJMART_PROGRAMM, $buchung["Buchung_Nr"]);
      if ( $wo != "" )
      {
        echo '<br /><span class="Programm">Programm</span>';
      }
      echo "<br />".Personenanzahl($buchung)." Personen (".$Altersgruppen[$buchung["Altersgruppe"]].")";
      if ( $buchung["BetreuerW"]+$buchung["BetreuerM"] > 0 )
        echo " davon ".($buchung["BetreuerW"]+$buchung["BetreuerM"])." Betreuer";      
      if ( $schluessel > 0 ) echo '<span style="background-color:#FF8080"> ('.$schluessel.' Schlüssel)</span>';
      echo '</td>';
      echo "</tr>\n";
      if ( $Color == "helleZeile" )
        $Color = "dunkleZeile";
      else
        $Color = "helleZeile";
    }
    sql_free_result($query);
    echo $design["table_end"];
  }
}
else
{
  echo '<br /><br />';
  create_editor_input("Anzahl Tage +/- in der Anzeige","TageLZ", "text_input");
}

?>
