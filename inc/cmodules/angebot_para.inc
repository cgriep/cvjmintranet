<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.12.2001
*/

  if($profile['editor']!='1') {
    load_lang('Angebot');
    if ( isset($_REQUEST["delete"]) && $_REQUEST["delete"] == 1 && $profile['angebotsposter'] == 1 )
    {
      destroy_node($id);
      remove_profile('editor_'.$id);
      $id = $document['parent_id'];
      echo "<b>Das Dokument wurde gel&ouml;scht!</b><br />";
      echo "&Uuml;bergeordnetes Dokument:<br />";
      $document = get_node($id, $flavour);
    }
    if ( isset($_REQUEST["saveangebot"])) {
      update_nodedata($id, 'body', $docinput['body'], $flavour,'extrans');
      update_nodedata($id, 'title', $docinput['title'], $flavour);
      if ( $docinput['GueltigAb'] == "" || strtotime($docinput['GueltigAb']) == -1 )
        $docinput['GueltigAb'] = 0;
      else
        $docinput['GueltigAb'] = strtotime($docinput['GueltigAb']);
      update_nodedata($id, 'GueltigAb', $docinput['GueltigAb'], $flavour, 'datetime');
      if ( $docinput['expiration'] != "" && strtotime($docinput['expiration']) != -1 ) {
        sql_query("UPDATE ".TABLE_NODES." SET expiration_date = ".strtotime($docinput['expiration']).
          " WHERE id=$id");
      }
      echo "<b>&gt;&gt;&gt;&nbsp;&nbsp;".LANG_ANGEBOT_GESPEICHERT."</b><br />";
      $document = get_node($id, $flavour);
    }
    if ( isset($_REQUEST["edit"]) && $_REQUEST["edit"] == 1 && $profile['angebotsposter'] == 1 )
    {
      ?><form action="<?=get_url($id)?>" method="post"><?=LANG_TITLE?>
        <br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>"
        maxlength="60" value="<?=$document['title']?>">
        <br /><?=LANG_BODY?><br /><textarea name="docinput[body]" cols="<?=$design['input_size']?>"
        wrap="virtual" rows="<?=$design['input_rows']?>"><?=$document["body"]?></textarea>
        <br /><br /><?=LANG_GUELTIG_AB?> (JJJJ-MM-DD)<br />
        <input type="text" name="docinput[GueltigAb]"  size="<?=$design['input_size']?>"
        maxlength="20" value="<?=date("Y-m-d",$document["GueltigAb"])?>" />
        <br /><br /><?=LANG_GUELTIG_BIS?> (JJJJ-MM-DD)<br />
        <input type="text" name="docinput[expiration]"  size="<?=$design['input_size']?>"
        maxlength="20" value="<?=date("Y-m-d",$document["expiration_date"])?>" />
        <br /><br /><input type="submit" name="saveangebot" value="<?=LANG_SUBMIT?>"></form><?
    }
    else {
      $docid = $id;
      $value = $document;
      include(INC_PATH.'misc/angebot.inc');
    }
    if(isset($parents)) {
      echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_SHOW_COMPLETE_DOC.'</a> ]');
    }
    if ( ! isset($_REQUEST['delete']) ) {
      if ( $profile['angebotsposter'] == 1 ) {
        echo ' [ <a href="'.get_url($id,'edit=1').'">'.LANG_ANGEBOT_EDIT.'</a> ] &nbsp;&nbsp;';
        echo ' [ <a href="'.get_url($id,'delete=1').'">'.LANG_DELETE.'</a> ]  ';
      }
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
                create_editor_input('G&uuml;ltig ab','GueltigAb','text_input');
                }

?>