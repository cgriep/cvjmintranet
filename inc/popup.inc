	<script language="javascript">
	<!--

<?
	if(SESSION_STATUS == 'ok' && count_messages(SESSION_DBID, 'instant', '0') > 0 && (!isset($profile['im_popup']) || $profile['im_popup'] == '1')) {

	$messages = search_messages(SESSION_DBID, 'instant', 0, time(), -1, '', '', false);

	while(list($key, $value) = each($messages)) { ?>
	newWindows<?=$key?> = open('message.php?docinput[lang]=<?=$lang?>&message_id=<?=$key?>&session_passwd=<?=$session_passwd?>&session_userid=<?=$session_userid?>','message<?=$key?>',"height=269,width=299,dependent=yes,toolbar=no,menubar=no,scrollbars=no,resizable=no,status=no,location=no");
	<? 
	} 
}
?>

	function sendmessage(user_id)
	{
	        window.open("message.php?docinput[lang]=<?=$lang?>&session_passwd=<?=$session_passwd?>&session_userid=<?=$session_userid?>&send_message=" + user_id, "SendInstantMessage", "width=299,height=269,dependent=yes,toolbar=no,menubar=no,scrollbars=no,resizable=no,status=no,location=no");
	}

	// -->
	</script>

<? 
if(file_exists('inc/custom/custom_popup.inc')) include('inc/custom/custom_popup.inc'); 
?>
