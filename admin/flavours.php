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

// Save settings in DB
if(isset($delete)) {
	sql_query("DELETE FROM ".TABLE_FLAVOURS." WHERE id=$save");
	}
elseif(isset($save) && is_numeric($newid)) {
	sql_query("UPDATE ".TABLE_FLAVOURS." SET id=$newid, name='".addslashes($name)."', lang='$lang', published=$published WHERE id=$save");
	}

// Create new flavour
elseif(isset($new)) {
	sql_query("INSERT INTO ".TABLE_FLAVOURS." (name) VALUES ('Unknown')");
	}

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
	while ($row = sql_fetch_row($qresult)) {
        	define(strtoupper($row[0]),$row[1]);
                }
	sql_free_result($qresult);
	}
?>

<html>
<head>
<meta name="title" content="Flavours">
<meta name="description" content="Allows you to add/remove flavours of your website.">
<meta name="sort_order" content="7">
<title>
Liquid Bytes AWF Flavours
</title>
<?php
        include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><?php echo SITE_TITLE; ?> Flavours</h2></center>
<br>
<table width="100%" cellpadding="2" cellspacing="1" border="0">
<tr>
<td style="font-weight: bold" width="7%" bgcolor="#ddffdd" align="center">ID</td>
<td style="font-weight: bold" width="33%" bgcolor="#ffdddd" align="left">Name</td>
<td style="font-weight: bold" width="20%" bgcolor="#ddddff" align="right">Language</td>
<td style="font-weight: bold" width="20%" bgcolor="#dddddd" align="center">Published</td>
<td width="20%" bgcolor="#dddddd" align="right">&nbsp;</td>
</tr>
<?php
$qres = sql_query("SELECT id, name, lang, published FROM ".TABLE_FLAVOURS." ORDER BY id");
while($row = sql_fetch_row($qres)) { ?>
	<form action="flavours.php" method="post">
	<tr>
	<td bgcolor="#eeffee" align="center"><input type="text" name="newid" value="<?=$row[0]?>" size="4" /></td>
	<td bgcolor="#ffeeee" align="left"><input type="text" name="name" value="<?=htmlentities(stripslashes($row[1]))?>"size="40" /></td>
	<td bgcolor="#eeeeff" align="right">
	<select name="lang" size="1">
        <?php
        $handle=opendir('../inc/lang');
        while (false!==($file = readdir($handle))) {
                if ($file != "." && $file != "..") {
                        if($file == $row[2]) { $selected = ' selected="selected"'; } else { $selected = ''; }
                        echo '<option value="'.$file.'"'.$selected.'>'.$file.'</option>';
                        }
                }
        closedir($handle);
        ?>
	</td>
	<td bgcolor="#eeeeee" align="center">
	<select name="published" size="1">
	<option value="1"<?php if($row[3] == '1') echo ' selected="selected"'; ?>>Yes</option>
	<option value="0"<?php if($row[3] == '0') echo ' selected="selected"'; ?>>No</option>
	</select>
	</td>
	<td bgcolor="#eeeeee" align="center" nowrap>
	<input type="hidden" name="save" value="<?=$row[0]?>">
	<input type="submit" name="delete"value="Delete">
	<input type="submit" value="Save">
	</td>
	</tr>
	</form>
	<?php
	}
?>
</table>
<br />
<br />
<center>
<a href="flavours.php?new=true">Add new flavour</a>
</td></tr>
</table>
<?php
	include('footer.inc');
?>
</center>
</body>
</html>
