<?php
if($profile['editor'] == '1')
	{
	echo 'This mails the messages of the thread ';
	$gettitle = get_nodedata($document[parent_id], $flavour);
	echo $gettitle[title].' to the user ';
	$user = get_user_email($document[email_user_id]);
	echo $user.'.';
	}
?>
