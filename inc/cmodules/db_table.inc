<?
/*
        The author is Christoph Griep (cgriep@fricom.com)
        Last update: 03.07.2003
        Einstellungen:
        Tabelle: Der Tabellenname, Join ist m&ouml;glich
        Feldnamen: mit |getrennt die Feldnamen, die ben&ouml;tigt werden
           Besonderheit: >Len>Feldname bedeutet, dass das Feld Len pt breit sein soll
        Id-Feld: Feldname des Prim&auml;rschl&uuml;ssels
        Auswahlkriterien: Auswahlkriterien (ohne WHERE)
        Summenfelder: ,mit | getrennt die Feldnamen, die Summiert werden sollen
        Sortierfelder: mit | getrennt die Feldnamen, nach denen Sortiert werden soll
        (in entspr. Reihenfolge)

*/
load_lang('db_table');
if ( $profile['editor'] != "1" ) {
  echo '<h1>'.transform($document['title'], $document['datatype']['title']).'</h1>';
  if($document['image'] != '' || $document['body'] != '') {
    echo '<p class="justify">';
    // Image
    echo transform($document['image'], $document['datatype']['image']);
    // Text
    echo transform($document['body'], $document['datatype']['body']).'</p><br clear="all" />';
  }
  $Auswahlkriterien = $document['Auswahlkriterien'];
  $suche ="";
  $Suchkriterien = "";
  // Suchfeld anzeigen
?>
  <form action="<?=get_url($id)?>" method="post">
  <?=LANG_WONACH_SUCHEN?>
  <input type="text" name="suche">
  <input type="submit" value="<?=LANG_SEARCH?>">
  <?php
  if ( isset($_REQUEST["Sort"]) )
    echo '<input type="hidden" name="Sort" value="'.$_REQUEST["Sort"].'">';
  echo '</form>';
  if ( isset($_REQUEST["suche"]) )
  {
    $suche = addslashes($_REQUEST["suche"]);
    echo "<p><b>" . LANG_GESUCHT_WURDE_NACH." <i>$suche</i></b></p>";
  }
  $Summe = explode("|",$document['Summenfelder']);
  $A_Summe = array();
  $Feldnamen = '';
  $Feldanzahl = 0;
  if ( $Auswahlkriterien != '' || $suche != '')
    $Auswahlkriterien = " WHERE " . $Auswahlkriterien;
  $Feld =  strtok($document['Felder'], '|');
  if ( ! $document['Datensatz'] )
    include(INC_PATH.'misc/tabelle_tabelle.inc');
  else
    include(INC_PATH.'misc/tabelle_datensatz.inc');
  echo "</p>";
  echo '<p><b><big>' . $Gesamtanzahl . " Datens&auml;tze gefunden.</big></b></p>";
  sql_free_result($query);
  if( SESSION_STATUS == 'ok' && $profile[$document['NewDS']] == '1' && is_numeric($document['EditDokuNr'])) {
    echo ' [ <a href="'.get_url($document['EditDokuNr'],$document['IDFeld'].'=').'">'.LANG_NEW_DATA.'</a> ]';
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
  create_editor_input('Felder','Felder','text_input');
  echo '<br /><br />';
  create_editor_input('Felder untereinander','Datensatz','onoff_input');
  echo '<br /><br />';
  create_editor_input('Auswahlkriterien (ohne WHERE, keine " !)','Auswahlkriterien','text_input');
  echo '<br /><br />';
  create_editor_input('Summenfelder','Summenfelder','text_input');
  echo '<br /><br />';
  create_editor_input('Sortierfelder','Sortierfelder','text_input');
  echo '<br /><br />';
  create_editor_input('Markierauswahlfeld','Markierung','text_input');
  echo '<br /><br />';
  create_editor_input('Markierbedingung', 'Markierbedingung','text_input');
  echo '<br /><br />';
  create_editor_input('Markierungsfarbe (HTML)','Markierungsfarbe','text_input');
  echo '<br /><br />';
  create_editor_input('ID-Feld','IDFeld','text_input');
  echo '<br /><br />';
  create_editor_input('ID Aufzurufendes Dokument','EditDokuNr','text_input');
  echo '<br /><br />';
  create_editor_input('Link-Feld','LinkFeld','text_input');
  echo '<br /><br />';
  create_editor_input('Link-URL (ID wird angehängt)','LinkURL','text_input');
  echo '<br /><br />';
  create_editor_input('Berechtigung neuer Datensatz','NewDS','text_input');
  echo '<br /><br />';
  create_editor_input('Anzahl Datensätze pro Seite','Anzahl','text_input');
}

?>