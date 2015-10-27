<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $design;

?>

<?=$title?><br />
<input type="text" name="docdata[<?=$value?>]" value="<?=$document[$value]?>" size="<?=$design['input_size']?>" />
<input type="hidden" name="docdatatype[<?=$value?>]" value="text" />
