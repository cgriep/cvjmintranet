<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 03.06.2002
*/

$children =  get_children (0, 100, $doctypes['paragraph']);

$inc_value = 10;
if(!isset($first_item) || !$first_item) $first_item = 0;
$max_newscount = $first_item + $inc_value;
 
$total_count=0;

if(isset($children)) {
	$total_count = count_children($doctypes['paragraph'], $flavour);
        }

if($profile['editor']!='1') {

if(isset($_REQUEST['action']) && $_REQUEST['action'] == "send_email") {
?>
	<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
	<input type="hidden" name="id" value="<?=$id?>">
	<input type="hidden" name="docinput[save]" value="send_email">
	<p>
	<b><?=LANG_SEND_THIS_DOCUMENT?></b><br />
	<input type="text" name="docinput[email]" size="50"><br />
	<?=LANG_MULTIPLE_RECIPIENTS?>
	<br />
	<br />
	<?php if(SESSION_STATUS != 'ok') { ?>
	<?=LANG_YOUR_EMAIL_ADDRESS?><br />
	<input type="text" name="docinput[sender]" size="50"><br />
	<br />
	<?php } 
	else { ?>
	<input type="hidden" name="docinput[sender]" value="<?=SESSION_EMAIL?>">	
	<?php } ?>
	<?=LANG_MESSAGE?><br />
	<input type="text" name="docinput[body]" size="50"><br />
	<br />
	<input type="submit" value="Send">
	</form>
	<?php
	}
elseif(isset($docinput['save']) && $docinput['save'] == "send_email" && isset($docinput['email']) && strlen($docinput['email']) > 3 && isemail($docinput['sender'])) {
	if(isset($children)) {
	reset($children);
		if($docinput['sender'] == '') { $docinput['sender'] = 'unknown_user@'.$SERVER_NAME; }
		$counter = 0;
		if($docinput['body'] != '') {
			$message_body = $docinput['body']."\n\n";
			}
		else {
			$message_body = '';
			}
	
		foreach ($children as $docid => $value) {
			$message_body .= transform($value['title'],'clean')."\n".
			transform(transform($value['body'], $value['datatype']['body']), 'html2txt')."\n\n";
			}
		$message_body .= "--------------------------------------------------------\n\n";
		$message_body .= LANG_EMAIL_SENT_BY.' '.email_to_string($docinput['sender'])."\n";
		$message_body .= LANG_ORIGINAL_DOC."\n".get_url($id, '', true)."\n";
	mail($docinput['email'], '['.SITE_TITLE.'] '.transform($document['title'],'clean'), $message_body,
		"From: ".email_to_string($docinput['sender']).' <'.$docinput['sender'].'>');
	?>
	<p><?=LANG_DOC_SENT_TO?> <b><?=$docinput['email']?></b></p>
	<?php
	}
	}

if(isset($children)) {
	reset($children);
	$counter = 0;
	while (list ($docid, $value) = each ($children) and $counter < $max_newscount) {
		if($value['type_id']==$doctypes['paragraph']) {
			$counter++;
		        if($counter > $first_item) {
				include(INC_PATH.'misc/paragraph.inc');
				}
			}
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
           echo '<br /><br />';
           }	 
//	if(isset($parents)) {
//		echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_BACK.'</a> ] ');
//	        }	
	if(!isset($action) && $document['allow_forward'] == '1') {
		echo '[ <a href="'.get_url($id,'action=send_email').'">'.LANG_SEND_TO_A_FRIEND.'</a> ]';
		}

}
else { 
	echo("<p><b>".LANG_NO_CONTENT."</b></p>"); 
	} 
}
 
elseif($profile['editor']=='1') {

if(isset($children)) {
        reset($children);
        $counter = 0;
        while (list ($docid, $value) = each ($children) and $counter < $max_newscount) {
                if($value['type_id']==$doctypes['paragraph']) {
			$counter++;
		        if($counter > $first_item) {
				include(INC_PATH.'misc/paragraph.inc');
        	              	}
                        }
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

?>
<p>
<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr>
<td nowrap="nowrap"><?php create_editor_input('User comments at bottom','comments','onoff_input'); ?></td>
<td nowrap="nowrap"><?php create_editor_input('Show published files at bottom','show_files','onoff_input'); ?></td>
<td nowrap="nowrap"><?php create_editor_input('Send to a friend','allow_forward','onoff_input'); ?></td>
</tr></table>
<?php
echo '<br />';
// Image Input
create_editor_input('Image','image','image_input');
echo '<br /><br />';
// Text Input
create_editor_input('Summary','summary','area_input');


        }

else {
        echo("<p><b>".LANG_NO_CONTENT."</b></p>");
        }
}
?>
