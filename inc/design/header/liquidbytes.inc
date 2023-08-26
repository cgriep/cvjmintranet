<?php
/*
        Copyright (C) 2000-2002 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 23.06.2002
*/

?>

<table width=100% height="83" cellpadding=0 cellspacing=0 border=0>
<tr>
<td width="356" height="83" bgcolor="#000000" valign="top" background=""><img src="img/platine_left_top_old.jpg" border="0" width="356" height="83" /></td>
<td width="100%" bgcolor="#000000" background="img/platine_top.jpg" align="right" valign="top">
<?php if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img style="background:gray" src="img/small_edit/editmode_on.gif" border="0" /></a>
<?php } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>"><img src="img/small_edit/editmode_off.gif" border="0" /></a>
<?php } else { ?>&nbsp;<?php } ?>
</td>
</tr>
</table>
<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor="#000000"><tr><td style="text-align: center; color: #ffffff; font-weight: bold;">
<?php 
        init_parents();
        if(isset($parents)) {
                reset($parents);
                while (list($docid, $value) = each ($parents)) {
                        $section_title = '<a style="color:#ffffff" href="'.get_url($docid).'">'.$value['title'].'</a> &gt; '.$section_title;
                        }
                }
        echo replace_wildcards($section_title." ".$document['title'], $profile);
?>
</td></tr></table>


