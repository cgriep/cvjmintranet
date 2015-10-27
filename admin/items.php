<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
	http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 14.06.2002
*/

error_reporting(1); // Disable Warnings

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
<meta name="title" content="Items">
<meta name="description" content="Add/remove item lists and categories.">
<meta name="sort_order" content="9">
<title>
Liquid Bytes AWF Item Management
</title>

<script type="text/javascript">
<!--
function selecttotext() {
        document.form.new_item.value=document.form.item.options[document.form.item.selectedIndex].text
	}
//-->
</script>

<?
	include('header.inc');
?>
<center>
<table width=90% cellpadding="0" cellspacing="0" border="0">
<tr><td align="left">
<center><h2><? echo SITE_TITLE; ?> Item Management</h2>

<?

$table_name = TABLE_ITEMS;

if(isset($_REQUEST['add_item']) && isset($_REQUEST['new_item']) && $_REQUEST['category'] != '' && is_numeric($_REQUEST['active'])) {
	if(isset($_REQUEST['item']) && $_REQUEST['item'] != '') {
		$qres = sql_query("SELECT sort_order FROM $table_name WHERE id=".$_REQUEST['item']);
		$row = sql_fetch_row($qres);
		sql_free_result($qres);
		$qres = sql_query("SELECT id,sort_order FROM $table_name WHERE sort_order>=".$row[0]." AND category='".addslashes($_REQUEST['category'])."' ORDER BY sort_order");
		$new_sort_order = $row[0]-1;
		unset($row);
		while($row[] = sql_fetch_row($qres));
		sql_free_result($qres);
		foreach($row as $line) if(is_numeric($line[1])) sql_query("UPDATE $table_name SET sort_order = ".($line[1]+1)." WHERE id =".$line[0]);
		}
	$qres = sql_query("SELECT sort_order FROM $table_name WHERE category='".addslashes(urldecode($_REQUEST['category']))."' ORDER BY sort_order DESC LIMIT 1");
	$row = sql_fetch_row($qres);
	sql_free_result($qres);
	if(!is_numeric($row[0])) $row[0] = 0;
	if(is_numeric($new_sort_order)) $row[0] = $new_sort_order;
	sql_query("INSERT INTO $table_name SET category='".addslashes(urldecode($_REQUEST['category']))."', caption='".addslashes($_REQUEST['new_item'])."', sort_order = ".($row[0] + 1).", active = ".$_REQUEST['active']);
	}

if(isset($_REQUEST['remove_category']) && isset($_REQUEST['category']) && isset($_REQUEST['confirm'])) {
	sql_query("DELETE FROM $table_name WHERE category='".addslashes(urldecode($_REQUEST['category']))."'");
	}

if(isset($_REQUEST['remove_item_x']) && isset($_REQUEST['item']) && $_REQUEST['item'] != '') {
	$qres = sql_query("SELECT sort_order FROM $table_name WHERE id = ".$_REQUEST['item']);
	$row = sql_fetch_row($qres);
        sql_free_result($qres);
	$sort_order = $row[0];
        $qres = sql_query("SELECT id, sort_order FROM $table_name WHERE sort_order >= ".$sort_order." AND category = '".addslashes($_REQUEST['category']).
			  "' ORDER BY sort_order");
	while($row = sql_fetch_array($qres)) {
		sql_query("UPDATE $table_name SET sort_order = ".(--$row['sort_order'])." WHERE id = ".$row['id']);
		}
                sql_free_result($qres);
	sql_query("DELETE FROM $table_name WHERE id=".$_REQUEST['item']);
	}

if(isset($_REQUEST['rename_item']) && isset($_REQUEST['new_item']) && isset($_REQUEST['item'])) {
	sql_query("UPDATE $table_name SET caption='".addslashes($_REQUEST['new_item'])."' WHERE id=".$_REQUEST['item']);
	}

if(isset($_REQUEST['change_status_x']) && isset($_REQUEST['item'])) {
	$qres = sql_query("SELECT active FROM $table_name WHERE id=".$_REQUEST['item']);
	$row = sql_fetch_row($qres);
	sql_free_result($qres);
	if($row[0] == '1') {
		sql_query("UPDATE $table_name SET active = 0 WHERE id=".$_REQUEST['item']);
		}
	else {
		sql_query("UPDATE $table_name SET active = 1 WHERE id=".$_REQUEST['item']);
		}
	}

