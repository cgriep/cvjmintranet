<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

// This is the main config file for all design issues

if (file_exists(BASE_PATH.INC_PATH.DESIGN_PATH.$design['name'].'.inc')) {
	$design_name = $design['name'];
	unset($design);
	include(BASE_PATH.INC_PATH.DESIGN_PATH.$design_name.'.inc');
	$design['name'] = $design_name;
	}
else {
	unset($design);
	include(BASE_PATH.INC_PATH.DESIGN_PATH.DEFAULT_DESIGN.'.inc');
	$design['name'] = DEFAULT_DESIGN;
	}
?>
