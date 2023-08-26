<?php

/*
        This is the forum posting content module for the AWF. It's a childmodule for the forum content module.
        maintained by Christoph Stickel <mixer2@web.de>

        23.06.2002: Improved email sending feature (now uses get_url($id, '', true))

*/

load_lang('forum');
$filename = INC_PATH.DATA_PATH.'smilies.conf';
$lines = file($filename);
foreach ($lines as $line_num => $line) {
        $buffer =  explode('\|', $line);
         if (trim($buffer[1]) != "" )
                    $replacearray[$buffer[0]] = '<img src = "'.$image_rpath.'smilies/'.
                      trim($buffer[1]).'" onClick="javascript:emoticon('."'".
                      trim($buffer[0])."'". ');">';
        }
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
--></script><?php

$filename = INC_PATH.DATA_PATH.'forum_buttonlist.conf';
$lines = file($filename);
foreach ($lines as $line_num => $line) {
        $buffer =  explode("\t", $line);
        $buffer2 =  explode(';', $buffer[1]);
         $buttonarray[$buffer[0]]['quote'] = '<img src = "'.$image_rpath.'buttons/forum/'.trim($buffer2[0]).'" border="0" alt="'.LANG_FORUM_QUOTE.'">';
        $buttonarray[$buffer[0]]['reply'] = '<img src = "'.$image_rpath.'buttons/forum/'.trim($buffer2[1]).'" border="0" alt="'.LANG_FORUM_REPLY.'">';
        }

$parentdata = get_nodedata($document['parent_id'], $flavour);
$parentnode = get_node($document['parent_id'],$flavour);

if($parentdata['descorder']==1) { $sortorder='DESC'; }
else { $sortorder='ASC'; }

$childcount['forum_posting'] = count_children($doctypes['forum_posting'],$flavour);
$childcount['forum_mailto'] = count_children($doctypes['forum_mailto'],$flavour);

include(INC_PATH.'misc/forum_functions.inc');

if(!isset($_REQUEST['first_item']) || !is_numeric($_REQUEST['first_item'])) $_REQUEST['first_item'] = 0;
if(!isset($document['body'])) { $document['body'] = $document['message']; }
if($document['body'] == '') { $document['body'] = '[empty message body]'; }

if(isset($docinput['updatenode']) && SESSION_DBID == $document['userid'] && $docinput['body'] != '' && $docinput['title'] != '') {
        update_nodedata ($id, 'body', $docinput['body'], $flavour, 'usertext');
        update_nodedata ($id, 'title', $docinput['title'], $flavour, 'usertext');
        if($docinput['timestamprefresh'] == 1) update_nodedata ($id, 'timestamp', time(), $flavour);
        if($parentnode['type_id'] == $doctypes['forum_posting']) {
                update_nodedata ($document['parent_id'], 'timestamp', time(), $flavour);
                }
        ?><b>&gt;&gt;</b> <font color="#00aa00"><?=LANG_FORUM_CHANGEDSUCC?></font><?php
        }
elseif(isset($docinput['updatenode']) && ($docinput['title'] == '' || $docinput['body'] == '')) {
        ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_FORUM_CHANGEDFAIL?></font><?php
        }

