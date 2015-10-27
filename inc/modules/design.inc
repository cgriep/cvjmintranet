<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $design;
global $self;

$handle = opendir(BASE_PATH.INC_PATH.DESIGN_PATH); 
while (false!==($file = readdir($handle))) { 
    if ($file != "." && $file != ".." && '.inc' == substr($file, -4)) { 
	$designs[] = strtok ($file,".");
    } 
}
closedir($handle);

sort($designs);

while(list($key, $value) = each($designs)) {
	if($value == $design['name']) {
	        $class = 'selectedNav';
                }
        else {
        	$class = 'normalNav';
                }
	echo '<a href="'.$self."docinput[design]=$value".'" class="'.$class.'">'.$value.'</a><br />'; 
	}
?>
