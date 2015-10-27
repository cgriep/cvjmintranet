<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 01.11.2003
*/
?>
<script language="JavaScript" type="text/javascript">
<!--
<?
	if(SESSION_STATUS == 'ok' && count_messages(SESSION_DBID, 'instant', '0') > 0 && (!isset($profile['im_popup']) || $profile['im_popup'] == '1')) {

	$messages = search_messages(SESSION_DBID, 'instant', 0, time(), -1, '', '', false);

	while(list($key, $value) = each($messages)) { ?>
	newWindows<?=$key?> = open('message.php?docinput[lang]=<?=$lang?>&message_id=<?=$key?>&session_passwd=<?=$session_passwd?>&session_userid=<?=$session_userid?>','message<?=$key?>',"height=269,width=309,dependent=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=no,status=no,location=no");
	<?
	}
}
?>
function sendmessage(user_id)
{
   window.open("message.php?docinput[lang]=<?=$lang?>&session_passwd=<?=$session_passwd?>&session_userid=<?=$session_userid?>&send_message=" + user_id, "SendInstantMessage", "width=309,height=269,dependent=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=no,status=no,location=no");
}

function open_print_window_org()
{
    F1 = window.open("<?=get_url($id,array_merge($_REQUEST,
           array('docinput[design]'=>'Printable')))?>","print","width=600,height=400,dependent=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=yes,status=no,location=no");
    F1.document.close();
}
function open_print_window()
{
    window.print();
}
// -->
</script>

<?
if(file_exists('inc/custom/custom_javascript.inc')) include('inc/custom/custom_javascript.inc');
?>