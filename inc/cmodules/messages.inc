<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

load_lang('messages');
load_lang('user');

$search = $_REQUEST['search'];

if(SESSION_STATUS == 'ok' && $profile['editor'] != '1') {
	if($document['image'] != '' || $document['body'] != '') {
                        echo '<p class="justify">';
                        // Image
                        echo transform($document['image'], $document['datatype']['image']);
                        // Text
                        echo transform($document['body'], $document['datatype']['body']).'</p>';
                        }

	if(isset($docinput['remove_message'])) remove_message($docinput['remove_message'], SESSION_DBID);
	
	?>
	<form action="<?=$_SERVER['PHP_SELF']?>" method="get">
	<input type="hidden" name="id" value="<?=$id?>" />
	<input type="text" name="search" value="<?=stripslashes($search)?>" size="30" />&nbsp;
	<input type="submit" value="<?=LANG_SEARCH?>" />
	</form><br />
	<table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="<?=$design['table_header']?>">
	<?php

	// $search set?
	if(!isset($search)) $search == '';

	$messages = search_messages(SESSION_DBID, '', -1, time(), -1, '', $search, true, 'DESC', time());

	if(count($messages) == 0) echo LANG_NO_MESSAGES;

	foreach ( $messages as $key => $value)  { ?>
		<tr bgcolor="<?=$design['table_row_dark']?>">
		<td><?=printf(LANG_MESSAGES_IM_SENT_BY, transform($value['sender_id'], 'user_id'), transform($value['created'], 'datetime'))?>
		<?php if($value['status'] == '0') echo ' <span style="color:#ff0000">'.LANG_NEW.'</span>'; ?></td>
		<td align="right" nowrap>[ <a href="<?=get_url($id, 'docinput[remove_message]='.$key)?>"><?=LANG_DELETE?></a> ]</td></tr>
		<tr bgcolor="<?=$design['table_row_bright']?>"><td colspan="2"><?=transform($value['message'], 'usertext')?></td></tr>
		<?php
		}
	?>
	</table>
	<?php
	}
elseif($profile['editor']=='1') {
	// Image Input
	create_editor_input('Image','image','image_input');
	echo '<br /><br />';
	// Text Input
	create_editor_input('Body','body','area_input');
	}

?>
