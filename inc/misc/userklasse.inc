<?php
load_lang('user');
global $profile;
echo $design['table_begin'];
echo '<tr bgcolor="'. $design['table_row_dark'] . '"><td>';
echo '<b>'. LANG_USERZUSATZ_INFO.'</b></td></tr>';
echo '<tr bgcolor="'.$design['table_row_bright'].'"><td>';
echo 'Klasse/Kurs (Bitte unbedingt angeben!)<br />';
echo '<input type="text" name="docinput[Klasse]" value="'.$profile['Klasse'].'" size="10" />';
echo '</td></tr>'.$design['table_end'];
echo '<br /><br />';

?>