<?php
/**
 * Modul Buchung
 * Parameter:
 * - Buchung_Nr (-1 für neue Buchung)
 * - Adressen_id
 * - Save - Sichern der Daten (per docinput)
 *
 * AWF-Name: cvjmbuchung
 *
 * Vorlagen-Dateien müssen im Verzeichnis Vorlagen/Buchungen liegen
 *
 * 07.02.07 - Beginn der Umstellung auf Smarty-Templates
 *
 */
require_once (INC_PATH.'Smarty/Smarty.class.php');
$Smarty = new Smarty;
$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
include_once (INC_PATH . 'cvjm_ajax.php');
$xajax->printJavascript('./javascript');
include (INC_PATH . 'misc/CVJM.inc');
include (INC_PATH . 'misc/cvjmBuchungstools.inc');
include (INC_PATH . 'misc/cvjmArtikeltools.inc');
include (INC_PATH . 'cvjm/Buchung.class.php');
require_once(INC_PATH . 'cvjm/Artikel.class.php');
require_once(INC_PATH . 'cvjm/Event.class.php');
session_unregister('LetzteRechnung');
function SortierteZusammenfassungAnzeigen($eintraege, $abgerechnet)
{
	global $Smarty;
	global $document;
	global $Wochentage;
	global $profile;
	$Smarty->assign_by_ref('Artikelarten', Artikel::getArtikelArten());
	$Smarty->assign('Edit', isset($_REQUEST['Edit']));
	$Smarty->assign('Aenderbar', !$abgerechnet && $profile[$document['SpeicherRecht']] == '1');
	$Smarty->assign('Buchungsuebersichturl', get_url(get_cmodule_id('cvjmbuchungsuebersicht'), array (
	'Buchung_Nr' => $_REQUEST['Buchung_Nr'])));
	$Artikel = array ();
	foreach ($eintraege as $meinkey => $meinvalue)
	{
		$daten = array ();
		foreach ($meinvalue as $meinkey2 => $eintrag)
		{
		$daten[$meinkey2] = $eintrag['Datum'];
		}
		// Sortieren der Artikel einer Kategorie nach Datum
		reset($meinvalue);
		array_multisort($daten, SORT_ASC, SORT_NUMERIC, $meinvalue);
		$Artikel[$meinkey] = array();
		foreach ($meinvalue as $meinkey2 => $eintrag)
		{
			if (isset ($eintrag['LetztesDatum']))
			{
				// Anzeigedatum: wenn vorhanden, mit Zeit
				$datum = date('d.m.Y', $eintrag['Datum']);
				if (date('H:i', $eintrag['Datum']) != '00:00')
				$datum .= date(' H:i', $eintrag['Datum']);
					
				$eintrag['LetztesDatum'] = strtotime('-1 day', $eintrag['LetztesDatum']);
				// Datumsbereich bei Zusammenfassung
				$datumvonbis = $eintrag['Datum'];
				if ($eintrag['LetztesDatum'] > $eintrag['Datum'])
				{
					$datum .= ' bis ' . substr($Wochentage[date('w', $eintrag['LetztesDatum'])], 0, 2) . ' ' .
					date('d.m.Y', $eintrag['LetztesDatum']);
					$datumvonbis = $eintrag['Datum'] . '-' . $eintrag['LetztesDatum'];
				}
				$eintrag['DatumVonBis'] = $datumvonbis;
				$eintrag['DatumAnzeige'] = $datum;
				$wieviel = 0;
				$Anzeigen = $eintrag['Mengen'][0];
				foreach ($eintrag['Mengen'] as $key => $value)
				{
					$wieviel += $value;
					if ($Anzeigen != $value)
					$Anzeigen = '';
				}
				if ($Anzeigen != $eintrag['Mengen'][0])
				{
				$Anzeigen = implode(',', $eintrag['Mengen']);
				}
				// Menge insgesamt
				$eintrag['MengeAnzeigen'] = $Anzeigen;
				// Menge an einem Tag
				$eintrag['wieviel'] = $wieviel;
				$eintrag['Zuf'] = rand(0,100);
				// Falls die Bezeichnung geändert wurde
				if ( $eintrag['F_Bezeichnung'] != $eintrag['Bezeichnung'] && $eintrag['F_Bezeichnung'] != '')
				{
				$eintrag['Bezeichnung'] = stripslashes($eintrag['F_Bezeichnung']);
				}
				$wieviel = 0;
				$Anzeigen = $eintrag['Dauern'][0];
				foreach ($eintrag['Dauern'] as $key=>$value)
				{
					$wieviel += $value;
					if ($Anzeigen != $value)
					{
					$Anzeigen = '';
					}
				}
				$eintrag['Master'] = holeArtikelMasterParent($eintrag['F_Art_id'],$eintrag['Position'], $eintrag['Einruecken']);
				if ($Anzeigen != $eintrag['Dauern'][0])
				{
				$Anzeigen = implode(',', $eintrag['Dauern']);
				}
				$eintrag['DauerAnzeige'] = $Anzeigen;
				$a = new Artikel($eintrag['F_Artikel_Nr']);
				$eintrag['Schlafplaetze'] = $a->BerechneSchlafplaetze();
				if ($eintrag['Schlafplaetze']<0)
				{
					$eintrag['Schlafplaetze'] = '';
				}
				$eintrag['Zustaendig'] = '';
				$sql = 'SELECT DISTINCT PersonenArt, Person FROM '.TABLE_BUCHUNGPERSONEN.' WHERE F_Buchung_Nr='.$eintrag['F_Buchung_Nr'].
				' AND F_Artikel_Nr='.$eintrag['F_Artikel_Nr'].' AND Datum BETWEEN '.$eintrag['Datum'].' AND '.
				$eintrag['LetztesDatum'];
				if (! $query = mysql_query($sql)) echo mysql_error();
				while ( $zustaendig = mysql_fetch_array($query))
				{
					$eintrag['Zustaendig'].= $zustaendig['PersonenArt'].' '.$zustaendig['Person'].' ';
				}
				// Events der Buchung/Artikel holen
				$events = Event::searchBuchungEvent($eintrag['Datum'], $eintrag['LetztesDatum'],
				$eintrag['F_Buchung_Nr'], $eintrag['F_Artikel_Nr']);
				$name = array();
				foreach ($events as $event)
				{
					foreach ($event->Betroffene as $nr => $betroffener)
					{
						$wer = $event->getBetroffenenName($nr);
						if ( $wer != '' && ! in_array($wer, $name) )
						{
							$name[] = $wer;
						}
					}	
				}
				$eintrag['Zustaendig'] .= implode(',',$name);
				$Artikel[$meinkey][] = $eintrag;
			}
		} // foreach
	} // foreach
	$Smarty->assign_by_ref('Artikel', $Artikel);
	return $Smarty->fetch('Buchung_Artikeltabelle.tpl');
}

