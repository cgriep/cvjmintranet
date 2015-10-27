<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Christoph Griep (griep@oszimt.de)
        Last update: 26.07.2003
*/

global $design;
global $document;
$wert = $document[$value];
?>
<?=$title?><br />
<input type="text" name="docdata[<?=$value?>]" value="<?=date('d.m.Y H:i',$wert)?>" size="<?=$design['input_size']?>" />
<input type="hidden" name="docdatatype[<?=$value?>]" value="datetime" />
