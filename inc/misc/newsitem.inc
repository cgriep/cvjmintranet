<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if($value['title'] != '' || $profile['editor'] == 1) {
	if($value['title'] == '') { $value['title'] = 'Not yet translated.'; }
	echo '<p class="justify"><span class="newsDate">'.transform($value['timestamp'], 'date')."</span><br>";
        echo '<b><a href="'.get_url($docid,'first_item='.$first_item).'">'.transform($value['title'],'clean').'</a></b>';
	$value['body'] = transform($value['body'], 'clean');
        if(strlen($value['body']) > 200) {
        	$value['body'] = substr($value['body'],0,200);
	        $rpos = strrpos($value['body']," ");
                $value['body'] = substr($value['body'],0,$rpos).'...';
                }
        echo('<br>'.transform($value['body'], $value['datatype']['body']).'
        	<a href="'.get_url($docid,'first_item='.$first_item).'">'.LANG_MORE.'</a>');
        if(COUNT_VIEWS == 1) {
        	echo('<br>( '.$value['views'].' '.LANG_VIEWS.' | '.count_children($doctypes['comment'], $flavour,
                	$docid).' '.LANG_COMMENTS.' )</p>');
        	}
        else {
        	echo('</p>');
                }
        }
?>