<?php

/*
This is a gallary module for the awf. It's job is managing the thumbnail module and combining multiple to a Picturegallery.
There are some functions which should make using the thumbnail module easier.
author Christoph Stickel <mixer2@web.de>
last update 27.3.02

Wichtige Berechtigungen:
- pictureposter  -- darf Bilder hochladen


*/

load_lang("thumbnail");
if(!isset($document['ppp'])) { $document['ppp'] = 10; }

if($profile['editor'] == '1') {
    echo "<br /><br />";
    create_editor_input('Recht f&uuml;r neue Bilder','pictureposter','text_input');
    echo "<br /><br />";
    if($_REQUEST["add"] != '1') {
      create_editor_input('Beschreibung', 'body', 'area_input');
      echo "<br /><br />";
      create_editor_input('Galerien pro Seite', 'ppp', 'text_input');
      echo "<br /><br />";
      /*
      echo  '<table width="1" border="0" cellpadding="0" cellspacing="0">
             <tr><td colspan="2" nowrap>Headline (optional)</td></tr><tr>
             <td colspan="2" nowrap><input type=text name="docdata[headline]"
              size=30 value="'.$document['headline'].'"><br><br></td></tr>
             <tr><td colspan="2" nowrap>Description (optional)</td></tr><tr>
             <td colspan="2" nowrap><textarea name="docdata[descr]" cols="60"
              rows="5">'.$document['descr'].'</textarea><br><br></td></tr>
             <tr><td colspan="2" nowrap>Linkcount</td></tr>
             <tr><td nowrap>Max per Page<br><input type=text
              name="docdata[ppp]" size=2 maxlength=2 value="'.
              $document['ppp'].'"><br><br></td></tr><tr><td colspan="2"
              nowrap>Other options</td></tr>
             <tr><td  nowrap><input type=checkbox name="add" value="1">
             <?=LANG_NEW_THUMB?></td><td nowrap><input type=checkbox
              name="delall" value="1">Remove All</td></tr></table>';
            */
    }
    if(isset($_REQUEST["deletet"])) {
      foreach($_REQUEST["deletet"] as $delkey) {
        destroy_node($delkey);
      }
    }
    if($_REQUEST["delall"] == '1') {
      $result = get_children(0, count_children($doctypes['thumbnail_thumbnail']),
                   $doctypes['thumbnail_thumbnail'], 0, 'DESC');
      while(list($key, $value) = each($result)) { destroy_node($key); }
    }
}
else if ( $profile[$document['pictureposter']] == 1  ) {
  if ( $_REQUEST["add"] == 1 && ! isset($_REQUEST["Titel"]) )
  {
    // Add-Bildschirm anzeigen
    ?>
    <b>Neue Galerie hinzuf&uuml;gen</b><br />
    <form action="<?=get_url($id)?>" method="post"><?=LANG_TITLE?>
        <input type="hidden" name="add" value="1">
        <br /><input type="text" name="Titel"
        size="<?=$design['input_size']?>" maxlength="80"
        value="Galerie Nr. <?=count_children($doctypes['thumbnail_thumbnail'])+1?>">
        <br /><input type="submit" value="<?=LANG_SUBMIT?>"></form>
    <?php
  }
}
if ($profile['editor'] == 1 || (SESSION_STATUS == 'ok' &&
     $profile[$document['pictureposter']] == '1'))
  if($_REQUEST["add"] == '1' && isset($_REQUEST["Titel"]) ) {
    $nodeid = create_node($id, $doctypes['thumbnail_thumbnail'],
         count_children($doctypes['thumbnail_thumbnail'])+1, 1, 0, "",
           $document['key_id'], $document['searchable'],
           $document['cacheable'], 0, $document['members_only']);
    if ( $_REQUEST["Titel"] == "" ) $_REQUEST["Titel"] = "(unknown)";
    add_nodedata($nodeid, 'title', $_REQUEST["Titel"], $flavour);
    add_nodedata($nodeid, 'pictureposter', $document['pictureposter'], $flavour);
    //add_nodedata($nodeid, 'show_files', 1, $flavour);
    echo "<b>Neue Galerie ".$_REQUEST["Titel"]." wurde erzeugt.</b><br />";
    echo '[ <a href="'.$PHP_SELF.'?id='.$nodeid.'">'.
      LANG_CONF_NEW_THUMB.'</a> ] [ <a href="'.
      $PHP_SELF.'?id='.$id.'">'.LANG_BACK.'</a> ]<br><br>';
  }
if($_REQUEST["add"] != '1') {
  echo '<center><table width="100%" border="0" cellpadding="1" cellspacing="1">';
  if(isset($document['title']) && $document['title'] != "" &&
    $profile['editor'] != '1') {
    echo '<tr><td><h3>'.$document['title'].'</h3>';
  }
  if(isset($document['body']) && $document['body'] != "" &&
    $profile['editor'] != '1') {
    echo '<tr><td>'.nl2br($document['body']);
  }
  echo '<br><br></td></tr>';
  $repeater2 = 1;
  if(!isset($repeater)) {$repeater="1";}
  $result = get_children($repeater-1, $document['ppp'],
    $doctypes['thumbnail_thumbnail'], 1, 'DESC');
  while(list($key, $value) = each($result)) {
    echo '<tr><td nowrap><a href="'.$PHP_SELF.'?id='.$key.'">'.
      $value['title'].'</a></td>';
    if($profile['editor'] == '1') {
      echo '<td nowrap width=1%><input type=checkbox name="deletet[]"
        value="'.$key.'"> <?=LANG_DELETE?></td>';
    }
    else  {
      echo '</tr>';
    }
  }
  echo '<tr><td><br><br>';
  while($repeater2 < (count_children($doctypes['thumbnail_thumbnail'])+1)) {
    echo ' [ <a href="'.$PHP_SELF.'?repeater='.$repeater2.'&id='.$id.'">'.
      $repeater2.'-'.($repeater2+($document['ppp']-1)).'</a> ] ';
    $repeater2 = $repeater2+$document['ppp'];
  }
  echo '</td></tr></table></center>';
}
if($profile['editor'] != '1' && SESSION_STATUS == 'ok' &&
  $profile[$document['pictureposter']] == '1' && (! isset($_REQUEST["add"]) || isset($_REQUEST["Titel"]))) {
  echo ' [ <a href="'.get_url($id,'add=1').'">'.LANG_NEW_THUMB.'</a> ]  ';
}
?>