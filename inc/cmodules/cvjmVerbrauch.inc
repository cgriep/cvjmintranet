<?php

/*

  cvjmVerbrauch.inc
  AWF-Modul: cvjm_Verbrauch

*/
define('TABLE_GROESSEN', 'cvjm_Verbrauchsgroessen');
define('TABLE_KOSTEN', 'cvjm_Verbrauchspreise');
define('TABLE_VERBRAUCH', 'cvjm_Verbrauchsdaten');

$turnusse[0] = 'täglich';
$turnusse[1] = 'wöchentlich';
$turnusse[2] = 'monatlich';

function holeAktuellenPreis($groesse_id, $Datum = -1)
{
	if ($Datum < 0)
		$Datum = time();
	$sql = 'SELECT Preis FROM ' . TABLE_KOSTEN .
	' WHERE F_Groesse_id=' . $groesse_id . ' AND Datum IN ' .
	'(SELECT Max(Datum) FROM ' . TABLE_KOSTEN .
	' WHERE F_Groesse_id=' . $groesse_id . ' AND Datum<' . $Datum . ')';
	if (!$q = sql_query($sql))
		echo sql_error();
	if (!$preis = sql_fetch_row($q))
		$preis[0] = '-nicht vorhanden-';
	sql_free_result($q);
	return $preis[0];
}

function verbrauchBerechnen($Datum, $Groesse, $Wert, $Fuellung = false, $NurBerechnung = false)
{
    $Bemerkung = get_user_nickname();
    // Eine Sekunde erhöhen, damit die Füllung immer die neuste am Tag ist
	if ( $Fuellung )	  
	{
      $Datum++;
      $Bemerkung .= ' getankt';
	}					
	$Faktor = 1;
	$q = sql_query('SELECT Faktor FROM ' . TABLE_GROESSEN . ' WHERE Groesse_id=' . $Groesse);
	$F = sql_fetch_row($q);
	sql_free_result($q);
	if (!is_numeric($F[0]))
		$Faktor = 1;
	else
		$Faktor = $F[0];
	$sql = 'SELECT Wert FROM ' . TABLE_VERBRAUCH .
	' WHERE F_Groesse_id=' . $Groesse . ' AND Datum IN(SELECT MAX(Datum) FROM '.
	TABLE_VERBRAUCH.' WHERE Datum<'.$Datum.' AND F_Groesse_id='.$Groesse.') ORDER BY Wert DESC';
	$query = sql_query($sql);
	if ($altwert = sql_fetch_row($query))
	{
		$altwert = $altwert[0];
	}
	else
	{
		$altwert = 0;
	}
	sql_free_result($query);
	if (! $NurBerechnung)
	{
	// Eintrag einfügen
	if (!sql_query('INSERT INTO ' . TABLE_VERBRAUCH .
		' (F_Groesse_id,Datum,Wert,Bemerkung,Verbrauch) VALUES (' .
		$Groesse . ',' . $Datum . ',' . $Wert . ',"' . $Bemerkung . '",1)'))  // Verbrauch muss ungleich null sein
		echo 'Fehler: ' . sql_error();
	}
	$Verbrauch = 0;
	// Alles ab Datum neu berechnen 
	$sql = 'SELECT * FROM ' . TABLE_VERBRAUCH . ' WHERE F_Groesse_id=' . $Groesse .
	' AND Datum>=' . $Datum . ' ORDER BY Datum';
	$query = sql_query($sql);
	while ($eintrag = sql_fetch_array($query))
	{
		// vorangehenden Wert holen, um Verbrauch zu berechnen 
		$Verbrauch = ($eintrag['Wert']-$altwert) * $Faktor;
		if ( $eintrag['Verbrauch'] != 0 || $eintrag['Verbrauchskosten'] != 0)
		{
		$Verbrauchskosten = holeAktuellenPreis($Groesse, $eintrag['Datum']) * $Verbrauch;
		if ($Fuellung)
		{
			$Verbrauch = 0;
			$Verbrauchskosten = 0;
			$Fuellung = false;
		}
		$sql = 'UPDATE ' . TABLE_VERBRAUCH . ' SET Verbrauch=' . $Verbrauch .
		',Verbrauchskosten=' . $Verbrauchskosten .
		' WHERE F_Groesse_id=' . $Groesse . ' AND Datum=' . $eintrag['Datum'];
		  if (!sql_query($sql)) echo sql_error();
		}
		$altwert = $eintrag['Wert'];		
	}
	sql_free_result($query);
}

