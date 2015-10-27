<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 20.09.2003
*/

$design['delimiter']            = ' &gt; ';

// Header and Footer 
$design['header']		= "printable";
$design['footer']		= "empty";

// Defaults
$design['default_background']	= "#ffffff";
$design['linkcolor']		= "#000000";
$design['vlinkcolor']		= "#000000";
$design['textcolor']		= "#000000";
$design['table_header']		= "#bbbbbb";
$design['table_row_dark']	= "#DEDEDE";
$design['table_row_bright']	= "#ffffff";
$design['css']			= "printable";
$design['close_img']		= "img/creative_close.gif";
$design['minimize_img']		= "img/creative_minimize.gif";
$design['maximize_img']		= "img/creative_maximize.gif";
$design['input_size']           = 60;
$design['input_rows']           = 20;
$design['table_cellpadding']    = 2;
$design['table_cellspacing']    = 2;
$design['table_border']         = 0;
$design['nav_bullet']           = 'img/default_nav.gif';
$design['arrow_bullet']         = 'img/default_arrow.gif';

// BODY tag
$design['bgcolor']		= $default_background;
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
$design['main_table_begin'] 	= '<table width="100%" cellpadding="5" cellspacing="0" border="0" 
bgcolor='.$default_background.'><tr>';
$design['main_table_end']	= '</tr></table>
<script language="javascript">
<!--
window.print();
// -->
</script>';

$design['left_td_begin']	= '<!--';

$design['left_td_end']		= '-->';

$design['right_td_begin']	= '<!--';

$design['right_td_end']		= '-->';

$design['center_td_begin']	= '<td width="100%" valign="top" bgcolor="#ffffff"><br />';

$design['center_td_end']	= '</td>';

$design['module_begin']['left']	= '';

$design['module_minimized']['left'] = '';

$design['module_endtitle']['left']  = '';

$design['module_end']['left']  		= '';
$design['module_end']['right']  	= $design['module_end']['left'];
$design['module_begin']['right']	= $design['module_begin']['left'];
$design['module_endtitle']['right']	= $design['module_endtitle']['left'];
$design['module_minimized']['right']	= $design['module_minimized']['left'];

function include_module () {
	return true;
	}

?>
