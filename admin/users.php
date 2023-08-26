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

foreach ( $_REQUEST  as $key => $val)
{
        $$key = $val;
}
$inc_value = 50;

$_BASEPATH = dirname(dirname($_SERVER['SCRIPT_FILENAME']));

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

if(isset($back) && $back = 'Back') {
	unset($action);
	unset($id);
	unset($save);
	}

if(isset($valid)) { $valid=1; } else { $valid=0; }
if(isset($editor)) { $editor=1; } else { $editor=0; }

if($save == 'new_userdata') {
        sql_query("INSERT ".TABLE_USERS." SET email='$email', password='$passwd2', valid=$valid");
	$id = sql_insert_id();
        }

if(!isset($first_item) || !is_numeric($first_item)) $first_item=0;
if(!isset($order_by) || $order_by == '') $order_by='id DESC';

if(isset($search)) { 
	$where = 'WHERE email LIKE "%'.$search.'%"'; 
	}
else {
	$where = ''; 
	}

if(isset($delete) && $delete > 1) {
	remove_user($delete);
	}

if(isset($id)) {
	define('SESSION_DBID', $id);
	}

if($save == 'userdata' && isset($id)) {
	sql_query("UPDATE ".TABLE_USERS." SET email='$email', password='$passwd2', valid=$valid WHERE id=$id"); 	
	set_profile('editor', $editor);
	}

if($save == 'userkey' && isset($id)) {
	if($prof_key) set_profile($prof_key, $prof_value);
	}

if($save == 'remove_userkey' && isset($id)) {
	if(isset($prof_key)) {
		remove_profile($prof_key);
		}
	}

if($save == 'groupmembership' && isset($id) && isset($group)) {
	$qprofile = sql_query ("SELECT value FROM ".TABLE_USERDATA." WHERE name='group_".$group."' AND user_id=".$id);
		if(sql_num_rows($qprofile) > 0) { $row = sql_fetch_row($qprofile); }
		if(sql_num_rows($qprofile) < 1 || $row[0] == '0') {
			add_user_to_group ($id, $group);
			}
		else {
			remove_user_from_group ($id, $group);
			}
	}

if(isset($id)) {

                // Read USER PROFILE from database
                $qprofile = sql_query ("SELECT name, value FROM ".TABLE_USERDATA." WHERE user_id=".$id);
                if(sql_num_rows($qprofile) > 0) {
	                while($prow = sql_fetch_row($qprofile)) {
        	                $profile[stripslashes($prow[0])] = stripslashes($prow[1]);
                                }
                        sql_free_result($qprofile);
                        }
	}


?>

<html>
<head>
<meta name="title" content="Users">
<meta name="description" content="Add/remove users and modify their profiles.">
<meta name="sort_order" content="1">
<title>
Liquid Bytes AWF User Management
</title>
<?php
	include('header.inc');
?>
<center>
<table width="90%" cellpadding="0" cellspacing="0" border="0">
<tr><td align="left">
<center><h2><?phpecho SITE_TITLE; ?> User Management</h2></center>
<?php
if($action=='delete' && isset($id) && $id < 2) {
	echo '<center><span style="color:#ff0000"><b>This user can\'t be removed.</b></span></center>';
	}
elseif($action=='delete' && isset($id)) {
	echo '<center><h3>Delete: Are you sure?</h3>';
	echo '<h3><a href="'.$_SERVER['PHP_SELF'].'?first_item='.$first_item.'">No</a>&nbsp;
		<a href="'.$_SERVER['PHP_SELF'].'?first_item='.$first_item.'&delete='.$id.'">Yes</a></h3>';
	}
elseif($action=='new') {
		echo '<h3>Add new user</h3><form action="'.$_SERVER['PHP_SELF'].'" method="post">';
                echo '<input type="hidden" name="action" value="edit">';
                echo '<input type="hidden" name="save" value="new_userdata">';
                echo '<table border="0" cellpadding="2" cellspacing="1"><tr><td>';
                echo '<b>eMail</b><br><input type="text" size="40" name="email"></td><td>';
                echo '<b>Password</b><br><input type="text" size="40" name="passwd2"></td></tr><tr><td>';
                echo '<br><input type="checkbox" name="valid" CHECKED> <b> Account is valid</b>';
                echo '</td><td></td></tr></table>';
                echo '<br><input type="submit" value="Add"></form><br><br><br>';
	}
