<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.12.2001

        AWF-Name: file_para

*/
if($profile['editor']!='1') {
  load_lang('file_doc');
  if(isset($_REQUEST['action']) && $_REQUEST['action'] == "send_email") {
?>
  <form action="<?=$_SERVER['PHP_SELF']?>" method="post">
  <input type="hidden" name="id" value="<?=$id?>" />
  <input type="hidden" name="docinput[save]" value="send_email" />
  <p>
  <strong><?=LANG_SEND_THIS_DOCUMENT?></strong><br />
  <input type="text" name="docinput[email]" size="50"><br />
  <?=LANG_MULTIPLE_RECIPIENTS?>
  <br />
  <br />
  <?php if(SESSION_STATUS != 'ok') { ?>
    <?=LANG_YOUR_EMAIL_ADDRESS?><br />
    <strong>Sie müssen eingeloggt sein, um Nachrichten zu versenden!</strong>
    <br />
  <?php }
  else { ?>
    <input type="hidden" name="docinput[sender]" value="<?=SESSION_EMAIL?>" />
  <?php } ?>
  <?=LANG_MESSAGE?><br />
  <input type="text" name="docinput[body]" size="50" /><br />
  <br />
  <input type="submit" value="Send" /></p>
  </form>
  <?php
  }
  elseif(isset($docinput['save']) && $docinput['save'] == "send_email" && isset($docinput['email']) &&
   strlen($docinput['email']) > 3 && isemail($docinput['sender'])) {
    if($docinput['sender'] == '') { $docinput['sender'] = 'unknown_user@'.$SERVER_NAME; }
    if($docinput['body'] != '') {
      $message_body = $docinput['body']."\n\n";
    }
    else {
      $message_body = '';
    }
    $message_body .= "--------------------------------------------------------\n\n";
    $message_body .= LANG_EMAIL_SENT_BY.' '.email_to_string($docinput['sender'])."\n";
    $message_body .= LANG_ORIGINAL_DOC."\n".get_url($id, '', true)."\n";
    $message_body .= LANG_ANLAGEN."\n";
    $bbase = $document['basedir'];
    if ( $bbase == "" ) $bbase = get_dir_name('files',$id).'/published/';
    $handle=opendir($bbase);
    while (false != ($file = readdir($handle))) {
      if(filetype($bbase.$file) == 'dir' || filetype($bbase.$file) == 'link') { $dirlist[] = $file; }
      if(filetype($bbase.$file) == 'file') {
        $filelist[] = $file." (".floor(filesize($bbase."/".$file) / 1024)." kB)";
      }
    }
    sort($filelist);
    sort($dirlist);
    $filelist = array_merge($dirlist,$filelist);
    foreach ($filelist as $nr => $file) {
      if ($file != "." && $file != ".." && strlen($file) > 0 && $file[0] != '.') {
        $message_body .= $file;
      }
    }
    mail($docinput['email'], '['.SITE_TITLE.'] '.transform($document['title'],'clean'), $message_body,
        "From: ".email_to_string($docinput['sender']).' <'.$docinput['sender'].'>');
    ?>
      <p><?=LANG_DOC_SENT_TO?> <b><?=$docinput['email']?></b></p>
    <?php
  }
  if ($document['author_id'] == SESSION_DBID ) {
    global $profile;
    if ( isset($_FILES["userfile"]) )
    {
      $profile['allow_upload'] = '1';
    }
    if ( isset($_REQUEST["docinput"]) )
    {
      $profile['allow_delete'] = '1';
      $profile['allow_createdir'] = '1';
    }
  }
  if ( isset($_REQUEST["action"]) && $_REQUEST["action"] == "delete" && $document['author_id'] == SESSION_DBID )
  {
    destroy_node($id);
    clear_cache($id);
    remove_profile('editor_'.$id);
    $id = $document['parent_id'];
    echo "<b>Das Dokument wurde gelöscht!</b><br />";
    echo "Übergeordnetes Dokument:<br />";
    $document = get_node($id, $flavour);
  }
  elseif ( $_REQUEST["action"] == "edit" && $document['author_id'] == SESSION_DBID) {
    // Bearbeiten
    echo '<form action="'.get_url($id).'" method="post">'.LANG_TITLE;
    echo '<br /><input type="text" name="docinput[title]"  size="'.$design['input_size'];
    echo '" maxlength="80" value="'.$document['title'].'">';
    echo '<br />'.LANG_BODY.'<br /><textarea name="docinput[body]" cols="'.$design['input_size'];
    echo '" wrap="virtual" rows="'.$design['input_rows'].'">';
    echo $document['body'];
    echo '</textarea><br />';
    echo '<input type="hidden" name="action" value="savefile">';
    echo '<input type="Submit" name="savefile" value="'.LANG_SUBMIT.'">';
    echo '</form><br /><br />';
    global $profile;
    $profile['allow_upload'] = '1';
    $profile['allow_delete'] = '1';
    $profile['allow_createdir'] = '1';
  }
  elseif ( $_REQUEST["action"] == "savefile" && $document['author_id'] == SESSION_DBID ) {
    if( $docinput['title'] == '') {
      ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_TITLE_AND_BODY_REQUIRED?></font><?php
    }
    else {
      update_nodedata($id, 'body', $docinput['body'], $flavour, 'extrans');
      update_nodedata($id, 'title', $docinput['title'], $flavour, 'text');
      $document["body"] = $docinput['body'];
      $document["title"] = $docinput['title'];
    }
  }
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
  if ( $document['Link'] != "" )
    echo '<p>Link erzeugt am '.transform($document['timestamp'],'datetime').' von ';
  else
    echo '<p>Seite erstellt am '.transform($document['timestamp'],'datetime').' von ';
  echo transform($document['author_id'], 'user_id')."</p>";
  if ( $document['Link'] != "" ) {
    echo '<strong>Zum Aufsuchen dieser Seite <a target="_blank" href="/redirect.php?url=';
    echo urlencode($document["Link"]).'">hier klicken</a></strong>.<br /><br />';
    echo '(die Seite öffnet sich in einem eigenen Fenster. Sie befindet sich auf einer anderen Website. Ihr Inhalt liegt ';
    echo 'außerhalb des Einflußbereiches der Betreiber dieser Seite.)<br /><br />';
  }
  if(isset($parents)) {
    echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_SHOW_COMPLETE_DOC.'</a> ]  ');
  }
  if ( ! isset($_REQUEST['action']) ) {
    if ( $document['author_id'] == SESSION_DBID ) {
      echo ' [ <a href="'.get_url($id,'action=delete').'">'.LANG_DELETE.'</a> ]  ';
      echo ' [ <a href="'.get_url($id,'action=edit').'">'.LANG_EDIT.'</a> ]  ';
    }
    if(SESSION_STATUS == 'ok')
      echo ' [ <a href="'.get_url($id,'action=send_email').'">'.LANG_SEND_TO_A_FRIEND.'</a> ]  ';
  }
}
else {
  if ( is_numeric($document["Kopie"]) )
  {
    if ( $document["redirect"] != get_url($document["Kopie"]) )
    {
      // Redirection einrichten
      add_nodedata($id, "redirect", get_url($document["Kopie"]), $flavour);
    }
  }
  elseif ( isset($document["redirect"]) )
  {
     // Redirection löschen
     delete_nodedata($id, "redirect", "", $flavour);
  }
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Body','body','area_input');
  echo '<br /><br />';
  create_editor_input('Link auf Seite', 'Link', 'text_input');
  echo '<br /><br />';
  create_editor_input('Interner Link auf (Kopie von) id', 'Kopie', 'text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Show published files at bottom','show_files','onoff_input');
  echo '<br /><br />';
  create_editor_input('User comments at bottom','comments','onoff_input');
}

?>