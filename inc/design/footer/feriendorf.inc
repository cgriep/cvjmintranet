<?
/*
        Copyright (C) Terra Nostra www.terra-nostra.de
        Design J&ouml;rg Podlinski

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 23.06.2002
*/

$doc = get_nodedata (get_cmodule_id('laufschrift'), $flavour);
?>
<marquee class="noprint" scrollamount="<?=$doc['Scrollamount']?>" scrolldelay="<?=$doc['Delay']?>">
<b><?=$doc['Laufschrift']?></b>
</marquee>

<div class="linkeSeite noprint">
<a href="javascript:open_print_window()"><img src="img/small_edit/print.gif" hspace="0" vspace="0"
width="16" height="16" border="0" /></a>
&nbsp;<a href="javascript:open_print_window()">Drucken</a>
</div>
<div class="rechteSeite noprint">
<a href="#top"><img src="img/small_edit/up.gif" border="0" /></a>
 Nach oben
</div>
<br />
<div class="lbfooter">&copy; 2003-2022 powered by CVJM Feriendorf Herbstein, Germany</div>