elseif($action=='edit' && isset($id)) {
	$qres = sql_query("SELECT id, email, password, valid, views, last_login FROM ".TABLE_USERS." WHERE id=$id");
	if(sql_num_rows($qres) > 0) {
		$row = sql_fetch_row($qres);
		echo '<h3>Edit user data (UserID '.$row[0].')</h3><form action="'.$_SERVER['PHP_SELF'].'" method="get">';
		echo '<input type="hidden" name="action" value="edit">';
		echo '<input type="hidden" name="id" value="'.$id.'">';
		echo '<input type="hidden" name="save" value="userdata">';
		echo '<input type="hidden" name="first_item" value="'.$first_item.'">';
		echo '<input type="hidden" name="order_by" value="'.$order_by.'">';
		echo '<table border="0" cellpadding="2" cellspacing="1"><tr><td>';
		echo '<b>eMail</b><br><input type="text" size="40" name="email" value="'.$row[1].'"></td><td>';
		echo '<b>Password</b><br><input type="text" size="40" name="passwd2" value="'.$row[2].'"></td></tr><tr><td>';
		echo '<br><input type="checkbox" name="valid"';
		if($row[3] == 1) echo ' CHECKED';
		echo '> <b> Account is valid</b>';
		echo '&nbsp;&nbsp;&nbsp;&nbsp;<input type="checkbox" name="editor"';
		if($profile['editor'] == 1) echo ' CHECKED';
		echo '> <b>User is editor</b></td><td></td></tr></table>';
		echo '<br><input type="submit" value="Save">';
		echo '&nbsp;<input type="submit" name="back" value="Back"></form>';
		?>
		<form action="<?=$_SERVER['PHP_SELF']?>" method="get">
		<input type="hidden" name="action" value="edit">
		<input type="hidden" name="id" value="<?=$id?>">
		<input type="hidden" name="save" value="groupmembership">
		<input type="hidden" name="first_item" value="<?=$first_item?>">
		<input type="hidden" name="order_by" value="<?=$order_by?>">
		<b>Group memberships</b><br />
		<select name="group" size="5">
		<?php foreach ( $groups as $key => $value) { ?>
		<option value="<?=$key?>"><?=$value?><?phpif($profile['group_'.$key] == 1) echo ' [Member]'; ?></option>
		<?php } ?>
		</select><br />
		<input type="submit" value="Add / Remove">
		</form>
		<b>Profile</b>
		<?php

		if(isset($profile)) {
		echo '<table width="450" border="0" cellpadding="2" cellspacing="1">';
		echo '<tr><td width="100" bgcolor="#dddddd">Key</td>
			<td width="300" bgcolor="#dddddd" align="right">Value</td>
			<td width="50" bgcolor="#dddddd">Edit</td></tr>';
		foreach ($profile as $key => $value) {
			echo '<tr><td width="100" bgcolor="#eeeeee">'.$key.'</td>
				<td width="300" bgcolor="#eeeeee" align="right">'.$value.'</td>
				<td width="50" bgcolor="#eeeeee">
				<a href="'.$_SERVER['PHP_SELF'].'?action=edit&id='.$id.'&save=remove_userkey&prof_key='.$key.'">Remove</a></td></tr>';
			}
		echo '</table>';
		}
		echo '<form action="'.$_SERVER['PHP_SELF'].'" method="get"><br>Key<br><input type=text name="prof_key" size="70"><br>
			<input type="hidden" name="action" value="edit">
                	<input type="hidden" name="id" value="'.$id.'">
                	<input type="hidden" name="save" value="userkey">
			<br>Value<br><input type=text name="prof_value" size="70"><br><br>';
		echo '<input type="submit" value="Add / Update"></form><br>';
		sql_free_result($qres);			
		}
	}
