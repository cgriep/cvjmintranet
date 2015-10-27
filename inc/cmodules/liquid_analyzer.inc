<?
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 10.04.2004
*/

if(is_numeric($docinput['month'])) {
	$month = $docinput['month'];
	}
else {
	$month = date("m");
	}

if(is_numeric($docinput['year'])) {
	$year = $docinput['year'];
	}
else {
	$year = date("Y");
	}

if($profile['editor'] != 1) {
?>
<form action="<?=$self?>" method="post">
<input type="hidden" name="id" value="<?=$id?>" />
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td align="left"><h2>Log Analysis for <?=date("F Y", mktime(0,0,0,$month,1,$year))?></h2></td>
<td nowrap="nowrap" align="right">
<select name="docinput[month]">
<?
for($i = 1; $i <= 12; $i++) {
	?><option <? if($i == $month) echo 'selected="selected" '; ?>value="<?=str_pad($i, 2, '0', STR_PAD_LEFT)?>"><?=str_pad($i, 2, '0',STR_PAD_LEFT)?></option><?
	}
?>
</select>
<select name="docinput[year]">
<?
for($i = 2002; $i <= date('Y'); $i++) {
	?><option <? if($i == $year) echo 'selected="selected" '; ?>value="<?=$i?>"><?=$i?></option><?
	}
?>
</select>
<input type="submit" value="Go" />
</td>
</tr>
</table>
</form>
<?

$filename = BASE_PATH.'logs/'.$year.'_'.$month.'.log';

// echo $filename;

$total_time = 0;
$pages = 0;
$ips = array();
$page_ids = array();
$user_ids = array();
$langs = array();
$days = array();
$hours = array();

if(file_exists($filename)) {

// echo cal_days_in_month(CAL_GREGORIAN, date("m"), date("Y"));

$fp = fopen($dir.$filename, 'r');

while (!feof($fp)) {
	$buffer = fgets($fp, 4096);
	if($buffer != '') {
		$log_line = explode("\t", $buffer);
		$total_time += $log_line[3];
		$page_ids[$log_line[2]]++;
		$user_ids[$log_line[4]]++;
		$ips[$log_line[0]]++;
		$langs[$log_line[5]]++;
		$days[date('j', $log_line[1])] = $days[date('j', $log_line[1])] + 1;
		$hours[date('G', $log_line[1])] = $hours[date('G', $log_line[1])] + 1;
		$pages++;
		}
	}

fclose ($fp);

if($pages > 0) {

?>
<table width="100%" border="0" bgcolor="<?=$design['table_row_bright']?>" cellspacing="1" cellpadding="1">
<tr bgcolor="<?=$design['table_row_dark']?>">
<td colspan="31" style="font-weight: bold;">
Page Views by Day of the Month
</td>
</tr>
<tr>
<?
$max_height = 0;
for($i = 1; $i <= 31; $i++) {
	$_height = $days[$i] / $pages;
	if($_height > $max_height) $max_height = $_height;
	}

for($i = 1; $i <= 31; $i++) {
?>
<td width="3%" valign="bottom" align="center">
<table bgcolor="<?=$design['linkcolor']?>" width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td valign="bottom"><img src="img/pixel.gif" width="1" border="0" height="<?=round(($days[$i] / ($max_height*$pages)) 
* 100)?>" /></td>
</tr>
</table>
</td>
<?	
}
?>
</tr>
<tr bgcolor="<?=$design['table_row_dark']?>">
<?
for($i = 1; $i <= 31; $i++) {
?>
<td width="3%" align="center">
<?=$i?>
</td>
<?	
}
?>
</tr>
</table>
<br />
<br />
<table width="100%" border="0" bgcolor="<?=$design['table_row_bright']?>" cellspacing="1" cellpadding="1">
<tr bgcolor="<?=$design['table_row_dark']?>">
<td colspan="24" style="font-weight: bold;">
Page Views by Hour of the Day
</td>
</tr>
<tr>
<?
$max_height = 0;
for($i = 1; $i <= 23; $i++) {
	$_height = $hours[$i] / $pages;
	if($_height > $max_height) $max_height = $_height;
	}

for($i = 0; $i <= 23; $i++) {
?>
<td width="4%" valign="bottom" align="center">
<table bgcolor="<?=$design['linkcolor']?>" width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td valign="bottom"><img src="img/pixel.gif" width="1" border="0" height="<?=round(($hours[$i] / ($max_height*$pages)) * 
100)?>"/></td>
</tr>
</table>
</td>
<?	
}
?>
</tr>
<tr bgcolor="<?=$design['table_row_dark']?>">
<?
for($i = 0; $i <= 23; $i++) {
?>
<td width="4%" align="center">
<?=$i?>
</td>
<?	
}
?>
</tr>
</table>
<br />
<br />


<table bgcolor="<?=$design['table_row_bright']?>" width="100%" border="0" cellspacing="1" cellpadding="1">
<tr bgcolor="<?=$design['table_row_dark']?>">
<td colspan="3" style="font-weight: bold;">
Languages
</td>
</tr>
<?
while(list($key, $value) = each($langs)) {
?>
<tr>
<td bgcolor="<?=$design['table_row_dark']?>" width="10%"><?=$key?></td>
<td bgcolor="<?=$design['table_row_dark']?>" width="10%" align="center"><?=round(($value / $pages) * 100)?>%</td>
<td width="80%">
<table 
bgcolor="<?=$design['linkcolor']?>" height="12" width="<?=round(($value / $pages) * 100)?>%" border="0" cellspacing="0" 
cellpadding="0">
<tr><td width="<?=round(($value / $pages) * 100)?>%"><img src="img/pixel.gif" border="0" /></td></tr>
</table>
</td>
</tr>
<?
	}
?>
</table>
<br />
<br />
<table bgcolor="<?=$design['table_row_bright']?>" width="100%" border="0" cellspacing="1" cellpadding="1">
<tr bgcolor="<?=$design['table_row_dark']?>">
<td colspan="2" style="font-weight: bold;">
Other Stuff
</td>
</td>
<tr>
<td width="20%" nowrap="nowrap" bgcolor="<?=$design['table_row_dark']?>">
Total Page Views
</td>
<td width="80%" align="right">
<?=$pages?>
</td>
</tr>
<tr>
<td width="20%" nowrap="nowrap" bgcolor="<?=$design['table_row_dark']?>">
Different Pages
</td>
<td align="right">
<?=count($page_ids)?>
</td>
</tr>
<tr>
<td width="20%" nowrap="nowrap" bgcolor="<?=$design['table_row_dark']?>">
Different Hosts
</td>
<td align="right">
<?=count($ips)?>
</td>
</tr>
<tr>
<td width="20%" nowrap="nowrap" bgcolor="<?=$design['table_row_dark']?>">
Different registered Users
</td>
<td align="right">
<?=count($user_ids)?>
</td>
</tr>
<tr>
<td width="20%" nowrap="nowrap" bgcolor="<?=$design['table_row_dark']?>">
Total Runtime
</td>
<td align="right">
<?=round($total_time / 60, 2)?> min
</td>
</tr>
<tr>
<td width="20%" nowrap="nowrap" bgcolor="<?=$design['table_row_dark']?>">
Average Time to Serve Pages</td>
<td align="right">
<?=round($total_time / $pages, 4)?> s
</td>
</tr>
</table>
<?
}
}
else {
	echo "No logs found.";
	}
}
?>
