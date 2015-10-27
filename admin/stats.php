<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$max = 25;

error_reporting(1); // Disable Warnings

define('SESSION_STATUS', 'ok');

while(list($key, $val) = each ($_REQUEST)) {
        $$key = $val;
        }

$_BASEPATH = dirname(dirname($_SERVER['SCRIPT_FILENAME']));

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');
include($_BASEPATH.'/inc/functions.inc');

                // Read all contstants from database
                $qresult = sql_query ("SELECT name, value FROM ".$db_table_prefix."setup");
                if(sql_num_rows($qresult) > 0) {
                        while ($row = sql_fetch_row($qresult)) {
                                define(strtoupper($row[0]),$row[1]);
                                }
                        sql_free_result($qresult);
                        }

?>

<html>
<head>
<meta name="title" content="Statistics">
<meta name="description" content="">
<meta name="sort_order" content="3">
<title>
Liquid Bytes AWF Statistics
</title>
<?
	include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><? echo SITE_TITLE; ?> Statistics</h2></center>
<h2>Top <?php echo $max; ?> Downloads/Links</h2>
<table width="100%" border="0" cellpadding="2" cellspacing="1">
<tr>
        <td bgcolor="#ddffdd" width="10%" align="right">
                <b>Views</b></td>
        <td bgcolor="#ffdddd" width="70%" align="left">
                <b>URL</b></td>
        <td bgcolor="#ddddff" width="20%" align="left">
                <b>Referer</b></td>
</tr>
<?

$qres = sql_query("SELECT url, referer, views FROM ".$db_table_prefix."redirect ORDER BY views DESC LIMIT $max");

if(sql_num_rows($qres) > 0) {
while($row = sql_fetch_row($qres)) {
	if(strlen($row[0]) > 100) { $row[0] = substr($row[0], 0, 100).'...'; }
	echo ('<tr><td bgcolor="#eeffee" width="10%" align="right">'.$row[2].'</td>'.
		'<td bgcolor="#ffeeee" width="70%" align="left"><a href="'.$row[0].'" target="_blank">'.$row[0].'</a></td>'.
		'<td bgcolor="#eeeeff" width="20%" align="left">'.$row[1].'</td></tr>');
	}
sql_free_result($qres);
}

?>

</table>
<br><br>
<h2>Top <?php echo $max; ?> Documents</h2>
<table width="100%" border="0" cellpadding="2" cellspacing="1">
<tr>
        <td bgcolor="#ddffdd" width="10%" align="right">
                <b>Views</b></td>
        <td bgcolor="#ffdddd" width="70%" align="left">
                <b>Title</b></td>
        <td bgcolor="#ddddff" width="20%" align="left">
                <b>ID</b></td>
</tr>
<?

$qres = sql_query("SELECT DISTINCT views, value, node_id FROM ".$db_table_prefix."nodes, ".$db_table_prefix."nodedata 
WHERE node_id=".$db_table_prefix."nodes.id AND name='title' AND flavour_id=".DEFAULT_FLAVOUR."
ORDER BY views DESC LIMIT $max");

if(sql_num_rows($qres) > 0) {
while($row = sql_fetch_row($qres)) {
	$row[1] = stripslashes($row[1]);
	if(strlen($row[1]) > 100) { $row[1] = substr($row[1], 0, 100).'...'; }
	echo ('<tr><td bgcolor="#eeffee" width="10%" align="right">'.$row[0].'</td>'.
		'<td bgcolor="#ffeeee" width="70%" align="left"><a href="../index.php?id='.$row[2].'" target="_blank">'.$row[1].'</a></td>'.
		'<td bgcolor="#eeeeff" width="20%" align="left">'.$row[2].'</td></tr>');
	}
sql_free_result($qres);
}

?>

</table>

</td></tr>
</table>
<?
        include('footer.inc');
?>
</center>
</body>
</html>
