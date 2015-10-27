<?php
// Datenbankverbindung herstellen
// Auftrag auslesen
define('VERZEICHNIS', '/srv/www/awftest/');

if ( isset($argv[1]) && $argv[1] != '' )
{
 $newname = pathinfo($argv[1]);
 if (!isset($newname['extension'])) $newname['extension']='';
 $name = basename($newname['basename'],$newname['extension']); 
 $datei = '/srv/pdf/'.$name.'pdf';
  @unlink($datei);
// copy('/var/lib/gdm/:0.Xauth','/srv/www/cgi-bin/:0.Xauth'); 
 passthru('/usr/lib/ooo-2.0/program/soffice -headless -display :0 -pt "PDF converter" '.VERZEICHNIS.'/'.$argv[1]);
  if ( file_exists($datei))
  {
    rename($datei,VERZEICHNIS.'/'.$newname['dirname'].'/'.$name.'pdf');
    $loc = 'http://' . $_SERVER['HTTP_HOST']. '/'.$newname['dirname'].'/'.$name.'pdf';    
    echo 'Status: 301 Moved Permanently'."\n";
    echo 'Location: '.$loc."\n\n";    
    echo "Content-type: text/html\n\n";
    echo 'Redirecting... <a href="'.$loc.'">PDF-Dokument</a>';
  }
  else
  {
  	echo "Content-Type: text/html\n\n";
    echo 'Fehler: '.$argv[1].' konnte nicht konvertiert werden!<br />';
    echo 'PDF-Name: '.$datei;
  if ( ! file_exists('/srv/www/cgi-bin/:0.Xauth'))
{
    echo '\n\nAuthorisationsdatei fuer X-Server fehlt!';
 }

  }
}
else
{
	echo "Content-Type: text/html\n\n";
    echo 'Fehler: Es wurde keine Datei zur Konvertierung übergeben.';
}
?>
