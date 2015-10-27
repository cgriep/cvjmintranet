<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.04.2004
*/

function lb_export_init() {
	global $export_qres;
	$export_qres = sql_query("SELECT ".TABLE_USERS.".id, email FROM ".TABLE_USERS.", ".TABLE_USERDATA." WHERE ".
			TABLE_USERS.".id = ".TABLE_USERDATA.".user_id AND ".TABLE_USERDATA.".name='birthday_day' AND ". 
			TABLE_USERDATA.".value != '' ORDER BY id");

	return array(LANG_ADMIN_FIRSTNAME,LANG_ADMIN_LASTNAME,LANG_ADMIN_EMAIL,LANG_ADMIN_BIRTHDAY); 
	}

function lb_export_get_dataset() {
	global $export_qres;
	if($result = sql_fetch_array($export_qres)) {
		$profile = get_profile($result['id'], FALSE);
		$row['firstname'] = $profile['firstname'];
		$row['lastname'] = $profile['lastname'];
		$row['email'] = $result['email'];
		$row['birthday'] = $profile['birthday_day'].LANG_ADMIN_DATE_SEPARATOR.
				   $profile['birthday_month'].LANG_ADMIN_DATE_SEPARATOR.
				   $profile['birthday_year'];

		return $row;
		}
	else {
		return FALSE;
		}
	}

function lb_export_finish() {
	global $export_qres;
	sql_free_result($export_qres);
	}
?>
