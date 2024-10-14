<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003

	This file was modified on 27.02.2003 by Paul Maxwell (hax@awfers.org)
*/

load_lang('search');

$search = $_REQUEST['search'];
$manner = $_REQUEST['manner'];

// if not set, default to 1 (phrase)
if (!isset($manner)) { $manner = 1; }

if($profile['editor'] != 1) {
?>
<p>
<form action="<?=$_SERVER['PHP_SELF']?>" method="get">
<input type="hidden" name="id" value="<?=$id?>">
<input type="text" size="50" name="search" value="<?=$search?>">
 
<input type="submit" value="<?=LANG_SEARCH?>"><br />   
<?php 
if ($manner == 1) {
echo '<input type="radio" value="1" name="manner" checked>'.LANG_SEARCH_PHRASE;
echo '<input type="radio" value="2" name="manner">'.LANG_SEARCH_ALL;
echo '<input type="radio" value="3" name="manner">'.LANG_SEARCH_ANY;
}
elseif ($manner == 2) { 
echo '<input type="radio" value="1" name="manner">'.LANG_SEARCH_PHRASE;
echo '<input type="radio" value="2" name="manner" checked>'.LANG_SEARCH_ALL;
echo '<input type="radio" value="3" name="manner">'.LANG_SEARCH_ANY;
}
elseif ($manner == 3) {
echo '<input type="radio" value="1" name="manner">'.LANG_SEARCH_PHRASE;
echo '<input type="radio" value="2" name="manner">'.LANG_SEARCH_ALL;
echo '<input type="radio" value="3" name="manner" checked>'.LANG_SEARCH_ANY;
}
?>
</form>
</p>
<br>
<?php

if(isset($search) && $search && is_numeric($flavour)) {

$lookfor = $search;

if ($manner > 1) { 

	//replace multiple spaces with single spaces

	// need code for this ... is this the best way?
	$lookfor = ereg_replace ('  ', ' ', $lookfor);
	addslashes($lookfor);

	if ($manner == 3) { 

		//replace single spaces with sql |%' OR LIKE '%| statements
		// eg
		// $search='1stword 2ndword 3rdword'
		// becomes '1stword%' OR LIKE '%2ndword%' OR LIKE '%3rdword'

		// need code for this ...will this work?
		$lookfor = ereg_replace (' ', '%" OR value LIKE "%', $lookfor);
		$way = LANG_SEARCH_ANY;
	} 

	elseif ($manner == 2) { 

		//replace single spaces with sql |%' AND LIKE '%| statements
		// eg
		// $search='1stword 2ndword 3rdword'
		// becomes '1stword%' AND LIKE '%2ndword%' AND LIKE '%3rdword'

		// need code for this ...will this work?
		$lookfor = ereg_replace (' ', '%" AND value LIKE "%', $lookfor);
		$way = LANG_SEARCH_ALL;
	} 

} 
else {
	addslashes($lookfor);
	$way = LANG_SEARCH_PHRASE;
}

if(DBMS == 'mysql') { $where = '(flavour_id = '.$flavour.' OR flavour_id = 0) AND value LIKE "%'.$lookfor.'%"'; } 
else { $where = "value LIKE '%".$lookfor."%'"; }

$search_results = search_nodes ($where);

if($search_results) {
	?>
	<table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="#ffffff"><tr>
	<td align="left" bgcolor="<?=$design['table_header']?>">
	<?php printf(LANG_SEARCHED_THIS_SITE.'<br />('.$way.')','<b>'.$search.'</b>'); ?>	</td>
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
	printf('<p>'.LANG_NOT_FOUND.' ('.$way.')</p>','<b>'.$search.'</b>');
	}
}
}

?>
