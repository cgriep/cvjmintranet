<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 05.01.2002
*/

// This is an example config file for design issues

$design['delimiter']		= ' &gt; ';

// Header and Footer
$design['header']		= "liquidbytes";
$design['footer']		= "liquidbytes";

// Defaults
$design['default_background']	= "#ffffff";
$design['linkcolor']		= "#000066";
$design['vlinkcolor']		= "#000044";
$design['textcolor']		= "#000000";
$design['table_header']		= "#D4DAFE";
$design['table_row_dark']	= "#DEDEDE";
$design['table_row_bright']	= "#EEEEEE";
$design['table_cellpadding']    = 2;
$design['table_cellspacing']    = 2;
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
$design['main_table_begin'] 	= '<table width="100%" cellpadding="0" cellspacing="0" border="0" 
bgcolor='.$default_background.'><tr>';
$design['main_table_end']	= '</tr></table>';
$design['left_td_begin']	= '<td height="600" width="175" valign="top" align="left" bgcolor="#ffffff" 
background="img/left_back.png">
<img src="img/pixel.gif" border="0" height="20" width="175" />';

$design['left_td_end']		= '<p style="text-align: center">
<a href="http://freshmeat.net/projects/awf/"><img src="img/built_with_awf.png" border="0" /></a></p>
</td><td height="600" width="20" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="20" border="0" /></td>';

$design['right_td_begin']	= '<td height="600" width="20" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="20" border="0" /></td><td width="175" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" border="0" height="20" width="175" /><br />';

$design['right_td_end']		= '</td><td height="600" width="20" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="20" border="0" /></td>';

$design['center_td_begin']	= '<td width="100%" valign="top" bgcolor="#ffffff"><br />';

$design['center_td_end']	= '<br /><br /></td>';

$design['module_begin']['left']	= '<table width=173 cellpadding=3 cellspacing=0 border=0 bgcolor="#aaaaaa" 
background=""><tr>
<td bgcolor="#ffffff" background="img/box_back2.png">
<table width="100%" background="img/pixel.gif" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left">%%title%%</td><td align="right">%%buttons%%</td></tr></table>
</td></tr>
';

$design['module_minimized']['left'] = '</table><br />';

$design['module_minimized']['right'] = '</td></tr><tr>
<td bgcolor="#f5f5f5" background="img/module_back_bottom_minimized.gif" align="left" width="173">
<img src="img/pixel.gif" border="0" height="2" /></td></tr></table><br />';

$design['module_endtitle']['left']  = '<tr>
<td bgcolor="#ffffff" background="img/box_back3.png" align="left">';

$design['module_end']['left']     	= '</td></tr></table><br />';
$design['module_end']['right']     	= '
<tr><td bgcolor="#f5f5f5" background="img/module_back_bottom.gif" align="left" width="173">
<img src="img/pixel.gif" border="0" height="2" />
</td></tr></table><br />';

$design['module_begin']['right']= '<table width="173" cellpadding="4" cellspacing="0" border="0"><tr>
<td bgcolor="#D4DAFE" background="img/module_back_top.gif" align="left" width="173">
<table width="100%" background="img/pixel.gif" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left">%%title%%</td><td align="right">%%buttons%%</td></tr></table>
</td></tr>
';

$design['module_endtitle']['right']  = '<tr><td
bgcolor="#f5f5ff" align="left" background="img/module_back_middle.gif">';

include(INC_PATH.'misc/design_functions.inc'); 

?>
