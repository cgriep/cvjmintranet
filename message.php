<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

	$send_message = $_REQUEST['send_message'];
	if ( isset($_REQUEST['type'])) 
          $type = $_REQUEST['type'];
	if ( isset($_REQUEST['body']))
          $body = $_REQUEST['body'];
        if ( isset($_REQUEST['message_id']))
	  $message_id = $_REQUEST['message_id'];
        include("inc/functions.inc");
        include("inc/licence.key");
        include("inc/sessions.inc");
        include("inc/caching.inc");
        include("inc/config.inc");
        include(INC_PATH.INIT_INC);
        include(INC_PATH.DESIGN_INC);

	echo '<?xml version="1.0" encoding="'.CHARSET.'"?>';
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3c.org/TR/xhtml1/DTD/transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta name="format" value="text/xhtml"/>
<meta name="robots" value="index,follow"/>
<meta name="charset" value="iso-8859-1"/>
<meta name="generator" value="adaptive website framework"/>
<meta name="description" content="awf instant message"/>
<meta http-equiv="expires" content="0"/>
<meta http-equiv="pragma" content="no-cache"/>
<title><?=SITE_TITLE?> &gt; <?=LANG_INSTANT_MESSAGE?></title>
<link rel=stylesheet type="text/css" href="css/default.css"/>
</head>
<body bgcolor="#ffffff" marginheight="0" marginwidth="0" topmargin="0" bottommargin="0" rightmargin="0" leftmargin="0" link="#0000ff" vlink="#0000ff" text="#000000">
<table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="#ffffff"><tr>
<td align="left" bgcolor="#dedede" valign="top" class="text">
<?

if(isset($send_message) && !isset($type)) {
	$_profile = get_profile($send_message);
	if(!isset($_profile['im_default']) || $_profile['im_default'] == '') {
                $type = 'instant';
                }
        else {
                $type = $_profile['im_default'];
                }
	}

if(!isset($type)) $type = 'default';

switch($type) {
	case 'instant':
	case 'default': $type_long = LANG_INSTANT_MESSAGE; break;
	case 'email'  : $type_long = LANG_EMAIL; break;
	case 'icq'  : $type_long = LANG_ICQ_MESSAGE; break;
	}

