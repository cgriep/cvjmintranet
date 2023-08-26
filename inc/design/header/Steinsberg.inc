<?php
/*
        12.05.2003: Adapted to XHTML 1.0 [poddel@terra-nostra.de]
*/
?>
<table width=100% height="50" cellpadding=0 cellspacing=0 border=0>
<tr>
<td width="200" height="50" bgcolor="#000000" valign="top" background="img/the_c2.jpg"><img src="img/the_c.jpg" border="0" width="200" height="50"></td>
<td width="100%" align="right" background="img/the_c2.jpg" valign="top">
<?php if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img src="img/small_edit/editmode_on.gif" style="background:gray" border="0" /></a>
<?php } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>"><img src="img/small_edit/editmode_off.gif" border="0" /></a>
<?php } ?>
</td></tr>
</table>
<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor="#000000"><tr>
<td style="color: #dbb600; text-align: center; font-weight: bold" nowrap="nowrap">
<?php
        init_parents();
        if(isset($parents)) {
                reset($parents);
                while (list($docid, $value) = each ($parents)) {
                        $section_title = '<a style="color:#dbb600" href="'.get_url($docid).'">'.$value['title'].'</a> &gt; '.$section_title;
                        }
                }
        echo replace_wildcards($section_title." ".$document['title'], $profile);
?>
</td></tr></table>

