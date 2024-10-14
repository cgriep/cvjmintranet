<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

error_reporting(1); // Disable Warnings

foreach ($_REQUEST as $key => $val) {
        $$key = $val;
        }

$_BASEPATH = dirname(dirname($_SERVER['SCRIPT_FILENAME']));

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');

// Save settings in DB
if($save=='config') {
	if($show_warnings != '1') { $show_warnings = 0; }
	if($count_views != '1') { $count_views = 0; }
	if($disable_url_to_link != '1') { $disable_url_to_link = 0; }
	if($disable_wildcards != '1') { $disable_wildcards = 0; }
	if($disable_email_check != '1') { $disable_email_check = 0; }
	if($force_ssl != '1') { $force_ssl = 0; }
	if($unique_nicknames != '1') { $unique_nicknames = 0; }
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$default_document."' WHERE name='default_document'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$default_flavour."' WHERE name='default_flavour'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$default_design."' WHERE name='default_design'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$default_lang."' WHERE name='default_lang'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$show_warnings."' WHERE name='show_warnings'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$count_views."' WHERE name='count_views'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$denied_action."' WHERE name='denied_action'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$denied_document."' WHERE name='denied_document'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$cache_time."' WHERE name='cache_time'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$disable_email_check."' WHERE name='disable_email_check'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$disable_url_to_link."' WHERE name='disable_url_to_link'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$disable_wildcards."' WHERE name='disable_wildcards'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$force_ssl."' WHERE name='force_ssl'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$unique_nicknames."' WHERE name='unique_nicknames'");

        $filename = $_BASEPATH.'/inc/caching.inc';
        $fd = fopen ($filename, "w");
	if($cache_time == 0) {
        $contents = fwrite ($fd,
'<?php
/*
        Copyright (C) 2000-2002 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: '.date("D M j G:i:s T Y").'
*/

// caching is disabled

?>');
		}
	else {
        $contents = fwrite ($fd,
'<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: '.date("D M j G:i:s T Y").'
*/
 
if(!isset($session_userid) && !isset($docinput["user_id"]) && !isset($docinput["passwd2"])) {
        define("CACHING","on");
        $cache_time = '.$cache_time.';
	define("FINGERPRINT", md5("nouser".$_SERVER["PHP_SELF"].serialize($_REQUEST).$lang.$design["name"].$flavour));
        if(isset($_REQUEST["id"])) {
		define("CACHE_FILENAME", get_dir_name("cache", $id)."/".FINGERPRINT);
		}
	else {
	        define("CACHE_FILENAME", "cache/default/".FINGERPRINT);
		}
        if(file_exists(CACHE_FILENAME) && (time() - filemtime(CACHE_FILENAME) < $cache_time) && $cache_time != 0)
                {
                readfile(CACHE_FILENAME);
		write_log();
                exit();
                }
        else {
                ob_start();
                }
        }
?>');
	}
	fclose ($fd);

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
<meta name="title" content="Config">
<meta name="description" content="Basic configuration settings.">
<meta name="sort_order" content="11">
<title>
Liquid Bytes AWF Config
</title>
<?php
        include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><?php echo SITE_TITLE; ?> Config</h2></center>
<br>
<form action="config.php" method="post">
Default document id<br>
<input type="text" name="default_document" value="<?php echo DEFAULT_DOCUMENT; ?>" size=20>
<br><br>
Default flavour id<br>
<input type="text" name="default_flavour" value="<?php echo DEFAULT_FLAVOUR; ?>" size=20>
<br><br>
Default language<br>
<select name="default_lang" size="1">
        <?php
        $handle=opendir($_BASEPATH.'/inc/lang');
        while (false!==($file = readdir($handle))) {
                if ($file != "." && $file != "..") {
                        if($file == DEFAULT_LANG) { $selected = ' SELECTED'; } else { $selected = ''; }
                        echo "<OPTION value=\"$file\"$selected>".$file.'</OPTION>';
                        }
                }
        closedir($handle);
        ?>
</select>
<br><br>
Default design<br>
<select name="default_design" size="1">
        <?php
        $handle=opendir($_BASEPATH.'/inc/design');
        while (false!==($file = readdir($handle))) {
                if ($file != "." && $file != ".." && '.inc' == substr($file, -4)) {
                        $tok = strtok ($file,".");
                        if($tok == DEFAULT_DESIGN) { $selected = ' SELECTED'; } else { $selected = ''; }
                        echo "<OPTION value=\"$tok\"$selected>".$tok.'</OPTION>';
                        }
                }
        closedir($handle); 
	?>
</select><br /><br />
Cache time (in seconds)<br>
<input type="text" name="cache_time" value="<?php echo CACHE_TIME; ?>" size=20>
<br><br>
<input type="checkbox" value="1" name="show_warnings"<?php if(SHOW_WARNINGS == 1) echo ' CHECKED'; ?>>
 Show all PHP warnings<br>
Note: For public web sites, this is NOT a recommended option. For development, it is.
<br><br>
<input type="checkbox" value="1" name="disable_url_to_link"<?php if(DISABLE_URL_TO_LINK == 1) echo ' CHECKED'; ?>>
 Disable URL recognition<br>
Note: Just do this, if you have problems. Normal URLs and email addresses can be recognised.
<br><br>
<input type="checkbox" value="1" name="disable_wildcards"<?php if(DISABLE_WILDCARDS == 1) echo ' CHECKED'; ?>>
 Disable wildcard replacement<br>
Note: Just do this, if you have problems or you need more performance.
<br><br>
<input type="checkbox" value="1" name="disable_email_check"<?php if(DISABLE_EMAIL_CHECK == 1) echo ' CHECKED'; ?>>
 Disable email address validation<br>
Note: This makes sense if you're using AWF for your intranet only.
<br><br>
<input type="checkbox" value="1" name="count_views"<?php if(COUNT_VIEWS == 1) echo ' CHECKED'; ?>>
 Count page views and set timestamps<br>
Note: This will cost some performance, because MySQL locks the table while writing.
<br><br>
<input type="checkbox" value="1" name="force_ssl"<?php if(FORCE_SSL == 1) echo ' CHECKED'; ?>>
 Force SSL (no login w/o SSL)
<br><br>
<input type="checkbox" value="1" name="unique_nicknames"<?php if(UNIQUE_NICKNAMES == 1) echo ' CHECKED'; ?>>
 Unique nicknames
<br><br>
What to do, if document access is denied?<br>
<input type="radio" value="1" name="denied_action"<?php if(DENIED_ACTION==1) echo ' CHECKED'; ?>> Load document with ID <input type="text" size="5" name="denied_document" value="<?php if(defined('DENIED_DOCUMENT')) { echo DENIED_DOCUMENT; } ?>"><br>
<input type="radio" value="2" name="denied_action"<?php if(DENIED_ACTION==2) echo ' CHECKED'; ?>> Ignore<br>
<input type="radio" value="3" name="denied_action"<?php if(DENIED_ACTION==3) echo ' CHECKED'; ?>> Ignore, but set flag (DOCUMENT_ACCESS = 0)<br>
<input type="radio" value="4" name="denied_action"<?php if(DENIED_ACTION==4) echo ' CHECKED'; ?>> Call exit()<br>
<input type="radio" value="5" name="denied_action"<?php if(DENIED_ACTION==5) echo ' CHECKED'; ?>> Print out 'Access denied.' and then call exit()<br>
<br><br>
<input type="hidden" name="save" value="config">
<input type="submit" value="Save">
</form>
</td></tr>
</table>
<?php
	include('footer.inc');
?>
</center>
</body>
</html>
