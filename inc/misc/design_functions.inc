<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

function add_buttons ($modid, $removeable) {
        if (SESSION_STATUS != 'ok' || !$modid) {
                return false;
                }
 
        global $design;
        global $profile;
	if($profile['module_size_'.$modid] == '2') {
                $design_size_img = $design['maximize_img'];
        } else {
                $design_size_img = $design['minimize_img'];
                }
        global $self;
        if($profile['module_size_'.$modid] != 2) { $minmax = 'min'; } else { $minmax = 'max'; }
        $buttons = get_image_tag($design_size_img, '', $self.'docinput['.$minmax.'_module]='.$modid);
        if($removeable == 1) {
		$buttons .= get_image_tag($design['close_img'], '', $self.'docinput[rem_module]='.$modid);
        	}
        return($buttons);
        }

function include_module($module, $size, $position) {
	if($module['position'] != $position || $module['visible'] != 1) return false;
	global $design;
	global $profile;
	$PHP_SELF = $_SERVER['PHP_SELF'];
	echo str_replace('%%buttons%%',add_buttons($module['id'], $module['removeable']),
		str_replace('%%title%%', $module['caption'], $design['module_begin'][$position]));
	if($size < 2) {
		echo $design['module_endtitle'][$position];
		include(INC_PATH.MODULE_PATH.$module['name']);
		echo $design['module_end'][$position];
		}
	else {
		echo $design['module_minimized'][$position];
		}
	}

?>
