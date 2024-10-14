<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $id;
global $self;
global $docinput;

$recover = $_GET['recover'];
$first_item = $_REQUEST['first_item'];
$subject = $_REQUEST['subject'];

if($recover == 'true' && isset($docinput['user_id'])) {
	$qresult = mysqli_query ($GLOBALS['dbconnection'], "SELECT password FROM ".TABLE_USERS." WHERE email='".$docinput['user_id']."'");
	if ($qresult) {
		$row = sql_fetch_row($qresult);
		$message = "$row[0]";
		mail($docinput['user_id'], '['.SITE_TITLE.'] Login', $message,
        	"From: ".WEBMASTER_MAIL."\nReply-To: ".WEBMASTER_MAIL);
		}
	echo '<span style="font-weight: bold; color: #00aa00">'.LANG_PASSWD_SENT.'</span><p>';
	}
if(FORCE_SSL && !SSL) {
	echo LANG_SSL_REQUIRED;
	}
elseif(SESSION_STATUS != 'ok') {
	// Prevent Netscape 4 from making the input fields too long...
	if(strstr($_SERVER['HTTP_USER_AGENT'], 'Mozilla/4') !== false && strstr($_SERVER['HTTP_USER_AGENT'], 'compatible') === false) {
		// Netscape 4.x
		$input_size = "12";
		}
	else {
		// NS6, Mozilla, MSIE
		$input_size = "19";
		}
        if(SESSION_STATUS == 'passwd' && $docinput['passwd2'] != '') {
        echo '<span style="font-weight: bold; color: #00aa00">'.LANG_BAD_PASSWORD.'</span><br />
	<a href="'.$self.'docinput[user_id]='.$docinput['user_id'].'&recover=true">'.LANG_RECOVER_PASSWD.'</a>';
        }
        if(SESSION_STATUS != 'passwd' && $docinput['passwd2'] != '') {
        echo '<span style="font-weight: bold; color: #00aa00">'.LANG_BAD_EMAIL.'</span>';
        }
        ?>
        <form action="<?=get_url($id)?>" method="post" class="text">
        <input type="hidden" name="id" value="<?=$id?>" />
        <input type="hidden" name="first_item" value="<?=$first_item?>" />
        <input type="hidden" name="subject" value="<?=$subject?>" />
        <?=LANG_EMAIL_ADDRESS?><br />
        <input type="text" name="docinput[user_id]" size="<?=$input_size?>" value="<?=$docinput['user_id']?>" /><br /><br />
        <?=LANG_PASSWORD?><br />
        <input type="password" name="docinput[passwd2]" size="<?=$input_size?>" /><br /><br />
        <input type="submit" value="<?=LANG_LOGIN?>" />
        </form>
        <?php
        }
else { ?>
        <p style="text-align: center; color: #00aa00; font-weight: bold">
	<?=str_replace('%%email%%',SESSION_EMAIL,LANG_LOGGED_IN)?>
	</p>
        <form style="text-align: center" action="<?=$_SERVER['PHP_SELF']?>" method="post" class="text">
        <input type="hidden" name="id" value="<?=$id?>" />
        <input type="hidden" name="first_item" value="<?=$first_item?>" />
        <input type="hidden" name="docinput[logout]" value="true" />
        <input type="submit" value="<?=LANG_LOGOUT?>" />
        </form>
        <?php
        }
?>

