<?php
global $profile;
echo $design['table_begin']?>

<tr bgcolor="<?=$design['table_row_dark']?>"><td>
<b><?=LANG_USERZUSATZ_INFO?></b></td></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<?=LANG_NAME?><br />
<input type="text" name="docinput[Name]" value="<?=$profile['Name']?>" size="40" />
<br />
<br />
<?=LANG_VORNAME?><br />
<input type="text" name="docinput[Vorname]" value="<?=$profile['Vorname']?>" size="40" />
<br />
<br />
<?=LANG_GENDER?><br />
<input type="radio" name="docinput[gender]" value="m"<? if($profile['gender'] == 'm') echo ' checked'; ?> /> m&auml;nnlich
&nbsp&nbsp;
<input type="radio" name="docinput[gender]" value="f"<? if($profile['gender'] == 'f') echo ' checked'; ?> /> weiblich
<br />
<br />
<?=LANG_BIRTHDAY.' '.LANG_BIRTHDAY_FORMAT?> <b>Bitte Format Monat Tag Jahr beachten !</b><br />
<input type="text" name="docinput[birthday]" value="<?=date('m/d/Y',$profile['birthday'])?>" size="40" />
<br />
<br />
Adresse<br />
<input type="text" name="docinput[Adresse]" value="<?=$profile['Adresse']?>" size="80" />
<br />
<br />
PLZ, Ort <br />
<input type="text" name="docinput[PLZ]" value="<?=$profile['PLZ']?>" size="5" />
<input type="text" name="docinput[Ort]" value="<?=$profile['Ort']?>" size="40" />
<br />
<br />
<?=LANG_COUNTRY?><br />
<select name="docinput[country]">
<?
$countries = file(INC_PATH.DATA_PATH.'/country_list_names_only.txt');
if ( ! isset($profile['country'] ) ) $profile['country'] = "Germany";
while(list($key, $value) = each($countries)) { ?>
        <option value="<?=trim($value)?>"<? if($profile['country'] == trim($value)) echo ' selected'; ?>><?=trim($value)?></option>
        <?
        }
?>
</select>
<br />
<br />
Telefon / Handy<br />
<input type="text" name="docinput[Telefon]" value="<?=$profile['Telefon']?>" size="30" />
<br />
<br />
Bemerkungen zur Person (Krankheiten, sonstiges was wir wissen sollten)<br />
<textarea name="docinput[Bemerkungen]" cols="60" rows="5"><?=stripslashes($profile['Bemerkungen'])?>
</textarea>
<br />
<br />
Conerfahrung in Tagen insgesamt<br />
<input type="text" name="docinput[Contage]" value="<?=$profile['Contage']?>" size="5" />
<br />
<br />
Vegetarier (falls kein Selbstverpfleger)<br />
<input type="radio" name="docinput[Vegetarier]" value="1"<? if($profile['Vegetarier'] == '1') echo ' checked'; ?> /> ja
&nbsp&nbsp;
<input type="radio" name="docinput[Vegetarier]" value="0"<? if($profile['Vegetarier'] == '0') echo ' checked'; ?> /> nein
</td></tr>
<?=$design['table_end']?>
<br />
<br />