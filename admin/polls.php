<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

error_reporting(1); // Disable Warnings

foreach ($_REQUEST as $key => $val= {
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

if(!isset($order_by)) { $order_by='sortorder'; }

if(isset($delete)) {
        sql_query("DELETE FROM ".TABLE_POLLS_ITEMS." WHERE poll_id=$delete");
	sql_query("DELETE FROM ".TABLE_POLLS." WHERE id=$delete");
        }

if(isset($removeanswer)) {
	sql_query("DELETE FROM ".TABLE_POLLS_ITEMS." WHERE id=$removeanswer");
	}

if(isset($save)){
	if($save == 'poll') {
		if(!isset($active) || $active != 1) $active = 0;
        	sql_query("UPDATE ".TABLE_POLLS." SET sortorder = $sortorder, question = '$question', active = $active , module_id=$module WHERE id=$save_id");
	        }

	if($save == 'new_poll') {
		if(!isset($active) || $active != 1) $active = 0;
		sql_query("INSERT ".TABLE_POLLS." (sortorder, question, active, module_id) VALUES ($sortorder, '$question', $active, $module)");
		$id = sql_insert_id();
        	}

	if($save == 'addanswer') {
		if(!isset($ans_counter) || $ans_counter < 1) $ans_counter = '0';
		sql_query("INSERT ".TABLE_POLLS_ITEMS." SET poll_id=$id, text='$answer', counter=$ans_counter");	
		}
	}
?>

<html>
<head>
<meta name="title" content="Polls">
<meta name="description" content="Add/remove polls.">
<meta name="sort_order" content="8">
<title>
Liquid Bytes AWF Polls Config
</title>
<?php
        include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><?php echo SITE_TITLE; ?> Polls Config</h2></center>
<?php
if($action=='delete' && isset($id)) {
        echo '<center><h3>Delete: Are you sure?</h3>';
        echo '<h3><a href="polls.php?first_item='.$first_item.'">No</a>&nbsp;
                <a href="polls.php?first_item='.$first_item.'&delete='.$id.'">Yes</a></h3>';
        } 
elseif($action=='new') {
	$pres = sql_query("SELECT max(sortorder) FROM ".TABLE_POLLS);
        $poll_max_sortorder = sql_fetch_row($pres);
        sql_free_result($pres);
	?>
	<form action="polls.php" method="post">
	<input type="hidden" name="save" value="new_poll">
	<input type="hidden" name="action" value="edit">
	<h3>Add new poll</h3>
	Question<br>
	<input type="text" name="question" maxlength="128" size="64"><br><br>
	<input type="checkbox" name="active" value="1" CHECKED>Active<br><br>
	Sort order<br>
	<input type="text" name="sortorder" value="<?php echo($poll_max_sortorder[0] + 1); ?>"><br><br>
	Target<br />
	<select name="module">
	<option value="0">All modules</option>
	<?php
	$mres = sql_query("SELECT id, caption FROM ".TABLE_MODULES." WHERE name='polls.inc'");
	while($mrow = sql_fetch_row($mres)) { ?>
		<option value="<?=$mrow[0]?>"><?=$mrow[1].' (ID '.$mrow[0].')'?></option>
		<?php
		}
	?>
	</select><br /><br />
	<input type="submit" value="Save">
	</form>
	<br>
	<form action="polls.php" method="get">
	<input type="submit" value="Back">
        </form>
	<?php
	}
elseif($action=='edit' && isset($id)) {
	$qres = sql_query("SELECT id, question, active, sortorder, last_access, module_id FROM ".TABLE_POLLS." WHERE id=$id");
	if(sql_num_rows($qres) > 0) { 
	$row = sql_fetch_row($qres);
	sql_free_result($qres);
	?>
	<form action="polls.php" method="post">
	<input type="hidden" name="save" value="poll">
	<input type="hidden" name="action" value="edit">
	<input type="hidden" name="save_id" value="<?php echo $id; ?>">
	<input type="hidden" name="id" value="<?php echo $id; ?>">
	<h3>Edit poll</h3>
	Question<br>
	<input type="text" name="question" maxlength="128" size="64" value="<?php echo $row[1]; ?>" /><br><br>
	<input type="checkbox" name="active" value="1"<?php if($row[2] == '1') echo ' CHECKED'; ?>>Active<br><br>
	Sort order<br />
	<input type="text" name="sortorder" value="<?php echo $row[3]; ?>" /><br /><br />
	Target<br />
	<select name="module">
	<option value="0">All modules</option>
	<?php
	$mres = sql_query("SELECT id, caption FROM ".TABLE_MODULES." WHERE name='polls.inc'");
	while($mrow = sql_fetch_row($mres)) { ?>
		<option value="<?=$mrow[0]?>"<?php if($row[5] == $mrow[0]) echo ' selected'; ?>><?=$mrow[1].' (ID '.$mrow[0].')'?></option>
		<?php
		}
	?>
	</select><br /><br />
	<input type="submit" value="Save">
	</form>
	<form action="polls.php" method="get">
	<input type="submit" value="Back">
        </form>
	<?php
	}
	?>
	<br><br>
        <table width="450" border="0" cellpadding="2" cellspacing="1">
	<tr><td width="300" bgcolor="#dddddd">Answer</td>
        <td width="100" bgcolor="#dddddd" align="right">Counter</td>
        <td width="50" bgcolor="#dddddd">Edit</td></tr>
	<?php
	$qres = sql_query("SELECT id, text, counter FROM ".TABLE_POLLS_ITEMS." WHERE poll_id=$id ORDER BY text"); 
	if(sql_num_rows($qres) > 0) {
	while($row = sql_fetch_row($qres)) {	
		echo '<tr><td bgcolor="#eeeeee">'.$row[1].'</td><td bgcolor="#eeeeee" align="right">'.$row[2].'</td><td
bgcolor="#eeeeee"><a href="polls.php?action=edit&id='.$id.'&removeanswer='.$row[0].'">Remove</a></td></tr>';
		}
	sql_free_result($qres);
	}
	echo '</table>';
        echo '<form action="polls.php" method="get">
		<br>Answer<br><input type=text name="answer" size="70"><br>
                <input type="hidden" name="action" value="edit">
                <input type="hidden" name="id" value="'.$id.'">
                <input type="hidden" name="save" value="addanswer">
                <br>Counter (optional)<br><input type=text name="ans_counter" size="70"><br><br>';
        echo '<input type="submit" value="Add"></form><br>';
	}
else {

$qres = sql_query("SELECT id, question, active, sortorder, last_access FROM ".TABLE_POLLS." ORDER BY $order_by");

?>
<table width="100%" border="0" cellpadding="2" cellspacing="1">
        <tr>
        <td bgcolor="#ddffdd" width="10%" align="center">
                <a href="polls.php?first_item=<?php echo $first_item; ?>&order_by=sortorder"><b>Sort order</b></a></td>
        <td bgcolor="#ffdddd" width="50%">
                <a href="polls.php?first_item=<?php echo $first_item; ?>&order_by=question"><b>Question</b></a></td>
        <td bgcolor="#ffffdd" width="10%">
                <a href="polls.php?first_item=<?php echo $first_item; ?>&order_by=active"><b>Active</b></a></td>
        <td bgcolor="#ddddff" width="10%">
                <a href="polls.php?first_item=<?php echo $first_item; ?>&order_by=last_access"><b>Last change</b></a></td>
        <td bgcolor="#ddddff" width="10%" align="right">
                <b>Answers</b></td>
        <td bgcolor="#dddddd" width="10%" align="center">
                <b>Options</b></td>
        </tr>
        <?php
	if(sql_num_rows($qres) > 0) {
		while($row = sql_fetch_row($qres)) {
			$cres = sql_query("SELECT count(*) FROM ".TABLE_POLLS_ITEMS." WHERE poll_id=$row[0]");
			$ans_counter = sql_fetch_row($cres);
			sql_free_result($cres); 
			if($row[2] == '1') { $row[2] = 'YES'; } else { $row[2] = '---'; } 
			?>
			<tr>
			<td bgcolor="#eeffee" align="center"><?php echo $row[3]; ?></td>
			<td bgcolor="#ffeeee" align="left"><?php echo $row[1]; ?></td>
			<td bgcolor="#ffffee" align="left"><?php echo $row[2]; ?></td>
			<td bgcolor="#eeeeff" align="left"><?php echo $row[4]; ?></td>
			<td bgcolor="#eeeeff" align="right"><?php echo $ans_counter[0]; ?></td>
			<td bgcolor="#dddddd" align="center">
			<a href="polls.php?id=<?php echo $row[0]; ?>&action=delete&first_item=<?php echo $first_item; ?>"><img
                        src="img/delete.gif" border="0" alt="Delete"></a>&nbsp;
                	<a href="polls.php?id=<?php echo $row[0]; ?>&action=edit&first_item=<?php echo $first_item; ?>"><img
                        src="img/edit.gif" border="0" alt="Edit"></a></td>		
			</tr>
			<?php
			} 
		sql_free_result($qres);
		}
	echo '</table>';
	
?>
<br>
<center><a href="polls.php?action=new">Add new poll</a></center>
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
