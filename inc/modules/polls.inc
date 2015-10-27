<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $profile;
// global $poll_vote;
// global $poll_id;
global $db_table_prefix;
global $docinput;

if($docinput['poll_vote'] && ($docinput['poll_id'] > $profile["Poll"])) {
	sql_query("UPDATE ".$db_table_prefix."polls_items SET counter=counter+1 WHERE id = ".$docinput['poll_vote']);
	set_profile("Poll",$docinput['poll_id']);
	$profile["Poll"] = $docinput['poll_id'];
	}

$polls_res = sql_query("SELECT id, question FROM ".$db_table_prefix."polls WHERE active = 1 AND (module_id = ".$module['id']." OR module_id = 0) ORDER BY sortorder");
if($polls_res) {

	$pollrow = sql_fetch_row($polls_res);

	sql_free_result($polls_res); 

	$poll_id = $pollrow[0];
	$poll_question = $pollrow[1];

      	if($poll_id > $profile["Poll"]) {

	if($pollrow[1]) { echo '<p>'.$pollrow[1].'</p>'; }

	$polls_res = sql_query("SELECT id, text FROM ".$db_table_prefix."polls_items WHERE poll_id = ".$poll_id." ORDER
			BY text");
	?>
	<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
	<input type="hidden" name="file" value="<?=$GLOBALS['file']?>">
	<input type="hidden" name="id" value="<?=$GLOBALS['id']?>">
	<input type="hidden" name="filter" value="<?=$GLOBALS['filter']?>">
	<input type="hidden" name="docinput[poll_id]" value="<?=$poll_id?>">
	<?php
	while($pollrow = sql_fetch_row($polls_res)) {
		?>
		<input type=radio name="docinput[poll_vote]" value="<? echo $pollrow[0]; ?>">&nbsp;<? echo $pollrow[1]; ?><br />
		<?php
		}
	?>
	<br />
	<input type="submit" value="<?=LANG_VOTE?>">
	</form>
	<?php
		} 
	else { // Show result
	$polls_res = sql_query("SELECT counter, text FROM ".$db_table_prefix."polls_items WHERE poll_id = ".$poll_id." ORDER BY text");

	while($pollrow = sql_fetch_row($polls_res)) {
		$total_votes += $pollrow[0];
		$polls_counter[$pollrow[1]] = $pollrow[0];
		}	
	if($total_votes > 0) {
	?><span style="font-size: 10px"><?=$poll_question?></span>
	<table border=0 width="100%" cellpadding="0" cellspacing="1">
	<?		
	$counter = 0;
	while(list($poll_text, $poll_counter) = each($polls_counter)) {
		++$counter; ?>
		<tr>
		<td width="100%" valign="top" align="left" background="img/polls/<?=round(($poll_counter/$total_votes)*100)?>.gif"><span style="font-size: 10px; color: white"><?=$poll_text?></span></td>
		<td align="right" width="0" valign="top"><span style="font-size: 10px; text-align: right" ><?=round(($poll_counter/$total_votes)*100)?>%</span></td>
		</tr><?
		} 
		?>
	</table>
		<?
		}
		}
	}

?>

