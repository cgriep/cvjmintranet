<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

while(list($key, $val) = each ($_REQUEST)) {
        $$key = $val;
        }

if(isset($back) && $back == 'Back') header("location: groups.php");

error_reporting(1); // Disable Warnings

$_BASEPATH = dirname(dirname($_SERVER['SCRIPT_FILENAME']));

define('SESSION_STATUS', 'ok');

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');
include($_BASEPATH.'/inc/functions.inc');

init_groups();

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
                if(sql_num_rows($qresult) > 0) {
                        while ($row = sql_fetch_row($qresult)) {
                                define(strtoupper($row[0]),$row[1]);
                                }
                        sql_free_result($qresult);
                        }
if(isset($delete)) {
	remove_group ($delete);
        }

if(isset($back) && $back = 'Back') {
	unset($action);
	unset($id);
	unset($save);
	}
if(isset($addgroup) && $addgroup == 'true') {
	add_group ($name);
	}

if(isset($id) && isset($removekey)) {
	remove_group_profile ($removekey, $id);
	}

if(isset($id) && isset($savekey) && $savekey != '') {
	set_group_profile ($savekey, $savevalue, $id);
	}

if(isset($id) && isset($name) && $name != '') {
	sql_query("UPDATE ".TABLE_GROUPS." SET group_name = '".addslashes($name)."' WHERE id = $id");
	}
?>
<html>
<head>
<meta name="title" content="Groups">
<meta name="description" content="Add/remove groups and their profiles.">
<meta name="sort_order" content="2">
<title>
Liquid Bytes AWF Group Management
</title>
<?
	include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><? echo SITE_TITLE; ?> Group Management</h2></center>
<?
if(isset($action) && $action=='delete' && isset($id)) {
	echo '<center><h3>Delete: Are you sure?</h3>';
	echo '<h3><a href="'.$_SERVER['PHP_SELF'].'">No</a>&nbsp;
		<a href="'.$_SERVER['PHP_SELF'].'?delete='.$id.'">Yes</a></h3>';
	}
elseif(isset($action) && $action == 'newgroup') { ?>
	<b>New Group</b>
	<form action="groups.php" method="post">
	<input type="hidden" name="addgroup" value="true">
	Name<br />
	<input type="text" name="name"><input type="submit" value="Save"><input type="submit" name="back" value="Back">
	<table width="100%" cellpadding="0" cellspacing="0" border="0">
	</form>
	<?
	}
elseif(!isset($id)) {
?>
<table width="100%" cellpadding="2" cellspacing="1" border="0">
<tr>
<td bgcolor="#ddffdd" align="center" width="7%"><b>ID</b></td>
<td bgcolor="#ffdddd" align="left" width="73%"><b>Name</b></td>
<td bgcolor="#ddddff" align="right" width="10%"><b>Members</b></td>
<td bgcolor="#dddddd" align="center" width="10%"><b>Options</b></td>

<?
$qres = sql_query ("SELECT id, group_name FROM ".TABLE_GROUPS);
while($row = sql_fetch_row($qres)) { ?>
	<tr>
	<td bgcolor="#eeffee" align="center"><?=$row[0]?></td>
	<td bgcolor="#ffeeee" align="left"><?=$row[1]?></td>
	<td bgcolor="#eeeeff" align="right"><? 
		$qres2 = sql_query ("SELECT count(*) FROM ".TABLE_USERDATA." WHERE name = 'group_".$row[0]."' AND value = '1'");
		$row2 = sql_fetch_row($qres2);
		echo $row2[0];
		?></td>
	<td bgcolor="#eeeeee" align="center">
	<a href="groups.php?action=delete&id=<?=$row[0]?>"><img src="img/delete.gif" border="0" alt="Delete"></a>&nbsp;
	<a href="groups.php?id=<?=$row[0]?>"><img src="img/edit.gif" border="0" alt="Edit"></a></td>
	</tr>
	<? 
	}

?>
</table>
<p align="center"><a href="groups.php?action=newgroup">Add new group</a></p>
<? } else { 
$qres = sql_query ("SELECT group_name FROM ".TABLE_GROUPS." WHERE id = $id");
$row = sql_fetch_row($qres);
?>
<b>Edit Group</b><p>
<form action="groups.php" method="post">
<input type="hidden" name="id" value="<?=$id?>">
Name<br />
<input type="text" name="name" value="<?=$row[0]?>"><input type="submit" value="Save"><input type="submit" name="back" value="Back">
<table width="100%" cellpadding="0" cellspacing="0" border="0">
</form>
<p><br /><br />
<b>Profile</b>
<?
 		$profile = get_group_profile($id, false);
                if(isset($profile)) {
                echo '<table width="450" border="0" cellpadding="2" cellspacing="1">';
                echo '<tr><td width="100" bgcolor="#dddddd">Key</td>
                        <td width="300" bgcolor="#dddddd" align="right">Value</td>
                        <td width="50" bgcolor="#dddddd">Edit</td></tr>';
                while(list($key, $value) = each($profile)) {
                        echo '<tr><td width="100" bgcolor="#eeeeee">'.$key.'</td>
                                <td width="300" bgcolor="#eeeeee" align="right">'.$value.'</td>
                                <td width="50" bgcolor="#eeeeee">
                                <a href="groups.php?id='.$id.'&removekey='.$key.'">Remove</a>
				</td></tr>';
                        }
                echo '</table>';
                }
                echo '<form action="groups.php" method="get"><br>Key<br><input type=text name="savekey" size="70"><br>
                        <input type="hidden" name="id" value="'.$id.'">
                        <br>Value<br><input type=text name="savevalue" size="70"><br><br>';
                echo '<input type="submit" value="Add"></form>'; ?>		
<? } ?>
</td></tr>
</table>
<?
        include('footer.inc');
?>
</center>
</body>
</html>
