<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$comments_per_page = 5;
if(!isset($_REQUEST['comment_item'])) { $_REQUEST['comment_item'] = 0; }

 echo '<p>';
 if(SESSION_STATUS == 'ok' && isset($docinput['remove_comment'])) {
  $del_comment = get_nodedata($docinput['remove_comment'], $flavour);
  if($del_comment['author_id'] == SESSION_DBID || $profile[editor] == 1) destroy_node($docinput['remove_comment']);
  }
 if(SESSION_STATUS == 'ok' && isset($docinput['save_comment']) && $docinput['title'] != '') {
  $msgid = create_node ($id, $doctypes['comment'], 0, 1, 0, '', $document['key_id'], 1, 1, 0, $document['members_only']);
  add_nodedata ($msgid, 'title', $docinput['title'], $flavour, 'clean');
  add_nodedata ($msgid, 'body', $docinput['body'], $flavour, 'usertext');
  add_nodedata ($msgid, 'author_id', SESSION_DBID, $flavour, 'id');
  add_nodedata ($msgid, 'author', SESSION_EMAIL, $flavour, 'email');
  add_nodedata ($msgid, 'timestamp', time(), $flavour, 'date');
  }
 echo '<b>'.LANG_COMMENTS.'</b><br />';
 $msgchilds = get_sorted_children ($_REQUEST['comment_item'], $comments_per_page, 'timestamp', $doctypes['comment']);
 if(isset($msgchilds) && is_array($msgchilds) && count($msgchilds) >= 1) {
  reset($msgchilds);
  $noflavcomments = true;
  while (list ($docid, $value) = each ($msgchilds)) {
   if($value['body'] && $value['title']) {
   $noflavcomments = false;
   if(strlen($value['body'])>200) {
    $value['body'] = substr($value['body'],0,200);
    $rpos = strrpos($value['body']," ");
    $value['body'] = substr($value['body'],0,$rpos)."...";
    }
   echo $design['table_begin'].'<tr><td align="left" bgcolor="'.$design['table_row_bright'].'">';
   printf(LANG_PUBLISHED_BY, transform($value['author_id'], 'user_id'), transform($value['timestamp'], 'date'));
   if($value['author_id']==SESSION_DBID || $profile['editor'] == 1) {
	echo '&nbsp; [ <a href="'.$self.'docinput[remove_comment]='.$docid.'">'.LANG_REMOVE.'</a> ]<br>';
	}
   else { echo '<br>'; }
   echo '<b><a href="'.get_url($docid).'">'.transform($value['title'],'clean').'</a></b><br>';
   echo transform($value['body'],'clean').'</td></tr>'.$design['table_end'].'</p><p>';
   }
   }
  $children_count = count_children($doctypes['comment'], $flavour);
  if($children_count > $comments_per_page) {
   echo 'Page ';
   $temp_counter = $comments_per_page;
   while($comments_per_page + $children_count > $temp_counter) {
    $comment_counter += 1;
    if($comment_item == ($temp_counter - $comments_per_page)) { echo '<b>'; }
    echo '<a href="'.get_url($id,'comment_item='.($temp_counter - $comments_per_page)).'">'.$comment_counter.'</a> ';
    if($comment_item == ($temp_counter - $comments_per_page)) { echo '</b>'; }
    $temp_counter += $comments_per_page;
    }
   if($children_count > $comment_item + $comments_per_page) {
    echo '&nbsp;<a href="'.get_url($id,'comment_item='.($comment_item + $comments_per_page)).'">Next</a>';
    }
   echo '<br><br>';
   }
  }
 else {
  echo '<br />'.LANG_NO_COMMENTS.'<br /><br />';
  }
 if($noflavcomments == true) {
  echo '<br />'.LANG_NO_COMMENTS.'<br /><br />';
  }
 if(SESSION_STATUS == 'ok' && $profile[editor] != 1) {
  echo '[ <a href="'.$self.'add_comment=true">'.LANG_ADD_COMMENT.'</a> ]';
  }
 elseif($profile[editor] == 0) {
  echo LANG_TO_ADD_A_COMMENT;
  }
 echo '</p>';
?>