<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 04.04.2004
*/

error_reporting(1); // Disable Warnings

if(isset($_REQUEST['page'])) {
        $page = $_REQUEST['page'];
        }
else {
        $page = 'links';
        }

$_BASEPATH = strrev(strstr(substr(strstr(strrev($_SERVER['PATH_TRANSLATED']),'/'), 1),'/'));

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');
include($_BASEPATH.'/inc/functions.inc');

// Load Liquid Installer Lib
include('linstaller_lib.inc');

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
<meta name="title" content="Link Management">
<meta name="description" content="">
<meta name="sort_order" content="5">
<title>
Liquid Bytes AWF Link Management
</title>
<?
	include('header.inc');
?>
<center>
<table width="90%" cellpadding="0" cellspacing="0" border="0">
<tr><td align="left">
<form action="<?=$_SERVER['PHP_SELF']?>" method="get">
<input type="hidden" name="page" value="<?=$page?>" />
<table width="100%" cellpadding="5" cellspacing="0" border="0"><tr>
<?
lbi_add_tab('links', LANG_ADMIN_LINKS);
?>
<td bgcolor="#ffffff" width="100%"><img src="img/pixel.gif" width="1" /></td>
</tr>
</table>
<table bgcolor="<?=$design['color_3']?>" width="100%" cellpadding="9" cellspacing="0" border="0">
<tr><td>
<? if($page == 'links') { 
lbi_display_page_links ('order', 36, 10, 1)
?>
<?=$design['table_begin']?>
<tr bgcolor="<?=$design['color_1']?>">
<td><b>Link</b></td>
<td align="center"><b>Options</b></td>
</tr>
<tr bgcolor="<?=$design['color_2']?>">
<td>http://www.heise.de/</td>
<td></td>
</tr>
</table>
<? } ?>
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
