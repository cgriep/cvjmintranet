<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 16.05.2002
*/
if(SESSION_STATUS == 'ok' && $profile['newsposter'] == '1' && $_POST['savenews'] == '1') {
        if($docinput['body'] == '' || $docinput['title'] == '') {
                ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_TITLE_AND_BODY_REQUIRED?></font><?php
                }
        else {
                $nodeid = create_node($id, $doctypes['newsitem'], count_children($doctypes['newsitem']), 1,0, "", $document['key_id']);
                add_nodedata($nodeid,'timestamp',time(), $flavour);
                add_nodedata($nodeid, 'body', $docinput['body'], $flavour, 'extrans');
                add_nodedata($nodeid, 'title', $docinput['title'], $flavour);
                add_nodedata($nodeid, 'comments', $docinput['comments'], $flavour);
                add_nodedata($nodeid, 'show_files', $docinput['show_files'], $flavour);
                add_nodedata($nodeid, 'basedir', $docinput['directory'], $flavour);
                add_nodedata($nodeid, 'icon', $docinput['icon'], $flavour);
                add_nodedata($nodeid, 'author', SESSION_EMAIL,$flavour,'email');
                add_nodedata($nodeid, 'author_id', SESSION_DBID, $flavour, 'user_id');
                clear_cache($id);
                /* Neu: L&ouml;schen des News-Cache */
                @unlink(BASE_PATH.'cache/modules/news_'.md5(get_cmodule_id('news').$design['name'].$flavour));
                }
        }
if($_REQUEST['postnews'] == '1' && SESSION_STATUS == 'ok' && $profile['newsposter'] == '1') {
        ?><form action="<?=get_url($id)?>" method="post"><?=LANG_TITLE?>
        <input type="hidden" name="savenews" value="1">
        <br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>" maxlength="60" value="<?=$docinput['title']?>">
        <br /><?=LANG_BODY?><br /><textarea name="docinput[body]" cols="<?=$design['input_size']?>" wrap="virtual" rows="<?=$design['input_rows']?>"></textarea>
        <br /><?=LANG_COMMENTS?><br />
        <input type="radio" value="1" name="docinput[comments]" /> Enable&nbsp;&nbsp;
        <input type="radio" value="0" name="docinput[comments]" CHECKED /> Disable
        <br /><br /><?=LANG_SHOW_PUBLISHED_FILES?><br />
        <input type="radio" value="1" name="docinput[show_files]" /> Enable&nbsp;&nbsp;
        <input type="radio" value="0" name="docinput[show_files]" CHECKED /> Disable
        <br /><br /><?=LANG_DIRECTORY?><br /><input type="text" name="docinput[directory]"  size="<?=$design['input_size']?>" maxlength="60" value="" />
        <br /><br /><?=LANG_ICON?><br />
        <select name="docinput[icon]"><option value="">No Icon</option>
        <?php
        $handle=opendir($image_path.'news/');
        while (false!==($file = readdir($handle))) {
        if ($file != "." && $file != "..") {
                $icons[] = $file;
                }
        }
        natcasesort($icons);
        foreach ( $icons as $key => $file) {
                echo '<option value="'.$file.'">'.substr($file, 0, -4).'</option>';
                }?>
        </select>

        <br /><br /><input type="submit" value="<?=LANG_SUBMIT?>"></form><?php
        }
else {
$pchildren = get_children (0, 100, $doctypes['paragraph']);

$inc_value = 10;
if(!isset($first_item) || !$first_item) $first_item = 0;
$max_newscount         = $first_item + $inc_value;

$children = get_sorted_children ($first_item, $inc_value, 'timestamp', $doctypes['newsitem']);

$total_count = count_children($doctypes['newsitem'], $flavour);

if($profile['editor']!='1') {

if(isset($pchildren)) {
        reset($pchildren);
        while (list ($docid, $value) = each ($pchildren)) {
                if($value['title'] != '' || $value['body']) {
                        include(INC_PATH.'misc/paragraph.inc');
                        }
                }
        }

if(isset($children)) {
        reset($children);
        foreach ($children as $docid => $value) {
          if ( $document['Kasten'] == 1 )
            include(INC_PATH.'misc/tablenewsitem.inc');
          else
            include(INC_PATH.'misc/newsitem.inc');
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
           echo '<br>';
           }
        printf('<p>'.LANG_THERE_ARE_X_NEWS.'</p>', $total_count);
        }
        }
elseif($profile['editor']=='1') {
  if(isset($pchildren)) {
    reset($pchildren);
    while (list ($docid, $value) = each ($pchildren)) {
      if($value['title'] != '') {
        include(INC_PATH.'misc/paragraph.inc');
      }
    }
  }
  if(isset($children)) {
    reset($children);
    $counter = 0;
    foreach ($children as $docid => $value) {
      include(INC_PATH.'misc/newsitem.inc');
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
    printf('<p>'.LANG_THERE_ARE_X_NEWS.'</p>', $total_count);
  }
  echo "<br /><br />";
  create_editor_input('News als Kasten','Kasten','onoff_input');
}
else {
        echo("<b>Sorry, there's no content in your flavour yet.</b>");
        }

}
if($profile['editor'] != '1' && SESSION_STATUS == 'ok' && $profile['newsposter'] == '1') {
        echo ' [ <a href="'.get_url($id,'postnews=1').'">'.LANG_POST_NEWS.'</a> ]';
        }


?>