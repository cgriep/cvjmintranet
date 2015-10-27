#!/usr/bin/php -q
<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

echo "\nThis script can be used to analyze the AWF logs.\n";
echo "Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.\n\n";

// error_reporting(1); // Disable Warnings

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

$dir = BASE_PATH.'logs/';

if ($handle = opendir($dir)) {

while (false !== ($filename = readdir($handle))) {

if(is_file($dir.$filename)) {

// $filename = BASE_PATH.'logs/'.date("Y_m").'.log';

echo "##################### Analyze of $filename ######################\n\n";

$total_time = 0;
$pages = 0;
$ips = array();
$page_ids = array();
$user_ids = array();
$langs = array();
$days = array();

$fp = fopen($dir.$filename, 'r');

while (!feof($fp)) {
	$buffer = fgets($fp, 4096);
	if($buffer != '') {
		$log_line = explode("\t", $buffer);
		$total_time += $log_line[3];
		$page_ids[$log_line[2]]++;
		$user_ids[$log_line[4]]++;
		$ips[$log_line[0]]++;
		$langs[$log_line[5]]++;
		$days[date('d.m.Y', $log_line[1])]++;
		$pages++;
		}
	}

fclose ($fp);

echo "* Page views by language\n";
while(list($key, $value) = each($langs)) {
	echo "$key: $value\n";
	}

echo "* Page views\n";
while(list($key, $value) = each($days)) {
	echo "$key: $value\n";
	}

echo "Total page views: ".$pages."\n";
echo "Different pages: ".count($page_ids)."\n";
echo "Different IPs: ".count($ips)."\n";
echo "Different registered users: ".count($user_ids)."\n";
echo "Total runtime: ".($total_time / 60)." min\n";
echo "Average page creation time: ".($total_time / $pages)." s\n\n";
}
}
}
else {
	echo "File not found.\n";
	}
echo "\n";

?>