else {
	?>
	<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
	<?php
	$qres = sql_query("SELECT count(*) FROM ".TABLE_USERS." $where");
	$row  = sql_fetch_row($qres);
        $total_count = $row[0];
	sql_free_result($qres);
	$qres = sql_query("SELECT id, email, password, valid, views, last_login FROM ".TABLE_USERS." $where ORDER BY $order_by LIMIT $first_item, $inc_value");
        echo '<center>';
        echo '<input type=hidden name="order_by" value="'.$order_by.'">';
        echo '<input type=text name="search" size="40"><input type=submit value="Search"></form><br><br>';
	if(sql_num_rows($qres) > 0) { ?>
	<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
        <input type=hidden name="order_by" value="<?=$order_by?>">
        <input type=hidden name="search" value="<?=$search?>">
        <input type=hidden name="save_valid" value="true">
        <input type=hidden name="first_item" value="<?=$first_item?>">
	<table width="100%" border="0" cellpadding="2" cellspacing="1">
        <tr>
        <td bgcolor="#ddffdd" width="7%" align="center">
                <a href="<?=$_SERVER['PHP_SELF']?>?first_item=<?php echo $first_item; ?>&order_by=id%20DESC"><b>ID</b></a></td>
        <td bgcolor="#ffdddd" width="61%">
                <a href="<?=$_SERVER['PHP_SELF']?>?first_item=<?php echo $first_item; ?>&order_by=email"><b>eMail</b></a></td>
<?php
/*
	<td bgcolor="#ffdddd" width="20%">
                <a href="<?=$_SERVER['PHP_SELF']?>?first_item=<?php echo $first_item; ?>&order_by=password"><b>Password</b></a></td>
*/
?>
	<td bgcolor="#ddddff" width="6%" align="center">
                <a href="<?=$_SERVER['PHP_SELF']?>?first_item=<?php echo $first_item; ?>&order_by=valid"><b>Valid</b></a></td>
        <td bgcolor="#ddddff" width="6%" align="right">
                <a href="<?=$_SERVER['PHP_SELF']?>?first_item=<?php echo $first_item; ?>&order_by=views%20DESC"><b>Views</b></a></td>
        <td bgcolor="#ffddff" width="10%" align="right">
                <a href="<?=$_SERVER['PHP_SELF']?>?first_item=<?php echo $first_item; ?>&order_by=last_login"><b>Last Login</b></a></td>
        <td bgcolor="#dddddd" width="10%" align="center"><b>Options</b></td>
        </tr>
		<?php
		while($row = sql_fetch_row($qres)) {
			if($save_valid == 'true' && $user_valid[$row[0]] != '1') $user_valid[$row[0]] = '0';
			if($save_valid == 'true' && $row[3] != $user_valid[$row[0]]) {
				sql_query("UPDATE ".TABLE_USERS." SET valid = ".$user_valid[$row[0]]." WHERE id = ".$row[0]);
				$row[3] = $user_valid[$row[0]];
				}
			if($row[3]==1) { $row[3] = '<input name="user_valid['.$row[0].']" value="1" type="checkbox" checked>'; } 
			else { $row[3] = '<input value="1" name="user_valid['.$row[0].']" type="checkbox">'; } 
			?>
	<tr>
        <td bgcolor="#eeffee" width="7%" align="center"><?php echo $row[0]; ?></td>
        <td bgcolor="#ffeeee" width="61%"><?php echo $row[1]; ?></td>
<?php
/*
        <td bgcolor="#ffeeee" width="20%"><?php echo $row[2]; ?></td>
*/
?>
        <td bgcolor="#eeeeff" width="6%" align="center"><?php echo $row[3]; ?></td>
        <td bgcolor="#eeeeff" width="6%" align="right"><?php echo $row[4]; ?></td>
        <td bgcolor="#ffeeff" width="10%" align="right"><?php echo $row[5]; ?></td>
        <td bgcolor="#eeeeee" width="10%" align="center">
		<a href="<?=$_SERVER['PHP_SELF']?>?id=<?php echo $row[0]; ?>&action=delete&first_item=<?php echo $first_item; ?>"><img
			src="img/delete.gif" border="0" alt="Delete"></a>&nbsp;
		<a href="<?=$_SERVER['PHP_SELF']?>?id=<?php echo $row[0]; ?>&action=edit&first_item=<?php echo $first_item; ?>"><img
			src="img/edit.gif" border="0" alt="Edit"></a></td>
        </tr>
			<?php
                	}
	?>
	<tr>
        <td bgcolor="#ffffff" width="7%" align="center"></td>
        <td bgcolor="#ffffff" width="61%"></td>
<?php
/*
        <td bgcolor="#ffffff" width="20%"></td>
*/
?>
        <td bgcolor="#eeeeff" width="6%" align="center"><input type="submit" value="Save"></td>
        <td bgcolor="#ffffff" width="6%" align="right"></td>
        <td bgcolor="#ffffff" width="10%" align="right"></td>
        <td bgcolor="#ffffff" width="10%" align="center"></td>
        </tr>
	</table></form><center>
	<?php
	if($total_count > $inc_value) {
           echo '<br />Page ';
           $temp_counter = $inc_value;
           while($inc_value + $total_count > $temp_counter) {
            ++$news_counter;
            if($first_item == ($temp_counter - $inc_value)) { echo '<b>'; }
            echo '<a href="'.$_SERVER['PHP_SELF'].'?first_item='.($temp_counter - $inc_value).'">'.$news_counter.'</a> ';
            if($first_item == ($temp_counter - $inc_value)) { echo '</b>'; }
            $temp_counter += $inc_value;
            if($news_counter >= 100) { break; }
            }
           if($total_count > $first_item + $inc_value) {
            echo '&nbsp;<a href="'.$_SERVER['PHP_SELF'].'?first_item='.($first_item + $inc_value).'">Next</a>';
            }
           echo '<br />';
	   }
	 }
	echo '<br /><a href="'.$_SERVER['PHP_SELF'].'?action=new">Add new user</a>';
	echo '<p>There are '.$total_count.' users in our database.';
	echo '</center></form>';
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
