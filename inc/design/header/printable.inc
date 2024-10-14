<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/
 
        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 14.09.2001
*/

?>

<p>
<?php 
        init_parents();
        if(isset($parents)) {
                reset($parents);
                foreach ($parents as $docid => $value)  {
                        $section_title = $value['title'].' &gt; '.$section_title;
                        }
                }
        echo $section_title." ".$document['title'];
?>
</p>

