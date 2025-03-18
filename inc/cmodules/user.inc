<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if($profile['editor'] == 1) {

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

$unique_nickname = !(UNIQUE_NICKNAMES && !is_unique_nickname($docinput['nickname'], SESSION_DBID));

if(isset($docinput['create_new_user']) && $docinput['create_new_user'] == 'true' && $docinput['email_adr'] != ''
	&& $docinput['passwd_1'] != '' && $docinput['passwd_1'] == $docinput['passwd_2'] && isemail($docinput['email_adr']) &&
	$docinput['nickname'] != '' && strlen($docinput['passwd_1']) > 5 && $unique_nickname) {
	$insert_id = add_user($docinput['email_adr'], $docinput['passwd_1']);
	if($insert_id != false) {
		set_profile('nickname', $docinput['nickname'], $insert_id);
		set_profile('website', $docinput['website'], $insert_id);
		set_profile('icq', $docinput['icq'], $insert_id);
		set_profile('interests', $docinput['interests'], $insert_id);
		set_profile('country', trim($docinput['country']), $insert_id);
		set_profile('gender', $docinput['gender'], $insert_id);
		if($_FILES['userfile']['size'] <= 250000) {
		  move_uploaded_file($_FILES['userfile']['tmp_name'], get_dir_name('users/public', $insert_id).'/picture.jpg');
		}
		if(isset ($docinput['birthday']) && $docinput['birthday'] != '') {
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
                     set_profile('birthday', strtotime($docinput['birthday']));
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
	set_profile('nickname', $docinput['nickname']);
	set_profile('website', $docinput['website']);
	set_profile('icq', $docinput['icq']);
	set_profile('interests', $docinput['interests']);
	set_profile('country', trim($docinput['country']));
	set_profile('gender', $docinput['gender']);
         if(isset ($docinput['birthday']) && $docinput['birthday'] != '') {
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
                     set_profile('birthday', strtotime($docinput['birthday']));
	}
	if($_FILES['userfile']['size'] <= 250000 &&
           isset($_FILES['userfile']['tmp_name']) && $_FILES['userfile']['tmp_name'] != '' &&
           $_FILES['userfile']['tmp_name'] != 'none' ) {
	  move_uploaded_file($_FILES['userfile']['tmp_name'], get_dir_name('users/public', SESSION_DBID).'/picture.jpg');
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
<?php if($document['ext_info'] == '1') { ?>
<?=$design['table_begin']?>
<tr bgcolor="<?=$design['table_row_dark']?>"><td>
<b><?=LANG_ADDITIONAL_INFO?></b></td></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<?=LANG_WEBSITE?><br />
<input type="text" name="docinput[website]" value="<?=htmlchars($profile['website'])?>" size="40" />
<br />
<br />
<?=LANG_ICQ_NO?><br />
<input type="text" name="docinput[icq]" value="<?=htmlchars($profile['icq'])?>" size="40" />
<br />
<br />
<?=LANG_INTERESTS?><br />
<input type="text" name="docinput[interests]" value="<?=htmlchars($profile['interests'])?>" size="40" />
<br />
<br />
<?=LANG_COUNTRY?><br />
<select name="docinput[country]">
<?php
$countries = file(INC_PATH.DATA_PATH.'/country_list_names_only.txt');
foreach ( $countries as $key => $value) { ?>
	<option value="<?=trim($value)?>"<?php if($profile['country'] == trim($value)) echo ' selected'; ?>><?=trim($value)?></option>
	<?php
	}
?>
</select>
<br />
<br />
<?=LANG_PICTURE?><br />
<input type="file" name="userfile">
<br />
<br />
<?=LANG_GENDER?><br />
<input type="radio" name="docinput[gender]" value="m"<?php if($profile['gender'] == 'm') echo ' checked'; ?> /> Male
&nbsp&nbsp;
<input type="radio" name="docinput[gender]" value="f"<?php if($profile['gender'] == 'f') echo ' checked'; ?> /> Female
<br />
<br />
<?=LANG_BIRTHDAY.' '.LANG_BIRTHDAY_FORMAT?><br />
<input type="text" name="docinput[birthday]" value="<?=date('m/d/Y',$profile['birthday'])?>" size="40" />
</td></tr>
<?=$design['table_end']?>
<br />
<br />
<?php } ?>
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
<?php if($document['ext_info'] == '1') { ?>
<?=$design['table_begin']?>
<tr bgcolor="<?=$design['table_row_dark']?>"><td>
<b><?=LANG_ADDITIONAL_INFO?></b></td></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<?=LANG_WEBSITE?><br />
<input type="text" name="docinput[website]" value="<?=$docinput['website']?>" size="40" />
<br />
<br />
<?=LANG_ICQ_NO?><br />
<input type="text" name="docinput[icq]" value="<?=$docinput['icq']?>" size="40" />
<br />
<br />
<?=LANG_INTERESTS?><br />
<input type="text" name="docinput[interests]" value="<?=$docinput['interests']?>" size="40" />
<br />
<br />
<?=LANG_COUNTRY?><br />
<select name="docinput[country]">
<option value=""></option>
<?php
$countries = file(INC_PATH.DATA_PATH.'/country_list_names_only.txt');
foreach ( $countries as $key => $value) { ?>
	<option value="<?=$value?>"<?php if($docinput['country'] == $value) echo ' selected'; ?>><?=$value?></option>
	<?php
	}
?>
</select>
<br />
<br />
<?=LANG_PICTURE?><br />
<input type="file" name="userfile">
<br />
<br />
<?=LANG_GENDER?><br />
<input type="radio" name="docinput[gender]" value="m"<?php if($docinput['gender'] == 'm') echo ' checked'; ?> /> Male
&nbsp&nbsp;
<input type="radio" name="docinput[gender]" value="f"<?php if($docinput['gender'] == 'f') echo ' checked'; ?> /> Female
<br />
<br />
<?=LANG_BIRTHDAY.' '.LANG_BIRTHDAY_FORMAT?><br />
<input type="text" name="docinput[birthday]" value="<?=$docinput['birthday']?>" size="40" />
</td></tr>
<?=$design['table_end']?>
<br />
<br />
<?php } ?>
<input type="submit" value="<?=LANG_REGISTER?>" />
</form>
</p>
<?php
}
}
}
?>