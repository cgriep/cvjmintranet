<?php
// top level navigation

init_published_roots();
reset($published_roots);
$pre = '&nbsp;';
        while (list ($docid, $value) = each ($published_roots)) {
                echo $pre;
                if($root_parent_id == $docid || $docid == $id) { echo '<b>';
                }
                echo '<a href="'.get_url($docid).'">'.$value['title'].'</a>';
                if($root_parent_id == $docid || $docid == $id) { echo '</b>'; 
                }
                $pre = ' | ';
                }
?>