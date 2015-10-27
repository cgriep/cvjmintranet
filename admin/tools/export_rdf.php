#!/usr/bin/php -q
<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

echo "This script can be used to add documents to a AWF database.\n";
echo "Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.\n\n";

$argc = $_SERVER['argc'];
$argv = $_SERVER['argv'];

if($argc < 2) {
	echo "Usage: export_rdf.php [server name]\n\n";
	exit();
	}

// error_reporting(1); // Disable Warnings

$server_name = $argv[1];

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

$fp = fopen("../../news.rdf","w");

$_SERVER['PHP_SELF'] = 'index.html';
define('URL_REWRITE', '1');

$news_node = get_node(get_cmodule_id('news'), DEFAULT_FLAVOUR);

fwrite($fp, '<?xml version="1.0" encoding="ISO-8859-1" ?>
<rdf:RDF
xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
xmlns="http://my.netscape.com/rdf/simple/0.9/">

<channel>
  <title>'.SITE_TITLE.' '.$news_node['title'].'</title>
  <link>'.$server_name.'/'.get_url(get_cmodule_id ('news')).'</link>
  <description>'.$news_node['description'].'</description>
</channel>
');

$children = get_sorted_children (0, 10, 'timestamp', $doctypes['newsitem'], get_cmodule_id ('news'));

if(isset($children)) {
        reset($children);
        while (list ($docid, $value) = each ($children)) {
	fwrite($fp,
'
<item>
  <title>'.$value['title'].'</title>
  <link>'.$server_name.'/'.get_url($docid).'</link>
</item>
');
                }
	}

fwrite($fp, "\n</rdf:RDF>");

fclose($fp);

echo "Done :-)\n\n";
?>
