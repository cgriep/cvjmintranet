<?php
/** Artikel hinzuf�gen
 * wird innerhalb einer n-spaltigen Tabelle verwendet.
 * dadurch wird die Ausrichtung an die vorher angezeigten Artikel gew�hrleistet
 * Die Suchanfrage steht in $docinput['ArtikelSearch']
 * @param Buchung $Buchung das Buchungsobjekt
 * @param int $spalten die Spaltenanzahl die angezeigt werden soll
 * @return int $Buchung_Nr - Buchungsnummer oder docinput[Rechnung_id]
 */
function anzeigeArtikelHinzufuegen($buchung, $spalten = 9)
{
	global $id;
	global $docinput;
	$Smarty = new Smarty();
	$Smarty->template_dir = TEMPLATEPATH; // '/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('id', $id);
	$Smarty->assign('spalten', $spalten);
	$Smarty->assign_by_ref('docinput', $docinput);
	$Smarty->assign_by_ref('Buchung', $buchung);
	$Smarty->display('Buchung_Artikel_hinzu.tpl');
}

/**
 *  tr�gt Artikel in die Datenbank ein
 * @param docinput[ArtikelSearch] - muss gesetzt sein
 * @param docinput[Menge] - numerisch, enth�lt die Menge
 *	@param docinput[Preis] - Preis ( nur Rechnungseintr�ge)
 * @param docinput[Rabatt] - Rabatt (nur Rechnungseintr�ge)
 * @param docinput[Rechnung_id] / REQUEST[Buchung_Nr] - die id
 * @return array die Liste mit den Fehlern beim Buchen
 */
function ArtikelHinzufuegen($tabelle)
{
	global $docinput;
	$l = array();
	if ( $tabelle == TABLE_RECHNUNGSEINTRAEGE )
	{
		// Buchungsnummer aus Rechnung holen
		$q = sql_query('SELECT F_Buchung_Nr FROM '.TABLE_RECHNUNGEN.' WHERE Rechnung_id='.$docinput['Rechnung_id']);
		if (! $b = sql_fetch_array($q) ) throw new Exception(sql_error());
		sql_free_result($q);
		$B = new Buchung($b['F_Buchung_Nr']);
	}
	elseif ( $tabelle = TABLE_BUCHUNGSEINTRAEGE )
	{
		$B = new Buchung($_REQUEST['Buchung_Nr']);
	}
	if ( isset($docinput['ArtikelSearch']) && isset($docinput['Menge']) )
	{
		foreach ( $docinput['Menge'] as $key => $value )
		{
			$value = str_replace(',','.',$value);
			if ( is_numeric($key) && is_numeric($value) && $value != 0)
			{
				if ( $tabelle == TABLE_RECHNUNGSEINTRAEGE )
				{					
					// Neuen Eintrag hinzuf�gen
					$sql = 'INSERT INTO '.TABLE_RECHNUNGSEINTRAEGE.
					' (F_Rechnung_id,F_Preis,F_Bezeichnung,Einheit,Rabatt,MWSt,Art,'.
					'Menge,Dauer,Datum,F_Artikel_Nr)'.
					' VALUES (';
					$sql .= $docinput['Rechnung_id'].',';
					$docinput['Preis'][$key] = str_replace(',','.',$docinput['Preis'][$key]);
					if ( is_numeric($docinput['Preis'][$key]) )
					{
						$sql .= $docinput['Preis'][$key].',';
					}
					else
					{
						$sql .= '0,';
					}
					//Einheit
					//Bezeichnung
					$q = sql_query('SELECT Bezeichnung,Einheit,Steuerpflicht,MWST,Art FROM ('.TABLE_ARTIKEL.
					' INNER JOIN '.TABLE_ARTIKELARTEN.' ON Art_id=F_Art_id) INNER JOIN '.
					TABLE_MWST.' ON F_MWSt=MWST_id WHERE id='.$key);
					$artikel = sql_fetch_array($q);
					sql_free_result($q);
					$sql .= "'".$artikel['Bezeichnung']."','".$artikel['Einheit']."',";
					$docinput['Rabatt'][$key] = str_replace(',','.',$docinput['Rabatt'][$key]);
					if ( ! is_numeric($docinput['Rabatt'][$key]) )
					{
						$docinput['Rabatt'][$key] = 0;
					}
					$sql .= $docinput['Rabatt'][$key].',';
					if (!$artikel['Steuerpflicht'])
					{
						$sql .= Min($B->Steuerbehandlung, $artikel['MWST']).',';
					}
					else
					{
						$sql .= $artikel['MWST'].',';
					}
					$sql .= "'".$artikel['Art']."',";
					$sql .= $value.',';
					$docinput['Dauer'][$key] = str_replace(',','.',$docinput['Dauer'][$key]);
					if ( is_numeric($docinput['Dauer'][$key] ))
					{
						$sql .= $docinput['Dauer'][$key].',';
					}
					else
					{
						$sql .= '0,';
					}
					$Datum = convertDatumToTimestamp($docinput['Datum'][$key]);
					if ( $docinput['EDatum'][$key] != '' )
					{
						$EDatum = convertDatumToTimestamp($docinput['EDatum'][$key]);
					}
					else
					{
						$EDatum = $Datum;
					}
					// Sicherheit: Maximal 30 Tage im Voraus
					if ( $EDatum > strtotime('+30 day', $Datum) )
					{
						$EDatum = strtotime('+30 day',$Datum);
					}
					for ( $i = $Datum; $i <= $EDatum; $i = strtotime('+1 day',$i))
					{
						if ( ! sql_query($sql.$i.','.$key.')'))
						{
							echo "Fehler $sql: ".sql_error();
						}
					}
				}
				elseif ( $tabelle = TABLE_BUCHUNGSEINTRAEGE )
				{
					$Datum = convertDatumToTimestamp($docinput['Datum'][$key]);

					if ( $docinput['EDatum'][$key] != '' )
					{
						$EDatum = convertDatumToTimestamp($docinput['EDatum'][$key]);
					}
					else
					{
						$EDatum = $Datum;
					}
					// Sicherheit: Maximal 30 Tage im Voraus
					if ( $EDatum > strtotime('+30 day', $Datum) )
					{
						$EDatum = strtotime('+30 day',$Datum);
					}
					$B->bucheArtikel($key, $Datum.'-'.$EDatum, $value, $docinput['Dauer'][$key],
					isset($docinput['Unberechnet'][$key]));
					$l = array_merge($l, $B->getFehlerliste());
				}
			}
		} // foreach
	} // wenn zus�tzliche Artikel angegeben
	return $l;
}

?>
