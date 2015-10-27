<?php

/*

This is a thumbnail content module for the AWF.
maintained by Christoph Stickel <mixer2@web.de>
last update 27.3.2002 by Christoph S.

*/

if(isset($basedir)) { unset($basedir); }

if(isset($document['basedir']) && $document['basedir'] != '') {
  // we can use this directory (default is public/)...
  $basedir = $document['basedir'];
}
else {
  // ...or we create our own one ;)
  mkdirs(get_dir_name('files',$id).'/published/thumbs', CONFIG_CREATE_MASK);
  $basedir = get_dir_name('files',$id).'/published/';
}
if (SESSION_STATUS == 'ok' && $profile[$document['pictureposter']] == '1') {
 if(isset($_FILES['userfile']['tmp_name']) &&
   $_FILES['userfile']['tmp_name'] != '' &&
   $_FILES['userfile']['tmp_name'] != 'none' &&
   !file_exists(BASE_PATH.$basedir.basename($_FILES['userfile']['name']))) {
   if(function_exists('move_uploaded_file')) {
     move_uploaded_file($_FILES['userfile']['tmp_name'],
       BASE_PATH.$basedir.basename($_FILES['userfile']['name']));
     chmod(BASE_PATH.$basedir.basename($_FILES['userfile']['name']), CONFIG_CREATE_MASK);
   }
   if ( ($_FILES['thumbfile']['tmp_name'] == '' ||
     $_FILES['thumbfile']['tmp_name'] == 'none') &&
     !file_exists(BASE_PATH.$basedir."thumbs/".basename(
     $_FILES['userfile']['name'])) &&
     function_exists('imagecreate') )
   {
     // thumb erstellen
     $types = array (1 => "gif", "jpeg", "png", "swf", "psd", "wbmp");
     $imagedata = getimagesize(BASE_PATH.$basedir.basename(
       $_FILES['userfile']['name']));
     $x = 100;
     $y = floor ($x * $imagedata[1] / $imagedata[0]);
     $image = call_user_func("imagecreatefrom".$types[$imagedata[2]],
       BASE_PATH.$basedir.basename($_FILES['userfile']['name']));
     //if ( function_exists('imagecreatetruecolor'))
     //  $thumb = imagecreatetruecolor ($x, $y);
     //else
       $thumb = imagecreate ($x, $y);
     if ( ! imagecopyresampled ($thumb, $image, 0, 0, 0, 0, $x, $y, $imagedata[0], $imagedata[1]))
       echo "Fehler beim resamplen";
     call_user_func("image".$types[$imagedata[2]], $thumb,
       BASE_PATH.$basedir."thumbs/".basename($_FILES['userfile']['name']));
     imagedestroy ($image);
     imagedestroy ($thumb);
     chmod(BASE_PATH.$basedir."thumbs/".basename($_FILES['userfile']['name']),
        CONFIG_CREATE_MASK);
   }
 }
 if(isset($_FILES['thumbfile']['tmp_name']) &&
          $_FILES['thumbfile']['tmp_name'] != '' &&
          $_FILES['thumbfile']['tmp_name'] != 'none' &&
          !file_exists(BASE_PATH.$basedir."thumbs/".basename(
          $_FILES['userfile']['name'])))
 // Thumbdatei wird gleich in die Bilddatei umbenannt
  if(function_exists('move_uploaded_file')) {
    move_uploaded_file($_FILES['thumbfile']['tmp_name'],
      BASE_PATH.$basedir."thumbs/".basename($_FILES['userfile']['name']));
    chmod(BASE_PATH.$basedir."thumbs/".basename($_FILES['userfile']['name']),
      CONFIG_CREATE_MASK);
  }

}
load_lang('thumbnail');
if($profile['editor']!='1') {
  if($_REQUEST['add'] == '1' && SESSION_STATUS == 'ok' &&
    $profile[$document['pictureposter']] == '1') {
    ?><form action="<?=get_url($id)?>" method="post" enctype="multipart/form-data">
        <b><?=LANG_PICTURE_FILE?></b><br /><input name="userfile" type="file"><br />
        <b><?=LANG_THUMB_FILE?></b><br /><input name="thumbfile" type="file">
        <br /><input type="submit" value="<?=LANG_SUBMIT?>"></form><?
  }
  else if(isset($_REQUEST['Del']) && SESSION_STATUS == 'ok' &&
    $profile[$document['pictureposter']] == '1') {
    // L&ouml;schen eines Bildes
    $handle = opendir(BASE_PATH.$basedir);
    $nr = 0;
    while ($file = readdir ($handle))  {
      if (eregi(".jpeg",substr($file,strlen($file)-5)) ||
          eregi(".jpg",substr($file,strlen($file)-4)) ||
          eregi(".gif",substr($file,strlen($file)-4)) ||
          eregi(".png",substr($file,strlen($file)-4))) {
        if ( $nr == $_REQUEST["Del"] ) {
          unlink(BASE_PATH.$basedir."/".$file);
          if ( file_exists(BASE_PATH.$basedir."/thumbs/".$file) )
            unlink(BASE_PATH.$basedir."/thumbs/".$file);;
          echo "<b>".$file." gel&ouml;scht.</b><br />";
        }
        $nr++;
      }
    }
    closedir($handle);
  }
  else if  (isset($_REQUEST["DelGal"]) && SESSION_STATUS == 'ok' &&
    $profile[$document['pictureposter']] == '1' )
  {
    // Gallerie l&ouml;schen
    destroy_node($id);
    remove_profile('editor_'.$id);
    $id = $document['parent_id'];
    echo "<b>Das Dokument wurde gel&ouml;scht!</b><br />";
    echo "&Uuml;bergeordnetes Dokument:<br />";
    $document = get_node($id, $flavour);
  }

  {
        ?><center><table width="100%" border="0" cellpadding="1" cellspacing="1"><?
        if(isset($document['title']) && $document['title'] != "") { ?><tr><td><h3><?=transform($document['title'], 'clean')?></h3></td></tr><? }
        if(isset($document['body']) && $document['body'] != "") { ?><tr><td><?=transform($document['body'], $document['datatype']['body'])?></td></tr><? }
        ?></table></center><?
        $handle = opendir(BASE_PATH.$basedir."/thumbs/");
        while ($file = readdir($handle)) { $existingth[] = $file; }
        closedir($handle);
        ?><p align="left"><table><?
        $handle = opendir(BASE_PATH.$basedir);
        $nr = 0;
        while ($file = readdir ($handle))  {
                if ( ($nr % 4) == 0 ) echo "<tr>";
                if (in_array($file, $existingth) && (
                  eregi(".jpeg",substr($file,strlen($file)-5)) ||
                  eregi(".jpg",substr($file,strlen($file)-4)) ||
                  eregi(".gif",substr($file,strlen($file)-4)) ||
                  eregi(".png",substr($file,strlen($file)-4)))) {
                        ?><td><a href="<?=$basedir.'/'.$file?>" target="_blank">
                        <img alt="<?
                        echo substr($file,0,-4). " - ";
                        echo round(@filesize("$basedir/$file")/1000). "kB";
                        ?>" src="<?=$basedir?>/thumbs/<?=$file?>" border=0></a><?
                        if ( $profile[$document["pictureposter"]] == "1" ) {
                          echo "<br />";
                          echo '<a href="?id='.$id.'&Del='.$nr.'">L&ouml;schen</a>';
                        }
                        echo "</td>";
                        $nr++;
                        }
                else // kein Thumbnail vorhanden
                  if (eregi(".jpeg",substr($file,strlen($file)-5)) ||
                    eregi(".jpg",substr($file,strlen($file)-4)) ||
                    eregi(".gif",substr($file,strlen($file)-4)) ||
                    eregi(".png",substr($file,strlen($file)-4))) {
                        ?><td><a href="<?=$basedir.'/'.$file?>" target="_blank">
                        <img width="150" height="100" alt="<?
                        echo substr($file,0,-4). " - ";
                        echo round(@filesize("$basedir/$file")/1000). "kB";
                        ?>" src="<?=$basedir?>/<?=$file?>" border="0" /></a><?
                        if ( $profile[$document["pictureposter"]] == "1" ) {
                          echo "<br />";
                          echo '<a href="?id='.$id.'&Del='.$nr.'">L&ouml;schen</a>';
                        }
                    echo "</td>";
                    $nr++;
                  }
                if ( ($nr % 4) == 0 ) echo "</tr>";
        }
        closedir($handle);
        if ( ($nr % 4) != 0 ) echo "</tr>";
        ?></table></p><?
  }
}
else {
        create_editor_input ('Description (optional)', 'body', 'area_input');
        ?><br /><br />
        <?
        /*
        $handle = opendir(BASE_PATH."/".$basedir);
        while ($dir = readdir ($handle)) {
                if ($dir != "." && $dir != "..")  {
                        if(is_dir("img/thumbnail/".$dir)) {
                                $done = "true";
                                if("img/thumbnail/".$dir."/" != $document['basedir']) {
                                        echo '<option value="img/thumbnail/'.$dir.'/">img/thumbnail/'.$dir.'</option>';
                                        }
                                else {
                                        echo '<option selected value="img/thumbnail/'.$dir.'/">img/thumbnail/'.$dir.'</option>';
                                        }
                                 }
                        }
                }
        echo        '</select>';
        if(!isset($done)) { echo '<br>sorry there is no directory in img/thumbnail/'; }
        */
        echo '<br /><br />';
        create_editor_input('User comments at bottom', 'comments', 'onoff_input');
        echo '<br /><br />';
        create_editor_input('Show published files at bottom','show_files','onoff_input');
        echo '<br /><br />';
        echo '<input type="hidden" name="docdataflavour[comments]" value="0" />';
        echo '<br /><br />';
        create_editor_input('Recht f&uuml;r neue Bilder', 'pictureposter', 'text_input');
        }
  $isgallery = get_node($document['parent_id'], $flavour);
  if(($isgallery['type_id'] == $doctypes['thumbnail_gallery']) && $isgallery != '') {
    ?><br><br>[ <a href="<?=$PHP_SELF?>?id=<?=$document['parent_id']?>"><?=LANG_BACKTOGALLERY?></a> ]<?
  }
  if($profile['editor'] != '1' && SESSION_STATUS == 'ok' && $profile[$document['pictureposter']] == '1') {
        echo ' [ <a href="'.get_url($id,'add=1').'">'.LANG_ADD_THUMB.'</a> ]  ';
        echo ' [ <a href="'.get_url($id,'DelGal=1').'">'.LANG_DEL_THUMB.'</a> ]  ';
  }
?>