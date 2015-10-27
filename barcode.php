<?php

include 'Image/Barcode.php';
if (isset($_REQUEST['Anzeige']))

{

	Image_Barcode::draw($_REQUEST['Anzeige'], 'Code39', 'png');


}

?>
