<?php
/*
        Copyright (C) 2000-2002 Liquid Bytes (R), Germany. All rights reserved.
	http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 09.01.2002
*/

global $id;
global $flavour;

$filename = 'cache/modules/nav_'.$id.'_'.$flavour.'_'.$design['name'];
 
if(time() - filemtime($filename) < CACHE_TIME && CACHE_TIME != 0 && SESSION_STATUS != 'ok') {
        readfile($filename);
        }
else {
	ob_start();

	init_parents();

	init_published_roots();
 
	init_published_children();

	init_published_related();

	global $published_roots;
	global $published_children;
	global $root_parent_id;
	global $published_related;
	global $document;

	if(isset($published_roots)) {
	reset($published_roots);
	while (list ($docid, $value) = each ($published_roots)) {
		if($root_parent_id == $docid || $docid == $id) echo '<span style="font-weight:bold">';
		echo '<a href="'.get_url($docid).'">'.transform($value['title'], 'clean').'</a><br />';
		if($root_parent_id == $docid || $docid == $id) {
			echo '</span>';
			if(isset($published_children) && $document[parent_id] == 0) {
				reset($published_children);
				while (list ($docid2, $value2) = each ($published_children)) {
				if ($value2['published'] != '0')
				echo '<img src="'.$design['nav_bullet'].'" border="0">'.
					'<a href="'.get_url($docid2).'">'.transform($value2['title'], 'clean').'</a><br />';
			
				}
			}
			if(isset($published_related)) {
				   reset($published_related);
				   while (list ($docid2, $value2) = each ($published_related)) {
					if($docid2 == $id) { $bold = '<span style="font-weight:bold">'; $unbold = '</span>'; }
					else { $bold = ''; $unbold = ''; }
			      		if ($value2['published'] != '0')
					echo '<img src="'.$design['nav_bullet'].'" border="0">'.
      					'<a href="'.get_url($docid2).'">'.$bold.transform($value2['title'], 'clean').$unbold.'</a><br />'."\n";
					if($bold != '') { 
						$children = get_children(0, 100, -1, 1, 'DESC', true);
						if($children) {
						while (list ($docid2, $value2) = each ($children)) {
						if ($value2['published'] != '0')
						echo '&nbsp;&nbsp;<img src="'.$design['nav_bullet'].'" border="0">'.
						'<a href="'.get_url($docid2).'">'.transform($value2['title'], 'clean').'</a><br />';
						}
						}
						}
			               	}
				
				   }
			
			}
		}
		}
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
