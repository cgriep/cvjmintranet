<?php

/**
 * 
 *   Modul cvjmAbrechnung
 *  AWF-Name: cvjmabrechnung
*
*   Vorlagen-Dateien müssen im Verzeichnis Vorlagen/Buchung liegen
*
 *  Parameter:
 *  docinput[Buchung_Nr] - Die Buchungsnummer
 *  Storno: Status bei Stornierungen. Zunächst wird eine Abrechnung gemacht, danach der
 *          neue Status gesetzt und die BuchungsEinträge gelöcht.
*/

include (INC_PATH . 'cvjm_ajax.php');
require_once (INC_PATH . 'cvjm/Buchung.class.php');
require_once (INC_PATH . 'cvjm/Artikel.class.php');
$xajax->printJavascript('./javascript');

function AufRechnung($rechnung_nr, $buchung_nr, $artikel_id, $datum)
{
	// Bestimmt, ob ein Eintrag auf einer Rechnung der Buchung vorhanden ist
	$query = sql_query('SELECT Menge FROM ' . TABLE_RECHNUNGSEINTRAEGE . ' INNER JOIN ' .
	TABLE_RECHNUNGEN . ' ON F_Rechnung_id=Rechnung_id WHERE F_Buchung_Nr=' . $buchung_nr .
	" AND F_Artikel_Nr=$artikel_id AND Datum=$datum AND Rechnung_id <> $rechnung_nr AND " .
	'Rechnung=1 AND Storno=0');
	$anzahl = NULL;
	while ($eintrag = sql_fetch_row($query))
	{
		$anzahl += $eintrag[0];
	}
	sql_free_result($query);
	return $anzahl;
}

function entferneBerechneteEintraege($Rechnung_id, $Buchung_nr)
{

	$query = sql_query('SELECT * FROM ' . TABLE_RECHNUNGSEINTRAEGE . " WHERE F_Rechnung_id=$Rechnung_id");
	echo "Bereinige $Rechnung_id auf Buchung $Buchung_nr...<br />";
	while ($eintrag = sql_fetch_array($query))
	{
		$anzahl = AufRechnung($Rechnung_id, $Buchung_nr, $eintrag['F_Artikel_Nr'], $eintrag['Datum']);
		if (!is_null($anzahl))
		{
			echo $eintrag['F_artikel_Nr'] . ": $anzahl vorh, " . $eintrag['Menge'] . ' auf Rechnung<br />';
			if ($eintrag['Menge'] - $anzahl == 0)
			{
				// Eintrag vorhanden - löschen
				if (!sql_query('DELETE FROM ' . TABLE_RECHNUNGSEINTRAEGE .
					" WHERE F_Rechnung_id=$Rechnung_id AND F_Artikel_Nr=" . $eintrag['F_Artikel_Nr'] .
					' AND Datum=' . $eintrag['Datum']))
					echo sql_error();
			} else
			{
				// Eintrag vorhanden - ändern
				if (!sql_query('UPDATE ' . TABLE_RECHNUNGSEINTRAEGE .
					' SET Menge=' . ($eintrag['Menge'] - $anzahl) .
					" WHERE F_Rechnung_id=$Rechnung_id AND F_Artikel_Nr=" . $eintrag['F_Artikel_Nr'] .
					' AND Datum=' . $eintrag['Datum']))
					echo sql_error();
			}
		}
	}
	sql_query('UPDATE ' . TABLE_BUCHUNGEN . ' SET Logtext=CONCAT("' . date('d.m.Y H:i') .
	' Abrechnung bereinigt (Splitrechnung) ' . get_user_nickname() . '\n",Logtext)' .
	' WHERE Buchung_Nr=' . $Buchung_nr);
	sql_free_result($query);
}

