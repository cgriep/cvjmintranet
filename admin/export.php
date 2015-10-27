<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.04.2004
*/

$_BASEPATH = strrev(strstr(substr(strstr(strrev($_SERVER['PATH_TRANSLATED']),'/'), 1),'/'));

// Load database stuff
include($_BASEPATH.'/inc/database.inc');
include($_BASEPATH.'/inc/db_tables.inc');
include($_BASEPATH.'/inc/functions.inc');
include($_BASEPATH.'/inc/sql_functions.inc');
include('lb_export_lib.inc');

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
        while ($row = sql_fetch_row($qresult)) {
                define(strtoupper($row[0]),$row[1]);
                }
        sql_free_result($qresult);
        }

$lang   = DEFAULT_LANG;

load_lang('admin');

if(isset($_REQUEST['format']) && isset($_REQUEST['type'])) {

$format = $_REQUEST['format'];
$type   = $_REQUEST['type'];

include('export/'.$type.'.inc');

$cols = lb_export_init();

$result = lb_export_get_header ($format, $cols);

while($row = lb_export_get_dataset()) {
	$result .= lb_export_get_row ($format, $row);
	}

$result .= lb_export_get_footer ($format, $cols);

lb_export_finish();

echo $result;
}

?>

