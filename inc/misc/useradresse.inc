<?php

echo $design['table_begin']?>

<tr bgcolor="<?=$design['table_row_dark']?>"><td>
<b><?=LANG_USERZUSATZ_INFO?></b></td></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<?=LANG_COUNTRY?><br />
<select name="docinput[country]">
<?php
$countries = file(INC_PATH.DATA_PATH.'/country_list_names_only.txt');
foreach ($countries as $key => $value)  { ?>
        <option value="<?=trim($value)?>"<?php if($profile['country'] == trim($value)) echo ' selected'; ?>><?=trim($value)?></option>
        <?php
        }
?>
</select>
<br />
<br />
<?=LANG_GENDER?><br />
<input type="radio" name="docinput[gender]" value="m"<?php if($profile['gender'] == 'm') echo ' checked'; ?> /> Male
&nbsp&nbsp;
<input type="radio" name="docinput[gender]" value="f"<?php if($profile['gender'] == 'f') echo ' checked'; ?> /> Female
<br />
<br />
<?=LANG_BIRTHDAY.' '.LANG_BIRTHDAY_FORMAT?><br />
<input type="text" name="docinput[birthday]" value="<?=date('m/d/Y',$profile['birthday'])?>" size="40" />
</td></tr>
<?=$design['table_end']?>
<br />
<br />

