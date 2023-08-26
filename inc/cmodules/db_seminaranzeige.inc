<?php
/*
        The author is Christoph Griep (cgriep@fricom.com)
        Last update: 11.07.2003
*/

if ( $profile['editor'] != "1" ) {
  echo '<h1 clear="all">'.transform($document['title'], $document['datatype']['title']).'</h1>';
  if($document['image'] != '' || $document['body'] != '') {
    echo '<p class="justify">';
    // Image
    echo transform($document['image'], $document['datatype']['image']);
    // Text
    echo transform($document['body'], $document['datatype']['body']).'</p><br clear="all" />';
  }
  // Datenbank auslesen
  if ( isset($_REQUEST[$document['IDFeld']]) && is_numeric($_REQUEST[$document['IDFeld']]))
  {
    // Hier die Feldnamen eintragen, Reihenfolge ist wichtig!
    $sql = "select Rubrik, Seminarnr ,Name ,Erg�nzung ,Zielgruppe, " .
    		"Ankunft, Abfahrt, Anfang, Ende, Werbetext, Bemerkungen, " .
    		"Lernziele, Leitung, Kooperation, Leistungen, NameKosten1 " .
    		",Kosten1,NameKosten2 ,Kosten2, NameKosten3 , Kosten3 , " .
    		"NameKosten4,  Kosten4 ,Mindestteilnehmer ,Maximalteilnehmer," .
    		"MinAlter ,Anmeldeschluss  " .
    		"from " . $document['Tabelle']." WHERE " .
      $document['IDFeld'] . " = " . $_REQUEST[$document['IDFeld']];
    echo '<style type="text/css">@import url(seminar.css) screen;</style>';    
    if ( $query = sql_query($sql)) {
      $data = sql_fetch_array($query);
  // *********************************
// Tabelle damit Daten immer untereinander stehen
      echo '<h1>'.$data['Name'].'</h1>';
      echo $data['Erg�nzung'].'<br />';
      if ( $data['Zielgruppe']!= '') 
        echo 'Zielgruppe: '.$data['Zielgruppe'].'<br />';
      echo 'Zeitraum der Veranstaltung: '.date('d.m.Y',strtotime($data['Ankunft']));
      echo ' '.$data['Anfang'].' ';
      echo ' bis '.date('d.m.Y',strtotime($data['Abfahrt'])).' '.$data['Ende'].'<br />';      
      echo '<p>'.transform($data['Werbetext'],"html").'</p>';
      echo '<p>'.$data[10].'</p>';
      if ( $data['Lernziele'] !='') 
        echo '<p>Ziele: '.$data['Lernziele'].'</p>';
      echo 'Leitung: '.$data['Leitung'];
      if ( $data['Kooperation'] != '') echo ' in Kooperation mit '.$data['Kooperation'];
      echo '<br />';
	  echo '<h2>Unsere Leistungen:</h2>';
      echo '<p>'.$data['Leistungen'].'</p>';
	  echo "<h2>Preise:</h2>";
	  echo '<dl>';
      echo '<dt>'.$data['NameKosten1']."</dt>";
      echo '<dd>'.$data['Kosten1'].'</dd>';
      if ( $data['Kosten2'] != 0){
      echo '<dt>'.$data['NameKosten2']."</dt>";
      echo '<dd>'.$data['Kosten2'].'</dd>';
      }
      
      if ( $data['Kosten3'] != 0)
      {
        echo '<dt>'.$data['NameKosten3']."</dt>";
        echo '<dd>'.$data['Kosten3'].'</dd>';
      }
      if ( $data['Kosten4'] != 0){
      echo '<dt>'.$data['NameKosten4']."</dt>";
      echo '<dd>'.$data['Kosten4'].'</dd>';
      }
      echo '</dl>';

//Teilnehmer
      echo "<h2>Teilnehmer:</h2>";
      echo '<dl>';
      echo "<dt>Mindestteilnehmerzahl:</dt>";
      echo '<dd>'.$data['Mindestteilnehmer'].'</dd>';
      echo "<dt>Maximalteilnehmerzahl:</dt>";
      echo '<dd>'.$data['Maximalteilnehmer'].'</dd>';
      echo "<dt>Mindestalter:</dt>";
      echo '<dd>'.$data['MinAlter'].'</dd>';
      echo '</dl>';
//Anmeldung
      echo "Die Anmeldung erfolgt schriftlich �ber das Anmeldeformular. Anmeldeschluss ist der ";
      echo date('d.m.Y',strtotime($data['Anmeldeschluss']));
      echo '<br/>';


// Ende der Tabelle
    }
    else
      echo sql_error();
    sql_free_result($query);
  }
  if(isset($parents)) {
    echo '[ <a href="'.get_url($document['parent_id'],"").'">'.LANG_SHOW_COMPLETE_DOC.'</a> ]';
  }

}

if ( $profile['editor']=='1' )
{
  echo '<br />';
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('body','body','area_input');
  echo '<br /><br />';
  create_editor_input('Tabelle','Tabelle','text_input');
  echo '<br /><br />';
  create_editor_input('ID-Feld','IDFeld','text_input');
}

?>
