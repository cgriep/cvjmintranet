<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 08.08.2003
        Christoph Griep  (cgriep@fricom.com)
        Farbe der &Uuml;berschrift aus $design['newsheader']
        Farbe der Linien aus $design['newsbordercolor']

*/

if($value['title'] != '' || $profile['editor'] == 1) {
  if($value[title] == '') { $value[title] = 'Not yet translated.'; }
  echo $design['table_begin'];
  echo '<tr bgcolor="'.$design['newsheader'].'"><th style="border-color:'.
    $design['newsbordercolor'].';border-style:solid;border-right-style:none;border-width:1pt" align="left"><b><a href="'.
    get_url($docid,'first_item='.$first_item).'">'.transform($value['title'],'clean').
    '</a></b></th>';
  echo '<th style="border-color:'.$design['newsbordercolor'].
    ';border-style:solid;border-left-style:none;border-width:1pt" align="right"><span class="newsDate">'.
    transform($value['timestamp'], 'date')."</span></th></tr>";
  $value['body'] = transform($value['body'], $value['datatype']['body']);
  //if(strlen($value['body']) > 200) {
  //  $value['body'] = substr($value['body'],0,200);
  //  $rpos = strrpos($value['body']," ");
  //  $value['body'] = substr($value['body'],0,$rpos).'...';
  //}
  echo('<tr><td style="border-color:'.$design['newsbordercolor'].';border-style:solid;border-bottom-style:none;border-width:1pt" colspan="2" >'.$value['body'].'
      <a href="'.get_url($docid,'first_item='.$first_item).'">'.LANG_MORE.'</a></td></tr>');
  if(COUNT_VIEWS == 1) {
    echo('<tr><td style="border-color:'.$design['newsbordercolor'].';border-style:solid;border-top-style:none;border-width:1pt" colspan="2" align="right" border="blr">( '.$value['views'].' '.LANG_VIEWS.' | '.count_children($doctypes[comment], $flavour,
          $docid).' '.LANG_COMMENTS.' )</td></tr>');
  }
  echo $design['table_end'];
}
?>