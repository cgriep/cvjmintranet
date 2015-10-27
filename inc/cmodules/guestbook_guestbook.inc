<?php

/* 	This is a guestbook content module for the AWF. 
	maintained by Christoph Stickel <mixer2@web.de>		

	23.06.2002: Adapted to XHTML 1.0 [michael@liquidbytes.net]
*/

$childcount = count_children($doctypes['guestbook_posting']);
load_lang('guestbook');

if(!isset($first_item) || !is_numeric($first_item)) { $first_item = 0; }
if(!isset($document['ppp']) || $document['ppp'] < 1) { $document['ppp'] = '20'; add_nodedata ($id, 'ppp', 20, 0); }
if(!isset($document['maxposts']) || $document['maxposts'] < 1) { $document['maxposts'] = '100'; add_nodedata ($id, 'maxposts',100,0); }
if(!isset($document['thx']) || $document['thx'] == '') { $document['thx'] = 'Thank you!'; add_nodedata($id, 'thx', thx, $flavour); }

if($profile['editor']=='1') {
	if(isset($docinput['deleteposting'])) { foreach($docinput['deleteposting'] as $delkey) { destroy_node($delkey); } }
	if($docinput['delall'] == "true") {
		$result = get_children(0, $childcount, $doctypes['guestbook_posting'], 0, 'DESC');
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
	$result = get_sorted_children($first_item, 100, 'sorttime',$doctypes['guestbook_posting']);
	if($childcount > 0) {
		?><br /><br /><?=$design['table_begin']?><tr bgcolor="<?=$design['table_header']?>">
		<td width="10%" nowrap>Nickname</td>
		<td width="98%" nowrap>Message</td>
		<td width="1%" nowrap>Edit</td>
		<td width="1%" nowrap>Remove</td></tr><?
		}
	while(list($key, $value) = each($result)) {
		?><tr bgcolor = "<?=$design['table_row_dark']?>">
		<td nowrap>[ <a href="mailto:<?=transform($value['email'], 'clean')?>"><?=transform($value['title'], 'clean')?></a> ] </td>
		<td nowrap><?=transform(substr($value['posting'],0,40), 'clean')?><?
		if(strlen($value['posting'])>40){echo '...';}
		?></td><td nowrap> [ <a href="<?=get_url($key)?>">Edit</a> ]</td>
		<td nowrap> <input type="checkbox" name="docinput[deleteposting][]" value="<?=$key?>"> Remove</td></tr><?
		}
	if($childcount > 0) {
		?><tr bgcolor="<?=$design['table_row_dark']?>">
		<td colspan="3" width="99%">&nbsp;</td>
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
	 	?><form action="<?=get_url($id)?>" method="post">
         	<input type="hidden" name="id" value="<?=$id?>">
	 	<input type="hidden" name="docinput[newmessage]" value="1">
	 	<?=LANG_NICKNAME?><br /><input type="text" name="docinput[nick]" size="<?=$design['input_size']?>" maxlength="30" >
	 	<br /><?=LANG_EMAIL_ADDRESS?><br /><input type="text" name="docinput[email]" size="<?=$design['input_size']?>" maxlength="30" >
	 	<br /><?=LANG_GUESTBOOK_HOMEPAGE?><br /><input type="text" name="docinput[url]"  size="<?=$design['input_size']?>" maxlength="60" value="http://">
	 	<br /><?=LANG_MESSAGE?><br /><textarea name="docinput[posting]" cols="<?=$design['input_size']?>" rows="5"></textarea>
	 	<br /><input type="submit" value="<?=LANG_GUESTBOOK_SIGNIN?>">
	 	</form><?
		
		$result = get_sorted_children ($first_item ,$document['ppp'], 'sorttime',$doctypes['guestbook_posting']);
		if($childcount > 0) echo '<br />'.$design['table_begin'];
		while(list($key, $value) = each($result)) {
			?><tr bgcolor = "<?=$design['table_row_dark']?>"><td width="70%"><?=LANG_NICKNAME?> [ <? 
			if(ereg('@',$value['email'])) { ?><a href="mailto:<?=transform($value['email'], 'clean')?>"><?=transform($value['title'], 'clean')?></a><? }
			else { echo transform($value['title'], 'clean'); }
			?> ] <?=LANG_GUESTBOOK_HOMEPAGE?> [ <?
			if(strlen($value['url']) > 11 )  { echo'<a href="'.transform($value['url'], 'clean').'" target="_blank">'.$value['url'].'</a>'; }
			else { echo 'none'; }
			?> ]</td><td width="1%" nowrap><?=LANG_GUESTBOOK_POSTEDAT?> <?=transform($value['sorttime'], 'datetime')?>
			</td></tr><tr bgcolor="<?=$design['table_row_bright']?>"><td colspan="2"><?=transform($value['posting'], 'usertext')?>
			</td></tr><?
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
	}
elseif(isset($docinput['newmessage'])) {
	if(isemail($docinput['email']) && $docinput['nick'] != "") {
		$nodeid = create_node($id, $doctypes['guestbook_posting'], $childcount, 1, 0, '');
		add_nodedata($nodeid, 'email', $docinput['email'], 0);
		add_nodedata($nodeid, 'url', $docinput['url'], 0);
		add_nodedata($nodeid, 'sorttime', time(), 0);
		add_nodedata($nodeid, 'posting', $docinput['posting'], 0);
		add_nodedata($nodeid, 'title', $docinput['nick'], 0);
		add_nodedata($nodeid, 'timestamp', time(), 0);
		echo '<br /><br /><span style="text-align: center">'.transform($document['thx'],$document['datatype']['thx']).'<br /><br /><br /> [ <a href="'.get_url($id).'">'.LANG_BACK.'</a> ]</span>';
		}
	else  {
		echo '<span style="text-align: center"><br /><br />'.LANG_GUESTBOOK_INVALID.'<br /><br /><br /> [ <a href="'.get_url($id).'">'.LANG_BACK.'</a> ]</span>';
		}
	}
?>
