<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.04.2004
*/

error_reporting(1); // Disable Warnings

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
lftk_import_argument('page', 'users');
lftk_import_argument('first_item', 0);
lftk_import_argument('sort_order', 'id');

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

?>

<html>
<head>
<meta name="title" content="User Management">
<meta name="description" content="Allows you to add, update and remove users/groups.">
<meta name="sort_order" content="1">
<title>
Adaptive Website Framework :: User Management
</title>
<?
	include('header.inc');
?>
<center>
<script language="javascript">
<!--
function open_window(url) {
        window.open(url, "<?=time()?>",
	"width=500,height=540,dependent=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=yes,status=no,location=no");
        }
// -->
</script>
<table width="90%" cellpadding="0" cellspacing="0" border="0">
<tr><td align="left">
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="page" value="<?=$page?>" />
<input type="hidden" name="sort_order" value="<?=$sort_order?>" />
<table width="100%" cellpadding="5" cellspacing="0" border="0"><tr>
<?
lftk_add_tab('users', LANG_ADMIN_USERS);
lftk_add_tab('add_user', LANG_ADMIN_ADD_USER);
lftk_add_tab('groups', LANG_ADMIN_GROUPS);
lftk_add_tab('add_group', LANG_ADMIN_ADD_GROUP);
lftk_add_tab('export', LANG_ADMIN_EXPORT);
?>
<td bgcolor="#ffffff" width="100%"><img src="img/pixel.gif" width="1" /></td>
</tr>
</table>
<table bgcolor="<?=$lftk_config['color_3']?>" width="100%" cellpadding="9" cellspacing="0" border="0">
<tr><td>
<?

/**********************************************************************************************************
Users
**********************************************************************************************************/

