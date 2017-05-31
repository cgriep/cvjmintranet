<?php
/**
 * Entfernt alle SXW Dateien im Verzeichnis Adressen für die ein PDF vorhanden ist
 */
DEFINE('VERZEICHNIS', 'Adressen');

function sucheDateien($bbase)
{
	$size = 0;
	print 'durchsuche '.$bbase."\n";
	$handle=opendir($bbase);
	while (false != ($file = readdir($handle)))
	{
		if(filetype($bbase."/".$file) == 'file')
		{
			if ( strpos($file,'.sxw') == strlen($file)-4 )
			{
				// swx gefunden
				if ( file_exists($bbase.'/'.substr($file,0, strlen($file)-4).'.pdf'))
				{
					print 'PDF für '.$bbase.'/'.$file.' gefunden '.floor(filesize($bbase."/".$file) / 1024)."\n";
                    $size += floor(filesize($bbase."/".$file) / 1024);					
                    unlink($bbase.'/'.substr($file,0, strlen($file)-4).'.sxw');
				}
			}
            if ( strpos($file,'.odt') == strlen($file)-4 )
			{
				// odt gefunden
				if ( file_exists($bbase.'/'.substr($file,0, strlen($file)-4).'.pdf'))
				{
					print 'PDF für '.$bbase.'/'.$file.' gefunden '.floor(filesize($bbase."/".$file) / 1024)."\n";
                    $size += floor(filesize($bbase."/".$file) / 1024);					
                    unlink($bbase.'/'.substr($file,0, strlen($file)-4).'.odt');
				}
			}
		}
		else
		{
			// rekursiv Unterverzeichnisse prüfen
			if ( filetype($bbase.'/'.$file) == 'dir' && $file != '.' && $file != '..' )
			{
				$size += sucheDateien($bbase.'/'.$file);
			}
		}
	}
	closedir($handle);
	return $size;
}

print sucheDateien(VERZEICHNIS).' Gesamt'."\n";

?>
