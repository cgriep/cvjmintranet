#!/usr/bin/php -q
<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 18.09.2003
*/

$argc = $_SERVER['argc'];
$argv = $_SERVER['argv'];

if($argc < 4) {
        echo "Usage: replace.php [from] [to] [filename]\n\n";
        exit();
        }

echo str_replace($argv[1], $argv[2], join('', file($argv[3])));
?>
