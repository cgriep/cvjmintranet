<?php
/*
        Copyright (C) 2000-2002 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 03.06.2002
*/

load_lang('news');
if($profile['editor']!='1') {
  if ( isset($_REQUEST["delete"]) && $_REQUEST["delete"] == 1 && $document['author_id'] == SESSION_DBID )
  {
    destroy_node($id);
    remove_profile('editor_'.$id);
    $id = $document['parent_id'];
    echo "<b>Das Dokument wurde gel&ouml;scht!</b><br />";
    echo "&Uuml;bergeordnetes Dokument:<br />";
    $document = get_node($id, $flavour);
  }
  if ( isset($_REQUEST["savenews"]) && SESSION_STATUS == 'ok' && $profile["newsposter"] == 1 )
  {
     update_nodedata($id, 'body', $docinput['body'], $flavour,'extrans');
     update_nodedata($id, 'title', $docinput['title'], $flavour, 'text');
     update_nodedata($id, 'icon', $docinput['icon'], $flavour);
     clear_cache($id);
     /* Neu: L&ouml;schen des News-Cache */
     @unlink(BASE_PATH.'cache/modules/news_'.md5(get_cmodule_id('news').$design['name'].$flavour));
     $document = get_node($id, $flavour);
  }
  if(isset($_REQUEST['action']) && $_REQUEST['action'] == "send_email") {
?>
    <form action="<?=$_SERVER['PHP_SELF']?>" method="post">
    <input type="hidden" name="id" value="<?=$id?>">
    <input type="hidden" name="docinput[save]" value="send_email">
    <p>
    <b><?=LANG_SEND_THIS_DOCUMENT?></b><br />
    <input type="text" name="docinput[email]" size="50"><br />
    <?=LANG_MULTIPLE_RECIPIENTS?>
    <br />
    <br />
    <? if(SESSION_STATUS != 'ok') { ?>
      <?=LANG_YOUR_EMAIL_ADDRESS?><br />
      <input type="text" name="docinput[sender]" size="50"><br />
      <br />
    <? }
    else { ?>
      <input type="hidden" name="docinput[sender]" value="<?=SESSION_EMAIL?>">
    <? } ?>
    <?=LANG_MESSAGE?><br />
    <input type="text" name="docinput[body]" size="50"><br />
    <br />
    <input type="submit" value="Send">
    </form>
    <?
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
    $message_body .= transform($document['timestamp'], 'date')."\n\n";
    if(isset($document['title'])) $message_body .= transform($document['title'],'clean')."\n\n";
    if(isset($document['body']))  $message_body .= transform(transform($document['body'],
      $value['datatype']['body']),'html2txt')."\n\n";
    $message_body .= "--------------------------------------------------------\n\n";
    $message_body .= LANG_EMAIL_SENT_BY.' '.email_to_string($docinput['sender'])."\n";
    $message_body .= LANG_ORIGINAL_DOC."\n".get_url($id, '', true)."\n";
    mail($docinput['email'], '['.SITE_TITLE.'] '.transform($document['title'],'clean'),
           $message_body, "From: ".email_to_string($docinput['sender']).' <'.$docinput['sender'].'>');
    ?>
    <p><?=LANG_DOC_SENT_TO?> <b><?=$docinput['email']?></b></p>
    <?
  }
  if ( $_REQUEST["Edit"] == "1" ) {
  ?>
    <form action="<?=get_url($id)?>" method="post"><?=LANG_TITLE?>
    <br /><input type="text" name="docinput[title]" size="<?=$design['input_size']?>"
    maxlength="60" value="<?=$document['title']?>">
    <br /><?=LANG_BODY?><br /><textarea name="docinput[body]" cols="<?=$design['input_size']?>"
    wrap="virtual" rows="<?=$design['input_rows']?>"><?=$document["body"]?></textarea>
    <br /><br /><?=LANG_ICON?><br />
    <select name="docinput[icon]"><option value="">No Icon</option>
    <?
    $handle=opendir($image_path.'news/');
    while (false!==($file = readdir($handle)))
      if ($file != "." && $file != "..")
        $icons[] = $file;
    natcasesort($icons);
    while(list($key, $file) = each($icons)) {
      echo '<option value="'.$file.'"';
      if ( $file == $document['icon'] ) echo ' selected="selected"';
      echo '>'.substr($file, 0, -4).'</option>';
    }?>
    </select>
    <br /><br /><input type="submit" value="<?=LANG_SUBMIT?>" name="savenews"></form>
  <?
  }
  else {
    echo '<p class="newsDate">'.transform($document['timestamp'], 'date')."</p>";
    echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
    if($document['source'] != '')
      echo '<i>'.LANG_SOURCE.': '.transform($document['source'], $document['datatype']['source']).'</i>';
    echo '<p class="justify">';
    if($document['icon'] != '') echo transform($image_rpath.'news/'.$document['icon'], 'image_right');
    echo transform($document['image'], $document['datatype']['image']).'</p>';
    echo transform($document['body'], $document['datatype']['body']).'</p>';
    echo "<br />";
    echo "<br />";
  } // else
  if(isset($parents)) {
    echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_BACK.'</a> ] ');
  }
  if(!isset($action)) {
    echo '[ <a href="'.get_url($id,'action=send_email').'">'.LANG_SEND_TO_A_FRIEND.'</a> ]';
  }
  if(SESSION_STATUS == 'ok' && $profile['newsposter'] == '1') {
    echo ' [ <a href="'.get_url($id,'Edit=1').'">'.LANG_EDIT.'</a> ]';
  }
  if ( ! isset($_REQUEST['delete']) ) {
    if ( $document['author_id'] == SESSION_DBID ) {
      echo ' [ <a href="'.get_url($id,'delete=1').'">'.LANG_DELETE.'</a> ]  ';
    }
  }
  echo('</p>');
  $related = array();
  $related = get_sorted_children(0, 10, 'timestamp', $doctypes['newsitem'], $document['parent_id']);
  if(count($related) > 1) {
    echo('<p>'.LANG_MORE_NEWS.'<br />');
    reset($related);
    while (list ($docid2, $value2) = each ($related)) {
      if($value2['title'] != '' && $docid2 != $id) {
        echo '<img src="'.$design['arrow_bullet'].'" border="0" hspace="2" width="10" height="7" />'.
             '<a href="'.get_url($docid2).'">'.transform($value2['title'], 'clean').
             '</a> ('.transform($value2['timestamp'], 'date').')<br />'."\n";
      }
    }
    echo('</p><br><br>');
  } // if count
} // Profile = Editor
else {
        ?>
        <p>
        <table width="100%" border="0" cellpadding="0" cellspacing="0"><tr>
        <td>
        <? create_editor_input('User comments at bottom', 'comments', 'onoff_input'); ?>
        <input type="hidden" name="docdataflavour[comments]" value="0" />
        </td><td>
        <? create_editor_input('Show published files at bottom','show_files','onoff_input'); ?>
        </td></tr></table><p>
        <? create_editor_input('Source','source','text_input'); ?>
        <br /><br />
        <? create_editor_input('Link to external URL (optional)','redirect','text_input'); ?>
        </p>
        <p>
        <input type="hidden" name="docdataflavour[icon]" value="0">
        <?
        create_editor_input('Icon','icon','icon_input');
        echo '<br /><br />';
        create_editor_input('Image', 'image', 'image_input');
        echo '<br /><br />';
        create_editor_input('Body', 'body', 'area_input');
    }

?>