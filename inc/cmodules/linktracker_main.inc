<?php

/* 	This is a guestbook content module for the AWF. 
	maintained by Christoph Stickel <mixer2@web.de>		*/

$childcount = count_children($doctypes['linktracker_posting']);
load_lang('linktracker');

if(!isset($first_item) || !is_numeric($first_item)) { $first_item = 0; }
if(!isset($document['ppp']) || $document['ppp'] < 1) { $document['ppp'] = '20'; add_nodedata ($id, 'ppp', 20, 0); }
if(!isset($document['maxposts']) || $document['maxposts'] < 1) { $document['maxposts'] = '100'; add_nodedata ($id, 'maxposts',100,0); }
if(!isset($document['thx']) || $document['thx'] == '') { $document['thx'] = 'Thank you!'; add_nodedata($id, 'thx', thx, $flavour); }

if(isset($docinput['savevote']) && $docinput['savevote'] != '' && $docinput['votevalue'] < 11 && $docinput['votevalue'] >= 0) {
	$votenode = get_nodedata($docinput['savevote'], $flavour);
	if($votenode['votings'] == '') $votenode['votings'] = 0;
	if($votenode['votes'] == '') $votenode['votes'] = 0;
	add_nodedata($docinput['savevote'], 'votings', $docinput['votevalue']+$votenode['votings'], 0);
	add_nodedata($docinput['savevote'], 'votes', $votenode['votes']+1, 0);
	unset($votenode);
	}

if($profile['editor']=='1') {
	if(isset($docinput['deleteposting'])) { foreach($docinput['deleteposting'] as $delkey) { destroy_node($delkey); } }
	if($docinput['delall'] == "true") {
		$result = get_children(0, $childcount, $doctypes['linktracker_posting'], 0, 'DESC');
		while(list($key, $value) = each($result)) { destroy_node($key);	}
		}
	
	create_editor_input ('Headline', 'headline', 'text_input');
	echo '<br /><br />';
	create_editor_input ('Description', 'descr', 'area_input');
	echo '<br /><br />';
	create_editor_input ('Words of thanks', 'thx', 'area_input');
	?><br /><h4>Postcount</h4>
	Max per Page<br /><input type="text" name="docdata[ppp]" size="2" maxlength="2" value="<?=$document['ppp']?>">
	<br /><br />Max total Posts<br /><input type="text" name="docdata[maxposts]" size="3" maxlength="3" value="<?=$document['maxposts']?>">
	<input type="hidden" name="docdataflavour[ppp]" value="0">
	<input type="hidden" name="docdataflavour[maxposts]" value="0"><?
	$result = get_sorted_children($first_item, 100, 'sorttime',$doctypes['linktracker_posting']);
	if($childcount > 0) {
		?><br /><br /><?=$design['table_begin']?><tr bgcolor="<?=$design['table_header']?>">
		<td width="10%" nowrap>Nickname</td>
		<td width="98%" nowrap>Message</td>
		<td width="1%" nowrap>Edit</td>
		<td width="1%" nowrap>Remove</td></tr><?
		}
	while(list($key, $value) = each($result)) {
		?><tr bgcolor = "<?=$design['table_row_dark']?>">
		<td nowrap>[ <?
		if(isset($value['userid'])) {
			echo transform($value['userid'], 'user_id');
			}
		else {
			?><a href="mailto:<?=transform($value['email'], 'clean')?>"><?=transform($value['nick'], 'clean')?></a><?
			}
		?> ]</td><td nowrap><?=transform(substr($value['posting'],0,40), 'clean')?><?
		if(strlen($value['posting'])>40){echo '...';}
		?></td><td nowrap> [ <a href="<?=get_url($key)?>">Edit</a> ]</td>
		<td nowrap> <input type="checkbox" name="docinput[deleteposting][]" value="<?=$key?>"> Remove</td></tr><?
		}
	if($childcount > 0) {
		?><tr bgcolor="<?=$design['table_row_dark']?>">
		<td colspan="3" width="99%"> </td>
		<td nowrap width="1%"><input type="checkbox" name="docinput[delall]" value="true"> Remove All</td></tr><?
		echo $design['table_end'];
		}
	if($childcount > 100)  {
	   		echo 'Page: ';
	   		$temp_counter = 100;
	   		while(100 + $childcount > $temp_counter) {
	  			$topic_counter += 1;
				echo '[ ';
	  			if($first_item == ($temp_counter - 100)) { echo '<b>'; }
	  			echo '<a href="'.get_url($id, 'first_item='.($temp_counter - 100)).'">'.((($topic_counter-1)*100)+1).'-'.($topic_counter*100).'</a>';
	   			if($first_item == ($temp_counter - 100)) { echo '</b>'; }
	    			echo ' ] ';
				$temp_counter += 100;
	    			if($topic_counter >= 30) { break; }
	    			}
			}
	}
