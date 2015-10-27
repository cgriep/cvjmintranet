<?php
load_lang('user');
global $profile;
echo $design['table_begin'];
echo '<tr bgcolor="'. $design['table_row_dark'] . '"><td>';
echo '<b>'. LANG_USERZUSATZ_INFO_SUCHT.'</b></td></tr>';
echo '<tr bgcolor="'.$design['table_row_bright'].'"><td>';
echo 'Bezirk<br />';
echo '<input type="text" name="docinput[Bezirk]" value="'.$profile['Bezirk'].'" size="20" />';
echo '</td></tr>'.$design['table_end'];
echo '<br /><br />';

?>