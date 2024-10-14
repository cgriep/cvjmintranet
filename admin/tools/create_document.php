#!/usr/bin/php -q
<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

echo "\nThis script can be used to add documents to a AWF database.\n";
echo "Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.\n\n";

$argc = $_SERVER['argc'];
$argv = $_SERVER['argv'];

if($argc < 6) {
	echo "Usage: create_document.php [parent_id] [doctype] [enabled] [published] [sort_order] --add [name] [value] [flavour] [datatype]\n\n";
	exit();
	}

error_reporting(1); // Disable Warnings

// Load database stuff
include('../../inc/functions.inc');
include('../../inc/database.inc');
include('../../inc/db_tables.inc');

init_doctypes();

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
	while ($row = sql_fetch_row($qresult)) {
        	define(strtoupper($row[0]),$row[1]);
                }
	sql_free_result($qresult);
	}

$parent_id = $argv[1];
$doctype   = $argv[2];
$enabled   = $argv[3];
$published = $argv[4];
$sort_order= $argv[5];

$id = create_node($parent_id, $doctypes[$doctype], $sort_order, $enabled, $published);
echo "Created new node #$id\n";
$counter = 6;
while($counter < $argc - 3) {
	if($argv[$counter] == '--add') {
		if($counter < 4) {
			echo "Error while adding nodedata. But please don't panic ;)\n";
			exit();
			}
		$name = $argv[$counter + 1];
		$value = $argv[$counter + 2];
		$flavour = $argv[$counter + 3];
		if($argv[$counter + 3] != '--add' && isset($argv[$counter + 3])) {
			$type = $argv[$counter + 4];
			$counter += 3;
			}
		else {
			$type = 'notrans';
			$counter += 2;
			}
		add_nodedata($id, $name, $value, $flavour, $type);
		echo "Added nodedata: $name\n";
		}
	++$counter;
	}
echo "Done :-)\n\n";
?>