elseif($profile['editor']!='1' && !isset($docinput['newmessage'])) {
		if(isset($document['headline']) && $document['headline'] != "") { echo '<h3>'.transform($document['headline'], 'clean').'</h3>'; }
	 	if(isset($document['descr']) && $document['descr'] != "" && $document['descr'] !="Unknown") {	echo transform($document['descr'], $document['datatype']['descr']).'<br /><br />'; }
	 	
		
		$result = get_sorted_children ($first_item ,$document['ppp'], 'sorttime',$doctypes['linktracker_posting']);
		if($childcount > 0) echo '<br />'.$design['table_begin'];
		while(list($key, $value) = each($result)) {
			?><tr bgcolor = "<?=$design['table_row_dark']?>"><td width="27%" nowrap><?=LANG_NICKNAME?> [ <? 
			if(isset($value['userid'])) {
				echo transform($value['userid'], 'user_id');
				}
			else {
				if(ereg('@',$value['email'])) { 
					?><a href="mailto:<?=transform($value['email'], 'clean')?>"><?=transform($value['nick'], 'clean')?></a><?
					}
				else { echo transform($value['nick'], 'clean'); }
				}
			?> ] </td><td width=70%><?=LANG_URL?> [ <?
			if(strlen($value['title']) > 11 )  { echo'<a href="'.transform($value['title'], 'clean').'" target="_blank">'.$value['title'].'</a>'; }
			else { echo 'none'; }
			?> ]</td><td width="1%" nowrap><?
			if(SESSION_DBID == $value['userid'] && SESSION_STATUS == 'ok') {
				?>[ <a href="<?=get_url($key)?>"><?=LANG_EDIT?> ]<?
				}
			?></td>
			<td width="1%" nowrap><?=LANG_GUESTBOOK_POSTEDAT?> <?=transform($value['sorttime'], 'datetime')?></td></tr>
			<tr bgcolor="<?=$design['table_header']?>"><td width="100%" colspan="4" nowrap>
			<form action="<?=get_url($id)?>" method="post"><?=LANG_RATING?> <select name="docinput[votevalue]"><?
			for($i = 0; $i<11; $i++) { ?><option value="<?=$i?>"><?=$i?></option><? }			
			unset($i);
			?></select><input type="submit" value="<?=LANG_VOTE?>">
			<input type="hidden" name="docinput[savevote]" value="<?=$key?>">
			<?
			if($value['votings'] == '') $value['votings'] = 0;
			if($value['votes'] == '') $value['votes'] = 0;
			?>
			<?=LANG_RATE?> <?=round($value['votings']/$value['votes'],2)?>
			<?=LANG_VOTES?> <?=$value['votes']?>
			</td></tr></form>
			<tr bgcolor="<?=$design['table_row_bright']?>"><td colspan="4">
			<?=transform($value['posting'], 'usertext')?></td></tr>
			 <tr><td colspan="4"><img src="img/pixel.gif" width="1" height="10" border="0" /></td></tr><?
			}
	if($childcount > 0) echo $design['table_end'];
	if($childcount > $document['ppp']) {
	   		echo LANG_GUESTBOOK_PAGE.' ';
	    		$temp_counter = $document['ppp'];
	   		while($document['ppp'] + $childcount > $temp_counter) {
	  	   		$topic_counter += 1;
	  	   		echo '[ ';
	 	   		if($first_item == ($temp_counter - $document['ppp'])) { echo '<b>'; }
		   		echo '<a href="'.get_url($id, 'first_item='.($temp_counter - $document['ppp'])).'">';
		   		echo ((($topic_counter-1)*$document['ppp'])+1).'-'.($topic_counter*$document['ppp']).'</a>';
	   	   		if($first_item == ($temp_counter - $document['ppp'])) { echo '</b>'; }
	   	   		echo ' ] ';
		   		$temp_counter += $document['ppp'];
	   	   		if($topic_counter >= ($document['maxposts']/$document['ppp'])) { break; }
	   	  		}
			}
			?><form action="<?=get_url($id)?>" method="post">
	 	<input type="hidden" name="docinput[newmessage]" value="1">
	 	<?
		if(SESSION_STATUS != 'ok'){
			?><?=LANG_NICKNAME?><br /><input type="text" name="docinput[nick]" size="<?=$design['input_size']?>" maxlength="30" >
	 		<br /><?=LANG_EMAIL_ADDRESS?><br /><input type="text" name="docinput[email]" size="<?=$design['input_size']?>" maxlength="30" >
	 		<br /><?
			}
		?><?=LANG_URL?><br /><input type="text" name="docinput[url]"  size="<?=$design['input_size']?>" maxlength="60" value="http://">
	 	<br /><?=LANG_COMMENTS?><br /><textarea name="docinput[posting]" cols="<?=$design['input_size']?>" rows="5"></textarea>
	 	<br /><input type="submit" value="<?=LANG_GUESTBOOK_SIGNIN?>">
	 	</form><?
	}
elseif(isset($docinput['newmessage'])) {
	if(((isemail($docinput['email']) && $docinput['nick'] != "") || SESSION_STATUS == 'ok') && $docinput['posting'] != '' && strlen($docinput['url']) > 11) {
		$nodeid = create_node($id, $doctypes['linktracker_posting'], $childcount, 1, 0, '');
		if(SESSION_STATUS != 'ok') {
			add_nodedata($nodeid, 'email', $docinput['email'], 0, 'clear');
			add_nodedata($nodeid, 'nick', $docinput['nick'], 0, 'clear');
			}
		else	{
			add_nodedata($nodeid, 'userid', SESSION_DBID, 0, 'clear');
			}
		add_nodedata($nodeid, 'title', $docinput['url'], 0, 'clear');
		add_nodedata($nodeid, 'sorttime', time(), 0);
		add_nodedata($nodeid, 'posting', $docinput['posting'], 0, 'usertext');
		add_nodedata($nodeid, 'timestamp', time(), 0);
		echo '<center><br /><br />'.transform($document['thx'],$document['datatype']['thx']).'<br /><br /><br /> [ <a href="'.get_url($id).'">'.LANG_BACK.'</a> ]</center>';
		}
	else  {
		echo '<center><br /><br />'.LANG_GUESTBOOK_INVALID.'<br /><br /><br /> [ <a href="'.get_url($id).'">'.LANG_BACK.'</a> ]</center>';
		}
	}

?>
