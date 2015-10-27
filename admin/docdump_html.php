<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

error_reporting(1); // Disable Warnings

define('SESSION_STATUS', 'ok');

$_BASEPATH = dirname(dirname($_SERVER['SCRIPT_FILENAME']));

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');
include($_BASEPATH.'/inc/functions.inc');

                // Read all contstants from database
                $qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
                if(sql_num_rows($qresult) > 0) {
                        while ($row = sql_fetch_row($qresult)) {
                                define(strtoupper($row[0]),$row[1]);
                                }
                        sql_free_result($qresult);
                        }
$order_by = 'id';

?>

<html>
<head>
<title>
Liquid Bytes AWF - HTML document dump
</title>
</head>
<body bgcolor="#ffffff" topmargin="0" rightmargin="0" leftmargin="0" marginwidth="0" marginheight="0">
<?
$qres = sql_query("SELECT id, sort_order, parent_id, enabled, published, searchable, cacheable, type_id, description, 
key_id, release_date, members_only, views FROM ".TABLE_NODES." ORDER BY $order_by");

$qres2 = sql_query("SELECT DISTINCT name FROM ".TABLE_NODEDATA." WHERE flavour_id=".DEFAULT_FLAVOUR." ORDER BY name");

while($row2 = sql_fetch_row($qres2)) {
	$nodedata[] = $row2[0];
	}
?>
<table width="100%" border="0" cellpadding="2" cellspacing="1">
        <tr>
        <td>id</td>
	<td>sort_order</td>
	<td>parent_id</td>
	<td>enabled</td>
        <td>published</td>
        <td>searchable</td>
	<?
	while(list($key, $value) = each($nodedata)) {
        echo "<td>$value</td>";
        }
	?>
        </tr>
	<?
while($row = sql_fetch_row($qres)) {
	if($row[3]==1) { $row[3] = '1'; } else { $row[3] = '0'; } 
	?>
	<tr>
        <td><?=$row[0]?></td>
        <td><?=$row[1]?></td>
        <td><?=$row[2]?></td>
        <td><?=$row[3]?></td>
        <td><?=$row[4]?></td>
        <td><?=$row[5]?></td>
	<?
	$node = get_nodedata ($row[0], DEFAULT_FLAVOUR);
	reset($nodedata);
	while(list($key, $value) = each($nodedata)) {
        echo "<td>$node[$value]</td>";
       }
	?>
        </tr>
		<? } ?>
</table>
</body>
</html>
