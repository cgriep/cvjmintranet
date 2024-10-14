<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $profile;
global $design;

if($profile['editor'] == 1) { 
	if(isset($_REQUEST['module_news_source_'.$module['id']])) {
		save_var('rdf_source_'.$module['id'], $_REQUEST['module_news_source_'.$module['id']]);
		}
	$source = load_var('rdf_source_'.$module['id']);
	if($source == '') $source = 'heise.rdf';
	?>
	<form style="text-align: center" action="<?=$_SERVER['PHP_SELF']?>" method="post">
	<input type="hidden" name="id" value="<?=$GLOBALS['id']?>">
	<input type="hidden" name="first_item" value="<?=$GLOBALS['first_item']?>">
	<input type="hidden" name="subject" value="<?=$GLOBALS['subject']?>">
	<input type="hidden" name="search" value="<?=$GLOBALS['search']?>">
	<select name="module_news_source_<?=$module['id']?>">
<?php
	$handle = opendir(BASE_PATH.INC_PATH.DATA_PATH.'rdf');
	while (false!==($file = readdir($handle))) {
	    if ($file != "." && $file != "..") {
		$files[] = $file;
	    }	
	}
	closedir($handle); 
	
	natcasesort($files);
	foreach ( $files as $key => $file) {
	        if($file == $source) { $sel = ' selected="selected"'; } else { $sel = ''; }
		echo "\t".'<option value="'.$file.'"'.$sel.'>'.$file."</option>\n";
        }
	
	?>
	</select><br />
	<input type="submit" value="<?=LANG_SAVE?>" />
	</form>
	<?php
	}
else {

$filename = BASE_PATH.'cache/modules/rdf_'.md5($module['id'].$design['name']);
$cache_time = 900;

if(time() - filemtime($filename) < $cache_time && $cache_time != 0) {
	readfile($filename);
	}
else {
	ob_start();

	include_once(BASE_PATH.INC_PATH."rdf.inc");
	$source = load_var('rdf_source_'.$module['id']);
	if($source == '') $source = 'heise.rdf';
	$item = new rdfFile(BASE_PATH.INC_PATH.DATA_PATH."rdf/$source");
	$item->parse(true);
	?><span style="font-size: 10px"><?php
	foreach ($item->Items as $key => $value) {
		?><img src="<?=$design['nav_bullet']?>" border="0" width="10" height="7">&nbsp;<a target="_blank" href="<?php
		echo $value['Link'].'">'.$value['Title'].'</a><br />';
		}

	?>
	</span>
	<?php 
	$content = ob_get_contents();
	ob_end_clean();
	if($cache_time != 0) {
	        $fp = fopen ($filename, "w");
        	fwrite($fp,$content);
	        fclose($fp);
		}
	echo $content;
	}
} ?>
