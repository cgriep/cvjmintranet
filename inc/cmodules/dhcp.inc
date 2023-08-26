<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/
?>

<table border=0 width="100%" cellpadding="0" cellspacing="0">
<tr><td><b>Hostname</b></td><td><b>MAC Adresse</b></td><td><b>IP Adresse</b></td><td><b>Status</b></td></tr>
<?php
$lines = file("/etc/dhcpd.conf");
$host = '';

while(list ($line_num, $line) = each ($lines)) {
	$sbuffer =  explode(' ', $line);
	if($line[0] == '}' && $host != '') {
		// if(fsockopen($ipadr, 80)) echo 'ONLINE: ';
		echo('<tr><td>'.$host.'</td><td>'.$macadr.'</td><td>'.$ipadr.'</td><td>');
		if(strstr(exec('ping -n -c 1 -w 1 '.$ipadr), '100% packet loss')) {
			echo '<font color="#770000">offline</font>';
			}
		else {
			echo '<font color="#009900"><b>online</b></font>';
			}
		echo('</td></tr>');
		$host = '';
		}
	if($line != '' && $sbuffer[0] == 'host') { 
		$host = $sbuffer[1];
		}
	if($host != '' && $sbuffer[0] == 'hardware') {
		$macadr = substr($sbuffer[2], 0, -2);
		}
	if($host != '' && $sbuffer[0] == 'fixed-address') {
		$ipadr = substr($sbuffer[1], 0, -2);
		}
	}
fclose($fd);
?>
</table>
