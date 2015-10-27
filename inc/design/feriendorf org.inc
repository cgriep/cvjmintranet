<?php
/*
        Copyright (C) 2003 TerraNostra www.terra-nostra.de
        Design created by Jörg Podlinski  12.06.2003

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 05.01.2002
*/

// This is an example config file for design issues

$design['delimiter']            = ' &gt; ';

// Header and Footer
$design['header']               = "feriendorf";
$design['footer']               = "feriendorf";

// Defaults
$design['default_background']   = "#ffffff";
$design['fd_background']        = "#ffffff";
$design['linkcolor']            = "#1E560D";
$design['vlinkcolor']           = "#59B706";
$design['textcolor']            = "#045F0D";
$design['table_header']         = "#CAFED1";
$design['table_row_dark']       = "#DEDEDE";
$design['table_row_bright']     = "#EEEEEE";
$design['table_cellpadding']    = 2;
$design['table_cellspacing']    = 2;
$design['table_border']         = 0;
$design['css']                  = 'feriendorf';
$design['close_img']            = 'img/design/feriendorf/fd_close.gif';
$design['minimize_img']         = 'img/design/feriendorf/fd_minimize.gif';
$design['maximize_img']         = 'img/design/feriendorf/fd_maximize.gif';
$design['input_size']           = 60;
$design['input_rows']           = 15;
$design['nav_bullet']           = 'img/design/feriendorf/fd_nav.gif';
$design['arrow_bullet']         = 'img/design/feriendorf/fd_arrow.gif';

// BODY tag
$design['bgcolor']              = $design['fd_background'];
$design['bodymarginheight']     = '0';
$design['bodymarginwidth']      = '0';
$design['bodytopmargin']        = '0';
$design['bodybottommargin']     = '0';
$design['bodyrightmargin']      = '0';
$design['bodyleftmargin']       = '0';

// Tables
$design['table_begin']          = '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="'.
                                        $design['table_cellspacing'].'" border="'.$design['table_border'].'">';
$design['table_end']            = '</table>';
$design['main_table_begin']     = '<table width="100%" cellpadding="0" cellspacing="0" border="0"
bgcolor="'.$design['default_background'].'"><tr><a id="top" name="top"></a>';
$design['main_table_end']       = '</tr></table>';

// Linke Spalte
$design['left_td_begin']        = '<td height="560" width="175" valign="top" align="left" bgcolor="#ffffff"
>
<img src="img/pixel.gif" border="0" height="20" width="175" />';

$design['left_td_end']          = '<p class="zentriert">
<a href="http://www.cvjm-feriendorf.de/"><img src="img/design/feriendorf/cvlogo1.gif" border="0" /></a></p>
</td><td height="560" width="20" valign="top" bgcolor="#ffffff">
<img src="img/design/feriendorf/fd_pixel.gif" width="20" border="0" /></td>';

// Rechte Spalte
$design['right_td_begin']        = '<td height="560" width="20" valign="top" bgcolor="#ffffff">
<img src="img/design/feriendorf/fd_pixel.gif" width="20" border="0" /></td><td width="175" valign="top" bgcolor="#ffffff">
<img src="img/design/feriendorf/fd_pixel.gif" border="0" height="20" width="175" /><br />';

$design['right_td_end']                = '</td><td height="560" width="20" valign="top" bgcolor="#ffffff">
<img src="img/design/feriendorf/fd_pixel.gif" width="20" border="0" /></td>';

// Mittlerer Block
$design['center_td_begin']        = '<td width="100%" valign="top" bgcolor="#ffffff"><br />';

$design['center_td_end']        = '<br /><br /></td>';

// Modulinhalt linke Spalte
$design['module_begin']['left']        = '<table width="173" cellpadding="3" cellspacing="0" border="0" bgcolor="#aaaaaa"
><tr>
<td bgcolor="#ffffff">
<table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left">%%title%%</td><td align="right">%%buttons%%</td></tr></table>
</td></tr>
';
$design['module_endtitle']['left']  = '<tr>
<td bgcolor="#ffffff" align="left">';

$design['module_end']['left']       = '</td></tr></table><br />';

$design['module_minimized']['left'] = '</table><br />';

// Modulinhalt rechte Spalte

$design['module_minimized']['right'] = '</td></tr><tr>
<td bgcolor="#f5f5f5" align="left" width="173">
<img src="img/design/feriendorf/fd_pixel.gif" border="0" height="2" /></td></tr></table><br />';

$design['module_end']['right']             = '
</td></tr><tr><td bgcolor="#f5f5f5" align="left" width="173">
<img src="img/design/feriendorf/fd_pixel.gif" border="0" height="2" />
</td></tr></table><br />';

$design['module_begin']['right']= '<table width="173" cellpadding="4" cellspacing="0" border="0"><tr>
<td bgcolor="#CAFED1" align="left" width="173">
<table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr><td align="left">%%title%%</td><td align="right">%%buttons%%</td></tr></table>
</td></tr>
';

$design['module_endtitle']['right']  = '<tr><td
bgcolor="#f5f5ff" align="left" >';

include(INC_PATH.'misc/design_functions.inc');

?>