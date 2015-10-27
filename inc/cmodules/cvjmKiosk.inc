<?php

if ($profile['editor'] != '1')
{
	include (INC_PATH . 'misc/CVJM.inc');
	require_once (INC_PATH.'Smarty/Smarty.class.php');
	include_once (INC_PATH . 'cvjm_ajax.php');
	$xajax->printJavascript('./javascript');

	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('id', $id);
	$Smarty->assign('title', transform($document['title'],$document['datatype']['title']));
	if (!isset($_REQUEST['Barcodes']))
	{
		$Smarty->assign_by_ref('docinput', $docinput);
		// Geldbuchung
		if ( isset($docinput['Geld']))
		{
			$docinput['Geld'] = str_replace(',','.',$docinput['Geld']);
			if ( is_numeric($docinput['Geld']))
			{
			  $sql = 'INSERT INTO cvjm_Kiosk (Menge,Preis,Bezeichnung,Benutzer) VALUES (1,'. $docinput['Geld'].',"'.
			    'Geldbuchung Intranet","'.get_user_nickname().'")';
			   mysql_query($sql);
			}
		}
		if ( isset($docinput['Mitarbeiter']) && $docinput['Mitarbeiter'] != '' )
		{	
			$sql = 'SELECT * FROM cvjm_Kiosk_Mitarbeiter WHERE Benutzer="'.mysql_escape_string($docinput['Mitarbeiter']).'"';
			$query = mysql_query($sql);
			$Mitarbeiter = array();
			while ($row = mysql_fetch_array($query))
			{
				$Mitarbeiter[] = $row;
			}
			mysql_free_result($query);
			$Smarty->assign('Mitarbeiterliste', $Mitarbeiter);
		}
		
		$query = mysql_query('SELECT Sum(Preis*Menge) FROM cvjm_Kiosk');
		$row = mysql_fetch_row($query);
		mysql_free_result($query);
		$Smarty->assign('Kassenstand', $row[0]);

		if ( isset($docinput['TageVon']))
		{
			$Datum = convertDatumToTimeStamp($docinput['TageVon']);
			$Datum2 = convertDatumToTimeStamp($docinput['TageBis']);
			$Buchungen = array();
			$query = mysql_query('SELECT Art, Sum(Menge*Preis) AS Umsatz FROM cvjm_Kiosk '.
			'LEFT JOIN cvjm_Artikel ON F_Artikel_id=id '.
			'LEFT JOIN cvjm_Artikelarten ON F_Art_id=Art_id '.
			'WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$Datum2).
			'" GROUP BY Art');
			while ($row = mysql_fetch_array($query))
			{
				$Arten[] = $row;
			}
			mysql_free_result($query);
			$Smarty->assign_by_ref('Arten', $Arten);
		}
		if ( isset($docinput['AnzeigeVon']))
		{
			$Datum = convertDatumToTimeStamp($docinput['AnzeigeVon']);
			$DatumP1 = strtotime('+1day',$Datum);
			$Buchungen = array();
			$sql = 'SELECT * FROM cvjm_Kiosk WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$DatumP1).'" ORDER BY Datum DESC';
			$query = mysql_query($sql);
			while ($row = mysql_fetch_array($query))
			{
				$Buchungen[] = $row;
			}
			mysql_free_result($query);
			$Smarty->assign_by_ref('Buchungen', $Buchungen);
		}
		if ( isset($docinput['ZusammenfassungVon']))
		{
			$Zusammenfassung = array();
			$Datum = convertDatumToTimeStamp($docinput['ZusammenfassungVon']);
			$Datum2 = convertDatumToTimeStamp($docinput['ZusammenfassungBis']);
			$Buchungen = array();
			$query = mysql_query('SELECT * FROM cvjm_Kiosk WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$Datum2).'" ORDER BY Datum DESC');
			while ($row = mysql_fetch_array($query))
			{
				if ( ! isset($Zusammenfassung[$row['Bezeichnung']]))
				{
					$Zusammenfassung[$row['Bezeichnung']] = 0;
				}
				$Zusammenfassung[$row['Bezeichnung']] += $row['Menge'];
			}
			mysql_free_result($query);
			$Smarty->assign_by_ref('Zusammenfassung', $Zusammenfassung);
		}
		if ( isset($docinput['UmsatzVon']))
		{
			$Zusammenfassung = array();
			$Datum = convertDatumToTimeStamp($docinput['UmsatzVon']);
			$Datum2 = convertDatumToTimeStamp($docinput['UmsatzBis']);
			$Umsatz = 0;
			
			$query = mysql_query('SELECT * FROM cvjm_Kiosk WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$Datum2).'" ORDER BY Datum DESC');
			while ($row = mysql_fetch_array($query))
			{
				// Ausschließen der Geldbuchungen
				if ($row['F_Artikel_id'] != 0)
				{
					$Umsatz += $row['Menge']*$row['Preis'];
				}
								
			}
			mysql_free_result($query);
			$Smarty->assign('Umsatz', $Umsatz);
		}
		// Mitarbeiterlisten anzeigen
		if ( isset($docinput['Einzahlen']) && $docinput['Einzahlen'] )
		{
			// Einzahlung 
			// $Geld berechnen
			$sql = 'SELECT Sum(Menge*Preis) FROM cvjm_Kiosk_Mitarbeiter '. 
			'WHERE Benutzer="'.$docinput['Einzahlen'].'"';
			$query = mysql_query($sql);
			if ( $Geld = mysql_fetch_array($query))
			{
				$Geld = $Geld[0];
				// 1. alle Einträge nach Kiosk verschieben
				$sql = 'INSERT INTO cvjm_Kiosk SELECT * FROM cvjm_Kiosk_Mitarbeiter '.	 
				'WHERE Benutzer="'.$docinput['Einzahlen'].'"';
			
				if ( ! mysql_query($sql) ) 
				{
					throw new Exception('Fehler: '.mysql_error());
				}
			
				$sql = 'DELETE FROM cvjm_Kiosk_Mitarbeiter WHERE Benutzer="'.$docinput['Einzahlen'].'"';
				// 	2. Gegenzahlung in Kiosk Einbuchen

				if ( ! mysql_query($sql) )
				{
					throw new Exception('Fehler löschen: '.mysql_error());
				}
				$sql = 'INSERT INTO cvjm_Kiosk (Menge, Preis, Bezeichnung, Benutzer, Datum, F_Artikel_id) '.
				' VALUES (-1, '.$Geld.', "Einzahlung Mitarbeiter '.
				mysql_escape_string($docinput['Einzahlen']).'", "'.get_user_nickname().
				'","'.date('Y-m-d H:n').'", -1)';
			
				if ( ! mysql_query($sql))
				{
					throw new Exception('Fehler: '.mysql_error());
				}
			}
			mysql_free_result($query);
		}
		$sql = 'SELECT DISTINCT Benutzer FROM cvjm_Kiosk_Mitarbeiter';
		$Mitarbeiter = array();
		$query = mysql_query($sql);
		while ( $result = mysql_fetch_array($query) )
		{
			$Mitarbeiter[$result['Benutzer']] = $result['Benutzer'];
		}
		mysql_free_result($query);
		$Smarty->assign('Mitarbeiter', $Mitarbeiter);
		$sql = 'SELECT Sum(Menge*Preis) FROM cvjm_Kiosk_Mitarbeiter';
		$Mitarbeiter = array();
		$query = mysql_query($sql);
		$Mitarbeiterbetrag = 0;
		if ( $result = mysql_fetch_array($query) )
		{
			$Mitarbeiterbetrag = $result[0];
		}
		mysql_free_result($query);
		$Smarty->assign('Mitarbeiterbetrag', $Mitarbeiterbetrag);
		$Smarty->display('Kiosk.tpl');
	}
	else
	{
		require_once (INC_PATH.'cvjm/Artikel.class.php');

		$query = mysql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE Barcode LIKE "CVJM%" ORDER BY Bezeichnung');
		$Artikel = array();
		while ($row = mysql_fetch_row($query))
		{
			$Artikel[] = new Artikel($row[0]);
		}
		mysql_free_result($query);
		$Smarty->assign_by_ref('Artikelliste', $Artikel);
		$Smarty->display('Artikel_Barcodeliste.tpl');
	}
} // if not Editor
?>
