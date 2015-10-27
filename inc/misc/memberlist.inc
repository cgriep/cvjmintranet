<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

		if($tr_bg != $design['table_row_dark']) {
			$tr_bg = $design['table_row_dark'];
			}
		else {
			$tr_bg = $design['table_row_bright'];
			}
		?>
		<tr bgcolor="<?=$tr_bg?>">
		<td width="125">
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
		<img src="img/activity/<?=round($value['activity'] / 25)?>.gif" width="16" border="0" alt="<?=$value['activity']?>%">
		<? if(file_exists(get_dir_name('users/public', $key).'/picture.jpg')) { ?>
		<a href="<?=get_dir_name('users/public', $key).'/picture.jpg'?>"><img src="img/buttons/picture.gif" border="0" width="16" height="16" alt="View Picture" /></a>
		<? } else { ?>
		<img src="img/buttons/nopicture.gif" border="0" alt="No picture available." width="16" height="16" />
		<? } ?>
		<? if(isset($value['icq']) && is_numeric($value['icq']) && SESSION_STATUS == 'ok') { ?><a href="javascript:sendicqmessage(<?=$key?>)"><img src="img/buttons/icq.gif" width="17" height="16" border="0" alt="<?='ICQ# '.$value['icq']?>" /></a><? } else { ?>
		<img src="img/buttons/noicq.gif" border="0" width="17" height="16" />
		<? } ?>
		<a href="javascript:sendmessage(<?=$key?>)"><img src="img/buttons/email_icon.gif" alt="Send Message" border="0" width="16" height="16" /></a>
		<? if(isset($friends[$key]) && SESSION_STATUS == 'ok') { ?>
		<a href="<?=get_url($id, 'search='.$search.'&remove_friend='.$key)?>"><img src="img/buttons/remove-orange.gif" alt="Remove from friends" border="0" width="16" height="16" /></a>
		<? } elseif(SESSION_STATUS == 'ok') { ?>
		<a href="<?=get_url($id, 'search='.$search.'&add_friend='.$key)?>"><img src="img/buttons/add-orange.gif" alt="Add to friends" border="0" width="16" height="16" /></a>
		<? } else { ?>
		<img src="img/buttons/add-bw.gif" alt="Can't add to friends" border="0" width="16" height="16" />
		<? } ?>
		</td>
		<td>
		<a href="<?=get_url($id, 'search='.$search.'&user='.$key)?>"><?=$value['nickname']?></a>
		</td>
		</tr>
