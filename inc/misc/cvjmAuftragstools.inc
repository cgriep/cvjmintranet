<?php

/*

   cvjmAuftragstools.inc
   Hilfsfunktionen für Aufträge

*/

/**
*  Betroffen enthält kommasepariert die Betroffenen des Auftrages in der form:
*  gXX  - Gruppe XX
*  XX - Benutzer XX
*  XX ist die jeweilige AWF-Nummer
* @param string $Titel
* @param string $Beschreibung 
* @param integer $Prioritaet
* @param string $Betroffen - kommaseparierte Liste der Betroffenen (ID, gID für Gruppen)
* @param date $BearbeitenBis
* @param integer $Ort
* @param integer $Status 1-4, 4=erledigt
* @param integer $Autor ID des Autors, Std=Session_DBID 
*/
function insertAuftrag($Titel, $Beschreibung, $Prioritaet, $Betroffen, $BearbeitenBis, $Ort = -1,
                       $Status = 1, $Autor = SESSION_DBID)
{
  global $Prioritaeten;
  global $groups;
  if ( ! is_numeric($Prioritaet) ) $Prioritaet = 0;
  if ( ! is_numeric($Ort) ) $Ort = -1 ;
  if ( ! is_numeric($Status) ) $Status = 1;
  if ( ! is_numeric($Prioritaet) ) $Prioritaet = 2;
  $sql = 'INSERT INTO '.TABLE_AUFTRAG." (Datum, Titel, Beschreibung, Prioritaet, Autor, Ort, Status, Betroffene, BearbeitenBis) VALUES ('";
  $sql .= date('Y-m-d H:i')."','";
  $sql .= sql_real_escape_string($Titel)."','".sql_real_escape_string($Beschreibung);
  $sql .= "',".$Prioritaet;
  $sql .= ",$Autor,".$Ort.','.$Status.',';
  $sql .= "'".sql_real_escape_string($Betroffen)."',";
  $sql .= $BearbeitenBis.')';
  // Mail an Betroffene schicken
  $wer = explode(',',$Betroffen);
  $ZusatzText = '';
  if ( SESSION_DBID != $Autor )
    $ZusatzText = 'Automatisch erstellt durch '.get_user_nickname(SESSION_DBID)."\n";
  $Adressat = '';
  foreach ( $wer as $value )
  {
  	if ( substr($value, 0, 1) == 'g' ) {
  		$Adressat .= 'Gruppe '.$groups[substr($value,1)].', ';
  	}
  	elseif ( is_numeric($value) )
  	{
  		$Adressat .= get_user_nickname($value).', ';
  	}
  }
  if ( $Adressat != '' ) $Adressat = substr($Adressat, 0, strlen($Adressat)-2);
  if ( $BearbeitenBis != 0 ) 
  {
  	$BearbeitenBis = 'Erledigen bis '.date('d.m.Y H:i', $BearbeitenBis)."\n";
  }
  else
    $BearbeitenBis = '';
  foreach ( $wer as $key => $value )
    if ( substr($value, 0, 1) == 'g' ) {
      // alle Mitglieder der Gruppe holen echo $groups[substr($value,1)].'<br />';
      $sql2 = 'SELECT DISTINCT user_id FROM '.TABLE_USERDATA.' INNER JOIN '.TABLE_USERS.
      ' ON '.TABLE_USERS.'.id=user_id'.
      " WHERE name='group_".substr($value,1)."' AND valid";
      $qu = sql_query($sql2);
      while ( $row = sql_fetch_row($qu) ){
        if ( $row[0] != SESSION_DBID )
        {
           echo 'E-Mail an:'.get_user_email($row[0]).'<br />';
        	mail(get_user_email($row[0]),
           '[Auftrag '.$Titel.'] '.LANG_ERTEILT,           
           'Von: '.get_user_email($Autor).' ('.get_user_nickname($Autor).
           ")\n".$ZusatzText.'Datum: '.date('d.m.Y H:i')."\n".
           'Zugewiesen an: '.$Adressat."\n".
           addslashes($Beschreibung)."\nOrt: ".Ortsname($Ort, false).
           "\nPriorität: ". $Prioritaeten[$Prioritaet]['name']."\n".
           $BearbeitenBis,             
           'From: '.get_user_nickname($Autor).' <'.get_user_email($Autor).'>');
        }
      }
      sql_free_result($qu);
    }
    elseif ( is_numeric($value) > 0 )
       if ( $value != SESSION_DBID )
       {
            mail(get_user_email($value), 
            '[Auftrag '.$Titel.'] '.LANG_ERTEILT,  
            'Von: '.get_user_email($Autor).' ('.get_user_nickname($Autor).
             ")\n$ZusatzText.'Datum: ".date('d.m.Y H:i')."\n".
             $Beschreibung."\nOrt: ".Ortsname($Ort,false)."\nPriorität: ".
             $Prioritaeten[$Prioritaet]['name'],
             'From: '.get_user_nickname($Autor).' <'.get_user_email($Autor).'>');
       }
  if ( ! sql_query($sql) )
  {
    echo '<strong>&gt;&gt; Fehler: '. sql_error().'</strong><br /><br />';
    return -1;
  }
  else
    return sql_insert_id();
}

function updateAuftrag($Auftrag, $Titel, $Beschreibung, $Bemerkung, $Prioritaet,
   $Betroffen, $BearbeitenBis, $Ort = -1, $Status = 1)
{
  if ( is_numeric($Auftrag) )
  {
    $qu = sql_query('SELECT Autor FROM '.TABLE_AUFTRAG.' WHERE id = '.$Auftrag);
    if ( sql_num_rows($qu) == 1 ) {
      if ( ! is_numeric($Prioritaet) ) $Prioritaet = 0;
      if ( ! is_numeric($Ort) ) $Ort = -1 ;
      if ( ! is_numeric($Status) ) $Status = 0;
      if ( ! is_numeric($BearbeitenBis)) $BearbeitenBis = 0;
      // Update, nur Autoren dürfen Auftrag ändern
      $sql = 'UPDATE '.TABLE_AUFTRAG." SET Titel = '".sql_real_escape_string($Titel);
      $sql .= "', Bemerkung = '".sql_real_escape_string($Bemerkung);
      $sql .= "', Beschreibung = '".sql_real_escape_string($Beschreibung)."',";
      $sql .= 'Prioritaet = '.$Prioritaet.',';
      $sql .= 'Status = '.$Status.',';
      $sql .= 'BearbeitenBis='.$BearbeitenBis.',';
      $sql .= "Betroffene = '".sql_real_escape_string($Betroffen)."',";
      $sql .= 'Ort = '.$Ort;
      $sql .= ' WHERE id = '.$Auftrag;
      if (! sql_query($sql)) echo '&gt;&gt;&gt; Fehler '.sql_error().'<br />';
    }
    sql_free_result($qu);
  }
}

?>