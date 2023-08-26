<?php

/*
        This is the forumoverview content module for the AWF. It's job is managing the forum content module.
        maintained by Christoph Stickel <mixer2@web.de>

        23.06.2002: Adapted to XHTML 1.0 [michael@liquidbytes.net]
*/


load_lang('forum');
$childcount = count_children($doctypes['forum_forum'], $flavour);

if($document['cacheable'] == 1) {
        sql_query("UPDATE ".TABLE_NODES." SET cacheable = 0 WHERE id = $id");
        $document['cachable'] = 0;
        }

include(INC_PATH.'misc/forum_functions.inc');

if($profile['editor'] == 1) {
        if($docinput['addmessageboard'] != 1) {
                if(isset($docinput['deleteposting'])) {
                        foreach($docinput['deleteposting'] as $delkey) { destroy_node($delkey); }
                        $childcount = $childcount-sizeof($docinput['deleteposting']);
                        }
                if($docinput['deleteall'] == '1') {
                        $result = get_children(0, $childcount, $doctypes['forum_forum'], 0, 'DESC');
                        while(list($key, $value) = each($result)) { destroy_node($key); }
                        $childcount = 0;
                        }


                create_editor_input ('Headline (optional)', 'headline', 'text_input');
                echo '<br /><br />';
                create_editor_input ('Description (optional)', 'descr', 'area_input');
                ?><br /><br /><input type="checkbox" name="docinput[addmessageboard]" value="1"> Add Forum </a>
                <br /><br /><br /><?php
                if($childcount > 0) {
                                        echo $design['table_begin'];
                                        ?><tr bgcolor="<?=$design['table_header']?>">
                                        <td width=99%>Forum</td>
                                        <td width=1% nowrap>Edit</td></tr><?php
                                        }
                $result = get_children (0 ,$childcount,$doctypes['forum_forum']);
                $colorswitch = '0';
                while(list($key, $value) = each($result)) {
                        echo '<tr bgcolor = "';
                        $colorswitch = getswitchcolor();
                        ?>"><td width="100%" nowrap><b><a href="<?=get_url($key)?>"><?=transform($value['title'], 'clean')?></a></b></td>
                        <td width="1%" nowrap> <input type="checkbox" name="docinput[deleteposting][]" value="<?=$key?>"> Remove</td></tr><?php
                        }
                if($childcount > 0) { ?><tr bgcolor = "<?=$design['table_row_dark']?>"><td width="100%">&nbsp;</td><td width="1%" nowrap>
                                      <input type="checkbox" name="docinput[deleteall]" value="1"> Remove All</td></tr><?php
                                      echo $design['table_end'];
                                      }
                }
        else {
                $nodeid = create_node($id, $doctypes['forum_forum'], $childcount+1, 1, 0, '', $document['key_id'], 1, 1, 0, $document['members_only']);
                add_nodedata($nodeid,'timestamp',time(), 0);
                add_nodedata($nodeid,'title','Forum number '.($childcount+1), $flavour);

                ?><span style="text-align: center">[ <a href="<?=get_url($nodeid)?>">Create new forum</a> ]</span><?php
                }
        }
else {
        if(isset($document['headline']) && $document['headline'] != "") { ?><h2><?=transform($document['headline'], 'clean')?></h2><?php }
        if(isset($document['descr']) && $document['descr'] != "") { ?><p><?=transform($document['descr'], $document['datatype']['descr'])?></p><?php }

        $result = get_children (0 ,$childcount,$doctypes['forum_forum']);

        echo $design['table_begin'];
        ?><tr bgcolor="<?=$design['table_header']?>">
        <td nowrap><?=LANG_FORUM_FORUM?></td>
        <td width="80%"><?=LANG_FORUM_DESCRIPTION?></td>
        </tr><?php
        $colorswitch = '0';
        foreach ( $result as $key => $value) {
                echo '<tr bgcolor = "';
                $colorswitch = getswitchcolor();
                ?>"><td nowrap><b><a href="<?=get_url($key)?>"><?=transform($value['title'], 'clean')?></a></b><?php
                if(!isset($value['childcount'])) {
                        $value['childcount'] = count_children($doctypes['forum_posting'], $flavour, $key);
                        add_nodedata($key, 'childcount', $value['childcount'], $flavour);
                }
                echo ' ('.$value['childcount'].')';
                ?></td><td width="80%"><?php
                if(isset($value['descr']) && $value['descr'] != "")        { echo transform($value['descr'], 'text'); }
                else { ?>&nbsp;<?php }
                // neu: Datum des letzten Eintrages
                $result2 = get_sorted_children (0 , 1, 'timestamp', $doctypes['forum_posting'], $key);
                $z1 = 0;
                while ( list($k, $v) = each($result2) ) {
                  //echo transform($v['timestamp'],"datetime");
                  if ( $v['timestamp'] > $z1 ) $z1 = $v['timestamp'];
                }
                if ( $z1 != 0 ) echo ' <font size="-2">('.transform($z1, 'datetime').')</font>';
                $ll = 2147483647;
                if ( defined(SESSION_LAST_LOGIN) ) {
                  $ll = SESSION_LAST_LOGIN;
                }
                else
                  $ll = time()-86400;
                if ( $z1 > $ll ) {
                   echo '<font color="#ff0000" size="-2"> '.LANG_FORUM_NEW.'</font>';
                }
                echo '</td>';
                echo '</tr>';
                }
        echo $design['table_end'];
        if($childcount < 1) {
                echo '<i>'.LANG_NO_CONTENT.'</i>';
                }
        }
?>