<?php
/*
        Copyright (C) Terra Nostra www.terra-nostra.de
        Design J&ouml;rg Podlinski

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 23.06.2002
        

*/

//echo "ID: " . get_cmodule_id("Laufschrift");
$doc = get_nodedata (get_cmodule_id("Laufschrift"), $flavour);
/* echo $doc['Title'];
echo "Count: " . count($doc);
*/
?>
<marquee scrollamount="<?=$doc['Scrollamount']?>" scrolldelay="<?=$doc['Delay']?>">
<b><?=$doc['Laufschrift']?></b>
</marquee>

<br><br>
<table widht="100%"><tr><td width="185"></td>
<td><a href="javascript:open_print_window()"><?=get_image_tag('img/small_edit/print.gif')?></a>
&nbsp;<a href="javascript:open_print_window()">Drucken</a></td>
</tr></table>
