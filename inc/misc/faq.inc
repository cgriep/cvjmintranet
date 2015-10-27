<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if($value['title'] != '') {
	if($profile['editor'] == '1') {
		global $docid;
		echo '<h2><img src="'.$design['arrow_bullet'].'" border="0" width="10" height="7" /> <a href="'.get_url($docid).'">'.transform($value['title'], $value['datatype']['title']).'</a></h2>';
		}
	else {
		echo '<h2><img src="'.$design['arrow_bullet'].'" border="0" width="10" height="7" /> <a name="'.$docid.'">'.transform($value['title'], $value['datatype']['title']).'</a></h2>';
		}
        }

if($value['image'] != '' || $value['body'] != '') {
	echo '<p class="justify" clear="all">';
        // Image
        echo transform($value['image'], $value['datatype']['image']);
        // Text
        echo transform($value['body'], $value['datatype']['body']).'</p>';
        }
?>

