<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 03.06.2002
*/


if($value['title'] != '') {
  global $docid;
  if($profile['editor'] == '1') {
    echo '<a href="'.get_url($docid).'">'.transform($value['title'], $value['datatype']['title']).'</a><br />';
  }
  else
  {
    // handelt es sich um eine Kopie?
    if ( is_numeric($value["Kopie"]) )
    {
      $d = $value["Kopie"];
      $value = get_node($value["Kopie"], $flavour, false);
      $value["id"] = $d;
    }
    echo '<td><a href="'.get_url($value["id"]).'">'.transform($value['title'],
      $value['datatype']['title']).'</a> <small>';
    if ( $value['Link'] != "" )
      echo '(<a href="/redirect.php?url='.$value['Link'].'">Link</a>)';
    else {
      echo "(Datei ";
      $size = 0;
      $bbase = $value['basedir'];
      if ( $bbase == "" ) $bbase = get_dir_name('files',$value['id']).'/published/';
      $handle=opendir($bbase);
      while (false != ($file = readdir($handle))) {
        if(filetype($bbase."/".$file) == 'file') {
          $size += floor(filesize($bbase."/".$file) / 1024);
        }
      }
      closedir($handle);
      echo $size . " kB)";
    }
    echo '</small></td>';

//if($value['image'] != '' || $value['body'] != '') {
    echo '<td align="justify">'.transform($value['body'], $value['datatype']['body']);
    echo transform($value['image'], $value['datatype']['image']);
    if ( $value['body'] != "" ) echo "<br />";
    echo "<small>".transform($value['author_id'], 'user_id').', '.
      transform($value['timestamp'], 'datetime').'</small></td>';
  }
}
?>