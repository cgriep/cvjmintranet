<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 24.08.2003
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
if($save=='config') {
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$datetime_format."' WHERE name='datetime_format'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$date_format."' WHERE name='date_format'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$time_format."' WHERE name='time_format'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$dec_point."' WHERE name='dec_point'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$decimals."' WHERE name='decimals'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$currency_sym."' WHERE name='currency_sym'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$thousands_sep."' WHERE name='thousands_sep'");
	sql_query("UPDATE ".TABLE_SETUP." SET value='".$charset."' WHERE name='charset'");
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
<meta name="title" content="l10n">
<meta name="description" content="Localisation settings.">
<meta name="sort_order" content="10">
<title>
Liquid Bytes AWF l10n
</title>
<?php
        include('header.inc');
?>
<center>
<table width=90% cellpadding=0 cellspacing=0 border=0>
<tr><td align="left">
<center><h2><?php echo SITE_TITLE; ?> l10n</h2></center>
<br>
<form action="l10n.php" method="post">
Datetime format<br>
<input type="text" name="datetime_format" value="<?php echo DATETIME_FORMAT; ?>" size=20>
<br><br>
Date format<br>
<input type="text" name="date_format" value="<?php echo DATE_FORMAT; ?>" size=20>
<br><br>
Time format<br>
<input type="text" name="time_format" value="<?php echo TIME_FORMAT; ?>" size=20>
<br><br>
Decimal Symbol<br>
<input type="text" name="dec_point" value="<?php echo DEC_POINT; ?>" size=20>
<br><br>
Thousands Symbol<br>
<input type="text" name="thousands_sep" value="<?php echo THOUSANDS_SEP ?>" size=20>
<br><br>
Currency Decimals<br>
<input type="text" name="decimals" value="<?php echo DECIMALS; ?>" size=20>
<br><br>
Currency Symbol<br>
<input type="text" name="currency_sym" value="<?php echo CURRENCY_SYM; ?>" size=20>
<br><br>
Charset<br>
<input type="text" name="charset" value="<?php echo CHARSET; ?>" size=20>
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
