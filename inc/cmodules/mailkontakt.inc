<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.12.2001
*/

load_lang('mailkontakt');
if ( isset($_REQUEST['Mailabsenden'])) {
  // Mail abschicken
  // Alle Parameter in die Mail
  $message_body = '';
  $sender = "";
  foreach ($_POST as $key => $value)  {
    if ( $key != "Mailabsenden" )
    {
      $message_body .= transform($key,'clean').": ".transform($value, 'clean')."\n";
      if ( strtoupper($key) == "EMAIL" ) $sender = transform($value, 'clean');
    }
  }
  if ( $sender == "" ) $sender = WEBMASTER_MAIL;
  $message_body .= "--------------------------------------------------------\n\n";
  $message_body .= LANG_EMAIL_SENT_BY.' '.email_to_string($sender)."\n";
  $message_body .= LANG_ORIGINAL_DOC."\n".get_url($id, '', true)."\n";
  $message_body .= "IP Adresse\n".$_SERVER["REMOTE_ADDR"]."\n";
  mail($document['eMail'], '['.SITE_TITLE.'] '.transform($document['title'],'clean'), $message_body,
    "From: ".email_to_string($sender).' <'.$sender.'>');
  echo LANG_EMAIL_GESENDET."<br />";
}
        if($profile['editor']!='1') {
                if($document['title'] != '') {
                        echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
                        }
                if($document['image'] != '' || $document['body'] != '') {
                        echo '<form method="post" action="?id='.$id.'">';
                        echo '<p class="justify">';
                        // Image
                        echo transform($document['image'], $document['datatype']['image']);
                        // Text
                        echo transform($document['body'], $document['datatype']['body']).'</p>';
                        echo '<input type="submit" name="Mailabsenden" value="'.LANG_ABSCHICKEN.'">';
                        echo '</form>';
                }
        }
        elseif($profile['editor']=='1') {
                // Image Input
                create_editor_input('Image','image','image_input');
                echo '<br /><br />';
                // Text Input
                create_editor_input('Body','body','area_input');
                echo '<br /><br />';
                // Text Input
                create_editor_input('eMail Empf&auml;nger','eMail','text_input');
                }
?>