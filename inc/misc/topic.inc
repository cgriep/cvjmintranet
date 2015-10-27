<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if($value['title'] != '') {
	echo '<h2><a href="'.get_url($docid).'">'.transform($value['title'], $value['datatype']['title']).'</a></h2>';
        }

if($value['image'] != '' || $value['summary'] != '') {
	echo '<p class="justify">';
        // Image
        echo transform($value['image'], $value['datatype']['image']);
        // Text
        echo transform($value['summary'], $value['datatype']['summary']).'</p>';
	
	/* 
	if(COUNT_VIEWS == 1) {
        echo('<br>( '.$value['views'].' '.LANG_VIEWS.' | '.count_children($doctypes[comment],
	        $flavour, $docid).' '.LANG_COMMENTS.' )</p>');
        } 
	*/
	
        }

?>

