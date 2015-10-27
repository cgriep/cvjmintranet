<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 06.04.2004
*/

// error_reporting(1); // Disable Warnings
define('FLAVOUR_CHARSET', 'iso-8859-1');

$rows_per_page = 50;

$_BASEPATH = strrev(strstr(substr(strstr(strrev($_SERVER['PATH_TRANSLATED']),'/'), 1),'/'));

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');
include($_BASEPATH.'/inc/functions.inc');
include($_BASEPATH.'/inc/sql_functions.inc');

// Load Liquid Bytes Frontent Toolkit
include('lb_frontend_toolkit.inc');

// Import Arguments
lftk_import_argument('page', 'login');
lftk_import_argument('target_id', FALSE);
lftk_import_argument('first_item', 0);
lftk_import_argument('sort_order', 'name');

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
       	while ($row = sql_fetch_row($qresult)) {
               	define(strtoupper($row[0]),$row[1]);
                }
        sql_free_result($qresult);
        }

$lang 	= DEFAULT_LANG;
load_lang('admin');

$row = sql_get_single_row(TABLE_USERS, 'id = '.$target_id);

?>

<html>
<head>
<title>
<?=$row['username']?>
</title>
<link rel="stylesheet" type="text/css" href="admin.css" />
</head>
<body topmargin="0" rightmargin="0" leftmargin="0" marginwidth="0" marginheight="0" bgcolor="#FFFFFF">
<table border="0" cellpadding="5" cellspacing="0" bgcolor="#ffffff" width="100%">
<tr>
<td valign="middle">
<?=get_image_tag('img/user.gif','','',0,0,'middle')?>
</td>
<td valign="middle" width="100%">
<span style="font-weight: bold; font-size: 16px;"><?=$row['username']?></span>
</td>
</tr>
</table>
<center>
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr><td align="left">
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
<input type="hidden" name="page" value="<?=$page?>" />
<input type="hidden" name="target_id" value="<?=$target_id?>" />
<table width="100%" cellpadding="5" cellspacing="0" border="0"><tr>
<?
lftk_add_tab('login', LANG_ADMIN_LOGIN);
lftk_add_tab('contact', LANG_ADMIN_CONTACT);
lftk_add_tab('details', LANG_ADMIN_DETAILS);
lftk_add_tab('picture', LANG_ADMIN_PICTURE);
lftk_add_tab('groups', LANG_ADMIN_GROUPS);
lftk_add_tab('profile', LANG_ADMIN_PROFILE);
?>
<td bgcolor="#ffffff" width="100%"><img src="img/pixel.gif" width="1" /></td>
</tr>
</table>
<table bgcolor="<?=$lftk_config['color_3']?>" width="100%" cellpadding="9" cellspacing="0" border="0">
<tr><td>
<?
/**********************************************************************************************************
Login
**********************************************************************************************************/

