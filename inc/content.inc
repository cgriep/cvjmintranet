<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

/* This is the main code for page compilation (header, footer, modules and content).
There are plans to do this usign some kind of 'template' mechanism, but that's not yet done. */

// Load an external document?
if(isset($document['external']) && $document['external'] != '' && $profile['editor'] != 1) { 
	include $document['external'];
	}
else {

$show_all = (!$document['show_doc_only'] || $profile['editor'] == 1);

if($show_all) {
	// Include header
	include(INC_PATH.HEADER_INC);

	// Start with main table
	echo $design['main_table_begin'];

	if($profile['editor'] == 1 || !$document['hide_left_col']) {
		echo $design['left_td_begin'];
	
		// Include modules for the left side
		include_modules('left');
		}
	}

// cmodule caching
if(defined('CACHE_FILENAME') && ( time() - filemtime(CACHE_FILENAME) < $cache_time && $cache_time != 0) )
{
	readfile(CACHE_FILENAME);
}
else {
		if(CACHING != 'on') 	ob_start();

		if($show_all) {
			if($profile['editor'] == 1 || !$document['hide_left_col']) {
				echo $design['left_td_end'];
				}

			echo $design['center_td_begin'];
			}
	
		if($profile['editor'] == 1) include(INC_PATH.'misc/editor_panel_begin.inc');
		
		if(isset($_GET['add_comment']) && $_GET['add_comment'] == 'true') {
	
			include(INC_PATH.CMODULES_PATH.$doctemplates[$doctypes['comment']].'.inc');
			} 
		else {
			include(INC_PATH.CMODULES_PATH.$doctemplates[$document['type_id']].'.inc'); 
		
			if($document['comments'] == '1') {
				include(INC_PATH.'misc/listcomments.inc');
				}

			if($document['show_files'] == '1') {
				include(INC_PATH.'cmodules/browser.inc');
				}
			}

		if($profile['editor'] == 1) include(INC_PATH.'misc/editor_panel_end.inc');

		if($show_all) {
			echo $design['center_td_end'];

			if($profile['editor'] == 1 || !$document['hide_right_col']) echo $design['right_td_begin'];
			}

		if(CACHING != 'on') {
			caching(ob_get_contents());
			ob_end_flush();
			}
	}

if($show_all) {
	if($profile['editor'] == 1 || !$document['hide_right_col']) {
		// Include modules for the right side
		include_modules('right'); 

		echo $design['right_td_end'];
		}

	// Close main table
	echo $design['main_table_end'];

	// Show footer
	include(INC_PATH.FOOTER_INC);
	}


}

?>

