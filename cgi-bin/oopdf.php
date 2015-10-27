<?php
// Datenbankverbindung herstellen
// Auftrag auslesen
define('VERZEICHNIS', '/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/');
define('DOCS', 'httpsdocs/');
if ( isset($_REQUEST['Datei']))
{
	$Datei = $_REQUEST['Datei'];
}
else
$Datei = '';

if ( isset($Datei) && $Datei != '' )
{
	if ( file_exists(VERZEICHNIS.DOCS.$Datei.'.pdf'))
	{
		echo 'Fehler: '.$Datei.'.pdf existiert bereits.';
	}
	else
	{
		if ( file_exists(VERZEICHNIS.DOCS.$Datei.'.sxw'))
		{
			$newname = pathinfo($Datei);
			if (!isset($newname['extension'])) $newname['extension']='';
			$name = basename($newname['basename'],$newname['extension']);
			// rufe Java auf um die Datei mit OO zu konvertieren
			passthru('/usr/bin/java -jar '.VERZEICHNIS.'jodconverter-2.2.2/lib/jodconverter-cli-2.2.2.jar -f pdf '.VERZEICHNIS.DOCS.$Datei.'.sxw');

			if ( file_exists(VERZEICHNIS.DOCS.$Datei.'.pdf'))
			{
			    $loc = 'https://' . $_SERVER['HTTP_HOST'].'/'.$Datei.'.pdf';
                header("HTTP/1.1 301 Moved Permanently");
                header( 'Location: '. $loc ) ;
            	
				echo 'Redirecting... <a href="'.$loc.'">PDF-Dokument</a>';
			}
			else
			{
				echo 'Fehler: '.$Datei.'.sxw konnte nicht konvertiert werden! OO Prozess prüfen.'."\n\n";
				echo 'PDF-Name: '.$Datei.'.pdf';
			}
		}
		else
		{
			echo 'Fehler: Datei '.$Datei.' existiert nicht oder es bestehen keine Zugriffsrechte.';
		}
	}
}
else
{
	echo 'Fehler: Es wurde keine Datei zur Konvertierung übergeben.';
}
?>
