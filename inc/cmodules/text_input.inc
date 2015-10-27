<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 01.01.2002
*/

global $design;

?>

<?=$title?><br />
<input type="text" name="docdata[<?=$value?>]" value="<?=$document[$value]?>" size="<?=$design['input_size']?>" />
<input type="hidden" name="docdatatype[<?=$value?>]" value="text" />