if($page == 'login') {

if(isset($_REQUEST['docinput']) && is_numeric($target_id)) {
	$docinput = $_REQUEST['docinput'];

	// check if email and username are unique
	if(awf_login_is_unique($docinput['username'], $target_id) && awf_login_is_unique($docinput['email'], $target_id)) {
		// if true, save to database
		sql_query("UPDATE ".TABLE_USERS." SET email = '".addslashes($docinput['email'])."', ".
				"username = '".addslashes($docinput['username'])."' WHERE id = $target_id");
		}

	$row['email']    = $docinput['email'];
	$row['username'] = $docinput['username'];

	// Save password
	sql_query("UPDATE ".TABLE_USERS." SET password = '".addslashes($docinput['password'])."' WHERE id = $target_id");
	$row['password']    = $docinput['password'];

	// check registered and expires date - if year is empty, set to 0
	if($docinput['registered_year'] == '') {
		$_registered = 0;
		}
	else {
		$_registered = mktime(12,0,0,$docinput['registered_month'],$docinput['registered_day'],$docinput['registered_year']);
		}

	if($docinput['expires_year'] == '') {
		$_expires = 0;
		}
	else {
		$_expires = mktime(12,0,0,$docinput['expires_month'],$docinput['expires_day'],$docinput['expires_year']);
		}

	sql_query("UPDATE ".TABLE_USERS." SET registered = $_registered, expires = $_expires WHERE id = $target_id");	

	$row['registered']    = $_registered;
	$row['expires']       = $_expires;

	// check if page views are numeric and >= 0
	if(is_numeric($docinput['views']) && $docinput['views'] >= 0) {
		// if true, save to database
		sql_query("UPDATE ".TABLE_USERS." SET views = ".$docinput['views']." WHERE id = $target_id");
	
		$row['views']    = $docinput['views'];

		}

	}
?>
<?=LANG_ADMIN_USERNAME?>
<br />
<?
if(!awf_login_is_unique($row['username'], $target_id)) {
	lftk_display_error (LANG_ADMIN_USERNAME_EXISTS);
	}
?>
<input type="text" size="60" name="docinput[username]" value="<?=htmlentities($row['username'])?>" />
<br />
<br />
<?=LANG_ADMIN_EMAIL?>
<br />
<?
if(!awf_login_is_unique($row['email'], $target_id)) {
	lftk_display_error (LANG_ADMIN_EMAIL_EXISTS);
	}
?>
<input type="text" size="60" name="docinput[email]" value="<?=htmlentities($row['email'])?>" />
<br />
<br />
<?=LANG_ADMIN_PASSWORD?>
<br />
<?
if(strlen($row['password']) < 6) {
	lftk_display_warning (LANG_ADMIN_PASSWORD_SHORT);
	}
?>
<input type="password" size="60" name="docinput[password]" value="<?=htmlentities($row['password'])?>" />
<br />
<br />
<?=LANG_ADMIN_REGISTERED?>
<br />
<?=lftk_get_number_dropdown ('docinput[registered_day]', 1, 31, date('d',$row['registered'])).LANG_ADMIN_DATE_SEPARATOR?>
<?=lftk_get_number_dropdown ('docinput[registered_month]', 1, 12, date('m',$row['registered'])).LANG_ADMIN_DATE_SEPARATOR?>
<?=lftk_get_number_dropdown ('docinput[registered_year]', 1980, 2030, date('Y',$row['registered']), TRUE)?>
<br />
<br />
<?=LANG_ADMIN_EXPIRES?>
<br />
<?=lftk_get_number_dropdown ('docinput[expires_day]', 1, 31, date('d',$row['expires'])).LANG_ADMIN_DATE_SEPARATOR?>
<?=lftk_get_number_dropdown ('docinput[expires_month]', 1, 12, date('m',$row['expires'])).LANG_ADMIN_DATE_SEPARATOR?>
<?=lftk_get_number_dropdown ('docinput[expires_year]', 1980, 2030, date('Y',$row['expires']), TRUE)?>
<br />
<br />
<?=LANG_ADMIN_PAGEVIEWS?>
<br />
<input type="text" size="60" name="docinput[views]" value="<?=htmlentities($row['views'])?>" />
<br />
<br />
<p align="right">
<input type="reset" value="<?=LANG_ADMIN_RESET?>" />
<input type="reset" onclick="window.close();" value="<?=LANG_ADMIN_CLOSE?>" />
<input type="submit" value="<?=LANG_ADMIN_SAVE?>" />
</p>
<?
}

/**********************************************************************************************************
Contact
**********************************************************************************************************/

