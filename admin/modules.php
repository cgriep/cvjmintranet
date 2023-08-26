<?php
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
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
	while ($row = sql_fetch_row($qresult)) {
        	define(strtoupper($row[0]),$row[1]);
                }
	sql_free_result($qresult);
	}

if(!isset($order_by)) { $order_by='visible DESC, placement, sort_order'; }

if(isset($delete)) {
        sql_query("DELETE FROM ".TABLE_MODULES." WHERE id=$delete");
        }

if($save == 'module') {
        sql_query("UPDATE ".TABLE_MODULES." SET name='$name', placement='$position', flavour_id=$flavour_id,
			section_id=$section_id, document_id=$document_id, visible=$visible, removeable=$removeable,
			sort_order = $sort_order, target = $target, caption = '".addslashes($caption)."' WHERE id=$save_id");
        }

if($save == 'new_module') {
        sql_query("INSERT ".TABLE_MODULES." SET name='$name', placement='$position', flavour_id=$flavour_id,
			section_id=$section_id, document_id=$document_id, visible=$visible, removeable=$removeable,
			sort_order = $sort_order, target = $target, caption = '".addslashes($caption)."'"); 
	// $id = sql_insert_id();
        }

?>

<html>
<head>
<meta name="title" content="Modules">
<meta name="description" content="Add/remove modules.">
<meta name="sort_order" content="5">
<title>
Liquid Bytes AWF Module Config
</title>
<?php
        include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><?php echo SITE_TITLE; ?> Module Config</h2></center>
<?php
if($action=='delete' && isset($id)) {
        echo '<center><h3>Delete: Are you sure?</h3>';
        echo '<h3><a href="modules.php?first_item='.$first_item.'">No</a>&nbsp;
                <a href="modules.php?first_item='.$first_item.'&delete='.$id.'">Yes</a></h3>';
        } 
elseif($action=='new') {
	?>
	<form action="modules.php" method="post">
	<input type="hidden" name="save" value="new_module">
	<h3>Module properties</h3>
	<table width="700" border="0" cellpadding="2" cellspacing="1"><tr><td>
	Name<br>
	<select name="name" size=1>
        <?php
        $handle=opendir('../'.INC_PATH.MODULE_PATH);
        while (false!==($file = readdir($handle))) {
        if ($file != "." && $file != ".." && '.inc' == substr($file, -4)) {
        echo '<option value="'.$file.'">'.$file.'</option>';
        }
        }
        closedir($handle);
        ?>
        </select>
	</td><td>
	Position<br>
	<select name="position" size=1>
	<option value="left" SELECTED>left</option>
	<option value="right">right</option>
	</select>
	</td><td>
	Visible<br>
	<select name="visible" size=1>
        <option value="0">No</option>
        <option value="1" SELECTED>Yes</option>
        </select>
	</td><td>
	Removeable<br>
	<select name="removeable" size=1>
        <option value="0">No</option>
        <option value="1" SELECTED>Yes</option>
        </select>
	</td><td>
	Target<br>
	<select name="target" size=1>
        <option value="0" SELECTED>---</option>
        <option value="1">PUBLIC</option>
        <option value="2">USERS</option>
        <option value="3"<?php if($row[9] == '3') echo 'SELECTED'; ?>>ADMIN</option>
        </select>
	</td>
	<tr>
	<td>
	Caption<br>
	<input type="text" name="caption" size="10" value="">
	</td><td>
	FlavourID (-1 for n/a)<br>
	<input type="text" name="flavour_id" size="10" value="-1">
	</td><td>
	SectionID (-1 for n/a)<br>
	<input type="text" name="section_id" size="10" value="-1" maxlength="10">
	</td><td>
	DocumentID (-1 for n/a)<br>
	<input type="text" name="document_id" size="10" value="-1" maxlength="10">
	</td><td>
	Sort order<br>
	<input type="text" name="sort_order" size="10" value="1" maxlength="10"><br><br>
	</td><td>&nbsp;
	</td></tr></table>
	<input type="submit" value="Save">
	</form>
	<br>
	<form action="modules.php" method="get">
	<input type="submit" value="Back">
        </form>
	<?php
	}
elseif($action=='edit' && isset($id)) {
$qres = sql_query("SELECT id, sort_order, name, placement, flavour_id, section_id, document_id, visible, removeable, 
target,
	caption FROM ".TABLE_MODULES." WHERE id=$id ORDER BY $order_by");
	if(sql_num_rows($qres) > 0) { 
	$row = sql_fetch_row($qres);
	sql_free_result($qres);
	?>
	<form action="modules.php" method="post">
	<input type="hidden" name="save" value="module">
	<input type="hidden" name="save_id" value="<?php echo $id; ?>">
	<h3>Module properties</h3>
	<table width="700" border="0" cellpadding="2" cellspacing="1"><tr><td>
	Name<br>
	<select name="name" size=1>
        <?php
        $handle=opendir('../'.INC_PATH.MODULE_PATH);
        while (false!==($file = readdir($handle))) {
        if ($file != "." && $file != ".." && '.inc' == substr($file, -4)) {
        if($file == $row[2]) { $selected = ' SELECTED'; } else { $selected = ''; }
        echo '<option value="'.$file.'"'.$selected.'>'.$file.'</option>';
        }
        }
        closedir($handle);
        ?>
        </select>
	</td><td>
	Position<br>
	<select name="position" size=1>
	<option value="left"<?php if($row[3] == 'left') echo 'SELECTED'; ?>>left</option>
	<option value="right"<?php if($row[3] == 'right') echo 'SELECTED'; ?>>right</option>
	</select>
	</td><td>
	Visible<br>
	<select name="visible" size=1>
        <option value="0"<?php if($row[7] == '0') echo 'SELECTED'; ?>>No</option>
        <option value="1"<?php if($row[7] == '1') echo 'SELECTED'; ?>>Yes</option>
        </select>
	</td><td>
	Removeable<br>
	<select name="removeable" size=1>
        <option value="0"<?php if($row[8] == '0') echo 'SELECTED'; ?>>No</option>
        <option value="1"<?php if($row[8] == '1') echo 'SELECTED'; ?>>Yes</option>
        </select>
	</td><td>
	Target<br>
        <select name="target" size=1>
        <option value="0"<?php if($row[9] == '0') echo 'SELECTED'; ?>>---</option>
        <option value="1"<?php if($row[9] == '1') echo 'SELECTED'; ?>>PUBLIC</option>
        <option value="2"<?php if($row[9] == '2') echo 'SELECTED'; ?>>USERS</option>
        <option value="3"<?php if($row[9] == '3') echo 'SELECTED'; ?>>ADMIN</option>
        </select>
	</td>
	</tr>
	<tr>
	<td>
	Caption<br>
	<input type="text" name="caption" size="10" value="<?php echo $row[10]; ?>">
	</td><td>
	FlavourID (-1 for n/a)<br>
	<input type="text" name="flavour_id" size="10" value="<?php echo $row[4]; ?>">
	</td><td>
	SectionID (-1 for n/a)<br>
	<input type="text" name="section_id" size="10" value="<?php echo $row[5]; ?>" maxlength="10">
	</td><td>
	DocumentID (-1 for n/a)<br>
	<input type="text" name="document_id" size="10" value="<?php echo $row[6]; ?>" maxlength="10">
	</td><td>
	Sort order<br>
	<input type="text" name="sort_order" size="10" value="<?php echo $row[1]; ?>" maxlength="10"><br><br>
	</td><td>&nbsp;
	</td></tr></table>
	<input type="submit" value="Save">
	</form>
	<form action="modules.php" method="get">
	<input type="submit" value="Back">
        </form>
	<?php
	}
	}
else {
$qres = sql_query("SELECT id, sort_order, name, placement, flavour_id, section_id, document_id, visible, removeable, 
	target, caption FROM ".TABLE_MODULES." ORDER BY $order_by");

if(sql_num_rows($qres) > 0) {
        while($row = sql_fetch_row($qres)) {
		$modules[$row[0]]['sort_order'] = $row[1];
		$modules[$row[0]]['name'] = $row[2];
		$modules[$row[0]]['position'] = $row[3];
		if($row[4]=='-1') $row[4] = '---';
		$modules[$row[0]]['flavour_id'] = $row[4];
		if($row[5]=='-1') $row[5] = '---';
		$modules[$row[0]]['section_id'] = $row[5];
		if($row[6]=='-1') $row[6] = '---';
		$modules[$row[0]]['document_id'] = $row[6];
		if($row[7]=='0') $row[7] = '---';
		if($row[7]=='1') $row[7] = 'YES';
		$modules[$row[0]]['visible'] = $row[7];
		if($row[8]=='0') $row[8] = '---';
		if($row[8]=='1') $row[8] = 'YES';
		$modules[$row[0]]['removeable'] = $row[8];
		if($row[9]=='0') $row[9] = '---';
		if($row[9]=='1') $row[9] = 'PUBLIC';
		if($row[9]=='2') $row[9] = 'USERS';
		if($row[9]=='3') $row[9] = 'ADMIN';
		$modules[$row[0]]['condition'] = $row[9];
		$modules[$row[0]]['caption']   = $row[10].'&nbsp';
		}
	sql_free_result($qres);
	
?>
<table width="100%" border="0" cellpadding="2" cellspacing="1">
        <tr>
        <td bgcolor="#ddffdd" width="3%" align="center">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=id"><b>ID</b></a></td>
        <td bgcolor="#ddffdd" width="7%" align="center" nowrap="nowrap">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=sort_order"><b>Sort order</b></a></td>
        <td bgcolor="#ffdddd" width="10%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=caption"><b>Caption</b></a></td>
        <td bgcolor="#ffdddd" width="10%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=name"><b>Name</b></a></td>
        <td bgcolor="#ffffdd" width="10%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=placement"><b>Position</b></a></td>
        <td bgcolor="#ddddff" width="10%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=flavour_id"><b>FlavourID</b></a></td>
        <td bgcolor="#ddddff" width="10%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=section_id"><b>SectionID</b></a></td>
        <td bgcolor="#ddddff" width="10%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=document_id"><b>DocumentID</b></a></td>
        <td bgcolor="#ffddff" width="6%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=visible"><b>Visible</b></a></td>
        <td bgcolor="#ffddff" width="6%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=removeable"><b>Removeable</b></a></td>
        <td bgcolor="#ffddff" width="8%">
                <a href="modules.php?first_item=<?php echo $first_item; ?>&order_by=target"><b>Target</b></a></td>
        <td bgcolor="#dddddd" width="10%" align="center">
                <b>Options</b></td>
        </tr>
        <?php
	foreach ( $modules as $key => $value) {
		?>
		<tr>
		<td bgcolor="#eeffee" align="center"><?=$key?></td>
		<td bgcolor="#eeffee" align="center"><?=$value['sort_order']?></td>
		<td bgcolor="#ffeeee" align="left"><?=$value['caption']?></td>
		<td bgcolor="#ffeeee" align="left"><?=$value['name']?></td>
		<td bgcolor="#ffffee" align="left"><?=$value['position']?></td>
		<td bgcolor="#eeeeff" align="left"><?=$value['flavour_id']?></td>
		<td bgcolor="#eeeeff" align="left"><?=$value['section_id']?></td>
		<td bgcolor="#eeeeff" align="left"><?=$value['document_id']?></td>
		<td bgcolor="#ffeeff" align="left"><?=$value['visible']?></td>
		<td bgcolor="#ffeeff" align="left"><?=$value['removeable']?></td>
		<td bgcolor="#ffeeff" align="left"><?=$value['condition']?></td>
		<td bgcolor="#dddddd" align="center">
			<a href="modules.php?id=<?php echo $key; ?>&action=delete&first_item=<?php echo $first_item; ?>"><img
                        src="img/delete.gif" border="0" alt="Delete"></a>&nbsp;
                	<a href="modules.php?id=<?php echo $key; ?>&action=edit&first_item=<?php echo $first_item; ?>"><img
                        src="img/edit.gif" border="0" alt="Edit"></a></td>		
		</tr>
		<?php
		} 
	echo '</table>';
	}
	?>
	<br>
	<center><a href="modules.php?action=new">Add new module</a></center>
	<?php
	}
?>
</td></tr>
</table>
<?php
	include('footer.inc');
?>
</center>
</body>
</html>
