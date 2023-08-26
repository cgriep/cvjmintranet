<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

load_lang('preferences');

if($profile['editor'] == 1) {
	create_editor_input('Message for users who are not logged in','denied','area_input');
	echo('<br /><br />');
	create_editor_input('Some notes for registered users','notes','area_input');
	} 
else {

if(SESSION_STATUS == 'ok') { 
	if(isset($document['notes'])) { echo '<p>'.transform($document['notes'], $document['datatype']['notes']).'</p>'; }
	
	if(isset($docinput['save']) && $docinput['save'] == 'prefs') {
		if($docinput['newsletter'] == '1') {
			set_profile('newsletter', '1');
			}
		else {
			set_profile('newsletter', '0');
			}
		if($docinput['im_popup'] == '1') {
			set_profile('im_popup', '0');
			}
		else {
			set_profile('im_popup', '1');
			}
		set_profile('im_default', $docinput['im_default']);
		}

	?>
	<p>
	<form action="<?$_SERVER['PHP_SELF']?>" method="post">
	<input type="hidden" name="id" value="<?=$id?>" />
	<input type="hidden" name="docinput[save]" value="prefs" />
	<table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="<?=$design['table_row_dark']?>">
	<input type="checkbox" name="docinput[newsletter]" value="1"<?if($profile['newsletter'] == '1') echo ' checked';?> /> <?=LANG_SUBSCRIBE_NEWSLETTER?><br />	
	<input type="checkbox" name="docinput[im_popup]" value="1"<?if($profile['im_popup'] == '0') echo ' checked';?> /> <?=LANG_DISABLE_IM_POPUP?><br />
	<br />
	<?=LANG_DEFAULT_MESSAGE_TYPE?><br />
	<select name="docinput[im_default]" size="1">
	<option value="instant"<?php if($profile['im_default'] == 'instant') echo ' selected';?>><?=LANG_INSTANT_MESSAGE?></option>
	<?php if(isset($profile['icq']) && is_numeric($profile['icq'])) { ?><option value="icq"<?php if($profile['im_default'] == 'icq') echo ' selected';?>>ICQ</option><?php } ?>
	<option value="email"<?php if($profile['im_default'] == 'email') echo ' selected';?>><?=LANG_EMAIL?></option>
	</select>
	</table><br>
	<input type="submit" value="<?=LANG_SAVE?>">
	</form>
	</p>
	<?php
	}
elseif(SESSION_STATUS != 'ok') {
	if(isset($document['denied'])) { echo '<p>'.transform($document['denied'], $document['datatype']['denied']).'</p>'; }
	}

}
?>
