#!/usr/bin/php -q
<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

echo "This script can be used to limit the number of AWF instant messages.\n";
echo "Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.\n\n";

$argc = $_SERVER['argc'];
$argv = $_SERVER['argv'];

if($argc < 2 || !is_numeric($argv[1])) {
	echo "Usage: update_im.php [number]\n\n";
	exit();
	}

error_reporting(1); // Disable Warnings

// Load database stuff
include('../../inc/functions.inc');
include('../../inc/database.inc');
include('../../inc/db_tables.inc');

// Read all contstants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
if(sql_num_rows($qresult) > 0) {
	while ($row = sql_fetch_row($qresult)) {
        	define(strtoupper($row[0]),$row[1]);
                }
	sql_free_result($qresult);
	}

$qres = sql_query("DELETE FROM ".TABLE_MESSAGES." WHERE status = 1 AND created > 0 AND created < ".(time() - 60*60*24*60));

echo sql_affected_rows($qres)." old messages deleted.\n";

$qres = sql_query("SELECT user_id, count(*) FROM ".TABLE_MESSAGES." WHERE status = 1 GROUP BY user_id HAVING count(*) > ".$argv[1]);

$total_count = 0;

if(sql_num_rows($qres) > 0) {
	while($row = sql_fetch_row($qres)) {
		$del_count = $row[1] - $argv[1];
		$total_count += $del_count;
		echo "DELETE ".$del_count." MESSAGES FROM USER ID ".$row[0].": ";
		$msg_res = sql_query("SELECT id FROM ".TABLE_MESSAGES." WHERE user_id = ".$row[0]." LIMIT ".$argv[1].", 99999");
		while($msg_row = sql_fetch_row($msg_res)) {
			echo '.';
			sql_query("DELETE FROM ".TABLE_MESSAGES." WHERE id = ".$msg_row[0]);
			}
		echo " OK\n";
		sql_free_result($msg_res);
		}

	sql_free_result($qres);
	}

echo "\n$total_count messages deleted.\n\nDone :-)\n\n";
?>
