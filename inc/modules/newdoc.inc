<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $doctypes;
global $document;
global $docparents;
global $docvisible;
global $docdescriptions;
global $id;
global $first_item;
global $docinput;

if($profile['editor']) { 
		$pubcheck = true;
		?>
                <form action="<?=$_SERVER['PHP_SELF']?>" method="get">
                <span title="The document type tells AWF how to display the new document.">Document Type</span><br />
                <select name="docinput[doctype]">
                <?php
                reset($doctypes);
                while (list ($docname, $value) = each ($doctypes)) {
			if($docdescriptions[$value] != '') { 
				$docname = $docdescriptions[$value]; 
				if(strlen($docname) > 18) { $docname = substr($docname, 0, 18).'...'; }
				}
                        if($docparents[$value] == $document['type_id']) { $sel=' SELECTED'; $pubcheck = false; } else { $sel=''; }
                        if($docvisible[$value]) {
				echo('<option value="'.$value.'"'.$sel.'>'.$docname.'</option>');
				}
                        }
                ?>
                </select>
		<input type="hidden" name="docinput[docenabled]" value="1" />
                <input type="hidden" name="docinput[docdescription]" value="" />
                <br />
		<span title="The higher the value, the higher the order.">Sort Order</span><br />
		<input type="text" size="16" name="docinput[docsort_order]" value="<?=(count_children() + 1)?>" />
                <br />
		<span title="ID of the next higher document in hierarchy (0 means n/a).">Parent document ID</span><br />
		<input type="text" size="16" name="docinput[docparent_id]" value="<?=$id?>" />
                <input type="hidden" name="docinput[create_new_node]" value="true" />
                <input type="hidden" name="id" value="<?=$id?>" />
                <input type="hidden" name="first_item" value="<?=$first_item?>" />
                <br />
                <input type="checkbox" name="docinput[docpublished]"<?php if($pubcheck) { echo 'CHECKED'; } ?> />&nbsp;
		<span title="If enabled, the new document appears in the navigation/overviews.">Published</span>
		<br />
                <br />
                <input type="submit" value="Create" />
                </form>
		<?
		}

