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
	$export_qres = sql_query("SELECT ".TABLE_USERS.".id, username, email FROM ".TABLE_USERS." ORDER BY id");

	return array(
		LANG_ADMIN_USERNAME,
		LANG_ADMIN_FIRSTNAME,
		LANG_ADMIN_LASTNAME,
		LANG_ADMIN_COMPANY,
		LANG_ADMIN_ADDRESS1,
		LANG_ADMIN_ADDRESS2,
		LANG_ADMIN_COUNTRY,
		LANG_ADMIN_EMAIL,
		LANG_ADMIN_PHONE,
		LANG_ADMIN_FAX,
		LANG_ADMIN_MOBILE,
		LANG_ADMIN_ICQ
		); 
	}

function lb_export_get_dataset() {
	global $export_qres;
	if($result = sql_fetch_array($export_qres)) {
		$profile = get_profile($result['id'], FALSE);
		$row['username'] = $result['username'];
		$row['firstname'] = $profile['firstname'];
		$row['lastname'] = $profile['lastname'];
		$row['company'] = $profile['company'];
		$row['address_1'] = $profile['address_1'];
		$row['address_2'] = $profile['address_2'];
		$row['country'] = code_to_country($profile['country']);
		$row['email'] = $result['email'];
		$row['phone'] = $profile['phone'];
		$row['fax'] = $profile['fax'];
		$row['mobile'] = $profile['mobile'];
		$row['icq'] = $profile['icq'];

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
