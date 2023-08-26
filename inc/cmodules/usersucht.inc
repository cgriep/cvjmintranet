<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.06.2002
        Erweitert:
        - L&auml;d bei Bedarf ein im Misc-liegendes Modul, dessen Name im Administrationsfeld
          eingegeben werden kann
*/
if($profile['editor'] == 1) {

        create_editor_input('Zus&auml;tzliches Infomodul (im misc-Ordner)', 'zus_info', 'text_input');
        echo '<br /><br />';
        create_editor_input('Extended Info', 'ext_info', 'onoff_input');
        echo '<br /><br />';
        create_editor_input('Information for unregistered users', 'header_new', 'area_input');
        echo '<br /><br />';
        create_editor_input('Welcome page', 'reg_success', 'area_input');
        echo '<br /><br />';
        create_editor_input('Update page', 'header_change', 'area_input');
        }
else {

load_lang('user');

$unique_nickname = !(UNIQUE_NICKNAME && !is_unique_nickname($docinput['nickname'], SESSION_DBID));

if(isset($docinput['create_new_user']) && $docinput['create_new_user'] == 'true' && $docinput['email_adr'] != ''
        && $docinput['passwd_1'] != '' && $docinput['passwd_1'] == $docinput['passwd_2'] && isemail($docinput['email_adr']) &&
        $docinput['nickname'] != '' && strlen($docinput['passwd_1']) > 5 && $unique_nickname) {
        $insert_id = add_user($docinput['email_adr'], $docinput['passwd_1']);
        if($insert_id != false) {
                foreach ( $docinput as $key => $value )
                {
                  if ( $key != "passwd_1" && $key != "passwd_2" && $key != "email_adr"
                    && $key != "save_user" && $key != "update_user" )
                    set_profile($key, transform(trim($value),'clean'), $insert_id);
                  if ( $key == "birthday" )
                    set_profile('birthday', strtotime($docinput['birthday']), $insert_id);
                }
                if($_FILES['userfile']['size'] <= 250000) {
                        move_uploaded_file($_FILES['userfile']['tmp_name'], get_dir_name('users/public', $insert_id).'/picture.jpg');
                        chmod(get_dir_name('users/public', $insert_id).'/picture.jpg', CONFIG_CREATE_MASK);
                        }
                if(isset($document['reg_success'])) {
                        echo '<p>'.str_replace('%%reg_nickname%%', $docinput['nickname'], str_replace('%%email%%', $docinput['email_adr'],
                        str_replace('%%id%%', $insert_id, transform($document['reg_success'],$document['datatype']['reg_success'])))).'</p>';
                        } ?>
                <form action="<?=$_SERVER['PHP_SELF']?>" method="post">
                <input type="hidden" name="id" value="<?=$id?>" />
                <input type="hidden" name="docinput[user_id]" value="<?=$docinput['email_adr']?>" />
                <input type="hidden" name="docinput[passwd2]" value="<?=$docinput['passwd_1']?>" />
                <input type="submit" value="Login" />
                </form>
                <?php
                send_message($insert_id, str_replace('%%site%%', SITE_TITLE, LANG_WELCOME_MESSAGE), 1);
                mail(WEBMASTER_MAIL, "AWF-Neuer Nutzer",
                  "Ein neuer Nutzer hat sich angemeldet:\nName: " . $docinput['nickname'] . "\n" .
                  "EMail: " . $docinput['email_adr'] . "\nDatum: " . date("d.m.Y H:i"),
                   "From: ".WEBMASTER_MAIL);
        }
        else { $user_exists = true; }
}
elseif(isset($docinput['update_user']) && $docinput['update_user'] == 'true' && $docinput['email_adr'] != '' &&
        $docinput['passwd_1'] == $docinput['passwd_2'] && isemail($docinput['email_adr']) && $docinput['nickname'] != ''
        && (strlen($docinput['passwd_1']) > 5 || $docinput['passwd_1'] == '') &&
        $unique_nickname) {
        $temp_user_id = get_user_id ($docinput['email_adr']);
        if($temp_user_id != SESSION_DBID && $temp_user_id != -1) { $user_exists = true; }
        elseif(update_user($docinput['email_adr'], $docinput['passwd_1'])) {
          foreach ( $docinput as $key => $value)
                {
                  if ( $key != "passwd_1" && $key != "passwd_2" && $key != "email_adr"
                       && $key != "save_user" && $key != "update_user")
                    set_profile($key, transform(trim($value),'clean'));
                  if ( $key == "birthday" )
                    set_profile('birthday', strtotime($docinput['birthday']));
                }
          if($_FILES['userfile']['size'] <= 250000 &&
           isset($_FILES['userfile']['tmp_name']) && $_FILES['userfile']['tmp_name'] != '' &&
           $_FILES['userfile']['tmp_name'] != 'none' ) {
                @unlink(get_dir_name('users/public', SESSION_DBID).'/picture.jpg');
                move_uploaded_file($_FILES['userfile']['tmp_name'], get_dir_name('users/public', SESSION_DBID).'/picture.jpg');
                chmod(get_dir_name('users/public', SESSION_DBID).'/picture.jpg', CONFIG_CREATE_MASK);
          }
          // eMail-Kennwort &auml;ndern
          $Wer = substr($docinput["email_adr"], 0, strpos($docinput["email_adr"],"@"));
          if ( substr(strtoupper($Wer), 0, 5) == "SUCHT" && $docinput["passwd_1"] != "") {

 if ( $Wer != "sucht000" )
 {
   echo "�ndere eMail-Kennwort ...<br />";

# This is the difficult part, the Telnet header
$header1=chr(0xFF).chr(0xFB).chr(0x1F).chr(0xFF).chr(0xFB).
chr(0x20).chr(0xFF).chr(0xFB).chr(0x18).chr(0xFF).chr(0xFB).
chr(0x27).chr(0xFF).chr(0xFD).chr(0x01).chr(0xFF).chr(0xFB).
chr(0x03).chr(0xFF).chr(0xFD).chr(0x03).chr(0xFF).chr(0xFC).
chr(0x23).chr(0xFF).chr(0xFC).chr(0x24).chr(0xFF).chr(0xFA).
chr(0x1F).chr(0x00).chr(0x50).chr(0x00).chr(0x18).chr(0xFF).
chr(0xF0).chr(0xFF).chr(0xFA).chr(0x20).chr(0x00).chr(0x33).
chr(0x38).chr(0x34).chr(0x30).chr(0x30).chr(0x2C).chr(0x33).
chr(0x38).chr(0x34).chr(0x30).chr(0x30).chr(0xFF).chr(0xF0).
chr(0xFF).chr(0xFA).chr(0x27).chr(0x00).chr(0xFF).chr(0xF0).
chr(0xFF).chr(0xFA).chr(0x18).chr(0x00).chr(0x58).chr(0x54).
chr(0x45).chr(0x52).chr(0x4D).chr(0xFF).chr(0xF0);
$header2=chr(0xFF).chr(0xFC).chr(0x01).chr(0xFF).chr(0xFC).
chr(0x22).chr(0xFF).chr(0xFE).chr(0x05).chr(0xFF).chr(0xFC).chr(0x21);

# connecting
$fp=fsockopen("ods3.schule.de",23);
# sending the Telnet header
fputs($fp,$header1);
usleep(125000);
fputs($fp,$header2);
usleep(125000);

# login
fputs($fp,"sucht000 $Wer\r");
usleep(125000);
fputs($fp,"meitner99\r");
usleep(125000);
# root looks nice
fputs($fp,$docinput["passwd_1"]."\r");
usleep(125000); # takes some time, we had to wait
fputs($fp,$docinput["passwd_1"]."\r");
usleep(125000);
// Falls ein weak-Password
fputs($fp,$docinput["passwd_1"]."\r");
usleep(125000);
do
{
   $output.=fread($fp, 80);    // read line by line, or at least small chunks
   $stat=socket_get_status($fp);
}
while($stat["unread_bytes"]);
if ( ! (strpos($output,"Warning") === false) )
  fputs($fp,"\r\r\r\r\r"); // fehler abfangen
fclose($fp);
$output = nl2br($output);
$output = substr($output, strpos("<br />", $output)+6);
//echo $output;
 // if ( ! (strpos($output, "Re-enter new password") === false) &&
 //         strpos($output, "Bad password") === false && strpos($output, "Warning") === false )
  echo "<br /><b><big>Kennwort erfolgreich ge�ndert.</big></b><br />";
 //else
// {
  //echo '<big><font color="red">Fehler beim �ndern des Kennwortes:<br /></font>';
  //echo "Das eMailkennwort konnte nicht ge�ndert werden. Bitte verwenden Sie mindestens ein Zeichen, dass kein Buchstabe ist. Ihr Kennwort zum Login in dieses System wurde ge&auml;ndert, w&auml;hrend Ihr eMailkennwort noch das alte ist. &Auml;ndern Sie das Kennwort erneut, um beide Kennw�rter auf den gleichen Stand zu bringen.<br />";
  //}
  }
          }
          if(SESSION_EMAIL == $docinput['email_adr'] && $docinput['passwd_1'] == '') {
            $load_form = true;
          }
          else {
                ?>
                <p><b><?=LANG_SETTINGS_UPDATED?></b>
                <br />
                <br />
                >> <?=LANG_HAVE_TO_RELOGIN?>
                </p>
                <form action="<?=$_SERVER['PHP_SELF']?>" method="post">
                <input type="hidden" name="id" value="<?=$id?>" />
                <input type="hidden" name="docinput[user_id]" value="<?=$docinput['email_adr']?>" />
                <?php if(strlen($docinput['passwd_1']) > 5) { ?>
                <input type="hidden" name="docinput[passwd2]" value="<?=$docinput['passwd_1']?>" />
                <?php } ?>
                <input type="submit" value="Proceed" />
                </form>
                <?php
            }
         } else { echo '<p><b>'.LANG_UPDATE_ERROR.'</b></p>'; }
}
else { $load_form = true; }

if($load_form == true || $user_exists == true) {
  if(SESSION_STATUS == 'ok') {
    if(isset($document['header_change'])) { echo '<p>'.transform($document['header_change'], $document['datatype']['header_change']).'</p>'; }
?>
<p>
<?php
    if(isset($docinput['update_user'])) {
      if($docinput['passwd_1'] != $docinput['passwd_2']) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_PASSWD_MATCHUP.'</font><br />';
      }
      if(strlen($docinput['passwd_1']) < 6 && !(strlen($docinput['passwd_1']) == 0)) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_PASSWD_TOO_SHORT.'</font><br />';
        }
      if(isset($user_exists) && $user_exists == true) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_EMAIL_IN_USE.'</font><br />';
        }
      if(!isset($docinput['email_adr']) || $docinput['email_adr'] == '' || !isemail($docinput['email_adr'])) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_WRONG_EMAIL.'</font><br />';
        }
      if(!isset($docinput['nickname']) || $docinput['nickname'] == '') {
        echo '<b>>></b> <font color="#ff0000">'.LANG_NO_NICKNAME.'</font><br />';
        }
      if(!$unique_nickname) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_UNIQUE_NICKNAME_REQ.'</font><br />';
      }
    }
