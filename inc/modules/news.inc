<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $flavour;

$news_id = get_cmodule_id('news');

$filename = BASE_PATH.'cache/modules/news_'.md5($news_id.$design['name'].$flavour);
$cache_time = 900;
 
if(time() - filemtime($filename) < $cache_time && $cache_time != 0) {
        readfile($filename);
        }
else {
ob_start();

global $doctypes;

$children = get_sorted_children (0, 5, 'timestamp', $doctypes['newsitem'], $news_id); 

?><span style="font-size: 10px"><?php

foreach ($children as $docid => $value) {
	if(strlen($value['title']) > 47) { $value['title'] = substr($value['title'], 0, 44).'...'; }
	?>
	<img src="<?=$design['arrow_bullet']?>" border="0" width="10" height="7" />
	<?=transform($value['timestamp'], 'date')?>:
	<a href="<?=get_url($docid)?>">
	<?=transform($value['title'], $value['datatype']['title'])?></a>
	<br />
	<?php
	}
	?>
	<a href="<?=get_url($news_id)?>"><?=LANG_MORE_NEWS?>...</a>
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
?>
