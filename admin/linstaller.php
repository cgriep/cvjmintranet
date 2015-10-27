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

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');
include($_BASEPATH.'/inc/functions.inc');

// Read all contstants from database
                $qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
                if(sql_num_rows($qresult) > 0) {
                        while ($row = sql_fetch_row($qresult)) {
                                define(strtoupper($row[0]),$row[1]);
                                }
                        sql_free_result($qresult);
                        }


define('LBI_SHARE_CLIENT', 'http://'.$_SERVER['SERVER_NAME'].dirname(dirname($_SERVER['SCRIPT_NAME'])).'linstaller_client.php');

?>

<html>
<head>
<meta name="title" content="Installer">
<meta name="description" content="Install/remove software packages.">
<meta name="sort_order" content="14">
<title>
Liquid Bytes AWF Installer
</title>
<?
	require('header.inc');

	require('linstaller.inc');

        require('footer.inc');
?>
</center>
</body>
</html>