if($page == 'users') { 

if(isset($_REQUEST['delete'])) {
	sql_delete(TABLE_USERS,    "id = ".$_REQUEST['delete']);
	sql_delete(TABLE_USERDATA, "user_id = ".$_REQUEST['delete']);
	sql_delete(TABLE_ITEMMAP,  "target LIKE 'user%' AND target_id = ".$_REQUEST['delete']);
	}

if(isset($_REQUEST['enable'])) {
	sql_query('UPDATE '.TABLE_USERS.' SET valid = 1 WHERE id = '.$_REQUEST['enable']);
	}

if(isset($_REQUEST['disable'])) {
	sql_query('UPDATE '.TABLE_USERS.' SET valid = 0 WHERE id = '.$_REQUEST['disable']);
	}

if(isset($_REQUEST['search']) && $_REQUEST['search'] != '') {
	$search = strtr(addslashes($_REQUEST['search']),'*','%');
	$_search = " WHERE username LIKE '$search' OR email LIKE '$search' OR id = '$search' ";
	}
else {
	$_search = '';
	}

$qres = sql_query('SELECT count(*) FROM '.TABLE_USERS.$_search);
$_temp_row = sql_fetch_row($qres);
$row_count = $_temp_row[0];
unset($_temp_row);

$rows = sql_query_array('SELECT id, username, email, valid, views, '.
			'date_format(last_login, \'%d'.LANG_ADMIN_DATE_SEPARATOR.'%m'.LANG_ADMIN_DATE_SEPARATOR.'%Y %H:%i\') as last_login, '.
			'expires, registered '.
			'FROM '.TABLE_USERS.$_search.' ORDER BY '.$sort_order.
			' '.sql_limit($rows_per_page, $first_item), 'id');

lftk_display_search_input ('search');

lftk_display_page_links ($sort_order, $row_count, $rows_per_page, $first_item);

lftk_display_table_header(
	array(
		'id' 		=> LANG_ADMIN_ID, 
		'username' 	=> LANG_ADMIN_USERNAME, 
		'email' 	=> LANG_ADMIN_EMAIL, 
		'registered' 	=> LANG_ADMIN_REGISTERED,
		'expires' 	=> LANG_ADMIN_EXPIRES,
		'last_login' 	=> LANG_ADMIN_LAST_LOGIN,
		'views' 	=> LANG_ADMIN_VIEWS,
		'valid' 	=> LANG_ADMIN_VALID,
		'_delete'	=> LANG_ADMIN_DELETE,
		'_edit'		=> LANG_ADMIN_EDIT), 
	array(
		'id'		=> 'right',
		'views'         => 'right',
		'registered'    => 'right',
		'expires'       => 'right',
		'last_login'    => 'right',
		'valid'         => 'center',
		'_delete'      	=> 'center',
		'_edit'      	=> 'center'),
	TRUE
	);

while(is_array($rows) && list($key, $row) = each($rows)) {

	if($row['registered'] != 0) {
		$_registered = date(LANG_ADMIN_DATE_FORMAT, $row['registered']);
		}
	else {
		$_registered = LANG_ADMIN_UNKNOWN;
		}

	if($row['expires'] != 0) {
		$_expires    = date(LANG_ADMIN_DATE_FORMAT, $row['expires']);
		}
	else {
		$_expires    = LANG_ADMIN_NEVER;
		}

	if($row['valid'] == 0) {
		$_valid = get_image_tag('img/not_checked.gif','',
			$_SERVER['PHP_SELF'].'?page='.$page.'&first_item='.$first_item.'&sort_order='.$sort_order.
			'&enable='.$row['id']);
		}
	else {
		$_valid = get_image_tag('img/checked.gif','',
			$_SERVER['PHP_SELF'].'?page='.$page.'&first_item='.$first_item.'&sort_order='.$sort_order.	
			'&disable='.$row['id']);
		}

	lftk_display_table_row (	
		array(
		'id' 		=> $key, 
		'username' 	=> $row['username'], 
		'email' 	=> $row['email'], 
		'registered' 	=> $_registered,
		'expires' 	=> $_expires,
		'last_login' 	=> $row['last_login'],
		'views' 	=> $row['views'],
		'valid' 	=> $_valid,
		'_delete'		=>	
			'<a onclick="return confirm(\''.LANG_ARE_YOU_SURE.'\')" href="'.$_SERVER['PHP_SELF'].
			"?sort_order=$sort_order&first_item=$first_item&page=$page&delete=".$row['id'].'">'.
			'<img src="img/lftk_delete.gif" border="0" alt="'.LANG_ADMIN_DELETE.'"></a> ', 
		'_edit'		=>	
			'<a href="javascript:open_window(\'edit_user.php?target_id='.$row['id'].'\')">'.
			'<img src="img/lftk_edit.gif" border="0" alt="'.LANG_ADMIN_EDIT.'"></a>'), 
			
		array(
		'id'		=> 'right',
		'views'         => 'right',
		'registered'   	=> 'right',
		'expires'    	=> 'right',
		'last_login'    => 'right',
		'valid'         => 'center',
		'_delete'      	=> 'center',
		'_edit'       	=> 'center')
		);
	}

lftk_display_table_footer();

}

/**********************************************************************************************************
Add User
**********************************************************************************************************/

