<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if(CACHING == 'on') {
	if($document['cacheable'] == 1) {
		caching(ob_get_contents());
        	}
	ob_end_flush();
	}

foreach ( $cache_refresh as $key => $value) {
 	if($value == true) clear_cache($key);
	}

write_log();

?>