if(isset($_REQUEST['move_up_x']) && isset($_REQUEST['item'])) {
	$qres = sql_query("SELECT sort_order FROM $table_name WHERE id=".$_REQUEST['item']);
	$row = sql_fetch_row($qres);
	sql_free_result($qres);
	if($row[0] > 1) {
		sql_query("UPDATE $table_name SET sort_order = sort_order + 1 WHERE sort_order = ".($row[0] - 1));
		sql_query("UPDATE $table_name SET sort_order = sort_order - 1 WHERE id = ".$_REQUEST['item']);
		}
	}

if(isset($_REQUEST['move_down_x']) && isset($_REQUEST['item'])) {
	$qres = sql_query("SELECT sort_order FROM $table_name WHERE id=".$_REQUEST['item']);
	$row = sql_fetch_row($qres);
	sql_free_result($qres);
	sql_query("UPDATE $table_name SET sort_order = sort_order - 1 WHERE sort_order = ".($row[0] + 1));
	sql_query("UPDATE $table_name SET sort_order = sort_order + 1 WHERE id = ".$_REQUEST['item']);
	}

?>
<? if(isset($_REQUEST['remove_category_x']) && $_REQUEST['category'] != '' && !isset($_REQUEST['confirm'])) { ?>

<p>Remove category <b><?=urldecode($_REQUEST['category'])?></b>?</p>
<a href="<?=$_SERVER['PHP_SELF']?>">No</a>&nbsp;&nbsp;<a href="<?=$_SERVER['PHP_SELF']?>?remove_category=1&confirm=1&category=<?=urlencode($_REQUEST['category'])?>">Yes</a>


<? } elseif((isset($_REQUEST['edit_category']) || isset($_REQUEST['edit_category_x'])) && isset($_REQUEST['category']) && !isset($_REQUEST['back_x'])) { ?>
<form name="form" action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="edit_category" value="1">
<input type="hidden" name="category" value="<?=$_REQUEST['category']?>">
<h2><?=urldecode($_REQUEST['category'])?></h2>
<select size="20" name="item" onDblClick="selecttotext()">
<?
$qres = sql_query("SELECT id, caption, active FROM $table_name WHERE category = '".addslashes(urldecode($_REQUEST['category']))."' ORDER BY sort_order");
while($row = sql_fetch_row($qres)) { 
	if($row[1] == '') { $_title = '[empty string]'; } else { $_title = $row[1]; }
	if($row[2] == '1') { $_active = ''; } else { $_active = ' (disabled)'; }
	echo '<option value="'.$row[0].'">'.stripslashes($_title).$_active.'</option>';
	}
?>
</select>
<br />
<span title="Back to categories"><input type="image" src="img/small/left.gif" name="back" value="1" border="0"></span>&nbsp;&nbsp;
<span title="Move up"><input type="image" src="img/small/up.gif" name="move_up" value="1" border="0"></span>
<span title="Move down"><input type="image" src="img/small/down.gif" name="move_down" value="Move Down" border="0"></span>&nbsp;&nbsp;
<span title="Activate/Deactivate"><input type="image" src="img/small/deactivate.gif"name="change_status" value="Enable/Disable" border="0"></span>
<span title="Refresh"><input type="image" src="img/small/refresh.gif" value="Refresh" border="0"></span>
<span title="Delete selected item"><input type="image" src="img/small/delete.gif" name="remove_item" value="Remove Item" border="0"></span><br />
<br />
<input type="hidden" name="active" value="1">
<input type="text" name="new_item" size="75"><br />
<input type="submit" name="rename_item" value="Rename Item">
<input type="submit" name="add_item" value="Add Item">
</form>

<? 
} else { 
// Edit categories ---------------------------------------------------------------------------------------------
?>


<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<select size="20" name="category">
<?
$qres = sql_query("SELECT category, count(*) FROM $table_name GROUP BY category");
while($row = sql_fetch_row($qres)) { 
	echo '<option value="'.urlencode(stripslashes($row[0])).'">'.stripslashes($row[0]).' ('.$row[1].')</option>';
	}
?>
</select><br />
<span title="Refresh"><input type="image" src="img/small/refresh.gif" value="Refresh" border="0"></span>&nbsp;&nbsp;
<span title="Edit category"><input type="image" src="img/small/edit.gif" name="edit_category" value="1" border="0"></span>&nbsp;&nbsp;
<span title="Remove category"><input type="image" src="img/small/delete.gif" name="remove_category" value="1" border="0"></span><br />
</form>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="new_item" value="">
<input type="hidden" name="sort_order" value="0">
<input type="hidden" name="active" value="1">
<input type="text" name="category" size="25"><input name="add_item" type="submit" value="Add Category">
</form>
<? 

// end (category list)
} 

?>
</td></tr>
</table>
<?
        include('footer.inc');
?>
</center>
</body>
</html>