if($profile['editor'] == 1) {
        if(isset($docinput['deleteposting'])) {
                 foreach($docinput['deleteposting'] as $delkey) { destroy_node($delkey); }
                $childcount['forum_posting'] = $childcount['forum_posting']-sizeof($docinput['deleteposting']);
                add_nodedata ($id, 'childcount', $childcount['forum_posting'],$flavour);
                }
        if($docinput['deleteall'] == "1") {
                $result = get_children(0, $childcount['forum_posting'], $doctypes['forum_posting'], 0, 'DESC');
                while(list($key, $value) = each($result)) { destroy_node($key);        }
                $childcount['forum_posting'] = 0;
                add_nodedata ($id, 'childcount', 0, $flavour);
                }
        ?><input type="hidden" name="id" value="'.$id.'">
        Nickname: <?php
        if(isset($document['nick'])) { echo transform($document['nick'], 'clean'); }
        else { echo transform(get_user_nickname($document['userid']), 'clean'); }
        ?>, Email: <?php
        if(isset($document['userid'])) { echo transform(get_user_email($document['userid']),'clean'); }
        else { echo transform($document['email'], 'clean'); }
        echo '<br /><br />';
        create_editor_input ('Message', 'body', 'area_input');
        echo '<br />';
        $result = get_sorted_children ($_REQUEST['first_item'] ,100,'timestamp',$doctypes['forum_posting'],-1,$sortorder);
        echo '<br />';
        if($childcount['forum_posting'] > 0)echo $design['table_begin'];
        $colorswitch = '0';
        if($parentnode['type_id'] == $doctypes['forum_forum'] && $childcount['forum_posting'] > 0) {
                ?><tr bgcolor = "<?=$design['table_header']?>">
                <td width="79%">Subject</td>
                <td width="10%" nowrap>Author</td>
                <td width="10%" nowrap>Posted at</td>
                <td width="1%" nowrap>Edit</td></tr><?php
                }
        while(list($key, $value) = each($result)) {
                echo '<tr bgcolor="';
                $colorswitch = getswitchcolor();
                ?>"><td nowrap><b><a href="<?=get_url($key)?>"><?=transform(substr($value['title'],0,30), 'clean')?><?php
                if(strlen($value['title'])>30){echo '...';}
                ?></b></a></td><td nowrap><?=printnick($value)?>
                <?=fhostadmin($value['host'])?></td>
                <td nowrap align=right><?=transform($value['timestamp'], 'datetime')?></td>
                <td nowrap><input type="checkbox" name="docinput[deleteposting][]" value="<?=$key?>"> Remove</td></tr><?php
                }
        if($childcount['forum_posting'] > 0) {
                ?><tr bgcolor = "<?=$design['table_row_dark']?>">
                <td colspan="3">&nbsp;</td>
                <td nowrap><input type="checkbox" name="docinput[deleteall]" value="1"> Remove All</td></tr><?php
                }
        if($childcount['forum_posting'] > 0)echo $design['table_end'];
        if($childcount['forum_posting'] > 100)  {
                echo 'Page: ';
                   $temp_counter = 100;
                   while(100 + $childcount['forum_posting'] > $temp_counter) {
                          $posting_counter += 1;
                        echo '[ ';
                          if($_REQUEST['first_item'] == ($temp_counter - 100)) { echo '<b>'; }
                          echo '<a href="'.get_url($id,'first_item='.($temp_counter - 100)).'">'.((($posting_counter-1)*100)+1).'-'.($posting_counter*100).'</a>';
                           if($_REQUEST['first_item'] == ($temp_counter - 100)) { echo '</b>'; }
                            echo ' ] ';
                        $temp_counter += 100;
                            if($posting_counter >= 30) { break; }
                        }
                }
        }