elseif($page == 'add_user') { 

if(isset($_REQUEST['docinput'])) {
	$docinput = $_REQUEST['docinput'];
	}

if(isset($docinput) && awf_login_is_unique($docinput['username']) && 
awf_login_is_unique($docinput['email'])) {
	$new_user_id = sql_insert (TABLE_USERS, array(
			'username' 	=> $docinput['username'],
			'email'		=> $docinput['email'],
			'password'	=> $docinput['password'],
			'registered'	=> time(),
			'valid'		=> 1,
			'online'	=> 0,
			'expires'	=> 0
			)
			);
	?>
	<script language="javascript">
	<!--
        open_window('edit_user.php?target_id=<?=$new_user_id?>');
	// -->
	</script>
	<?=LANG_ADMIN_USER_ADDED?> <?=htmlentities($docinput['username'])?>
	<br />
	<br />
	<p align="right">
	<input type="submit" value="<?=LANG_ADMIN_OK?>" />
	</p>
	<?
	}
	
else {

?>
<?=LANG_ADMIN_USERNAME?>
<br />
<?
if(isset($docinput['username']) && $docinput['username'] == '') {
        lftk_display_error (LANG_ADMIN_USERNAME_REQUIRED);
	}
elseif(isset($docinput['username']) && !awf_login_is_unique($docinput['username'])) {
        lftk_display_error (LANG_ADMIN_USERNAME_EXISTS);
        }
?>
<input type="text" size="60" name="docinput[username]" value="<?=htmlentities($docinput['username'])?>" />
<br />
<br />
<?=LANG_ADMIN_EMAIL?>
<br />
<?
if(isset($docinput['email']) && $docinput['email'] == '') {
        lftk_display_error (LANG_ADMIN_EMAIL_REQUIRED);
	}
elseif(isset($docinput['email']) && !awf_login_is_unique($docinput['email'])) {
        lftk_display_error (LANG_ADMIN_EMAIL_EXISTS);
        }
?>
<input type="text" size="60" name="docinput[email]" value="<?=htmlentities($docinput['email'])?>" />
<br />
<br />
<?=LANG_ADMIN_PASSWORD?>
<br />
<?
if(isset($docinput['password']) && strlen($docinput['password']) < 6) {
        lftk_display_warning (LANG_ADMIN_PASSWORD_SHORT);
        }
?>
<input type="password" size="60" name="docinput[password]" value="<?=htmlentities($docinput['password'])?>" />
<br />
<br />
<p align="right">
<input type="submit" value="<?=LANG_ADMIN_ADD_USER?>" />
</p>
<?
}
} 

/**********************************************************************************************************
Groups
**********************************************************************************************************/

elseif($page == 'groups') { 

if(isset($_REQUEST['delete'])) {
	sql_delete(TABLE_USERDATA,  "name = 'group_".$_REQUEST['delete']."'");
	sql_delete(TABLE_GROUPS,    "id = ".$_REQUEST['delete']);
	sql_delete(TABLE_GROUPDATA, "group_id = ".$_REQUEST['delete']);
	}

if(isset($_REQUEST['search']) && $_REQUEST['search'] != '') {
	$search = strtr(addslashes($_REQUEST['search']),'*','%');
	$_search = " WHERE group_name LIKE '$search' OR id = '$search' ";
	}
else {
	$_search = '';
	}

$qres = sql_query('SELECT count(*) FROM '.TABLE_GROUPS.$_search);
$_temp_row = sql_fetch_row($qres);
$row_count = $_temp_row[0];
unset($_temp_row);

$rows = sql_query_array('SELECT id, group_name FROM '.TABLE_GROUPS.$_search.' ORDER BY '.$sort_order.
			' '.sql_limit($rows_per_page, $first_item), 'id');

lftk_display_search_input ('search');

lftk_display_page_links ($sort_order, $row_count, $rows_per_page, $first_item);

lftk_display_table_header(
	array(
		'id' 		=> LANG_ADMIN_ID, 
		'group_name'	=> LANG_ADMIN_NAME, 
		'members' 	=> LANG_ADMIN_MEMBERS, 
		'_delete'	=> LANG_ADMIN_DELETE,
		'_edit'		=> LANG_ADMIN_EDIT), 
	array(
		'id'		=> 'right',
		'members'	=> 'right',
		'_delete'      	=> 'center',
		'_edit'      	=> 'center'),
	TRUE
	);

while(is_array($rows) && list($key, $row) = each($rows)) {
	$qres = sql_query ("SELECT count(*) FROM ".TABLE_USERDATA." WHERE name = 'group_$key' AND value = '1'");
        $_temp_row = sql_fetch_row($qres);
	sql_free_result($qres);

	lftk_display_table_row (	
		array(
		'id' 		=> $key, 
		'group_name'	=> $row['group_name'], 
		'members' 	=> $_temp_row[0], 
		'_delete'		=>	
			'<a onclick="return confirm(\''.LANG_ARE_YOU_SURE.'\')" href="'.$_SERVER['PHP_SELF'].
			"?sort_order=$sort_order&first_item=$first_item&page=$page&delete=".$row['id'].'">'.
			'<img src="img/lftk_delete.gif" border="0" alt="'.LANG_ADMIN_DELETE.'"></a> ', 
		'_edit'		=>	
			'<a href="javascript:open_window(\'edit_group.php?target_id='.$row['id'].'\')">'.
			'<img src="img/lftk_edit.gif" border="0" alt="'.LANG_ADMIN_EDIT.'"></a>'), 
			
		array(
		'id'		=> 'right',
		'members'	=> 'right',
		'_delete'      	=> 'center',
		'_edit'       	=> 'center')
		);

	unset($_temp_row);
	}

lftk_display_table_footer();
}