if ($profile['editor'] != 1)
{
	include (INC_PATH . 'misc/Kalenderjs.inc');
	include (INC_PATH . 'misc/CVJM.inc');
	require (INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('id', $id);
	if ($document['title'] != '')
		echo '<h1>' . transform($document['title'], $document['datatype']['title']) . '</h1>';
	KalenderScriptEinfuegen();
	if ( isset($_REQUEST['Neuberechnen']))
	{
		$Datum = convertDatumToTimeStamp($_REQUEST['Neuberechnen']);
		$query = sql_query('SELECT Groesse_id FROM ' .TABLE_GROESSEN);
		while ($groessen = sql_fetch_array($query))
		{		
		  verbrauchBerechnen($Datum, $groessen['Groesse_id'], 0, false, true);
		  echo $groessen['Groesse_id'];
		}
		sql_free_result($query);
	}
	if (isset ($_REQUEST['SaveValues']))
	{
		// Werte abspeichern
		if (is_array($docinput['Wert']))
		{
			$Datum = convertDatumToTimeStamp($docinput['Datum']);
			if ($Datum == 0)
				$Datum = time();
			foreach ($docinput['Wert'] as $Groesse => $Wert)
			{
				$Wert = str_replace(',', '.', $Wert);
				if (is_numeric($Wert))
				{
					verbrauchBerechnen($Datum, $Groesse, $Wert);
				}
			}
		}
	}
	if (isset ($_REQUEST['Eingabe']))
	{
		// Eingabemaske mit allen Werten anzeigen
		$query = sql_query('SELECT Max(Datum) AS Dat, Bezeichnung, Groesse_id, Ableseeinheit FROM ' .
		TABLE_GROESSEN . ' LEFT JOIN ' . TABLE_VERBRAUCH .
		' ON F_Groesse_id=Groesse_id GROUP BY Bezeichnung ORDER BY Bezeichnung');
		$dieGroessen = array ();
		while ($groessen = sql_fetch_array($query))
		{
			if (!$q = sql_query('SELECT Wert FROM ' . TABLE_VERBRAUCH . ' WHERE Datum=' . $groessen['Dat'] .
				' AND F_Groesse_id=' . $groessen['Groesse_id']))
				sql_error();
			$groessen['werte'] = array ();
			if ($wert = sql_fetch_row($q))
			{
				$groessen['werte'][] = $wert[0];
			}
			$dieGroessen[] = $groessen;
			sql_free_result($q);
		}
		sql_free_result($query);
		$Smarty->assign('groessen', $dieGroessen);
		$Smarty->assign('id', $id);
		setlocale(LC_ALL, "de_DE@euro", "de_DE", "de", "ge");
		$Smarty->display('Verbrauch_Eingabe.tpl');
		setlocale(LC_ALL, '');
	}
	elseif (isset ($docinput['Groesse']) && is_numeric($docinput['Groesse']))
	{
		// Größe bearbeiten
		if (!is_numeric($docinput['Turnus']))
			$docinput['Turnus'] = 0;
		if (!is_numeric($docinput['Faktor']))
			$docinput['Faktor'] = 1;
		$docinput['Preis'] = str_replace(',', '.', $docinput['Preis']);
		if (!is_numeric($docinput['Preis']))
			$docinput['Preis'] = 1;
		$docinput['Wert'] = str_replace(',', '.', $docinput['Wert']);		
		$docinput['Datum'] = convertDatumToTimestamp($docinput['Datum']);
		if ($docinput['Groesse'] == -1)
		{
			// Neue Größe
			$sql = 'INSERT INTO ' . TABLE_GROESSEN . " (Bezeichnung,Einheit,Turnus,Faktor,Ableseeinheit) VALUES ('";
			$sql .= sql_real_escape_string($docinput['Bezeichnung']) . "','" .
			sql_real_escape_string($docinput['Einheit']) . "','" .
			sql_real_escape_string($docinput['Ableseeinheit']) .
			"'," . $docinput['Turnus'] . ',' . $docinput['Faktor'] . ')';
			if (!sql_query($sql))
			  echo '<div class="Fehler">Fehler: ' . sql_error() . ' (' . $sql . ')</div>';			
		} else
		{
			// Vorhandene Größe
			if (isset ($docinput['Einheit']))
			{
				$sql = 'UPDATE ' . TABLE_GROESSEN . " SET Bezeichnung='" . $docinput['Bezeichnung'] .
				"', Einheit='" . sql_real_escape_string($docinput['Einheit']) .
				"', Turnus=" . $docinput['Turnus'] .
				", Ableseeinheit='" . sql_real_escape_string($docinput['Ableseeinheit']) . "'" .
				", Faktor=" . $docinput['Faktor'] . ' WHERE Groesse_id=' . $docinput['Groesse'];
				if (!sql_query($sql))
			  echo '<div class="Fehler">Fehler: ' . sql_error() . ' (' . $sql . ')</div>';
			}
			elseif (isset ($docinput['Hinweis']))
			{
				$sql = 'INSERT INTO ' . TABLE_KOSTEN . ' (F_Groesse_id,Datum,Preis,Hinweis) VALUES (' .
				$docinput['Groesse'] . ',' . $docinput['Datum'] . ',' . $docinput['Preis'] . ",'" .
				sql_real_escape_string($docinput['Hinweis']) . "')";
				if (!sql_query($sql))
			  echo '<div class="Fehler">Fehler: ' . sql_error() . ' (' . $sql . ')</div>';
			}
			elseif (is_numeric($docinput['Wert']))
			{
				// Aktuellen Preis bestimmen
				$fuellung = isset ($docinput['Fuell']);
				verbrauchBerechnen($docinput['Datum'], $docinput['Groesse'], $docinput['Wert'], $fuellung);								
			}
			elseif (isset ($docinput['Del']) && is_numeric($docinput['Del']))
			{
				$sql = 'DELETE FROM ' . TABLE_VERBRAUCH . ' WHERE Wert_id=' . $docinput['Del'];
				if (!sql_query($sql))
			  echo '<div class="Fehler">Fehler: ' . sql_error() . ' (' . $sql . ')</div>';
			}
			if ( isset($docinput['WertVorher']))
			{
				$docinput['WertVorher'] = str_replace(',', '.', $docinput['WertVorher']);
				$docinput['WertNachher'] = str_replace(',', '.', $docinput['WertNachher']);		
				verbrauchBerechnen($docinput['Datum'], $docinput['Groesse'], $docinput['WertVorher'], false);
				verbrauchBerechnen($docinput['Datum'], $docinput['Groesse'], $docinput['WertNachher'], true);
			}
		}
	}
	if (isset ($_REQUEST['Gesamt']))
	{
		if (!$query = sql_query('SELECT * FROM ' . TABLE_GROESSEN . ' INNER JOIN ' . TABLE_VERBRAUCH .
			' ON F_Groesse_id=Groesse_id ORDER BY Datum, Bezeichnung'))
			echo sql_error();
		$Verbrauch = array ();
		$Jahre = array ();
		$JahresWert = array ();
		$MonatGroessen  = array();
		while ($groesse = sql_fetch_array($query))
		{
			$zaehldatum = strtotime('-1 month', $groesse['Datum']);
			if ( ! isset($MonatGroessen[$groesse['Groesse']][date('YM', $zaehldatum)]))
				{
					$MonatGroessen[$groesse['Groesse']][date('YM', $zaehldatum)] = true;
				}
				else
				{
					$zaehldatum = $groesse['Datum'];
				}
			if (!isset ($Verbrauch[$groesse['Bezeichnung']][date('Y', $zaehldatum)]))
			{
				$Verbrauch[$groesse['Bezeichnung']][date('Y', $zaehldatum)]['Kosten'] = 0;
				$Verbrauch[$groesse['Bezeichnung']][date('Y', $zaehldatum)]['Verbrauch'] = 0;
				if (!in_array(date('Y', $zaehldatum), $Jahre))
				{
					$Jahre[] = date('Y', $zaehldatum);
					$JahresWert[date('Y', $zaehldatum)]['Kosten'] = 0;
					$JahresWert[date('Y', $zaehldatum)]['Verbrauch'] = 0;
				}
			}
			$MonatVerbrauch[$groesse['Bezeichnung']][date('Y', $zaehldatum)][date('M', $zaehldatum)]['Kosten'] += $groesse['Verbrauchskosten'];
			$MonatVerbrauch[$groesse['Bezeichnung']][date('Y', $zaehldatum)][date('M', $zaehldatum)]['Verbrauch'] += $groesse['Verbrauch'];
			$Verbrauch[$groesse['Bezeichnung']][date('Y', $zaehldatum)]['Kosten'] += $groesse['Verbrauchskosten'];
			$Verbrauch[$groesse['Bezeichnung']][date('Y', $zaehldatum)]['Verbrauch'] += $groesse['Verbrauch'];
			$JahresWert[date('Y', $zaehldatum)]['Kosten'] += $groesse['Verbrauchskosten'];
			$JahresWert[date('Y', $zaehldatum)]['Verbrauch'] += $groesse['Verbrauch'];
		}
		sql_free_result($query);
		$Smarty->assign('Ueberschrift', $_REQUEST['Gesamt']);
		$Smarty->assign('Verbrauch', $Verbrauch);
		$Smarty->assign('Jahre', $Jahre);
		$Smarty->assign('JahresWert', $JahresWert);
		$Smarty->assign('MonatVerbrauch', $JahresWert);
		$Smarty->display('Verbrauch_jahr.tpl');
	}
	if (isset ($_REQUEST['Groesse']) && is_numeric($_REQUEST['Groesse']))
	{
		$query = sql_query('SELECT * FROM ' . TABLE_GROESSEN . ' WHERE Groesse_id=' . $_REQUEST['Groesse']);
		if (!$Groesse = sql_fetch_array($query))
		{
			$Groesse['Bezeichnung'] = 'Neue Verbrauchsgröße';
			$Groesse['Faktor'] = 1;
		}
		sql_free_result($query);
		if (isset ($_REQUEST['Stat']))
		{
			// Daten für Template vorbereiten
			$Smarty->assign('Ueberschrift', $Groesse['Bezeichnung']);
			$query = sql_query('SELECT * FROM ' . TABLE_VERBRAUCH . ' WHERE F_Groesse_id=' .
			$Groesse['Groesse_id'] . ' ORDER BY Datum ASC');
			$MonatVerbrauch = array ();
			$MonatGroessen = array();
			while ($wert = sql_fetch_array($query))
			{
				$zaehldatum = strtotime('-1 month', $wert['Datum']);													
				if ( ! isset($MonatGroessen[$groesse['Groesse']][date('YM', $zaehldatum)]))
				{
					$MonatGroessen[$wert['Groesse']][date('YM', $zaehldatum)] = true;
				}
				else
				{
					$zaehldatum = $wert['Datum'];
				}
				if (!isset ($MonatVerbrauch[date('Y', $zaehldatum)]))
				{
					// Initialisierung der Monatsabrechnungen
					$MonatVerbrauch[date('Y', $zaehldatum)] = array ();
					for ($i = 1; $i < 13; $i++)
					{
						$MonatVerbrauch[date('Y', $zaehldatum)][$i]['Kosten'] = 0;
						$MonatVerbrauch[date('Y', $zaehldatum)][$i]['Verbrauch'] = 0;						
					}
				}
				$MonatVerbrauch[date('Y', $zaehldatum)][date('n', $zaehldatum)]['Kosten'] += $wert['Verbrauchskosten'];
				$MonatVerbrauch[date('Y', $zaehldatum)][date('n', $zaehldatum)]['Verbrauch'] += $wert['Verbrauch'];
			}
			$Smarty->assign('MonatVerbrauch', $MonatVerbrauch);
			sql_free_result($query);
			$Smarty->display('Verbrauch_groesse.tpl');
		}
		elseif (!isset ($_REQUEST['Wert']))
		{
			echo '<form action="' . get_url($id) . '" method="post">';
			echo '<table>';
			echo '<tr><td>Bezeichnung</td><td>';
			echo '<input type="Text" name="docinput[Bezeichnung]" value="' .
			htmlspecialchars($Groesse['Bezeichnung']) . '" size="30" maxlength="30" />';
			echo '</td></tr>';
			echo '<tr><td>Einheit</td><td>';
			echo '<input type="Text" name="docinput[Einheit]" value="' .
			htmlspecialchars($Groesse["Einheit"]) . '" size="20" maxlength="20" />';
			echo '</td></tr>';
			echo '<tr><td>Ableseeinheit</td><td>';
			echo '<input type="Text" name="docinput[Ableseeinheit]" value="' .
			htmlspecialchars($Groesse["Ableseeinheit"]) . '" size="20" maxlength="20" />';
			echo '</td></tr>';
			echo '<tr><td>Faktor</td><td>';
			echo '<input type="Text" name="docinput[Faktor]" value="' .
			htmlspecialchars($Groesse["Faktor"]) . '" size="10" maxlength="10" />';
			echo '</td></tr>';
			echo '<tr><td>Ableseturnus</td><td>';
			echo '<select name="docinput[Turnus]">';
			foreach ($turnusse as $key => $value)
			{
				echo '<option value="' . $key . '" ';
				if ($key == $Groesse["Turnus"])
					echo 'selected="selected"';
				echo '>' . $value . '</option>';
			}
			echo '</select>';
			echo '</td></tr>';
			echo '</table>';
			echo '<input type="hidden" name="docinput[Groesse]" value="' . $_REQUEST["Groesse"] . '"/>';
			echo '<input type="Submit" value="Speichern" />';
			echo "</form>\n";
			if ($_REQUEST["Groesse"] != -1)
			{
				echo 'Kosten pro Einheit:<br />';
				$query = sql_query("SELECT * FROM " . TABLE_KOSTEN . " WHERE F_Groesse_id=" . $_REQUEST["Groesse"]);
				echo '<table >';
				echo '<tr><td>Datum</td><td>Preis/' . $Groesse["Einheit"] . '</td><td>Hinweis</td></tr>';
				while ($kosten = sql_fetch_array($query))
				{
					echo '<tr><td>' . date("d.m.Y", $kosten["Datum"]) . '</td><td align="right">';
					echo $kosten["Preis"] . '</td><td>';
					echo nl2br(htmlentities($kosten["Hinweis"])) . '</td></tr>';
				}
				echo '<form action="' . get_url($id) . '" method="post" name="PreisForm" />';
				echo '<tr><td>';
				echo '<input type="Text" name="docinput[Datum]" value="';
				echo date("d.m.Y") . '" size="10" maxlength="10" ';
				KalenderEvents("PreisForm", "docinput[Datum]");
				echo '/>';
				echo '</td><td>';
				echo '<input type="Text" name="docinput[Preis]" size="5" maxlength="8"/>';
				echo '</td><td>';
				echo '<input type="Text" name="docinput[Hinweis]" size="40" />';
				echo '<input type="Submit" value="Hinzu"/>';
				echo '</td></tr>';
				echo '<input type="hidden" name="docinput[Groesse]" value="' . $_REQUEST["Groesse"] . '"/>';
				echo '</form>';
				echo "</table>\n";
			}
		} else
		{
			// Werte eingeben
			$Smarty->assign('Groesse', $Groesse);
			$query = sql_query("SELECT * FROM " . TABLE_VERBRAUCH . " WHERE F_Groesse_id=" .
			$Groesse["Groesse_id"] . " ORDER BY Datum DESC");
			$Werte = array();
			while ($wert = sql_fetch_array($query))
			{
				$Werte[] = $wert;				
			}
			$Smarty->assign('Werte', $Werte);			
			sql_free_result($query);
			if ( $_REQUEST['Wert'] < 0)
			{
			$Smarty->display('Verbrauch_Eingabe_Daten.tpl');
			}
			else
			{
				$Smarty->display('Verbrauch_Eingabe_Tanken.tpl');
			}
		}
		
	}
		$query = sql_query("SELECT * FROM " . TABLE_GROESSEN . " ORDER BY Bezeichnung");
		echo '<ul class="noprint">';
		while ($groesse = sql_fetch_array($query))
		{
			echo '<li><a href="' . get_url($id, "Groesse=" . $groesse["Groesse_id"]) . '">';
			echo $groesse["Bezeichnung"] . "</a>\n";
			$preis = holeAktuellenPreis($groesse["Groesse_id"]);
			echo ' (' . $preis . " &euro;) ";
			echo '[ <a href="' . get_url($id, array (
				"Groesse" => $groesse["Groesse_id"],
				"Wert" => -1
			)) . '">';
			echo 'Werte eingeben</a> ';
			if ( $groesse['Faktor']<0)
			{
			echo '| <a href="' . get_url($id, array (
				"Groesse" => $groesse["Groesse_id"],
				"Wert" => 1
			)) . '">';
			echo 'Tanken</a> ';
			}
			echo '| <a href="' . get_url($id, array (
				"Groesse" => $groesse["Groesse_id"],
				"Stat" => 1
			)) . '">';
			echo 'Statistik</a> ';
			echo '</li>';
		}
		sql_free_result($query);
		echo '<li><a href="' . get_url($id, "Groesse=-1") .
		'">Neue Verbrauchsgröße hinzufügen</a></li>';
		echo '</ul>';
		echo '<div class="noprint">';
		echo '[ <a href="' . get_url($id, "Eingabe=Alle") . '">Eingabemaske für alle Werte</a> ] ';
		echo '[ <a href="' . get_url($id, "Gesamt=Verbrauch") . '">Gesamtstatistik</a> ] ';
		echo '</div>';	
}
?>
