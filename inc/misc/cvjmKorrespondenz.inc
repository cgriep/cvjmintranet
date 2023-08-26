<?php
/**
 * Zeigt die Vorlagen in Form eines Menüs an. Es muss die Flyout-Css-Vorlage eingebunden sein
 * @param int $Adressen_id die ID der Adresse
 * @param int $InstAdressen_id die ID der Institution
 * @param array $Vorlagen die Vorlagen, die angezeigt werden sollen
 * @param String $Datenbank der Name der Datenbank, aus der später die Vorlagen erstellt werden sollen
 * @returns String eine HTML-Zeichenkette, die die Vorlagen enthält
 */
function ListeVorlagen($Adressen_id, $InstAdressen_id, $Vorlagen, $Datenbank='Adressen')
{
	$s = '';
	foreach ($Vorlagen as $key => $value )
	{
		$s .= '[ <a href="cvjmVorlagen.php?Vorlage='.$value.'&id=';
		$s .= $Adressen_id.'&InstId='.$InstAdressen_id.
		'&db='.$Datenbank.'&User='.get_user_nickname().'" title="Datei aus Vorlage '.
		basename($value,CVJM_ENDUNG).' erstellen" target="_blank">'.
		$key.'</a> ]&nbsp;';
	}	
	return $s;
}

function holeVorlagen($Wofuer)
{
	if ( $Wofuer != '' ) $Wofuer = '/'.$Wofuer;
	$Buchungsvorlagen = glob(CVJM_VORLAGEN_DIR.$Wofuer.'/*'.CVJM_ENDUNG);
	return $Buchungsvorlagen;
}

function ZeigeVorlagen($idnummer, $Wofuer, $Datenbank = '')
{
	$Buchungsvorlagen = holeVorlagen($Wofuer);
	echo '<strong>Vorlagen: </strong>';
	$anz = 0;
	if ( $Datenbank == "" ) $Datenbank = $Wofuer;
	foreach ($Buchungsvorlagen as $filename)
	{
		echo '[ <a href="cvjmVorlagen.php?Vorlage='.basename($filename).'&id=';
		echo $idnummer.'&db='.$Datenbank.'&User='.get_user_nickname().'" target="_blank" ';
		echo ' onClick="javascript:return window.confirm(\'Wirklich ein neues Dokument ';
		echo basename($filename,CVJM_ENDUNG)." erstellen? ";
		echo "Die vorhandenen Dokumente befinden sich am Ende der Seite.');";
		echo '">'.basename($filename,CVJM_ENDUNG)."</a> ] \n";
		$anz++;
	}
	if ( $anz == 0 ) echo '-keine vorhanden-';
	echo "<br />\n";
	return $Buchungsvorlagen;
}

/**
 * Zeit die vorhandene Korrespondenz an.
 * Voraussetzung:
 * xajax muss initialisiert sein (printJavaScript)
 * @param korrespondenz Datensatz mit den notwendigen Schlüsselinformationen (Adressen_id, $Buchung_Nr und ggf. Rechnung_id)
 * @param array Buchungsunerlagen Feld mitden Namen der vorhandenen Vorlagen-Namen
 * @param array params Die Parameter die beim Aufruf von Links übergeben werden sollen 
 */
function ZeigeKorrespondenz($korrespondenz, $Buchungsvorlagen, $params)
{
	global $id;
	global $document;
	global $profile;
	echo '<div id="Korrespondenzdiv">';
	echo holeKorrespondenzListe($id, $Buchungsvorlagen, $korrespondenz, $params, $profile[$document['SpeicherRecht']] == 1);
	echo '</div>';
}
/**
 * Erstellt eine Tabelle mit der Korrespondenz zu einer Adresse, einer Buchung oder einer Abrechnung.
 * @param int $id die id der aktuellen Seite
 * @param array $Buchungsvorlagen ein Feld mit den Namen der relevanten Vorlagen
 * @param array $korrespondenz Feld mit Buchung_Nr, Adressen_id und Rechnung_id je nach Ausgabe
 * @param array $params Parameter, die beim Verlinken angefgt werden sollen
 * @param boolean $Speichern true, wenn gelöscht werden darf, false sonst 
 * @return String die Tabelle im HTML-Format 
 */
