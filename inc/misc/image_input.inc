<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if($profile['simple_edit'] != 1) {

if(!isset($document['datatype'][$value])) $document['datatype'][$value] = 'image_hide';

?>
<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr><td nowrap="nowrap">
<?=$title?><br />
<select name="docdata[<?=$value?>]"><option value="">No Image</option>
<?

$_dirname = get_dir_name('files', $id).'/public/images/';

if(file_exists($_dirname)) {
	$handle=opendir($_dirname);
	if($handle) {
	while (false!==($file = readdir($handle))) {
        	if ($file != "." && $file != "..") {
                	$images[] = $_dirname.$file;
	                }
        	}
 
	natcasesort($images);
 
	while(list($key, $file) = each($images)) {
	        if($file == $document[$value]) { $sel = ' SELECTED'; } else { $sel = ''; }
	        echo '<option value="'.$file.'"'.$sel.'>'.basename($file).'</option>';
	        }
	}
	}
?>
</select>
<input type="hidden" name="docdatatype[<?=$value?>]" value="notrans" />
&nbsp;&nbsp;&nbsp;&nbsp;
</td><td nowrap="nowrap" width="100%">
<?=$title?> Position<br />
<select name="docdatatype[<?=$value?>]">
<option value="image_hide"<? if($document['datatype'][$value] == 'image_hide') echo ' SELECTED'; ?>>Hidden</option>
<option value="image_left"<? if($document['datatype'][$value] == 'image_left') echo ' SELECTED'; ?>>Left</option>
<option value="image_right"<? if($document['datatype'][$value] == 'image_right') echo ' SELECTED'; ?>>Right</option>
<option value="image_popup"<? if($document['datatype'][$value] == 'image_popup') echo ' SELECTED'; ?>>Pop-up window</option>
</select>
</td><td>
<?=$title?> Upload<br />
<input type="file" name="docupload[public/images/*]" />
</td>
</tr></table>
<?
}
?>
