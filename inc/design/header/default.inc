<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.10.2003
*/

?>
<center>
<table width="100%" height="78" cellpadding="0" cellspacing="0" border="0">
<tr>
<td width="180" height="78" bgcolor="#cccccc" valign="top" align="left" background="">
<? if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img src="img/small_edit/editmode_on.gif" style="background:gray" border="0" /></a>
<? } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>"><img src="img/small_edit/editmode_off.gif" border="0" /></a>
<? 
if($profile['simple_edit'] == 1) { ?>
<a href="<?=get_url($id, array('simple_edit' => 0))?>"><img src="img/small_edit/advanced_edit.gif" border="0" /></a>
<? } else { ?>
<a href="<?=get_url($id, array('simple_edit' => 1))?>"><img src="img/small_edit/simple_edit.gif" border="0" /></a>
<? } ?>
<? } ?>
&nbsp;
</td>
<td align="center">
<table width="180" height="78" background="img/default/yellow_box.png" cellpadding="6" cellspacing="0" border="0">
<tr>
<td align="left" valign="top"><?
if(SESSION_STATUS != 'ok' || $profile['editor'] != 1) {
?>Welcome to AWF! Log in and click on <img src="img/small_edit/editmode_on.gif" style="background:gray" border="0" /> to 
edit this 
page. Visit the <a href="admin/">admin area</a> to manage user accounts. Have fun :-)</td><? 
} else { 
?>Welcome to AWF's edit mode! Having fun?<br />
To preview this page press save and then click on <img src="img/small_edit/editmode_off.gif" border="0" />.<? 
} ?>
</tr>
</table>
</td>
<td bgcolor="#ffffff" align="right" width="191" valign="top">
<img src="img/default/awf.png" border="0" hspace="0" vspace="0" /></td>
</tr>
</table>
<table width="100%" cellpadding="1" cellspacing="0" border="0" bgcolor="#aaaaaa">
<tr>
<td width="180">&nbsp;</td>
<td align="left" bgcolor="#aaaaaa" style="color: white;">
<table cellpadding="0" cellspacing="0" border="0"><tr>
<?
// top level navigation

init_published_roots();

reset($published_roots);
	$pre = '';
        while (list ($docid, $value) = each ($published_roots)) {
                echo $pre;
                ?><td><a href="<?=get_url($docid)?>" style="color: white;<? if($root_parent_id == $docid || $docid == $id) echo 'font-weight: bold;'; ?>"><?=$value['title']?></a></td><?
                $pre = '<td>'.get_image_tag('img/nav_sep.png').'</td>';
                }
?>
</tr></table>
</td>
<td align="right" bgcolor="#aaaaaa" style="color: white;">
<table cellpadding="0" cellspacing="0" border="0"><tr>

<? if(SESSION_STATUS == 'ok') { 
?><td><a style="color: white;" title="Bye." href="<?=get_url($id, 'docinput[logout]=true')?>">Logout</a></td>
<td><?=get_image_tag('img/nav_sep.png')?></td><? 
} 

$qresult = sql_query ("SELECT id, name, lang FROM ".TABLE_FLAVOURS." WHERE published=1 ORDER BY name");
if (sql_num_rows($qresult) > 0) {
	$pre = '';
        while ($row = sql_fetch_row($qresult)) {
		echo $pre;
                ?><td><a href="<?=$self.'docinput[flavour]='.$row[0].'&docinput[lang]='.$row[2]?>" style="color: white;<? if($row[0] == $flavour) echo 'font-weight: bold;'; ?>"><?=$row[1]?></a></td><?
                $pre = '<td>'.get_image_tag('img/nav_sep.png').'</td>';
                }
	sql_free_result($qresult);
        }
?></tr></table>
</td></tr>
</table>


