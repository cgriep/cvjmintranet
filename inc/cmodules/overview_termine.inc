<?php

/* &Uuml;berblick: Veranstaltungen */

$children =  get_children (0, 100, -1, false, 'DESC', true);

if($profile['editor']!='1') {

        if($document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
        if($document['body'] != '') echo '<p class="justify">'.transform($document['image'],$document['datatype']['image']).transform($document['body'],$document['datatype']['body']).'</p>';

        }

elseif($profile['editor']=='1') {
?>

Redirection (optional)<br />
<select name="docdata[redirect]">
<option value="">Disabled</option>
        <?
        if(isset($children)) {
        reset($children);
        while (list ($docid, $value) = each ($children)) {
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
<?
create_editor_input('Image', 'image', 'image_input');
echo '<br /><br />';
create_editor_input('Body', 'body', 'area_input');

}

if(isset($children)) {
        reset($children);
        while (list ($docid, $value) = each ($children)) {
                if($value['title'] != '') {
                        include(INC_PATH.'misc/topic.inc');
                        }
                }

        }

else {
        echo("<p><b>".LANG_NO_CONTENT."</b></p>");
        }


?>