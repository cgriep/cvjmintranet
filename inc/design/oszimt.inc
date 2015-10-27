<?php
/*	This is the oszimt Design
   	maintained by Christoph Griep <griep@oszimt.de>



*/

$design['delimiter']            = ' &gt; ';

// Header and Footer
$design['header']		= 'oszimt';
$design['footer']		= 'oszimt';

// Defaults
$design['default_background']	= '#ffffff';
$design['linkcolor']		= '#6699cc';
$design['vlinkcolor']		= '#6699cc';
$design['textcolor']		= '#000000';
$design['table_header']		= '#aaaaaa';
$design['table_row_dark']	= '#dddddd';
$design['table_row_bright']	= '#eeeeee';
$design['css']			= 'oszimt';
$design['close_img']		= 'img/oszimt_close.gif';
$design['minimize_img']		= 'img/oszimt_minimize.gif';
$design['maximize_img']		= 'img/oszimt_maximize.gif';
$design['input_size']           = 60;
$design['input_rows']		= 15;
$design['nav_bullet']           = 'img/oszimt_nav.gif';
$design['arrow_bullet']         = 'img/oszimt_arrow.gif';
$design['table_cellpadding']    = 3;
$design['table_cellspacing']    = 0;
$design['table_border']         = 0;

// BODY tag
$design['bgcolor']		= '#ffffff';
$design['bodymarginheight']	= '0';
$design['bodymarginwidth']	= '0';
$design['bodytopmargin']	= '0';
$design['bodybottommargin']	= '0';
$design['bodyrightmargin']	= '0';
$design['bodyleftmargin']	= '0';

// Tables
$design['table_begin']          = '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="'.
                                        $design['table_cellspacing'].'" border="'.$design['table_border'].'">';
$design['table_end']            = '</table>';
$design['main_table_begin'] 	= '<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor="'.$design['default_background'].'"><tr>';
$design['main_table_end']	= '</tr></table>';
$design['left_td_begin']	= '
<td height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" alt="" /></td>
<td height="600" width="175" valign="top" align="left" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" border="0" height="20" width="175" alt="" />';

$design['left_td_end']		= '<p style="text-align: center">
<a href="http://freshmeat.net/projects/awf/"><img src="img/built_with_awf.png" border="0" alt="awf" /></a></p></td>
<td height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" alt="" /></td>
<td height="600" width="20" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="20" border="0" alt="" /></td>
';

$design['right_td_begin']	= '
<td height="600" width="20" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="20" border="0" alt="" /></td>
<td height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" alt="" /></td>
<td width="175" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" border="0" height="20" width="175" alt="" /><br />';

$design['right_td_end']		= '</td><td
height="600" width="10" valign="top" bgcolor="'.$design['default_background'].'">
<img src="img/pixel.gif" width="10" border="0" alt="" /></td>';

$design['center_td_begin']	= '<td width="100%" valign="top" bgcolor="'.$design['default_background'].'"><br />';

$design['center_td_end']	= '<br /><br /></td>';

$design['module_begin']['left']  	= '
<table width="173" cellpadding="3" cellspacing="0" border="0" bgcolor="'.$design['default_background'].'">
<tr><td bgcolor="'.$design['table_row_bright'].'">
<table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left"><b>%%title%%</b></td>
<td align="right">%%buttons%%</td></tr></table>
</td></tr>
';

$design['module_minimized']['left'] = '</table><br />';

$design['module_endtitle']['left']  = '<tr>
<td bgcolor="'.$design['table_row_dark'].'" align="left">';

$design['module_end']['left']     	= '</td></tr></table><br />';
$design['module_end']['right']     	= $design['module_end']['left'];
$design['module_begin']['right']     	= $design['module_begin']['left'];
$design['module_endtitle']['right']	= $design['module_endtitle']['left'];
$design['module_minimized']['right']	= $design['module_minimized']['left'];

include(INC_PATH.'misc/design_functions.inc');

?>