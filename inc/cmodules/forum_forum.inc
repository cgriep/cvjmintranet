<?php

/*        This is the forum content module for the AWF.
        maintained by Christoph Stickel <mixer2@web.de>

        23.06.2002: Adapted to XHTML 1.0 [michael@liquidbytes.net]
*/

load_lang('forum');
$childcount = count_children($doctypes['forum_posting'], $flavour);

?>
<script language="javascript"><!--
                function emoticon(text) {
                  var txtarea = document.getElementById("newstext");
                  text = ' ' + text + ' ';
                  if (txtarea.createTextRange && txtarea.caretPos) {
                    var caretPos = txtarea.caretPos;
                   caretPos.text = caretPos.text.charAt(caretPos.text.length - 1) == ' ' ? text + ' ' : text;
                  txtarea.focus();
        } else {
                txtarea.value  += text;
                txtarea.focus();
        }
}
--></script><?

$filename = INC_PATH.DATA_PATH.'forum_buttonlist.conf';
$lines = file($filename);
while (list ($line_num, $line) = each ($lines)) {
        $buffer = split("\t", $line);
        $buffer2 = split(';', $buffer[1]);
         $buttonarray[$buffer[0]]['quote'] = '<img src = "'.$image_rpath.'buttons/forum/'.trim($buffer2[0]).'" />';
        $buttonarray[$buffer[0]]['reply'] = '<img src = "'.$image_rpath.'buttons/forum/'.trim($buffer2[1]).'" />';
        }

if(!isset($document['button']) || (!isset($buttonarray[$document['button']]['quote']) && $document['button'] != 'Text')) { $document['button'] = 'Icons only'; add_nodedata ($id, 'button', 'Icons only', 0); }
if(!isset($document['ppp']) || $document['ppp'] < 1) { $document['ppp'] = '20'; add_nodedata ($id, 'ppp', 20, 0); }
if(!isset($document['ppp2']) || $document['ppp2'] < 1) { $document['ppp2'] = '20'; add_nodedata ($id, 'ppp2', 20, 0); }
if(!isset($document['maxposts']) || $document['maxposts'] < 1) { $document['maxposts'] = '100'; add_nodedata ($id, 'maxposts',100,0); }
if(!isset($document['maxposts2']) || $document['maxposts2'] < 1) { $document['maxposts2'] = '100'; add_nodedata ($id, 'maxposts2',100,0); }
if(!isset($document['onlyforusers'])) { $document['onlyforusers'] = '0'; add_nodedata ($id, 'onlyforusers', 0, 0); }
if(!isset($document['marknew'])) { $document['marknew'] = '1'; add_nodedata ($id, 'marknew', 1, 0); }
if(!isset($_REQUEST['first_item']) || !is_numeric($_REQUEST['first_item'])) $_REQUEST['first_item'] = 0;
if(!isset($document['descorder'])) { $document['descorder'] = '0'; add_nodedata ($id, 'descorder', 0, 0); }
if(!isset($document['showhost'])) { $document['showhost'] = '1'; add_nodedata ($id, 'showhost', 1, 0); }


include(INC_PATH.'misc/forum_functions.inc');