if (isset ($docinput['DelBuchung']) && is_numeric($docinput['DelBuchung']) &&
$profile[$document['SpeicherRecht']] == '1')
{
	// Einträge in die internen Bemerkungen speichern
	$Buchung = new Buchung($docinput['DelBuchung']);
	$Buchung->stornieren(!isset($docinput['Storno']));
	$docinput['Buchung_Nr'] = $Buchung->Buchung_Nr;
}
if (isset ($_REQUEST['Save']) && $profile[$document['SpeicherRecht']] == '1')
{
	if (!isset ($docinput['Buchung_Nr']) || ! is_numeric($docinput['Buchung_Nr']))
	{
		$docinput['Buchung_Nr'] = -1;
	}
	$Buchung = new Buchung($docinput['Buchung_Nr'], $docinput['F_Adressen_id']);
	// Daten speichern
	if ( isset($docinput['KuNr']) && is_numeric($docinput['KuNr']))
	{
		// Adressen-id aus Kundennummer suchen
		try
		{ 
			$A = Adresse::getKundePerKundennummer($docinput['KuNr']);
			$Buchung->KundeAendern($A->Adressen_id);
		}
		catch (Exception $e)
		{
		 
		}		
	}
	if (is_numeric($docinput['Abfahrtszeit']))
	{
		while (strlen($docinput['Abfahrtszeit']) < 4)
		{
			$docinput['Abfahrtszeit'] = '0' . $docinput['Abfahrtszeit'];
		}
		$docinput['Abfahrtszeit'] = substr($docinput['Abfahrtszeit'], 0, 2) . ':' .
		substr($docinput['Abfahrtszeit'], 2, 2);
	}
	if (is_numeric($docinput['Ankunftszeit']))
	{
		while (strlen($docinput['Ankunftszeit']) < 4)
		{
			$docinput['Ankunftszeit'] = '0' . $docinput['Ankunftszeit'];
		}
		$docinput['Ankunftszeit'] = substr($docinput['Ankunftszeit'], 0, 2) . ':' .
		substr($docinput['Ankunftszeit'], 2, 2);
	}

	if ($docinput['Abfahrtszeit'] != '')
	{
		$docinput['Abfahrtszeit'] = convertDatumToTimestamp($docinput['Bis'].' '.$docinput['Abfahrtszeit']); //strtotime($docinput['Abfahrtszeit']);
	}
	if ($docinput['Ankunftszeit'] != '')
	{
		$docinput['Ankunftszeit'] = convertDatumToTimestamp($docinput['Von'].' '.$docinput['Ankunftszeit']);
	}
	$docinput['Von'] = convertDatumToTimestamp($docinput['Von']);
        $docinput['Bis'] = convertDatumToTimestamp($docinput['Bis']);
	$docinput['Eingang'] = convertDatumToTimestamp($docinput['Eingang']);
        /*
	if ($docinput['EMailKorresponenz'] == 'v')
	{
		$docinput['EMailKorresponenz'] = 1;
	}
	else
	{
		$docinput['EMailKorresponenz'] = 0;
	}
	*/
	if (isset ($docinput['Kuechenhilfe']))
	{
		$docinput['Kuechenhilfe'] = 1;
	}
	else
	{
		$docinput['Kuechenhilfe'] = 0;
	}
	$docinput['Anzahlung'] = str_replace(',', '.',$docinput['Anzahlung']);
	$Buchung->uebertrageFelder($docinput);
	$Buchung->save();
	$_REQUEST['Buchung_Nr'] = $Buchung->Buchung_Nr;
	$docinput['Buchung_Nr'] = $Buchung->Buchung_Nr;
}
if (isset ($_REQUEST['Buchung_Nr']) && is_numeric($_REQUEST['Buchung_Nr']))
{
	// Statusänderungen
	$Buchung = new Buchung($_REQUEST['Buchung_Nr'], $_REQUEST['Adressen_id']);
	if ($profile[$document['SpeicherRecht']] == '1' && ! $Buchung->isNeu())
	{
		if (isset ($docinput['Reaktiv']) && is_numeric($docinput['Reaktiv']))
		{
			$Buchung->reaktivieren($docinput['Reaktiv']);
		}
		if (isset ($docinput['Seminar']) && is_numeric($docinput['Seminar']))
		{
			$Buchung->macheSeminar($docinput['Seminar']);
		}
		if (isset ($_REQUEST['DelEintrag']) )
		{
			// Eintrag entfernen
			list ($Artikelnr, $Datum) = explode(',', $_REQUEST['DelEintrag']);
			$Buchung->entbucheArtikel($Artikelnr, $Datum, 0);
		}
		if (isset ($_REQUEST['Menge']) )
		{
			// Eintragsmenge erhöhen
			$Richtung = $_REQUEST['Richtung'];
			$Datum = $_REQUEST['Datum'];
			$Artikelnr = $_REQUEST['Menge'];
			if ($Richtung == 'P')
			{
				$Buchung->bucheArtikel($Artikelnr, $Datum);
			}
			else
			{
				if ($Richtung == 'M')
				{
					$Buchung->entbucheArtikel($Artikelnr, $Datum);
				}
			}
		}
		// Abrechnung löschen
		if (isset ($docinput['DelAbrechnung']) && is_numeric($docinput['DelAbrechnung']))
		{
			$Buchung->loescheAbrechnung($docinput['DelAbrechnung']);
		}
		// Verschieben
		if (isset ($docinput['NeuBis']) && isset ($docinput['NeuVon']))
		{
			// Verschiebung der Buchung
			$docinput['NeuBis'] = convertDatumToTimeStamp($docinput['NeuBis']);
			$docinput['NeuVon'] = convertDatumToTimeStamp($docinput['NeuVon']);
			if ( is_numeric($docinput['NeuBis']) && is_numeric($docinput['NeuVon']))
			{
				if ( $Buchung->verschiebeBuchung($docinput['NeuVon'],$docinput['NeuBis']))
				{
					echo '<div class="Fehler">Der neue Buchungszeitraum unterscheidet sich vom alten!</div>';
				} else
				{
					echo '<div class="Achtung">Verschiebung der Buchung erfolgt.</div>';
				}
			}
			else
			{
				echo '<div class="SchwererFehler">Keine gültigen Daten für die Verschiebung angegeben!</div>';
			}
		}

		// Selbstverpflegung berücksichtigen
		if (isset ($docinput['Selbstverpflegung']) )
		{
			if ($docinput['Selbstverpflegung'] == 'J')
			{
				$tage = $Buchung->berechneUebernachtungen();
				for ($i = 0; $i <= $tage; $i++)
				{
					// Doppelte Einträge werden von der Datenbank abgewiesen
					sql_query('INSERT INTO ' . TABLE_BUCHUNGSEINTRAEGE .
					' (F_Buchung_Nr, F_Artikel_Nr, Menge, Datum) VALUES (' .
					$Buchung->Buchung_Nr . ',' . CVJM_SELBSTVERPFLEGUNG . ',1,' .
					strtotime("+$i day", $Buchung->Von) . ')');
				}
			}
			elseif ($docinput['Selbstverpflegung'] == 'N')
			{
				// Alle Einträge von Selbstverpflegung löschen
				if (!sql_query('DELETE FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' WHERE F_Buchung_Nr=' .
				$Buchung->Buchung_Nr . ' AND F_Artikel_Nr=' . CVJM_SELBSTVERPFLEGUNG))
				{
					echo mysql_error();
				}
			}
		} // Selbstverpflegung setzen / löschen

		// Selbstverpflegung feststellen
		if (isset ($_REQUEST['SaveEdit']) )
		{
			// Einträge speichern
			foreach ($docinput['Bezeichnung'] as $key => $value)
			{
				list ($Artikelnr, $datum) = explode(',', $key);
				list ($von, $bis) = explode('-', $datum);
				if ($bis == '')
				{
					$Datum = '='.$von;
				}
				else
				{
					$Datum = "BETWEEN $von AND $bis";
				}
				$sql = 'UPDATE ' . TABLE_BUCHUNGSEINTRAEGE . ' SET F_Bezeichnung="';
				$sql .= mysql_real_escape_string($value) . '"';
				if (isset ($docinput['Dauer'][$key]))
				{
					$docinput['Dauer'][$key] = str_replace(',', '.', $docinput['Dauer'][$key]);
					if (is_numeric($docinput['Dauer'][$key]))
					{
					$sql .= ',Dauer=' . $docinput['Dauer'][$key];
					}
				}
				if (isset ($docinput['Menge'][$key]))
				{
					$docinput['Menge'][$key] = str_replace(',', '.', $docinput['Menge'][$key]);
					if (is_numeric($docinput['Menge'][$key]))
					{
					$sql .= ',Menge=' . $docinput['Menge'][$key];
					}
				}
				if (isset($docinput['Unberechnet'][$key]))
				{
					$sql .= ',Unberechnet=1';
				}
				else
				{
					$sql .= ',Unberechnet=0';
				}
				$sql .= ' WHERE F_Artikel_Nr = '.$Artikelnr.' AND Datum '.$Datum.' AND ';
				$sql .= 'F_Buchung_Nr = ' . $Buchung->Buchung_Nr;
				if (!sql_query($sql))
				{
					echo 'Fehler '.$sql.': '. mysql_error();
				}
			}
			$Buchung->logAction('Gebuchte Artikel verändert');
		}
	} // alte Buchung und Speicherrecht
	$check1 = '';
	$check2 = '';
	if ($Buchung->isFertig()) // Abgerechnet / Storniert
	{
		$check1 = ' readonly="readonly"';
		$check2 = ' disabled="disabled"';
	}
	$Smarty->assign('id', $id);
	$Smarty->assign('check1', $check1);
	$Smarty->assign('check2', $check2);
	// Buchungs-Objekt
	$Smarty->assign_by_ref('Buchung', $Buchung);
	$Smarty->assign('Adressebearbeiten', get_url(get_cmodule_id('cvjmadressen')));
	$Smarty->assign('KundeAendern', isset ($_REQUEST['KuNrA']));

	$Smarty->assign('Speichern', $profile[$document['SpeicherRecht']] == 1 && ! $Buchung->isFertig() );
	$Smarty->assign('Verschieben', isset ($docinput['Verschieben']) && $profile[$document['SpeicherRecht']]);

	$query = sql_query('SELECT * FROM ' . TABLE_PREISLISTEN . ' WHERE (NOT Inaktiv) OR Preisliste_id='.
	$Buchung->F_Preisliste_id.' ORDER BY GueltigAb DESC');
	$Preislisten = array ();
	while ($preisliste = sql_fetch_array($query))
	{
		$Preislisten[$preisliste['Preisliste_id']] = $preisliste['Bezeichnung'];
		if ( $preisliste['Inaktiv']) $Preislisten[$preisliste['Preisliste_id']] .= ' (inaktiv)';
	}
	sql_free_result($query);
	$Smarty->assign('Preislisten_values', array_keys($Preislisten));
	$Smarty->assign('Preislisten_output', array_values($Preislisten));
	$Smarty->assign('Steuerarten_values', array_keys($Steuerarten));
	$Smarty->assign('Steuerarten_output', array_values($Steuerarten));
	// Einstellungen auf deutsch stellen, damit Datumsangaben entsprechend funktionieren

	$Smarty->assign('Buchungsuebersichturl', get_url(get_cmodule_id('cvjmbuchungsuebersicht')));
	$Smarty->assign('Personalplanungurl', get_url(get_cmodule_id('cvjmpersonalplanung')));
	$Smarty->assign('Seminarurl', get_url(get_cmodule_id('cvjmseminarteilnehmer')));
	$Smarty->assign('Abrechnungurl', get_url(get_cmodule_id('cvjmabrechnung')));
	if ( ! $Buchung->isNeu())
	{
		$sql = 'SELECT Rechnung_id, Rechnungsdatum, Rechnung, Rechnungsnummer FROM ' . TABLE_RECHNUNGEN .
		' WHERE F_Buchung_Nr = ' . $Buchung->Buchung_Nr . ' ORDER BY Rechnungsdatum, Rechnung_id';
		if (!$query = sql_query($sql))
		{
			echo "Fehler: $sql /" . mysql_error();
		}
		$Rechnungen = array();
		while ($rechnung = sql_fetch_array($query))
		{
			$Rechnungen[] = $rechnung;
		}
		$Smarty->assign_by_ref('Rechnungen', $Rechnungen);
		sql_free_result($query);
	}
	include (INC_PATH . 'misc/cvjmKorrespondenz.inc');
	ob_start();
	$Buchungsvorlagen = ZeigeVorlagen($Buchung->Buchung_Nr, 'Buchungen');
	$Buchungsvorlagen = array_merge($Buchungsvorlagen, ZeigeVorlagen($Buchung->Buchung_Nr, 'Kuechenzettel'));
	
	$Smarty->assign('Vorlagen', ob_get_contents());
	ob_end_clean();
	$Smarty->assign('Auftragurl', get_url(get_cmodule_id('auftrag')));
	$Smarty->assign('Buchungsbemerkungenurl', get_url(get_cmodule_id('cvjmbuchungsbemerkungen')));

	setlocale(LC_ALL, 'de_DE@euro', 'de_DE', 'de', 'ge');
	$Smarty->assign('spalten', 8);
	$Smarty->assign_by_ref('docinput', $docinput);
	// Einzelheiten
	if (! $Buchung->isNeu())
	{
		include (INC_PATH . 'misc/cvjmArtikelHinzu.inc');
		$Fehlerliste = ArtikelHinzufuegen(TABLE_BUCHUNGSEINTRAEGE);
		// Fehler beim Buchen anzeigen		
		$s = '';
		foreach ($Fehlerliste as $feld)
		{
			$s .= 'Artikel '.$feld['Bezeichnung'].' doppelt gebucht am '.date('d.m.Y',$feld['Datum']);
			if ( date('H:i',$datum) != '00:00' )
 	    	{
 	      		$s .= ' '.date('H:i',$datum);
	 	    }
	 	    $s .= ' von Buchung <a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$feld['Buchung_Nr']).
 	      '">'.$feld['Buchung_Nr'].'</a> ('.$feld['Menge'].' mal)<br />';
		}
		$Smarty->assign('Fehlerliste',$s);
		
		if ($Buchung->isFertig() )
		{
			unset ($_REQUEST['Edit']);
		}
		$params = array (	'Buchung_Nr' => $Buchung->Buchung_Nr	);
		if (isset ($_REQUEST['Edit']))
		{
			$params = array_merge(array ('Edit' => 1), $params);
		}
		if (isset ($_REQUEST['Alle']))
		{
			$Sort = 'F_Art_id, Datum, Bezeichnung';
		}
		else
		{
			$Sort = 'F_Art_id, Bezeichnung, F_Artikel_Nr,Datum';
		}
		$sql = 'SELECT * FROM ' . TABLE_BUCHUNGSEINTRAEGE .
		' INNER JOIN (' . TABLE_ARTIKEL . ' INNER JOIN ' . TABLE_ARTIKELARTEN .
		' ON F_Art_id=Art_id) ON id = F_Artikel_Nr' .
		' WHERE F_Buchung_Nr = ' . $Buchung->Buchung_Nr . ' ORDER BY '.$Sort;
		$query = sql_query($sql);
		$Art = -1;
		$Meineintrag['Datum'] = 0;
		$Meineintrag['LetztesDatum'] = 0;
		$Meineintrag['F_Artikel_Nr'] = 0;
		$Meineintrag['Mengen'][0] = 0;
		$Meineintrag['Dauern'][0] = 0;

		$AlleEintraege = array ();
		while ($eintrag = sql_fetch_array($query))
		{
			if ($Meineintrag['F_Artikel_Nr'] != $eintrag['F_Artikel_Nr'] || $Meineintrag['LetztesDatum'] != $eintrag['Datum'] || $_REQUEST['Alle'] == 1)
			{
				if ($Meineintrag['F_Artikel_Nr'] != 0)
				{
					$AlleEintraege[$Meineintrag['F_Art_id']][] = $Meineintrag;
				}
				// Zusammenfassen
				$Meineintrag = $eintrag;
				$Meineintrag['LetztesDatum'] = strtotime('+1 day', $Meineintrag['Datum']);
				$Meineintrag['Mengen'][0] = $eintrag['Menge'];
				$Meineintrag['Dauern'][0] = $eintrag['Dauer'];
			} else
			{
				$Meineintrag['LetztesDatum'] = strtotime('+1 day', $eintrag['Datum']);
				$Meineintrag['Mengen'][Count($Meineintrag['Mengen'])] = $eintrag['Menge'];
				$Meineintrag['Dauern'][Count($Meineintrag['Dauern'])] = $eintrag['Dauer'];
			}
		} // while

		if (sql_num_rows($query) == 0)
		{
			$Smarty->assign('eintraege', '<tr><td colspan="7" class="zentriert">keine Einträge vorhanden</td></tr>');
		}
		else
		{
			// den letzten Eintrag einfügen
			$AlleEintraege[$Meineintrag["F_Art_id"]][] = $Meineintrag;
			$Smarty->assign('eintraege', SortierteZusammenfassungAnzeigen($AlleEintraege, $Buchung->isFertig() ));
		}
		$Smarty->assign('Eintragsanzahl', sql_num_rows($query));
		sql_free_result($query);
		$Smarty->assign('Buchungsbemerkungenurl', get_url(get_cmodule_id("cvjmbuchungsbemerkungen")));

		ob_start();
		$kunden = $Buchung->bisherigeKunden();
		foreach ( $kunden as $KuNr)
		{
			$adresse = Adresse::getKundePerKundennummer($KuNr);
		  	ZeigeKorrespondenz(array('Adressen_id' => $adresse->Adressen_id,
		'Buchung_Nr'=>$Buchung->Buchung_Nr), array('*'), array (
		'Buchung_Nr' => $Buchung->Buchung_Nr
		));
		}
		$Smarty->assign('Korrespondenz', ob_get_contents());
		ob_end_clean();
		$Smarty->assign('params', feldToParams($params));
	} // buchung_nr > 0
	$Smarty->display('Buchung_Daten.tpl');

}
elseif ($profile["editor"] != 1)
{
	include (INC_PATH . "misc/cvjmBuchungstabelle.inc");

	$WhereSeminar = '';
	if (isset ($_REQUEST["Search"]) && trim($_REQUEST["Search"]) != '')
	{
		$Datum = convertDatumToTimestamp($_REQUEST["Search"]);
		if ($Datum != 0)
		{
			$Where = "(Von <= $Datum AND Bis >= $Datum) OR Eingang = " . $Datum;
		}
		elseif (is_numeric($_REQUEST["Search"]))
		{
			$Where = "PLZ = " . $_REQUEST["Search"];
			$Where .= " OR Buchung_Nr=" . $_REQUEST["Search"];
		} else
		{
			$Where = 'Internes REGEXP "' . mysql_real_escape_string($_REQUEST["Search"]) .
			'" OR Name REGEXP "' .
			mysql_real_escape_string($_REQUEST['Search']) . '" OR Zusatz REGEXP "' .
			mysql_real_escape_string($_REQUEST['Search']) . '" OR Ort REGEXP "' .
			mysql_real_escape_string($_REQUEST['Search']) . '"';
			$WhereSeminar = 'Name REGEXP "'. mysql_real_escape_string($_REQUEST['Search']) .
			'" OR Zusatz REGEXP "' .
			mysql_real_escape_string($_REQUEST['Search']) . '" OR Ort REGEXP "' .
			mysql_real_escape_string($_REQUEST['Search']) . '"';
		}
	} else
	{
		$Where = "Von >= " . mktime(0, 0, 0, date("m", time()), date("d", time()), date("Y", time())) .
		" OR Bis >= " . mktime(0, 0, 0, date("m", strtotime("-5 day")), date("d", strtotime("-5 day")), date("Y", strtotime("-5 day"))) .
		" OR BStatus < " . BUCHUNG_FERTIG;
	}
	BuchungstabelleAnzeigen($Where, false, $WhereSeminar);
}

if ($profile["editor"] == "1")
{
	create_editor_input("Recht für Veränderungen", "SpeicherRecht", "text_input");
}
?>
