<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 03.06.2002
*/


if($value['title'] != '') {
    global $docid;
    echo '<h2 clear="all"><a href="'.get_url($docid).'">'.transform($value['title'], $value['datatype']['title']).'</a></h2>';
//  Anchor removed: Looked quite bad on Mozilla (hover -> color of text changed).
//  echo '<h2 clear="all"><a name="'.$docid.'">'.transform($value['title'], $value['datatype']['title']).'</a></h2>';
}
if($value['image'] != '' || $value['summary'] != '') {
  echo '<p class="justify">';
  // Image
  echo transform($value['image'], $value['datatype']['image']);
  // Text
  echo transform($value['summary'], $value['datatype']['summary']).'</p><br clear="all" />';
}

// Die Eintr&auml;ge dieses Dokumentes anzeigen
  global $docid;
  $mchildren =  get_children (0, 100, -1, false, 'DESC', false, $docid);
  if(isset($mchildren)) {
    foreach ( $mchildren as $docid => $value)  {
      if ($value['expiration_date'] == 0 || $value['expiration_date'] > time())
        include(INC_PATH.'misc/angebot.inc');
    }
  }

?>