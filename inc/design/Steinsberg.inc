<?php
/*        Dieses Design basiert auf: Community Design
           maintained by Christoph Stickel <mixer2@web.de>
           Angepasst durch Poddel <poddel@terra-nostra.de>

        12.05.2003
*/

$design['delimiter']            = ' &gt; ';

// Header and Footer
$design['header']                = 'Steinsberg';
$design['footer']                = 'Steinsberg';

// Defaults
$design['default_background']        = '#000000';
$design['linkcolor']                = '#dbb600';
$design['vlinkcolor']                = '#dbb600';
$design['textcolor']                = '#aaaaaa';
$design['table_header']                = '#111111';
$design['table_row_dark']        = '#222222';
$design['table_row_bright']        = '#333333';
$design['css']                        = 'steinsberg';
$design['close_img']                = 'img/community_close.gif';
$design['minimize_img']                = 'img/community_minimize.gif';
$design['maximize_img']                = 'img/community_maximize.gif';
$design['input_size']           = 60;
$design['input_rows']                = 15;
$design['nav_bullet']           = 'img/community_nav.gif';
$design['arrow_bullet']         = 'img/community_arrow.gif';
$design['table_cellpadding']    = 3;
$design['table_cellspacing']    = 0;
$design['table_border']         = 0;

// BODY tag
$design['bgcolor']                = $design['default_background'];
$design['bodymarginheight']        = '0';
$design['bodymarginwidth']        = '0';
$design['bodytopmargin']        = '0';
$design['bodybottommargin']        = '0';
$design['bodyrightmargin']        = '0';
$design['bodyleftmargin']        = '0';

// Tables
$design['table_begin']          = '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="'.
                                        $design['table_cellspacing'].'" border="'.$design['table_border'].'">';
$design['table_end']            = '</table>';
$design['main_table_begin']         = '<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor='.$default_background.'><tr>';
$design['main_table_end']        = '</tr></table>';
$design['left_td_begin']        = '
<td height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" /></td>
<td height="600" width="175" valign="top" align="left" bgcolor="'.$design['default_background'].'" background="">
<img src="img/pixel.gif" border="0" height="20" width="175" />';

$design['left_td_end']                = '<p style="text-align: center">
<img src="../img/alogoklein.gif" border="0" />
</p></td>
<td height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" /></td>
<td height="600" width="20" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="20" border="0" /></td>
';

$design['right_td_begin']        = '
<td height="600" width="20" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="20" border="0" /></td>
<td height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" /></td>
<td width="175" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" border="0" height="20" width="175" /><br />';

$design['right_td_end']                = '</td><td
height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" /></td>';

$design['center_td_begin']        = '<td width="100%" valign="top" bgcolor="'.$design['default_background'].'"><br />';

$design['center_td_end']        = '<br /><br /></td>';

$design['module_begin']['left']          = '
<table width="173" cellpadding="3" cellspacing="0" border="0" bgcolor="'.$design['default_background'].'">
<tr><td bgcolor="'.$design['table_row_bright'].'" background="">
<table width="100%" background="img/pixel.gif" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left"><b>%%title%%</b></td>
<td align="right">%%buttons%%</td></tr></table>
</td></tr>
';

$design['module_minimized']['left'] = '</table><br />';

$design['module_endtitle']['left']  = '<tr>
<td bgcolor="'.$design['table_row_dark'].'" background="" align="left">';

$design['module_end']['left']             = '</td></tr></table><br />';
$design['module_end']['right']             = $design['module_end']['left'];
$design['module_begin']['right']             = $design['module_begin']['left'];
$design['module_endtitle']['right']        = $design['module_endtitle']['left'];
$design['module_minimized']['right']        = $design['module_minimized']['left'];

include(INC_PATH.'misc/design_functions.inc');

?>