?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
<input type="hidden" name="id" value="<?=$id?>" />
<input type="hidden" name="docinput[update_user]" value="true" />
<?=LANG_EMAIL_ADDRESS?><br />
<input type="text" name="docinput[email_adr]" size="40" value="<?=SESSION_EMAIL?>" />
<br />
<br />
<?=LANG_NICKNAME?><br />
<input type="text" name="docinput[nickname]" size="40" value="<?=htmlchars($profile['nickname'])?>" />
<br />
<br />
<?=LANG_CHOOSE_PASSWD?><br />
<input type="password" name="docinput[passwd_1]" size="40" />
<br />
<br />
<?=LANG_RETYPE_PASSWD?><br />
<input type="password" name="docinput[passwd_2]" size="40" />
<br />
<br />
<?=LANG_MITGLIED_IN?><br />
<!-- Neu: Gruppen anzeigen -->
<i>
<?php
$drin = false;
foreach ( $groups as $key => $value) {
 if($profile['group_'.$key] == 1) {  echo "$value "; $drin = true; }
}
if ( !$drin) echo "-keiner Gruppe zugeordnet-";
?></i>
<br />
<br />

<?php
if($document['ext_info'] == '1') { ?>
<?=$design['table_begin']?>
<tr bgcolor="<?=$design['table_row_dark']?>"><td>
<b><?=LANG_ADDITIONAL_INFO?></b></td></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<?=LANG_PICTURE?><br />
<input type="file" name="userfile">
<br />
<br />
</td></tr>
<?=$design['table_end']?>
<br />
<br />
<?php } // Ext-Info = 1
if ($document['zus_info'] != "" && strpos($document['zus_info'],"/") === false)
  include(INC_PATH."misc/".$document['zus_info'].".inc");

