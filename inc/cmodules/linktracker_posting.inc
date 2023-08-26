<?php

/*	This is the guestbookposting content module for the AWF. It's a childmodule for the guestbook contentmodule.
	maintained by Christoph Stickel <mixer2@web.de>									*/

load_lang('linktracker');

if($docinput['savechanges'] == 1 && SESSION_STATUS == 'ok' && SESSION_DBID == $document['userid']) {
	if($docinput['removeposting'] == 'on') { 
		destroy_node($id); 
		?><b>&gt;&gt;</b> <font color="#00aa00"><?=LANG_DELSUCC?></font><?php
		$postingdeleted = '1';
		}
	elseif($docinput['posting'] != '' && strlen($docinput['url']) > 11) {
		update_nodedata ($id, 'title', $docinput['url'], 0, 'clear');
		update_nodedata ($id, 'posting', $docinput['posting'], 0, 'usertext');
		?><b>&gt;&gt;</b> <font color="#00aa00"><?=LANG_CHANGEDSUCC?></font><?php
		$document = array_merge(get_nodedata($id, $flavour), get_node($id, $flavour));
		}
	else {
		?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_CHANGEDFAIL?></font><?php
		}
	}

if($profile['editor']=='1') {
	create_editor_input ('E-Mail', 'email', 'text_input');
	echo '<br /><br />';
	create_editor_input ('Homepage', 'url', 'text_input');
	echo '<br /><br />';
	create_editor_input ('Message', 'posting', 'area_input');	
	?><input type="hidden" name="docdataflavour[email]" value="0">
	<input type="hidden" name="docdataflavour[url]" value="0">
	<input type="hidden" name="docdataflavour[posting]" value="0">
	<input type="hidden" name="docdataflavour[title]" value="0"><?php
	}
elseif($profile['editor']!='1') {
	if(SESSION_STATUS == 'ok' && SESSION_DBID == $document['userid'] && $postingdeleted != '1') {
		?><form action="<?=get_url($id)?>" method="post">
         	<input type="hidden" name="id" value="<?=$id?>">
	 	<input type="hidden" name="docinput[savechanges]" value="1">
	 	<?=LANG_URL?><br /><input type="text" name="docinput[url]"  size="<?=$design['input_size']?>" maxlength="60" value="<?=$document['title']?>">
	 	<br /><?=LANG_COMMENTS?><br /><textarea name="docinput[posting]" cols="<?=$design['input_size']?>" rows="5"><?=$document['posting']?></textarea>
	 	<br /><input type="checkbox" name="docinput[removeposting]">LANG_REMOVE
		<br /><br /><input type="submit" value="<?=LANG_EDIT?>">
	 	</form><?php
		}
	elseif(SESSION_STATUS == 'ok' && SESSION_DBID == $document['userid'] && $postingdeleted == '1') {
		echo '<center>[ <a href="'.get_url($document['parent_id']).'">'.LANG_BACK.'</a> ]</center>';
		}
	else {	echo $design['table_begin'];
		?><tr bgcolor = "<?=$design['table_row_dark']?>"><td width="28%"><?=LANG_NICKNAME?> [ <?php 
		if(isset($document['userid'])) {
			echo transform($document['userid'], 'user_id');
			}
		else {
			if(ereg('@',$document['email'])) { 
				?><a href="mailto:<?=transform($document['email'], 'clean')?>"><?=transform($document['nick'], 'clean')?></a><?php
				}
			else { echo transform($document['nick'], 'clean'); }
			}
		?> ] </td><td width=70%><?=LANG_URL?> [ <?php
		if(strlen($document['title']) > 11 )  { echo'<a href="'.transform($document['title'], 'clean').'" target="_blank">'.$document['title'].'</a>'; }
		else { echo 'none'; }
		?> ]</td><td width="1%" nowrap><?=LANG_GUESTBOOK_POSTEDAT?> <?=transform($document['sorttime'], 'datetime')?>
		</td></tr><tr bgcolor="<?=$design['table_row_bright']?>"><td colspan="3">
		<?=transform($document['posting'], 'usertext')?></td></tr><?php
		echo $design['table_end'];
		}
	}
?>