<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.06.2002
*/

load_lang('steinsberg');

if(SESSION_STATUS == 'ok' && ! $profile['editor'] == "1" ) {

$SpielerNr = SESSION_DBID;
$Verweis = "";
if ( isset ($_REQUEST["Spieler"]) && is_numeric($_REQUEST["Spieler"]) && $profile['SteinsbergSL'] == "1" )
{
  $SpielerNr = $_REQUEST["Spieler"];
  $Verweis = "&Spieler=$SpielerNr";
}
// Abspeichern der Daten
  if(isset($_REQUEST['savechar']) ) {
        while (list($key, $value) = each($docinput) )
        {
          set_profile($key, transform(trim($value),'text'), $SpielerNr);
        }
        if ( $_FILES['nscfile']['size'] <= 150000) {
          if ( file_exists(get_dir_name('users/public', $SpielerNr).'/nscpicture.jpg') && $_FILES['nscfile']['size'] > 0 )
            unlink (get_dir_name('users/public', $SpielerNr).'/nscpicture.jpg');
          move_uploaded_file($_FILES['nscfile']['tmp_name'], get_dir_name('users/public', $SpielerNr).'/nscpicture.jpg');
          chmod (get_dir_name('users/public', $SpielerNr).'/nscpicture.jpg', CONFIG_CREATE_MASK);
        }

  }
$daten = get_profile($SpielerNr, false);
?>
<p>Um dich als NSC m&ouml;glichst deinen F&auml;higkeiten und Erfahrungen gem&auml;&szlig; einsetzen zu k&ouml;nnen,
f&uuml;lle bitte folgende Charakterisierung aus.<br />
<?
//  if ( isset($_REQUEST['Ref']))
//    echo 'Zur&uuml;ck zur <a href="?id='.$document['parent_id'].'&Ref='.$_REQUEST['Ref'].
//      $Verweis.'">Anmeldung</a>';
?>
<br />
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
<?=$design['table_begin'];?>
<tr bgcolor="<?=$design['table_row_dark']?>"><th>NSC-Charakterisierung</th></tr>
<tr><td>
<input type="hidden" name="id" value="<?=$id?>" />
<? if ( isset($_REQUEST['Ref'])) ?>
<input type="hidden" name="Ref" value="<?=$_REQUEST['Ref']?>" />
Ich bin eher
<input type="radio" name="docinput[Aktiv]" value="tags"<? if($daten['Aktiv'] == 'tags') echo ' checked'; ?> /> tags
&nbsp&nbsp;
<input type="radio" name="docinput[Aktiv]" value="nachts"<? if($daten['Aktiv'] == 'nachts') echo ' checked'; ?> /> nachts
&nbsp&nbsp;
<input type="radio" name="docinput[Aktiv]" value="immer"<? if($daten['Aktiv'] == 'immer') echo ' checked'; ?> /> immer einsetzbar
<br />
Ich k&auml;mpfe
<input type="radio" name="docinput[Kampf]" value="gern"<? if($daten['Kampf'] == 'gern') echo ' checked'; ?> /> gern
&nbsp&nbsp;
<input type="radio" name="docinput[Kampf]" value="nicht"<? if($daten['Kampf'] == 'nicht') echo ' checked'; ?> /> nicht gern
&nbsp&nbsp;
<input type="radio" name="docinput[Kampf]" value="egal"<? if($daten['Kampf'] == 'egal') echo ' checked'; ?> /> ist mir egal<br />
Ich kann problemlos Masken tragen
<input type="radio" name="docinput[Maske]" value="ja"<? if($daten['Maske'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[Maske]" value="nein"<? if($daten['Maske'] == 'nein') echo ' checked'; ?> /> nein<br />
Ich lasse mich schminken
<input type="radio" name="docinput[Schminken]" value="ja"<? if($daten['Schminken'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[Schminken]" value="nein"<? if($daten['Schminken'] == 'nein') echo ' checked'; ?> /> nein<br />
<br />
Ich habe Probleme mit <br />
<textarea name="docinput[Probleme]" cols="60" rows="5"><?=transform($daten['Probleme'],"clean")?>
</textarea><br />
<br />
Ich m&ouml;chte als Springer eingesetzt werden
<input type="radio" name="docinput[Springer]" value="ja"<? if($daten['Springer'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[Springer]" value="nein"<? if($daten['Springer'] == 'nein') echo ' checked'; ?> /> nein<br />
Als Ambienterolle zwischen den Einsetzen/als Festrolle w&uuml;rde ich gern folgendes spielen<br />
<input type="text" name="docinput[Ambienterolle]" size="40" value="<?=transform($daten['Ambienterolle'],"clean")?>" /><br />
(Die Gewandung daf&uuml;r
<input type="radio" name="docinput[Gewandung]" value="ja"<? if($daten['Gewandung'] == 'ja') echo ' checked'; ?> /> habe ich selbst
&nbsp&nbsp;
<input type="radio" name="docinput[Gewandung]" value="nein"<? if($daten['Gewandung'] == 'nein') echo ' checked'; ?> /> br&auml;uchte ich)<br />
<br />
Ich habe bisher folgende Rollen gespielt:<br >
Magier <input type="radio" name="docinput[RolleMagier]" value="ja"<? if($daten['RolleMagier'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[RolleMagier]" value="nein"<? if($daten['RolleMagier'] == 'nein') echo ' checked'; ?> /> nein<br />
Priester <input type="radio" name="docinput[RollePriester]" value="ja"<? if($daten['RollePriester'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[RollePriester]" value="nein"<? if($daten['RollePriester'] == 'nein') echo ' checked'; ?> /> nein<br />
Heiler <input type="radio" name="docinput[RolleHeiler]" value="ja"<? if($daten['RolleHeiler'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[RolleHeiler]" value="nein"<? if( $daten['RolleHeiler'] == 'nein') echo ' checked'; ?> /> nein<br />
K&auml;mpfer <input type="radio" name="docinput[RolleKaempfer]" value="ja"<? if( $daten['RolleKaempfer'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[RolleKaempfer]" value="nein"<? if( $daten['RolleKaempfer'] == 'nein') echo ' checked'; ?> /> nein<br />
Geschichtenerz&auml;hler <input type="radio" name="docinput[RolleErzaehler]" value="ja"<? if( $daten['RolleErzaehler'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[RolleErzaehler]" value="nein"<? if( $daten['RolleErzaehler'] == 'nein') echo ' checked'; ?> /> nein<br />
Barde <input type="radio" name="docinput[RolleBarde]" value="ja"<? if( $daten['RolleBarde'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[RolleBarde]" value="nein"<? if( $daten['RolleBarde'] == 'nein') echo ' checked'; ?> /> nein<br />
Adel <input type="radio" name="docinput[RolleAdel]" value="ja"<? if( $daten['RolleAdel'] == 'ja') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[RolleAdel]" value="nein"<? if( $daten['RolleAdel'] == 'nein') echo ' checked'; ?> /> nein<br />
Sonstiges <br />
<input type="text" name="docinput[RolleSonstiges]" size="40" value="<?=transform($daten['RolleSonstiges'],"clean")?>" /><br />
<br />
<br />

Ich sehe etwa so aus (Kurzbeschreibung nicht mehr als 5 Zeilen)<br />
<textarea name="docinput[Personenbeschreibung]" cols="60" rows="5"><?=transform($daten['Personenbeschreibung'],"clean")?>
</textarea>
<br />
<br />
<?=LANG_PICTURE?> (max. 150 kB, muss .jpg-Datei sein)<br />
<input type="file" name="nscfile">
<?php
  if ( @file_exists(get_dir_name('users/public', $SpielerNr).'/nscpicture.jpg') ) {
    echo '&nbsp;&nbsp;&nbsp;<a href="'.get_dir_name('users/public', $SpielerNr).
      '/nscpicture.jpg">vorhandenes Bild allein</a><br />';
    echo '<img src="'.get_dir_name('users/public', $SpielerNr).
      '/nscpicture.jpg">';
    }
  ?>
<br />
<br />

Sonstige W&uuml;nsche<br />
<textarea name="docinput[Wuensche]" cols="60" rows="5"><?=transform($docinput['Wuensche'],"clean")?>
</textarea>
</td></tr>
<?=$design['table_end']?>
<br />
<br />
<input type="submit" name="savechar" value="Update" />
</form>
</p>
<?
//  if ( isset($_REQUEST['Ref']))
//    echo 'Zur&uuml;ck zur <a href="?id='.$document['parent_id'].'&Ref='.$_REQUEST['Ref'].
//      $Verweis.'">Anmeldung</a>';
 }
?>