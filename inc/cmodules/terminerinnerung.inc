<?php
/*
        Copyright (C) 2003 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Christoph Griep (griep@oszimt.de)
        Last update: 26.03.2003
        AWF-Name: Termin

*/

if ( ! isset($_REQUEST['Termin_id']) || ! is_numeric($_REQUEST['Termin_id']))
  {
  	echo '<div class="Fehler">Keine Termin-ID angegeben</div>';
  }
elseif($profile['editor']!='1') {
  include(INC_PATH.'misc/Termin.inc');
  
  if ( isset($_REQUEST['SaveErinnerung']))
  {
  	if (! is_numeric($docinput['Art']))
  	  $docinput['Art'] = 0;
  	// Daten entfernen
  	foreach ( $docinput['Entfernen'] as $value)
  	{
  		$sql = 'DELETE FROM cvjm_Erinnerung WHERE Termin_id='.
  		  $_REQUEST['Termin_id'].' AND User_id='.$value;
  		
  		if ( ! sql_query($sql)) echo sql_error();
  	}
  	// Daten speichern
  	foreach ( $docinput['Betroffen'] as $value )
  	{
  		$sql = 'INSERT INTO cvjm_Erinnerung (Termin_id,User_id,Art) VALUES (' .
  				$_REQUEST['Termin_id'].','.$value.','.$docinput['Art'].')';
  		sql_query($sql);
  	}
  	foreach ( $docinput['Gruppe'] as $value )
  	{
  		//sql_query('')
      $qres2 = sql_query ('SELECT user_id FROM '.TABLE_USERDATA.
        " WHERE name = 'group_".$value."' AND value = '1'");
      while ( $row2 = sql_fetch_row($qres2) ) {
        $sql = 'INSERT INTO cvjm_Erinnerung (Termin_id,User_id,Art) VALUES (' .
  				$_REQUEST['Termin_id'].','.$row2[0].','.$docinput['Art'].')';
  		sql_query($sql);
      }
      sql_free_result($qres2);
    }
  }
  
  if($document['title'] != '')
      echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
  
  echo 'Erinnerung f�r Termin '.$_REQUEST['Termin_id'];
  echo '<form action="'.get_url($id,'Termin_id='.$_REQUEST['Termin_id']).'" method="post">';
  echo 'Vorhandene Erinnerungen (markieren zum Entfernen)<br />' .
  		'<select name="docinput[Entfernen][]" multiple="multiple">';
  $query = sql_query('SELECT * FROM cvjm_Erinnerung WHERE Termin_id='.$_REQUEST['Termin_id']);
  while ( $Termin = sql_fetch_array($query) )
  {
  	echo '<option value="'.$Termin['User_id'].'">'.
  	  get_user_nickname($Termin['User_id']);
  	echo ' ('.$Termin['Art'].' Tage vorher)';
  	echo '</option>';
  }
  sql_free_result($query);
  echo '</select><br />';
  echo 'Benachrichtigung erfolgt per Mail ';
  echo '<select name="docinput[Art]">' .
        '<option value="0" selected="selected">am selben Tag</option>'.
        '<option value="1">einen Tag vorher</option>'.
        '<option value="2">zwei Tage vorher</option>'.
        '<option value="7">eine Woche vorher</option>'.
  		'</select>';  
  $User = search_users();
  asort($User);
  reset($User);
  echo '<br />an <select name="docinput[Betroffen][]" size="5" multiple="multiple">';
  foreach ( $User as $key => $value) 
  {
      echo '<option value="'.$key.'"';
      if ( in_array($key, explode(",",$document['Person'])) )
        echo ' selected="selected"';
      echo '>'.$value['nickname'].'</option>';
  }
  echo '</select> <small>Strg halten, um mehrere zu markieren</small><br />';
  echo 'an Gruppe ';
  echo '<select name="docinput[Gruppe][]" size="5" multiple="multiple">';
  foreach ($groups as $key => $gruppe) {
        // Nur Gruppenmitglieder k�nnen die Gruppe einladen!
        if ( InGruppe($key) || $profile[$document['Terminrecht']] == '1' )
        {
          echo '<option value="' . $key . '"';
          if ( in_array($key, explode(",",$document['Gruppe'])) )
            echo ' selected="selected"';
          echo '>';
          echo stripslashes($gruppe) . '</option>';
        }
  }
  echo '</select> <small>Strg halten, um mehrere zu markieren</small><br />';
  echo '<input type="submit" value="Erinnerungseinstellungen speichern" ' .
  		'name="SaveErinnerung"/>';
  echo '</form>';
  echo('[ <a href="'.get_url($_REQUEST['Termin_id']).'">Zur�ck zum Termin'.
      '</a> ]');
}
?>