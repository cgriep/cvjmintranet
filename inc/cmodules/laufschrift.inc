<?php

  /* Laufschrift-Modul */

if ( $profile['editor'] == "1" ) {
  create_editor_input("Laufschrifttext", "Laufschrift", "area_input");
  echo "<br /><br />";
  create_editor_input("Verz&ouml;gerung", "Delay", "text_input");
  echo "<br /><br />";
  create_editor_input("Verschiebung in Pt", "Scrollamount", "text_input");
}
else {

?>

<marquee scrollamount="<?=$document['Scrollamount']?>" scrolldelay="<?=$document['Delay']?>">
<b><?=$document['Laufschrift']?></b>
</marquee>

<?php
}

?>