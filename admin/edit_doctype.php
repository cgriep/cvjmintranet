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
lftk_import_argument('page', 'fields');
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

$row = sql_get_single_row(TABLE_DOCTYPES, 'id = '.$target_id);

?>

<html>
<head>
<title>
<?=$row['label']?>
</title>
<link rel="stylesheet" type="text/css" href="admin.css" />
</head>
<body topmargin="0" rightmargin="0" leftmargin="0" marginwidth="0" marginheight="0" bgcolor="#FFFFFF">
<table border="0" cellpadding="5" cellspacing="0" bgcolor="#ffffff" width="100%">
<tr>
<?
/*
<td valign="middle">
// =get_image_tag('img/doctype.gif','','',0,0,'middle')
</td>

*/
?>
<td valign="middle" width="100%">
<span style="font-weight: bold; font-size: 16px;"><?=$row['label']?></span>
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
lftk_add_tab('fields', LANG_ADMIN_FIELDS);
?>
<td bgcolor="#ffffff" width="100%"><img src="img/pixel.gif" width="1" /></td>
</tr>
</table>
<table bgcolor="<?=$lftk_config['color_3']?>" width="100%" cellpadding="9" cellspacing="0" border="0">
<tr><td>
<?
/**********************************************************************************************************
Fields
**********************************************************************************************************/

if($page == 'fields') {

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
