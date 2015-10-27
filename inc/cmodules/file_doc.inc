<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 03.06.2002

        Wichtige Berechtigungen:
        fileposter = 1   -- Dateien / Links k&ouml;nnen hochgeladen werden

*/
load_lang('file_doc');
if ( ! isset($document['backgroundcolor'])) $document['backgroundcolor'] = $design['table_header'];
if ( ! isset($document['bordercolor'])) $document['bordercolor'] = $design['table_border'];

if(SESSION_STATUS == 'ok' && $profile[$document['fileposter']] == '1') {
  if ( isset($_REQUEST["delete"]) && $_REQUEST["delete"] == 1 && $document['author_id'] == SESSION_DBID )
  {
    destroy_node($id);
    clear_cache($id);
    remove_profile('editor_'.$id);
    $id = $document['parent_id'];
    echo "<b>Das Dokument wurde gelöscht!</b><br />";
    echo "Übergeordnetes Dokument:<br />";
    $document = get_node($id, $flavour);
  }
  if ( $_REQUEST['savefile'] == '1') {
    if( $docinput['title'] == '') {
      ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_TITLE_AND_BODY_REQUIRED?></font><?
    }
    else {
      clear_cache($id);
      $q = sql_query("SELECT MAX(sort_order) FROM ".TABLE_NODES." WHERE parent_id=".$id);
      if ( ! $z = sql_fetch_row($q) )
        $maxsort = 10;
      else
        $maxsort = $z[0]+10;
      sql_free_result($q);
      $nodeid = create_node($id, $doctypes['file_para'],
                  $maxsort, 1, 0, "", $document['key_id'],
                  $document['searchable'], $document['cacheable'], 0, $document['members_only']);
      add_nodedata($nodeid,'timestamp',time(), $flavour);
      add_nodedata($nodeid, 'body', $docinput['body'], $flavour, 'extrans');
      add_nodedata($nodeid, 'title', $docinput['title'], $flavour, 'text');
      add_nodedata($nodeid, 'comments', $docinput['comments'], $flavour);
      if(!file_exists(get_dir_name('files',$nodeid).'/published')) {
        mkdirs(get_dir_name('files',$nodeid).'/published', CONFIG_CREATE_MASK);
      }
      $bbasedir = get_dir_name('files',$nodeid, true).'/published/';
      if(isset($_FILES['userfile']['tmp_name']) &&
                     $_FILES['userfile']['tmp_name'] != '' &&
                     $_FILES['userfile']['tmp_name'] != 'none' &&
                     !file_exists(BASE_PATH.$bbasedir.basename($_FILES['userfile']['name'])) &&
                     ($docinput['Link'] == "" )
       ) {
         $_FILES['userfile']['name'] = str_replace("+", "", $_FILES['userfile']['name']);
         if(function_exists('move_uploaded_file')) {
           move_uploaded_file($_FILES['userfile']['tmp_name'],
              BASE_PATH.$bbasedir.basename($_FILES['userfile']['name']));
           chmod(BASE_PATH.$bbasedir.basename($_FILES['userfile']['name']), CONFIG_CREATE_MASK);
           //echo "Datei ".BASE_PATH.$bbasedir.basename($_FILES['userfile']['name'])." hochgeladen.<br />";
         }
         add_nodedata($nodeid, 'show_files', 1, $flavour);
      }
      if ( $docinput['Link'] != "" )
      {
        add_nodedata($nodeid, 'Link', $docinput['Link'], $flavour);
      }
      add_nodedata($nodeid, 'author', SESSION_EMAIL,$flavour,'email');
      add_nodedata($nodeid, 'author_id', SESSION_DBID, $flavour, 'user_id');
      add_nodedata($nodeid, 'basedir', "", $flavour);
      //add_nodedata($nodeid, 'icon', $docinput['icon'], $flavour);
    }
  }
  elseif ( $_REQUEST['savesub'] == '1') {
    if($docinput['summary'] == '' || $docinput['title'] == '') {
      ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_TITLE_AND_BODY_REQUIRED?></font><?
    }
    else {
      clear_cache($id);
      $nodeid = create_node($id, $doctypes['file_doc'],
                  count_children($doctypes['file_doc']), 1, 1, "", $document['key_id'],
                  $document['searchable'], $document['cacheable'], 0, $document['members_only']);
      add_nodedata($nodeid,'timestamp',time(), $flavour);
      add_nodedata($nodeid, 'summary', $docinput['summary'], $flavour);
      add_nodedata($nodeid, 'fileposter', $document['fileposter'], $flavour);
      add_nodedata($nodeid, 'bordercolor', $document['bordercolor'], $flavour);
      add_nodedata($nodeid, 'backgroundcolor', $document['backgroundcolor'], $flavour);
      add_nodedata($nodeid, 'title', $docinput['title'], $flavour);
      add_nodedata($nodeid, 'comments', $docinput['comments'], $flavour);
      add_nodedata($nodeid, 'author', SESSION_EMAIL,$flavour,'email');
      add_nodedata($nodeid, 'author_id', SESSION_DBID, $flavour, 'user_id');
    }
  }
}
if($_REQUEST['postfile'] == '1' && SESSION_STATUS == 'ok' && $profile[$document['fileposter']] == '1') {
        ?><form action="<?=get_url($id)?>" method="post" enctype="multipart/form-data"><?=LANG_TITLE?>
        <input type="hidden" name="savefile" value="1">
        <br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>" maxlength="80" value="<?=$docinput['title']?>">
        <br /><?=LANG_BODY?><br /><textarea name="docinput[body]" cols="<?=$design['input_size']?>" wrap="virtual" rows="<?=$design['input_rows']?>"></textarea>
        <br /><?=LANG_COMMENTS?><br />
        <input type="radio" value="1" name="docinput[comments]" checked="checked" /> Ja&nbsp;&nbsp;
        <input type="radio" value="0" name="docinput[comments]" /> Nein
        <br /><b>Inhalt:</b> entweder<br />
        <b>Datei hochladen</b><br /><input name="userfile" type="file">
        <br />
        oder <b>Link auf Seite</b><br /><input name="docinput[Link]" type="text" size=60>
        <br /><input type="submit" value="<?=LANG_SUBMIT?>"></form><?
        }