if($profile['editor'] == 1) {
        create_editor_input('Description (optional)','descr', 'text_input');
        ?><br /><br />Buttons<br /><select size="1" name="docdata[button]"><option value="Text" <?
        if ($document['button'] == 'Text') { echo 'SELECTED'; }
        ?>>Text</option><?
        while(list($key, $value) = each($buttonarray)) {
                echo '<option value="'.$key.'" ';
                if ($document['button'] == $key) { echo 'SELECTED'; }
                echo '>'.$key.'</option>';
                }
        ?></select>
        <input type="hidden" name="docdataflavour[button]" value="0"><?
        echo ' Example: ';
        if($document['button'] == 'Text') { echo '[ Quote ] [ Reply ]'; }
        else {
                echo ' '.$buttonarray[$document['button']]['quote'].'&nbsp;&nbsp;&nbsp;'.$buttonarray[$document['button']]['reply'];
                }
        ?>
        <br /><h4>Counter</h4>
        Postings per page (forum)<br /><input type="text" name="docdata[ppp2]" size="2" maxlength="2" value="<?=$document['ppp2']?>">
        <br /><br />Max postings (forum)<br /><input type="text" name="docdata[maxposts2]" size="3" maxlength="3" value="<?=$document['maxposts2']?>"><br /><br />
        Replies per page (posting)<br /><input type="text" name="docdata[ppp]" size="2" maxlength="2" value="<?=$document['ppp']?>">
        <br /><br />Max replies (posting)<br /><input type="text" name="docdata[maxposts]" size="3" maxlength="3" value="<?=$document['maxposts']?>">
        <br /><br /><?
        create_editor_input('Only registered users are able to add a posting or reply to a posting.','onlyforusers','onoff_input');
        ?><br /><br /><?
        create_editor_input('Mark postings created in the last 24 hours.','marknew', 'onoff_input');
        ?><br /><br /><?
        create_editor_input('Sort replies descending.','descorder', 'onoff_input');
        ?><br /><br /><?
        create_editor_input('Show hosts from which on the replies where posted.','showhost', 'onoff_input');
        ?><input type="hidden" name="docdataflavour[ppp]" value="0">
        <input type="hidden" name="docdataflavour[ppp2]" value="0">
        <input type="hidden" name="docdataflavour[maxposts]" value="0">
        <input type="hidden" name="docdataflavour[maxposts2]" value="0">
        <input type="hidden" name="docdataflavour[onlyforusers]" value="0">
        <input type="hidden" name="docdataflavour[marknew]" value="0">
        <input type="hidden" name="docdataflavour[descorder]" value="0">
        <input type="hidden" name="docdataflavour[showhost]" value="0">

        <?
        if(isset($docinput['deleteposting'])) {
                foreach($docinput['deleteposting'] as $delkey) { destroy_node($delkey); }
                $childcount = $childcount-sizeof($docinput['deleteposting']);
                add_nodedata ($id, 'childcount', $childcount,$flavour);
                }

        if($docinput['deleteall'] == '1') {
                $result = get_children(0, $childcount, $doctypes['forum_posting'], 0, 'DESC');
                while(list($key, $value) = each($result)) { destroy_node($key); }
                $childcount = 0;
                add_nodedata ($id, 'childcount', 0, $flavour);
                }
        $result = get_sorted_children ($_REQUEST['first_item'] ,100, 'timestamp',$doctypes['forum_posting']);
        echo '<br /><br /><br />';
        if($childcount > 0) {
                echo $design['table_begin'];
                ?><tr bgcolor="<?=$design['table_header']?>">
                  <td width="79%">Subject</td>
                  <td width="10%" nowrap>Author</td>
                  <td width="10%" nowrap>Last Change</td>
                  <td width="1%" nowrap>Edit</td></tr><?
                }
        $colorswitch = '0';
        while(list($key, $value) = each($result)) {
                echo ' <tr bgcolor = "';
                $colorswitch = getswitchcolor();
                echo         '"><td width="79%"><b><a href="'.get_url($key).'">'.transform($value['title'], 'clean').'</a></b></td>
                        <td width="10%" nowrap>'.printnick($value).'</td>
                <td width="10%" nowrap align=right>'.transform($value['timestamp'], 'datetime').'</td>
                <td width="1%" nowrap> <input type="checkbox" name="docinput[deleteposting][]" value="'.$key.'">Remove</td></tr>';
                }
        if($childcount > 0) {
                echo '        <tr bgcolor = "'.$design['table_row_dark'].'"><td colspan="3">&nbsp;</td>
                        <td nowrap><input type="checkbox" name="docinput[deleteall]" value="1"> Remove All</td></tr>
                        <tr><td colspan="4">';
                if($childcount > 100)  {
                           echo 'Page: ';
                           $temp_counter = 100;
                           while(100 + $childcount > $temp_counter) {
                                  $topic_counter += 1;
                                echo '[ ';
                                  if($_REQUEST['first_item'] == ($temp_counter - 100)) { echo '<b>'; }
                                  echo '<a href="'.get_url($id,'first_item='.($temp_counter - 100)).'">'.((($topic_counter-1)*100)+1).'-'.($topic_counter*100).'</a>';
                                   if($_REQUEST['first_item'] == ($temp_counter - 100)) { echo '</b>'; }
                                    echo ' ] ';
                                $temp_counter += 100;
                                    if($topic_counter >= 30) { break; }
                                    }
                        }
                echo '</td></tr>';
                }
        if($childcount > 0) echo $design['table_end'];
        }
