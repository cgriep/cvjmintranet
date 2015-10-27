<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 13.04.2004
*/

// error_reporting(1); // Disable Warnings

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
lftk_import_argument('page', 'html');
lftk_import_argument('first_item', 0);
lftk_import_argument('sort_order', 'label');

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
<meta name="title" content="Document Types">
<meta name="description" content="Allows you to manage AWF's Document Types.">
<meta name="sort_order" content="5">
<title>
Adaptive Website Framework :: Document Types
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
lftk_add_tab('html', 'Overview');
lftk_add_tab('add_dotype', 'Add Document Type');
?>
<td bgcolor="#ffffff" width="100%"><img src="img/pixel.gif" width="1" /></td>
</tr>
</table>
<table bgcolor="<?=$lftk_config['color_3']?>" width="100%" cellpadding="9" cellspacing="0" border="0">
<tr><td>
<?

/**********************************************************************************************************
HTML (other Types will be supported in AWF2)
**********************************************************************************************************/

if($page == 'html') { 

if(isset($_REQUEST['delete']) && is_numeric($_REQUEST['delete'])) {
	sql_delete(TABLE_DOCTYPES, "id = ".$_REQUEST['delete']);
	}
	
if(isset($_REQUEST['refresh']) && is_numeric($_REQUEST['refresh'])) {
	$row = sql_get_single_row(TABLE_DOCTYPES, 'id = '.$_REQUEST['refresh']);
	if(!file_exists(BASE_PATH.INC_PATH.CMODULES_PATH.$row['filename'])) {
		sql_delete(TABLE_DOCTYPES, "id = ".$_REQUEST['refresh']);
		}	
	elseif($file_header = awf_get_file_header(BASE_PATH.INC_PATH.CMODULES_PATH.$row['filename'], 'doctype')) {
		sql_update(TABLE_DOCTYPES, $file_header, "id = ".$_REQUEST['refresh']);
		}
	}

if(isset($_REQUEST['show']) && is_numeric($_REQUEST['show'])) {
	sql_update(TABLE_DOCTYPES, array('visible' => '1'), 
			"id = '".addslashes($_REQUEST['show'])."'");
	}

if(isset($_REQUEST['hide']) && is_numeric($_REQUEST['hide'])) {
	sql_update(TABLE_DOCTYPES, array('visible' => '0'), 
			"id = '".addslashes($_REQUEST['hide'])."'");
	}

// If file exisits, add a refresh button
// If file not exists, add a remove button

if($dp = opendir(BASE_PATH.INC_PATH.CMODULES_PATH)) {
	while(FALSE !== ($filename = readdir($dp))) {
		if($filename != '.' && $filename != '..' &&
			!sql_get_single_row (TABLE_DOCTYPES, "filename = '$filename'")) {
			$file_header = awf_get_file_header(BASE_PATH.INC_PATH.CMODULES_PATH.$filename, 'doctype');
			if($file_header['name'] != '' && $file_header['label'] != '') { 
				sql_insert(TABLE_DOCTYPES, $file_header); 
				}
			}
		}

	closedir($dp);
	unset($_doctype);
	}

if(isset($_REQUEST['search']) && $_REQUEST['search'] != '') {
        $search = strtr(addslashes($_REQUEST['search']),'*','%');
        $_search = " WHERE format = 'html' AND (name LIKE '$search' OR description LIKE '$search' OR id = '$search') ";
        }
else {
        $_search = " WHERE format = 'html'";
        }

$qres = sql_query('SELECT count(*) FROM '.TABLE_DOCTYPES.$_search);
$_temp_row = sql_fetch_row($qres);
$row_count = $_temp_row[0];
unset($_temp_row);

$rows = sql_query_array('SELECT * FROM '.TABLE_DOCTYPES.$_search.' ORDER BY '.$sort_order.
			' '.sql_limit($rows_per_page, $first_item), 'id');

lftk_display_search_input ('search');

lftk_display_page_links ($sort_order, $row_count, $rows_per_page, $first_item);

lftk_display_table_header(
	array(
		'label' 	=> LANG_ADMIN_LABEL, 
		'name' 		=> LANG_ADMIN_NAME, 
		'filename'	=> LANG_ADMIN_FILENAME, 
		'type' 		=> LANG_ADMIN_TYPE, 
		'license' 	=> LANG_ADMIN_LICENSE, 
		'version' 	=> LANG_ADMIN_VERSION, 
		'lastchange' 	=> LANG_ADMIN_LAST_CHANGE, 
		'visible' 	=> LANG_ADMIN_VISIBLE, 
		'_delete'	=> LANG_ADMIN_DELETE,
		'_refresh'	=> LANG_ADMIN_REFRESH,
		'_edit'		=> LANG_ADMIN_EDIT), 
	array(
		'version' 	=> 'right',
		'lastchange' 	=> 'right',
		'visible'      	=> 'center',
		'_delete'     	=> 'center',
		'_refresh'     	=> 'center',
		'_edit'      	=> 'center'),
	TRUE
	);

while(is_array($rows) && list($key, $row) = each($rows)) {

	if($row['visible'] == 0) {
                $_visible = get_image_tag('img/not_checked.gif','',
                        $_SERVER['PHP_SELF'].'?page='.$page.'&first_item='.$first_item.'&sort_order='.$sort_order.
                        '&show='.$row['id']);
                }
        else {
                $_visible = get_image_tag('img/checked.gif','',
                        $_SERVER['PHP_SELF'].'?page='.$page.'&first_item='.$first_item.'&sort_order='.$sort_order.
                        '&hide='.$row['id']);
                }


	lftk_display_table_row (	
		array(
		'label' 	=> htmlentities($row['label']), 
		'name' 		=> $row['name'], 
		'filename'	=> $row['filename'], 
		'type' 		=> ucfirst($row['type']), 
		'license' 	=> htmlentities($row['license']), 
		'version' 	=> htmlentities($row['version']), 
		'lastchange' 	=> htmlentities($row['lastchange']), 
		'visible' 	=> $_visible, 
		'_delete'		=>	
			'<a onclick="return confirm(\''.LANG_ARE_YOU_SURE.'\')" href="'.$_SERVER['PHP_SELF'].
			"?sort_order=$sort_order&first_item=$first_item&page=$page&delete=".$row['id'].'">'.
			'<img src="img/lftk_delete.gif" border="0" alt="'.LANG_ADMIN_DELETE.'"></a> ', 
		'_refresh'		=>	
			'<a onclick="return confirm(\''.LANG_ARE_YOU_SURE.'\')" href="'.$_SERVER['PHP_SELF'].
			"?sort_order=$sort_order&first_item=$first_item&page=$page&refresh=".$row['id'].'">'.
			'<img src="img/lftk_refresh.gif" border="0" alt="'.LANG_ADMIN_DELETE.'"></a> ', 
		'_edit'		=>	
			'<a href="javascript:open_window(\'edit_doctype.php?target_id='.$row['id'].'\')">'.
			'<img src="img/lftk_edit.gif" border="0" alt="'.LANG_ADMIN_EDIT.'"></a>'), 
			
		array(
		'version' 	=> 'right',
		'lastchange' 	=> 'right',
		'visible'      	=> 'center',
		'_delete'     	=> 'center',
		'_refresh'     	=> 'center',
		'_edit'       	=> 'center')
		);
	}

lftk_display_table_footer();
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
