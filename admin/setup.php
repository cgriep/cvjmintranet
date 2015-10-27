<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

error_reporting(1); // Disable Warnings

$_BASEPATH = dirname(dirname($_SERVER['SCRIPT_FILENAME']));

set_magic_quotes_runtime(0); // Disable magic_quotes_runtime

define('AWF_VERSION', trim(join('', file($_BASEPATH.'/inc/VERSION'))));

while(list($key, $val) = each ($_REQUEST)) {
        $$key = $val;
        }

if(!$step) { $step = 1; }
if($button_back && ($step > 1)) $step--;
if($button_next) $step++;

if($step == 2 && isset($button_next) && strpos($licence_mail,'@') === false) { 
	$step = 1; 
	$mail_warning = true;
	}

include($_BASEPATH.'/inc/functions.inc');

?>
<html>
<head>
<meta name="title" content="Setup">
<meta name="description" content="">
<meta name="sort_order" content="15">
<title>
Liquid Bytes AWF Setup - Step <? echo $step; ?>
</title>
<?php
        include('header.inc');

if(!function_exists('version_compare')) { ?>
<p style="text-align: center; font-weight: bold; font-size: 14px; color: #000000">AWF <?=AWF_VERSION?> requires at least PHP version 4.1.0</p>
<? 
} else { ?>
<form action="setup.php" method="post">
<input type="hidden" name="step" value="<? echo $step; ?>">
<center>
<br>
<table width="700" cellpadding="5" cellspacing="1" border="0" bgcolor="#cccccc"><tr>
<td height="350" valign="top" bgcolor="#eeeeee">
<table width="100%" cellpadding="3" cellspacing="0 border="0" bgcolor="#cccccc"><tr>
<td><span style="font-weight: bold; font-size: 14px; color: #000000">AWF <?=AWF_VERSION?> Setup - Step <? echo $step; 
?></span></td>
</tr>
</table>
<br /><br />
<blockquote>
<?php
if($step == 1) { 
	if(file_exists($_BASEPATH.'/inc/licence.key')) include($_BASEPATH.'/inc/licence.key');
	?>
	Welcome to Liquid Bytes<sup>&reg;</sup> Adaptive Website Framework (AWF) Setup!
	<br><br><?php
	$key=md5($licence_mail.'AWF1');
	if($mail_warning) { 
		echo '<b><font color="#aa0000">Your email address is incorrect.</b></font><br /><br />'; }
	?>
	<input type="hidden" name="key" value="<? echo LICENCE_KEY; ?>">
	Please enter your email address<br />
	<input type="text" name="licence_mail" size="40" width="40" value="<? echo LICENCE_EMAIL; ?>" /><br /><br />
	We wanna know how many sites are running AWF. If this is a first time installation, please consider
	using our registration function. Thanks!
	<br><br>
	<input type="checkbox" name="register" value="yes"> Register this copy of AWF (no more information must be provided)<br>
	<input type="checkbox" name="awflist" value="yes"> Subscribe to the AWF mailing list<br>
	<input type="checkbox" name="updates" value="yes"> I want to receive update notifications by email<br>
	<?php
	}
elseif($step == 2) {
	if(isset($licence_mail) && $licence_mail != '') {
	if($register == "yes") {
		$info_content = "\n\nPHP-Version: ".phpversion()."\nAWF-Version: ".AWF_VERSION;
		if(function_exists('php_uname')) { $info_content .= "\nOS: ".php_uname(); }
		mail("awfreg@liquidbytes.net","AWF Registration",
		"This user just registered his copy of AWF:\n\n$licence_mail".$info_content,
		"From: $licence_mail\nReply-To: $licence_mail");
		}
	if($updates == "yes") {
		mail("awfupdates@liquidbytes.net","AWF Update Notification Request",
		"This user wants to have AWF update notifications:\n\n$licence_mail",
		"From: $licence_mail\nReply-To: $licence_mail");
		}
	if($awflist == "yes") {
		mail("awf-list-request@ssc.com","subscribe",
		"", "From: $licence_mail\nReply-To: $licence_mail");
		}
		}
	if(file_exists($_BASEPATH.'/inc/database.inc')) include($_BASEPATH.'/inc/database.inc');
	if($button_next) {
		$key=md5($licence_mail.'AWF1');
		$filename = $_BASEPATH.'/inc/licence.key';
		$fd = fopen ($filename, "w");
		$contents = fwrite ($fd, '<? define("LICENCE_KEY","'.$key.'"); define("LICENCE_EMAIL","'.$licence_mail.'"); ?>');
		fclose ($fd);
		}
	?>
	Please enter the 
	<select size="1" name="dbms">
		<option value="mysql"<? if(DBMS != 'postgresql') { echo ' SELECTED'; } ?>>MySQL</option>
		<?php
		if(function_exists("pg_connect")) { ?>
		<option value=postgresql<? if(DBMS == 'postgresql') { echo ' SELECTED'; } ?>>PostgreSQL (experimental)</option>
		<?php } ?>
	</select> 
	database access information:<br><br>
	Host<br>
	<input type="text" name="host" size="40" width="40" value="<? echo $db_host; ?>"><br><br>
	Database<br>
	<input type="text" name="database" size="40" width="40" value="<? echo $db_name; ?>"><br><br>
	Username<br>
	<input type="text" name="username" size="40" width="40" value="<? echo $db_username; ?>"><br><br>
	Password<br>
	<input type="text" name="password" size="40" width="40" value="<? echo $db_password; ?>"><br><br>
	Table Prefix (optional)<br>
	<input type="text" name="prefix" size="40" width="40" value="<? echo $db_table_prefix; ?>"><br><br>
	<?
	}
elseif($step == 3) {
	if($button_next) {
		$filename = $_BASEPATH.'/inc/database.inc';
		if(is_writable($filename)) {;
		$fd = fopen ($filename, "w");
		if($dbms == 'mysql') {
		$contents = fwrite ($fd, 
'<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: '.date("D M j G:i:s T Y").'
*/

define("DBMS","mysql");

function sql_query ($query) {
        return mysql_query($query);
        }
 
function sql_affected_rows ($qresult = -1) {
        return mysql_affected_rows();
        }

function sql_num_rows ($qresult) {
	if($qresult > 0) {
                return mysql_num_rows($qresult);
                }
        else {
                return 0;
                }
        }
 
function sql_fetch_row ($qresult) {
        return mysql_fetch_row($qresult);
        }

function sql_fetch_array ($qres) {
        return mysql_fetch_assoc($qres);
        }
 
function sql_free_result ($qresult) {
        return mysql_free_result($qresult);
        }

function sql_insert_id ($qresult = -1) {
	return mysql_insert_id();
	}

function sql_limit ($count, $offset = 0) {
	return("LIMIT $offset, $count");
	}

// MySQL Database settings
$db_host        = "'.$host.'";
$db_name        = "'.$database.'";
$db_username    = "'.$username.'";
$db_password    = "'.$password.'";
$db_table_prefix= "'.$prefix.'";
 
if(function_exists("mysql_connect")) {
	mysql_connect($db_host,$db_username,$db_password);
	define(SQL_CONNECTION, mysql_select_db($db_name));
	}

?>');
} else {
		$contents = fwrite ($fd, 
'<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: '.date("D M j G:i:s T Y").'
*/

define("DBMS","postgresql");

function sql_query ($query) {
	global $sql_query_result;
        $sql_query_result = pg_exec (SQL_CONNECTION, $query);
	return $sql_query_result;
        }
 
function sql_affected_rows($qresult = -1) {
	if($qresult == -1) { $qresult = $GLOBALS[sql_query_result]; }
	return pg_cmdtuples ($qresult);
        }

function sql_num_rows($qresult) {
        return pg_numrows($qresult);
        }
 
function sql_fetch_row($qresult) {
	global $i;
	if(isset($i[$qresult])) { $i[$qresult]++; } 
	else { $i[$qresult] = 0; }
	return pg_fetch_row($qresult, $i[$qresult]);
        }
 
function sql_free_result($qresult) {
        return pg_freeresult($qresult);
        }

function sql_insert_id($qresult = -1) {
	if($qresult == -1) { $qresult = $GLOBALS[sql_query_result]; }
	global $db_table_prefix;
	$qresult = sql_query("SELECT id FROM ".TABLES_NODES." WHERE oid=".pg_getlastoid($qresult));
	if(sql_num_rows($qresult) == 1) {
		$row = sql_fetch_row($qresult);
		return($row[0]); 
		}
	return false;
	}

function sql_limit($count, $offset = 0) {
	return("LIMIT $count OFFSET $offset");
	}

// PSQL Database settings
$db_host        = "'.$host.'";
$db_name        = "'.$database.'";
$db_username    = "'.$username.'";
$db_password    = "'.$password.'";
$db_table_prefix= "'.$prefix.'";
 
if(function_exists("pg_connect")) {
	define(SQL_CONNECTION, pg_connect("host=$db_host port=5432 dbname=$db_name user=$db_username password=$db_password")); 
	}

?>');
}
     		fclose ($fd);
		} else { echo '<b><font color="#aa0000">inc/database.inc is not writable.</font></b><br><br>'; }
		}
	include($_BASEPATH.'/inc/database.inc');
	include($_BASEPATH.'/inc/db_tables.inc');
	if(!defined("SQL_CONNECTION") || SQL_CONNECTION == '') {
		echo '<b><font color="#aa0000">Can\'t connect to SQL server or database.</font></b><br><br>';
		}
	else { 
		// Read all contstants from database
		$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
		if(sql_num_rows($qresult) > 0) {
			while ($row = sql_fetch_row($qresult)) {
        			define(strtoupper($row[0]),$row[1]);
				}
			sql_free_result($qresult);
			} 
		}
	if(!defined('WEBMASTER_MAIL')) define('WEBMASTER_MAIL', $_SERVER['SERVER_ADMIN']);
	if(!defined('SITE_TITLE')) define('SITE_TITLE', $_SERVER['SERVER_NAME']);
	?>
	Title of Web site<br>
	<input type="text" name="sitetitle" size="40" width="40" value="<? echo SITE_TITLE; ?>"><br><br>
	Webmaster's email address<br>
	<input type="text" name="webmastermail" size="40" width="40" value="<? echo WEBMASTER_MAIL; ?>"><br><br>
	Path on server<br>
	<input type="text" name="serverpath" size="40" width="40" value="<? echo
		strrev(strstr(substr(strstr(strrev($_SERVER['PATH_TRANSLATED']),'/'), 1),'/')); ?>"><br><br>
	Default design<br>
	<select name="default_design" size="1">
	<?
	$handle=opendir($_BASEPATH.'/inc/design');
	while (false!==($file = readdir($handle))) {
    		if ($file != "." && $file != ".." && '.inc' == substr($file, -4)) {
        		$tok = strtok ($file,".");
			if($tok == DEFAULT_DESIGN) { $selected = ' SELECTED'; } else { $selected = ''; }
        		echo "<OPTION value=\"$tok\"$selected>".$tok.'</OPTION>';
    			}
		}
	closedir($handle);
	echo '</select><br /><br />';

// 	Phorum Support disabled (maybe just for some time...)
//	<input type="checkbox" name="install_phorum" value="1" /> Install Phorum 3.3.1a<br />
//	<input type="checkbox" name="install_content" value="1" /> Install demo content<br />

	?>
	<input type="checkbox" name="update_db" value="1" /> Update existing database to <?=AWF_VERSION?> 
	<input type="hidden" name="um_store_method" value="sql" />
	<?
	}
elseif($step == 4) { 
	include($_BASEPATH.'/inc/database.inc');
	include($_BASEPATH.'/inc/db_tables.inc');
	if(DBMS == 'postgresql') { 
		$filename = 'setup.psql.sql'; 
		$phorum_filename = '';
		$content_filename = '';
		$update_filename = '';
		}
	else { 
		$filename = 'setup.mysql.sql'; 
		$phorum_filename  = 'phorum.mysql.sql';
		$content_filename = 'content.mysql.sql';
		$update_filename  = 'update_database.mysql.sql';
		}

	if($update_db == '1') {
		unset($fcontents);
		$fcontents = file ($update_filename);
		while(list($id, $line) = each($fcontents)) {
	       		if($line && $line[0] != '#') $sql_dump .= $line;
			$sql_dump = str_replace ('1_', $db_table_prefix, $sql_dump);
			if(strpos($line,';')) {
				$sql_dump[strlen($sql_dump)-2] = ' ';
				sql_query($sql_dump);
				// echo nl2br($sql_dump).'<br><br>'; // debug option
				$sql_dump = '';
				}
			}
		}


	$qresult = sql_query("select * from ".TABLE_SETUP); 

	if($qresult && sql_num_rows($qresult) > 0) { 
		echo 'Database already seems to be ok :-)<br><br>';
		}
	else {
		if(file_exists('navigation.cache')) unlink('navigation.cache');
		unset($fcontents);
		$fcontents = file ($filename);
		while(list($id, $line) = each($fcontents)) {
	       		if($line && $line[0] != '#') $sql_dump .= $line;
			if(strpos($line,';')) {
				$sql_dump = str_replace ('1_', $db_table_prefix, $sql_dump);
				$sql_dump[strlen($sql_dump)-2] = ' ';
				sql_query($sql_dump);
				// echo nl2br($sql_dump).'<br><br>'; // debug option
				$sql_dump = '';
				}
			}
		}
	
	if($install_content == '1') {
		unset($fcontents);
		$fcontents = file ($content_filename);
		while(list($id, $line) = each($fcontents)) {
	       		if($line && $line[0] != '#') $sql_dump .= $line;
			$sql_dump = str_replace ('1_', $db_table_prefix, $sql_dump);
			if(strpos($line,';')) {
				$sql_dump[strlen($sql_dump)-2] = ' ';
				sql_query($sql_dump);
				// echo nl2br($sql_dump).'<br><br>'; // debug option
				$sql_dump = '';
				}
			}
		}

	if(!defined("SQL_CONNECTION") || SQL_CONNECTION == '') {
		echo '<b><font color="#aa0000">Can\'t connect to SQL server or database.</font></b><br><br>';
		}
	else { 
		// Read all contstants from database
		$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
		if(sql_num_rows($qresult) > 0) {
			while ($row = sql_fetch_row($qresult)) {
        			define(strtoupper($row[0]),$row[1]);
				}
			sql_free_result($qresult);
			} 
		}
	
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$serverpath."' WHERE name='base_path'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$webmastermail."' WHERE name='webmaster_mail'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$sitetitle."' WHERE name='site_title'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$default_design."' WHERE name='default_design'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$um_store_method."' WHERE name='um_store_method'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".AWF_VERSION."' WHERE name='version'");

	if((int) VERSION < 1.04) {
		sql_query("ALTER TABLE ".TABLE_NODES." ADD COLUMN created int");
		sql_query("ALTER TABLE ".TABLE_NODES." ADD COLUMN last_change int");
		}

	if((int) VERSION < 1.05) {
		sql_query("ALTER TABLE ".TABLE_USERS ." ADD COLUMN expires int(10) unsigned NOT NULL default '0'");

		// Place for more stuff

		}

	?>
	<b>Thank you for using AWF!</b><br><br>
	AWF was created by <a href="mailto:michael@liquidbytes.net">michael@liquidbytes.net</a><br /><br />
	For more information, please visit <a href="http://www.liquidbytes.net/" 
	target="_blank">http://www.liquidbytes.net/awf/</a><br /><br />
	'Liquid Bytes' is a registered trademark of Liquid Bytes&reg; Technologies, Germany<br /><br />
	<b>Please consider making a donation to support the future development of this Free Software product:</b><br /><br />
	<a target="_blank" href="https://www.paypal.com/xclick/business=michael%40liquidbytes.net&item_name=Adaptive+Website+Framework+%28AWF%29&no_note=1&tax=0&currency_code=EUR"><img src="img/donate.png" border="0" /></a> 
	<?

//	Not included anymore
//	This product includes software developed by the	<a href="http://phorum.org/">Phorum Development Team</a>.

	}
else {
	echo('You try to cheat, don\'t you?<br><br>');
	echo('If you have serious problems using AWF, please write a mail to '); 
	echo('<a href="maito:awf@liquidbytes.net">awf@liquidbytes.net</a>.');
	}
?>
</font>
</blockquote>
</td></tr><tr>
<td align="right">
<? if ($step > 1) { ?>
<input type="submit" name="button_back" value="<< Back">
<? }

if ($step < 4) { ?>
<input type="submit" name="button_next" value="<? if($step == 3) { echo 'Finish >>'; } else { echo 'Next >>'; } ?>">
<? } ?>
</td></tr>
</table>
<?
}
        include('footer.inc');
?>
</center>
</form>
</body>
</html>