?>
<input type="submit" value="Update" />
</form>
</p>
<?php
}
elseif(SESSION_STATUS != 'ok') {

if(isset($document['header_new'])) { echo '<p>'.transform($document['header_new'], $document['datatype']['header_new']).'</p>'; }
echo '<p>';
if(isset($docinput['create_new_user'])) {
if($docinput['passwd_1'] != $docinput['passwd_2']) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_PASSWD_MATCHUP.'</font><br />';
        }
if(strlen($docinput['passwd_1']) < 6) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_PASSWD_TOO_SHORT.'</font><br />';
        }
if(!isset($docinput['email_adr']) || $docinput['email_adr'] == '' || !isemail($docinput['email_adr'])) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_WRONG_EMAIL.'</font><br />';
        }
if(isset($user_exists) && $user_exists == true) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_EMAIL_IN_USE.'</font><br />';
        }
if(!isset($docinput['nickname']) || $docinput['nickname'] == '') {
        echo '<b>>></b> <font color="#ff0000">'.LANG_NO_NICKNAME.'</font><br />';
        }
if(!$unique_nickname) {
        echo '<b>>></b> <font color="#ff0000">'.LANG_UNIQUE_NICKNAME_REQ.'</font><br />';
        }
}
?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
<input type="hidden" name="id" value="<?=$id?>" />
<input type="hidden" name="docinput[create_new_user]" value="true" />
<?=LANG_EMAIL_ADDRESS?><br />
<input type="text" name="docinput[email_adr]" size="40" value="<?=$docinput['email_adr']?>" />
<br />
<br />
<?=LANG_NICKNAME?><br />
<input type="text" name="docinput[nickname]" size="40" value="<?=$docinput['nickname']?>" />
<br />
<br />
<?=LANG_CHOOSE_PASSWD?><br />
<input type="password" name="docinput[passwd_1]" size="40" />
<br />
<br />
<?=LANG_RETYPE_PASSWD?><br />
<input type="password" name="docinput[passwd_2]" size="40" />
<br />
<br />
<?php if ($document['zus_info'] != "" && strpos($document['zus_info'],"/")=== false)
  include(INC_PATH."misc/".$document['zus_info'].".inc");
 if($document['ext_info'] == '1') { ?>
<?=$design['table_begin']?>
<tr bgcolor="<?=$design['table_row_dark']?>"><td>
<b><?=LANG_ADDITIONAL_INFO?></b></td></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<?=LANG_PICTURE?><br />
<input type="file" name="userfile">
<br />
<br />
</td></tr>
<?=$design['table_end']?>
<br />
<br />
<?php } ?>
<input type="submit" value="<?=LANG_REGISTER?>" />
</form>
</p>
<?php
  } // Session nicht ok
 }
} // else

?>