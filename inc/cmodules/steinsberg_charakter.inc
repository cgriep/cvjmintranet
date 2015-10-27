<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.06.2002
*/

load_lang('steinsberg');

$bbasedir = get_dir_name('files',$id).'/published/';
$SpielerNr = $document['SpielerID'];
if(SESSION_STATUS == 'ok' && ! $profile['editor'] == "1" &&
  ($SpielerNr == SESSION_DBID || $profile['SteinsbergSL'] == "1") ) {
  $prow = get_profile($SpielerNr, false);
  $SpielerName = $prow['Vorname']." ".$prow['Name'];
  if ( $profile["SteinsbergSL"] == "1" )
    echo 'Zur&uuml;ck zur <a href="?id='.get_cmodule_id('steinsberg_liste').'&Veranstaltung='.$_REQUEST['Ref'].'">Liste</a>';
// Abspeichern der Daten
  if(isset($_REQUEST['savechar']) ) {
        while (list($key, $value) = each($docinput) )
        {
          add_nodedata($id, $key, transform(trim($value),'text'), $flavour);
        }
        global $flavour;
        global $document;
        $document = get_node($id, $flavour, false);
        if ( $_FILES['spielerbild']['size'] <= 150000 && $_FILES['spielerbild']['size'] > 0) {
          if(!file_exists($bbasedir)) {
            mkdirs($bbasedir, CONFIG_CREATE_MASK);
          }
          if ( file_exists(BASE_PATH.$bbasedir.'Spielerbild.jpg') )
             unlink (BASE_PATH.$bbasedir.'Spielerbild.jpg');
          move_uploaded_file($_FILES['spielerbild']['tmp_name'], BASE_PATH.$bbasedir.'Spielerbild.jpg');
          chmod (BASE_PATH.$bbasedir.'Spielerbild.jpg', CONFIG_CREATE_MASK);
        }
        if ( $document['title'] == "" ) {
          update_nodedata ($id, 'title', '(unbenannter Charakter)', $flavour);
          $document['title'] = "(unbenannter Charakter)";
        }
        echo '<b>&gt;&gt; Ihre Charakterdaten wurden gespeichert!</b><br /><br />';
  }

?>
<p>Um auf deinen Charakter m&ouml;glichst gut eingehen zu k&ouml;nnen, beantworte bitte folgende Fragen
zu deinem Charakter. Am Ende gibt es zus&auml;tzlich die M&ouml;glichkeit, Hintergrundgeschichten und andere
Informationen in Dateiform hochzuladen. <br />
<br />
<?
  if ( isset($_REQUEST['Ref']))
    echo 'Zur&uuml;ck zur <a href="?id='.$_REQUEST['Ref'].
      $Verweis.'">Anmeldung</a>';
?>
<br />
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
<?=$design['table_begin'];?>
<tr bgcolor="<?=$design['table_row_dark']?>"><th>Charakterhintergrund</th></tr>
<tr><td>Spieler: <?=$SpielerName?>
</td></tr>
<tr><td>
<input type="hidden" name="id" value="<?=$id?>" />
<? if ( isset($_REQUEST['Ref'])) ?>
<input type="hidden" name="Ref" value="<?=$_REQUEST['Ref']?>" />
Charaktername<br />
<input type="text" name="docinput[title]" size="40" value="<?=stripslashes($document['title'])?>" /><br />
<br />
Rasse<br />
<input type="text" name="docinput[Rasse]" size="40" value="<?=stripslashes($document['Rasse'])?>" /><br />
<br />
Contage<br />
<input type="text" name="docinput[Contage]" size="5" value="<?=stripslashes($document['Contage'])?>" /><br />
<br />
Kurzbeschreibung des Charakters (nicht mehr als 5 Zeilen)<br />
<textarea name="docinput[Charakterbeschreibung]" cols="60" rows="5"><?=transform($document['Charakterbeschreibung'], 'clean')?>
</textarea><br /><br />
<?=LANG_PICTURE?> (max. 150 kB, muss .jpg-Datei sein)<br />
<input type="file" name="spielerbild">
<?php
  if ( file_exists($bbasedir.'Spielerbild.jpg') ) {
    echo '&nbsp;&nbsp;&nbsp;<a href="'.$bbasedir.
      'Spielerbild.jpg">vorhandenes Bild allein</a><br />';
    echo '<img src="'.$bbasedir.'Spielerbild.jpg">';
  }
  ?>
<br />
<br />
Ich gehe in Dungeons <input type="radio" name="docinput[Dungeon]" value="ja"<? if( $document['Dungeon'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[Dungeon]" value="nein"<? if( $document['Dungeon'] == 'nein') echo ' checked'; ?> /> nein
<br /><br />
Ich will Tr&auml;umen <input type="radio" name="docinput[TraumOk]" value="ja"<? if( $document['TraumOk'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[TraumOk]" value="nein"<? if( $document['TraumOk'] == 'nein') echo ' checked'; ?> /> nein<br />
<br />&nbsp;
</td></tr>
<tr bgcolor="<?=$design['table_row_dark']?>"><th>Fragen zum Charakter</td></tr>
<tr bgcolor="<?=$design['table_row_dark']?>"><td align="center">Bitte in Kurzform beantworten!</td></tr>
<tr><td>
Was ist dem Charakter wichtig, welche Ziele verfolgt der Charakter?<br />
<textarea name="docinput[Ziele]" cols="60" rows="5"><?=transform($document['Ziele'],'clean')?>
</textarea>
<br /><br />
Woran glaubt der Charakter?<br />
<textarea name="docinput[Glaube]" cols="60" rows="5"><?=transform($document['Glaube'],'clean')?>
</textarea>
<br /><br />
Vor was f&uuml;rchtet er sich, welche &Auml;ngste hat er?<br />
<textarea name="docinput[Angst]" cols="60" rows="5"><?=transform($document['Angst'],'clean')?>
</textarea>
<br /><br />
Woran liegt ihm am meisten, was w&uuml;rde ihm am meisten fehlen?<br />
<textarea name="docinput[Liebe]" cols="60" rows="5"><?=transform($document['Liebe'],'clean')?>
</textarea>
<br /><br />
Wovon tr&auml;umt der Charakter?<br />
<textarea name="docinput[Traum]" cols="60" rows="5"><?=transform($document['Traum'],'clean')?>
</textarea>
<br /><br />
Welche dunklen Punkte gibt es in der Vergangenheit des Charakters?<br />
<textarea name="docinput[DunkelPunkt]" cols="60" rows="5"><?=transform($document['DunkelPunkt'],'clean')?>
</textarea>
<br /><br />
Angenommen, der Charakter kommt in ein karg ausgestattetes Zimmer mit Tisch,
Sitzgelegenheit und Bett. Welchen der folgenden Gegenst&auml;nde nimmt er an sich und
was macht er damit?<br />
<select name="docinput[Gegenstand]">
<?php
  $gegenstaende = Array();
  $gegenstaende[] = "schwarze Kerze und Streichholz";
  $gegenstaende[] = "Buch mit unleserlichen Runen auf dem Einband";
  $gegenstaende[] = "leuchtendes Kristall";
  $gegenstaende[] = "St&uuml;ck Kaminholz";
  reset($gegenstaende);
  while ( list ($key, $was) = each($gegenstaende) )
  {
    echo '<option ';
    if ( $document['Gegenstand'] == $was ) echo " selected";
    echo ">$was</option>";
  }
?>
</select><br />
<textarea name="docinput[Aktion]" cols="60" rows="5"><?=transform($document['Aktion'],'clean')?>
</textarea>
</td></tr>
<?=$design['table_end']?>
<br />
<br />
<input type="submit" name="savechar" value="Update" />
</form>
</p>
<?
  if ( isset($_REQUEST['Ref']))
    echo 'Zur&uuml;ck zur <a href="?id='.$_REQUEST['Ref'].
      $Verweis.'">Anmeldung</a>';
 }
 else
 {
   echo "Falsche Charakternummer!<br />";
   $document['show_files'] = false;
   $document['comments'] = false;
 }
?>