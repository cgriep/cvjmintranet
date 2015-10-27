<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 05.01.2002
*/

$design['delimiter']            = ' > ';

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
$design['bodyrightmargin']	= '10';
$design['bodyleftmargin']	= '10';

// Tables
$design['table_begin']          = '<table width="100%" class="tabelle">';
$design['table_end']            = '</table>';
$design['main_table_begin'] 	= '<table width="100%" cellpadding="0" cellspacing="0" border="0"
bgcolor='.$default_background.'><tr>';
$design['main_table_end']	= '</tr></table>';
$design['left_td_begin']	= '<!--';

$design['left_td_end']		= '-->';

$design['right_td_begin']	= '<!--';

$design['right_td_end']		= '-->';

$design['center_td_begin']	= '<td width="100%" valign="top" bgcolor="#ffffff"><br />';

$design['center_td_end']	= '<p style="text-align: center" class="noprint">[ <a href="javascript:window.close()'.
//  get_url($id,'docinput[design]='.DEFAULT_DESIGN).
'">'.LANG_BACK.'</a> ]</p>
<script language="javascript">
<!--
window.print();
// -->
</script>
</td>';

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