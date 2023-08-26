<?php

/*	This is the guestbookposting content module for the AWF. It's a childmodule for the guestbook contentmodule.
	maintained by Christoph Stickel <mixer2@web.de>									*/

load_lang('guestbook');

if($profile['editor']=='1') {
	create_editor_input ('E-Mail', 'email', 'text_input');
	echo '<br /><br />';
	create_editor_input ('Homepage', 'url', 'text_input');
	echo '<br /><br />';
	create_editor_input ('Message', 'posting', 'area_input');	
	?><input type="hidden" name="docdataflavour[email]" value="0">
	<input type="hidden" name="docdataflavour[url]" value="0">
	<input type="hidden" name="docdataflavour[posting]" value="0">
	<input type="hidden" name="docdataflavour[title]" value="0"><?php
	}
elseif($profile['editor']!='1') {
	echo $design['table_begin'];
	?><tr bgcolor="<?=$design['table_row_dark']?>"><td width="70%">
	<?=LANG_NICKNAME?> [ <?php
	if(ereg('@',$document['email'])) { ?><a href="mailto:<?=transform($document['email'],'clean')?>"><?=transform($document['title'], 'clean')?></a><?php }
	else { echo transform($document['title'], 'clean'); }
	?> ]&nbsp;&nbsp;<?=LANG_GUESTBOOK_HOMEPAGE?> [ <?php
	if(strlen($document['url']) > 11) { ?><a href="<?=transform($document['url'],'clean')?>" target="_blank"><?=transform($document['url'],'clean')?></a><?php }
	else { echo 'none'; }
	?> ]</td><td width="1%" nowrap><?=LANG_GUESTBOOK_POSTEDAT?> <?=transform($document['sorttime'], 'datetime')?>
	</td></tr><tr bgcolor="<?=$design['table_row_bright']?>"><td colspan="2"><?php
	if(isset($document['datatype']['posting'])) { echo transform($document['posting'], $document['datatype']['posting']); }
	else { echo transform($document['posting'], 'usertext'); }
	?></td></tr><?php
	echo $design['table_end'];
	}
?>
