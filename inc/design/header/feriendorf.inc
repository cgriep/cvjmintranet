<?php
/*
        Copyright (C) 2003 Terra Nostra www.terra-nostra.de
        Design by J&ouml;rg Podlinski

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 14.09.2001
*/

?>

<table class="volleTabelle noprint">
<tr>
    <td width="170" height="70" bgcolor="#ffffff" valign="middle" rowspan="2" align="center" >
        <a href="http://www.cvjm-feriendorf.de/index.php">
        <img src="../img/design/feriendorf/logodorf_mini.gif" /></a>
        <div class="linkeSeite" width="20%">
<?php if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img src="../img/small_edit/editmode_on.gif" style="background:gray" /></a>
<?php } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>"><img src="../img/small_edit/editmode_off.gif" /></a>
<?php } ?>
</div>
     </td>

    <td height="22"  bgcolor="ffffff" valign="top" align="left" colspan="1"><img src="../img/design/feriendorf/banner_3.jpg" />
    </td>
    <td height="22"  bgcolor="ffffff" valign="middle" align="center" colspan="0" >
    <?php
         if ( $_SERVER["SERVER_NAME"] == "feriendorf.dnsalias.org" ||$_SERVER["SERVER_ADDR"] == "192.168.110.102" || $_SERVER["SERVER_ADDR"] == "192.168.100.102") echo '<strong><font color="red"><big><big>&nbsp;&nbsp; Willkommen im INTRANET</big></big></font></strong>';
    ?>
    </td>
    <td height="22" width="120" bgcolor="ffffff" valign="top" align="right" colspan="4" >
    <?php
         if(SESSION_STATUS == 'ok') { echo '<br><strong>'.SESSION_EMAIL.'</strong>&nbsp;&nbsp;'; }
    ?>
    </td>
</tr>

<tr>
<td>
<?php
/*  Schnipsel für Felder 1-5 mit Namen und Hintergrundbilddatei ../img/design/feriendorf/ebene1_th.gif bis   ../img/design/feriendorf/ebene5_th.gif und Sprungbefehl
*/

if ( !isset($flavour) ) $flavour = $default_flavour;
init_published_roots();
global $root_parent_id;
global $published_roots;
$ids = Array();
foreach ( $published_roots as $did => $value)  {
  $ids[] = $did;
  $titel[] = $value['title'];
}

/* Nun stehen in $ids[] die ID der obersten Ebene
abgerufen werden k&ouml;nnen sie mit $ids[0]...$ids[count($ids)-1]
Beispiel:
Code von oben, dann PHP ende: ?>
<a href="?id=<?=$ids[0]?>">...<a href="?id=<?=$ids[1]?>...
*/
?>

<a href="?id=<?=$ids[1]?>"><?=$titel[1]?></a>
<a href="?id=<?=$ids[2]?>"><?=$titel[2]?></a>
<a href="?id=<?=$ids[3]?>"><?=$titel[3]?></a>
<a href="?id=<?=$ids[4]?>"><?=$titel[4]?></a>
<a href="?id=<?=$ids[5]?>"><?=$titel[5]?></a>
</td>
<td colspan="2">
<div class="rechteSeite"><strong>
<?php
  init_parents();
  if(isset($parents)) {
    reset($parents);
    foreach ($parents as $docid => $value)  {
      $section_title = '<a href="'.get_url($docid).'">'.$value['title'].'</a> &gt; '.$section_title;
    }
  }
  echo replace_wildcards($section_title." ".$document['title'], $profile);
?>
</strong></div>
</td>
</tr>
</table>