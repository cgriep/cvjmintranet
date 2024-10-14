<?php

/*
  AWF-Modul cvjmseminarteilnehmer
  Datei: cvjmSeminarteilnehmer.inc

  Parameter: Buchung_Nr - Die Buchungsnummer die bearbeitet wird
*/

if ( $profile['editor'] != 1 )
{
  unset($_SESSION['Sem']);
  
  // Buchungsdaten anzeigen
  include(INC_PATH.'misc/CVJM.inc');
  include(INC_PATH.'misc/cvjmBuchungstools.inc');
  include(INC_PATH.'misc/cvjmKorrespondenz.inc');
  require_once(INC_PATH.'cvjm/Buchung.class.php');
  require_once(INC_PATH.'cvjm/Adresse.class.php'); 
 // Speichern der Seminardaten
  if ( isset($docinput['Bezeichnung']) )
  {
    if ( ! is_numeric($docinput['MinTeilnehmer'])) $docinput['MinTeilnehmer'] = 0;
    if ( ! is_numeric($docinput['MaxTeilnehmer'])) $docinput['MaxTeilnehmer'] = 0;
    $docinput['Anmeldeschluss'] = convertDatumToTimestamp($docinput['Anmeldeschluss']);
    $sql = 'UPDATE '.TABLE_SEMINARE." SET Bezeichnung='".
      sql_real_escape_string($docinput['Bezeichnung']).
      "',Beschreibung='".sql_real_escape_string($docinput['Beschreibung']).
      "',MinTeilnehmer=".$docinput['MinTeilnehmer'].
      ",MaxTeilnehmer=".$docinput['MaxTeilnehmer'].
      ",Leitung='".sql_real_escape_string($docinput['Leitung']).
      "',Anmeldeschluss=".$docinput['Anmeldeschluss'].
      " WHERE F_Buchung_Nr=".$_REQUEST['Buchung_Nr'];
    if ( ! sql_query($sql) || sql_affected_rows() == 0 )
    {
      // Neuer Datensatz!
      sql_query('INSERT INTO '.TABLE_SEMINARE.' (F_Buchung_Nr,Bezeichnung,Beschreibung,'.
        'MinTeilnehmer,MaxTeilnehmer,Leitung,Anmeldeschluss) VALUES ('.
        $_REQUEST['Buchung_Nr'].",'".sql_real_escape_string($docinput['Bezeichnung'])
        ."','".
        sql_real_escape_string($docinput['Beschreibung'])."',".
        $docinput['MinTeilnehmer'].','.
        $docinput['MaxTeilnehmer'].",'".sql_real_escape_string($docinput['Leitung'])."',".
        $docinput['Anmeldeschluss'].')');
    }
    // Preise bearbeiten
    foreach ( $docinput['Preis'] as $key => $value )
    {
    	if ( ! isset($docinput['DelPreis'][$key]) )
      {
      	$value = str_replace(',','.',$value);
      	if (! sql_query('UPDATE '.TABLE_SEMINARPREISE.' SET Gebuehr='.$value.
      
          ' WHERE F_Buchung_Nr='.$_REQUEST['Buchung_Nr']." AND Gebuehrbezeichnung='".
          sql_real_escape_string($key)."'"))
          echo sql_error();
      }
      else
        sql_query('DELETE FROM '.TABLE_SEMINARPREISE.' WHERE F_Buchung_Nr='.
          $_REQUEST['Buchung_Nr']." AND Gebuehrbezeichnung='".
          sql_real_escape_string($key)."'");
    }
    $docinput['NeuPreis'] = str_replace(',','.',$docinput['NeuPreis']);
    if ( trim($docinput['NeuBez']) != '' && is_numeric($docinput['NeuPreis']) )
    {
      sql_query('INSERT INTO '.TABLE_SEMINARPREISE.
        ' (F_Buchung_Nr,Gebuehr,Gebuehrbezeichnung) VALUES ('.$_REQUEST['Buchung_Nr'].','.
        $docinput['NeuPreis'].",'".sql_real_escape_string($docinput['NeuBez'])."')");
    }
  }
  
  // Seminardaten laden
  $query = sql_query('SELECT * FROM '.TABLE_SEMINARE.' WHERE F_Buchung_Nr='.
    $_REQUEST['Buchung_Nr']);
  $seminar = sql_fetch_array($query);
  sql_free_result($query);
  $Buchung = new Buchung($_REQUEST['Buchung_Nr']);
  if ( $Buchung->isFertig() )
  {
    $check = ' readonly="readonly"';
  }
  else
  {
  	$check = '';
  }
  require_once(INC_PATH.'Smarty/Smarty.class.php');
  $Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';	
  $Smarty = new Smarty();
  $Smarty->assign('title', transform($document['title'],$document['datatype']['title']));
  $Smarty->assign('Buchung', $Buchung);
  $Smarty->assign('check', $check);
  $Smarty->assign('id', $id);
  $Smarty->assign('seminar', $seminar);
  $Preise = array();
  $query = sql_query("SELECT * FROM ".TABLE_SEMINARPREISE." WHERE F_Buchung_Nr=".
    $Buchung->Buchung_Nr." ORDER BY Gebuehrbezeichnung");  
  while ( $preis = sql_fetch_array($query) )
  {
    $Preise[] = $preis;
  }
  sql_free_result($query);
  $Smarty->assign('Preise', $Preise);
  ob_start();
  $Vorlagen = ZeigeVorlagen($Buchung->Buchung_Nr, 'Seminar');
  $Smarty->assign('Vorlagen', ob_get_contents());
  ob_end_clean();
  //$Smarty->assign('buchung', $buchung);
  $Smarty->assign('Buchungurl', get_url(get_cmodule_id('cvjmbuchung')));      
  $Smarty->assign('Adressenurl', get_url(get_cmodule_id('cvjmadressen')));
  // Teilnehmer hinzufügen
  $geaendert = false;
  if ( isset($docinput['Teilnehmer']) && is_array($docinput['Teilnehmer']))
  {
    foreach($docinput['Teilnehmer'] as $key => $Adressen_id )
    {
      if ( is_numeric($Adressen_id) )
      {
        $query = sql_query('SELECT Kunden_Nr FROM '.TABLE_ADRESSEN.' WHERE Adressen_id='.$Adressen_id);
        if ( $kundennummer = sql_fetch_row($query) )
        {
          if ( $kundennummer[0] <= 0 )
          {
            // Neue Kundennummer erzeugen
            $kundennummer[0] = holeKundenNummer($Adressen_id);
          }
          if ( ! is_numeric($docinput['Art'][$Adressen_id]) )
            $docinput['Art'][$Adressen_id] = 0;
          sql_query('INSERT INTO '.TABLE_SEMINARTEILNEHMER.
            ' (F_Buchung_Nr,F_Adressen_id,SeminarPreis,SeminarGruppe) VALUES ('.
            $Buchung->Buchung_Nr.','.
            $Adressen_id.','.$docinput['Art'][$Adressen_id].',0)');
          $geaendert = true;
        }
        sql_free_result($query);
      }
      else
        die('ungültige Adressen-id!');
    }
  }
  // Teilnehmerdaten ändern
  if ( isset($docinput['SeminarPreis']) && is_array($docinput['SeminarPreis']) )
  {
    foreach($docinput['SeminarPreis'] as $Adressen_id => $Preis )
    {
      if ( ! is_numeric($Preis) ) $Preis = 0;
      if ( $docinput['SeminarVegetarier'][$Adressen_id] == 1) 
        $Vegetarier = 1;
      else
        $Vegetarier = 0;
      if ( $docinput['SeminarSchweinelos'][$Adressen_id] == 1) 
        $Schweinelos = 1;
      else
        $Schweinelos = 0;
      if ( ! is_numeric($docinput['SeminarGruppe'][$Adressen_id]))
        $SeminarGruppe = 0;
      else
        $SeminarGruppe = $docinput['SeminarGruppe'][$Adressen_id];
      if ( ! is_numeric($docinput['Seminaranzahlung'][$Adressen_id]) )
        $docinput['Seminaranzahlung'][$Adressen_id] = 0;
      if ( is_numeric($Adressen_id) )
      {
        if ( ! sql_query('UPDATE '.TABLE_SEMINARTEILNEHMER.' SET SeminarPreis='.$Preis.
          ',SeminarAnzahlung='.$docinput['Seminaranzahlung'][$Adressen_id].
          ',SeminarGruppe='.$SeminarGruppe.
          ",SeminarVegetarier=$Vegetarier,SeminarSchweinelos=$Schweinelos," .
          "SeminarBemerkung='".sql_real_escape_string(
            $docinput['SeminarBemerkung'][$Adressen_id]).
          "',SeminarAnzahlungsBemerkung='".sql_real_escape_string(
          $docinput['SeminarAnzahlungsBemerkung'][$Adressen_id])."' WHERE F_Adressen_id=".$Adressen_id.
          ' AND F_Buchung_Nr='.$Buchung->Buchung_Nr)) echo '<div class="Fehler">'.sql_error().'</div>';
      }
    }
  }
  // Teilnehmer löschen
  if ( isset($docinput['DelTeili']) && is_array($docinput['DelTeili']) )
  {
     foreach($docinput['DelTeili'] as $key => $Adressen_id )
     {
       if ( is_numeric($Adressen_id) )
       {
         sql_query('DELETE FROM '.TABLE_SEMINARTEILNEHMER.' WHERE F_Buchung_Nr='.
           $Buchung->Buchung_Nr.' AND F_Adressen_id='.$Adressen_id);
         $geaendert = true;
       }
     }
  }
  if ( ! $Buchung->isFertig())
  {    
    $Smarty->assign('docinput', $docinput);
    $Smarty->assign('Adressenurl', get_url(get_cmodule_id('cvjmadressen')));    
    $Adressen = array();
    if ( isset($docinput["Search"]) && trim($docinput["Search"]) != "" )
    {
      // Teilnehmer suchen
      $Search = sql_real_escape_string($docinput["Search"]);
      $sql = "SELECT Adressen_id FROM ".TABLE_ADRESSEN." WHERE NOT Geloescht AND ".
          "(Name REGEXP '$Search' OR Vorname REGEXP '$Search' OR Strasse ".
          "REGEXP '$Search' OR Ort REGEXP '$Search' ".
          "OR Telefon1 REGEXP '$Search' OR Telefon2 REGEXP '$Search' OR ".
          "Kunden_Nr REGEXP '$Search' OR PLZ REGEXP '$Search') ".
          "ORDER BY Name,Vorname";
      $query = sql_query($sql);
      while ( $adresse = sql_fetch_array($query))
      {
      	$Adressen[] = new Adresse($adresse['Adressen_id']);
      }
      sql_free_result($query);
    }
    $Smarty->assign('Adressen', $Adressen);
    $Smarty->assign('sem', urlencode('id='.$id.'&Buchung_Nr='.$Buchung->Buchung_Nr));
  } // wenn Buchung nicht fertig
  setlocale(LC_ALL, 'de_DE@euro', 'de_DE', 'de', 'ge');
  $Smarty->display('Seminardaten.tpl');
  
  // nun noch die Teilnehmer zeigen und eine Auswahl zulassen
  include (INC_PATH . 'cvjm_ajax.php');
  $xajax->printJavascript('./javascript');

  /* gibt die Korrespondenz aus */
  $Vorlagen = ZeigeVorlagen($Buchung->Buchung_Nr, 'Seminare');
      
  if ( ! $query = sql_query('SELECT * FROM ('.TABLE_SEMINARTEILNEHMER.' INNER JOIN '.TABLE_ADRESSEN.
    ' ON F_Adressen_id=Adressen_id) INNER JOIN '.TABLE_ANREDEN.
    ' ON '.TABLE_ADRESSEN.'.F_Anrede_id=Anrede_id WHERE F_Buchung_Nr='.
    $Buchung->Buchung_Nr.' ORDER BY Name, Vorname')) echo sql_error();
  $Anzahlen =array();
  for ( $i= 1; $i< 7; $i++)
  {
    $Anzahlen[$i][0] = 0;
    $Anzahlen[$i][1] = 0;
  }
  $Vegetarier = 0;
  $Schweinelos = 0;
  while ( $teilnehmer = sql_fetch_array($query) )
  {
     // Ausgabe der Teilnehmer und möglichkeit, diesen einen Brief zu schreiben.
     // Eingabefeld für Anzahlung, -bemerkung
     if ( $teilnehmer['SeminarGruppe'] == 0 ) $teilnehmer['SeminarGruppe'] = '';      
     
     if ( $teilnehmer['Geburtsdatum'] != 0 )
       $Alter = floor((time()-$teilnehmer['Geburtsdatum'])/31536000);
     else
       $Alter = 0;
     if ( $teilnehmer['Geschlecht'] > 1)
       $Alter = 0;
     if ( $Alter > 64 )
       $Anzahlen[6][$teilnehmer['Geschlecht']]++;
     elseif ( $Alter >27 )
       $Anzahlen[5][$teilnehmer['Geschlecht']]++;
     elseif ( $Alter >14 )
       $Anzahlen[4][$teilnehmer['Geschlecht']]++;
     elseif ( $Alter > 5 )
       $Anzahlen[3][$teilnehmer['Geschlecht']]++;
     elseif ( $Alter >1 )
       $Anzahlen[2][$teilnehmer['Geschlecht']]++;
     else
     {
       $Anzahlen[1][$teilnehmer['Geschlecht']]++;  // wenn nichts angegeben immer in kleinste Altersgruppe
     }
     $teilnehmer['Alter'] = $Alter;
     if ( $teilnehmer['SeminarVegetarier'] ) 
     {
     	$Vegetarier++;
     }
     if ( $teilnehmer['SeminarSchweinelos'] ) 
     {
     	$Schweinelos++;
     }  
     $teilnehmer['Vorlagen'] = ListeVorlagen($Buchung->Buchung_Nr, $teilnehmer['Adressen_id'], $Vorlagen, 'Seminare');
     $Teilnehmer[] = $teilnehmer;     
  }
  
  $Smarty->assign('SeminarTeilnehmer', $Teilnehmer);
  $Smarty->assign('Teilnehmeranzahl', Count($Teilnehmer));
  $Smarty->display('Seminardaten_Liste.tpl');
  // Errechnete Altersgruppen in die Buchung schreiben
  
  if ( $geaendert || isset($docinput["Aktualisieren"]) )
  {
    $sql = "Vegetarier=$Vegetarier,Schweinelos=$Schweinelos,";
    $Gesamt = 0;
    for ( $i= 1; $i< 7; $i++)
    {
      $sql .= "Personen$i=".$Anzahlen[$i][0].",";
      $sql .= "Personen".$i."w=".$Anzahlen[$i][1].",";
      $Gesamt += $Anzahlen[$i][0]+$Anzahlen[$i][1];
    }
    $sql .= "Logtext=CONCAT('".date("d.m.Y H:i").
       " Teilnehmerzahl/Vegetarier/Schweinelos per Seminar auf $Gesamt/$Vegetarier/$Schweinelos geändert ".
      get_user_nickname()."\n',Logtext) ";
    if ( ! sql_query("UPDATE ".TABLE_BUCHUNGEN." SET ".$sql." WHERE Buchung_Nr=".$Buchung->Buchung_Nr))
      echo sql_error();
  }
  sql_free_result($query);
  
}

?>
