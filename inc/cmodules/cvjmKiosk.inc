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
		$Smarty->assignbyref('docinput', $docinput);
		// Geldbuchung
		if ( isset($docinput['Geld']))
		{
			$docinput['Geld'] = str_replace(',','.',$docinput['Geld']);
			if ( is_numeric($docinput['Geld']))
			{
			  $sql = 'INSERT INTO cvjm_Kiosk (Menge,Preis,Bezeichnung,Benutzer) VALUES (1,'. $docinput['Geld'].',"'.
			    'Geldbuchung Intranet","'.get_user_nickname().'")';
			   sql_query($sql);
			}
		}
		if ( isset($docinput['Mitarbeiter']) && $docinput['Mitarbeiter'] != '' )
		{	
			$sql = 'SELECT * FROM cvjm_Kiosk_Mitarbeiter WHERE Benutzer="'.sql_escape_string($docinput['Mitarbeiter']).'"';
			$query = sql_query($sql);
			$Mitarbeiter = array();
			while ($row = sql_fetch_array($query))
			{
				$Mitarbeiter[] = $row;
			}
			sql_free_result($query);
			$Smarty->assign('Mitarbeiterliste', $Mitarbeiter);
		}
		
		$query = sql_query('SELECT Sum(Preis*Menge) FROM cvjm_Kiosk');
		$row = sql_fetch_row($query);
		sql_free_result($query);
		$Smarty->assign('Kassenstand', $row[0]);

		if ( isset($docinput['TageVon']))
		{
			$Datum = convertDatumToTimeStamp($docinput['TageVon']);
			$Datum2 = convertDatumToTimeStamp($docinput['TageBis']);
			$Buchungen = array();
			$query = sql_query('SELECT Art, Sum(Menge*Preis) AS Umsatz FROM cvjm_Kiosk '.
			'LEFT JOIN cvjm_Artikel ON F_Artikel_id=id '.
			'LEFT JOIN cvjm_Artikelarten ON F_Art_id=Art_id '.
			'WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$Datum2).
			'" GROUP BY Art');
			while ($row = sql_fetch_array($query))
			{
				$Arten[] = $row;
			}
			sql_free_result($query);
			$Smarty->assignbyref('Arten', $Arten);
		}
		if ( isset($docinput['AnzeigeVon']))
		{
			$Datum = convertDatumToTimeStamp($docinput['AnzeigeVon']);
			$DatumP1 = strtotime('+1day',$Datum);
			$Buchungen = array();
			$sql = 'SELECT * FROM cvjm_Kiosk WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$DatumP1).'" ORDER BY Datum DESC';
			$query = sql_query($sql);
			while ($row = sql_fetch_array($query))
			{
				$Buchungen[] = $row;
			}
			sql_free_result($query);
			$Smarty->assignbyref('Buchungen', $Buchungen);
		}
		if ( isset($docinput['ZusammenfassungVon']))
		{
			$Zusammenfassung = array();
			$Datum = convertDatumToTimeStamp($docinput['ZusammenfassungVon']);
			$Datum2 = convertDatumToTimeStamp($docinput['ZusammenfassungBis']);
			$Buchungen = array();
			$query = sql_query('SELECT * FROM cvjm_Kiosk WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$Datum2).'" ORDER BY Datum DESC');
			while ($row = sql_fetch_array($query))
			{
				if ( ! isset($Zusammenfassung[$row['Bezeichnung']]))
				{
					$Zusammenfassung[$row['Bezeichnung']] = 0;
				}
				$Zusammenfassung[$row['Bezeichnung']] += $row['Menge'];
			}
			sql_free_result($query);
			$Smarty->assignbyref('Zusammenfassung', $Zusammenfassung);
		}
		if ( isset($docinput['UmsatzVon']))
		{
			$Zusammenfassung = array();
			$Datum = convertDatumToTimeStamp($docinput['UmsatzVon']);
			$Datum2 = convertDatumToTimeStamp($docinput['UmsatzBis']);
			$Umsatz = 0;
			
			$query = sql_query('SELECT * FROM cvjm_Kiosk WHERE Datum BETWEEN "'.date('Y-m-d',$Datum).'" AND "'.date('Y-m-d',$Datum2).'" ORDER BY Datum DESC');
			while ($row = sql_fetch_array($query))
			{
				// Ausschließen der Geldbuchungen
				if ($row['F_Artikel_id'] != 0)
				{
					$Umsatz += $row['Menge']*$row['Preis'];
				}
								
			}
			sql_free_result($query);
			$Smarty->assign('Umsatz', $Umsatz);
		}
		// Mitarbeiterlisten anzeigen
		if ( isset($docinput['Einzahlen']) && $docinput['Einzahlen'] )
		{
			// Einzahlung 
			// $Geld berechnen
			$sql = 'SELECT Sum(Menge*Preis) FROM cvjm_Kiosk_Mitarbeiter '. 
			'WHERE Benutzer="'.$docinput['Einzahlen'].'"';
			$query = sql_query($sql);
			if ( $Geld = sql_fetch_array($query))
			{
				$Geld = $Geld[0];
				// 1. alle Einträge nach Kiosk verschieben
				$sql = 'INSERT INTO cvjm_Kiosk SELECT * FROM cvjm_Kiosk_Mitarbeiter '.	 
				'WHERE Benutzer="'.$docinput['Einzahlen'].'"';
			
				if ( ! sql_query($sql) ) 
				{
					throw new Exception('Fehler: '.sql_error());
				}
			
				$sql = 'DELETE FROM cvjm_Kiosk_Mitarbeiter WHERE Benutzer="'.$docinput['Einzahlen'].'"';
				// 	2. Gegenzahlung in Kiosk Einbuchen

				if ( ! sql_query($sql) )
				{
					throw new Exception('Fehler löschen: '.sql_error());
				}
				$sql = 'INSERT INTO cvjm_Kiosk (Menge, Preis, Bezeichnung, Benutzer, Datum, F_Artikel_id) '.
				' VALUES (-1, '.$Geld.', "Einzahlung Mitarbeiter '.
				sql_escape_string($docinput['Einzahlen']).'", "'.get_user_nickname().
				'","'.date('Y-m-d H:n').'", -1)';
			
				if ( ! sql_query($sql))
				{
					throw new Exception('Fehler: '.sql_error());
				}
			}
			sql_free_result($query);
		}
		$sql = 'SELECT DISTINCT Benutzer FROM cvjm_Kiosk_Mitarbeiter';
		$Mitarbeiter = array();
		$query = sql_query($sql);
		while ( $result = sql_fetch_array($query) )
		{
			$Mitarbeiter[$result['Benutzer']] = $result['Benutzer'];
		}
		sql_free_result($query);
		$Smarty->assign('Mitarbeiter', $Mitarbeiter);
		$sql = 'SELECT Sum(Menge*Preis) FROM cvjm_Kiosk_Mitarbeiter';
		$Mitarbeiter = array();
		$query = sql_query($sql);
		$Mitarbeiterbetrag = 0;
		if ( $result = sql_fetch_array($query) )
		{
			$Mitarbeiterbetrag = $result[0];
		}
		sql_free_result($query);
		$Smarty->assign('Mitarbeiterbetrag', $Mitarbeiterbetrag);
		$Smarty->display('Kiosk.tpl');
	}
	else
	{
		require_once (INC_PATH.'cvjm/Artikel.class.php');

		$query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE Barcode LIKE "CVJM%" ORDER BY Bezeichnung');
		$Artikel = array();
		while ($row = sql_fetch_row($query))
		{
			$Artikel[] = new Artikel($row[0]);
		}
		sql_free_result($query);
		$Smarty->assignbyref('Artikelliste', $Artikel);
		$Smarty->display('Artikel_Barcodeliste.tpl');
	}
} // if not Editor
?>
