<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

// Load database stuff
if(file_exists('inc/database.inc')) {
	include('inc/database.inc');
	include('inc/db_tables.inc');
	include('inc/functions.inc');

	// Read all contstants from database
	$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
	if(sql_num_rows($qresult) > 0) {
       		while ($row = sql_fetch_row($qresult)) {
                	define(strtoupper($row[0]),$row[1]);
                	}
        	sql_free_result($qresult);
        	}
	}

include('admin/linstaller_lib.inc');

lbi_load_config('admin/'.LBI_CONFIG_FILE);

$basename = strtr(strtolower($_REQUEST['name']), ' -/\()[]=?.;,!����', '______________aous');
$baseversion = strtr(strtolower($_REQUEST['version']), ' -/\()[]=?;,!����', '................');

echo base64_encode(file_get_contents('admin/'.LBI_PACKAGE_PATH.$basename.'-'.$baseversion.'.lbi'));
?>