function holeKorrespondenzListe($id, $Buchungsvorlagen, $korrespondenz, $params = array(), $Speichern=true)
{
	// $Buchungsvorlagen muss ein Feld mit den Vorlagen sein
	// Korrespondenz anzeigen
	if ( isset($korrespondenz['Rechnung_id']) )
	{
		$Nr = $korrespondenz['Rechnung_id'];
		$did = 'R'.$Nr;
	}
	else if ( isset($korrespondenz['Buchung_Nr']) )
	{
		$Nr = $korrespondenz['Buchung_Nr'];
		$did = 'B'.$Nr;
	}
	else
	$Nr = -1;
	$dateien['Zeit'] = array();
	$dateien['Name'] = array();
	foreach ($Buchungsvorlagen as $filename)
	{
		if ( strpos($filename,'*') !== false)
		{
			$Dateiname = getAdressenAnhangLink($korrespondenz['Adressen_id'],
		$Nr, '*', $filename, '*');
			foreach (glob($Dateiname) as $Anhang)
			{
				if ( ! in_array($Anhang, $dateien['Name']))
				{
					$dateien['Zeit'][] = filemtime($Anhang);
					$dateien['Name'][] = $Anhang;
				}				
			}			
		}
		else
		{
			$Dateiname = getAdressenAnhangLink($korrespondenz['Adressen_id'],
			$Nr, CVJM_ENDUNG, basename($filename,CVJM_ENDUNG), '*');
			foreach (glob($Dateiname) as $Anhang)
			{
				$dateien['Zeit'][] = filemtime($Anhang);
				$dateien['Name'][] = $Anhang;
			}
			// Nun muss die Vorlage (Endung CVJM_ENDUNG) als PDF gesucht werden
			$Dateiname = getAdressenAnhangLink($korrespondenz['Adressen_id'],
			$Nr, CVJM_ENDUNG_PDF, basename($filename,CVJM_ENDUNG), '*');
			foreach (glob($Dateiname) as $Anhang)
			{
				$dateien['Zeit'][] = filemtime($Anhang);
				$dateien['Name'][] = $Anhang;
			}
		}
	}
	/*
	if ( isset($korrespondenz['Buchung_Nr']) && !isset($korrespondenz['Rechnung_id']) )
	{
		if ( !$query = sql_query('SELECT Rechnung_id FROM '.TABLE_RECHNUNGEN.' WHERE F_Buchung_Nr='.
		$korrespondenz['Buchung_Nr'].' ORDER BY Rechnungsdatum DESC'))
		{
			throw new Exception('Fehler bei Korrespondenzliste: '.sql_error());
		}
		while ( $rechnung = sql_fetch_row($query) )
		{
			$Dateiname = getAdressenAnhangLink($korrespondenz['Adressen_id'],
			$rechnung[0], CVJM_ENDUNG, '*', '*');
			foreach (glob($Dateiname) as $Anhang)
			{
				$dateien['Zeit'][] = filemtime($Anhang);
				$dateien['Name'][] = $Anhang;
			}
			$Dateiname = getAdressenAnhangLink($korrespondenz['Adressen_id'],
			$rechnung[0], CVJM_ENDUNG_PDF, '*', '*');
			foreach (glob($Dateiname) as $Anhang)
			{
				$dateien['Zeit'][] = filemtime($Anhang);
				$dateien['Name'][] = $Anhang;
			}
		}
		sql_free_result($query);
	}
	*/
	arsort($dateien['Zeit']);
	$Liste = array();
	foreach ( $dateien['Zeit'] as $i => $zeit )
	{
		$datei['Name'] = $dateien['Name'][$i];
		$data = pathinfo($dateien['Name'][$i]);
		$datei['Extension'] = $data['extension'];
		$datei['NurName'] = basename($dateien['Name'][$i],'.'.$data['extension']);
		$datei['Zeit'] = $zeit;
		$datei['id'] = $did;
		$datei['Art'] = '';
		if ( isset($Rechnung_Nr))
		{
			$datei['id'] .= 'R'.$Rechnung_Nr;
		}
		$datei['Link'] = getAdressenAnhangLink($korrespondenz['Adressen_id'],
		-1, '.'.$data['extension'], basename($dateien['Name'][$i],'.'.$data['extension']));
		$Liste[] = $datei;
	}
	// Standardvorlagen für die Dokumentart anfügen 
	foreach (glob('Vorlagen/Dokumente/*') as $Anhang)
	{
		$datei['Name'] = $Anhang;
		$data = pathinfo($Anhang);
		$datei['Extension'] = $data['extension'];
		$datei['NurName'] = basename($Anhang,'.'.$data['extension']);
		$datei['Zeit'] = filemtime($Anhang);
		$datei['id'] = '';
		$datei['Art'] = 'Standard';
		$datei['Link'] = $Anhang;
		$Liste[] = $datei;
	}
	require_once (INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; // '/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('Korrespondenzurl', get_url(get_cmodule_id('cvjmkorrespondenzverwaltung')));
	$Smarty->assign_by_ref('Dateien', $Liste);
	$Smarty->assign('id', $id);
	$Smarty->assign('Speichern', $Speichern);
	$Smarty->assign('params', implode('&', $params));
	//require_once(INC_PATH.'cvjm/Adresse.class.php');
	$Adresse = new Adresse ($korrespondenz['Adressen_id']);
	$Smarty->assign_by_ref('Adresse', $Adresse);
	return $Smarty->fetch('Korrespondenz.tpl');
}
?>