else {
        if($_REQUEST['dsendperemail']=='1') { delsendpermail(SESSION_DBID);        }
        if($_REQUEST['sendcomplete'] == '1' && SESSION_STATUS == 'ok') { sendcomplete(SESSION_DBID, $id); ?><b>&gt;&gt;</b> <font color="#00aa00"><?=LANG_FORUM_SENT?></font><?php }
        if(isset($docinput['saveanswer']) && ((isemail($docinput['email']) && $docinput['nick'] != '' && $parentdata['onlyforusers'] == '0') || SESSION_STATUS == 'ok') && $docinput['body'] != '' && $docinput['title'] != '') {
                $result=get_children(0,$childcount['forum_mailto'],$doctypes['forum_mailto']);
                while(list($key, $value) = each($result)) {
                        if(SESSION_STATUS == 'ok') {
                                $docinput['nick'] = get_user_nickname(SESSION_DBID);
                                $docinput['email'] = get_user_email(SESSION_DBID);
                                }
                        $user = get_user_email($value['email_user_id']);
                        $message_body = transform(time(), 'date')."\n\n";
                               $message_body .= transform($docinput['title'],'clean')."\n\n";
                         $message_body .= transform(transform($docinput['body'], 'usertext'),'html2txt')."\n\n";
                         $message_body .= "--------------------------------------------------------\n\n";
                         $message_body .= LANG_EMAIL_SENT_BY.' '.email_to_string($docinput['email'])."\n";
                               $message_body .= LANG_ORIGINAL_DOC."\n".get_url($id,'',true)."\n";
                             mail($user, '['.SITE_TITLE.'] '.transform($docinput['title'],'clean'), stripslashes($message_body), "From: ".email_to_string($docinput['email']).' <'.$docinput['email'].'>');
                             }

                if($docinput['sendreplys']=='1' && SESSION_STATUS == 'ok') {
                        if(issendpermail(SESSION_DBID) != '1') {
                        $nodeid = create_node($id, $doctypes['forum_mailto'], $childcount['forum_mailto']+1, 1, 0, '', $document['key_id']);
                                ++$count_children['forum_mailto'];
                                add_nodedata($nodeid,'timestamp',time(), $flavour);
                                add_nodedata($nodeid,'email_user_id', SESSION_DBID, $flavour);
                                add_nodedata($nodeid,'title', 'mailto', $flavour);
                                }
                        }
                $nodeid = create_node($id, $doctypes['forum_posting'], $childcount['forum_posting'], 1, 0, '', $document['key_id'], 1, 1, 0, $document['members_only']);
                ++$childcount['forum_posting'];
                add_nodedata($nodeid,'timestamp',time(), $flavour);
                if(SESSION_STATUS == 'ok') { add_nodedata($nodeid, 'userid',SESSION_DBID, $flavour); }
                else {
                        add_nodedata($nodeid, 'nick',$docinput['nick'], $flavour);
                        add_nodedata($nodeid, 'email',$docinput['email'], $flavour);
                        }
                add_nodedata($nodeid, 'host',gethostbyaddr($_SERVER['REMOTE_ADDR']), $flavour);
                add_nodedata($nodeid, 'body', $docinput['body'],$flavour);
                add_nodedata($nodeid, 'title', $docinput['title'], $flavour);
                add_nodedata ($id, 'childcount', $childcount['forum_posting'],$flavour);
                add_nodedata ($id, 'timestamp', time(), $flavour);
                clear_cache($id);
                }


        elseif((isset($docinput['saveanswer']) || isset($_REQUEST['quote']) || isset($_REQUEST['answer'])) && (((!isemail($docinput['email']) || $docinput['nick'] == '' ||  $parentdata['onlyforusers'] == '1') && SESSION_STATUS != 'ok') || $docinput['body'] == '' || $docinput['title'] == '')) {
                if(!isset($_REQUEST['quote']) && !isset($_REQUEST['answer'])) {
                        ?><b>&gt;&gt;</b> <font color="#ff0000"><?=LANG_FORUM_ALLREQUIRED?></font><?php
                        if(DISABLE_EMAIL_CHECK != 1 && SESSION_STATUS != 'ok') {
                                ?><font color="#ff0000"><?=LANG_FORUM_EMAILVERIFIED?></font><?php
                                }
                        ?><br /><?php
                        }
                elseif(isset($_REQUEST['quote'])) { $get = get_nodedata ($_REQUEST['quote'], $flavour); }
                elseif(isset($_REQUEST['answer'])) {
                        $get = get_nodedata ($_REQUEST['answer'], $flavour);
                        echo $design['table_begin'];
                        ?><tr bgcolor="<?=$design['table_header']?>">
                        <td width="89%"><?=LANG_FORUM_SUBJECT?></td>
                        <td width="10%" nowrap><?=LANG_FORUM_AUTHOR?></td>
                        <td width="1%" nowrap><?=LANG_FORUM_LATEST?></td></tr>
                        <tr bgcolor = "<?=$design['table_row_dark']?>">
                        <td width="89%"><b><?=transform($get['title'], 'clean')?></b></td>
                        <td width="10%" nowrap><?=printnick($get)?></a><?=fhost($get['host'])?></td>
                        <td width="1%" nowrap align=right><?=transform($get['timestamp'], 'datetime')?></td></tr>
                         <tr bgcolor = "<?=$design['table_row_bright']?>"><td colspan="3"><?php
                        $get['body'] = transform($get['body'], 'usertext');
                        $get['body']= strtr ($get['body'], $replacearray);
                        echo $get['body'];
                        ?></td></tr><?=$design['table_end']?><br /><?php
                        }
                if(!isset($docinput['title'])) { $docinput['title']='RE: '.$get['title']; }
                ?><form action="<?=get_url($id)?>" method="post">
                <input type="hidden" name="docinput[saveanswer]" value="1"><?php
                if(SESSION_STATUS != 'ok') { ?><?=LANG_NICKNAME?><br /><input type="text" name="docinput[nick]" value="<?=$docinput['nick']?>" size="<?=$design['input_size']?>" maxlength="15"><?php }
                if(SESSION_STATUS != 'ok') { ?><br /><?=LANG_EMAIL_ADDRESS?><br /><input type="text" name="docinput[email]" value="<?=$docinput['email']?>" size="<?=$design['input_size']?>" maxlength="60"><br /><?php }
                ?><?=LANG_FORUM_SUBJECT?><br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>" maxlength="60" value="<?=$docinput['title']?>">
                <br /><?=LANG_FORUM_MESSAGE?><br /><textarea name="docinput[body]" cols="<?=$design['input_size']?>" wrap="virtual" rows="<?=$design['input_rows']?>"><?php
                if(isset($_REQUEST['quote'])) {
                        if(isset($get['userid'])) {
                                $get['nick'] = get_user_nickname($get['userid']);
                                }
                        $get['body'] = "On ".transform($get['timestamp'], 'datetime')." '".$get['nick']."' wrote: \n".$get['body'];
                        echo str_replace("\n","\n> ",$get['body']);
                        }
                else { echo $docinput['body']; }
                echo '</textarea>';
                if(SESSION_STATUS == 'ok' && issendpermail (SESSION_DBID) != '1') {
                        ?><br /><input type="checkbox" name="docinput[sendreplys]" value="1"> <?=LANG_FORUM_SENDBYEMAIL?><?php
                        }
                ?><br /><input type="submit" value="<?=LANG_FORUM_SREPLY?>"></form><?php
                $corrected = '1';
                }
        if($corrected != '1') {
                $document = array_merge(get_nodedata($id, $flavour), get_node($id, $flavour));
                if($childcount['forum_posting'] > 0 || SESSION_DBID != $document['userid']) {
                        echo '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="'.$design['table_cellspacing'].'" border="'.$design['table_border'].'">';
                        ?><tr><td align="left" valign="middle">
                        <h2><?=transform($document['title'], 'clean')?></h2>
                        </td><td align="right"><?php
                        $ismoverview = get_node($parentnode['parent_id'], $flavour);
                        if($ismoverview['type_id'] == $doctypes['forum_overview']) {
                                ?><form action="<?=$_SERVER['PHP_SELF']?>" method="post"><?=LANG_FORUM_JUMPTO?> <select size="1" name="id"><?php
                                $jumpforums = get_children(0, count_children($doctypes['forum_forum'],$flavour,$parentnode['parent_id']), $doctypes['forum_forum'], false,'DESC', false, $parentnode['parent_id']);
                                while(list($key, $value) = each($jumpforums)) {
                                        echo '<option value="'.$key.'" ';
                                        if($key == $document['parent_id']) { echo 'SELECTED'; }
                                        echo '>'.transform($value['title'], 'clean').'</option>';
                                        }
                                ?></select><input type="submit" value="<?=LANG_FORUM_GO?>" /></form><?php
                                }
                        ?></td></tr></table><?php
                        echo $design['table_begin'];
                        ?><tr bgcolor="<?=$design['table_header']?>">
                        <td width="79%"><?=LANG_FORUM_SUBJECT?></td>
                        <td width="10%" nowrap><?=LANG_FORUM_AUTHOR?></td>
                        <td width="10%" nowrap><?=LANG_FORUM_LATEST?></td><?php
                        if($parentnode['type_id'] == $doctypes['forum_forum'] && ($parentdata['onlyforusers'] != '1' || SESSION_STATUS == 'ok')) {
                                ?><td width="1%" nowrap align="center"><?=LANG_FORUM_REPLY?></td><?php
                                }
                        ?></tr><tr bgcolor = "<?=$design['table_row_dark']?>">
                        <td width="79%"><b><?=transform($document['title'], 'clean')?></b></td>
                        <td width="10%" nowrap><?=printnick($document)?><?=fhost($document['host'])?></td>
                        <td width="10%" nowrap align=right><?=transform($document['timestamp'], 'datetime')?></td><?php
                        if($parentnode['type_id'] == $doctypes['forum_forum'] && ($parentdata['onlyforusers'] != '1' || SESSION_STATUS == 'ok')) {
                                ?><td width="1%" nowrap align="center"><?php
                                if($parentdata['button'] != 'Text') {
                                        echo '<a href="'.get_url($id,'quote='.$id).'">'.$buttonarray[$parentdata['button']]['quote'].'</a>
                                        <a href="'.get_url($id,'answer='.$id).'">'.$buttonarray[$parentdata['button']]['reply'].'</a>';
                                        }
                                else {
                                        echo '[ <a href="'.get_url($id,'quote='.$id).'">'.LANG_FORUM_QUOTE.'</a> ]
                                                [ <a href="'.get_url($id,'answer='.$id).'">'.LANG_FORUM_REPLY.'</a> ]';
                                        }
                                echo '</td>';
                                }
                        ?></tr><tr bgcolor = "<?=$design['table_row_bright']?>"><td colspan="<?php
                        if($parentnode['type_id'] == $doctypes['forum_forum'] && ($parentdata['onlyforusers'] != '1' || SESSION_STATUS == 'ok')) { echo '4'; }
                        else { echo '3'; }
                        echo '">';
                        $document['body'] = transform($document['body'], 'usertext');
                        $document['body']= strtr ($document['body'], $replacearray);
                        echo $document['body'];
                        echo          '</td></tr>';
                        echo '<tr colspan="';
                        if($parentnode['type_id'] == $doctypes['forum_forum'] && ($parentdata['onlyforusers'] != '1' || SESSION_STATUS == 'ok')) { echo '4'; }
                        else { echo '3'; }
                        ?>"><td bgcolor="<?=$design['default_background']?>"><br /></td></tr><?php
                        }
                elseif(SESSION_DBID == $document['userid']) {
                        ?><form action="<?=get_url($id)?>" method="post">
                        <input type="hidden" name="docinput[updatenode]" value="1">
                        <?=LANG_FORUM_SUBJECT?><br /><input type="text" name="docinput[title]"  size="<?=$design['input_size']?>" maxlength="60" value="<?php
                        if(!isset($docinput['title'])) { echo $document['title']; }
                        else { echo $docinput['title']; }
                        ?>"><br /><?=LANG_FORUM_MESSAGE?><br /><textarea id="newstext" name="docinput[body]" cols="<?=$design['input_size']?>" wrap="virtual" rows="<?=$design['input_rows']?>"><?php
                        if(!isset($docinput['body'])) { echo $document['body']; }
                        else { echo $docinput['body']; }
                        ?></textarea>
                        <br /><input type="checkbox" name="docinput[timestamprefresh]" value="1" checked> <?=LANG_FORUM_REFRESHTIMESTAMP?>
                        <br /><br /><input type="submit" value="<?=LANG_FORUM_SUBMITCH?>"></form>
                        [ <a href="<?=get_url($document['parent_id'])?>"><?=LANG_BACK?></a> ]<?php
                        echo '<hr />';
                        echo "<p>Folgende Smilies stehen zur Verf&uuml;gung (einfach auf die Bilder klicken oder die" .
                          " Zeichen eingeben, die Leser sehen dann das Bild). </p>";
                        $anz = 0;
                        foreach($replacearray as $key=>$value) {
                          echo $value . ' ' . $key . ' ';
                          $anz++;
                          if ( $anz % 10 == 0 ) echo "<br />";
                        }
                        echo '<hr />';
                        }
                $result = get_sorted_children ($_REQUEST['first_item'] ,$parentdata['ppp'], 'timestamp',$doctypes['forum_posting'],-1,$sortorder);
                while(list($key, $value) = each($result)) {
                        ?><tr bgcolor = "<?=$design['table_row_dark']?>"><td width="79%"><b><?=transform(substr($value['title'],0,60), 'clean')?><?php
                        if(strlen($value['title'])>60){echo '...';}
                        ?></b><?php
                        if($value['userid'] == SESSION_DBID) {
                                echo ' [ <a href="'.get_url($key).'">'.LANG_FORUM_EDIT.'</a> ]';
                                }
                        if($value['timestamp']>time()-86400 && $parentdata['marknew'] == 1) {
                                echo '<font color="#ff0000" size="-2"> '.LANG_FORUM_NEW.'</font>';
                                }
                        ?></td><td width="10%" nowrap><?php
                        echo printnick($value);
                        echo fhost($value['host']);
                        ?></td><td width="10%" nowrap align=right><?=transform($value['timestamp'], 'datetime')?></td><?php
                        if($parentnode['type_id'] == $doctypes['forum_forum'] && ($parentdata['onlyforusers'] != '1' || SESSION_STATUS == 'ok')) {
                                echo '<td width="1%" nowrap align="center">';
                                if($parentdata['button'] != 'Text') {
                                        echo '<a href="'.get_url($id, 'quote='.$key).'">'.$buttonarray[$parentdata['button']]['quote'].'</a>
                                         <a href="'.get_url($id, 'answer='.$key).'">'.$buttonarray[$parentdata['button']]['reply'].'</a>';
                                        }
                                else {
                                        echo'[ <a href="'.get_url($id,'quote='.$key).'">'.LANG_FORUM_QUOTE.'</a> ]
                                        [ <a href="'.get_url($id,'answer='.$key).'">'.LANG_FORUM_REPLY.'</a> ]';
                                        }
                                echo '</td>';
                                }
                        echo '</tr>
                         <tr bgcolor = "'.$design['table_row_bright'].'"><td colspan="';
                        if($parentnode['type_id'] == $doctypes['forum_forum'] && ($parentdata['onlyforusers'] != '1' || SESSION_STATUS == 'ok')) { echo '4'; }
                        else { echo '3'; }
                        echo '">';
                        $value['body'] = transform($value['body'], 'usertext');
                        $value['body'] = strtr ($value['body'], $replacearray);
                        echo $value['body'];
                        echo          '</td></tr>';
                        }

                if($childcount['forum_posting'] > 0 || SESSION_DBID != $document['userid']) {
                        echo $design['table_end'];
                        if($childcount['forum_posting'] > $parentdata['ppp'] && $parentnode['type_id'] == $doctypes['forum_forum']) {
                          echo LANG_FORUM_PAGE.' ';
                          $temp_counter = $parentdata['ppp'];
                          while($parentdata['ppp'] + $childcount['forum_posting'] > $temp_counter) {
                                             $topic_counter += 1;
                                             echo '[ ';
                                            if($_REQUEST['first_item'] == ($temp_counter - $parentdata['ppp'])) { echo '<b>'; }
                                           echo '<a href="'.get_url($id,'first_item='.($temp_counter - $parentdata['ppp'])).'">';
                                           echo ((($topic_counter-1)*$parentdata['ppp'])+1).'-'.($topic_counter*$parentdata['ppp']).'</a>';
                                              if($_REQUEST['first_item'] == ($temp_counter - $parentdata['ppp'])) { echo '</b>'; }
                                              echo ' ] ';
                                           $temp_counter += $parentdata['ppp'];
                                              if($topic_counter >= ($parentdata['maxposts']/$parentdata['ppp'])) { break; }
                          }
                        }
                        echo '<p>[ <a href="'.get_url($document['parent_id']).'">'.LANG_BACK.'</a> ] ';
                        if($parentdata['onlyforusers'] == '1' && SESSION_STATUS != 'ok') {
                                echo LANG_FORUM_HAVETOBELOGGEDIN;
                                }
                        if(SESSION_STATUS == 'ok') {
                                if(issendpermail (SESSION_DBID) == '1') {
                                        echo ' [ <a href="'.get_url($id,'dsendperemail=1').'">'.LANG_FORUM_DONTSENDANYMORE.'</a> ]';
                                        }
                                if($parentnode['type_id'] == $doctypes['forum_forum']) {
                                        echo ' [ <a href="'.get_url($id,'sendcomplete=1').'">'.LANG_FORUM_SENDCOMPLETE.'</a> ]';
                                        }
                                }
                        }
                }
        }
?>