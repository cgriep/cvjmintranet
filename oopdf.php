<?php
// Datenbankverbindung herstellen
// Auftrag auslesen
define('VERZEICHNIS', '/var/www/');
define('DOCS', 'httpdocs/');
define('CVJM_ENDUNG', '.odt');

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
		if ( file_exists(VERZEICHNIS.DOCS.$Datei.CVJM_ENDUNG))
		{
			$newname = pathinfo($Datei);
			if (!isset($newname['extension'])) $newname['extension']='';
			$name = basename($newname['basename'],$newname['extension']);
			// rufe Java auf um die Datei mit OO zu konvertieren
			passthru('/usr/bin/java -jar '.VERZEICHNIS.'jodconverter-2.2.2/lib/jodconverter-cli-2.2.2.jar -f pdf '.VERZEICHNIS.DOCS.$Datei.CVJM_ENDUNG);

			if ( file_exists(VERZEICHNIS.DOCS.$Datei.'.pdf'))
			{
				$loc = 'https://' . $_SERVER['HTTP_HOST'].'/'.$Datei.'.pdf';
				header("HTTP/1.1 301 Moved Permanently");
				header( 'Location: '. $loc ) ;
				echo 'PDF-Dokument: '.$loc;
			}
			else
			{
				echo 'Fehler: '.$Datei.CVJM_ENDUNG.' konnte nicht konvertiert werden! OO Prozess prüfen: Nach Login: ps -ef |grep soffice. Wenn kein Ergebnis ausser "grep soffice" dann ./OOStarten.sh eingeben.'."\n\n";
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
	echo 'Fehler: Es wurde keine Datei zur Konvertierung �bergeben.';
}
?>
