<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.06.2002
        Erweitert:
        - Läd bei Bedarf ein im Misc-liegendes Modul, dessen Name im Administrationsfeld
          eingegeben werden kann
        - Zeigt fremde Userdaten an wenn Parameter UNr in Gebrauch
*/

if($profile["editor"] == 1) {

        create_editor_input('Zusätzliches Infomodul (im misc-Ordner)', 'zus_info', 'text_input');
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

$UserNummer = SESSION_DBID;
$UserEMail = SESSION_EMAIL;
if ( isset($_REQUEST["UNr"]) && SESSION_STATUS=="ok" )
{
  $Nr = $_REQUEST["UNr"];
  if ( substr($Nr, 0, strlen(SESSION_DBID)) == SESSION_DBID )
  {
    $Nr = substr($Nr, strlen(SESSION_DBID));
    if ( substr($Nr, 0, 1) == "A" )
    {
      $Nr = substr($Nr, 1);
      if ( is_numeric($Nr) )
      {
        $profile = get_profile($Nr, false);
        $UserNummer = $Nr;
        $UserEMail = get_user_email($Nr);
      }
    }
  }
}

$unique_nickname = !(UNIQUE_NICKNAME && !is_unique_nickname($docinput['nickname'], $UserNummer));

if(isset($docinput['create_new_user']) && $docinput['create_new_user'] == 'true' && $docinput['email_adr'] != ''
        && $docinput['passwd_1'] != '' && $docinput['passwd_1'] == $docinput['passwd_2'] && isemail($docinput['email_adr']) &&
        $docinput['nickname'] != '' && strlen($docinput['passwd_1']) > 5 && $unique_nickname) {
        $insert_id = add_user($docinput['email_adr'], $docinput['passwd_1']);
        if($insert_id != false) {
                foreach ( $docinput as $key => $value )
                {
                  if ( $key != "passwd_1" && $key != "passwd_2" && $key != "email_adr"
                    && $key != "save_user" && $key != "update_user" )
                  {
                    set_profile($key, transform(trim($value),'clean'), $insert_id);
                  }
                  if ( $key == "birthday" ) {
                    if ( strpos($docinput['birthday'],".") === false )
                     {
                     }
                     else {
                       $tag = substr($docinput['birthday'], 0, strpos($docinput['birthday'], "."));
                       $monat = substr($docinput['birthday'], strpos($docinput['birthday'], ".")+1);
                       $jahr = substr($monat, strpos($monat, ".")+1);
                       $monat = substr($monat, 0, strpos($monat, "."));
                       $docinput['birthday'] = $monat."/".$tag."/".$jahr;
                     }
                     set_profile('birthday', strtotime($docinput['birthday']), $insert_id);
                    }
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
                <?php /*if ($document['zus_info'] != "" && strpos($document['zus_info'],"/") === false)
		 include(INC_PATH."misc/".$document['zus_info'].".inc"); */
?>
                <input type="hidden" name="id" value="<?=$id?>" />
                <input type="hidden" name="docinput[user_id]" value="<?=$docinput['email_adr']?>" />
                <input type="hidden" name="docinput[passwd2]" value="<?=$docinput['passwd_1']?>" />
                <input type="submit" value="Login" />
                </form>
                <?php
                set_profile("im_default", "email", $insert_id);
                set_profile("newsletter", "1", $insert_id);
                send_message($insert_id, str_replace('%%site%%', SITE_TITLE, LANG_WELCOME_MESSAGE), 1);
                mail(WEBMASTER_MAIL, "[ ".SITE_TITLE." ] Neuer Nutzer",
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
        if($temp_user_id != $UserNummer && $temp_user_id != -1) { $user_exists = true; }
        elseif(update_user($docinput['email_adr'], $docinput['passwd_1'])) {
          foreach ( $docinput as $key => $value )
          {
            if ( $key != "passwd_1" && $key != "passwd_2" && $key != "email_adr"
                  && $key != "save_user" && $key != "update_user")
              set_profile($key, transform(trim($value),'clean'), $UserNummer);
	   if ( $key == "birthday" ) {
              if ( strpos($docinput['birthday'],".") === false )
              {
              }
              else {
                $tag = substr($docinput['birthday'], 0, strpos($docinput['birthday'], "."));
                $monat = substr($docinput['birthday'], strpos($docinput['birthday'], ".")+1);
                $jahr = substr($monat, strpos($monat, ".")+1);
                $monat = substr($monat, 0, strpos($monat, "."));
                $docinput['birthday'] = $monat."/".$tag."/".$jahr;
              }
              set_profile('birthday', strtotime($docinput['birthday']), $UserNummer);
            }
          }
          //$profile = get_profile($UserNummer, true);
          if($_FILES['userfile']['size'] <= 250000 &&
           isset($_FILES['userfile']['tmp_name']) && $_FILES['userfile']['tmp_name'] != '' &&
           $_FILES['userfile']['tmp_name'] != 'none' ) {
                @unlink(get_dir_name('users/public', $UserNummer).'/picture.jpg');
                move_uploaded_file($_FILES['userfile']['tmp_name'], get_dir_name('users/public', $UserNummer).'/picture.jpg');
                chmod(get_dir_name('users/public', $UserNummer).'/picture.jpg', CONFIG_CREATE_MASK);
                }
          if($UserEMail == $docinput['email_adr'] && $docinput['passwd_1'] == '') {
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
                <? } ?>
                <input type="submit" value="Proceed" />
                </form>
                <?
            }
         } else { echo '<p><b>'.LANG_UPDATE_ERROR.'</b></p>'; }
}
else {
  $load_form = true;
  foreach ( $docinput as $key => $value )
  {
    $profile[$key] = transform(trim($value),'clean');
    if ( $key == "birthday" ) {
      if ( strpos($docinput['birthday'],".") === false )
      {
      }
      else {
        $tag = substr($docinput['birthday'], 0, strpos($docinput['birthday'], "."));
        $monat = substr($docinput['birthday'], strpos($docinput['birthday'], ".")+1);
        $jahr = substr($monat, strpos($monat, ".")+1);
        $monat = substr($monat, 0, strpos($monat, "."));
        $docinput['birthday'] = $monat."/".$tag."/".$jahr;
      }
      $profile['birthday'] = strtotime($docinput['birthday']);
    }
  }
}

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
<?php
if ( isset($_REQUEST["UNr"]) && $UserNummer != SESSION_DBID )
  echo '<input type="hidden" name="UNr" value="'.$_REQUEST["UNr"].'" />';
?>
<input type="hidden" name="docinput[update_user]" value="true" />
<?=LANG_EMAIL_ADDRESS?><br />
<input type="text" name="docinput[email_adr]" size="40" value="<?=$UserEMail?>" />
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
foreach ( $groups as $key => $value )
{
  if($profile['group_'.$key] == 1) {  echo "$value "; $drin = true; }
}
reset($profile);
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
<? } // Ext-Info = 1
if ($document['zus_info'] != "" && strpos($document['zus_info'],"/") === false)
{
 include(INC_PATH."misc/".$document['zus_info'].".inc");
}
?>
<input type="submit" value="Update" />
</form>

<?php
}
elseif(SESSION_STATUS != 'ok') {

if(isset($document['header_new'])) { echo '<p>'.transform($document['header_new'], $document['datatype']['header_new']).'</p>'; }
echo '<p>';
if(isset($docinput['create_new_user'])) {
  if($docinput['passwd_1'] != $docinput['passwd_2']) {
        echo '<b>&gt;&gt;</b> <font color="#ff0000">'.LANG_PASSWD_MATCHUP.'</font><br />';
        }
  if(strlen($docinput['passwd_1']) < 6) {
        echo '<b>&gt;&gt;</b> <font color="#ff0000">'.LANG_PASSWD_TOO_SHORT.'</font><br />';
        }
  if(!isset($docinput['email_adr']) || $docinput['email_adr'] == '' || !isemail($docinput['email_adr'])) {
        echo '<b>&gt;&gt;</b> <font color="#ff0000">'.LANG_WRONG_EMAIL.'</font><br />';
        }
  if(isset($user_exists) && $user_exists == true) {
        echo '<b>&gt;&gt;</b> <font color="#ff0000">'.LANG_EMAIL_IN_USE.'</font><br />';
        }
  if(!isset($docinput['nickname']) || $docinput['nickname'] == '') {
        echo '<b>&gt;&gt;</b> <font color="#ff0000">'.LANG_NO_NICKNAME.'</font><br />';
        }
  if(!$unique_nickname) {
        echo '<b>&gt;&gt;</b> <font color="#ff0000">'.LANG_UNIQUE_NICKNAME_REQ.'</font><br />';
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
<? /* if ($document['zus_info'] != "" && strpos($document['zus_info'],"/")=== false)
  include(INC_PATH."misc/".$document['zus_info'].".inc"); */
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
<? } ?>
<input type="submit" value="<?=LANG_REGISTER?>" />
</form>
</p>
<?php
  } // Session nicht ok
 }
} // else
?>
