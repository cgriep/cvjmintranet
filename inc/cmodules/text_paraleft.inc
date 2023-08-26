<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.12.2001
*/

        if($profile['editor']!='1') {
                if($document['title'] != '') {
                        echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
                        }
                if($document['image'] != '' || $document['body'] != '') {
                        echo '<p class="left">';
                        // Image
                        echo transform($document['image'], $document['datatype']['image']);
                        // Text
                        echo transform($document['body'], $document['datatype']['body']).'</p>';
                        }

                if(isset($parents)) {
                        echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_SHOW_COMPLETE_DOC.'</a> ]');
                        }

                }
        elseif($profile['editor']=='1') {
                // Image Input
                create_editor_input('Image','image','image_input');
                echo '<br /><br />';
                // Text Input
                create_editor_input('Body','body','area_input');
                }
?>