else {
        echo '<p>';
        if(isset($docinput['savetopic']) && ((isemail($docinput['email']) && $docinput['nick'] != '' && $document['onlyforusers'] != '1') || SESSION_STATUS == 'ok') && $docinput['body'] != '' && $docinput['title'] != '') {
                ++$childcount;
                $nodeid = create_node($id, $doctypes['forum_posting'], $childcount, 1, 0, '', $document['key_id'], 1, 1, 0, $document['members_only']);
                $correctadded = 1;
                add_nodedata($id, 'childcount', $childcount, $flavour);
                add_nodedata($nodeid,'timestamp',time(), $flavour);
                if(SESSION_STATUS == 'ok') {
                        add_nodedata($nodeid, 'userid',SESSION_DBID, $flavour);
                          }
                else {  add_nodedata($nodeid, 'nick',$docinput['nick'], $flavour);
                        add_nodedata($nodeid, 'email',$docinput['email'], $flavour);
                        }
                add_nodedata($nodeid, 'host',gethostbyaddr($_SERVER['REMOTE_ADDR']), $flavour);
                add_nodedata($nodeid, 'body', $docinput['body'], $flavour);
                add_nodedata($nodeid, 'title', $docinput['title'], $flavour);
                add_nodedata($nodeid, 'childcount', 0, $flavour);

                if(SESSION_STATUS == 'ok' && $docinput['sendreplys'] == '1') {
                         $nodeid2 = create_node($nodeid, $doctypes['forum_mailto'], 1, 1, 0, '',$document['key_id']);
                         add_nodedata($nodeid,'timestamp',time(), $flavour);
                         add_nodedata($nodeid2,'email_user_id', SESSION_DBID, $flavour);
                         add_nodedata($nodeid2,'title', 'mailto', $flavour);
                         }
                clear_cache($id);
                }

        elseif($_REQUEST['addtopic'] == '1'|| (isset($docinput['savetopic']) && $correctadded != 1)) {
                if(!isset($_REQUEST['addtopic'])) echo '<b>&gt;&gt;</b> <font color="#ff0000">'.LANG_FORUM_ALLREQUIRED.'</font>';
                if(DISABLE_EMAIL_CHECK != 1 && SESSION_STATUS != 'ok' && !isset($_REQUEST['addtopic'])) {
                        echo '<font color="#ff0000"> '.LANG_FORUM_EMAILVERIFIED.'</font>';
                        }
                if(!isset($_REQUEST['addtopic']))echo '<br /><br />';
                echo '<form action="'.get_url($id).'" method="post" name="news">
                <input type="hidden" name="id" value="'.$id.'">
                <input type="hidden" name="docinput[savetopic]" value="1">';
                if(SESSION_STATUS != 'ok') {
                        ?><br /><?=LANG_NICKNAME?><br /><input type="text" name="docinput[nick]" value="<?=$docinput['nick']?>" size="<?=$design['input_size']?>" maxlength="15"><?
                        }
                if(SESSION_STATUS != 'ok') {
                        ?><br /><?=LANG_EMAIL_ADDRESS?><br /><input type="text" name="docinput[email]" value="<?=$docinput['email']?>" size="<?=$design['input_size']?>" maxlength="60"><?
                        }
                ?><br /><?=LANG_FORUM_SUBJECT?><br /><input type="text" name="docinput[title]" value="<?=$docinput['title']?>" size="<?=$design['input_size']?>" maxlength="60">
                <br /><?=LANG_FORUM_MESSAGE?><br /><textarea id="newstext" name="docinput[body]" cols="<?=$design['input_size']?>" rows="<?=$design['input_rows']?>"><?=$docinput['body']?></textarea><?
                if(SESSION_STATUS == 'ok') {
                        echo '<br /><input type="checkbox" name="docinput[sendreplys]" value="1"';
                        if($docinput['sendreplys'] == '1') {
                                echo 'CHECKED';
                                }
                        echo '> '.LANG_FORUM_SENDBYEMAIL;
                        }

                ?><br /><br /><input type="submit" value="<?=LANG_FORUM_ADDPOSTING?>"></form>
                <?
                $filename = INC_PATH.DATA_PATH.'smilies.conf';
                $lines = file($filename);
                while (list ($line_num, $line) = each ($lines)) {
                  $buffer = explode(chr(124), $line);
                  if (trim($buffer[1]) != "" )
                    $replacearray[$buffer[0]] = '<img src = "'.$image_rpath.'smilies/'.
                      trim($buffer[1]).'" onClick="javascript:emoticon('."'".
                      trim($buffer[0])."'". ');">';
                }
                echo '<hr />';
                echo "<p>Folgende Smilies stehen zur Verf&uuml;gung (einfach auf die Bilder klicken oder die" .
                          " Zeichen eingeben, die Leser sehen dann das Bild). </p>";
                $anz = 0;
                foreach($replacearray as $key=>$value) {
                  $anz++;
                  echo $value . ' ' . $key . ' ';
                  if ($anz % 10 == 0) echo "<br />";
                }
                echo '<hr />';
                $corrected = '1';
                }
        if($_REQUEST['addtopic'] != '1' && $corrected != '1') {
                $ismoverview = get_node($document['parent_id'], $flavour);
                if($ismoverview['type_id'] == $doctypes['forum_overview']) {
                        echo '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="'.$design['table_cellspacing'].'" border="'.$design['table_border'].'">';
                        ?><tr><td align="left" valign="middle">
                        <? echo '<h1>'.transform($document['title'], 'clean').'</h1>'; ?>
                        </td><td align="right"><?
                         echo '<form action="'.$_SERVER['PHP_SELF'].'" method="post">'.LANG_FORUM_JUMPTO.' <select size="1" name="id">';
                        $jumpforums = get_children(0, count_children($doctypes['forum_forum'],$flavour,$document['parent_id']), $doctypes['forum_forum'], false,'DESC', false, $document['parent_id']);
                        while(list($key, $value) = each($jumpforums)) {
                                echo '<option value="'.$key.'" ';
                                if($key == $id) {
                                        echo 'SELECTED';
                                        }
                                echo '>'.transform($value['title'], 'clean').'</option>';
                                }
                        ?></select><input type="submit" value="<?=LANG_FORUM_GO?>" /></form><?
                        ?></td></tr></table><?
                        }
                echo $design['table_begin'];
                ?><tr bgcolor="<?=$design['table_header']?>">
                <td width="79%"><?=LANG_FORUM_SUBJECT?></td>
                <td width="10%" nowrap><?=LANG_FORUM_AUTHOR?></td>
                <td width="10%" nowrap><?=LANG_FORUM_REPLIES?></td>
                <td width="1%" nowrap><?=LANG_FORUM_LATEST?></td></tr><?
                $result = get_sorted_children ($_REQUEST['first_item'] , $document['ppp2'], 'timestamp', $doctypes['forum_posting']);
                $colorswitch = '0';
                while(list($key, $value) = each($result)) {
                        echo '<tr bgcolor = "';
                        $colorswitch = getswitchcolor();
                        echo '"><td width="79%"><b><a href="'.get_url($key).'">'.transform($value['title'], 'clean').'</a></b>';
                        // Neu: time()-86400 durch last_login ersetzt
                        $ll = 2147483647;
                        if ( defined(SESSION_LAST_LOGIN) ) {
                          $ll = SESSION_LAST_LOGIN;
                        }
                        else
                        $ll = time()-86400;
                        if($value['timestamp']>$ll && $document['marknew'] == 1) {
                                echo '<font color="#ff0000" size="-2"> '.LANG_FORUM_NEW.'</font>';
                                }
                        echo '</td><td width="10%" nowrap>'.printnick($value).'</td><td width="10%" nowrap><span style="text-align: center">';
                        if(!isset($value['childcount'])) {
                                $value['childcount'] = count_children($doctypes['forum_posting'], $flavour, $key);
                                add_nodedata($key,'childcount',$value['childcount'],$flavour);
                                }
                        echo $value['childcount'];
                        ?></span></td>
                        <td width="1%" nowrap align=right><?=transform($value['timestamp'], 'datetime')?></td></tr><?
                        }
                echo $design['table_end'];
                if($childcount > $document['ppp2']) {
                           echo LANG_FORUM_PAGE.' ';
                            $temp_counter = $document['ppp2'];
                           while($document['ppp2'] + $childcount > $temp_counter) {
                                     $topic_counter += 1;
                                     echo '[ ';
                                    if($_REQUEST['first_item'] == ($temp_counter - $document['ppp2'])) { echo '<b>'; }
                                   echo '<a href="'.get_url($id,'first_item='.($temp_counter - $document['ppp2'])).'">';
                                   echo ((($topic_counter-1)*$document['ppp2'])+1).'-'.($topic_counter*$document['ppp2']).'</a>';
                                      if($_REQUEST['first_item'] == ($temp_counter - $document['ppp2'])) { echo '</b>'; }
                                      echo ' ] ';
                                   $temp_counter += $document['ppp2'];
                                      if($topic_counter >= ($document['maxposts2']/$document['ppp2'])) { break; }
                                     }
                        }
                if($childcount < 1) {
                         echo '<i>'.LANG_NO_CONTENT.'</i><br />';
                         }
                echo '<p>';
                if($ismoverview['type_id'] == $doctypes['forum_overview']) {
                         echo '<p>[ <a href="'.get_url($document['parent_id']).'">'.LANG_BACK.'</a> ] ';
                         }
                if($document['onlyforusers'] == '0' || SESSION_STATUS == 'ok') {
                        echo '[ <a href="'.get_url($id,'addtopic=1').'">'.LANG_FORUM_ADDNEW.'</a> ]';
                         }
                else {
                         echo LANG_FORUM_HAVETOBELOGGEDIN;
                         }
                }
        }
?>