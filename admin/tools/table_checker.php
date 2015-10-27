#!/usr/bin/php -q
<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/
 
echo "Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.\n\n";

$argc = $_SERVER['argc'];
$argv = $_SERVER['argv'];
 
if($argc < 2) {
	echo("Usage: table_checker.php [filename]\n\n");
	exit();
        }

elseif(!file_exists($argv[1])) {
	echo("Error: File not found.\n\n");
	exit();
        }

$filename = $argv[1];
$erro	  = false;

function check_tag ($tag) {
	global $error;
	$tag_open  = substr_count ($GLOBALS['file'], '<'.$tag);
	$tag_close = substr_count ($GLOBALS['file'], '</'.$tag.'>');
	if(($tag_open - $tag_close) > 0) {
		echo $GLOBALS['filename'].": Too many open <$tag> tags.\n";
		$error = true;
		}
	elseif(($tag_open - $tag_close) < 0) {
		echo $GLOBALS['filename'].": Too many closed <$tag> tags.\n";
		$error = true;
		}
	}

$file = join('', file($filename));
check_tag('table');
check_tag('tr');
check_tag('td');
if(!$error) echo "$filename: OK :-)\n";
echo("\n");
?>
