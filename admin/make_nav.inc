<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$filename = 'navigation.cache';

$_cache_time = 0;

if(file_exists($filename) && time() - filemtime($filename) < $_cache_time && $_cache_time != 0) {
        $admin_modules = unserialize(trim(join('', file($filename))));
        }
else {
	$dir = @opendir(".");
	while(($file = readdir($dir)) !== false) {
	$path_parts = pathinfo($file);
	if(is_file($file) && $path_parts['extension'] == 'php') {
		$meta = get_meta_tags($file);
		if(isset($meta['title'])) {
			$admin_modules[$meta['sort_order']]['filename'] = $file;
			$admin_modules[$meta['sort_order']]['title'] = $meta['title'];
			$admin_modules[$meta['sort_order']]['description'] = $meta['description'];
			}
		}
	}
	closedir($dir);

	ksort($admin_modules);
		
	reset($admin_modules);
		
	$fp = fopen ($filename, "w");
	fwrite($fp,serialize($admin_modules));
	fclose($fp);
	}

$pre = '';

foreach ($admin_modules as $key => $value) {
	echo $pre.'<td class="nav_a">';
	if($value['filename'] == basename($_SERVER['PHP_SELF'])) $value['title'] = '<b>'.$value['title'].'</b>';
	echo('<a href="'.$value['filename'].'"><span style="font-size: 11px; color: #ffffff;" title="'.$value['description'].'">'.$value['title']."</span></a>\n");
	echo '</td>';
	$pre = '<td width="9"><img src="img/nav_sep.png" border="0" hspace="0" vspace="0" width="9" height="13" /></td>';
	}


?>
