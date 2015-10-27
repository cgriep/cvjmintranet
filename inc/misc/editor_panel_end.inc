<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/
?>
	<br /><br /><p style="text-align: center">
	<input type="hidden" name="docinput[save_doc_properties]" value="true" />
        <input type="hidden" name="id" value="<?=$id?>" />
        <input type="hidden" name="first_item" value="<?=$first_item?>" />
        <input type="hidden" name="back_dest" value="<?=$document['parent_id']?>" />

	<input type="submit" name="docinput[submit]" value="Back" title="Back to parent document." />&nbsp;&nbsp;
	<input type="submit" name="docinput[submit]" value="Reload" title="Reload this document. Useful to undo changes." />&nbsp;&nbsp;	
	<input type="submit" name="docinput[submit]" value="Save" title="Make your changes to this document permanent." />
	</p>
	</td></tr>
	</table> 
	</form>

<!-- AWF editor panel END -->



