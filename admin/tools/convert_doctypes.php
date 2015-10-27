#!/usr/bin/php -q
<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

echo "This script can be used to convert the types table to the doctypes table, used by AWF >= 1.20.\n";
echo "Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.\n\n";

$argc = $_SERVER['argc'];
$argv = $_SERVER['argv'];

error_reporting(1); // Disable Warnings

// Load database stuff
include('../../inc/functions.inc');
include('../../inc/database.inc');
include('../../inc/sql_functions.inc');
include('../../inc/db_tables.inc');

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
	while ($row = sql_fetch_row($qresult)) {
        	define(strtoupper($row[0]),$row[1]);
                }
	sql_free_result($qresult);
	}

$total_count = 0;

$rows = sql_query_array("SELECT type_id, name, template, parent_id, visible, description FROM ".
		TABLE_TYPEDATA.",".TABLE_TYPES." WHERE ".TABLE_TYPES.".id=".TABLE_TYPEDATA.".
		type_id AND platform='phpweb' ORDER BY type_id", 'type_id');

while(list($key, $row) = each($rows)) {

	$newid = sql_insert(TABLE_DOCTYPES, array(
		'id' 		=> $key,
		'format'	=> 'html',
		'name'		=> $row['name'],
		'label'		=> $row['description'],
		'description'	=> '',
		'visible'	=> $row['visible'],
		'filename'	=> $row['template'].'.inc',
		'license'	=> 'GPL',
		'version'	=> 'Unknown',
		'last_change'	=> 'Unknown',
		'type'		=> 'single',
		'author'	=> 'Unknown')
		);

	sql_insert(TABLE_DOCTYPERELATIONS, array(
		'child_id'	=> $key,
		'parent_id'	=> $row['parent_id'])
		, FALSE);

	$total_count++;
	}

echo "$total_count doctypes converted.\n\nDone :-)\n\n";
?>
