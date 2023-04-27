<?php

/*
 AWF-Name: cvjmstatistik
 cvjmStatistik.inc

 Wenn SaveStat gesetzt wird, werden die berechneten Werte in die Tabelle
 cvjm_Belegungsstatistik geschrieben.
 */

define('CVJM_ZELTBEREICH', 'ZB');

if ( $profile['editor'] != 1 )
{
	include(INC_PATH.'misc/CVJM.inc');
	include(INC_PATH.'misc/cvjmArtikeltools.inc');
	include(INC_PATH.'misc/cvjmBuchungstools.inc');
	include(INC_PATH.'cvjm/Artikel.class.php');
	if ( isset($_REQUEST['SaveStat']) || (isset($docinput['Jahr']) && is_numeric($docinput['Jahr'])))
	{
		if ( $docinput['Jahr'] > 0 )
		{
			$Jahr = 'FROM_UNIXTIME(Datum,"%Y")='.$docinput['Jahr'];
		}
		else
		{
			$Jahr = '1';
		}

		$Artikelarten = Artikel::getArtikelArten();
		if ( $document['title'] != '')
		{
			echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
		}

		$kurz = true;
		if ( isset($docinput['Jahr']))
		{
			$Anhang = '&docinput[Jahr]='.$docinput['Jahr'];
		}
		else
		{
			$Anhang = '';
		}
		if ( isset($docinput['Kurz']) )
		{
			$kurz = false;
			echo '[ <a href="'.get_url($id.$Anhang).'">Jahresweise Übersicht</a> ]';
		}
		else
		{
			echo '[ <a href="'.get_url($id,'docinput[Kurz]=1'.$Anhang).'">Monatsweise Übersicht</a> ]';
		}
		// Forecast: Zukünftige Belegung
		//
		echo '<h2>Zukünftige Belegung</h2>';
		$sql = 'SELECT '.TABLE_BUCHUNGEN.'.*, F_Artikel_Nr, Datum, F_Art_id, Position,Einruecken FROM ('.
		TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.TABLE_ARTIKEL.
		' ON id=F_Artikel_Nr) INNER JOIN '.TABLE_BUCHUNGEN.
		' ON Buchung_Nr=F_Buchung_Nr WHERE (BStatus='.BUCHUNG_RESERVIERUNG.
		' OR BStatus<='.BUCHUNG_VORRESERVIERUNG.') '.
		//') AND Datum >= Von AND Datum > '.time(). // Vorhut wird nicht berücksichtigt
		' AND F_Art_id='.CVJMART_ORT.
		' AND Menge > 0 AND '.$Jahr.' ORDER BY Datum';
		if ( isset($_REQUEST['SaveStat']))
		{
			$start = mktime(0,0,0,1,1,date('Y'));
			$sql = 'SELECT '.TABLE_BUCHUNGEN.'.*, F_Artikel_Nr, Datum, F_Art_id, Position,Einruecken FROM ('.
			TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.TABLE_ARTIKEL.
			' ON id=F_Artikel_Nr) INNER JOIN '.TABLE_BUCHUNGEN.
			' ON Buchung_Nr=F_Buchung_Nr WHERE (BStatus<>'.BUCHUNG_STORNIERT.
			' AND BStatus<='.BUCHUNG_FERTIG.') '.
			//' AND Datum >= Von 
			' AND Datum > '.$start. 
			' AND F_Art_id='.CVJMART_ORT.
			' AND Menge > 0 ORDER BY Datum';
		}

		$query = sql_query($sql);
		$uebernachtungen = array();
		$plaetze = array();
		$summe = array();
		$buchung = array();
		while ( $eintrag = sql_fetch_array($query) )
		{
			$a = new Artikel($eintrag['F_Artikel_Nr']);
			$schlafplaetze = $a->BerechneSchlafplaetze();
			if ( $schlafplaetze >= 0 )
			{
				if ( $schlafplaetze > 0 )
				{
					// Hütten-Übernachtung
					$bereich = getOrtsname(holeArtikelMasterParent($eintrag['F_Art_id'],
					$eintrag['Position'],$eintrag['Einruecken']));
				}
				else
				$bereich = CVJM_ZELTBEREICH;
				$eintrag['Datum'] = strtotime(date('Y-m-d', $eintrag['Datum']));
				if ( ! isset($plaetze[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich]) )
				{
					$plaetze[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich] = 0;
				}
				$buchung[$eintrag['Buchung_Nr']]['Von'] = $eintrag['Von'];
				$buchung[$eintrag['Buchung_Nr']]['Bis'] = $eintrag['Bis'];
				// 2017-02-05 am ersten Tag gibt es keine Übernachtungen - da in Buchung jeder Ort an jedem Tag gezählt wird einmal streichen
				if ($eintrag['Datum'] <> $eintrag['Von'] ) 
 				  	$plaetze[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich] += $schlafplaetze;

				//echo $eintrag['Buchung_Nr'].': Bereich $bereich mit $schlafplaetze<br />';
				$leute[$eintrag['Buchung_Nr']] = Personenanzahl($eintrag);
			}
		}
		sql_free_result($query);

		$sql = 'SELECT Buchung_Nr, Von FROM cvjm_Buchungen WHERE FROM_UNIXTIME(Von,"%Y")='.$docinput['Jahr'].' AND BStatus<='.BUCHUNG_FERTIG;
		$query = mysql_query($sql);
		for ( $i = 1; $i<13; $i++)
		{
			if ( $i < 10 ) 			
				$summe[$docinput['Jahr']]['0'.$i] = 0;
			else
			$summe[$docinput['Jahr']][$i] = 0;
		}
		while ( $eintrag = mysql_fetch_array($query))
		{
			// Neuesten Eintrag aus den Angeboten/Berechnungen/Rechnungen holen
			$sql = 'SELECT Sum( Menge * F_Preis * CASE WHEN Dauer =0 THEN 1 ELSE Dauer END - ( Menge * F_Preis * CASE WHEN Dauer =0 THEN 1 ELSE Dauer
	END * Rabatt ) /100 ) AS Summe FROM cvjm_Rechnungseintraege INNER JOIN cvjm_Rechnungen ON F_Rechnung_Id = Rechnung_Id
	WHERE F_Buchung_Nr ='.$eintrag['Buchung_Nr'].' AND Storno=0 GROUP BY Rechnung_id ORDER BY Rechnungsdatum DESC';
			$qr = mysql_query($sql);
			if ( $result = mysql_fetch_row($qr) )
			{
				if ( ! isset($summe[date('Y',$eintrag['Von'])][date('m',$eintrag['Von'])]) )
				{
					$summe[date('Y',$eintrag['Von'])][date('m',$eintrag['Von'])] = 0;
				}
				$summe[date('Y',$eintrag['Von'])][date('m',$eintrag['Von'])] += $result[0];
			}
			else
			{
				//keine Abrechnung etc. vorhanden
				//$summe[$eintrag['Buchung_Nr']] = 0;
			}
			mysql_free_result($qr);
		}
		mysql_free_result($query);


		// Nun die Auslastung der Bereiche berechnen
		foreach ( $leute as $buchung_nr => $anzahl )
		{
			// Datum durchgehen
			$query = sql_query('SELECT Count(*) FROM '.TABLE_BUCHUNGSEINTRAEGE.
			' WHERE F_Artikel_Nr='.CVJM_SELBSTVERPFLEGUNG." AND F_Buchung_Nr=$buchung_nr");
			$row = sql_fetch_row($query);
			mysql_free_result($query);
			$Selbstverpflegung[$buchung_nr] = $row[0];
			foreach ( $plaetze[$buchung_nr] as $datum => $bereiche )
			{
				$anz = $anzahl;
				$zda = false;
				foreach ( $bereiche as $bereich => $schlafplaetze )
				{
					$Selbst = $Selbstverpflegung[$buchung_nr];
					//echo "Buchung $buchung_nr: Bereich $bereich mit $schlafplaetze vorhanden<br />";
					// Initialisierung
					if ( ! isset($uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]) )
					{
						$uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)] = array();
						$uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Anzahl'] = 0;
						$uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Buchungen'] = array();
						$uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Selbst'] = 0;
                                                
					}
					if ( $schlafplaetze > 0 )
					{
						$personen = 0;
						if ( $anz <= $schlafplaetze )
						{
							$personen = $anz;
							$anz = 0;
						}
						elseif ( $anz > $schlafplaetze )
						{
							$personen = $schlafplaetze;
							$anz -= $schlafplaetze;
						}
						$uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Anzahl'] += $personen;
						if ( $Selbst > 0)
						{
							$Selbst--; // ein Tag ist verbraucht
							$uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Selbst'] += $personen;
						}
						//echo "$buchung_nr  $personen  $bereich  ".date('Y-m-d',$datum)."<br />";
					}
					else
					$zda = true;
					if ( ! in_array($buchung_nr, $uebernachtungen[$bereich][date('Y',
					$datum)][date('m',$datum)]['Buchungen']) )
					$uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Buchungen'][] =
					$buchung_nr;
				}
				if ( $anz > 0 )
				{
					if ( $zda && $buchung[$buchung_nr]['Bis'] >= $datum && $buchung[$buchung_nr]['Von'] < $datum ) 
					{
						
						$uebernachtungen[CVJM_ZELTBEREICH][date('Y',$datum)][date('m',$datum)]['Anzahl'] += $anz;
						if ( $Selbstverpflegung[$buchung_nr] > 0)
						{
							//$Selbst--; // ein Tag ist verbraucht
							$uebernachtungen[CVJM_ZELTBEREICH][date('Y',$datum)][date('m',$datum)]['Selbst'] += $anz;
						}
						//echo "$buchung_nr  $anz  ZB  ".date('Y-m-d',$datum)."<br />";
					}
					else
					{
						if ($buchung[$buchung_nr]['Bis'] >= $datum && $buchung[$buchung_nr]['Von'] < $datum ) 
					        {
						  echo "Nicht zuordbare Personenzahl: $anz in Buchung $buchung_nr am ".
						  date('d.m.Y',$datum).'<br />';
						  if ( ! isset($uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]) )
						  {
							$uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)] = array();
							$uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Anzahl'] = 0;
							$uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Buchungen'] = array();
							$uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Selbst'] = 0;
						  }
						  if ( ! in_array($buchung_nr, $uebernachtungen['unbekannt'][date('Y',
						  $datum)][date('m',$datum)]['Buchungen']) )
						    $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Buchungen'][] = $buchung_nr;
		 				  $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Anzahl'] += $anz;
						}
					}
				}
			}
		}
		echo '<table style="text-align:right">';
		echo '<tr class="ueberschrift"><th>Jahr</th><th>Monat</th><th>Buchungen</th>' .
		'<th>Übernachtungen</th><th>Selbstvers.</th></tr>';
		$allgesamt = 0;
		$dieJahre = array();
		foreach ( $uebernachtungen as $bereich => $jahre )
		{
			echo '<tr class="helleZeile"><td colspan="6" align="left">'.$bereich.'</td></tr>';
			$gesamt = 0;
			$summegesamt = 0;
			$buanz = 0;
			$selbst = 0;
			foreach ( $jahre as $jahr => $monate )
			{
				$gesamtj = 0;
				$buanzj = 0;
				$selbstj = 0;
				$summej =0;
				if ( ! in_array($jahr, $dieJahre) )
				$dieJahre[] = $jahr;
				foreach ( $monate as $monat => $value )
				{
					$buanz += count($value['Buchungen']);
					$buanzj += count($value['Buchungen']);
					if ( ! $kurz )
					{
						echo '<tr><td>';
						echo $jahr;
						echo '</td><td>';
						echo $monat;
						echo '</td><td>';
						echo count($value['Buchungen']).':';
						foreach ( $value['Buchungen'] as $key => $nr )
						echo '<a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$nr).'">'.$nr.'</a>,';
						echo '</td><td>';
						echo $value['Anzahl'];
						echo '</td><td>';
						echo $value['Selbst'];
						echo '</td></tr>';
					}
					$gesamt += $value['Anzahl'];
					$gesamtj += $value['Anzahl'];
					$selbst += $value['Selbst'];
					$selbstj += $value['Selbst'];
					$summej += $summe[$jahr][$monat];
				}
				if ( $kurz ) {
					echo '<tr class="dunkleZeile"><td>Gesamt</td><td>'.$jahr;
					echo "</td><td>$buanzj</td><td>$gesamtj</td><td>$selbstj</td></tr>\n";
				}
			}
			$allgesamt += $gesamt;
			echo '<tr class="dunkleZeile"><td>Gesamt</td><td>';
			echo "</td><td>$buanz</td><td>$gesamt</td><td>$selbst</td></tr>\n";
		}
		echo '</table>';
		echo '<table style="text-align:right">';
		echo '<tr class="ueberschrift"><th>Bereich</th><th>Jahr</th><th>Buchungen</th>' .
		'<th>Übernachtungen</th><th>Selbstvers.</td></tr>';
		$datum = time();
		foreach ( $dieJahre as $jahr )
		{
			echo '<tr class="helleZeile"><td colspan="5" align="left">'.$jahr."</td></tr>\n";
			$gesamt = 0;
			$buanz = 0;
			$selbst = 0;
			foreach ( $uebernachtungen as $bereich => $jahre )
			{
				$gesamtj = 0;
				$buanzj = 0;
				$selbstj = 0;
				if ( is_array($jahre[$jahr])) foreach ( $jahre[$jahr] as $monat => $value )
				{
					$buanz += count($value['Buchungen']);
					$buanzj += count($value['Buchungen']);
					if ( ! $kurz )
					{
						echo '<tr><td>';
						echo $bereich;
						echo '</td><td>';
						echo $monat;
						echo '</td><td>';
						echo count($value['Buchungen']).':';
						foreach ( $value['Buchungen'] as $key => $nr )
						echo '<a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$nr).'">'.$nr."</a>,";
						echo '</td><td>';
						echo $value['Anzahl'];
						echo '</td><td>';
						echo $value['Selbst'];
						echo "</td></tr>\n";
					}
					$gesamt += $value['Anzahl'];
					$gesamtj += $value['Anzahl'];
					$selbst += $value['Selbst'];
					$selbstj += $value['Selbst'];
				}
				if ( $kurz ) {
					echo '<tr class="dunkleZeile"><td>'.$bereich.' Gesamt</td><td>'.$jahr;
					echo "</td><td>$buanzj</td><td>$gesamtj</td><td>$selbstj</td></tr>";
					if ( isset($_REQUEST["SaveStat"]))
					sql_query("INSERT INTO cvjm_Belegungsstatistik (Datum,Bereich,Buchungen,Menge,Jahr,SV) " .
					"VALUES ($datum,'$bereich',$buanzj,$gesamtj,$jahr,$selbstj)");
				}
			}
			$allgesamt += $gesamt;
			echo '<tr class="dunkleZeile"><td>Gesamt</td><td>'.$jahr;
			echo "</td><td>$buanz</td><td>$gesamt</td><td>$selbst</td></tr>";
		}
		// echo '<tr class="dunkleZeile"><td>Gesamt</td><td></td><td></td><td>'.
		// "$allgesamt</td></tr>\n";
		echo '</table>';
		echo "<h2>Angebote</h2>";
		echo "<h3>Programmmodule</h3>";
		// Nur Rechnungen berücksichtigen
		$sql = 'SELECT Buchung_Nr, Datum, Menge, F_Artikel_Nr, Einruecken, '.
		'Position, F_Preis, Rabatt FROM ('.
		TABLE_BUCHUNGSEINTRAEGE.
		' INNER JOIN '.
		TABLE_ARTIKEL.' ON F_Artikel_Nr=id) INNER JOIN '.TABLE_BUCHUNGEN.
		' ON F_Buchung_Nr=Buchung_Nr WHERE F_Art_id="'.CVJMART_PROGRAMM.
		'" AND (BStatus='.BUCHUNG_RESERVIERUNG.' OR BStatus<='.BUCHUNG_VORRESERVIERUNG.
		') AND Datum > '.time().' AND '.$Jahr.
		' ORDER BY YEAR(FROM_UNIXTIME(Datum)),MONTH(FROM_UNIXTIME(Datum)),F_Artikel_Nr';
		if (! $query = sql_query($sql))
		echo mysql_error();
		$paket = array();
		while ( $eintrag = sql_fetch_array($query) )
		{
			$parent = holeArtikelParent(CVJMART_PROGRAMM, $eintrag['Position'], $eintrag['Einruecken']);

			$jahr = date('Y', $eintrag['Datum']);
			$monat = date('m',$eintrag['Datum']);
			if ( ! isset($paket[$parent][$jahr][$monat]) )
			{
				$paket[$parent][$jahr][$monat]['Buchungen'] = 0;
				$paket[$parent][$jahr][$monat]['Personen'] = 0;
			}
			$paket[$parent][$jahr][$monat]['Buchungen'] += 1;
			$paket[$parent][$jahr][$monat]['Personen'] += $eintrag['Menge'];
		}
		echo '<table style="text-align:right">';
		echo '<tr class="ueberschrift"><th>Jahr</th><th>Monat</th><th>Anzahl</th><th>Personen</th></tr>';
		$personen = 0;
		foreach ( $paket as $artikelnr => $buchungen )
		{
			echo '<tr class="helleZeile"><td colspan="5" align="left">'.getOrtsname($artikelnr).'</td></tr>';
			foreach ( $buchungen as $year => $value )
			{
				foreach ( $value as $monat => $anzahl )
				{
					echo '<tr><td>';
					echo $year;
					echo '</td><td>';
					echo $monat;
					echo '</td><td>';
					echo $anzahl["Buchungen"];
					echo '</td><td>';
					echo $anzahl["Personen"];
					echo "</td></tr>\n";
					$personen += $anzahl["Personen"];
				}
			}
		}
		sql_free_result($query);
		echo '<tr class="dunkleZeile"><td>Gesamt</td><td>';
		echo '</td><td></td><td>'.$personen.'</td></tr>';
		echo "</table>\n";
		// Programmpakete
		echo "<h3>Programmpakete</h3>";
		if (! $query = sql_query("SELECT Datum, Menge, F_Artikel_Nr, Einruecken, Position, F_Preis, Rabatt FROM (".
		TABLE_BUCHUNGSEINTRAEGE.
		" INNER JOIN ".
		TABLE_ARTIKEL." ON F_Artikel_Nr=id) INNER JOIN ".TABLE_BUCHUNGEN.
		" ON F_Buchung_Nr=Buchung_Nr WHERE F_Art_id='".CVJMART_PROGRAMMPAKET.
		"' AND (BStatus=".BUCHUNG_RESERVIERUNG." OR BStatus<=".BUCHUNG_VORRESERVIERUNG.
		") AND Datum > ".time().' AND '.$Jahr.
		" GROUP BY YEAR(FROM_UNIXTIME(Datum)),MONTH(FROM_UNIXTIME(Datum)),F_Artikel_Nr"))
		echo mysql_error();
		$paket = array();
		while ( $eintrag = sql_fetch_array($query) )
		{
			$parent = holeArtikelParent(CVJMART_PROGRAMMPAKET, $eintrag['Position'], $eintrag['Einruecken']);
			$jahr = date('Y', $eintrag['Datum']);
			$monat = date('m',$eintrag['Datum']);
			if ( ! isset($paket[$parent][$jahr][$monat]) )
			{
				$paket[$parent][$jahr][$monat]['Buchungen'] = 0;
				$paket[$parent][$jahr][$monat]['Personen'] = 0;
			}
			$paket[$parent][$jahr][$monat]['Buchungen'] += 1;
			$paket[$parent][$jahr][$monat]['Personen'] += $eintrag['Menge'];
		}
		sql_free_result($query);
		echo '<table style="text-align:right">';
		echo '<tr class="ueberschrift"><th>Jahr</th><th>Monat</th><th>Anzahl</th><th>Personen</th></tr>';
		$personen = 0;
		$gesamt = 0;
		foreach ( $paket as $artikelnr => $buchungen )
		{
			echo '<tr class="helleZeile"><td colspan="5" align="left">'.getOrtsname($artikelnr).'</td></tr>';
			foreach ( $buchungen as $year => $value )
			{
				foreach ( $value as $monat => $anzahl )
				{
					echo '<tr><td>';
					echo $year;
					echo '</td><td>';
					echo $monat;
					echo '</td><td>';
					echo $anzahl['Buchungen'];
					echo '</td><td>';
					echo $anzahl['Personen'];
					$personen += $anzahl['Personen'];
					echo "</td></tr>\n";
				}
			}
		}
		echo '<tr class="dunkleZeile"><td>Gesamt</td><td></td><td></td><td>'.
		$personen."</td></tr>\n";

		echo '</table>';

		// Geldwerte
		echo '<h3>Angebotswerte (exkl. MwSt inkl. Rabatte) soweit vorhanden</h3>';
		echo '<table style="text-align:right">';
		echo '<tr class="ueberschrift"><th>Jahr</th><th>Monat</th><th>Geldwert</th></tr>';
		foreach ( $summe as $Jahr => $werte )
		{
			foreach ( $werte as $Monat => $wert)
			{
				echo '<tr><td>'.$Jahr.'</td><td>'.$Monat.'</td><td>'.sprintf("%01.2f",$wert).'</td></tr>';
			}
		}
		echo '</table>';
	} // wenn Jahr übergeben wurde
	echo '<form action="" method="post">';
	echo 'Jahr ';
	$query = mysql_query('SELECT DISTINCT FROM_UNIXTIME(Datum,"%Y") FROM '.
	TABLE_BUCHUNGSEINTRAEGE.' WHERE Datum>='.strtotime('-5 days').' ORDER BY Datum DESC');
	echo '<select name="docinput[Jahr]">';
	echo '<option value="-1">-alle-</option>';
	while ( $jahr = mysql_fetch_row($query))
	{
		echo '<option ';
		if ( $jahr[0] == date('Y') ) echo 'selected="selected"';
		echo '>'.$jahr[0].'</option>'."\n";
	}
	echo '</select> ';
	mysql_free_result($query);
	echo '<input type="submit" value="Anzeigen" />';
	echo '</form>';
}
?>
