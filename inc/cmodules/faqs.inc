<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$children = get_sorted_children (0, 500, 'title', $doctypes['paragraph'], -1, 'ASC'); 

if($profile['editor']!='1') {

if(isset($action) && $action=='send_email') {
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
	<? if(SESSION_STATUS != 'ok') { ?>
	<?=LANG_YOUR_EMAIL_ADDRESS?><br />
	<input type="text" name="docinput[sender]" size="50"><br />
	<br />
	<? } 
	else { ?>
	<input type="hidden" name="docinput[sender]" value="<?=SESSION_EMAIL?>">	
	<? } ?>
	<?=LANG_MESSAGE?><br />
	<input type="text" name="docinput[body]" size="50"><br />
	<br />
	<input type="submit" value="Send">
	</form>
	<?
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
	
		while (list ($docid, $value) = each ($children)) {
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
	<?
	}
	}

if(isset($children)) {
	echo '<h1>'.transform($document['title'], $value['datatype']['title']).'</h1><ul>';
	reset($children);
	while (list ($docid, $value) = each ($children)) {
		echo '<li><a href="'.get_url($id).'#'.$docid.'">'.transform($value['title'], 
$value['datatype']['title']).'</a></li>';
		}
	echo '</ul><hr size="1" />';
	reset($children);
	while (list ($docid, $value) = each ($children)) {
		include(INC_PATH.'misc/faq.inc');
		}
	if(isset($parents)) {
		echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_BACK.'</a> ] ');
	        }	
	if(!isset($action)) {
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
        while (list ($docid, $value) = each ($children)) {
		if($value['title'] == '') $value['title'] = 'Unknown';
		include(INC_PATH.'misc/faq.inc');
                }

?>
<p>
<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr>
<td nowrap="nowrap"><? create_editor_input('User comments at bottom','comments','onoff_input'); ?></td>
<td nowrap="nowrap"><? create_editor_input('Show published files at bottom','show_files','onoff_input'); ?></td>
</tr></table>
<?
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
