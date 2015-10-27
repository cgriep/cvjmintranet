<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 14.09.2001
*/

?>

<table width=100% height="70" cellpadding=0 cellspacing=0 border=0>
<tr>
<td width="551" height="70" bgcolor="#000000" valign="top" background=""><img src="img/creative_left_top.gif" 
border="0" width="551" height="70"></td>
<td width="100%" bgcolor="#000000" align="right" valign="top"><? if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img src="img/small_edit/editmode_on.gif" style="background:gray" border="0" /></a>
<? } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>"><img src="img/small_edit/editmode_off.gif" border="0" /></a>
<? } ?>
<p style="color:#ffffff"><b>
<?php 
        init_parents();
        if(isset($parents)) {
                reset($parents);
                while (list($docid, $value) = each ($parents)) {
                        $section_title = '<a style="text-decoration:none; color:#aaaaff" href="'.get_url($docid).'">'.$value['title'].'</a> &gt; '.$section_title;
                        }
                }
        echo replace_wildcards($section_title." ".$document['title'], $profile);
?>
</b>&nbsp;
<br />
<?php echo time(); ?>&nbsp;
<?php 
if(SESSION_STATUS == 'ok') { echo '<br>'.SESSION_EMAIL.'&nbsp;&nbsp;'; }
?>
<br />
</td>
</tr>
</table>
<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor="#000000">
<tr colspan="2"><td background="img/creative_middle.gif">
<br />
</td></tr>
</table>


