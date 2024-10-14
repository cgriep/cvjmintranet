<?php
/*	Functions for the forum module.
	maintained by Christoph Stickel <mixer2@web.de>	*/

function SendPostingMail($mid, $docinput, $neu = true)
{
  if ( $neu )
    $message = "Neuer Eintrag von ".get_user_nickname(SESSION_DBID);
  else
    $message = "Eintrag korrigiert von ".get_user_nickname(SESSION_DBID);

  global $flavour;
  global $parents;
  global $doctypes;
  reset($parents);
  global $document;
  // Neueinträge bearbeiten
  if ( $document["type_id"] == $doctypes["forum_forum"] )
  {
    global $id;
    $Forumid = $id;
    $Forum = $document;
  }
  else
  {
    while ( list($Forumid, $Forum) = each($parents) )
    {
      $Forum = get_node($Forumid, $flavour, false);
      if ( $Forum['type_id'] == $doctypes["forum_forum"] ) {
        break;
      }
    }
  }
  $message .= " in Forum ".$Forum["title"]."\n\n";
  if ( $document["type_id"] == $doctypes["forum_posting"] )
  {
    $message .= "Antwort auf: ".transform($document['title'], 'clean').", geschrieben von ".
      get_user_nickname($document['userid'])." am ".transform($document["timestamp"],"datetime")."\n\n";
  }
  $message .= $docinput["title"]."\n".stripslashes($docinput["body"])."\n";
  $message .= "Direkter Link: ".get_url($mid,'',true);
  $message .= "\n\nGeschrieben am ".date("d.m.Y H:i");
  // Alle User durchgehen
  if ( $result = sql_query("SELECT user_id FROM ".TABLE_USERDATA." WHERE name='MailForum_".$Forumid."'") )
  {
    while ( $row = sql_fetch_row($result) )
    {
      send_message ($row[0], $message, SESSION_DBID, '', '['.$Forum['title'].'] - '.$docinput['title'],'email');

//! mail(get_user_email($row[0]), "[".$Forum["title"]."] - ".$docinput['title'],
 //          $message, "From: ".get_user_nickname(SESSION_DBID).' <'.
 //            get_user_email(SESSION_DBID).">\nContent-type: text/html; charset=iso-8859-1") ) echo "Fehler: ".$row[0];
         //else echo "Mail an ".$row[0]."<br />";
    }
    sql_free_result($result);
  }
  else
    echo "Fehler: ".sql_error();
}

function printnick($value) {
	if(isset($value['userid'])) {
		$nick = '<b>'.transform($value['userid'], 'user_id').'</b>';
		}
	else {
		if(ereg('@',$value['email'])) {
			$nick = '<a href="mailto:'.transform($value['email'], 'clean').'">'.transform(substr($value['nick'],0,15), 'clean');
			if(strlen($value['nick'])>15) { $nick = $nick.'...'; }
			$nick = $nick.'</a>';
			}
		else { $nick = transform(substr($value['nick'],0,15), 'clean'); if(strlen($value['nick'])>15){$nick = $nick.'...';} }
		}
	return $nick;
	}

function getswitchcolor() {
	global $design;
	global $colorswitch;
	if($colorswitch == '0') {  echo $design['table_row_dark']; return '1'; }
	else { echo $design['table_row_bright']; return '0'; }
	}

function delsendpermail ($userid) {
	if(!isset($userid) || $userid < 0) {
		return 'error';
		}
	else	{
		global $id;
		global $doctypes;
		global $childcount;
		$result=get_children(0,$childcount['forum_mailto'],$doctypes['forum_mailto']);
		while(list($key, $value) = each($result)) {
			if($value['email_user_id'] == $userid) {
				destroy_node($key);
				--$childcount['forum_mailto'];
				}
			}
		}
	}

function issendpermail ($userid)
	{
	if(!isset($userid) || $userid < 0)
		{
		return 'error';
		}
	else	{
		global $id;
		global $doctypes;
		global $childcount;
		$result=get_children(0,$childcount['forum_mailto'],$doctypes['forum_mailto']);
		while(list($key, $value) = each($result)) {
			if($value['email_user_id'] == $userid) {
				$alreadyex = '1';
				}
			}
		if($alreadyex == '1') {	return '1'; }
		else { return '0'; }
		}
	}

function sendcomplete ($userid, $docid)
	{
	global $flavour;
	global $doctypes;
	$SERVER_NAME = $_SERVER['SERVER_NAME'];
        $REQUEST_URI = $_SERVER['REQUEST_URI'];
	global $webmaster_mail;
	global $childcount;
	global $id;

	$result = get_nodedata($docid, $flavour);
	$mailemail = get_user_email($userid);
	$mailtitle = $result['title'];

	$message_body = transform(time(), 'date')."\n\n";

	if(isset($result['userid'])) {
		$message_body .= '['.transform($result['title'],'clean').']'."\n(".email_to_string(get_user_email($result['userid']))." <".get_user_email($result['userid']).">, ".transform($result['time'], 'datetime').")\n".transform(transform($result['body'], 'usertext'),'html2txt'); }
	else { $message_body .= '['.transform($result['title'],'clean').']'."\n(".email_to_string($result['email'])." <".$result['email'].">, ".transform($result['time'], 'datetime').")\n".transform(transform($result['body'], 'usertext'),'html2txt'); }

	$result = get_children(0, $childcount['forum_posting'], $doctypes['forum_posting'], 0, 'DESC', 0, $docid);
	while(list($key, $value) = each($result)) {
		if(isset($value['userid'])) {
		$message_body .= "\n\n".'['.transform($value['title'],'clean').']'."\n(".email_to_string(get_user_email($value['userid']))." <".get_user_email($value['userid']).">, ".transform($value['time'], 'datetime').")\n".transform(transform($value['body'], 'usertext'),'html2txt'); }
		else { $message_body .= "\n\n".'['.transform($value['title'],'clean').']'."\n(".email_to_string($value['email'])." <".$value['email'].">, ".transform($value['time'], 'datetime').")\n".transform(transform($value['body'], 'usertext'),'html2txt'); }
		}
	$message_body .= "\n\n--------------------------------------------------------\n\n";
 	$message_body .= LANG_EMAIL_SENT_BY.' '.email_to_string($webmaster_mail)."\n";
       	$message_body .= LANG_ORIGINAL_DOC."\nhttp://".$SERVER_NAME.get_url($id)."\n";
	mail($mailemail, '['.SITE_TITLE.'] '.transform($mailtitle,'clean'), stripslashes($message_body), "From: ".email_to_string($webmaster_mail).' <'.$webmaster_mail.'>');
	return '0';
	}

function fhost($host)
	{
	global $parentdata;
	if($parentdata['showhost'] == 1) { return ' ( ---'.strstr($host,'.').')'; }
	else { return ''; }
	}

function fhostadmin($host)
	{
	return $host;
	}

?>
