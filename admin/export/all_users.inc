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
	$export_qres = sql_query("SELECT id, username, email, password, valid, views, ".
		"date_format(last_login, '%d".LANG_ADMIN_DATE_SEPARATOR."%m".LANG_ADMIN_DATE_SEPARATOR."%Y %H:%i') ".
		"as last_login, expires, registered FROM ".TABLE_USERS." ORDER BY id");

	return array(
                LANG_ADMIN_ID,
                LANG_ADMIN_USERNAME,
                LANG_ADMIN_EMAIL,
                LANG_ADMIN_PASSWORD,
                LANG_ADMIN_VALID,
                LANG_ADMIN_PAGEVIEWS,
                LANG_ADMIN_LAST_LOGIN,
                LANG_ADMIN_EXPIRES,
                LANG_ADMIN_REGISTERED
                );

	}

function lb_export_get_dataset() {
	global $export_qres;
	if($result = sql_fetch_array($export_qres)) {
                if($result['expires'] == 0) {
                        $result['expires'] = LANG_ADMIN_NEVER;
                        }
                else {
                        $result['expires'] = date(LANG_ADMIN_DATE_FORMAT, $result['expires']);
                        }

                if($result['registered'] == 0) {
                        $result['registered'] = LANG_ADMIN_UNKNOWN;
                        }
                else {
                        $result['registered'] = date(LANG_ADMIN_DATE_FORMAT, $result['registered']);
                        }

		if($result['valid'] == '1') { $result['valid'] = LANG_ADMIN_YES; }
		else { $result['valid'] = LANG_ADMIN_NO; }
		return $result;
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
