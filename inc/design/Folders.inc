<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 23.06.2002
*/

$design['delimiter']            = ' :: ';

// Header and Footer 
$design['header']		= 'folders';
$design['footer']		= 'folders';

// Defaults
$design['default_background']	= '#ffffff';
$design['linkcolor']		= '#aaaaff';
$design['vlinkcolor']		= '#000044';
$design['textcolor']		= '#000000';
$design['table_header']		= '#bbbbbb';
$design['table_row_dark']	= '#DEDEDE';
$design['table_row_bright']	= '#eeeeee';
$design['table_cellpadding']	= 3;
$design['table_cellspacing']	= 0;
$design['table_border']		= 0;
$design['css']			= 'folders';
$design['close_img']		= 'img/creative_close.gif';
$design['minimize_img']		= 'img/creative_minimize.gif';
$design['maximize_img']		= 'img/creative_maximize.gif';
$design['input_size']           = 65;
$design['input_rows']		= 15;
$design['nav_bullet']           = 'img/default_nav.gif';
$design['arrow_bullet']         = 'img/default_arrow.gif';

// BODY tag
$design['bgcolor']		= $design['default_background'];
$design['bodymarginheight']	= 0;
$design['bodymarginwidth']	= 0;
$design['bodytopmargin']	= 0;
$design['bodybottommargin']	= 0;
$design['bodyrightmargin']	= 0;
$design['bodyleftmargin']	= 0;

// Tables
$design['table_begin']		= '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="'.
					$design['table_cellspacing'].'" border="'.$design['table_border'].'">';
$design['table_end']		= '</table>';
$design['main_table_begin'] 	= '<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor="'.$default_background.'"><tr>';
$design['main_table_end']	= '</tr></table>';
$design['left_td_begin']	= '
<td height="600" width="90" valign="top" align="left" bgcolor="'.$default_background.'" background="img/design/left_bg.png">
<img src="img/pixel.gif" border="0" height="10" width="90" /><br clear="all" />';

$design['left_td_end']		= '
</td>
<td height="600" width="10" valign="top" bgcolor="'.$default_background.'">
<img src="img/pixel.gif" width="10" border="0" /></td>
<td height="600" width="20" valign="top" bgcolor="'.$default_background.'">
<img src="img/pixel.gif" width="20" border="0" /></td>
';

$design['right_td_begin']	= '
<td height="600" width="20" valign="top" bgcolor="'.$default_background.'">
<img src="img/pixel.gif" width="20" border="0" /></td>
<td height="600" width="10" valign="top" bgcolor="'.$default_background.'">
<img src="img/pixel.gif" width="10" border="0" /></td>
<td width="175" valign="top" bgcolor="'.$default_background.'">
<img src="img/pixel.gif" border="0" height="20" width="175" /><br />';

$design['right_td_end']		= '</td><td
height="600" width="10" valign="top" bgcolor="'.$default_background.'">
<img src="img/pixel.gif" width="10" border="0" /></td>';

$design['center_td_begin']	= '<td width="100%" valign="top" bgcolor="'.$default_background.'"><br />';

$design['center_td_end']	= '<br /><br /></td>';

$design['module_begin']['right']  	= '
<table width="173" cellpadding="3" cellspacing="1" border="0" bgcolor="#aaaaaa">
<tr><td bgcolor="#dddddd" background="">
<table width="100%" background="img/pixel.gif" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left"><b>%%title%%</b></td>
<td align="right">%%buttons%%</td></tr></table>
</td></tr>
';

$design['module_minimized']['right'] = '</table><br />';

$design['module_endtitle']['right']  = '<tr>
<td bgcolor="#dddddd" background="" align="left">';

$design['module_end']['right']     	= '</td></tr></table><br />';

$design['module_end']['left']     	= '';
$design['module_begin']['left']		= '';
$design['module_endtitle']['left']	= '';
$design['module_minimized']['left']	= '';

include(INC_PATH.'misc/design_functions.inc');

?>
