<?php
/* AWF-Modul zur Verwaltung von htaccess-Dateien

   Christoph Griep 27.07.03   cgriep@fricom.com

*/


echo "Bitte das Passwort angeben:";
echo '<FORM ACTION="?id=291" METHOD=post>';
echo '<INPUT type=text name="klartext" maxlength=12>';
echo '<INPUT type=submit value=Go>';
echo '</FORM>';
$klartext = $_REQUEST['klartext'];
if ( $klartext != "" ) {
  $salt=21;
  echo "Der Crypt lautet:".crypt($klartext,$salt);
}
echo "<hr>";



?>