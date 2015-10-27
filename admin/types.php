<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

error_reporting(1); // Disable Warnings

while(list($key, $val) = each ($_REQUEST)) {
        $$key = $val;
        }

$_BASEPATH = dirname(dirname($_SERVER['SCRIPT_FILENAME']));

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".$db_table_prefix."setup");
if(sql_num_rows($qresult) > 0) {
	while ($row = sql_fetch_row($qresult)) {
        	define(strtoupper($row[0]),$row[1]);
                }
	sql_free_result($qresult);
	}

if(!isset($order_by)) { $order_by='id'; }

if(isset($delete)) {
	sql_query("DELETE FROM ".$db_table_prefix."types WHERE id=$delete");
	sql_query("DELETE FROM ".$db_table_prefix."typedata WHERE type_id=$delete");
	}

if($save == 'type') {
	sql_query("UPDATE ".$db_table_prefix."types SET parent_id=$parent_id, name='$name', description='$description' WHERE
			id=$save_id");
	sql_query("UPDATE ".$db_table_prefix."typedata SET template='$template', visible=$visible WHERE
			type_id=$save_id AND platform='phpweb'");
	}

if($save == 'new_type') {
	if(isset($parent_id) && isset($name) && isset($template) && strlen($name) > 0) {
	sql_query("INSERT INTO ".$db_table_prefix."types SET parent_id=$parent_id, name='$name', description='$description'");
        sql_query("INSERT INTO ".$db_table_prefix."typedata SET template='$template', visible=$visible,
			type_id=".sql_insert_id().", platform='phpweb'");
        }
	}

?>

<html>
<head>
<meta name="title" content="Document Types">
<meta name="description" content="Add/remove document types.">
<meta name="sort_order" content="6">
<title>
Liquid Bytes AWF Document Types
</title>
<?
        include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><? echo SITE_TITLE; ?> Document Types</h2></center>
<?
if($action=='delete' && isset($id)) {
        echo '<center><h3>Delete: Are you sure?</h3>';
        echo '<h3><a href="types.php?first_item='.$first_item.'">No</a>&nbsp;
                <a href="types.php?first_item='.$first_item.'&delete='.$id.'">Yes</a></h3>';
        } 
elseif($action=='new') {
	?>
	<form action="types.php" method="post">
        <input type="hidden" name="save" value="new_type">
        <h3>Type properties</h3>
        Name<br>
        <input type="text" name="name" size="50" value="<? echo $row[2]; ?>"><br><br>
        Description<br>
        <input type="text" name="description" size="50" value="<? echo $row[4]; ?>" maxlength="128"><br><br>
        Module<br>
        <select name="template" size=1>
        <?
        $handle=opendir('../'.INC_PATH.CMODULES_PATH);
        while (false!==($file = readdir($handle))) {
        if ($file != "." && $file != ".." && '.inc' == substr($file, -4)) {
        $tok = strtok ($file,".");
        if($tok == $row[3]) { $selected = ' SELECTED'; } else { $selected = ''; }
        echo '<option value="'.$tok.'"'.$selected.'>'.$tok.'</option>';
        }
        }
        closedir($handle);
        ?>
        </select><br><br>
        Parent type<br>
        <select name="parent_id" size=1>
        <?
        $qres = sql_query("SELECT ".$db_table_prefix."types.id, parent_id, name, template, description
                        FROM ".$db_table_prefix."types, ".$db_table_prefix."typedata WHERE type_id = ".$db_table_prefix."types.id
                        AND platform='phpweb' ORDER BY $order_by");
        if(sql_num_rows($qres) > 0) {
                while($row2 = sql_fetch_row($qres)) {
                        if($row2[0] == $row[1]) { $selected = ' SELECTED'; } else { $selected = ''; }
                        echo '<option value="'.$row2[0].'"'.$selected.'>'.$row2[2].'</option>';
                        }
                sql_free_result($qres);
                }
        ?>
        </select><br><br>
	Visible<br>
	<select name="visible" size=1>
	<option value="0">---</option>
	<option value="1" SELECTED>YES</option>
	</select><br><br>
        <input type="submit" value="Save">
        </form>
	<br>
        <form action="types.php" method="get">
        <input type="submit" value="Back">
        </form>
	<?
	}
elseif($action=='edit' && isset($id)) {
	$qres = sql_query("SELECT ".$db_table_prefix."types.id, parent_id, name, template, description, visible
                        FROM ".$db_table_prefix."types, ".$db_table_prefix."typedata WHERE type_id = ".$db_table_prefix."types.id
                        AND platform='phpweb' AND ".$db_table_prefix."types.id=$id ORDER BY $order_by");
	if(sql_num_rows($qres) > 0) { 
	$row = sql_fetch_row($qres);
	sql_free_result($qres);
	?>
	<form action="types.php" method="post">
	<input type="hidden" name="save" value="type">
	<input type="hidden" name="save_id" value="<? echo $id; ?>">
	<h3>Type properties</h3>
	Name<br>
	<input type="text" name="name" size="50" value="<? echo $row[2]; ?>"><br><br>
	Description<br>
	<input type="text" name="description" size="50" value="<? echo $row[4]; ?>" maxlength="128"><br><br>
	Module<br>
	<select name="template" size=1>
	<?
	$handle=opendir('../'.INC_PATH.CMODULES_PATH);
	while (false!==($file = readdir($handle))) {
    	if ($file != "." && $file != ".." && '.inc' == substr($file, -4)) {
        $tok = strtok ($file,".");
        if($tok == $row[3]) { $selected = ' SELECTED'; } else { $selected = ''; }
	echo '<option value="'.$tok.'"'.$selected.'>'.$tok.'</option>';
    	}
	}
	closedir($handle);
	?>	
	</select><br><br>
	Parent type<br>
	<select name="parent_id" size=1>
	<?
	$qres = sql_query("SELECT ".$db_table_prefix."types.id, parent_id, name, template, description
                        FROM ".$db_table_prefix."types, ".$db_table_prefix."typedata WHERE type_id = ".$db_table_prefix."types.id
                        AND platform='phpweb' ORDER BY $order_by");
	if(sql_num_rows($qres) > 0) {
        	while($row2 = sql_fetch_row($qres)) {
			if($row2[0] == $row[1]) { $selected = ' SELECTED'; } else { $selected = ''; }
	                echo '<option value="'.$row2[0].'"'.$selected.'>'.$row2[2].'</option>';
                	}
        	sql_free_result($qres);
		}
	?>
	</select><br><br>
	Visible<br>
	<select name="visible" size=1>
	<option value="0"<? if(!$row[5]) echo ' SELECTED'; ?>>---</option>
	<option value="1"<? if($row[5]) echo ' SELECTED'; ?>>YES</option>
	</select><br><br>
	<input type="submit" value="Save">
	</form>
	<br>
	<form action="types.php" method="get">
	<input type="submit" value="Back">
        </form>
	<?
	}
	}
else {
$qres = sql_query("SELECT ".$db_table_prefix."types.id, parent_id, name, template, description, visible
			FROM ".$db_table_prefix."types, ".$db_table_prefix."typedata WHERE type_id = ".$db_table_prefix."types.id
			AND platform='phpweb' ORDER BY $order_by");
if(sql_num_rows($qres) > 0) {
        while($row = sql_fetch_row($qres)) {
		$doctype[$row[0]]['parent_id'] = $row[1];
		$doctype[$row[0]]['name'] = $row[2];
		$doctype[$row[0]]['template'] = $row[3];
		$doctype[$row[0]]['description'] = $row[4];
		$doctype[$row[0]]['visible'] = $row[5];
		}
	sql_free_result($qres);
	
?>
<table width="100%" border="0" cellpadding="2" cellspacing="1">
        <tr>
        <td bgcolor="#ddffdd" width="10%" align="center">
                <a href="types.php?first_item=<? echo $first_item; ?>&order_by=id"><b>ID</b></a></td>
        <td bgcolor="#ffdddd" width="20%">
                <a href="types.php?first_item=<? echo $first_item; ?>&order_by=name"><b>Name</b></a></td>
        <td bgcolor="#ffddff" width="25%">
                <a href="types.php?first_item=<? echo $first_item; ?>&order_by=description"><b>Description</b></a></td>
        <td bgcolor="#ffddff" width="9%">
                <a href="types.php?first_item=<? echo $first_item; ?>&order_by=parent_id"><b>Parent</b></a></td>
        <td bgcolor="#ffddff" width="9%">
                <a href="types.php?first_item=<? echo $first_item; ?>&order_by=visible"><b>Visible</b></a></td>
        <td bgcolor="#ddddff" width="17%">
                <a href="types.php?first_item=<? echo $first_item; ?>&order_by=template"><b>Module</b></a></td>
        <td bgcolor="#dddddd" width="10%" align="center">
                <b>Options</b></td>
        </tr>
        <?
	while(list($key, $value) = each($doctype)) {
		?>
		<tr>
		<td bgcolor="#eeffee" align="center"><? echo $key; ?></td>
		<td bgcolor="#ffeeee" align="left"><? echo $value['name']; ?></td>
		<td bgcolor="#ffeeff" align="left"><? echo $value['description']; ?></td>
		<td bgcolor="#ffeeff" align="left"><? echo $doctype[$value['parent_id']]['name']; ?></td>
		<td bgcolor="#ffeeff" align="center"><? if($value['visible']) { echo 'YES'; } else { echo '---'; } ?></td>
		<td bgcolor="#eeeeff" align="left"><? echo $value['template']; ?></td>
		<td bgcolor="#dddddd" align="center">
			<a href="types.php?id=<? echo $key; ?>&action=delete&first_item=<? echo $first_item; ?>"><img
                        src="img/delete.gif" border="0" alt="Delete"></a>&nbsp;
                	<a href="types.php?id=<? echo $key; ?>&action=edit&first_item=<? echo $first_item; ?>"><img
                        src="img/edit.gif" border="0" alt="Edit"></a></td>		
		</tr>
		<?
		} 
	echo '</table>';
	}
	?>
	<br>
	<center><a href="types.php?action=new">Add new document type</a></center>
	<?
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