function ergaenzeFreibetraege($eintrag, $eintraege, $nullMenge = '')
{
	// Sonderfall: Uebernachtungen am ersten Tag.
	if (is_numeric($nullMenge) && $nullMenge == 0)
		$eintrag['Menge'] = 0;
	if ($eintrag['Menge'] != 0 && ($eintrag['Typ'] == ABRECHNUNGSTYP_KOPFNACHT || $eintrag['Typ'] == ABRECHNUNGSTYP_KOPF || $eintrag['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNGNACHT || $eintrag['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNGPAUSCHAL || $eintrag['Typ'] == ABRECHNUNGSTYP_PAUSCHAL))
	{
		// Datum allgemein: es ist egal wann die zugehörigen Dinge gebucht sind
		$eintrag['Datum'] = 0;
	}
	$query = sql_query('SELECT * FROM ' . TABLE_ARTIKELBEZIEHUNGEN .
	' WHERE F_Artikel_id=' . $eintrag['id']);
	while ($artikel = sql_fetch_array($query))
	{
		// freie Menge festlegen
		if (!isset ($eintraege[$eintrag['Datum']][$artikel['F_Unterartikel_id']]))
		{
			$eintraege[$eintrag['Datum']][$artikel['F_Unterartikel_id']] = array ();
			$eintraege[$eintrag['Datum']][$artikel['F_Unterartikel_id']]['Frei'] = 0;
		}
		// Zu Debug-Zwecken
		/*
		echo 'FREI ('.$artikel['F_Unterartikel_id'].') ';
		if ( $eintrag['datum'] == 0 ) echo 'EGAL WANN';
		echo ': '.$eintrag['Menge'].'/'.$artikel['Menge'];
		*/
		// Ende Debug
		$eintraege[$eintrag['Datum']][$artikel['F_Unterartikel_id']]['Frei'] += $artikel['Menge'] * $eintrag['Menge'];
		$eintraege = ergaenzeFreibetraege(array (
			'id' => $artikel['F_Unterartikel_id'],
			'Datum' => $eintrag['Datum'],
			'Menge' => ($eintrag['Menge'] * $artikel['Menge']
		)), $eintraege);
	}
	sql_free_result($query);
	return $eintraege;
}

function EintragAufnehmen($eintrag, $buchung, $Abrechnungseintraege)
{
	global $MWST;
	global $Schlaforte;
	if (!isset ($Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]))
	{
		$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']] = array ();
		//$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Frei'] = 0;
	}
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Bezeichnung'] = $eintrag['Bezeichnung'];
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Menge'] = $eintrag['Menge'];
	// unterscheidung: Welche Steuer muss ausgewiesen werden?     
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['MWST'] = $MWST[$eintrag['F_MWSt']];
	// Alles was nicht geringwertig ist kann ermäßigt werden
	if (!$eintrag['Steuerpflicht'])
	{
		$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['MWST'] = Min($buchung['Steuerbehandlung'], $MWST[$eintrag['F_MWSt']]);
	}
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Einheit'] = $eintrag['Einheit'];
	// Rabatt nur wenn Artikel rabattfaehig!
	if ($eintrag['Rabattfaehig'])
	{
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Rabatt'] = $buchung['Buchungsrabatt'];
	}
	else
	{
		$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Rabatt'] = 0;
	}
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Art'] = $eintrag['Art'];
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Typ'] = $eintrag['Typ'];
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Dauer'] = $eintrag['Dauer'];
	$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Preis'] = holePreis($eintrag['id'], $buchung['F_Preisliste_id'], $eintrag['Dauer'] > 0);
	$neuMenge = $eintrag['Menge'];
	// Sonderfälle prüfen
	// Schritt 1: Essen?
	if ($eintrag['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNG || $eintrag['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNGNACHT || $eintrag['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNGPAUSCHAL)
	{
		// Pro Kopf und Altersgruppe
		if (PersonenAnzahl($buchung) != $eintrag['Menge'])
		{
			// zusätzliches Essen
			echo '<div class="Fehler">';
			echo date('d.m.Y', $eintrag['Datum']) . ': ';
			echo 'Essensanzahl ' . $eintrag['Bezeichnung'] .
			' (' . $eintrag['Menge'] . ' St&uuml;ck) weicht von der gebuchten Personenanzahl (' .
			PersonenAnzahl($buchung) . ') ab!</div>';
		}
		// Wenn erste Nacht dann Menge = 0 setzen
		if ($eintrag['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNGNACHT && (!in_array($eintrag['id'], $Schlaforte) || !isset ($Abrechnungseintraege[strtotime('-1 day', $eintrag['Datum'])][$eintrag['id']])))
		{
			// Menge = 0 bedeutet Preis wird angezeigt
			$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Menge'] = 0;
			$neuMenge = 0;
			$Schlaforte[] = $eintrag['id'];
		}
		if ($eintrag['id'] != CVJM_SELBSTVERPFLEGUNG && $neuMenge > 0)
		{
			// prüfen, ob Pauschale Vegetarisch anfällt
			if ($buchung['Vegetarier'] > 0 && !isset ($Abrechnungseintraege[$eintrag['Datum']][CVJM_VEGETARIER]))
			{
				// Eintrag bauen:
				$q = sql_query('SELECT * FROM ' . TABLE_ARTIKEL . ' INNER JOIN ' .
				TABLE_ARTIKELARTEN . ' ON F_Art_id=Art_id WHERE id=' . CVJM_VEGETARIER);
				$vegetarier = sql_fetch_array($q);
				sql_free_result($q);
				$vegetarier['Menge'] = $buchung['Vegetarier'];
				$vegetarier['Datum'] = $eintrag['Datum'];
				$vegetarier['Dauer'] = 0;
				$Abrechnungseintraege = EintragAufnehmen($vegetarier, $buchung, $Abrechnungseintraege);
			}
			// prüfen, ob Pauschale Schweinefleischlos anfällt
			if ($buchung['Schweinelos'] > 0 && !isset ($Abrechnungseintraege[$eintrag['Datum']][CVJM_SCHWEINEFLEISCHLOS]))
			{
				// Eintrag bauen:
				$q = sql_query('SELECT * FROM ' . TABLE_ARTIKEL . ' INNER JOIN ' .
				TABLE_ARTIKELARTEN . ' ON F_Art_id=Art_id WHERE id=' . CVJM_SCHWEINEFLEISCHLOS);
				$vegetarier = sql_fetch_array($q);
				sql_free_result($q);
				$vegetarier['Menge'] = $buchung['Schweinelos'];
				$vegetarier['Datum'] = $eintrag['Datum'];
				$vegetarier['Dauer'] = 0;
				$Abrechnungseintraege = EintragAufnehmen($vegetarier, $buchung, $Abrechnungseintraege);
			}
			// prüfen, ob Pauschale 'ohne Hilfe' anfällt
			if (!$buchung['Kuechenhilfe'] && !isset ($Abrechnungseintraege[$eintrag['Datum']][CVJM_OHNEMITHILFE]))
			{
				// nur wenn noch keine Hilfe an diesem Tag
				if (!$q = sql_query('SELECT * FROM ' . TABLE_ARTIKEL . ' INNER JOIN ' .
					TABLE_ARTIKELARTEN .
					' ON F_Art_id=Art_id WHERE id=' . CVJM_OHNEMITHILFE))
					echo sql_error();
				$hilfe = sql_fetch_array($q);
				sql_free_result($q);
				$hilfe['Datum'] = $eintrag['Datum'];
				$hilfe['Menge'] = $eintrag['Menge'];
				$hilfe['Dauer'] = 0;
				$Abrechnungseintraege = EintragAufnehmen($hilfe, $buchung, $Abrechnungseintraege);
			}
		} // nicht Selbstverpflegung
	} // wenn Verpflegung
	// Schritt 2: Programm?
	elseif ($eintrag['Typ'] == ABRECHNUNGSTYP_KOPF)
	{
		// Pro Kopf-Bezahlung
		// Preis anpassen !!!!
		if ($eintrag['Menge'] != PersonenAnzahl($buchung))
		{
			echo '<div class="Fehler">';
			echo date('d.m.Y', $eintrag['Datum']) . ': ';
			echo 'Personenanzahl (' . $eintrag['Menge'] . ') bei ' . $eintrag['Bezeichnung'] .
			' weicht von der gebuchten Personenzahl (' . PersonenAnzahl($buchung) . ') ab!</div>';
		}
	}
	// Schritt 3 & 4: Ort -> Uebernachtung?
	elseif ($eintrag['Typ'] == ABRECHNUNGSTYP_NACHT || $eintrag['Typ'] == ABRECHNUNGSTYP_KOPFNACHT)
	{
		// Uebernachtungen!
		// wenn Schlafplatz >0 sonst normal!
		$Artikel = new Artikel($eintrag['id']);
		$betten = $Artikel->berechneSchlafplaetze(); //	BerechneSchlafplaetze($eintrag['id']);
		if ($betten >= 0 || $eintrag['Typ'] == ABRECHNUNGSTYP_KOPFNACHT || ($eintrag['Typ'] == ABRECHNUNGSTYP_NACHT && $eintrag['F_Art_id'] != CVJMART_ORT))
		{
			// der erste Tag wird nicht berechnet, weil in Uebernachtungen
			// gerechnet wird.
			if (!in_array($eintrag['id'], $Schlaforte) || !isset ($Abrechnungseintraege[strtotime('-1 day', $eintrag['Datum'])][$eintrag['id']]))
			{
				//$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Frei'] +=
				// Menge = 0 bedeutet Preis wird angezeigt
				$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Menge'] = 0;
				$neuMenge = 0;
				$Schlaforte[] = $eintrag['id'];
			}
		}
		if ($eintrag['Typ'] == ABRECHNUNGSTYP_KOPFNACHT)
		{
			if ($eintrag['Menge'] < PersonenAnzahl($buchung))
			{
				echo '<div class="Fehler">';
				echo date('d.m.Y', $eintrag['Datum']) . ': ';
				echo 'Personenzahl (' . $eintrag['Menge'] . ') bei ' . $eintrag['Bezeichnung'] .
				' weicht von der gebuchten Personenzahl (' . PersonenAnzahl($buchung) . ') ab!</div>';
			}
		} else
		{
			// Speisesaal und Aufenthaltsraum werden nicht berechnet
			if ($eintrag['id'] == $buchung['F_Speiseraum_id'] || $eintrag['id'] == $buchung['F_Aufenthaltsraum_id'])
				$Abrechnungseintraege[$eintrag['Datum']][$eintrag['id']]['Preis'] = 0;
		}
	}
	// Freibeträge aufnehmen
	$Abrechnungseintraege = ergaenzeFreibetraege($eintrag, $Abrechnungseintraege, $neuMenge);
	return $Abrechnungseintraege;
}

function ueberarbeiteEintraege($rechnung_id, $buchung, $Abrechnungseintraege)
{
	global $Alterswerte;
	global $Schlaforte;
	//Debug
	//print_r($Abrechnungseintraege);
	foreach ($Abrechnungseintraege as $datum => $artikel)
	{ // Debug: 
		//echo date('d.m.Y H:i',$datum).'<br />';
		foreach ($artikel as $artikel_id => $value)
		{
			// Freibeträge verrechnen
			if (isset ($value['Menge']))
			{
				// Debug
				//echo date('d.m.Y',$datum)." ($artikel_id):".$value['Menge'].'/'.$value['Frei'].'<br />';
				$Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] = $value['Menge'] - $value['Frei'];
				if (!isset ($Abrechnungseintraege[$datum][$artikel_id]['Freieintrag']))
				{
					$Abrechnungseintraege[$datum][$artikel_id]['Freieintrag'] = (is_numeric($value['Frei']) && $value['Frei'] >= 0) || isset ($Abrechnungseintraege[0][$artikel_id]['Frei']);
				}
				if ($Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] > 0)
				{
					// gibt es FreiEinträge, bei denen der Abrechnungszeitraum egal ist?
					if (isset ($Abrechnungseintraege[0][$artikel_id]['Frei']))
					{
						$frei = $Abrechnungseintraege[0][$artikel_id]['Frei'];
						// Debug
						//echo 'allgemein frei '.$frei.'!<br />';
						if ($frei <= $Abrechnungseintraege[$datum][$artikel_id]['Berechnen'])
						{
							//Debug
							//echo 'FREI:'.$Abrechnungseintraege[$datum][$artikel_id]['Berechnen'].'/'.$frei.'//'; 
							$Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] -= $frei;
							unset ($Abrechnungseintraege[0][$artikel_id]['Frei']);
							unset ($Abrechnungseintraege[0][$artikel_id]);
						} else
						{
							$Abrechnungseintraege[0][$artikel_id]['Frei'] -= $Abrechnungseintraege[$datum][$artikel_id]['Berechnen'];
							$Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] = 0;
						}
					}
				}
				if ($value['Menge'] == 0 && $value['Frei'] == 0)
					$Abrechnungseintraege[$datum][$artikel_id]['Uebernachtung'] = true;
				else
					$Abrechnungseintraege[$datum][$artikel_id]['Uebernachtung'] = false;
				if ($Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] < 0)
					$Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] = 0;
			}
			if ($value['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNG || $value['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNGNACHT || $value['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNGPAUSCHAL)
			{

				// Pro Kopf und Altersgruppe
				$berechnen = $Abrechnungseintraege[$datum][$artikel_id]['Berechnen'];
				$menge = $Abrechnungseintraege[$datum][$artikel_id]['Menge'];
				//Debug
				//echo 'Verpflegung '.$artikel_id.':'.$berechnen.'/'.$menge.'<br />';
				for ($i = 1; $i <= Count($Alterswerte); $i++)
				{
					$anz = $buchung['Personen' . $i] + $buchung['Personen' . $i . 'w'];
					if ($anz > 0 && $menge > 0)
					{
						if ($anz > $menge)
							$anz = $menge;
						$artid = holeUnterArtikelID($artikel_id, $Alterswerte[$i] . ' Jahre');
						if ($artid > 0)
						{
							// Neuer Eintrag
							$Abrechnungseintraege[$datum][$artid] = $value;
							// debug
							//echo $artid.' '.$value['Menge'].' '.$berechnen.' '.$anz."/";
							if ($anz > $berechnen)
							{
								$Abrechnungseintraege[$datum][$artid]['Berechnen'] = $berechnen;
								$berechnen = 0;
							} else
							{
								$Abrechnungseintraege[$datum][$artid]['Berechnen'] = $anz;
								$berechnen -= $anz;
							}
							$Abrechnungseintraege[$datum][$artid]['Menge'] = $anz;
							$Abrechnungseintraege[$datum][$artid]['Bezeichnung'] = $value['Bezeichnung'] .
							' ' . $Alterswerte[$i] . ' Jahre';
							$Abrechnungseintraege[$datum][$artid]['Preis'] = holePreis($artid, $buchung['F_Preisliste_id']);
						} else
							echo '<div class="Fehler">&gt;&gt;&gt; Altersartikel zu <a href="' .
							get_url(get_cmodule_id('cvjmartikel'), 'docinput[Artikel_Nr]=' . $artikel_id) .
							'">' . $value['Bezeichnung'] . '</a> f&uuml;r Altergruppe ' .
							$Alterswerte[$i] . ' nicht gefunden!</div>';
						$menge -= $anz;
					}
				} // for
				//Debug
				//echo 'Ende: '.$berechnen.'/'.$Abrechnungseintraege[$datum][$artikel_id]['Berechnen'].'<br />';
				if ($berechnen != $Abrechnungseintraege[$datum][$artikel_id]['Berechnen'])
					$Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] = $berechnen;
				// wenn Essen abgearbeitet, dann weg
				if ($Abrechnungseintraege[$datum][$artikel_id]['Berechnen'] <= 0)
					unset ($Abrechnungseintraege[$datum][$artikel_id]);
			}
		} // foreach
	}
	// Nun noch alle Einträge in die Datenbank schreiben
	$gesamt = 0;
	// TODO: Wofür ist $vorhanden???
	$vorhanden = 0;
	foreach ($Abrechnungseintraege as $datum => $artikel)
		foreach ($artikel as $artikel_id => $value)
		{
			if (isset ($value['Menge'])) // die reinen 'frei'-Artikel ausschalten
			{
				if ($value['Berechnen'] > 0 || (!$value['Freieintrag'] && $value['Uebernachtung']))
				{
					$sql = 'INSERT INTO ' . TABLE_RECHNUNGSEINTRAEGE .
						' (F_Rechnung_id, Datum,MWSt,Rabatt,Art,Einheit,Menge,Dauer,' .
						'F_Bezeichnung,F_Preis,F_Artikel_Nr) VALUES (' .
						$rechnung_id . ',' . $datum . ',' . $value['MWST'] . ',' . $value['Rabatt'] .
						',"' . $value['Art'] . '","' . sql_real_escape_string($value['Einheit']) . '",' . str_replace(',','.',$value['Berechnen'] - $vorhanden) .
						',' . $value['Dauer'] . ',"' . sql_real_escape_string($value['Bezeichnung']) . '",' .
						$value['Preis'] . ',' . $artikel_id . ')';
					if (!sql_query($sql))
						echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler 1:' . sql_error() . "/".$sql."</div>";
					$gesamt += berechnePreis($value['Preis'], $value['Berechnen'], $value['Dauer'], $value['Rabatt']);
				}
				// wenn teilweise Berechnung
				if (($value['Menge'] != $value['Berechnen'] && $value['Menge'] != 0) || ($value['Berechnen'] == 0 && $value['Freieintrag'])) //&& ! $value['Uebernachtung']) )
				{
					// die überzähligen Artikel werden mit Preis 0 eingetragen
					// Uhrzeit ist dann 00:00:01
					if (!sql_query('INSERT INTO ' . TABLE_RECHNUNGSEINTRAEGE .
						' (F_Rechnung_id, Datum,MWSt,Rabatt,Art,Einheit,Menge,Dauer,' .
						'F_Bezeichnung,F_Preis,F_Artikel_Nr) VALUES (' .
						$rechnung_id . ',' . ($datum +1) . ',' . $value['MWST'] . ',' . $value['Rabatt'] .
						",'" . $value['Art'] . "','" . sql_real_escape_string($value['Einheit']) . "'," . str_replace(',','.',$value['Menge'] - $value['Berechnen']) .
						',' . $value['Dauer'] . ",'" . sql_real_escape_string($value['Bezeichnung']) . "',0," . $artikel_id . ')'))
						echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler 2:' . sql_error() . "</div>";
				}
			}
		}
	return $gesamt;
}

function EintragBearbeiten($sql, $buchung, $eintrag)
{
	global $Alterswerte;
	global $Abgerechnet;
	global $Schlaforte;
	global $Artikelarten;
	global $docinput;
	global $MWST;
	global $gesamt;

	$mySql = $sql . $eintrag['Datum'] . ',' . $MWST[$eintrag['F_MWSt']] . ',';
	// Rabatt nur wenn Artikel rabattfaehig!
	if ($eintrag['Rabattfaehig'])
	{
	$mySql .= $buchung['Buchungsrabatt'] . ',';
	}
	else
	{
		$mySql .= '0,';
	}
	$mySql .= "'" . $eintrag['Art'] . "','" . sql_real_escape_string($eintrag['Einheit']) . "',";
	// &Uuml;bertragen des Eintrages nach Rechnungseintr&auml;ge
	// Schritt 1: Essen?
	if ($eintrag['Typ'] == ABRECHNUNGSTYP_VERPFLEGUNG) // $eintrag['F_Art_id'] == CVJMART_VERPFLEGUNG )
	{
		// Pro Kopf und Altersgruppe
		$essenBezahlt = 0;
		for ($i = 1; $i <= Count($Alterswerte); $i++)
		{
			$fSql = $mySql;
			$anz = $buchung['Personen' . $i] + $buchung['Personen' . $i . 'w'];
			$preis = 0;
			if ($anz > 0)
			{
				$artid = holeUnterArtikelID($eintrag['id'], $Alterswerte[$i] . ' Jahre');
				if ($artid < 0)
				{
					$artid = $eintrag['id'];
					$bez = $eintrag['Bezeichnung'];
					$i = Count($Alterswerte) + 1;
					$anz = $eintrag['Menge'];
				} else
					$bez = $eintrag['Bezeichnung'] . ' ' . $Alterswerte[$i] . ' Jahre';
				$preis = holeEndPreis($artid, $buchung);
				// Sonderfall: Essen ist als Artikel zugeordnet
				if (pruefeAlleOberArtikel($eintrag['id'], $buchung) != -1)
					$preis = 0;
				$fSql .= $anz . ',' . $eintrag['Dauer'] . ",'" . $bez . "'," . $preis . ',' . $artid . ')';
				if (!sql_query($fSql))
					echo "&gt;&gt;&gt; Fehler $fSql: " . sql_error() . '<br />';
				$essenBezahlt += $anz;
				$gesamt += berechnePreis($preis, $anz);
			}
		} // for
		$mySql = '';
		if ($essenBezahlt < $eintrag['Menge'])
		{
			// zusätzliches Essen
			echo '<div class="Fehler">';
			echo date('d.m.Y', $eintrag['Datum']) . ': ';
			echo 'Weniger Essen ' . $eintrag['Bezeichnung'] .
			' abgerechnet als Personen gebucht!</div>';
		}
		if ($eintrag['id'] != CVJM_SELBSTVERPFLEGUNG && $essenBezahlt > 0)
			// TODO: Wozu dient das: ? && $tag != $eintrag['Datum'])
		{
			$tag = $eintrag['Datum'];
			// prüfen, ob Pauschale Vegetarisch anfällt
			if ($buchung['Vegetarier'] > 0)
			{
				$mySql .= $sql . $eintrag['Datum'] . ',';
				$q = sql_query('SELECT * FROM ' . TABLE_ARTIKEL . ' WHERE id=' . CVJM_VEGETARIER);
				$vegetarier = sql_fetch_array($q);
				sql_free_result(q);
				$mySql .= $MWST[$vegetarier['F_MWSt']] . ',';
				if ($vegetarier['Rabattfaehig'])
				{
				$mySql .= $buchung['Buchungsrabatt'] . ',';
				}
				else
				{
					$mySql .= '0,';
				}
				$mySql .= "'" . $Artikelarten[$vegetarier['F_Art_id']] . "','" . sql_real_escape_string($vegetarier['Einheit']) . "',";
				$mySql .= $buchung['Vegetarier'] . ',' . $eintrag['Dauer'] . ",'" . $vegetarier['Bezeichnung'] . "',";
				$preis = holeEndPreis(CVJM_VEGETARIER, $buchung);
				$mySql .= $preis . ',' . CVJM_VEGETARIER . ');';
				sql_query($mySql);
				// keine Fehlerausgabe, da nur einmal Vegetarier pro Tag
				// doppelte Eintr&auml;ge werden dadurch ignoriert
				//if (! sql_query($mySql) )
				//echo 'Vegetarier: '.sql_error();
				$mySql = '';
				$gesamt += berechnePreis($preis, $buchung['Vegetarier']);
			}
			// Pr&uuml;fen, ob Pauschale 'ohne Hilfe' anf&auml;llt
			if (!$buchung['Kuechenhilfe'])
			{
				// nur wenn noch keine Hilfe an diesem Tag
				if (!$q = sql_query('SELECT Count(*) FROM ' . TABLE_RECHNUNGSEINTRAEGE .
					' WHERE F_Artikel_Nr=' . CVJM_OHNEMITHILFE . ' AND F_Rechnung_id=' .
					$docinput['Rechnung_id'] . ' AND Datum=' . $eintrag['Datum']))
					echo sql_error();
				$hilfe = sql_fetch_row($q);
				sql_free_result(q);
				if ($hilfe[0] == 0)
				{
					if (!$q = sql_query('SELECT * FROM ' . TABLE_ARTIKEL . ' INNER JOIN ' . TABLE_ARTIKELARTEN .
						' ON F_Art_id=Art_id WHERE id=' . CVJM_OHNEMITHILFE))
						{
						echo sql_error();
						}
					$hilfe = sql_fetch_array($q);
					sql_free_result(q);
					$hilfe['Datum'] = $eintrag['Datum'];
					$hilfe['Menge'] = $essenBezahlt;
					$hilfe['Dauer'] = 0;
					EintragBearbeiten($sql, $buchung, $hilfe);
				}
			}
		} // nicht Selbstverpflegung
	} // wenn Verpflegung
	// Schritt 2: Programm?
	elseif ($eintrag['Typ'] == ABRECHNUNGSTYP_KOPF) // CVJMART_PROGRAMM
	{
		// Pro Kopf-Bezahlung
		$mySql .= $eintrag['Menge']; // PersonenAnzahl($buchung)
		$mySql .= ',' . $eintrag['Dauer'] . ",'" . $eintrag['Bezeichnung'] . "',";
		// Preis anpassen !!!!
		$preis = holeEndPreis($eintrag['id'], $buchung, $eintrag['Dauer'] > 0);
		$mySql .= $preis;
		$mySql .= ',' . $eintrag['id'];
		$gesamt += berechnePreis($preis, $eintrag['Menge'], $eintrag['Dauer']); //PersonenAnzahl($buchung)
		if ($eintrag['Menge'] < PersonenAnzahl($buchung))
		{
			echo '<div class="Fehler">';
			echo date('d.m.Y', $eintrag['Datum']) . ': ';
			echo 'Weniger Personen bei ' . $eintrag['Bezeichnung'] .
			' berechnet als Personen gebucht!</div>';
		}
	}
	// Schritt 3: Ort -> Uebernachtung?
	elseif ($eintrag['Typ'] == ABRECHNUNGSTYP_NACHT) //$eintrag['F_Art_id'] == CVJMART_ORT )
	{
		// Uebernachtungen!
		// wenn Schlafplatz >0 sonst normal!
		$Artikel = new Artikel($eintrag['id']);
		$betten = $Artikel->BerechneSchlafplaetze();
		if ($betten >= 0)
		{
			// der erste Tag wird nicht berechnet, weil in Uebernachtungen
			// gerechnet wird.
			if (!in_array($eintrag['id'], $Schlaforte))
			{
				$mySql .= 0;
				$anz = 0;
				$Schlaforte[] = $eintrag['id'];
			} else
			{
				$mySql .= $eintrag['Menge'];
				$anz = $eintrag['Menge'];
			}
		} else
		{
			$mySql .= $eintrag['Menge'];
			$anz = $eintrag['Menge'];
		}
		$mySql .= ',' . $eintrag['Dauer'] . ",'" . $eintrag['Bezeichnung'] . "',";
		// Speisesaal und Aufenthaltsraum werden nicht berechnet
		if ($eintrag['id'] != $buchung['F_Speiseraum_id'] && $eintrag['id'] != $buchung['F_Aufenthaltsraum_id'])
			$preis = holeEndPreis($eintrag['id'], $buchung, $eintrag['Dauer'] > 0);
		else
			$preis = 0;
		$mySql .= $preis . ',' . $eintrag['id'];
		$gesamt += berechnePreis($preis, $anz, $eintrag['Dauer']);
	}
	// Schritt 4: nach Kopf und Nacht
	elseif ($eintrag['Typ'] == ABRECHNUNGSTYP_KOPFNACHT)
	{
		// der erste Tag wird nicht berechnet, weil in Uebernachtungen
		// gerechnet wird.
		if (!in_array($eintrag['id'], $Abgerechnet))
		{
			$anz = 0;
			$Abgerechnet[] = $eintrag['id'];
		} else
		{
			$anz = $eintrag['Menge'];
			if ($eintrag['Menge'] < PersonenAnzahl($buchung))
			{
				echo '<div class="Fehler">';
				echo date('d.m.Y', $eintrag['Datum']) . ': ';
				echo 'Weniger Personen bei ' . $eintrag['Bezeichnung'] .
				' berechnet als Personen gebucht!</div>';
			}
		}
		$mySql .= $anz . ',' . $eintrag['Dauer'] . ",'" . $eintrag['Bezeichnung'] . "',";
		$preis = holeEndPreis($eintrag['id'], $buchung, $eintrag['Dauer'] > 0);
		$mySql .= $preis;
		$mySql .= ',' . $eintrag['id'];
		$gesamt += berechnePreis($preis, $anz, $eintrag['Dauer']);
	}
	// Schritt 5: Sonstiges
	else
	{
		$mySql .= $eintrag['Menge'] . ',' . $eintrag['Dauer'] . ",'" . $eintrag['Bezeichnung'] . "',";
		$preis = holeEndPreis($eintrag['id'], $buchung, $eintrag['Dauer'] > 0);
		$mySql .= $preis;
		$mySql .= ',' . $eintrag['id'];
		$gesamt += berechnePreis($preis, $eintrag['Menge'], $eintrag['Dauer']);
	}
	if ($mySql != '')
	{
		$mySql .= ')';
		if (!sql_query($mySql))
			echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler ' .
			$mySql . ': ' . sql_error() . '</div>';
	}
}

function holeBeschreibung($artikel_id)
{
	$query = sql_query('SELECT Beschreibung FROM ' . TABLE_ARTIKEL . " WHERE id=$artikel_id");
	if ($artikel = sql_fetch_row($query))
		$b = $artikel[0];
	else
		$b = '';
	sql_free_result($query);
	return $b;
}

function Rechnungspositionen($rechnung)
{
	global $design;
	global $MwStSumme;
	global $Restsumme;
	global $Gesamtsumme;
	global $Rabattsumme;
	global $buchung;
	$Color = $design['table_row_bright'];
	if (isset ($_REQUEST['Alle']) && $_REQUEST['Alle'] == 1)
		$Sort = 'Art, Datum, F_Bezeichnung';
	else
		$Sort = 'Art, F_Bezeichnung, F_Artikel_Nr,Datum';
	$sql = 'SELECT * FROM ' . TABLE_RECHNUNGSEINTRAEGE .
	' WHERE F_Rechnung_id = ' . $rechnung['Rechnung_id'] . ' ORDER BY ' . $Sort;
	if (!$query = sql_query($sql))
		echo 'Fehler ' . $sql . ': ' . sql_error();
	// Ausgaben unterdruecken
	ob_start();
	$Art = -1;
	$Meineintrag = array ();
	$Meineintrag['Datum'] = 0;
	$Meineintrag['LetztesDatum'] = 0;
	$Meineintrag['F_Artikel_Nr'] = 0;
	$Meineintrag['Mengen'][0] = 0;
	$Gesamtsumme = 0;
	$Rabattsumme = 0;
	$MwStSumme = 0;

	while ($eintrag = sql_fetch_array($query))
	{
		if ($Meineintrag['F_Artikel_Nr'] != $eintrag['F_Artikel_Nr'] || ($Meineintrag['LetztesDatum'] != $eintrag['Datum'] && $Meineintrag['LetztesDatum'] + 1 != $eintrag['Datum']) || $_REQUEST['Alle'] == 1 || $Meineintrag['F_Preis'] != $eintrag['F_Preis'])
		{
			if ($Meineintrag['F_Artikel_Nr'] != 0)
			{
				// bisherige Zusammenfassung anzeigen
				$Gesamtsumme += ZusammenfassungAnzeigen($Meineintrag, $Color, $Art);
			}
			// Zusammenfassen
			if ($Color == $design['table_row_bright'])
				$Color = $design['table_row_dark'];
			else
				$Color = $design['table_row_bright'];
			$Meineintrag = $eintrag;
			$Meineintrag['LetztesDatum'] = strtotime('+1 day', $Meineintrag['Datum']);
			$Meineintrag['Mengen'][0] = $eintrag['Menge'];
		} else
		{
			$Meineintrag['LetztesDatum'] = strtotime('+1 day', $eintrag['Datum']);
			$Meineintrag['Mengen'][Count($Meineintrag['Mengen'])] = $eintrag['Menge'];
		}
	} // while
	if (sql_num_rows($query) == 0)
		echo '<tr><td colspan="11" class="zentriert">keine Einträge vorhanden</td></tr>';
	else
	{
		$Gesamtsumme += ZusammenfassungAnzeigen($Meineintrag, $Color, $Art);
		if ($Rabattsumme != 0)
		{
			echo '<tr><td colspan="3"></td><td colspan="6">Zwischensumme';
			echo '</td>';
			echo '<td align="right">' . number_format($Gesamtsumme + $Rabattsumme, 2);
			echo "</td></tr>\n";
			echo '<tr><td colspan="3"></td><td colspan="6">abzüglich Rabatt';
			echo '</td>';
			echo '<td align="right">' . number_format(- $Rabattsumme, 2);
			echo "</td></tr>\n";
		}
		echo '<tr><td colspan="3"></td><td colspan="6"><strong><em>Gesamtsumme</em></strong>';
		echo ' (' . sql_num_rows($query) . ' Einträge)</td>';
		echo '<td align="right">' . number_format($Gesamtsumme, 2);
		echo "</td></tr>\n";
		//if ( $buchung['MwStAusweisen'])
		{
			echo '<tr><td colspan="3"></td><td colspan="6">zzgl. MwSt</strong>';
			echo '</td>';
			echo '<td align="right">' . number_format($MwStSumme, 2);
			global $id;
			global $docinput;
			echo '</td><td>';
			if ($MwStSumme > 0)
			{
				echo '<a href="' . get_url($id, array (
					'docinput[Rechnung_id]' => $docinput['Rechnung_id'],
					'MWST' => number_format($MwStSumme, 2)
				)) . '" title="MWST erlassen">E</a>';
			}
			echo "</td></tr>\n";
		}
		//else 
		//  $MwStSumme = 0;
		// Sonstige Zahlen ausgeben
		echo '<tr><td colspan="3"></td><td colspan="6"><em>Anzahlung</em>';
		echo '</td>';
		echo '<td align="right">' . number_format(- $rechnung['VorhAnzahlung'], 2);
		echo "</td></tr>\n";
		echo '<tr><td colspan="3"></td><td colspan="6"><em>Barzahlung</em>';
		echo '</td>';
		echo '<td align="right">-' . number_format($rechnung['Barzahlung'], 2);
		echo "</td></tr>\n";
		echo '<tr><td colspan="3"></td><td colspan="6"><strong><em>Restsumme</em></strong>';
		echo '</td>';
		$Restsumme = number_format(- $rechnung['VorhAnzahlung'] - $rechnung['Barzahlung'] + $Gesamtsumme + $MwStSumme, 2);
		echo '<td align="right"><strong>' . $Restsumme;
		echo "</strong></td></tr>\n";
	}
	$content = ob_get_contents();
	// Ausgabenpufferung beenden, Puffer leeren
	ob_end_clean();
	return $content;
	sql_free_result($query);
}

function ZusammenfassungAnzeigen($eintrag, $Color, & $LetzteArt)
{
	global $id;
	global $Wochentage;
	global $Artikelarten;
	global $document;
	global $docinput;
	global $profile;
	global $buchung;
	global $Rabattsumme;
	global $MwStSumme;
	// überschrift
	if ($eintrag['Art'] != $LetzteArt)
	{
		$LetzteArt = $eintrag['Art'];
		echo '<tr><td colspan="3"></td><td colspan="3"><strong>' . $LetzteArt .
		'</strong></td></tr>';
	}
	if (isset ($eintrag['LetztesDatum']))
	{
		$datum = date('d.m.Y', $eintrag['Datum']);
		if (date('H:i', $eintrag['Datum']) != '00:00')
			$datum .= date(' H:i', $eintrag['Datum']);
		$eintrag['LetztesDatum'] = strtotime('-1 day', $eintrag['LetztesDatum']);
		$datumvonbis = $eintrag['Datum'];
		if ($eintrag['LetztesDatum'] > $eintrag['Datum'])
		{
			$datum .= ' bis ' . substr($Wochentage[date('w', $eintrag['LetztesDatum'])], 0, 2) . ' ' .
			date('d.m.Y', $eintrag['LetztesDatum']);
			$datumvonbis = $eintrag['Datum'] . '-' . $eintrag['LetztesDatum'];
		}
		echo '<tr bgcolor="' . $Color . '">';
		echo '<td>';
		$wieviel = 0;
		$Anzeigen = $eintrag['Mengen'][0];
		foreach ($eintrag['Mengen'] as $key => $value)
		{
			$wieviel += $value;
			if ($Anzeigen != $value)
				$Anzeigen = '';
		}
		if ($Anzeigen != $eintrag['Mengen'][0])
			$Anzeigen = implode(',', $eintrag['Mengen']);

		echo $wieviel;
		echo "</td>\n";
		echo '<td valign="right">';
		if (isset ($_REQUEST['Edit']) && $Anzeigen == $eintrag['Mengen'][0])
		{
			// Menge edit
			echo '<input type="Text" name="docinput[Menge][' . $eintrag['F_Artikel_Nr'] .
			',' . $datumvonbis . ']" value="';
		} else
			echo '<span id="Menge' . $eintrag['F_Artikel_Nr'] . '-' . $datumvonbis . '">';
		echo $Anzeigen;
		if (isset ($_REQUEST['Edit']) && $Anzeigen == $eintrag['Mengen'][0])
			echo '" size="2" maxlength="5" />';
		else
			echo '</span>';
		if (strpos($Anzeigen, ',') === false && !isset ($_REQUEST['Edit']))
		{
			/*echo '<a href="'.get_url($id,array('docinput[Rechnung_id]'=>$docinput['Rechnung_id'],
			  'Menge'=>$eintrag['F_Artikel_Nr'],'Richtung'=>'+','Datum'=>$datumvonbis,
			  'Alle'=>$_REQUEST['Alle'],'Zuf'=>rand(0,100))).
			  '#Einzelheiten"><img src="img/plus.gif" border="0" /></a>';
			*/
			echo '<a href="#" onClick="xajax_erhoeheRechnungsMenge(' . $docinput['Rechnung_id'] . ',' .
			$eintrag['F_Artikel_Nr'] . ',\'' . $datumvonbis . '\'); return false;">' .
			'<img src="img/plus.gif" border="0" /></a>';
			echo '<a href="#" onClick="xajax_senkeRechnungsMenge(' . $docinput['Rechnung_id'] . ',' .
			$eintrag['F_Artikel_Nr'] . ',\'' . $datumvonbis . '\'); return false;">' .
			'<img src="img/minus.gif" border="0" /></a>';
			/*
			echo '<a href="'.get_url($id,array('docinput[Rechnung_id]'=>$docinput['Rechnung_id'],
			  'Menge'=>$eintrag['F_Artikel_Nr'],'Richtung'=>'-','Datum'=>$datumvonbis,
			  'Alle'=>$_REQUEST['Alle'],'Zuf'=>rand(0,100))).
			  '#Einzelheiten"><img src="img/minus.gif" border="0" /></a>';
			*/
		}
		echo "</td>\n";
		echo '<td align="right">';
		if (isset ($_REQUEST['Edit']))
			echo '<input type="Text" name="docinput[Dauer][' . $eintrag['F_Artikel_Nr'] .
			',' . $eintrag['Datum'] . ']" value="' . sprintf('%01.2f', $eintrag['Dauer']) .
			'" size="2" maxlength="4" title="Einzelpreis/Stunde: ' .
			holePreis($eintrag['F_Artikel_Nr'], $buchung['F_Preisliste_id'], true) . '"/>';
		else
			if ($eintrag['Dauer'] != 0)
				echo sprintf('%01.2f', $eintrag['Dauer']);
		echo "</td>\n";
		echo '<td>';
		if (isset ($_REQUEST['Edit']))
			echo '<input type="Text" name="docinput[Einheit][' . $eintrag['F_Artikel_Nr'] .
			',' . $datumvonbis . ']" value="' . stripslashes($eintrag['Einheit']) .
			'" size="10" maxlength="20" />';
		else
			echo stripslashes($eintrag['Einheit']);
		echo "</td>\n<td>";
		$bez = stripslashes($eintrag['F_Bezeichnung']);
		if (isset ($_REQUEST['Edit']))
			echo '<input type="Text" name="docinput[Bezeichnung][' . $eintrag['F_Artikel_Nr'] .
			',' . $datumvonbis . ']" value="' . $bez . '" size="40" maxlength="60" />';
		else
		{
			$b = holeBeschreibung($eintrag['F_Artikel_Nr']);
			if ($b != '')
			{
				echo '<span onMouseOver="return overlib(\'' .
				str_replace("\r","",str_replace("\n","",nl2br(htmlentities($b)))). "',CAPTION,'Artikel " .
				stripslashes($bez) . "');";				
				echo '" onMouseOut="return nd();" >';
			}
			echo $bez;
			if ($b != '')
				echo '</span>';
		}
		echo "</td>\n";
		echo '<td>';
		echo substr($Wochentage[date('w', $eintrag['Datum'])], 0, 2) . ' ';
		echo $datum . '</td>';
		$Preis = $eintrag['F_Preis'];
		echo '<td align="right">';
		if (isset ($_REQUEST['Edit']))
			echo '<input type="Text" name="docinput[Preis][' . $eintrag['F_Artikel_Nr'] .
			',' . $datumvonbis . ']" value="' . $Preis . '" size="4" maxlength="8" title="Einzelpreis: ' .
			holePreis($eintrag['F_Artikel_Nr'], $buchung['F_Preisliste_id'], false) . '"/>';
		else
		{
			echo number_format($Preis, 2);
		}
		echo "</td>\n";
		echo '<td align="right">';
		if (isset ($_REQUEST['Edit']))
			echo '<input type="Text" name="docinput[Rabatt][' . $eintrag['F_Artikel_Nr'] .
			',' . $datumvonbis . ']" value="' . number_format($eintrag['Rabatt'], 1) . '" size="1" maxlength="5">';
		else
			echo number_format($eintrag['Rabatt'], 1);
		$gesamt = berechnePreis($Preis, $wieviel, $eintrag['Dauer'], $eintrag['Rabatt']);
		if ($eintrag['Rabatt'] != 0)
		{
			if ($eintrag['Dauer'] == 0)
				$eintrag['Dauer'] = 1;
			$Rabattsumme += $Preis * $wieviel * $eintrag['Dauer'] * $eintrag['Rabatt'] / 100;
		}
		$MwStSumme += $eintrag['MWSt'] / 100 * $gesamt;
		echo "%</td>\n";
		echo '<td align="right">';
		if (isset ($_REQUEST['Edit']))
		{
			echo '<select name="docinput[Steuer][' . $eintrag['F_Artikel_Nr'] .
			',' . $datumvonbis . ']" value="' . $eintrag['MWSt'] . '" title="Steuerliche Behandlung">';
			$da = false;
			global $SteuerKurz;
			global $Steuerarten;
			foreach ($SteuerKurz as $steuer => $kurz)
			{
				echo '<option value="' . $steuer . '" ';
				if ($eintrag['MWSt'] == $steuer)
				{
					echo 'selected="selected"';
					$da = true;
				}
				echo ' title="' . $Steuerarten[$steuer] . '"';
				echo '>' . $kurz . '</option>' . "\n";
			}
			if (!$da)
			{
			echo '<option value="' . $eintrag['MWSt'] . '" selected="selected">' . $eintrag['MWSt'] . '</option>';
			}
			echo '</select>';
		} else
		{
			if (isset ($Steuerarten[$eintrag['MWSt']]))
			{
				echo $Steuerarten[$eintrag['MWSt']];
			}
			else
			{
			echo $eintrag['MWSt'];
			}
		}
		echo "</td>\n";

		echo '<td align="right" id="Gesamtpreis' . $eintrag['F_Artikel_Nr'] . '-' . $datumvonbis . '">' .
		number_format($gesamt, 2) . "</td>\n"; // Eizelpreis und Preis
		$del = $eintrag['F_Artikel_Nr'] . ',' . $datumvonbis;
		echo '<td>';
		if (isset ($_REQUEST['Alle']) && $_REQUEST['Alle'] == 1)
		{
			echo '<a href="' . get_url($id, array (
				'DelEintrag' => $del,
				'docinput[Rechnung_id]' => $docinput['Rechnung_id'],
				'Alle' => 1
			));
		}
		else
		{
			echo '<a href="' . get_url($id, array (
				'DelEintrag' => $del,
				'docinput[Rechnung_id]' => $docinput['Rechnung_id']
			));
		}
		echo '#Einzelheiten" onClick="javascript:return window.confirm(\'Eintrag ' .
		stripslashes($eintrag['F_Bezeichnung']) .
		' ' . $datum;
		echo ' wirklich löschen?\');"><img src="/img/small_edit/delete.gif" border="0"/></a>';
		echo "</td></tr>\n";
		return $gesamt;
	}
}

if ($profile['editor'] != 1)
{
	include (INC_PATH . 'misc/CVJM.inc');
	include (INC_PATH . 'misc/cvjmArtikeltools.inc');
	include (INC_PATH . 'misc/cvjmBuchungstools.inc');
	include (INC_PATH . 'misc/Kalenderjs.inc');
	include (INC_PATH . 'misc/cvjmKorrespondenz.inc');
	require_once(INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	
	echo '<script src="/javascript/overlib.js" type="text/javascript"></script>';
	echo '<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>';
	if ($document['title'] != '')
		echo '<h1>' . transform($document['title'], $document['datatype']['title']) . '</h1>';
	KalenderScriptEinfuegen();
	if (isset ($docinput['Rechnungsnummer']) && is_numeric($docinput['Rechnungsnummer']) && !isset ($docinput['Buchung_Nr']))
	{
		$query = sql_query('SELECT F_Buchung_Nr, Rechnung_id FROM ' . TABLE_RECHNUNGEN . ' WHERE Rechnungsnummer=' .
		$docinput['Rechnungsnummer']);
		$rechnung = sql_fetch_row($query);
		$docinput['Buchung_Nr'] = $rechnung[0];
		$docinput['Rechnung_id'] = $rechnung[1];
		sql_free_result($query);
	}
	if (isset ($docinput['Rechnung_id']) && is_numeric($docinput['Rechnung_id']) && !isset ($docinput['Buchung_Nr']))
	{
		$query = sql_query('SELECT F_Buchung_Nr FROM ' . TABLE_RECHNUNGEN . ' WHERE Rechnung_id=' .
		$docinput['Rechnung_id']);
		$rechnung = sql_fetch_row($query);
		$docinput['Buchung_Nr'] = $rechnung[0];
		sql_free_result($query);
	}
	if (isset ($_REQUEST['Save']) && isset ($docinput['Rechnung_id']) && is_numeric($docinput['Rechnung_id']) && is_numeric($docinput['IstRechnung']))
	{
		// Rechnung sichern
		$docinput['VorhAnzahlung'] = str_replace(',', '.', $docinput['VorhAnzahlung']);
		$docinput['Barzahlung'] = str_replace(',', '.', $docinput['Barzahlung']);
		if (!is_numeric($docinput['VorhAnzahlung']))
			$docinput['VorhAnzahlung'] = 0;
		if (!is_numeric($docinput['Rechnungsnummer']))
			$docinput['Rechnungsnummer'] = 0;
		$docinput['ReVon'] = convertDatumToTimestamp($docinput['ReVon']);
		$docinput['Rechnungsdatum'] = convertDatumToTimestamp($docinput['Rechnungsdatum']);
		$docinput['ReBis'] = convertDatumToTimestamp($docinput['ReBis']);
		$Rechnung = '';
		if (isset ($docinput['IstRechnung']) && is_numeric($docinput['IstRechnung']) && $docinput['IstRechnung'] == 1 && $docinput['Rechnungsnummer'] == 0)
		{
			// Rechnung wurde erzeugt
			$q = sql_query('SELECT Max(Rechnungsnummer) FROM ' . TABLE_RECHNUNGEN);
			$reNr = sql_fetch_row($q);
			sql_free_result($q);
			if (is_numeric($reNr[0]))
				$docinput['Rechnungsnummer'] = $reNr[0] + 1;
			else
				$docinput['Rechnungsnummer'] = 1;
			$Rechnung = '';
			$Buchung = new Buchung($docinput['Buchung_Nr']);
			$buchung = holeBuchung($docinput['Buchung_Nr']);
			$anzString = '';
			// wenn keine Anzahlung getätigt wurde (Bemerkung leer) dann auf 0 setzen
			// damit die Anzahlung aus der Liste der offenen Anzahlungen rausfällt 
			$Buchung->anzahlungNullen();
			$Buchung->logAction('Rechnung Nr. '.$docinput['Rechnungsnummer'].' erstellt');
		}
		if (isset ($docinput['IstKeineRechnung']))
		{
			$Rechnung = ',Storno=1';
			//$docinput['Storno'] = 1;
		} else
		{
			$Rechnung = ',Storno=0';
		}
		//$MwSt = $docinput['MwStAusweisen'];
		//if ( ! is_numeric($MwSt) ) $MwSt = 0;
		if (isset ($docinput['Zahlungsziel']))
			$Zahlungsziel = 1;
		else
			$Zahlungsziel = 0;
		// Rechnung speichern
		$sql = 'UPDATE ' . TABLE_RECHNUNGEN . ' SET VorhAnzahlung=' . $docinput['VorhAnzahlung'] .
		',Barzahlung=' . $docinput['Barzahlung'] . ",Hinweise='" . trim(sql_real_escape_string($docinput['Hinweise'])) .
		"',Rechnungsdatum=" . $docinput['Rechnungsdatum'] . ",Rechnungsnummer=" .
			$docinput['Rechnungsnummer'] . ",Zahlungsziel=$Zahlungsziel," . //MwStAusweisen=$MwSt,".
			'Rechnung=' . $docinput['IstRechnung'] . ", Name='" . sql_real_escape_string($docinput['Name']) . "'" .
			",Vorname='" . sql_real_escape_string($docinput['Vorname']) . "',PLZ='" . sql_real_escape_string($docinput['PLZ']) . "'," .
			"Ort='" . sql_real_escape_string($docinput['Ort']) . "',Strasse='" . sql_real_escape_string($docinput['Strasse']) . "'" .
		" $Rechnung WHERE Rechnung_id=" . $docinput['Rechnung_id'];
		if (!sql_query($sql))
		{
		echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler '.$sql.':' . sql_error().'</div>';
		}
	}
	if (isset ($docinput['Rechnung_id']) && isset ($docinput['Kundenwechsel']) && is_numeric($docinput['Kundenwechsel']) && is_numeric($docinput['Rechnung_id']))
	{
		// Kunde wechselt: Die neue Adressenid steht in Kundenwechsel
		$query = sql_query('SELECT Kunden_Nr FROM ' . TABLE_ADRESSEN . ' WHERE Adressen_id=' .
		$docinput['Kundenwechsel'] . ' AND NOT Geloescht');
		$Adresse = sql_fetch_array($query);
		sql_free_result($query);
		if (!is_numeric($Adresse['Kunden_Nr']) || $Adresse['Kunden_Nr'] <= 0)
		{
			// Neuer Kunde: Kundennummer vergeben!
			holeKundenNummer($docinput['Kundenwechsel']);
		}
		if (!sql_query('UPDATE ' . TABLE_RECHNUNGEN . ',' . TABLE_ADRESSEN . ' SET ' .
			TABLE_RECHNUNGEN . '.Name=' . TABLE_ADRESSEN . '.Name,' .
			TABLE_RECHNUNGEN . '.Vorname=' . TABLE_ADRESSEN . '.Vorname,' .
			TABLE_RECHNUNGEN . '.PLZ=' . TABLE_ADRESSEN . '.PLZ,' .
			TABLE_RECHNUNGEN . '.Ort=' . TABLE_ADRESSEN . '.Ort,' .
			TABLE_RECHNUNGEN . '.Strasse=' . TABLE_ADRESSEN . '.Strasse,' .
			TABLE_RECHNUNGEN . '.Zusatz=' . TABLE_ADRESSEN . '.Zusatz,' .
			TABLE_RECHNUNGEN . '.Titel=' . TABLE_ADRESSEN . '.Titel,' .
			'F_Kunden_Nr=Kunden_Nr,F_Adressen_id=Adressen_id ' .
			' WHERE Rechnung_id=' . $docinput['Rechnung_id'] . ' AND Adressen_id=' .
			$docinput['Kundenwechsel']))
			echo sql_error();
	}
	
	if (isset ($docinput['Buchung_Nr']) && is_numeric($docinput['Buchung_Nr']))
	{
		// Status auf erledigt setzen
		if (isset ($docinput['Erledigt']) && is_numeric($docinput['Erledigt']))
		{
			$Buchung = new Buchung($docinput['Buchung_Nr']);
			$Buchung->aendereStatus($docinput['Erledigt']);
			$Buchung->save();
		}
		// Buchung holen
		$bquery = sql_query('SELECT * FROM ' . TABLE_BUCHUNGEN . ' INNER JOIN ' . TABLE_ADRESSEN .
		' ON Adressen_id = F_Adressen_id WHERE Buchung_Nr = ' . $docinput['Buchung_Nr']);
		if ($buchung = sql_fetch_array($bquery))
		{
			echo '<div class="zentriert">[ <a href="' . get_url(get_cmodule_id('cvjmbuchung')) . '">Zur BuchungsÜbersicht</a> ]</div>';
			// Kopf: Wichtige Daten
			ZeigeBuchungskopf($buchung);
			// Ende Kopf
			if ($buchung['BStatus'] >= BUCHUNG_FERTIG)
			{
				echo '<div class="SchwererFehler zentriert ganzeZeile">Buchung ist bereits abgerechnet!</div>';
			}
			// Bemerkungen
			if (!isset ($docinput['Kundenwahl']))
			{
				$query = sql_query('SELECT * FROM ' . TABLE_BUCHUNGSBEMERKUNGEN .
				' WHERE F_Buchung_Nr = ' . $docinput['Buchung_Nr'] . ' ORDER BY Bemerkung_id DESC');
				echo '<div class="zentriert">[ <a href="' . get_url(get_cmodule_id('cvjmbuchungsbemerkungen'), 'docinput[Buchung_Nr]=' .
				$docinput['Buchung_Nr']) . '">Bemerkungen hinzufügen</a> ]';
				echo '</div>';
				if (sql_num_rows($query) > 0)
				{
					echo '<div class="ueberschriftzeile">';
					echo sql_num_rows($query) . ' Bemerkungen';
					echo '</div>';
					$Color = 'helleZeile';
					while ($bemerkung = sql_fetch_array($query))
					{
						echo '<div class="' . $Color . ' ganzeZeile">';
						echo nl2br(trim(stripslashes($bemerkung['Bemerkung'])));
						echo '&nbsp;&nbsp;[ <a href="' . get_url(get_cmodule_id('cvjmbuchungsbemerkungen'), 'docinput[Buchung_Nr]=' .
						$buchung['Buchung_Nr']) . '">Bearbeiten</a> ]';
						if ($bemerkung['Hinweis'] != '')
							echo '<br />' . nl2br(trim(stripslashes($bemerkung['Hinweis'])));
						echo '</div>';
						if ($Color == 'helleZeile') // $design['table_row_bright'] )
							$Color = 'dunkleZeile'; // $design['table_row_dark'];
						else
							$Color = 'helleZeile'; // $design['table_row_bright'];
					}
					sql_free_result($query);
				}
				// Ende Bemerkungen
			} // wenn nicht Artikelsuche
			// Abrechnung erstellen ?
			if (!isset ($docinput['Rechnung_id']) || !is_numeric($docinput['Rechnung_id']))
			{
				if (!is_numeric($_SESSION['LetzteRechnung']))
				{
					// neue Abrechnung erstellen
					$Anzahlung = $buchung['Anzahlung'];
					// Anzahlung nur nach Buchung angeben!
					$Anredearten = holeAnredearten();
					if ($buchung['AnzahlungsBemerkung'] == '' || !is_numeric($Anzahlung))
					{
						$Anzahlung = 0;
					}
					$sql = 'INSERT INTO ' . TABLE_RECHNUNGEN . ' (Name, Vorname, Strasse, PLZ, Ort, Zusatz,' .
					'Titel,Anrede,ReVon, ReBis,Rechnungsdatum,VorhAnzahlung,Barzahlung,Rechnung, ' .
					'F_Adressen_id, F_Kunden_Nr,F_Buchung_Nr,Rechnungsnummer) ' .
					"VALUES ('" . sql_real_escape_string($buchung['Name']) . "','" . 
					sql_real_escape_string($buchung['Vorname']) . "','" . sql_real_escape_string($buchung['Strasse']) .
					"','" . $buchung["PLZ"] . "','" . sql_real_escape_string($buchung['Ort']) . "','" . 
					sql_real_escape_string($buchung['Zusatz']) . "','" .
					sql_real_escape_string($buchung['Titel']) . "','" . 
					$Anredearten[$buchung['F_Anrede_id']] . "'," . $buchung['Von'] . ',' .
						$buchung['Bis'] . ',' . strtotime(date('Y-m-d')) . ',' . 
						$Anzahlung . ',0,3,' . // 3- Bestätigung
					$buchung['Adressen_id'] . ',' . $buchung['Kunden_Nr'] .
					',' . $buchung['Buchung_Nr'] . ',0)';
					if (!sql_query($sql))
						echo '<div class="SchwererFehler">Fehler $sql: ' . sql_error() . '</div>';
					$docinput['Rechnung_id'] = sql_insert_id();
					$_SESSION['LetzteRechnung'] = $docinput['Rechnung_id'];
					// Nun noch die Einträge übernehmen
					$sql = 'INSERT INTO ' . TABLE_RECHNUNGSEINTRAEGE . ' (F_Rechnung_id, Datum,MWSt,' .
					'Rabatt,Art,Einheit,Menge,Dauer,F_Bezeichnung,F_Preis,F_Artikel_Nr) VALUES (' .
					$docinput['Rechnung_id'] . ',';
					$Artikelarten = holeArtikelarten();
					$MWST = holeMWST();
					if (!$query = sql_query('SELECT * FROM ' . TABLE_BUCHUNGSEINTRAEGE .
						' INNER JOIN (' . TABLE_ARTIKEL . ' INNER JOIN ' . TABLE_ARTIKELARTEN .
						' ON F_Art_id=Art_id) ON id=F_Artikel_Nr ' .
						' WHERE F_Buchung_Nr = ' . $buchung['Buchung_Nr'] . ' AND NOT Unberechnet ORDER BY Datum'))
						{
						echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler ' . sql_error() . '</div>';
						}
					$tag = 0;
					$gesamt = 0; // Gesamtpreis
					$Schlaforte = array ();
					$Abgerechnet = array ();
					$Abrechnungseintraege = array ();
					while ($eintrag = sql_fetch_array($query))
					{
						$Abrechnungseintraege = EintragAufnehmen($eintrag, $buchung, $Abrechnungseintraege);
					} // solange Eintraege vorhanden
					sql_free_result($query);
					$gesamt = ueberArbeiteEintraege($docinput['Rechnung_id'], $buchung, $Abrechnungseintraege);
					// auf ganze 10 Euro aufrunden
					if (!is_numeric($gesamt))
						$gesamt = 0;
					$gesamt = ceil($gesamt * 0.025) * 10;
					// Minimalkosten fuer Anzahlung: 50
					if ($gesamt < 50)
						$gesamt = 50;
					// prüfen: Wenn Anzahlung noch nicht vorhanden, dann eintragen
					if (($buchung['Anzahlung'] == '' || $buchung['Anzahlung'] == 0) && $buchung['Anzahlungsbemerkung'] == '')
					{
						sql_query('UPDATE ' . TABLE_BUCHUNGEN . ' SET Anzahlung=' . $gesamt .
						' WHERE Buchung_Nr=' . $buchung['Buchung_Nr']);
						$buchung['Anzahlung'] = $gesamt;
					}
					sql_query('UPDATE ' . TABLE_BUCHUNGEN . ' SET Logtext=CONCAT("' . date('d.m.Y H:i') .
					' Abrechnung erstellt ' . get_user_nickname() . '\n",Logtext) WHERE Buchung_Nr=' .
					$buchung['Buchung_Nr']);
				} // Session
				else
					$docinput['Rechnung_id'] = $_SESSION['LetzteRechnung'];
			} // neue Abrechnung
			// Statusänderung:
			if (isset ($docinput['Storno']) && is_numeric($docinput['Storno']) && $docinput['Storno'] == BUCHUNG_STORNIERT)
			{
				// Wenn Storno dann kann nun gelöscht werden!
				echo '<strong>&gt;&gt;&gt; Storniere Buchung...</strong>';
				if (!sql_query('DELETE FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' WHERE F_Buchung_Nr=' . $buchung['Buchung_Nr']))
					echo sql_error();
				if (!sql_query('UPDATE ' . TABLE_BUCHUNGEN . ' SET BStatus=' . $docinput['Storno'] .
					', Logtext=CONCAT("' . date('d.m.Y H:i') . ' storniert ' . get_user_nickname() . '\n",Logtext) ' .
					' WHERE Buchung_Nr=' . $buchung['Buchung_Nr']))
					echo sql_error();
			}
			if (isset ($_REQUEST['DelEintrag']))
			{
				// Eintrag entfernen
				list ($Artikelnr, $Datum) = explode(',', $_REQUEST['DelEintrag']);
				if (strpos($Datum, '-') === false)
					sql_query('DELETE FROM ' .
					TABLE_RECHNUNGSEINTRAEGE .
					" WHERE F_Artikel_Nr = $Artikelnr AND Datum = $Datum AND F_Rechnung_id = " .
					$docinput['Rechnung_id']);
				else
				{
					list ($anf, $end) = explode('-', $Datum);
					sql_query('DELETE FROM ' . TABLE_RECHNUNGSEINTRAEGE .
					" WHERE F_Artikel_Nr = $Artikelnr AND Datum BETWEEN $anf AND $end AND F_Rechnung_id = " .
					$docinput['Rechnung_id']);
				}
			}
			// Mehrwertsteuer erlassen
			if (isset ($_REQUEST['MWST']) && is_numeric($_REQUEST['MWST']))
			{
				// Berechnung des Nachlasses für MWST
				$steuer = array_search('Geschäftsbetrieb', $Steuerarten);

				// CVJM_PAUSCHALE holen
				$sql = 'SELECT * FROM (' . TABLE_ARTIKEL . ' INNER JOIN ' . TABLE_MWST .
				' ON F_MWSt=MWST_id) INNER JOIN ' . TABLE_ARTIKELARTEN . ' ON F_Art_id=Art_id' .
				' WHERE id=' . CVJM_PAUSCHALE;
				$q = sql_query($sql);
				if ($artikel = sql_fetch_array($q))
				{
					$NachlassMWST = Min($artikel['MWST'], $buchung['Steuerbehandlung']);
					$nachlass = -number_format($_REQUEST['MWST']/(1+$NachlassMWST/100),2); 
					// Einfügen eines entsprechenden Artikels 
					$sql = 'INSERT INTO ' . TABLE_RECHNUNGSEINTRAEGE .
						' (F_Rechnung_id, Datum,MWSt,Rabatt,Art,Einheit,Menge,Dauer,' .
						'F_Bezeichnung,F_Preis,F_Artikel_Nr) VALUES (' .
						$docinput['Rechnung_id'] . ',' . $buchung['Bis'] . ',' . $NachlassMWST .
						',0,"' . sql_real_escape_string($artikel['Art']) . '","' . 
						sql_real_escape_string($artikel['Einheit']) .
						 '",1,0,"' . 
						sql_real_escape_string($artikel['Bezeichnung']) . '",' . str_replace(',','.',$nachlass) . ',-1)';
					if (!sql_query($sql))
					{
						echo '<div class="SchwererFehler">Der Mahrwertsteuernachlass konnte nicht in die Datenbank eingefügt werden!</div>';
						echo sql_error().'/'.$sql;
					}
					else
					{
						echo '<div class="Erledigt">Die Mehrwertsteuer wurde durch Einfügen eines Nachlasses genullt.</div>';
					}

				} else
				{
					echo '<div class="SchwererFehler">Der Gutschrift-Artikel ' . CVJM_PAUSCHALE . ' konnte nicht geladen werden!</div>';
					echo $sql;
				}
				sql_free_result($q);
			}

			if (isset ($_REQUEST['Menge']) && isset ($_REQUEST['Richtung']) && isset ($_REQUEST['Datum']) && is_numeric($_REQUEST['Menge']))
			{
				// Eintragsmenge erhöhen
				$Richtung = $_REQUEST['Richtung'];
				if ($Richtung != '+' && $Richtung != '-')
					$Richtung = '+';
				$Datum = $_REQUEST['Datum'];
				$Artikelnr = $_REQUEST['Menge'];
				$sql = 'UPDATE ' . TABLE_RECHNUNGSEINTRAEGE . " SET Menge=Menge $Richtung 1 WHERE " .
				"F_Artikel_Nr = $Artikelnr AND F_Rechnung_id= " . $docinput['Rechnung_id'] . ' AND Datum';
				if (strpos($Datum, '-') === false)
					$sql .= ' = $Datum';
				else
				{
					list ($anf, $end) = explode('-', $Datum);
					$sql .= " BETWEEN $anf AND $end";
				}
				if (!sql_query($sql))
					echo 'Fehler: ' . $sql . ': ' . sql_error();
			}
			// Die Rechnungsdaten holen
			if (!$query = sql_query('SELECT * FROM ' . TABLE_RECHNUNGEN . ' WHERE Rechnung_id=' .
				$docinput['Rechnung_id']))
				echo 'Fehler: ' . sql_error();
			$rechnung = sql_fetch_array($query);
			sql_free_result($query);
			if (isset ($docinput['Bereinigen']) && $rechnung['Rechnung'] == 1)
			{
				entferneBerechneteEintraege($docinput['Rechnung_id'], $buchung['Buchung_Nr']);
			}
			// Editieren einzelner Einträge
			if (isset ($_REQUEST['SaveEdit']))
			{
				// Einträge speichern
				foreach ($docinput['Bezeichnung'] as $key => $value)
				{
					list ($Artikelnr, $datum) = explode(',', $key);
					list ($von, $bis) = explode('-', $datum);
					if ($bis == '')
						$bis = $von;
					if (!is_numeric($von) || !is_numeric($bis) || !is_numeric($Artikelnr))
						die('Fehler - Nicht-numerisches Argument beim Speichern:' .
						$von . '/' . $bis . '/' . $Artikelnr);
					if ($bis == '')
						$Datum = '=' . $von;
					else
						$Datum = "BETWEEN $von AND $bis";
					$sql = 'UPDATE ' . TABLE_RECHNUNGSEINTRAEGE . " SET F_Bezeichnung='";
					$sql .= sql_real_escape_string($value) . "', F_Preis=";
					$docinput['Preis'][$key] = str_replace(',', '.', $docinput['Preis'][$key]);
					if (is_numeric($docinput['Preis'][$key]))
						$sql .= $docinput['Preis'][$key];
					else
						$sql .= '0';
					if (isset ($docinput['Menge'][$key]))
					{
						$Menge = str_replace(',', '.', $docinput['Menge'][$key]);
						if (is_numeric($Menge))
							$sql .= ',Menge=' . $Menge;
					}
					if (isset ($docinput['Steuer'][$key]))
					{
						$Steuer = str_replace(',', '.', $docinput['Steuer'][$key]);
						if (is_numeric($Steuer))
							$sql .= ',MWSt=' . $Steuer;
					}
					if (isset ($docinput['Dauer'][$key]))
					{
						$Dauer = str_replace(',', '.', $docinput['Dauer'][$key]);
						if (is_numeric($Dauer))
							$sql .= ',Dauer=' . $Dauer;
					}
					$sql .= ',Rabatt=' . str_replace(',', '.', $docinput['Rabatt'][$key]);
					$sql .= ",Einheit='" . addslashes($docinput['Einheit'][$key]) . "'";
					$sql .= "WHERE F_Artikel_Nr = $Artikelnr AND Datum $Datum AND ";
					$sql .= 'F_Rechnung_id = ' . $docinput['Rechnung_id'];
					if (!sql_query($sql))
						echo "Fehler $sql: " . sql_error();
				}
			}			    			
			$Smarty->assign('id', $id);
			$Smarty->assign('docinput', $docinput);
			$Smarty->assign('rechnung', $rechnung);						
			asort($DokArten);
			$Smarty->assign('DokArten_values', array_keys($DokArten));
			$Smarty->assign('DokArten_output', array_values($DokArten));
			include (INC_PATH . 'misc/cvjmArtikelHinzu.inc');
			ArtikelHinzufuegen(TABLE_RECHNUNGSEINTRAEGE);
					
			if (isset ($docinput['Kundenwahl']) && isset ($docinput['Kunde']) && $docinput['Kunde'] != '') 
			{
				// die Kunden aufzählen und eine Auswahl möglich machen
					// Anzeigen der gefundenen Kunden
					$query = sql_query('SELECT * FROM ' . TABLE_ADRESSEN . ' WHERE Name LIKE "%' .
					$docinput['Kunde'] . '%" ORDER BY Name');
					$Adressen = array();
					while ($row = sql_fetch_array($query))
					{
						$Adressen[$row['Adressen_id']] = $row;						
					}
					sql_free_result($query);
					$Smarty->assign('Kunden', $Adressen);
					$Smarty->display('Abrechnung_Daten.tpl');					
			} else
			{
				// Artikel bereitstellen. Dabei wird die Gesamtsumme berechnet
				$content = Rechnungspositionen($rechnung);
				if ($buchung['Anzahlungsbemerkung'] == '' && is_numeric($Gesamtsumme) && isset($docinput['NeuAnzahlung']) )
				{
					$anzahlung = ceil($Gesamtsumme * 0.025) * 10;
					// Minimalkosten fuer Anzahlung: 50
					if ($anzahlung < 50)
						$anzahlung = 50;	
					sql_query('UPDATE ' . TABLE_BUCHUNGEN . ' SET Anzahlung=' . $anzahlung.
						',Logtext=CONCAT("' . date('d.m.Y H:i') .
						' Anzahlung neu berechnet ' . get_user_nickname() . '\n",Logtext)'.
						' WHERE Buchung_Nr=' . $buchung['Buchung_Nr']);
						$buchung['Anzahlung'] = $anzahlung;
				}
				$rechnung['Anzahlung'] = $buchung['Anzahlung'];
				$rechnung['Restsumme'] = $Restsumme;
				$rechnung['Buchungsrabatt'] = $buchung['Buchungsrabatt'];
				$Smarty->assign('Buchungfertig', $buchung['BStatus'] >= BUCHUNG_FERTIG);
				$Smarty->assign('Buchungaktuell', date('Ymd', $buchung['Bis']) <= date('Ymd', time()));
				if ($buchung['BStatus'] == BUCHUNG_STORNIERT)
					$Smarty->assign('Status',BUCHUNG_STORNIERTABGERECHNET);
				else
					$Smarty->assign('Status',BUCHUNG_FERTIG);
				$Smarty->assign('Buchungurl', get_url(get_cmodule_id('cvjmbuchung'), 'Buchung_Nr=' .$buchung['Buchung_Nr']));			
			    $Smarty->display('Abrechnung_Daten.tpl');			
			}
			/* Ende des Templates */ 
			if (!isset ($docinput['Kundenwahl']))
			{
				$Buchungsvorlagen = ZeigeVorlagen($rechnung['Rechnung_id'], 'Fakturierung/' .
				$DokArten[$rechnung['Rechnung']], 'Rechnung');
				// Einzelheiten
				
				echo '<div id="Artikel_Hinzu">';
				anzeigeArtikelHinzufuegen(new Buchung($buchung['Buchung_Nr']), 10);
				echo '</div>';
				echo '<table width="100%">';
				echo '<tr class="ueberschrift"><th colspan="10">';
				echo '<a id="Einzelheiten" name="Einzelheiten"></a>Einzelheiten';
				echo '</th></tr>';
				echo '<tr><td colspan="2">Menge</td><td>Dauer</td><td>Einheit</td><td>Artikel</td><td>Datum&nbsp;&nbsp; ';
				if (isset ($_REQUEST['Alle']) && $_REQUEST['Alle'] == 1)
					echo '[&nbsp;<a href="' . get_url($id, array (
						'docinput[Rechnung_id]' => $rechnung['Rechnung_id']
					)) .
					'#Einzelheiten">Zusammenfassung</a>&nbsp;]';
				else
					echo '[&nbsp;<a href="' . get_url($id, array (
						'docinput[Rechnung_id]' => $rechnung['Rechnung_id'],
						'Alle' => 1
					)) . '#Einzelheiten">jedes Datum einzeln</a>&nbsp;]';
				echo '</td><td>Einzelpreis</td><td>Rabatt</td><td>Steuer</td><td>Preis</td><td></td></tr>';
				if (isset ($_REQUEST['Edit']))
				{
					echo '<form action="' . get_url($id) . '#Einzelheiten" method="post">';
					echo '<input type="hidden" name="docinput[Rechnung_id]" value="' . $rechnung['Rechnung_id'] . '" />';
					echo '<tr><td colspan="9" class="zentriert"><input type="Submit" ';
					echo 'name="SaveEdit" value="Einträge speichern">';
					echo "</td></tr>\n";
				} else
				{
					echo '<tr><td colspan="9" class="zentriert">';
					echo '[ <a href="' . get_url($id, 'docinput[Rechnung_id]=' . $docinput['Rechnung_id'] .
					'&Edit=1&Alle=1#Einzelheiten');
					echo '">Einträge einzeln bearbeiten</a> ]&nbsp;&nbsp;';
					echo '[ <a href="' . get_url($id, 'docinput[Rechnung_id]=' . $docinput['Rechnung_id'] .
					'&Edit=1#Einzelheiten');
					echo '">Einträge bearbeiten</a> ]';
				    if ( $buchung['AnzahlungsBemerkung'] == '')
					{
					  echo ' [ <a href="' . get_url($id, 'docinput[Rechnung_id]=' . $docinput['Rechnung_id'] .
					'&docinput[NeuAnzahlung]#Einzelheiten');
					  echo '">Anzahlung neu berechnen</a> ]';
					}
					echo "</td></tr>\n";
				}

				// die gepufferten Artikel ausgeben                
				echo $content;

				if (isset ($_REQUEST['Edit']))
				{
					echo '<tr><td colspan="9" class="zentriert"><input type="Submit" ';
					echo 'name="SaveEdit" value="Einträge speichern">';
					echo "</td></tr>\n";
					echo '</form>';
				} else
				{
					echo '<tr><td colspan="9" class="zentriert">';
					echo '[ <a href="' . get_url($id, 'docinput[Rechnung_id]=' . $docinput['Rechnung_id'] .
					'&Edit=1&Alle=1#Einzelheiten');
					echo '">Einträge einzeln bearbeiten</a> ]&nbsp;&nbsp;';
					echo '[ <a href="' . get_url($id, 'docinput[Rechnung_id]=' . $docinput['Rechnung_id'] .
					'&Edit=1#Einzelheiten');
					echo '">Einträge bearbeiten</a> ] ';
					if ( $buchung['AnzahlungsBemerkung'] == '')
					{
					  echo '[ <a href="' . get_url($id, 'docinput[Rechnung_id]=' . $docinput['Rechnung_id'] .
					'&docinput[NeuAnzahlung]#Einzelheiten');
					  echo '">Anzahlung neu berechnen</a> ]';
					}
					echo "</td></tr>\n";
				}
				echo '</table>';
				$buchung['Rechnung_id'] = $docinput['Rechnung_id'];
				ZeigeKorrespondenz($buchung, $Buchungsvorlagen, array (
					'docinput[Rechnung_id]' => $docinput['Rechnung_id']
				));
			}
		} else
		{
			echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler: Buchungsnummer nicht vorhanden!</div>';
		}
		sql_free_result($bquery);
	} else
	{
		echo '&gt;&gt;&gt; Es muss eine Buchungsnummer angegeben werden!<br />';
	}
	echo '<div class="zentriert">[ <a href="' . get_url(get_cmodule_id('cvjmbuchung')) . '">Zur Buchungs&uuml;bersicht</a> ]</div>';
} // editor
else
	create_editor_input('Recht für Veränderungen', 'SpeicherRecht', 'text_input');
?>
