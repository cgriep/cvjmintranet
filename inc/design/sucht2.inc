<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 05.01.2002
*/

// This is an example config file for design issues

$design['delimiter']                = ' &gt; ';

// Header and Footer
$design['header']                = "sucht";
$design['footer']                = "sucht2";

// Defaults
$design['default_background']        = "#ffffff";
$design['linkcolor']                = "#000066";
$design['vlinkcolor']                = "#000044";
$design['textcolor']                = "#000000";
$design['table_header']                = "#D4DAFE";
$design['table_row_dark']        = "#DEDEDE";
$design['table_row_bright']        = "#EEEEEE";
$design['table_cellpadding']    = 2;
$design['table_cellspacing']    = 2;
$design['table_border']         = 0;
$design['css']                  = 'sucht2';
$design['close_img']            = 'img/default_close.gif';
$design['minimize_img']         = 'img/default_minimize.gif';
$design['maximize_img']         = 'img/default_maximize.gif';
$design['input_size']                = 60;
$design['input_rows']           = 15;
$design['nav_bullet']           = 'img/default_nav.gif';
$design['arrow_bullet']         = 'img/default_arrow.gif';

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
$design['main_table_begin']         = '<table width="100%" cellpadding="0" cellspacing="0" border="0"
bgcolor="'.$default_background.'"><tr>';
$design['main_table_end']        = '</tr></table>';
$design['left_td_begin']        = '<td class="SpalteLinks">';

$design['left_td_end']                = '<div class="zentriert">Unterstützt durch<br /> 
<a href="/redirect.php?url=http://www.schule.de">
  <img src="img/odslogo.gif" alt="Offenes deutsches Schulnetz" />
</a><br />
<a href="/redirect.php?url=http://www.oszimt.de">
  <img src="http://img.oszimt.de/logo/oszimtlogo.gif" alt="OSZ IMT" />
</a><br />
<a href="/redirect.php?url=http://www.taz.de">
  <img src="http://www.taz.de/taz/archiv/gifs/taz48mm.gif" alt="TAZ" />
</a><br />
<a href="/redirect.php?url=http://freshmeat.net/projects/awf/">
  <img src="img/built_with_awf.png" alt="AWF" />
</a></div></td>';

$design['right_td_begin'] = '<td class="SpalteRechts">';
$design['right_td_end'] = '</td>';

$design['center_td_begin'] = '<td width="100%" valign="top" bgcolor="#ffffff"><br />';

$design['center_td_end']  = '<br /><br /></td>';

$design['module_begin']['left'] = '<div class="module_left">
<div class="module_ueberschrift_links">
<span class="rechteSeite">%%buttons%%</span>
%%title%%
</div>
';

$design['module_minimized']['left'] = '</div></div>';

$design['module_minimized']['right'] = '</div></div>';

$design['module_endtitle']['left']  = '<div class="module_endtitle">';

$design['module_end']['left']  = '</div></div>';
$design['module_end']['right'] = '</div></div>';

$design['module_begin']['right']= '<div class="module_right">
<div class="module_ueberschrift">
<span class="rechteSeite">%%buttons%%</span>
%%title%%
</div>';

$design['module_endtitle']['right']  = '<div class="module_endtitle_right">';

include(INC_PATH.'misc/design_functions.inc');

?>