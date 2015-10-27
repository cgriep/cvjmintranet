<?php
$mail = '';
foreach ($_REQUEST as $key => $value )
{
	$mail .= $key.' = '.$value."\n";	 
}
if ( $mail != '' )
  mail('bestellung@cvjm-feriendorf.de', 'Bestellung '.date('d.m.Y H:i'), $mail);
  
?>
<html>
<body>
Ihr Bestellung wurde vermerkt.
<a href="http://cvjm-feriendorf.de">Zurück</a>
</body>
</html>