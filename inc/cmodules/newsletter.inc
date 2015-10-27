<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.12.2001
*/

load_lang('newsletter');
if ( isset($_REQUEST['Mailabsenden'])) {
  // Mail abschicken
  // Alle Parameter in die Mail
  $message_body = stripslashes($_REQUEST['body']);
  $que = sql_query("SELECT id FROM " . TABLE_USERS." WHERE valid");
  echo "<big><strong>".LANG_NEWSLETTER_VERSCHICKEN."</strong></big><br />";
  $sendemail = get_user_email();
  if ( ! isemail($sendemail)) $sendemail = WEBMASTER_MAIL;
  $Gruppen = $_REQUEST["Gruppen"];
  if ( count($Gruppen) == 0 ) unset($Gruppen);
  while ( $u = sql_fetch_row($que) )
  {
    $prof = get_profile($u[0]);
    $ja = 0;
    if ( $_REQUEST['AnAlle'] == "1" && $profile["SendToAll"] == 1 ) $ja = 1;
    if ( isset($prof["newsletter"]) ) {
      if ( $prof["newsletter"] == "1" ) $ja = 1;
    }
    if ( $ja == 1 ) {
      $drin = true;
      if ( isset($Gruppen) )
      {
        $drin = false;
        reset($Gruppen);
        while ( (list($key, $value) = each($Gruppen)) && (! $drin)) {
          if ( $prof["group_$value"] == 1 )
            $drin = true;
          if ( $value == '-alle-')
          {
          	$drin = true;
          }
        }
      }
      if ( $drin ) {
        $email = get_user_email($u[0]);
        if ( isemail($email) ) {
           mail($email, transform($_REQUEST['Titel'],'clean'), $message_body,
            "From: ".transform($sendemail,'clean'));
          echo "User $u[0] - ".LANG_DOC_SENT_TO." $email<br />";
        }
        else
          echo "User $u[0] - <strong>".LANG_UNGUELTIGE_MAIL.":</strong> $email<br />";
      }
      //else
      //  echo LANG_NOT_GROUP_MEMBER.": ".$prof["nickname"]."<br />";
    }
    //else
    //  echo "User $u[0] - ".LANG_NOT_SUBSCRIBED." ".$prof["nickname"]."<br />";
  }
  sql_free_result($que);
}
if($profile['editor']!='1') {
  if($document['title'] != '') {
    echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
  }
  echo '<form method="post" action="'.get_url($id).'">';
  echo '<p class="justify">';
  // Image
  echo transform($document['image'], $document['datatype']['image']);
  // Text
  echo transform($document['body'], $document['datatype']['body']).'</p>';
  echo LANG_BETREFF.'<br /><input type="text" name="Titel" size="60" value="['.SITE_TITLE.'] Newsletter"><br />';
  echo LANG_INHALT.'<br /><textarea cols="60" rows="10" name="body"></textarea><br />';
  echo "An folgende Gruppen senden<br />";
  echo '<select name="Gruppen[]" size="5" multiple="multiple">';
  reset($profile);
  while(list($key, $value) = each($groups)) {
    if($profile['group_'.$key] == 1 || $profile["SendToAll"] == 1 ) {
      echo '<option value="'.$key.'">'.$value.'</option>';
    }
  }
  if ( $profile['SendToAll'] == 1 ) 
    echo '<option value="-alle-">-alle-</option>';
  reset($profile);
  echo '</select><br /><br />';
  if ( $profile['SendToAll'] == 1 ) {
    echo "An alle User (auch ohne Newsletterwunsch) senden<br />";
    echo '<input type="radio" value="1" name="AnAlle" /> Ja&nbsp;&nbsp;';
    echo '<input type="radio" value="0" name="AnAlle" Checked="checked" /> Nein<br /><br />';
  }
  echo '<input type="submit" name="Mailabsenden" value="'.LANG_ABSCHICKEN.'">';
  echo '</form>';
}
elseif($profile['editor']=='1') {
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Body','body','area_input');
  echo '<br /><br />';
}
?>