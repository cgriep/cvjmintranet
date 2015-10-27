<?php

/*
  cvjmImportDB.inc
  AWF-Name: cvjmimportdb

*/

if ( $profile["editor"] != "1" )
{
  include(INC_PATH."misc/CVJM.inc");
  if ( isset($_FILES['Datei']['tmp_name']) &&
       $_FILES['Datei']['tmp_name'] != '' )
  {
    echo "Lade ".$_FILES["Datei"]["name"]."<br />";
    $handle = fopen($_FILES['Datei']['tmp_name'],"r");
    // erste Zeile lesen
    $ueberzeile = fgets($handle, 4096);
    $ueberzeile = str_replace('"','', $ueberzeile);
    $felder = explode(";",$ueberzeile);
    $sql = "INSERT INTO ".TABLE_ADRESSEN." (";
    $sql .= implode($felder, ",").") VALUES ('";
    while ( ! feof($handle) ) {
      $zeile = fgets($handle, 4096);
      $zeile = implode("','",addslashes(str_replace('"','',explode(";",$zeile))));
      $sql2 = $sql.$zeile."')";
      if ( ! mysql_query($sql2) ) echo "Fehler beim Importieren ($sql2):".mysql_error()."<br />";
      else echo "Hinzugefügt Nr. ".sql_insert_id().'<br />';
    }
    fclose($handle);
    echo '<hr />';
  }
  echo '<form action="'.get_url($id).'" method="post" enctype="multipart/form-data">';
  echo 'Adressenimport-Datei (CSV) <input type="file" name="Datei"><br />';
  echo '<input type="submit" value="Datei einlesen">';
  echo '</form><br />';

} // not editor

?>
