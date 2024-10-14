<?php
/*
        Copyright (C) 2000-2002 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 23.06.2002
*/

?>

<div class="header">
<div class="rechteSeite">
  <a href="http://www.berlin.de">
    <img style="background:white" src="img/Berlinlogo.png" title="Logo der Stadt Berlin" alt="Berlinlogo" />
  </a>
<?php if($profile['editmode'] === 0) { ?>
<a href="<?=get_url($id, array('editmode' => 1))?>"><img style="background:gray"
src="img/small_edit/editmode_on.gif" border="0" alt="Edit on" /></a>
<?php } elseif($profile['editmode'] !== 0 && $profile['editor'] == '1') { ?>
<a href="<?=get_url($id, array('editmode' => 0))?>">
<img src="img/small_edit/editmode_off.gif" border="0" alt="Edit off" /></a>
<?php
if($profile['simple_edit'] == 1) { ?>
<a href="<?=get_url($id, array('simple_edit' => 0))?>">
<img src="img/small_edit/advanced_edit.gif" border="0" alt="Advanced Edit" /></a>
<?php } else { ?>
<a href="<?=get_url($id, array('simple_edit' => 1))?>">
<img src="img/small_edit/simple_edit.gif" border="0" alt="Edit" /></a>
<?php }
} else { ?>&nbsp;<?php } ?>
</div>
<img class="linkeSeite" src="img/SuProLogo.png" title="Logo der Suchtprophylaxe" alt="SuProLogo"/>
<div>
  <span class="titleueberschrift">Suchtprophylaxe</span>
  <span class="titlezeile">in der Berliner Schule</span>
</div>
<span class="titleLocation">
<?php
        init_parents();
        if(isset($parents)) {
                reset($parents);
                while (list($docid, $value) = each ($parents)) {
                        $section_title = '<a href="'.get_url($docid).'">'.$value['title'].
                        '</a> &gt; '.$section_title;
                        }
                }
        echo replace_wildcards($section_title." ".$document['title'], $profile);
?>
</span>
</div>