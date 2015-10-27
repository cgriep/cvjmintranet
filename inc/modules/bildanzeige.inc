<?php

   /* Bild abh&auml;ngig von der obersten Ebene anzeigen */


   // 1. Feststellen, wer der Vater ist (solange nach oben, bis der Vater 1 ist
   // dann Bild raussuchen und anzeigen
   global $root_parent_id;
   // echo "Mein Root ist " . $root_parent_id;
   init_published_roots();
   $nr = 0;
   global $published_roots;
   while ( list($did, $value) = each($published_roots) ) {
     $nr++;
     // echo "--->".$did." ";
     if ( $root_parent_id == $did ) $dienr = $nr;
   }
   global $design;
   echo '<img src="img/design/'.$design['name'].'/ebene'.$dienr.'_gr.jpg" border="0">';
?>