<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 03.06.2002
*/

if($profile['editor']!='1') {
  load_lang('Angebot');
  if(SESSION_STATUS == 'ok' && $profile['angebotsposter'] == '1' && isset($_REQUEST['saveangebot'])) {
    if($docinput['body'] == '' || $docinput['title'] == '') {
      ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_TITLE_AND_BODY_REQUIRED?></font><?
    }
    else {
      $nodeid = create_node($id, $doctypes['angebot_para'], count_children($doctypes['angebot_para']), 1,0, "", $document['key_id'],
      $document['searchable'], $document['cacheable'], 0, $document['members_only']);
      add_nodedata($nodeid,'timestamp',time(), $flavour);
      add_nodedata($nodeid, 'body', $docinput['body'], $flavour,'extrans');
      add_nodedata($nodeid, 'title', $docinput['title'], $flavour);
      add_nodedata($nodeid, 'comments', $docinput['comments'], $flavour);
      add_nodedata($nodeid, 'show_files', $docinput['show_files'], $flavour);
      add_nodedata($nodeid, 'basedir', $docinput['directory'], $flavour);
      add_nodedata($nodeid, 'icon', $docinput['icon'], $flavour);
      add_nodedata($nodeid, 'author', SESSION_EMAIL,$flavour,'email');
      add_nodedata($nodeid, 'author_id', SESSION_DBID, $flavour, 'user_id');
      if ( $docinput['GueltigAb'] == "" || strtotime($docinput['GueltigAb']) == -1 )
        $docinput['GueltigAb'] = 0;
      else
        $docinput['GueltigAb'] = strtotime($docinput['GueltigAb']);
      add_nodedata($nodeid, 'GueltigAb', $docinput['GueltigAb'], $flavour, 'datetime');
      if ( $docinput['expiration'] != "" && strtotime($docinput['expiration']) != -1 )
        sql_query("UPDATE ".TABLE_NODES." SET expiration_date = ".strtotime($docinput['expiration']).
          " WHERE id=$nodeid");
      clear_cache($id);
      echo LANG_ANGEBOT_GESPEICHERT."<br />";
    }
  }
  if($_REQUEST['postangebot'] == '1' && SESSION_STATUS == 'ok' && $profile['angebotsposter'] == '1') {
        ?><form action="<?=get_url($id)?>" method="post"><?=LANG_TITLE?>
        <br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>" maxlength="60" value="<?=$docinput['title']?>">
        <br /><?=LANG_BODY?><br /><textarea name="docinput[body]" cols="<?=$design['input_size']?>" wrap="virtual" rows="<?=$design['input_rows']?>"></textarea>
        <br /><?=LANG_COMMENTS?><br />
        <input type="radio" value="1" name="docinput[comments]" /> Enable&nbsp;&nbsp;
        <input type="radio" value="0" name="docinput[comments]" CHECKED /> Disable
        <br /><br /><?=LANG_SHOW_PUBLISHED_FILES?><br />
        <input type="radio" value="1" name="docinput[show_files]" /> Enable&nbsp;&nbsp;
        <input type="radio" value="0" name="docinput[show_files]" CHECKED /> Disable
        <br /><br /><?=LANG_DIRECTORY?><br /><input type="text" name="docinput[directory]"  size="<?=$design['input_size']?>" maxlength="60" value="" />
        <br /><br /><?=LANG_GUELTIG_AB?> (JJJJ-MM-DD)<br />
        <input type="text" name="docinput[GueltigAb]"  size="<?=$design['input_size']?>" maxlength="20" value="" />
        <br /><br /><?=LANG_GUELTIG_BIS?> (JJJJ-MM-DD)<br />
        <input type="text" name="docinput[expiration]"  size="<?=$design['input_size']?>" maxlength="20" value="" />
        <br /><br /><input type="submit" name="saveangebot" value="<?=LANG_SUBMIT?>"></form><?
  }
  if($document['title'] != '') {
    echo '<h2 clear="all">'.transform($document['title'], $document['datatype']['title']).'</h2>';
    if($document['image'] != '' || $document['summary'] != '') {
      echo '<p class="justify">';
      // Image
      echo transform($document['image'], $document['datatype']['image']);
      // Text
      echo transform($document['summary'], $document['datatype']['summary']).'</p><br clear="all" />';
    }
  }
  $children =  get_children (0, 100, $doctypes['angebot_para']);
  $inc_value = 10;
  if(!isset($first_item) || !$first_item) $first_item = 0;
  $max_newscount = $first_item + $inc_value;
  $total_count=0;
  if(isset($children)) {
        $total_count = count_children($doctypes['angebot_para'], $flavour);
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
  elseif(isset($docinput['save']) && $docinput['save'] == "send_email" && isset($docinput['email']) && strlen($docinput['email']) > 3 && isemail($docinput['sender'])) {
    if(isset($children)) {
      reset($children);
      if($docinput['sender'] == '') { $docinput['sender'] = 'unknown_user@'.$SERVER_NAME; }
      $counter = 0;
      if($docinput['body'] != '') {
        $message_body = $docinput['body']."\n\n";
      }
      else {
        $message_body = '';
      }
      while (list ($docid, $value) = each ($children)) {
        $message_body .= transform($value['title'],'clean')."\n".
        transform(transform($value['body'], $value['datatype']['body']), 'html2txt')."\n\n";
      }
      $message_body .= "--------------------------------------------------------\n\n";
      $message_body .= LANG_EMAIL_SENT_BY.' '.email_to_string($docinput['sender'])."\n";
      $message_body .= LANG_ORIGINAL_DOC."\n".get_url($id, '', true)."\n";
      mail($docinput['email'], '['.SITE_TITLE.'] '.transform($document['title'],'clean'), $message_body,
        "From: ".email_to_string($docinput['sender']).' <'.$docinput['sender'].'>');
      ?>
        <p><?=LANG_DOC_SENT_TO?> <b><?=$docinput['email']?></b></p>
        <?
    }
  }
  $counter = 0;
  if(isset($children)) {
    reset($children);
    while (list ($docid, $value) = each ($children) and $counter < $max_newscount) {
      if($value['type_id']==$doctypes['angebot_para'] && (
        $value['expiration_date'] == 0 || $value['expiration_date'] > time())) {
        $counter++;
        if($counter > $first_item) {
          include(INC_PATH.'misc/angebot.inc');
        }
      }
    }
    if($total_count > $inc_value) {
      echo LANG_PAGE.' ';
      $temp_counter = $inc_value;
      while($inc_value + $total_count > $temp_counter) {
        $news_counter += 1;
        if($first_item == ($temp_counter - $inc_value)) { echo '<b>'; }
        echo '<a href="'.get_url($id,'first_item='.($temp_counter - $inc_value)).'">'.$news_counter.'</a> ';
        if($first_item == ($temp_counter - $inc_value)) { echo '</b>'; }
        $temp_counter += $inc_value;
        if($news_counter >= 30) { break; }
      }
      if($total_count > $first_item + $inc_value) {
        echo '&nbsp;<a href="'.get_url($id,'first_item='.($first_item + $inc_value)).'">'.LANG_NEXT.'</a>';
      }
      echo '<br /><br />';
    }
//        if(isset($parents)) {
//                echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_BACK.'</a> ] ');
//                }
    if(!isset($action) && $document['allow_forward'] == '1') {
      echo '[ <a href="'.get_url($id,'action=send_email').'">'.LANG_SEND_TO_A_FRIEND.'</a> ]';
    }
  }
  if ( $counter == 0 ) {
    echo("<p><b>".LANG_NO_CONTENT."</b></p>");
  }
  if( SESSION_STATUS == 'ok' && $profile['angebotsposter'] == '1') {
    echo ' [ <a href="'.get_url($id,'postangebot=1').'">'.LANG_POST_ANGEBOT.'</a> ]';
  }
}
elseif($profile['editor']=='1') {
  if(isset($children)) {
    reset($children);
    $counter = 0;
    while (list ($docid, $value) = each ($children) and $counter < $max_newscount) {
      if($value['type_id']==$doctypes['angebot_para']) {
        if($value['title'] == '') $value['title'] = 'Unknown';
        $counter++;
        if($counter > $first_item) {
          include(INC_PATH.'misc/angebot.inc');
        }
      }
    }
    if($total_count > $inc_value) {
      echo LANG_PAGE." ";
      $temp_counter = $inc_value;
      while($inc_value + $total_count > $temp_counter) {
        $news_counter += 1;
        if($first_item == ($temp_counter - $inc_value)) { echo '<b>'; }
        echo '<a href="'.get_url($id,'first_item='.($temp_counter - $inc_value)).'">'.$news_counter.'</a> ';
        if($first_item == ($temp_counter - $inc_value)) { echo '</b>'; }
        $temp_counter += $inc_value;
        if($news_counter >= 30) { break; }
      }
      if($total_count > $first_item + $inc_value) {
        echo '&nbsp;<a href="'.get_url($id,'first_item='.($first_item + $inc_value)).'">Next</a>';
      }
      echo '<br>';
    }
  }
  else {
    echo("<p><b>".LANG_NO_CONTENT."</b></p>");
  }
?>
<p>
<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr>
<td nowrap="nowrap"><? create_editor_input('User comments at bottom','comments','onoff_input'); ?></td>
<td nowrap="nowrap"><? create_editor_input('Show published files at bottom','show_files','onoff_input'); ?></td>
<td nowrap="nowrap"><? create_editor_input('Send to a friend','allow_forward','onoff_input'); ?></td>
</tr></table>

<?
echo '<br />';
// Image Input
create_editor_input('Image','image','image_input');
echo '<br /><br />';
// Text Input
create_editor_input('Summary','summary','area_input');
  }

?>