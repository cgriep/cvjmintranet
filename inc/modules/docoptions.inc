<?php
/*
        Copyright (C) 2000-2002 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 25.05.2002
*/

global $document;
global $id;
$first_item = $_REQUEST['first_item'];
$doc = $_REQUEST['doc'];

if($profile['editor']) {
	if($_REQUEST['save'] == 'docoptions') {
		echo is_integer($doc['membersonly']);
		if($doc['membersonly'] != 1) $doc['membersonly'] = 0;
		if($doc['sslonly'] != 1) $doc['sslonly'] = 0;
		if($doc['doconly'] != 1) $doc['doconly'] = 0;
		if($doc['hideleft'] != 1) $doc['hideleft'] = 0;
		if($doc['hideright'] != 1) $doc['hideright'] = 0;
		if($doc['releasedate'] < 0) {
			if(!($doc['releasedate'] = strtotime($doc['release_date_string']))) $doc['releasedate'] = 0;
			}
		if($doc['expirationdate'] < 0) {
			if(!($doc['expirationdate'] = strtotime($doc['expiration_date_string']))) $doc['expirationdate'] = 0;
			}

		/* Begin BEMA CODE  */
		if($doc['publicationdate'] < 0) {
			if(!($doc['publicationdate'] = strtotime($doc['publication_date_string']))) $doc['publicationdate'] = 0;
			}

                 sql_query("UPDATE ".TABLE_NODEDATA." SET value = '".$doc['authorid']."' WHERE node_id = $id and name='author_id'" );
                 sql_query("UPDATE ".TABLE_NODEDATA." SET value = ".$doc['publicationdate']." WHERE node_id = $id and name='timestamp'" );
		/* end BEMA CODE  */

		sql_query("UPDATE ".TABLE_NODES." SET members_only = ".$doc['membersonly']." WHERE id = $id");
		sql_query("UPDATE ".TABLE_NODES." SET release_date = ".$doc['releasedate']." WHERE id = $id");
		sql_query("UPDATE ".TABLE_NODES." SET expiration_date = ".$doc['expirationdate']." WHERE id = $id");
		sql_query("UPDATE ".TABLE_NODES." SET ssl_only     = ".$doc['sslonly']."     WHERE id = $id");
		add_nodedata ($id, 'refresh', $doc['refresh'], 0, 'notrans');
		add_nodedata ($id, 'show_doc_only', $doc['doconly'], 0, 'notrans');
		add_nodedata ($id, 'hide_left_col', $doc['hideleft'], 0, 'notrans');
		add_nodedata ($id, 'hide_right_col', $doc['hideright'], 0, 'notrans');
		$document['members_only'] = $doc['membersonly'];
		$document['release_date'] = $doc['releasedate'];
		$document['expiration_date'] = $doc['expirationdate'];
		$document['expiration_date'] = $doc['expirationdate'];
		$document['refresh']      = $doc['refresh'];
		$document['ssl_only']     = $doc['sslonly'];
		$document['show_doc_only']= $doc['doconly'];
		$document['hide_left_col']= $doc['hideleft'];
		$document['hide_right_col']= $doc['hideright'];
		}
	?>
                <form action="<?=$_SERVER['PHP_SELF']?>" method="post">
                <input type="checkbox" value="1" name="doc[membersonly]"<?php if($document['members_only']) echo 'CHECKED'; ?> />&nbsp;Members only<br />
                <input type="checkbox" value="1" name="doc[doconly]"<?php if($document['show_doc_only']) echo 'CHECKED'; ?> />&nbsp;Show document only<br />
                <input type="checkbox" value="1" name="doc[hideleft]"<?php if($document['hide_left_col']) echo 'CHECKED'; ?> />&nbsp;Hide left column<br />
                <input type="checkbox" value="1" name="doc[hideright]"<?php if($document['hide_right_col']) echo 'CHECKED'; ?> />&nbsp;Hide right column<br />
                <input type="checkbox" value="1" name="doc[sslonly]"<?php if($document['ssl_only']) echo 'CHECKED'; ?> />&nbsp;SSL only<br /><br />
                <span title="Nummer des Autors, wenn nicht man selbst">
		Author-ID<br />
		<input name="doc[authorid]" type="text" value="<?=$document[author_id]?>" maxlength="5" />
                 <br />
		<!--begin BEMA CODE
		This code is used to keep the the publication date the same, even if the news was edited after it has been submitted.-->
		<?php $date = transform($document['timestamp'], 'datetime'); ?>
		<span title="Use this field when you want leave the original Date">
		Publication Date (Last Update)<br />
		<!--- <input name="doc[publicationdate]" value="0" type="radio" /> Now<br /> --->
		<input name="doc[publicationdate]" value="-1" type="radio" checked />
		<input mmTranslatedValueHiliteColor="HILITECOLOR=%22Dyn%20Untranslated%20Color%22" mmTranslatedValueDynAttrs="DynAttrs=value" type="text" name="doc[publication_date_string]" size="15" value="<?php echo $date;?>" /><br /></span>
		<br />
		<?php if(!update_nodedata($id, 'timestamp', time(), $flavour, 'datetime')) add_nodedata($id, 'timestamp', time(), $flavour, 'datetime');?>

		<!--end BEMA CODE -->
		Release Date<br />
		<input name="doc[releasedate]" value="0" type="radio"<?php if($document['release_date'] == 0) echo ' CHECKED'; ?> /> Now<br />
		<input name="doc[releasedate]" value="-1" type="radio"<?php if($document['release_date'] != 0) echo 'CHECKED'; ?> />
		<input type="text" name="doc[release_date_string]" size="15" value="<?=date('j M Y H:i',$document['release_date'])?>" /><br />
		<br />
		Expiration Date<br />
		<input name="doc[expirationdate]" value="0" type="radio"<?php if($document['expiration_date'] == 0) echo ' CHECKED'; ?> /> Never<br />
		<input name="doc[expirationdate]" value="-1" type="radio"<?php if($document['expiration_date'] != 0) echo 'CHECKED'; ?> />
		<input type="text" name="doc[expiration_date_string]" size="15" value="<?=date('j M Y H:i',$document['expiration_date'])?>" /><br />
		<br />


		Auto Refresh<br />
		<select mmTranslatedValueHiliteColor="HILITECOLOR=%22No Color%22" mmTranslatedValueHiliteColor="HILITECOLOR=%22No Color%22" mmTranslatedValueHiliteColor="HILITECOLOR=%22No Color%22" mmTranslatedValueHiliteColor="HILITECOLOR=%22No Color%22" name="doc[refresh]">
		<option value="0"<?if($document['refresh'] == '0') echo ' selected'; ?>>Disable</option>
		<option value="1"<?if($document['refresh'] == '1') echo ' selected'; ?>>1 sec</option>
		<option value="5"<?if($document['refresh'] == '5') echo ' selected'; ?>>5 sec</option>
		<option value="15"<?if($document['refresh'] == '15') echo ' selected'; ?>>15 sec</option>
		<option value="30"<?if($document['refresh'] == '30') echo ' selected'; ?>>30 sec</option>
		<option value="60"<?if($document['refresh'] == '60') echo ' selected'; ?>>1 min</option>
		<option value="120"<?if($document['refresh'] == '120') echo ' selected'; ?>>2 min</option>
		<option value="300"<?if($document['refresh'] == '300') echo ' selected'; ?>>5 min</option>
		<option value="600"<?if($document['refresh'] == '600') echo ' selected'; ?>>10 min</option>
		</select>
                <input type="hidden" name="first_item" value="<?=$first_item?>" />
		<input type="hidden" name="save" value="docoptions" />
		<input type="hidden" name="id" value="<?=$id?>" />
                <br /><br /><input type="submit" value="Apply" />
                </form>
		<?php

	}
