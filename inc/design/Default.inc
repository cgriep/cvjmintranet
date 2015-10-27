<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$design['delimiter']		= ' :: ';

// Header and Footer
$design['header']		= "default";
$design['footer']		= "default";

// Defaults
$design['default_background']	= "#ffffff";
$design['linkcolor']		= "#000066";
$design['vlinkcolor']		= "#000044";
$design['textcolor']		= "#000000";
$design['table_header']		= "#aaaaaa";
$design['table_row_dark']	= "#dedede";
$design['table_row_bright']	= "#eeeeee";
$design['table_cellpadding']    = 3;
$design['table_cellspacing']    = 0;
$design['table_border']         = 0;
$design['css']                  = 'default';
$design['close_img']            = 'img/default_close.gif';
$design['minimize_img']         = 'img/default_minimize.gif';
$design['maximize_img']         = 'img/default_maximize.gif';
$design['input_size']		= 60;
$design['input_rows']           = 15;
$design['nav_bullet']           = 'img/default_nav.gif';
$design['arrow_bullet']         = 'img/default_arrow.gif';

// BODY tag
$design['bgcolor']		= $design['default_background'];
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
$design['main_table_begin'] 	= '<table width="100%" cellpadding="0" cellspacing="0" border="0" bgcolor='.$default_background.'><tr>';
$design['main_table_end']	= '</tr></table>';

$design['left_td_begin']	= '<td height="500" width="180" valign="top" align="center" bgcolor="#cccccc"> 
<img src="img/pixel.gif" border="0" height="20" width="180" />';

$design['left_td_end']		= '</td><td height="500" width="20" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="20" border="0" /></td>';

$design['right_td_begin']	= '<td height="500" width="20" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="20" border="0" /></td><td width="180" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" border="0" height="20" width="180" /><br />';

$design['right_td_end']		= '</td><td height="500" width="10" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="10" border="0" /></td>';

$design['center_td_begin']	= '<td width="100%" valign="top" bgcolor="#ffffff" align="left"><br />';

$design['center_td_end']	= '<br /><br /></td>';

$design['module_begin']['left']	= '<table width="170" cellpadding="2" cellspacing="0" border="0" bgcolor="#ffffff"><tr>
<td bgcolor="#aaaaaa" align="left">%%title%%</td></tr>';

$design['module_begin']['right']= '<table width="170" cellpadding="2" cellspacing="0" border="0"><tr>
<td bgcolor="#aaaaaa" align="left">%%title%%</td><td bgcolor="#aaaaaa" align="right">%%buttons%%</td></tr>';

$design['module_minimized']['left'] = '</table><br />';

$design['module_minimized']['right'] = '</td></tr></table><br />';

$design['module_endtitle']['left']  = '<tr><td bgcolor="#dddddd" align="left">';
$design['module_endtitle']['right']  = '<tr><td colspan="2" bgcolor="#dddddd" align="left">';

$design['module_end']['left']     	= '</td></tr></table><br />';
$design['module_end']['right']     	= '</td></tr></table><br />';

include(INC_PATH.'misc/design_functions.inc'); 

?>
