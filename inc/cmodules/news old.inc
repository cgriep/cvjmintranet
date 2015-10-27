<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 16.05.2002
*/
$pchildren = get_children (0, 100, $doctypes['paragraph']);

$inc_value = 10;
if(!isset($first_item) || !$first_item) $first_item = 0;
$max_newscount 	= $first_item + $inc_value;

$children = get_sorted_children ($first_item, $inc_value, 'timestamp', $doctypes['newsitem']);

$total_count = count_children($doctypes['newsitem'], $flavour);

if($profile['editor']!='1') {

if(isset($pchildren)) {
        reset($pchildren);
        while (list ($docid, $value) = each ($pchildren)) {
                if($value['title'] != '' || $value['body']) {
			include(INC_PATH.'misc/paragraph.inc');
			}
                }
        }
 
if(isset($children)) {
        reset($children);
        while (list ($docid, $value) = each ($children)) {
		include(INC_PATH.'misc/newsitem.inc');
                }
	  if($total_count > $inc_value) {
	   echo LANG_PAGE.' ';
	   $temp_counter = $inc_value;
	   while($inc_value + $total_count > $temp_counter) {
	    $news_counter += 1;
	    if($first_item == ($temp_counter - $inc_value)) { echo '<b>'; }
	    echo '<a href="'.get_url($id,'first_item='.($temp_counter - $inc_value)).'">'.$news_counter.'</a> ';
	    if($first_item == ($temp_counter - $inc_value)) { echo '</b>'; }
	    $temp_counter += $inc_value;
	    if($news_counter >= 30) { break; }
	    }
	   if($total_count > $first_item + $inc_value) {
	    echo '&nbsp;<a href="'.get_url($id,'first_item='.($first_item + $inc_value)).'">'.LANG_NEXT.'</a>';
	    }
	   echo '<br>';
	   }
	printf('<p>'.LANG_THERE_ARE_X_NEWS.'</p>', $total_count);
	}
	}
elseif($profile['editor']=='1') {

if(isset($pchildren)) {
        reset($pchildren);
        while (list ($docid, $value) = each ($pchildren)) {
                if($value['title'] != '') {
			include(INC_PATH.'misc/paragraph.inc');
			}
                }
        }

if(isset($children)) {
        reset($children);
        $counter = 0;
        while (list ($docid, $value) = each ($children)) {
		include(INC_PATH.'misc/newsitem.inc');
                }
	  if($total_count > $inc_value) {
	   echo 'Page ';
	   $temp_counter = $inc_value;
	   while($inc_value + $total_count > $temp_counter) {
	    $news_counter += 1;
	    if($first_item == ($temp_counter - $inc_value)) { echo '<b>'; }
	    echo '<a href="'.get_url($id,'first_item='.($temp_counter - $inc_value)).'">'.$news_counter.'</a> ';
	    if($first_item == ($temp_counter - $inc_value)) { echo '</b>'; }
	    $temp_counter += $inc_value;
	    if($news_counter >= 30) { break; }
	    }
	   if($total_count > $first_item + $inc_value) {
	    echo '&nbsp;<a href="'.get_url($id,'first_item='.($first_item + $inc_value)).'">Next</a>';
	    }
	   echo '<br>';
	   }
	printf('<p>'.LANG_THERE_ARE_X_NEWS.'</p>', $total_count);
        }
	}
else {
        echo("<b>Sorry, there's no content in your flavour yet.</b>");
        }
 
?>