elseif($page == 'contact') {

if(isset($_REQUEST['docinput']) && is_numeric($target_id)) {
	$docinput = $_REQUEST['docinput'];
	
	while(list($key, $value) = each($docinput)) {
		if($value == '') {
			remove_profile($key, $target_id);
			}
		else {
			set_profile ($key, $value, $target_id);
			}
		}
	}

$profile = get_profile ($target_id, FALSE);

?>
<?=LANG_ADMIN_FIRSTNAME?>, <?=LANG_ADMIN_LASTNAME?>
<br />
<input type="text" size="27" name="docinput[firstname]" value="<?=htmlentities($profile['firstname'])?>" />
<input type="text" size="27" name="docinput[lastname]" value="<?=htmlentities($profile['lastname'])?>" />
<br />
<br />
<?=LANG_ADMIN_COMPANY?>
<br />
<input type="text" size="60" name="docinput[company]" value="<?=htmlentities($profile['company'])?>" />
<br />
<br />
<?=LANG_ADMIN_ADDRESS?>
<br />
<input type="text" size="60" name="docinput[address_1]" value="<?=htmlentities($profile['address_1'])?>" />
<br />
<input type="text" size="60" name="docinput[address_2]" value="<?=htmlentities($profile['address_2'])?>" />
<br />
<br />
<?=LANG_ADMIN_COUNTRY?>
<br />
<?
echo '<select name="docinput[country]">'."\n";
$countries = unserialize(join('', file($_BASEPATH.INC_PATH.DATA_PATH.'/countries.ser')));
while(list($ckey, $cvalue) = each($countries)) {
	?>      <option value="<?=$ckey?>"<? if($profile['country'] == $ckey) echo ' selected="true"'; ?>><?=$cvalue?></option><?
	echo "\n";
	}
echo '</select>';
?>
<br />
<br />
<?=LANG_ADMIN_PHONE?>
<br />
<input type="text" size="60" name="docinput[phone]" value="<?=htmlentities($profile['phone'])?>" />
<br />
<br />
<?=LANG_ADMIN_FAX?>
<br />
<input type="text" size="60" name="docinput[fax]" value="<?=htmlentities($profile['fax'])?>" />
<br />
<br />
<?=LANG_ADMIN_MOBILE?>
<br />
<input type="text" size="60" name="docinput[mobile]" value="<?=htmlentities($profile['mobile'])?>" />
<br />
<br />
<?=LANG_ADMIN_ICQ?>
<br />
<input type="text" size="60" name="docinput[icq]" value="<?=htmlentities($profile['icq'])?>" />
<br />
<br />
<p align="right">
<input type="reset" value="<?=LANG_ADMIN_RESET?>" />
<input type="reset" onclick="window.close();" value="<?=LANG_ADMIN_CLOSE?>" />
<input type="submit" value="<?=LANG_ADMIN_SAVE?>" />
</p>
<?
}

/**********************************************************************************************************
Details
**********************************************************************************************************/

elseif($page == 'details') {

if(isset($_REQUEST['docinput']) && is_numeric($target_id)) {
	$docinput = $_REQUEST['docinput'];
	
	while(list($key, $value) = each($docinput)) {
		if($value == '') {
			remove_profile($key, $target_id);
			}
		else {
			set_profile ($key, $value, $target_id);
			}
		}
	}

$profile = get_profile ($target_id, FALSE);

?>
<?=LANG_ADMIN_NOTES?>
<br />
<textarea cols="60" rows="7" name="docinput[notes]"><?=htmlentities($profile['notes'])?></textarea>
<br />
<br />
<?=LANG_ADMIN_BIRTHDAY?>
<br />
<?=lftk_get_number_dropdown ('docinput[birthday_day]', 1, 31, $profile['birthday_day'],TRUE).LANG_ADMIN_DATE_SEPARATOR?>
<?=lftk_get_number_dropdown ('docinput[birthday_month]', 1, 12, $profile['birthday_month'],TRUE).LANG_ADMIN_DATE_SEPARATOR?>
<?=lftk_get_number_dropdown ('docinput[birthday_year]', 1900, date('Y'), $profile['birthday_year'], TRUE)?>
<br />
<br />
<?=LANG_ADMIN_GENDER?>
<br />
<select name="docinput[gender]">
<option value=""></option>
<option <? if($profile['gender'] == 'm') echo 'selected="true" '; ?>value="m"><?=LANG_ADMIN_MALE?></option>
<option <? if($profile['gender'] == 'f') echo 'selected="true" '; ?>value="f"><?=LANG_ADMIN_FEMALE?></option>
</select>
<br />
<br />
<?=LANG_ADMIN_HOMEPAGE?>
<br />
<input type="text" size="60" name="docinput[homepage]" value="<?=htmlentities($profile['homepage'])?>" />
<br />
<br />
<p align="right">
<input type="reset" value="<?=LANG_ADMIN_RESET?>" />
<input type="reset" onclick="window.close();" value="<?=LANG_ADMIN_CLOSE?>" />
<input type="submit" value="<?=LANG_ADMIN_SAVE?>" />
</p>
<?
}

/**********************************************************************************************************
Picture
**********************************************************************************************************/

