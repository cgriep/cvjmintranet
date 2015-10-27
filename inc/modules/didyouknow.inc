<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$filename = INC_PATH.DATA_PATH.'did_you_know.txt';
$lines = file($filename);
srand((double) microtime() * 1000000);
echo $lines[rand(0,sizeof($lines) - 1)];

?>
