<?php

/*
This is a gallery module for the awf. It's job is managing the thumbnail module and combining multiple to a Picturegallery.
There are some functions which should make using the thumbnail module easier.
author Christoph Stickel <mixer2@web.de>
last update 27.3.02
--------------
v. 1.0
update: 20040129 by jesse@ilk.com
+ made thumbnail doctype name variable and configurable from the edit page
+ make 'add', 'delall', and 'deletet' work...
+ added icon display
--------------
v. 1.0.a
update: 20040125 by wolfgang@schaefernet.de
+ cosmetic correction for the display of the gallery names:
  - omit font tag so that default css style is used
  - display description in a separate table line - needs 1.0.a of gallery_thumbnail.inc as well

*/
if(!isset($document['tn_doctype'])) { $document['tn_doctype'] = 'gallery_thumbnail'; }
if(!isset($document['ppp'])) { $document['ppp'] = 10; }
//setup the non-standard form vars.
$add = $_POST['add'];
$delall = $_POST['delall'];
$deletet = $_POST['deletet[]'];

//DO EDITOR STUFF
if($profile['editor'] == '1') {
	if($add != '1') {
		echo  '	<table width="1" border="0" cellpadding="0" cellspacing="0">
			<tr><td colspan="2" nowrap>Thumbnail DocType name: (required!)</td></tr><tr><td colspan="2" nowrap><input type="text" name="docdata[tn_doctype]" size="30" value="'.$document['tn_doctype'].'"><br><br></td></tr>
			<tr><td colspan="2" nowrap>Headline (optional)</td></tr><tr><td colspan="2" nowrap><input type="text" name="docdata[headline]" size="30" value="'.$document['headline'].'"><br><br></td></tr>
			<tr><td colspan="2" nowrap>Description (optional)</td></tr><tr><td colspan="2" nowrap><textarea name="docdata[descr]" cols="60" rows="5">'.$document['descr'].'</textarea><br><br></td></tr>
			<tr><td colspan="2" nowrap>Linkcount</td></tr>
			<tr><td nowrap>Max per Page<br><input type="text" name="docdata[ppp]" size=2 maxlength=2 value="'.$document['ppp'].'"><br><br></td></tr>
			<tr><td colspan="2" nowrap>Other options</td></tr>
			<tr><td  nowrap><input type="checkbox" name="add" value="1">Add New Gallery</td><td nowrap><input type=checkbox name="delall" value="1">Remove All</td></tr></table>
			';
		}
	if($add == '1') {
		$nodeid = create_node($id, $doctypes[$document['tn_doctype']], count_children($doctypes[$document['tn_doctype']])+1, 1);
		add_nodedata($nodeid, 'title', 'Thumbnails number '.count_children($doctypes[$document['tn_doctype']]), $flavour);
		echo '<center>[ <a href="'.$PHP_SELF.'?id='.$nodeid.'">Configure new thumbnails</a> ] [ <a href="'.$PHP_SELF.'?id='.$id.'">Back</a> ]<br><br></center>';
		}
	if(isset($deletet)) {
 		foreach($deletet as $delkey) {
			destroy_node($delkey);
			}
		}
	if($delall == '1') {
		$result = get_children(0, count_children($doctypes[$document['tn_doctype']]), $doctypes[$document['tn_doctype']], 0, 'DESC');
		while(list($key, $value) = each($result)) { destroy_node($key); }
		}
	}
	if($add != '1') {

	echo '<center><table width="100%" border="0" cellpadding="1" cellspacing="1">';


	if(isset($document['headline']) && $document['headline'] != "" && $profile['editor'] != '1') {
		echo '<tr><td colspan="2"><h3>'.$document['headline'].'</h3>';
		}
	if(isset($document['descr']) && $document['descr'] != "" && $profile['editor'] != '1') {
		echo '<tr><td colspan="2">'.nl2br($document['descr']);
		}
	echo '<br><br></td></tr>';
	$repeater2 = 1;
	if(!isset($repeater)) {$repeater="1";}
	$result = get_children($repeater-1, $document['ppp'], $doctypes[$document['tn_doctype']], 1, 'DESC');
	while(list($key, $value) = each($result))
		{
		echo '<tr><td width="5" align="left" valign="top">';
		//show the icon or a blank space
	        if($value['icon'] != ''){
        	         echo transform($image_rpath.'news/'.$value['icon'], 'image_left');
	        } else {
			 echo transform($image_rpath.'gallery/bullet.gif', 'image_left');
		}
		echo '</td>';

		echo '<td align="left" valign="top" width="100%"><a href="'.$PHP_SELF.'?id='.$key.'"><b>'.$value['title'].'</b></a><br>'.$value['descr'].'</td>';

		if($profile['editor'] == '1') {
			echo '<td nowrap width=1%><input type=checkbox name="deletet[]" value="'.$key.'"> REMOVE</td>';
			}
		else  {
			echo '</tr>';
			}
		}
	echo '<tr><td colspan=2><br><br>';
	while($repeater2 < (count_children($doctypes[$document['tn_doctype']])+1))
		{
		echo ' [ <a href="'.$PHP_SELF.'?repeater='.$repeater2.'&id='.$id.'">'.$repeater2.'-'.($repeater2+($document['ppp']-1)).'</a> ] ';
		$repeater2 = $repeater2+$document['ppp'];
		}
	echo '</td></tr></table></center>';
	}
?>