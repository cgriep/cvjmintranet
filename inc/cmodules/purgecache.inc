<?php

/* Modul, um den Cache zu leeren.
&Uuml;berpr&uuml;ft wird folgendes:
- Datei INC/DATA/common_cmodules
- Alle Dateien in cache/modules

*/

load_lang("cache");
if ( $profile['editor'] != "1" ) {
echo "Klicken, um den Cache zu leeren!";
?>
<form action=".">
<input type="hidden" name="id" value="<?=$id?>">
<input type="hidden" name="Remove" value="1">
<input type="submit" value="<?=LANG_CACHE_LEEREN?>">
</form>

<?php
echo "Klicken, um die Rechte in files/0 korrekt zu setzen";
echo " (Notwendig, wenn Dateien nicht runtergelanden werden können).";
?>

<form action=".">
<input type="hidden" name="id" value="<?=$id?>">
<input type="hidden" name="Rechte" value="1">
<input type="submit" value="<?=LANG_RECHTE_SETZEN?>">
</form>

<?php
echo "Besucherzähler zurücksetzen (alle Seiten und Gesamtanzahl auf 0 setzen)";
?>

<form action=".">
<input type="hidden" name="id" value="<?=$id?>">
<input type="hidden" name="Zaehler" value="1">
<input type="submit" value="<?=LANG_ZAEHLER_RUECKSETZEN?>">
</form>

<?php
}
if ( isset($_REQUEST["Zaehler"]) )
{
  // alle Seiten auf 0 setzen
  sql_query("UPDATE ".TABLE_NODES." SET views=0");
  $document['views'] = 0;
}

if ( isset($_REQUEST["Remove"]) )
{
  // Cache leeren
  if ( @unlink(INC_PATH.DATA_PATH."vars/common_cmodules") ) echo "cmodules-Cache gelöscht.<br />";
  $d = dir('cache/modules');
  while($entry=$d->read()) {

    if ( @unlink($d->path . "/". $entry) ) echo "$entry gel&ouml;scht<br />";
  }
  $d->close();
  echo "Cache wurde geleert.";
}

function recDir($dirname)
{
  echo "Bearbeite $dirname<br />";
  if ( is_dir($dirname) ) {
    $d = dir($dirname);
    while ($entry = $d->read() )
    {
      $full = $d->path . "/".$entry;
      if ( substr($entry, 0, 1) != "." ) {
        echo "$full<br />";
        chmod($full, CONFIG_CREATE_MASK);
        if ( is_dir($full) ) {
          //echo "Dir: $full<br />";
          recDir($full);
        }
      }
    }
  }
}

if ( isset($_REQUEST['Rechte'])) {
  echo "Beginne<br />";
  recDir("files/0");
  recDir("files/1");
  recDir("users/public/0");
  recDir("users/private");
  recDir("cache/0");
  recDir("cache/1");
  recDir("inc/data/nodes/0");
  echo "Fertig<br />";
}



?>