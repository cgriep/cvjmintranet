<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

?>
<table width="67" border="0" cellpadding="0" cellspacing="0">
<?php
// top level navigation

global $published_roots;

init_published_roots();

reset($published_roots);

foreach ( $published_roots as $docid => $value)  {
	if(file_exists('img/design/nav/'.$docid.'.png')) {
		?><tr><td align="center"><?=get_image_tag('img/design/nav/'.$docid.'.png',$value['title'],get_url($docid))?><br clear="all" />
		<a href="<?=get_url($docid)?>"><?=htmlentities($value['title'])?></a><br /><br /></td></tr><?php
		}
	else {
		?><tr><td align="center"><?=get_image_tag('img/design/folder.png',$value['title'],get_url($docid))?><br clear="all" />
		<a href="<?=get_url($docid)?>"><?=htmlentities($value['title'])?></a><br /><br /></td></tr><?php
		}
        }
?>
</table>
