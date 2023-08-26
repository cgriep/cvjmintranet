<?php
// top level navigation

init_published_roots();
reset($published_roots);
$pre = '&nbsp;';
        foreach ($published_roots as $docid => $value)  {
                echo $pre;
                if($root_parent_id == $docid || $docid == $id) { echo '<b>';
                }
                echo '<a href="'.get_url($docid).'">'.$value['title'].'</a>';
                if($root_parent_id == $docid || $docid == $id) { echo '</b>'; 
                }
                $pre = ' | ';
                }
?>