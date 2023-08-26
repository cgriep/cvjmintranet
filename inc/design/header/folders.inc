<table width="100%" order="0" cellpadding="0" cellspacing="0">
<tr>
<td height="45" background="img/design/header_bg.png">
&nbsp;[ Logo ]
</td>
<td align="right" background="img/design/header_bg.png">
<?php if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img src="img/small_edit/editmode_on.gif" style="background:gray" border="0" /></a>
<?php } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>"><img src="img/small_edit/editmode_off.gif" border="0" /></a>
<?php } ?>
</td>
</tr>
</table>
