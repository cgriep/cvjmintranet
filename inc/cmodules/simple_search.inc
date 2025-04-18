<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$search = $_REQUEST['search'];

if($profile['editor'] != 1) {
?>
<p>
<form action="<?=$_SERVER['PHP_SELF']?>" method="get">
<input type="hidden" name="id" value="<?=$id?>">
<input type="text" size="50" name="search" value="<?=$search?>">
&nbsp;
<input type="submit" value="<?=LANG_SEARCH?>">
</form>
</p>
<br>
<?php

if(isset($search) && $search && is_numeric($flavour)) {

if(DBMS == 'mysql') { $where = '(flavour_id = '.$flavour.' OR flavour_id = 0) AND value LIKE "%'.addslashes($search).'%"'; } 
else { $where = "value LIKE '%".addslashes($search)."%'"; }

$search_results = search_nodes ($where);

if($search_results) {
	?>
	<table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="#ffffff"><tr>
	<td align="left" bgcolor="<?=$design['table_header']?>">
	<?php printf(LANG_SEARCHED_THIS_SITE, $search); ?>
	</td>
	<td align="right" bgcolor="<?=$design['table_header']?>">
	<?php printf(LANG_RESULT_COUNT, count($search_results), $search_time); ?>
	</td>
	</tr>
	</table>
	<table width="100%" border="0" cellpadding="5" cellspacing="0" bgcolor="#ffffff">
	<?php
	$counter = 0; 
	foreach ($search_results as $key =>$value)  {
		$counter++;
		echo '<tr><td align="right" bgcolor="'.$design['table_row_dark'].'" valign="top" width="5%">'.$counter.'</td>
	        <td align="left" bgcolor="'.$design['table_row_bright'].'" valign="top" width="95%">
		<b><a href="'.get_url($key).'">'.transform($value['title'], 'clean').'</a></b><br />';
		if(isset($value['author_id'])) printf(LANG_RESULT_CREATED_BY.' ', transform($value['author_id'], 'user_id'));
		echo(transform($value['timestamp'], 'datetime'));
		if(COUNT_VIEWS==1) {
			echo(', '.$value['views'].' '.LANG_VIEWS.', '.count_children($doctypes['comment'], $flavour, $key).' '.LANG_COMMENTS_LOW);
			}
		echo('</td></tr>');
		}
	?> 
	</table> 
	<?php
	}
else {
	printf('<p>'.LANG_NOT_FOUND.'</p>', $search);
	}
}
}

?>
