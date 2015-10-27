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
require($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
        while ($row = sql_fetch_row($qresult)) {
                define(strtoupper($row[0]),$row[1]);
                }
        sql_free_result($qresult);
        }

if(isset($save_changes)) {
	if(!is_numeric($views)) $views = 0;
	if(!is_numeric($counter)) $counter = 0;
	sql_query("UPDATE ".TABLE_ADS." SET host='$host', destination='$destination', counter=$counter, views=$views, url='$url', banner='$banner' WHERE id=$save_changes");
	}

if(isset($create_new)) {
	if(!is_numeric($views)) $views = 0;
	if(!is_numeric($counter)) $counter = 0;
	sql_query("INSERT INTO ".TABLE_ADS." SET host='$host', destination='$destination', counter=$counter, views=$views, url='$url', banner='$banner'");
	}

if(isset($delete2)) {
        sql_query("DELETE FROM ".TABLE_ADS." WHERE id=$delete2");
        }
?>
<html>
<head>
<meta name="title" content="Ads">
<meta name="description" content="Simple banner management.">
<meta name="sort_order" content="4">
<title>
Liquid Bytes AWF Banner Management
</title>
<?
        include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><? echo SITE_TITLE; ?> Banner Management</h2></center>
<? if(isset($id)) { 
$queryres = sql_query("SELECT host, destination, counter, views, url, banner FROM ".TABLE_ADS." WHERE id=$id");
$row = sql_fetch_row($queryres);
sql_free_result($queryres);
?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="save_changes" value="<?=$id?>" />
Host
<br />
<input type="text" name="host" value="<?=$row[0]?>" size="50" />
<br />
<br />
Host URL (optional)<br />
<input type="text" name="url" value="<?=$row[4]?>" size="50" />
<br />
<br />
Destination URL<br />
<input type="text" name="destination" value="<?=$row[1]?>" size="50" />
<br />
<br />
Clicks<br />
<input type="text" name="counter" value="<?=$row[2]?>" size="50" />
<br />
<br />
Views<br />
<input type="text" name="views" value="<?=$row[3]?>" size="50" />
<br />
<br />
Filename<br />
<input type="text" name="banner" value="<?=$row[5]?>" size="50" />
<br />
<br />
<input type="submit" value="Save">
</form>
<? } elseif(isset($delete)) { ?>
<center><h3>Delete: Are you sure?</h3>
<h3><a href="<?=$_SERVER['PHP_SELF']?>">No</a>&nbsp;<a href="<?=$_SERVER['PHP_SELF']?>?delete2=<?=$delete?>">Yes</a></h3>
<? } elseif(isset($new)) { ?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="create_new" value="true" />
Host
<br />
<input type="text" name="host" value="<?=$host?>" size="50" />
<br />
<br />
Host URL (optional)<br />
<input type="text" name="url" value="<?=$url?>" size="50" />
<br />
<br />
Destination URL<br />
<input type="text" name="destination" value="<?=$destination?>" size="50" />
<br />
<br />
Clicks<br />
<input type="text" name="counter" value="<?=$counter?>" size="50" />
<br />
<br />
Views<br />
<input type="text" name="views" value="<?=$views?>" size="50" />
<br />
<br />
Filename<br />
<input type="text" name="banner" value="<?=$banner?>" size="50" />
<br />
<br />
<input type="submit" value="Save">
</form>
<? } else { ?>
<table width="100%" border="0" cellpadding="2" cellspacing="1">
<TR><TD WIDTH=20% bgcolor="#ddddff"><B>Host</B></TD>
<TD WIDTH=30% bgcolor="#ddffdd"><B>Destination URL</B></TD>
<TD WIDTH=20% bgcolor="#ffdddd"><B>Banner</B></TD>
<TD WIDTH=10% bgcolor="#dddddd" align="right"><B>Clicks</B></TD>
<TD WIDTH=10% bgcolor="#dddddd" align="right"><B>Views</B></TD>
<TD WIDTH=10% bgcolor="#dddddd" align="center"><B>Options</B></TD></TR>
<?
  $queryres = sql_query("SELECT host, destination, counter, views, url, banner, id FROM ".TABLE_ADS." ORDER BY host");
  WHILE($row = sql_fetch_row($queryres)) {
    echo('<TR>'
	.'<TD bgcolor="#eeeeff"><A HREF="'.$row[4].'">'.$row[0].'</A></TD>'
	.'<TD bgcolor="#eeffee">'.$row[1]
       	.'</TD><TD bgcolor="#ffeeee">'.$row[5]
       	.'</TD><TD bgcolor="#eeeeee" align="right">'.$row[2]
       	.'</TD><TD bgcolor="#eeeeee" align="right">'.$row[3]
       	.'</TD><TD bgcolor="#eeeeee" align="center"><a href="'.$_SERVER['PHP_SELF'].'?delete='.$row[6].'"><img src="img/delete.gif" border="0" alt="Delete"></a>&nbsp;'
	.'<a href="'.$_SERVER['PHP_SELF'].'?id='.$row[6].'"><img src="img/edit.gif" border="0" alt="Edit"></a>'
       	."</TD></TR>\n");
  } 
?>
</TABLE>
<br>
<center><a href="<?$_SERVER['PHP_SELF']?>?new=true">Add new host</a></center>
</td></tr>
</table>
<?
}
        include('footer.inc');
?>
</center>
</body>
</html>