if(SESSION_STATUS != 'ok' && !isset($send_message)) { ?>

<p><b>Bye!</b></p>
</td></tr></table>

<? } elseif(SESSION_STATUS != 'ok' && isset($send_message) && !isset($body)) { ?>
<b><?=printf(LANG_SEND_MESSAGE,LANG_ANONYMOUS_MESSAGE)?></b><br />
<span style="font-size:10px"><?=LANG_TO?> <?=get_user_nickname($send_message)?></span>
</td>
<td align="right" bgcolor="#dedede" valign="middle">
<img src="img/icons_32/mail_reply.png" border="0" />
</td>
</tr><tr>
<td colspan="2" align="left" bgcolor="#ffffff" valign="top" style="text-align: center">
<form action="<?=$_SERVER['PHP_SELF']?>"?>
<input type="hidden" name="send_message" value="<?=$send_message?>">
<input type="hidden" name="type" value="<?=$type?>">
<textarea wrap="virtual" name="body" cols="25" rows="9"></textarea><br />
<input type="submit" value="<?=LANG_SEND?>">
</form>
</td></tr></table>
</p>

<? } elseif(SESSION_STATUS != 'ok' && isset($send_message) && isset($body)) {
send_message ($send_message, $body, -1, 'Anonymous User', '['.SITE_TITLE.'] Anonymous Message', $type);
?>
<b><?=printf(LANG_SENT_MESSAGE, LANG_ANONYMOUS_MESSAGE)?></b><br />
<span style="font-size:10px"><?=LANG_TO?> <?=get_user_nickname($send_message)?></span>
</td>
<td align="right" bgcolor="#dedede" valign="middle">
<img src="img/icons_32/mail_generic.png" border="0" />
</td>
</tr>

<tr>
<td colspan="2" align="left" bgcolor="#ffffff" valign="top" style="text">
<p>
<?=stripslashes($body)?>
</p>
<p style="text-align: center">
[ <a href="javascript:window.close();"><?=LANG_CLOSE?></a> ]
</p>
</td>
</tr>

</table>
</p>

<? } elseif(!isset($send_message)) { ?>

<b><?=$type_long?></b><br />
<?php $message = get_message($message_id, true); 
if($message['user_id'] != SESSION_DBID) exit(); ?>
<span style="font-size:10px"><?=LANG_SENT_BY?> 
<? if(isset($message['sender_id']) && $message['sender_id'] > 0) { ?>
<a href="<?=$_SERVER['PHP_SELF']?>?message_id=<?=$message['message_id']?>&send_message=<?=$message['sender_id']?>&session_passwd=<?=$session_passwd?>&session_userid=<?=$session_userid?>"><?=get_user_nickname($message['sender_id'])?></a> 
<? } elseif(isset($message['sender']) && $message['sender'] != '') { ?>
<?=url_to_link($message['sender'], true)?> 
<? } else { ?>
<?=LANG_ANONYMOUS_USER?>
<? } ?>
<?=LANG_ON?> <?=timestamp_to_string($message['created'])?></span></p></td>
<td align="right" bgcolor="#dedede" valign="middle">
<? if(isset($message['sender_id']) && $message['sender_id'] > 0) { ?>
<form action="<?=$_SERVER['PHP_SELF']?>"?>
<input type="hidden" name="type" value="<?=$type?>">
<input type="hidden" name="send_message" value="<?=$message['sender_id']?>">
<input type="hidden" name="message_id" value="<?=$message_id?>">
<input type="hidden" name="session_userid" value="<?=$session_userid?>">
<input type="hidden" name="session_passwd" value="<?=$session_passwd?>">
<input type="submit" value="<?=LANG_REPLY?>"></form><? } ?></td>
</tr><tr>
<td colspan="2" align="left" bgcolor="#ffffff" valign="top">
<?=transform(stripslashes($message['message']), 'usertext')?>
</p><p style="text-align: center">
[ <a href="javascript:window.close();"><?=LANG_CLOSE?></a> ]</p>
</td></tr></table>
</p>

<? } elseif(isset($send_message) && !isset($body)) { ?>
<b><?=printf(LANG_SEND_MESSAGE, $type_long)?></b><br />
<?php 
	if(isset($message_id) && $message_id != '') { 
		$message = get_message($message_id, false); 
		if($message['user_id'] != SESSION_DBID) exit();
		}
	else { $message['sender_id'] = $send_message; }
?>
<span style="font-size:10px"><?=LANG_TO?> <?=get_user_nickname($message['sender_id'])?></span>
</td>
<td align="right" bgcolor="#dedede" valign="middle">
<img src="img/icons_32/mail_reply.png" border="0" />
</td>
</tr><tr>
<td colspan="2" align="left" bgcolor="#ffffff" valign="top" style="text-align: center">
<form action="<?=$_SERVER['PHP_SELF']?>"?>
<input type="hidden" name="type" value="<?=$type?>">
<input type="hidden" name="send_message" value="<?=$message['sender_id']?>">
<input type="hidden" name="message_id" value="<?=$message_id?>">
<input type="hidden" name="session_userid" value="<?=$session_userid?>">
<input type="hidden" name="session_passwd" value="<?=$session_passwd?>">
<textarea wrap="virtual" name="body" cols="25" rows="9"></textarea><br />
<input type="submit" value="<?=LANG_SEND?>">
</form>
</td></tr></table>
</p>

<? } elseif(isset($send_message) && isset($body)) { 
	if(isset($message_id) && $message_id != '') { 
		$message = get_message($message_id, false); 
		if($message['user_id'] != SESSION_DBID) exit();
		}
	else { $message['sender_id'] = $send_message; }
	send_message ($send_message, $body, SESSION_DBID, '', '['.SITE_TITLE.'] Message', $type);
?>
<b><?=printf(LANG_SENT_MESSAGE, $type_long)?></b><br />
<span style="font-size:10px"><?=LANG_TO?> <?=get_user_nickname($message['sender_id'])?></span>
</td>
<td align="right" bgcolor="#dedede" valign="middle">
<img src="img/icons_32/mail_generic.png" border="0" />
</td>
</tr><tr>
<td colspan="2" align="left" bgcolor="#ffffff" valign="top">
<?=transform(stripslashes($body), 'usertext')?>
</p><p style="text-align: center">
[ <a href="javascript:window.close();"><?=LANG_CLOSE?></a> ]</p>
</td></tr></table>
</p>
<? } ?>
</body>
</html>
