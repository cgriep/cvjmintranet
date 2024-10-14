<?php
/*
        23.06.2002: Adapted to XHTML 1.0 [michael@liquidbytes.net]
*/
?>
<table width="100%" cellpadding="0" cellspacing="0" border="0">
<tr>
<td  height="50" bgcolor="#FFFFFF" valign="middle" >
&nbsp;&nbsp;<a href="http://www.oszimt.de"><img src="img/oszimtlogo.gif" border="0" alt="OSZ IMT"/></a>
</td>
<td width="90%" align="center"><strong><big><big>
Oberstufenzentrum Informations- und Medizintechnik</big><br />
Informationen zum Unterricht in den Klassen von Herrn Griep</big></strong></td>
<td width="10%" align="right" valign="middle">
&nbsp;&nbsp;<a href="http://www.berlin.de"><img src="img/berlinlogo.gif" border="0" alt="Berlin" /></a>
<?php if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img src="img/small_edit/editmode_on.gif"
style="background:gray" border="0" alt="Editmode" /></a>
<?php } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>"><img
src="img/small_edit/editmode_off.gif" border="0" alt="Editmode aus" /></a>
<?php
if($profile['simple_edit'] == 1) { ?>
<a href="<?=get_url($id, array('simple_edit' => 0))?>">
<img src="img/small_edit/advanced_edit.gif" border="0" alt="Advanced Edit" /></a>
<?php } else { ?>
<a href="<?=get_url($id, array('simple_edit' => 1))?>">
<img src="img/small_edit/simple_edit.gif" border="0" alt="Edit" /></a>
<?php } ?>
<?php } ?>
</td></tr>
</table>
<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor="#ffffff"><tr>
<td style="color: #6699cc; text-align: center; font-weight: bold" nowrap="nowrap">
<?php
        init_parents();
        if(isset($parents)) {
                reset($parents);
                while (list($docid, $value) = each ($parents)) {
                        $section_title = '<a href="'.get_url($docid).'">'.$value['title'].'</a> &gt; '.$section_title;
                        }
                }
        echo replace_wildcards($section_title." ".$document['title'], $profile);
?>
</td></tr></table>