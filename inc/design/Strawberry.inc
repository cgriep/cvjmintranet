<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 05.01.2002
*/

$design['delimiter']            = ' &gt; ';

// Header and Footer
$design['header']		= "strawberry";
$design['footer']		= "strawberry";

// Defaults
$design['default_background']	= "#ffffff";
$design['linkcolor']		= "#aa0000";
$design['vlinkcolor']		= "#000044";
$design['textcolor']		= "#000000";
$design['table_header']		= "#ffaaaa";
$design['table_row_dark']	= "#DEDEDE";
$design['table_row_bright']	= "#FFEEEE";
$design['table_cellpadding']    = 2;
$design['table_cellspacing']    = 2;
$design['table_border']         = 0;
$design['input_size']           = 60;
$design['input_rows']           = 20;
$design['close_img']	        = $image_rpath.'raqua_close.gif';
$design['minimize_img']  	= $image_rpath.'raqua_minimize.gif';
$design['maximize_img']  	= $image_rpath.'raqua_maximize.gif';
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
$design['css'] 			= 'strawberry';

// Tables
$design['table_begin']          = '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="'.
                                        $design['table_cellspacing'].'" border="'.$design['table_border'].'">';
$design['table_end']            = '</table>';
$design['main_table_begin'] 	= '<table width="100%" cellpadding="0" cellspacing="0" border="0" 
bgcolor='.$default_background.'><tr>';
$design['main_table_end']	= '</tr></table>';
$design['left_td_begin']	= '<td height="600" width="225" valign="top" align="left" bgcolor="#ffffff">
<img src="img/pixel.gif" border="0" height="20" width="225"><br />';

$design['left_td_end']		= '<p style="text-align: center">
<a href="http://freshmeat.net/projects/awf/"><img src="img/built_with_awf.png" border="0"></a></p>
</td><td height="600" width="20" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="20" border="0"></td>';

$design['right_td_begin']	= '<td height="600" width="20" valign="top" bgcolor="#ffffff" 
background="!img/points_right.gif">
<img src="img/pixel.gif" width="20" border="0"></td>
<td width="175" valign="top" bgcolor="#ffffff" align="left">
<img src="img/pixel.gif" border="0" height="20" width="175"><br />';

$design['right_td_end']		= '</td><td height="600" width="20" valign="top" bgcolor="#ffffff">
<img src="img/pixel.gif" width="20" border="0"></td>';

$design['center_td_begin']	= '
<td width="100%" valign="top" bgcolor="#ffffff">
<img src="img/topberry.gif" border="0" style="margin-left: 50%; margin-right: 50%" />
<br />
';

$design['center_td_end']		= '<br /><br /></td>';

$design['module_begin']['left']   	= '<img src="img/strawberry_top_b.jpg" /><br />
<table width="225" cellpadding=3 cellspacing=0 border=0 bgcolor="#ffffff"><tr>
<td bgcolor="#ffffff" background="img/strawberry_middle_1.gif" width="32">
<img src="img/pixel.gif" border="0" width="26" height="1">
</td><td bgcolor="#ffffff" width="171" align="center">
<table width="100%" background="img/pixel.gif" border="0" cellpadding="0" cellspacing="0">
<tr><td align="center"><b>=| %%title%% |=</b></td><td align="right">%%buttons%%</td></tr></table>
</td><td bgcolor="#ffffff" background="img/strawberry_middle_2.gif" width="22">
<img src="img/pixel.gif" border="0">
</td></tr>
';

$design['module_end']['right']  = '</p>';

$design['module_endtitle']['left']  = '
<tr>
<td bgcolor="#ffffff" background="img/strawberry_middle_1.gif" width="32">
<img src="img/pixel.gif" border="0" width="26" height="1">
</td><td bgcolor="#ffffff" width="171">
<img src="img/pixel.gif" border="0" width="165" height="1"><br />
';

$design['module_minimized']['left'] = '
</table><img src="img/strawberry_bottom.jpg" /><br />';

$design['module_minimized']['right'] = '</p>';

$design['module_end']['left']     	= 
'</td><td bgcolor="#ffffff" background="img/strawberry_middle_2.gif" width="22">
<img src="img/pixel.gif" border="0" width="16" height="1">
</td></tr></table><img src="img/strawberry_bottom.jpg" /><br />';
$design_module_end['right']     	= '<br /><br />';

$design['module_begin']['right']= '
<p align="left">
<table width="100%" background="img/pixel.gif" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left">
<img src="img/smallberry.gif" border="0" align="middle">
<b>%%title%%</b></td><td align="right">%%buttons%%</td></tr></table>
';

$design['module_endtitle']['right']  = '';

include(INC_PATH.'misc/design_functions.inc'); 
?>
