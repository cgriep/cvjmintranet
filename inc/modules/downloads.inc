<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$filename = INC_PATH.DATA_PATH.'downloads.txt';
$lines = file($filename);
while (list ($line_num, $line) = each ($lines)) {
  $buffer = split('\|', $line);
  echo('<a href="'.$buffer[1].'">'.$buffer[0].'</a><br />');
}
?>

