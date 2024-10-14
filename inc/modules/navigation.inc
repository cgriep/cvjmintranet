<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $id;
global $flavour;
$cache_time = CACHE_TIME;

if(!function_exists('display_items')) {
	function display_items (&$parents, $level = 0, $pos = 0) {
		global $document;
		global $id;
		global $design;
		//$key = key($parents);
		//$value = current($parents);
		for($i = 0; $i < $level; $i++) {
			$pre .= '&nbsp;&nbsp;';
			}
		if($level > 0) $pre .= '<img src="'.$design['nav_bullet'].'" />';
		$related = get_children(0, 100, -1, true, 'DESC', true, $pos);
		foreach ($related as $docid => $doc) {
			if(isset($parents[$docid])) {
				echo $pre.'<a href="'.get_url($docid).'" class="selectedNav">'.$doc['title']."</a><br />\n";
				display_items($parents, $level + 1, $docid);
				}
			else {
				echo $pre.'<a href="'.get_url($docid).'" class="normalNav">'.$doc['title']."</a><br />\n";
				}
			}
		}
	}

$filename = BASE_PATH.'cache/modules/nav_'.md5($id.$flavour.$design['name']);

if(time() - filemtime($filename) < $cache_time && $cache_time != 0 && SESSION_STATUS != 'ok') {
        readfile($filename);
        }
else {
	ob_start();

	init_parents();

	$parents = $GLOBALS['parents'];
	$parents[0] = true;
	$parents = array_reverse($parents, true);
	$parents[$id] = true;

	display_items($parents);

	$content = ob_get_contents();
        ob_end_clean();
        if($cache_time != 0 && SESSION_STATUS != 'ok') {
                $fp = fopen ($filename, "w");
                fwrite($fp,$content);
                fclose($fp);
                }
        echo $content;
	}
?>