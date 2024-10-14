<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.08.2003 Christoph Griep (cgriep@fricom.com)
*/

if ($profile['editor'] != "1" ) {
  load_lang('Angebot');
  if(SESSION_STATUS == 'ok' && $profile['angebotsheaderposter'] == '1' && $_POST['saveangebot'] == '1') {
    if($docinput['body'] == '' || $docinput['title'] == '') {
      ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_TITLE_AND_BODY_REQUIRED?></font><?php
    }
    else {
      $nodeid = create_node($id, $doctypes['angebot_doc'], count_children($doctypes['angebot_doc']), 1,0, "", $document['key_id']);
      add_nodedata($nodeid,'timestamp',time(), $flavour);
      add_nodedata($nodeid, 'summary', $docinput['body'], $flavour,'extrans');
      add_nodedata($nodeid, 'title', $docinput['title'], $flavour);
      add_nodedata($nodeid, 'comments', $docinput['comments'], $flavour);
      add_nodedata($nodeid, 'show_files', $docinput['show_files'], $flavour);
      add_nodedata($nodeid, 'basedir', $docinput['directory'], $flavour);
      add_nodedata($nodeid, 'icon', $docinput['icon'], $flavour);
      add_nodedata($nodeid, 'author', SESSION_EMAIL,$flavour,'email');
      add_nodedata($nodeid, 'author_id', SESSION_DBID, $flavour, 'user_id');
      clear_cache($id);
      echo LANG_ANGEBOT_GESPEICHERT."<br />";
    }
  }
  if($_REQUEST['postangebot'] == '1' && SESSION_STATUS == 'ok' && $profile['angebotsheaderposter'] == '1') {
        ?><form action="<?=get_url($id)?>" method="post"><?=LANG_TITLE?>
        <input type="hidden" name="saveangebot" value="1">
        <br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>" maxlength="60" value="<?=$docinput['title']?>">
        <br /><?=LANG_BODY?><br /><textarea name="docinput[body]" cols="<?=$design['input_size']?>" wrap="virtual" rows="<?=$design['input_rows']?>"></textarea>
        <br /><?=LANG_COMMENTS?><br />
        <input type="radio" value="1" name="docinput[comments]" /> Enable&nbsp;&nbsp;
        <input type="radio" value="0" name="docinput[comments]" CHECKED /> Disable
        <br /><br /><?=LANG_SHOW_PUBLISHED_FILES?><br />
        <input type="radio" value="1" name="docinput[show_files]" /> Enable&nbsp;&nbsp;
        <input type="radio" value="0" name="docinput[show_files]" CHECKED /> Disable
        <br /><br /><?=LANG_DIRECTORY?><br /><input type="text" name="docinput[directory]"  size="<?=$design['input_size']?>" maxlength="60" value="" />
        <br /><br /><input type="submit" value="<?=LANG_SUBMIT?>"></form><?php
  }
}
$children =  get_children (0, 100, -1, false, 'DESC', false);
if($profile['editor']!='1') {

        if($document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
        if($document['body'] != '') echo '<p class="justify">'.transform($document['image'],$document['datatype']['image']).transform($document['body'],$document['datatype']['body']).'</p>';

        }

elseif($profile['editor']=='1') {
?>

Redirection (optional)<br />
<select name="docdata[redirect]">
<option value="">Disabled</option>
        <?php
        if(isset($children)) {
        reset($children);
        foreach ($children as $docid => $value) {
                $value['title'] = htmlspecialchars(strip_tags($value['title']));
                if(strlen($value['title']) > 30) { $value['title'] = substr($value['title'], 0, 27).'...'; }
                if($document['redirect'] == get_url($docid)) { $sel = ' selected'; } else { $sel = ''; }
                echo '<option value="'.get_url($docid).'"'.$sel.'>'.$value['title'].'</option>';
                }
        }
        ?>
        </select>
        <input type="hidden" name="docdataflavour[redirect]" value="0" />
<br />
<br />
<?php
create_editor_input('Image', 'image', 'image_input');
echo '<br /><br />';
create_editor_input('Body', 'body', 'area_input');
}

if(isset($children)) {
  reset($children);
  foreach ($children as $docid => $value) {
    if ( $value['title'] != '') {
      include(INC_PATH.'misc/angebotkategorie.inc');
    }
  }
}
else {
  echo("<p><b>".LANG_NO_CONTENT."</b></p>");
}

if( $profile['editor'] != "1" && SESSION_STATUS == 'ok' && $profile['angebotsposter'] == '1') {
    echo ' [ <a href="'.get_url($id,'postangebot=1').'">'.LANG_POST_ANGEBOTSHEADER.'</a> ]';
  }

?>