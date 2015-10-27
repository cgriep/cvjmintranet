<?php

echo $design['table_begin']?>

<tr bgcolor="<?=$design['table_row_dark']?>"><td>
<b><?=LANG_USERZUSATZ_INFO?></b></td></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<?=LANG_COUNTRY?><br />
<select name="docinput[country]">
<?
$countries = file(INC_PATH.DATA_PATH.'/country_list_names_only.txt');
while(list($key, $value) = each($countries)) { ?>
        <option value="<?=trim($value)?>"<? if($profile['country'] == trim($value)) echo ' selected'; ?>><?=trim($value)?></option>
        <?
        }
?>
</select>
<br />
<br />
<?=LANG_GENDER?><br />
<input type="radio" name="docinput[gender]" value="m"<? if($profile['gender'] == 'm') echo ' checked'; ?> /> Male
&nbsp&nbsp;
<input type="radio" name="docinput[gender]" value="f"<? if($profile['gender'] == 'f') echo ' checked'; ?> /> Female
<br />
<br />
<?=LANG_BIRTHDAY.' '.LANG_BIRTHDAY_FORMAT?><br />
<input type="text" name="docinput[birthday]" value="<?=date('m/d/Y',$profile['birthday'])?>" size="40" />
</td></tr>
<?=$design['table_end']?>
<br />
<br />

