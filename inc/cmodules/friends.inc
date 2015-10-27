<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

load_lang('user');

$user           = $_REQUEST['user'];
$add_friend     = $_REQUEST['add_friend'];
$remove_friend  = $_REQUEST['remove_friend'];

?>

<script language="javascript">
<!--

function sendicqmessage(user_id)
{
        window.open("message.php?session_passwd=<?=$session_passwd?>&session_userid=<?=$session_userid?>&type=icq&send_message=" + user_id, "SendInstantMessage", "width=299,height=249,dependent=yes,toolbar=no,menubar=no,scrollbars=no,resizable=no,status=no,location=no");
}
 
// -->
</script> 

<?

if(SESSION_STATUS == 'ok') {
	$friends = load_var('friends', SESSION_DBID);
	if($friends == NULL) $friends = array();
	if(isset($add_friend)) $friends[$add_friend] = 1;
	if(isset($remove_friend)) unset($friends[$remove_friend]);
	}

if(isset($user)) {
	include(INC_PATH.'misc/member.inc');
	}
else {
	if($profile['editor']!='1') {
	if($document['title'] != '') {
		echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
		}
	if($document['image'] != '' || $document['body'] != '') {
		echo '<p class="justify">';
		// Image
		echo transform($document['image'], $document['datatype']['image']);
		// Text
		echo transform($document['body'], $document['datatype']['body']).'</p>';
		}

	if(count($friends) == 0) {
		echo LANG_NO_FRIENDS;
		}
	else {
		$users = get_users($friends, 10);

		echo $design['table_begin'];
		
		while(list($key, $value) = each ($users)) { 
			// Does the user still exist?
			if($key < 1) { 
				unset($users[$key]); 
				}
			else { 			
				include(INC_PATH.'misc/memberlist.inc'); 
				}
			}

		echo $design['table_end'];
		}

	}
	elseif($profile['editor']=='1') {
		// Image Input
		create_editor_input('Image','image','image_input');
		echo '<br /><br />';
		// Text Input
		create_editor_input('Body','body','area_input');
		}
	}

if(SESSION_STATUS == 'ok') {
	save_var('friends', $friends, SESSION_DBID);
	}
?>
