<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $design;
?>

<!-- AWF editor panel BEGIN -->
	<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
	<table width="100%" border="0" cellpadding="3" cellspacing="1">
	<tr><td>
	<b>Edit document (<span title="Each document has an unique ID as identifier.">ID <?=$id?></span>)</b>
	</td></tr>
	<tr><td>
<?

if($profile['simple_edit'] == 1) { ?>
	<input type="hidden" name="doc[description]" value="<?=$document['description']?>" />
	<input type="hidden" name="doc[searchable]" value="<?=$document['searchable']?>" />
	<input type="hidden" name="doc[cacheable]" value="<?=$document['cacheable']?>" />
	<input type="hidden" name="docdata[keywords]" value="<?=$document['keywords']?>" />
	<input type="hidden" name="doc[key_id]" value="<?=$document['key_id']?>" />
	<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr><td>
                <span title="The document type tells AWF how to display the current document.">Document Type</span><br />
                <select name="doc[type_id]">
                <?php
                reset($doctypes);
                while (list ($docname, $value) = each ($doctypes)) {
			if($docdescriptions[$value] != '') {
                                $docname = $docdescriptions[$value];
                                if(strlen($docname) > 20) { $docname = substr($docname, 0, 20).'...'; }
                                }
                        if($value == $document['type_id']) { $sel=' SELECTED'; } else { $sel=''; }
                        echo('<option value="'.$value.'"'.$sel.'>'.$docname.'</option>');
                        }
                ?>
                </select>
                </td><td>
		<input type="checkbox" name="docinput[remove_document]" value="1">
		&nbsp;<span title="Use this to permanently delete this document (it may still be in cache for some time).">Delete document</span>
		</td><td>
                <input type="checkbox" value="1" name="doc[enabled]"<?php if($document['enabled'] == 1) echo ' CHECKED'; ?>>
		&nbsp;<span title="Check to make this document available to users.">Enabled</span>
                </td><td>
                <input type="checkbox" value="1" name="doc[published]"<?php if($document['published'] == 1) echo ' CHECKED'; ?>>
		&nbsp;<span title="Check, if you want this document to appear in the navigation/overview.">Published</span>
                </td></tr><tr><td>
		<span title="The higher the value, the higher the order.">Sort Order</span><br />
		<input type="text" size="20" name="doc[sort_order]" value="<?=$document['sort_order']?>">
		</td><td>
		<span title="ID of the next higher document in hierarchy (0 means n/a).">Parent document ID</span><br />
		<input type="text" size="20" name="doc[parent_id]" value="<?=$document['parent_id']?>">
                </td><td>
		&nbsp;
		</td><td>
		</td></tr>
	</table>
        <br />
	Document Title (Note: HTML tags will be converted to text)<br />
	<input type="text" size="<?=$design['input_size']?>" name="docdata[title]" value="<?=htmlentities(transform($document['title'], $document['datatype']['title']))?>">
	<input type="hidden" name="docdatatype[title]" value="clean"><br clear="all" />
	</p>
<? } else { ?>
	<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr><td>
                <span title="The document type tells AWF how to display the current document.">Document Type</span><br />
                <select name="doc[type_id]">
                <?php
                reset($doctypes);
                while (list ($docname, $value) = each ($doctypes)) {
			if($docdescriptions[$value] != '') {
                                $docname = $docdescriptions[$value];
                                if(strlen($docname) > 20) { $docname = substr($docname, 0, 20).'...'; }
                                }
                        if($value == $document['type_id']) { $sel=' SELECTED'; } else { $sel=''; }
                        echo('<option value="'.$value.'"'.$sel.'>'.$docname.'</option>');
                        }
                ?>
                </select>
                </td><td>
		<span title="The higher the value, the higher the order.">Sort Order</span><br />
		<input type="text" size="20" name="doc[sort_order]" value="<?=$document['sort_order']?>">
		</td><td>
                <input type="checkbox" value="1" name="doc[enabled]"<?php if($document['enabled'] == 1) echo ' CHECKED'; ?>>
		&nbsp;<span title="Check to make this document available to users.">Enabled</span>
                </td><td>
                <input type="checkbox" value="1" name="doc[published]"<?php if($document['published'] == 1) echo ' CHECKED'; ?>>
		&nbsp;<span title="Check, if you want this document to appear in the navigation/overview.">Published</span>
                </td></tr><tr><td>
		<span title="Optional description that will be used in HTML meta tags.">Meta-Description</span><br />
		<input type="text" size="20" name="doc[description]" value="<?=$document['description']?>">
                </td><td>
		<span title="ID of the next higher document in hierarchy (0 means n/a).">Parent document ID</span><br />
		<input type="text" size="20" name="doc[parent_id]" value="<?=$document['parent_id']?>">
                </td><td>
		<input type="checkbox" value="1" name="doc[searchable]"<?php if($document['searchable'] == 1) echo ' CHECKED'; ?>>
                &nbsp;<span title="Check, if the search function is allowed to find this document.">Searchable</span>
                </td><td>
		<input type="checkbox" value="1" name="doc[cacheable]"<?php if($document['cacheable'] == 1) echo ' CHECKED'; ?>>
		&nbsp;<span title="Check, if you want this document to be cached.<? if(CACHE_TIME == 0) echo ' Note: You will have to enable the caching feature in admin/config.php too, as it is disabled at the moment.' ?>">Allow caching</span>
                </td></tr><tr><td>
		<span title="Optional keywords that will be used in HTML meta tags.">Meta-Keywords</span><br />
		<input type="text" size="20" name="docdata[keywords]" value="<?=$document['keywords']?>">
                </td><td>
		<span title="Use this to limit access to certain users/groups (0 means disabled)."> Key ID</span><br />
		<input type="text" size="20" name="doc[key_id]" value="<?=$document['key_id']?>">
                </td><td>
		<input type="checkbox" value="1" name="docinput[default_doc]"<?php if($default_document == $id) echo ' CHECKED'; ?>>
		&nbsp;<span title="If enabled, this document will be displayed in case a requested document is not available.">Default document</span>
                </td><td>
		<input type="checkbox" name="docinput[remove_document]" value="1">
		&nbsp;<span title="Use this to permanently delete this document (it may still be in cache for some time).">Delete document</span>
		</td></tr>
	</table>
	<?php
	if(!$document['timestamp']) { $date = 'UNKNOWN'; } else { $date = transform($document['timestamp'], 'datetime'); }
        if(!$document['author']) $document['author'] = 'UNKNOWN';
	echo '<p>Last update: '.$date.' by '.transform($document[author_id], 'user_id').'</p>';
	?>
        <p>
	Document Title (Note: HTML tags will be converted to text)<br />
	<input type="text" size="<?=$design['input_size']?>" name="docdata[title]" value="<?=htmlentities(transform($document['title'], $document['datatype']['title']))?>">
	<input type="hidden" name="docdatatype[title]" value="clean">
	</p>
<? } ?>