elseif($page == 'picture') {

if(isset($_REQUEST['delete_image']) && file_exists($_BASEPATH.'img/users/'.$target_id.'.jpg')) {
	unlink($_BASEPATH.'img/users/'.$target_id.'.jpg');
	}
elseif($_FILES['userfile']['size'] <= 250000) {
	move_uploaded_file($_FILES['userfile']['tmp_name'], $_BASEPATH.'img/users/'.$target_id.'.jpg');
	}

?>
<table width="100%" border="0" cellspacing="0" cellpadding="3">
<tr>
<td valign="top">
<?
if(file_exists($_BASEPATH.'img/users/'.$target_id.'.jpg')) { ?>
<a href="<?='../img/users/'.$target_id.'.jpg'?>">
<img src="<?='../img/users/'.$target_id.'.jpg'?>" border="0" width="150" /></a>
<? } else { ?>
<img src="../img/nopic.gif" border="0">
<? } ?>
</td>
<td valign="top" width="100%">
<input type="file" name="userfile">
<br />
<br />
<input type="submit" name="delete_image" value="<?=LANG_ADMIN_DELETE_IMAGE?>" />
<input type="submit" value="<?=LANG_ADMIN_UPLOAD?>" />
</td>
</tr>
</table>

<?
}
/**********************************************************************************************************
Groups
**********************************************************************************************************/

elseif($page == 'groups') {

if(isset($_REQUEST['group'])) {
	$group = $_REQUEST['group'];
        $qprofile = sql_query ("SELECT value FROM ".TABLE_USERDATA." WHERE name='group_".$group."' AND user_id=".$target_id);
                if(sql_num_rows($qprofile) > 0) { $row = sql_fetch_row($qprofile); }
                if(sql_num_rows($qprofile) < 1 || $row[0] == '0') {
                        add_user_to_group ($target_id, $group);
                        }
                else {
                        remove_user_from_group ($target_id, $group);
                        }
        }

init_groups();
$profile = get_profile ($target_id, FALSE);

?>
<select name="group" size="5">
<? while(list($key, $value) = each($groups)) { ?>
<option value="<?=$key?>"><?=$value?><? if($profile['group_'.$key] == 1) echo ' [Member]'; ?></option>
<? } ?>
</select>
<br />
<br />
<input type="submit" value="Add / Remove">
<?
}
/**********************************************************************************************************
Profile
**********************************************************************************************************/

elseif($page == 'profile') {

if(isset($_REQUEST['delete'])) {
	$delete = $_REQUEST['delete'];
	remove_profile($delete, $target_id);
	}

if(isset($_REQUEST['docinput'])) {
	$docinput = $_REQUEST['docinput'];

	set_profile($docinput['key'], $docinput['value'], $target_id);
	}

$qres = sql_query('SELECT count(*) FROM '.TABLE_USERDATA." WHERE user_id = $target_id");
$_temp_row = sql_fetch_row($qres);
$row_count = $_temp_row[0];
unset($_temp_row);

$rows = sql_query_array("SELECT id, name, value FROM ".TABLE_USERDATA." WHERE user_id = $target_id ".
			"ORDER BY $sort_order ".sql_limit($rows_per_page, $first_item), 'id');

lftk_display_page_links ($sort_order, $row_count, $rows_per_page, $first_item);

lftk_display_table_header(
	array(
		'name'		=> LANG_ADMIN_KEY,
		'value'		=> LANG_ADMIN_VALUE,
		'_delete'	=> LANG_ADMIN_DELETE
		),
	array(
		'name'		=> 'left',
		'value'		=> 'left',
		'_delete'	=> 'center'
		),
	TRUE);

while(is_array($rows) && list($key, $row) = each($rows)) {
	lftk_display_table_row(
		array(
			'name'		=> htmlentities($row['name']),
			'value'		=> htmlentities($row['value']),
			'_delete'	=> 
				'<a onclick="return confirm(\''.LANG_ARE_YOU_SURE.'\')" href="'.$_SERVER['PHP_SELF'].
                        	"?page=$page&target_id=$target_id&delete=".urlencode($row['name']).'">'.
                        	'<img src="img/lftk_delete.gif" border="0" alt="'.LANG_ADMIN_DELETE.'"></a> '
			),
		array(
			'name'		=> 'left',
			'value'		=> 'left',
			'_delete'	=> 'center'
			)
		);
	}

lftk_display_table_footer();
?>

<?=LANG_ADMIN_KEY?> <input type="text" size="20" name="docinput[key]" />&nbsp;
<?=LANG_ADMIN_VALUE?> <input type="text" size="20" name="docinput[value]" />
<input type="submit" value="<?=LANG_ADMIN_ADD_UPDATE?>" />
<?

}
?>
</td></tr>
</table>
</form>
</td></tr>
</table>
</center>
</body>
</html>
