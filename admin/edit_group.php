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
lftk_import_argument('page', 'profile');
lftk_import_argument('target_id', FALSE);
lftk_import_argument('first_item', 0);

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

$row = sql_get_single_row(TABLE_GROUPS, 'id = '.$target_id);

?>

<html>
<head>
<title>
<?=$row['group_name']?>
</title>
<link rel="stylesheet" type="text/css" href="admin.css" />
</head>
<body topmargin="0" rightmargin="0" leftmargin="0" marginwidth="0" marginheight="0" bgcolor="#FFFFFF">
<script language="javascript">
<!--
function open_window(url) {
        window.open(url, "<?=time()?>",
        "width=500,height=540,dependent=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=yes,status=no,location=no");
        }
// -->
</script>
<table border="0" cellpadding="5" cellspacing="0" bgcolor="#ffffff" width="100%">
<tr>
<td valign="middle">
<?=get_image_tag('img/group.gif','','',0,0,'middle')?>
</td>
<td valign="middle" width="100%">
<span style="font-weight: bold; font-size: 16px;"><?=$row['group_name']?></span>
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
<?php
lftk_add_tab('profile', LANG_ADMIN_PROFILE);
lftk_add_tab('members', LANG_ADMIN_MEMBERS);
lftk_add_tab('name', LANG_ADMIN_NAME);
?>
<td bgcolor="#ffffff" width="100%"><img src="img/pixel.gif" width="1" /></td>
</tr>
</table>
<table bgcolor="<?=$lftk_config['color_3']?>" width="100%" cellpadding="9" cellspacing="0" border="0">
<tr><td>
<?php
/**********************************************************************************************************
Profile
**********************************************************************************************************/

if($page == 'profile') {

lftk_import_argument('sort_order', 'name');

if(isset($_REQUEST['delete'])) {
	$delete = $_REQUEST['delete'];
	remove_group_profile($delete, $target_id);
	}

if(isset($_REQUEST['docinput'])) {
	$docinput = $_REQUEST['docinput'];

	set_group_profile($docinput['key'], $docinput['value'], $target_id);
	}

unset($qres);
$qres = sql_query('SELECT count(*) FROM '.TABLE_GROUPDATA." WHERE group_id = $target_id");
$_temp_row = sql_fetch_row($qres);
$row_count = $_temp_row[0];
unset($_temp_row);

$rows = sql_query_array("SELECT id, name, value FROM ".TABLE_GROUPDATA." WHERE group_id = $target_id ".
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
<?php
}
/**********************************************************************************************************
Members
**********************************************************************************************************/

elseif($page == 'members') {

lftk_import_argument('sort_order', 'id');

$qres = sql_query("SELECT count(*) FROM ".TABLE_USERDATA." WHERE ".TABLE_USERDATA.
".name = 'group_$target_id' AND ".TABLE_USERDATA.".value = '1'");
$_temp_row = sql_fetch_row($qres);
$row_count = $_temp_row[0];
unset($_temp_row);

$rows = sql_query_array("SELECT ".TABLE_USERS.".id, username, email FROM ".TABLE_USERS.",".TABLE_USERDATA.
" WHERE ".TABLE_USERDATA.".name = 'group_$target_id' AND ".TABLE_USERDATA.".value = '1' AND ".
TABLE_USERS.".id = ".TABLE_USERDATA.".user_id ORDER BY $sort_order ".
sql_limit($rows_per_page, $first_item), 'id');

lftk_display_page_links ($sort_order, $row_count, $rows_per_page, $first_item);

lftk_display_table_header(
	array(
		'id'		=> LANG_ADMIN_ID,
		'username'	=> LANG_ADMIN_USERNAME,
		'email'		=> LANG_ADMIN_EMAIL,
		'_edit'		=> LANG_ADMIN_EDIT
		),
	array(
		'id'		=> 'left',
		'username'	=> 'left',
		'email'		=> 'left',
		'_edit'		=> 'center'
		),
	TRUE);

while(is_array($rows) && list($key, $row) = each($rows)) {
	lftk_display_table_row(
		array(
			'id'		=> $key,
			'username'	=> htmlentities($row['username']),
			'email'		=> htmlentities($row['email']),
			'_edit'		=> 
					'<a href="javascript:open_window(\'edit_user.php?target_id='.$key.'\')">'.
					'<img src="img/lftk_edit.gif" border="0" alt="'.LANG_ADMIN_EDIT.'"></a>'
			),
		array(
			'id'		=> 'left',
			'username'	=> 'left',
			'email'		=> 'left',
			'_edit'		=> 'center'
			)
		);
	}
lftk_display_table_footer();
}
/**********************************************************************************************************
Name
**********************************************************************************************************/

elseif($page == 'name') {

$row = sql_get_single_row(TABLE_GROUPS, "id = $target_id");

if(isset($_REQUEST['docinput'])) {
	$docinput = $_REQUEST['docinput'];

	$qres = sql_query("SELECT count(*) FROM ".TABLE_GROUPS." WHERE group_name = '".
		addslashes($docinput['group_name'])."' AND id != $target_id");
	$_temp_row = sql_fetch_row($qres);
	$row_count = $_temp_row[0];
	unset($_temp_row);
	
	if($row_count == 0) {
		sql_update(TABLE_GROUPS, 
			array(
				'group_name' => $docinput['group_name']
				),
			"id = $target_id");

		}
	$row['group_name'] = $docinput['group_name'];
	}
?>
<?=LANG_ADMIN_NAME?>
<br />
<?php
if(isset($docinput)) {

if($row_count > 0) {
        lftk_display_error (LANG_ADMIN_NAME_EXISTS);
        }
}
?>
<input type="text" size="60" name="docinput[group_name]" value="<?=htmlentities($row['group_name'])?>" />
<br />
<br />
<p align="right">
<input type="reset" value="<?=LANG_ADMIN_RESET?>" />
<input type="reset" onclick="window.close();" value="<?=LANG_ADMIN_CLOSE?>" />
<input type="submit" value="<?=LANG_ADMIN_SAVE?>" />
</p>
<?php
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
