<?php

/* Funktionen für den Javascript-Kalender */

function KalenderScriptEinfuegen()
{
  // Style Sheet mit umfassenden <style>-Tags!
//  include("css/popcalendar.css");
  echo '
  <style type="text/css">
@import url(css/popcalendar.css);
</style>';
  echo '<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>';
  echo '<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>';
}
// Trägt die beiden Events für den PopUp-Kalender ein (und nur diese)
// muss innerhalb eines Input-Tags stehen
function KalenderEvents($FormName, $FieldName, $mitPruefung = true)
{
  echo ' onClick="popUpCalendar(this,'.$FormName.
        "['".$FieldName."'],'dd.mm.yyyy')".'"';
  if ( $mitPruefung )
    echo ' onBlur="autoCorrectDate(\''.$FormName."','".$FieldName."' , false )".'"';
}

?>