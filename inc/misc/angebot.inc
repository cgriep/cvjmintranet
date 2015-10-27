<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 03.06.2002
*/


if($value['title'] != '') {
  if($profile['editor'] == '1') {
    global $docid;
    echo '<h3 clear="all"><a href="'.get_url($docid).'">'.transform($value['title'], $value['datatype']['title']).'</a></h3>';
  }
  else {
    echo $design['table_begin'];
    echo '<tr bgcolor="'.$design['angebotsheader'].'"><th style="border-color:'.
    $design['angebotsbordercolor'].';border-style:solid;border-right-style:none;border-width:1pt" align="left"><b><a href="'.
    get_url($docid,'first_item='.$first_item).'">'.transform($value['title'],$value['datatype']['title']).
      '</a></b></th>';
    echo '<th style="border-color:'.$design['angebotsbordercolor'].
      ';border-style:solid;border-left-style:none;border-width:1pt" align="right"><span class="newsDate">'.
      transform($value['timestamp'], 'date')."</span></th></tr>";
    //$value['body'] = transform($value['body'], $value['datatype']['body']);
    if ( $value['GueltigAb'] != 0 || $value['expiration_date'] != 0 )
    {
      echo '<tr><td style="border-color:'.$design['angebotsbordercolor'].
        ';border-style:solid;border-right-style:none;border-width:1pt">';
      if ( $value['GueltigAb'] != 0 ) {
        $dd = transform($value['GueltigAb'], 'datetime');
        if ( substr($dd, strlen($dd)-5) == "00:00" )
          $dd = substr($dd,0,strlen($dd)-6);
       echo 'G&uuml;ltig von ' . $dd;
      }
      else echo "&nbsp;";
      echo '</td><td style="border-color:'.$design['angebotsbordercolor'].
        ';border-style:solid;border-left-style:none;border-width:1pt" align="right">';
      if ( $value['expiration_date'] != 0 ) {
       $dd = transform($value['expiration_date'], 'datetime');
       if ( substr($dd, strlen($dd)-5) == "00:00" )
         $dd = substr($dd,0,strlen($dd)-6);
       echo 'G&uuml;ltig bis ' . $dd;
      }
      else echo "&nbsp;";
      echo "</td></tr>";
    }
    if($value['image'] != '' || $value['body'] != '') {
      echo '<tr><td colspan="2" style="border-color:'.$design['angebotsbordercolor'].
      ';border-style:solid;border-width:1pt">';
      // Image
      echo transform($value['image'], $value['datatype']['image']);
      // Text
      echo url_to_link(transform($value['body'], $value['datatype']['body'])).'</p><br clear="all" />';
      echo '</td></tr>';
    }
    echo $design['table_end'];
  }
}
?>