else if($_REQUEST['postsub'] == '1' && SESSION_STATUS == 'ok' && $profile[$document['fileposter']] == '1') {
        ?><form action="<?=get_url($id)?>" method="post" ><?=LANG_TITLE?>
        <input type="hidden" name="savesub" value="1">
        <br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>" maxlength="80" value="<?=$docinput['title']?>">
        <br /><?=LANG_BODY?><br /><textarea name="docinput[summary]" cols="<?=$design['input_size']?>" wrap="virtual" rows="<?=$design['input_rows']?>"></textarea>
        <br />
        <br /><input type="submit" value="<?=LANG_SUBMIT?>"></form><?
        }
else {
  $children =  get_children (0, 100, $doctypes['file_para']);

$inc_value = 10;
if(!isset($first_item) || !$first_item) $first_item = 0;
$max_newscount = $first_item + $inc_value;

$total_count=0;

if(isset($children)) {
        $total_count = count_children($doctypes['file_para'], $flavour);
        }

if($profile['editor']!='1') {

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
  if ( $document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  if ( $document["summary"] != "" ||$document["image"] != "" ) {
    echo '<table width="100%"><tr><td>';
    echo transform($document['image'], $document['datatype']['image']);
    echo transform($document['summary'], $document['datatype']['summary']);
    echo "</td></tr></table>\n";
  }
  $uchildren =  get_children (0, 100, $doctypes['file_doc']);
  if(isset($uchildren)) {
    $ueberda = false;
    while (list ($docid, $value) = each ($uchildren) ) {
      if($value['type_id']==$doctypes['file_doc']) {
        if ( ! $ueberda ) {
          echo "<p><b><i>Unterkategorien:</i></b><br />";
          $ueberda = true;
        }
        echo '<a href="'.get_url($docid).'"><b>' .
          transform($value['title'],$value['datatype']['title']) .
          "</b></a>";
        $hinweis = " <small>(";
        $anz = count_children($doctypes['file_doc'], $flavour, $docid);
        if ( $anz > 1 ) $hinweis .= $anz." Kategorien, ";
        else if ($anz > 0) $hinweis .= $anz." Kategorie, ";
        $anz = count_children($doctypes['file_para'], $flavour, $docid);
        $hinweis .= $anz." Eintr&auml;ge";
        $hinweis .= ")</small>";
        echo $hinweis;
        echo "<br />";
      }
    }
    echo "</p><br />";
  }

if(isset($children)) {
        echo '<table width="100%" border="1" style="border-collapse:collapse;border-color: '.$document['bordercolor'].';">';
        echo '<tr style="background: '.$document['backgroundcolor'].';border: thin solid;border-bottom-width: 2pt; border-left-width: 1pt;' .
         'border-right-width: 1pt; border-top-width: 1pt; padding: 2pt"><th>Name</th><th>Beschreibung</th></tr>';
        reset($children);
        $counter = 0;
        while (list ($docid, $value) = each ($children) and $counter < $max_newscount) {
                if($value['type_id']==$doctypes['file_para']) {
                        $counter++;
                        if($counter > $first_item) {
                                echo '<tr style="border: thin solid;border-bottom-width: 2pt; border-left-width: 1pt;' .
         'border-right-width: 1pt; border-top-width: 1pt; padding: 2pt">';
                                include(INC_PATH.'misc/descfile.inc');
                                echo "</tr>";
                                }
                        }

        }
        echo "</table>";
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
else {
        echo("<p><b>".LANG_NO_CONTENT."</b></p>");
        }
}
elseif($profile['editor']=='1') {
  if(isset($children)) {
        reset($children);
        $counter = 0;
        while (list ($docid, $value) = each ($children) and $counter < $max_newscount) {
                if($value['type_id']==$doctypes['file_para']) {
                        if($value['title'] == '') $value['title'] = 'Unknown';
                                $counter++;
                                if($counter > $first_item) {
                                        include(INC_PATH.'misc/descfile.inc');
                                              }
                        }
                }
        if($total_count > $inc_value) {
           echo 'Page ';
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
<td nowrap="nowrap"><? create_editor_input('Send to a friend','allow_forward','onoff_input'); ?></td>
</tr></table>
<?
echo '<br />';
// Image Input
create_editor_input('Image','image','image_input');
echo '<br /><br />';
// Text Input
create_editor_input('Summary','summary','area_input');
echo '<br /><br />';
// Text Input
create_editor_input('Farbe Linien','bordercolor','text_input');
echo '<br /><br />';
// Text Input
create_editor_input('Backgroundfarbe Titelzeile','backgroundcolor','text_input');
echo '<br /><br />';
// Text Input
create_editor_input('Recht f&uuml;r neue Postings','fileposter','text_input');
echo '<br /><br />';
create_editor_input('User comments at bottom','comments','onoff_input');

}
  if($profile['editor'] != '1' && SESSION_STATUS == 'ok' && $profile[$document['fileposter']] == '1') {
    echo ' [ <a href="'.get_url($id,'postfile=1').'">'.LANG_POST_FILE.'</a> ]  ';
    echo ' [ <a href="'.get_url($id,'postsub=1').'">'.LANG_POST_SUB.'</a> ]  ';
    if ( ! isset($_REQUEST['delete']) && $document['author_id'] == SESSION_DBID && $total_count == 0 && count_children($doctypes['file_doc']) == 0) {
        echo ' [ <a href="'.get_url($id,'delete=1').'">'.LANG_DELETE.'</a> ]  ';
    }
  }
}
?>