/**********************************************************************************************************
Add Group
**********************************************************************************************************/

elseif($page == 'add_group') { 

if(isset($_REQUEST['docinput'])) {
	$docinput = $_REQUEST['docinput'];
	$qres = sql_query("SELECT count(*) FROM ".TABLE_GROUPS." WHERE group_name = '".
                addslashes($docinput['name'])."'");
        $_temp_row = sql_fetch_row($qres);
        $row_count = $_temp_row[0];
        unset($_temp_row);
	}

if(isset($docinput) && $docinput['name'] != '' && $row_count == 0) {
	$new_group_id = sql_insert (TABLE_GROUPS, array(
			'group_name' 		=> $docinput['name']
			)
			);
	?>
	<script language="javascript">
	<!--
        open_window('edit_group.php?target_id=<?=$new_group_id?>');
	// -->
	</script>
	<?=LANG_ADMIN_GROUP_ADDED?> <?=htmlentities($docinput['name'])?>
	<br />
	<br />
	<p align="right">
	<input type="submit" value="<?=LANG_ADMIN_OK?>" />
	</p>
	<?
	}
	
else {
?>
<?=LANG_ADMIN_NAME?>
<br />
<?
if(isset($docinput['name']) && $docinput['name'] == '') {
        lftk_display_error (LANG_ADMIN_NAME_REQUIRED);
	}
elseif(isset($docinput['name']) && $row_count > 0) {
        lftk_display_error (LANG_ADMIN_NAME_EXISTS);
        }
?>
<input type="text" size="60" name="docinput[name]" value="<?=htmlentities($docinput['name'])?>" />
<br />
<br />
<p align="right">
<input type="submit" value="<?=LANG_ADMIN_ADD_GROUP?>" />
</p>
<?
}
}

/**********************************************************************************************************
Export
**********************************************************************************************************/

elseif($page == 'export') { 

if(isset($_REQUEST['docinput'])) {
$docinput = $_REQUEST['docinput'];
?>
<script language="javascript">
<!--
function open_export_window(url) {
        window.open(url, "<?=time()?>",
	"width=700,height=500,dependent=yes,toolbar=yes,menubar=yes,scrollbars=yes,resizable=yes,status=yes,location=no");
        }

open_export_window('export.php?type=<?=$docinput['export_type']?>&format=<?=$docinput['export_format']?>');
// -->
</script>
<? } ?>
<?=LANG_ADMIN_EXPORT?>&nbsp;
<select name="docinput[export_type]">
<option value="all_users_realnames"><?=LANG_ADMIN_USERS_WITH_NAMES?></option>
<option value="all_users"><?=LANG_ADMIN_USERS_WITH_PASSWD?></option>
<option value="expired_users"><?=LANG_ADMIN_EXPIRED_USERS?></option>
<option value="birthday_list"><?=LANG_ADMIN_BIRTHDAY_LIST?></option>
<option value="contact_list"><?=LANG_ADMIN_CONTACT_LIST?></option>
</select>
&nbsp;<?=LANG_ADMIN_AS?>&nbsp;
<select name="docinput[export_format]">
<option value="html">HTML</option>
<option value="xml">XML</option>
<option value="text">Plain Text</option>
<option value="csv">CSV</option>
</select>
<p align="right">
<input type="submit" value="<?=LANG_ADMIN_EXPORT?>" />
</p>
<?
}
?>
</td></tr>
</table>
</form>
</td></tr>
</table>
<?
        include('footer.inc');
?>
</center>
</body>
</html>
