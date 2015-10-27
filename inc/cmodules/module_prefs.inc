<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if($profile[editor] == 1) {
	create_editor_input('Message for users who are not logged in','denied','area_input');
	echo('<br /><br />');
	create_editor_input('Some notes for registered users','notes','area_input');
	echo('<br /><br />');
	
	$qresult = sql_query("SELECT id, caption, visible FROM ".TABLE_MODULES." WHERE removeable = 1 AND (target = 0 OR
			target = 2) ORDER BY sort_order");

	if(sql_num_rows($qresult) > 0) {
		while($row = sql_fetch_row($qresult)) {
			create_editor_input('Description of module <b> '.$row[1].'</b>', 'mod_info_'.$row[0], 'text_input');
			echo('<br /><br />');
			}
		sql_free_result($qresult);
		}
	} 
else {

if(SESSION_STATUS == 'ok') { 
	if(isset($document['notes'])) { echo '<p>'.transform($document['notes'],$document['datatype']['notes']).'</p>'; }
	?>
	<p>
	<form action="<?$_SERVER['PHP_SELF']?>" method="post">
	<input type="hidden" name="id" value="<?=$id?>" />
	<input type="hidden" name="docinput[save]" value="modules" />
	<table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="<?=$design['table_row_dark']?>">
	<?php	
	
	$qresult = sql_query("SELECT id, caption, visible FROM ".TABLE_MODULES." WHERE removeable = 1 AND (target = 0 OR
			target = 2) ORDER BY sort_order");

	if(sql_num_rows($qresult) > 0) {
		while($row = sql_fetch_row($qresult)) {
			echo('<tr><td width="5%" valign="top">');
			if(($row[2] == 1 && !isset($profile['module_'.$row[0]])) || $profile['module_'.$row[0]] == 1) { 
				$modvisible = ' CHECKED'; } 
			else { $modvisible = ''; }
			echo('<input type="checkbox" value="1" name="docinput[module_'.$row[0].']"'.$modvisible.'></td><td width="95%">
				<b>'.$row[1].'</b><br>');
			if(isset($document['mod_info_'.$row[0]]) && strlen($document['mod_info_'.$row[0]]) > 1) { 
				echo($document['mod_info_'.$row[0]].'</td></tr>');
				}
			else {
				echo('</td></tr>');
				}
			}
		sql_free_result($qresult);
		}
	
	?>
	</table><br />
	<input type="submit" value="<?=LANG_SAVE?>" />
	</form>
	</p>
	<?php
	}
elseif(SESSION_STATUS != 'ok') {
	if(isset($document[denied])) { echo '<p>'.$document[denied].'</p>'; }
	}

}
?>
