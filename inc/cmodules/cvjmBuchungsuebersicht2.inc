<?php
/**

Modul BuchungsÜbersicht monatlich

AWF-Name: cvjmbuchungsuebersicht2

Parameter:
Tag - erster Tag der Anzeige
Tage - Anzahl der anzuzeigenden Tage
docinput[Kategorie][] - Kategorien, die angezeigt werden sollen
*/

/* Anzahl der Tage die vor dem aktuellen Datum angezeigt werden */
DEFINE('VORLAUF', 3);
if ($profile['editor'] != 1)
{
	// Anzahl Tage die angezeigt werden
	if (isset ($_REQUEST['Tage']) && is_numeric($_REQUEST['Tage']) && $_REQUEST['Tage'] >= 10)
		define('ANZAHLTAGE', $_REQUEST['Tage']);
	else
		define('ANZAHLTAGE', 30);

	include (INC_PATH . 'misc/CVJM.inc');
	include (INC_PATH . 'misc/cvjmArtikeltools.inc');
	include (INC_PATH . 'misc/cvjmBuchungstools.inc');
	include (INC_PATH . 'misc/Kalenderjs.inc');
    require_once (INC_PATH.'Smarty/Smarty.class.php');
    require_once (INC_PATH.'cvjm/Feiertag.class.php');
  $Smarty = new Smarty;
    $Smarty->template_dir = TEMPLATEPATH; //'/srv/www/htdocs';
    setlocale(LC_ALL, "de_DE@euro", "de_DE", "de", "ge");
    
	$BuchungFarbe = array ();
	$Tag = mktime(0, 0, 0, date('m', time()), date('d', time()), date('Y', time()));
	$Smarty->assign('Title', transform($document['title'], $document['datatype']['title']));
	if (isset ($_REQUEST['Tag']) && is_numeric($_REQUEST['Tag']))
		$Tag = $_REQUEST['Tag'];
	if (isset ($_REQUEST['Datum']))
	{
		$Tag = convertDatumToTimestamp($_REQUEST['Datum']);
		$dat = '&Datum=' . $_REQUEST['Datum'];
	}
	// Auswahlfelder für Arten
	$Kategorien = array ();
	if (!isset ($docinput['Kategorie']))
		$docinput['Kategorie'] = explode(',', $profile['Kategorie' . $id]);
	else
		set_profile('Kategorie' .
		$id, implode(',', $docinput['Kategorie']));	
        $Smarty->assign('docinput', $docinput);
	if (!$query = sql_query('SELECT * FROM ' . TABLE_ARTIKEL .
		' WHERE Einruecken=0 AND F_Art_id IN (' . CVJMART_ORT . ',' . CVJMART_VERPFLEGUNG . ',' .
		CVJMART_PROGRAMM . ',' . CVJMART_VERLEIH . ',' . CVJMART_PROGRAMMPAKET . ') ORDER BY Bezeichnung'))
		echo 'Fehler Kategorie: ' . sql_error();
	while ($kategorie = sql_fetch_array($query))
	{
		$Kategorienamen[$kategorie['id']] = $kategorie['Bezeichnung'];		
		$Kategorien[$kategorie['id']] = $kategorie;
	}
	sql_free_result($query);
	$Smarty->assign('Kategorien', $Kategorienamen);
	$Smarty->assign('id', $id);
	$Smarty->assign('Buchungurl', get_url(get_cmodule_id('cvjmbuchung')));	
	$Smarty->assign('params', "Tag=$Tag&Tage=".ANZAHLTAGE);
	$Smarty->assign('anzahltage', ANZAHLTAGE);
	$Smarty->assign('Tag', $Tag);
	$Smarty->assign('TageZurueck', strtotime('-5 day', $Tag));
	$Smarty->assign('TageVor', strtotime('+5 day', $Tag));
	$Tage = array ();
	$feiertag = array();
	for ($i = 0; $i < ANZAHLTAGE; $i++)
	{
		if ($i -VORLAUF < 0)
			$Plus = '';
		else
			$Plus = '+';
		$datum = strtotime($Plus . ($i -VORLAUF) . ' day', $Tag);
		$Tage[] = $datum;
		$feiertag[] = Feiertag::isFeiertag($datum);
	}
	$Smarty->assign('Tage', $Tage);
	$Smarty->assign('feiertag', $feiertag);
	$Smarty->assign('Artikelurl', get_url(get_cmodule_id('cvjmartikel')));
	$Artikel = array ();
	foreach ($docinput['Kategorie'] as $kategorie)
	{
	        $Artikel[$kategorie] = array ();
		$Artikel[$kategorie]['Bezeichnung'] = $Kategorien[$kategorie]['Bezeichnung'];
		$Artikel[$kategorie]['Artikel'] = array();
		// Zeigt alle Orte als Baum an
		$sql = 'SELECT id, Bezeichnung, Beschreibung, Einruecken FROM ' . TABLE_ARTIKEL .
		' WHERE Position > ' . $Kategorien[$kategorie]['Position'] . ' AND Einruecken>0 AND Position<' .
		findeArtikelPos($Kategorien[$kategorie], 'r') .
		' AND F_Art_id=' . $Kategorien[$kategorie]['F_Art_id'] .
		' AND Anzeigen ORDER BY Position';
		$qu = sql_query($sql);
		while ($row = sql_fetch_array($qu))
		{
			$row['Pfad'] = getOrtsName($row['id'], $row['Einruecken'] > 1);
			$sql = 'SELECT Buchung_Nr, Datum, Name, Zusatz, Ort, BStatus FROM (' . TABLE_BUCHUNGEN .
			' INNER JOIN ' . TABLE_ADRESSEN .
			' ON F_Adressen_id = Adressen_id) INNER JOIN ' . TABLE_BUCHUNGSEINTRAEGE .
			' ON F_Buchung_Nr=Buchung_Nr WHERE F_Artikel_Nr = ' . $row['id'] .
			' AND Datum BETWEEN ' . strtotime(-VORLAUF . ' day', $Tag) . ' AND ' .
			strtotime('+' . ANZAHLTAGE . ' day', $Tag) .
			' ORDER BY Datum, Buchung_Nr';
			$buchungen = sql_query($sql);
			if (!$buchung = sql_fetch_array($buchungen))
				unset ($buchung);
			$row['Tage'] = array ();
			// Tage mit Belegung anzeigen
			for ($i = 0; $i < ANZAHLTAGE; $i++)
			{
				$row['Tage'][$i] = array ();
				while (isset ($buchung) && $buchung['Datum'] >= strtotime(($i -VORLAUF) . ' day', $Tag) && $buchung['Datum'] < strtotime(($i +1 - VORLAUF) . ' day', $Tag))
				{
					// Durchnummerierung der Buchung für Farben
					if (!isset ($BuchungFarbe[$buchung['Buchung_Nr']]) && Count($BuchungFarbe) < Count($Belegungsfarbe))
						$BuchungFarbe[$buchung['Buchung_Nr']] = $Belegungsfarbe[Count($BuchungFarbe)];
					$buchung['Color'] = $BuchungFarbe[$buchung['Buchung_Nr']];
					if ($buchung['BStatus'] <= BUCHUNG_VORRESERVIERUNG)
						$buchung['Status'] = ' / Vorreservierung!';
					$row['Tage'][$i][] = $buchung;
					if (!$buchung = sql_fetch_array($buchungen))
					{
						sql_free_result($buchungen);
						unset ($buchung);
					}
				}
			}
			$Artikel[$kategorie]['Artikel'][] = $row;
		}
		sql_free_result($qu);		
	}
	$Smarty->assign('Anzeige', $Artikel);
	$Smarty->display('Buchungsuebersicht_kurz.tpl');
} // wenn nicht Editor
?>
