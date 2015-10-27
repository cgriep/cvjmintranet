<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$key = $user;

$value = get_profile($key);
$value['online'] = is_online($key, 10);
$value['registered'] = user_since($key);

?>
<table width="100%" border="0" cellpadding="1" cellspacing="0">
<tr><td width="175" valign="top">
<?

if(file_exists(get_dir_name('users/public', $key).'/picture.jpg')) { ?>
<a href="<?=get_dir_name('users/public', $key).'/picture.jpg'?>"><img src="<?=get_dir_name('users/public', $key).'/picture.jpg'?>" border="0" width="150" /></a>
<? } else { ?>
<img src="img/nopic.gif" border="0">
<? } 

?><br clear="all" /><br />
		<? if(isset($value['gender']) && $value['gender'] == 'm' && $value['online'] == true) { ?>
                <img src="img/buttons/male.gif" border="0" alt="Online Male" width="16" height="16" /></a>
                <? } elseif(isset($value['gender']) && $value['gender'] == 'm' && $value['online'] == false) { ?>
                <img src="img/buttons/offline_male.gif" border="0" alt="Offline Male" width="16" height="16" /></a>
                <? } elseif(isset($value['gender']) && $value['gender'] == 'f' && $value['online'] == true) { ?>
                <img src="img/buttons/female.gif" border="0" alt="Online Female" width="16" height="16" /></a>
                <? } elseif(isset($value['gender']) && $value['gender'] == 'f' && $value['online'] == false) { ?>
                <img src="img/buttons/offline_female.gif" border="0" alt="Offline Female" width="16" height="16" /></a>
                <? } elseif($value['online'] == true) { ?>
                <img src="img/buttons/online.gif" border="0" alt="Online (Unknown gender)" width="16" height="16" /></a>
                <? } else { ?>
                <img src="img/buttons/offline.gif" border="0" alt="Offline (Unknown gender)" width="16" height="16" /></a>
                <? } ?>		
                <? if(file_exists(get_dir_name('users/public', $key).'/picture.jpg')) { ?>
                <a href="<?=get_dir_name('users/public', $key).'/picture.jpg'?>"><img src="img/buttons/picture.gif" border="0" width="16" height="16" alt="View Picture" /></a>
                <? } else { ?>
                <img src="img/buttons/nopicture.gif" border="0" alt="No picture available." width="16" height="16" />
                <? } ?>
                <? if(isset($value['icq']) && is_numeric($value['icq']) && SESSION_STATUS == 'ok') { ?>
                <a href="javascript:sendicqmessage(<?=$key?>)"><img src="img/buttons/icq.gif" width="17" height="16" border="0" alt="<?='ICQ# '.$value['icq']?>" /></a>
                <? } else { ?>
                <img src="img/buttons/noicq.gif" border="0" width="17" height="16" />
                <? } ?>
                <a href="javascript:sendmessage(<?=$key?>)"><img src="img/buttons/email_icon.gif" alt="Send Message" border="0" width="16" height="16" /></a>
                <? if(isset($friends[$key]) && SESSION_STATUS == 'ok') { ?>
                <a href="<?=get_url($id, 'user='.$key.'&remove_friend='.$key)?>"><img src="img/buttons/remove-orange.gif" alt="Remove from friends" border="0" width="16" height="16" /></a>                
		<? } elseif(SESSION_STATUS == 'ok') { ?>
                <a href="<?=get_url($id, 'user='.$key.'&add_friend='.$key)?>"><img src="img/buttons/add-orange.gif" alt="Add to friends" border="0" width="16" height="16" /></a>
                <? } else { ?>
                <img src="img/buttons/add-bw.gif" alt="Can't add to friends" border="0" width="16" height="16" />
                <? } ?>
</td>
<td valign="top">
<b><?=LANG_NICKNAME?></b><br />
<?=transform($value['nickname'], 'clean')?>
<br />
<br />
<? if(isset($value['interests'])) { ?>
<b><?=LANG_INTERESTS?></b><br />
<?=transform($value['interests'], 'usertext')?>
<br />
<br />
<? } ?>
<? if(isset($value['website'])) { ?>
<b><?=LANG_WEBSITE?></b><br />
<?=transform($value['website'], 'usertext')?>
<br />
<br />
<? } ?>
<? if(isset($value['country'])) { ?>
<b><?=LANG_COUNTRY?></b><br />
<?=transform($value['country'], 'usertext')?>
<br />
<br />
<? } ?>
<? if(isset($value['birthday'])) { ?>
<b><?=LANG_BIRTHDAY?></b><br />
<?=transform($value['birthday'], 'date')?>
<br />
<br />
<? } ?>
<? if(isset($value['registered'])) { ?>
<b><?=LANG_MEMBER_SINCE?></b><br />
<?=transform($value['registered'], 'date')?>
<? } ?>
</td>
</tr>
</table>
<br />
[ <a href="<?=get_url($id, 'search='.$search)?>"><?=LANG_BACK?></a> ]
