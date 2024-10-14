<?php

/**
 *
 * Modul BuchungsÜbersicht
*
* AWF-Name: cvjmbuchungsuebersicht
*
* Parameter:
* Buchung_Nr - Nummer der anzuzeigenden Buchung; -1 für allgemeine Übersicht
* Tag - erster Tag der Anzeige
* Tage - Anzahl der anzuzeigenden Tage
* docinput[Page] - id der Kategorie, die angezeigt werden soll
* docinput[Alle] - 1 wenn alle Orte (unabhängig von "Anzeigen") angezeigt werden sollen
* docinput[Art] - Art der Artikel, die ausgewählt werden soll, wenn keine Angabe = Ort
*
 */
require_once(INC_PATH.'cvjm/Artikel.class.php');
require_once(INC_PATH.'cvjm/Buchung.class.php');
// dieArt Standard CVJMART_ORT
function ArtikelAnzeigen($DieArt, $Tag, $parent = -1)
{
	global $Artikel;
	global $Schlafplaetze;
	global $docinput;
	global $Anzahl;
	global $design;
	global $buchung;
	global $Alle;
	global $Buchungszahl;
	global $Belegungsfarbe;

	$Wann = array ();
	if (!is_numeric($parent))
	{
	return 0;
	}
	$query = sql_query("SELECT * FROM " . TABLE_ARTIKEL . " WHERE id=" . $parent);
	if (!$row = sql_fetch_array($query))
	{
	echo "Fehler p=$parent: " . sql_error();
	}
	sql_free_result($query);
	$parentPos = $row["Position"];
	$nextPos = findeArtikelPos($row, "r");
	// Zeigt alle Orte als Baum an
	$sql = "SELECT id, Bezeichnung, Anzeigen, Einruecken, Beschreibung FROM " . TABLE_ARTIKEL .
	" WHERE Position BETWEEN " . ($parentPos +1) . " AND " . ($nextPos -1) . " AND F_Art_id = " .
	$DieArt . " AND Anzeigen ORDER BY Position"; // TODO: $Alle wieder rein
	$qu = sql_query($sql);
	//$Ebenen = array ();
	//$LetztesEinruecken = 0;
	$BuchungFarbe = array ();
	$Artikel = array ();
	while ($row = sql_fetch_array($qu))
	{
		$a = new Artikel($row['id']);

		$row['Pfad'] = implode(' > ',$a->getArtikelPfad($a->Einruecken > 1));
		
		$docinput["Artikelnummern"] .= $a->Artikel_id . ",";
		if ($buchung["Buchung_Nr"] > 0)
		{
			$sql = "SELECT Count(*) FROM " . TABLE_BUCHUNGSEINTRAEGE .
			" WHERE F_Artikel_Nr=" . $a->Artikel_id . " AND F_Buchung_Nr = " . $buchung["Buchung_Nr"];
			$query = sql_query($sql);
			$anzahl = sql_fetch_row($query);
			sql_free_result($query);
			$row['anzahl'] = $anzahl[0];
		}
		// Tage mit Belegung anzeigen
		$sql = "SELECT F_Buchung_Nr, Menge, Datum, Name, Zusatz, Ort, BStatus FROM " . TABLE_BUCHUNGSEINTRAEGE .
		" INNER JOIN " . TABLE_BUCHUNGEN . " ON F_Buchung_Nr = Buchung_Nr INNER JOIN " . TABLE_ADRESSEN .
		" ON F_Adressen_id = Adressen_id WHERE F_Artikel_Nr = " . $a->Artikel_id .
		" AND Datum BETWEEN $Tag AND " . strtotime("+" . ($_SESSION['ANZAHLTAGE'] - 1) . " days", $Tag) . " ORDER BY Datum, Eingang";
		$buchungen = sql_query($sql);
		for ($i = 0; $i < $_SESSION['ANZAHLTAGE']; $i++)
		{
			$row['tage'][strtotime('+'.$i.'days',$Tag)]['Buchungen'] = array ();
			$row['tage'][strtotime('+'.$i.'days',$Tag)]['Aktiv'] = array ();
		}
		while ($diebuchung = sql_fetch_array($buchungen))
		{
			// Sommerzeit!
			$tag = floor(($diebuchung['Datum'] - $Tag) / 86400);
			if ( ($diebuchung['Datum'] - $Tag) / 86400 != $tag && date('n', $Tag) <= 4)
			{
				$tag = ceil(($diebuchung['Datum'] - $Tag) / 86400);
			}
			if ($diebuchung['F_Buchung_Nr'] == $buchung['Buchung_Nr'])
			{
				if ($docinput['Art'] == CVJMART_ORT)
				{
					$Schlafplaetze[strtotime('+'.$tag.'days',$Tag)] += Max(0,$a->BerechneSchlafplaetze());
				}
				$diebuchung['Farbe'] = $design['table_header'];
				$row['tage'][strtotime('+'.$tag.'days',$Tag)]['Aktiv'][] = $diebuchung;
			} else
			{
				$Buchungszahl = $diebuchung['F_Buchung_Nr'];
				if (!isset ($BuchungFarbe[$Buchungszahl]) && Count($BuchungFarbe) < Count($Belegungsfarbe))
				{
					$BuchungFarbe[$Buchungszahl] = $Belegungsfarbe[Count($BuchungFarbe)];
				}
				if ($diebuchung['BStatus'] <= BUCHUNG_VORRESERVIERUNG)
				$diebuchung['Status'] = ' / Vorreservierung !';
				$diebuchung['Farbe'] = $BuchungFarbe[$Buchungszahl];
				$row['tage'][strtotime('+'.$tag.'days',$Tag)]['Buchungen'][] = $diebuchung;
			}
		}
		$Anzahl++;
		$Artikel[] = $row;
	}
	sql_free_result($qu);
}

function TagDefinition($buchung = '')
{
	
	if (isset ($_REQUEST['Tage']) && is_numeric($_REQUEST['Tage']) && $_REQUEST['Tage'] >= 10)
	{
		$_SESSION['ANZAHLTAGE'] = $_REQUEST['Tage'];
	}
	elseif ($buchung == '')
	{
		$_SESSION['ANZAHLTAGE'] = 10;
	} else
	{
		$Dauer = round(berechneUebernachtungen($buchung));
		if (!isset($_SESSION['ANZAHLTAGE']) || $Dauer +3 > $_SESSION['ANZAHLTAGE'])
		{
			$_SESSION['ANZAHLTAGE'] = $Dauer +3;
		}
		elseif ( $_SESSION['Buchung'] != $buchung['Buchung_Nr'] )
		{
			$_SESSION['ANZAHLTAGE'] = 10;
		}
	}
}

if ($profile['editor'] != 1)
{

	include (INC_PATH . 'misc/CVJM.inc');
	include (INC_PATH . 'misc/cvjmArtikeltools.inc');
	include (INC_PATH . 'misc/cvjmBuchungstools.inc');

	include (INC_PATH . 'cvjm_ajax.php');
	$xajax->printJavascript('./javascript');

	$Anzahl = 0;
	// Wenn nichts angegeben dann &Uuml;bersicht anzeigen
	if (!isset ($_REQUEST['Buchung_Nr']) || !is_numeric($_REQUEST['Buchung_Nr']))
	{
	$_REQUEST['Buchung_Nr'] = -1;
	}


	// Wenn nichts anderes angegeben suchen wir nach Orten
	if (!isset ($docinput['Art']) || !is_numeric($docinput['Art']))
	{
		if ( isset($_SESSION['docinput[Art]']))
		{
		$docinput['Art'] = $_SESSION['docinput[Art]'];
		}
		else
		{
		$docinput['Art'] = CVJMART_ORT;
		unset($_SESSION['docinput[Page]']);
		}
	}
	else
	{
		unset($_SESSION['docinput[Page]']);
	}
	// Wir stellen fest, nach welchem Modus die Art abgerechnet wird.
	$Abrechnungstyp = getAbrechnungstyp($docinput['Art']);

	if (isset ($_REQUEST['Buchung_Nr']) && is_numeric($_REQUEST['Buchung_Nr']) && isset ($_REQUEST['Save']))
	{
		// Buchungen abspeichern
		if (!$query = sql_query('SELECT * FROM ' . TABLE_BUCHUNGEN . ' WHERE Buchung_Nr=' .
		$_REQUEST['Buchung_Nr']))
		{
			echo '<div class="SchwererFehler">Fehler Buchung: ' . sql_error() . "</div>";
		}
		$buchung = sql_fetch_array($query);
		sql_free_result($query);

		TagDefinition($buchung);
		// vorhandene Buchungen im Bereich löschen
		foreach (explode(",", $docinput["Artikelnummern"]) as $key => $value)
		{
			entbucheArtikel($_REQUEST['Buchung_Nr'], $value, $_REQUEST["Tag"] . "-" .
			strtotime("+" . ($_SESSION['ANZAHLTAGE'] - 1) . " day", $_REQUEST["Tag"]), 0);
		}
		$Artikel = $_REQUEST["Artikel"];
		$paketArtikel = array ();
		foreach ( $Artikel as $key => $value )
		{
			foreach ( $value as $datum => $wann )
			{
				if (is_numeric($datum))
				{
					$Anzahl = 0;
					if ($docinput['Art'] == CVJMART_PROGRAMM || $docinput['Art'] == CVJMART_VERLEIH)
					{
						// Mit Zeitangabe!
						if (is_numeric($wann))
						{
							while (strlen($wann) < 4)
							$wann = '0' . $wann;
							$wann = substr($wann, 0, 2) . ':' . substr($wann, 2, 2);
						}
						if (strpos($wann, ':') !== false)
						{
							$datum = strtotime(date('Y-m-d', $datum) . ' ' . $wann);
						} else
						{
							$datum = 0;
						}
					}
					if ($datum != 0)
					{
						if ($Abrechnungstyp != ABRECHNUNGSTYP_KOPFNACHT && $Abrechnungstyp != ABRECHNUNGSTYP_KOPF && $Abrechnungstyp != ABRECHNUNGSTYP_VERPFLEGUNG && $Abrechnungstyp != ABRECHNUNGSTYP_VERPFLEGUNGNACHT && $Abrechnungstyp != ABRECHNUNGSTYP_VERPFLEGUNGPAUSCHAL)
						{
							$Anzahl = 1;
						} else
						{
							$Anzahl = Personenanzahl($buchung);
							if ($docinput['Art'] == CVJMART_PROGRAMM)
							{
								$oberartikel = pruefeAlleOberartikel($key, $buchung, TABLE_ARTIKELGRUPPEN);
								if ($oberartikel >= 0)
								{
									// Anzahl feststellen
									if (!$anzquery = sql_query("SELECT Menge FROM " . TABLE_BUCHUNGSEINTRAEGE .
									" WHERE F_Artikel_Nr=$oberartikel AND F_Buchung_Nr=" . $buchung["Buchung_Nr"]))
									{
										echo sql_error();
									}
									if (!$anz = sql_fetch_row($anzquery))
									{
										echo sql_error();
									}
									sql_free_result($anzquery);
									if ($anz[0] != 0)
									{
										$Anzahl = $anz[0];
									}
								}
							}
						}
					}
					if ($docinput['Art'] == CVJMART_PROGRAMMPAKET)
					{
						// Anzahl wird angegeben
						if (is_numeric($wann))
						{
							$Anzahl = $wann;
						} else
						{
							$Anzahl = 0;
						}
					}
					if ($Anzahl > 0)
					{
						if ($docinput['Art'] == CVJMART_PROGRAMMPAKET)
						{
							if (!in_array($key, $paketArtikel))
							{
								bucheArtikel($_REQUEST['Buchung_Nr'], $key, $datum, $Anzahl);
							}
						} else
						{
							bucheArtikel($_REQUEST['Buchung_Nr'], $key, $datum, $Anzahl);
						}
						$paketArtikel[] = $key;
					}
				}
			}
				
		} // wenn nicht Gesamt
		$Artikelarten = holeArtikelarten();
		if (!sql_query('UPDATE ' . TABLE_BUCHUNGEN . " SET Logtext=CONCAT('" . date("d.m.Y H:i") .
		" " . $Artikelarten[$docinput["Art"]] . " geändert " . get_user_nickname() . "\n',Logtext) " .
		" WHERE Buchung_Nr=" . $_REQUEST["Buchung_Nr"]))
		{
		echo sql_error();
		}
	}
	require_once(INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	if ( isset($docinput['Page']))
	{
		$_SESSION['docinput[Page]'] = $docinput['Page'];
	}
	$docinput['Page'] = $_SESSION['docinput[Page]'];
	
	if (isset ($_REQUEST["Buchung_Nr"]) && is_numeric($_REQUEST["Buchung_Nr"]))
	{
		holeArtikelarten();
		// sinnlose Seite dann lassen wir die Seite automatisch holen
		if (isset ($docinput["Page"]) && trim($docinput["Page"]) == '')
		{
			unset ($docinput["Page"]);
		}
		if ( isset($_SESSION['Tag']))
		{
			$Tag = $_SESSION['Tag'];
		}
		else
		{
			$Tag = mktime(0, 0, 0, date("m", time()), date("d", time()), date("Y", time()));
		}
		if (isset ($_REQUEST["Tag"]) && is_numeric($_REQUEST["Tag"]))
		{
			$Tag = $_REQUEST["Tag"];
		}
		if (isset ($_REQUEST["Datum"]))
		{
			$dat = "&Datum=" . $_REQUEST["Datum"];
			$Tag = convertDatumToTimestamp($_REQUEST["Datum"]);
		} else
		{
			$dat = "";
		}
		if ($_REQUEST["Buchung_Nr"] >= 0)
		{
			if (!$query = sql_query("SELECT * FROM " . TABLE_BUCHUNGEN . " INNER JOIN " . TABLE_ADRESSEN .
			" ON Adressen_id = F_Adressen_id WHERE Buchung_Nr = " . $_REQUEST["Buchung_Nr"]))
			{
				echo sql_error();
			}
			$buchung = sql_fetch_array($query);
			sql_free_result($query);
			$Buchung = new Buchung($_REQUEST['Buchung_Nr']);
			$Menge = $Buchung->standardMenge($Abrechnungstyp);
			
			// Anzahl Tage die angezeigt werden
			TagDefinition($buchung);
			if ( $_SESSION['Buchung'] != $_REQUEST['Buchung_Nr'] || ! isset($_SESSION['Tag']))
			{
				$Tag = strtotime("-1 day", $Buchung->Von);
			}
			
			// Kopf der Buchung: Die wichtigsten Informationen
			ob_start();
			ZeigeBuchungskopf($buchung);
			$Smarty->assign('Buchungskopf', ob_get_contents());
			ob_end_clean();
			$dat = '&Buchung_Nr=' . $Buchung->Buchung_Nr . $dat;
		} else
		{
			TagDefinition();
			$buchung['Buchung_Nr'] = -1;
			$Buchung = new Buchung(-1,-1);
		}
		$Smarty->assign('Title', transform($document['title'], $document['datatype']['title']));
		$Links = array ();
		$Links['Belegung'] = "docinput[Art]=" . CVJMART_ORT . $dat;
		if ($Buchung->personenanzahl() > 0)
		{
			$Links['Programmpaket'] = "docinput[Art]=" . CVJMART_PROGRAMMPAKET . $dat;
			$Links['Programmmodul'] = "docinput[Art]=" . CVJMART_PROGRAMM . $dat;
			$Links['Verpflegung'] = "docinput[Art]=" . CVJMART_VERPFLEGUNG . $dat;
			$Smarty->assign('Personalplanungurl', get_url(get_cmodule_id('cvjmpersonalplanung'), 'docinput[Buchung_Nr]='.$Buchung->Buchung_Nr));
		}
		$Links['Verleih'] = "docinput[Art]=" . CVJMART_VERLEIH . $dat;		
		$Smarty->assign('Links', $Links);
		$Smarty->assign('dat', $dat);
		$Smarty->assign('Programmpaket', $docinput['Art'] == CVJMART_PROGRAMMPAKET);
		$Smarty->assign('mitZeit', $docinput["Art"] == CVJMART_VERLEIH || 
		$docinput["Art"] == CVJMART_PROGRAMM || $docinput["Art"] == CVJMART_PROGRAMMPAKET);

		// Reiter aufbauen
		if (!isset ($docinput["Alle"]))
		{
			$Alle = " AND Anzeigen";
		}
		else
		{
			$Alle = "";
		}
		if (!$query = sql_query("SELECT * FROM " . TABLE_ARTIKEL . " WHERE Einruecken=0 AND F_Art_id=" .
		$docinput["Art"] . " $Alle ORDER BY Position"))
		{
			echo "Fehler Kategorie: " . sql_error();
		}
		$s = ZeigeBereiche($docinput["Art"], $Buchung->Buchung_Nr);
		if ($s != "")
		{
			$BelegteBereiche = explode(",", $s);
		}
		else
		{
			$BelegteBereiche = array ();
		}
		$Kategorien = array ();
		while ($kategorie = sql_fetch_array($query))
		{
			// wenn kein Reiter gewählt ist den ersten anzeigen in dem eine Buchung ist
			if (!isset ($docinput["Page"]) && Count($BelegteBereiche) > 0)
			{
				if (in_array($kategorie["Bezeichnung"], $BelegteBereiche))
				{
					$docinput["Page"] = $kategorie["id"];
				}
			}
			elseif (!isset ($docinput["Page"])) $docinput["Page"] = $kategorie["id"];
			if ($docinput["Page"] == $kategorie["id"])
			{
				$kategorie['Color'] = "helleZeile";
				$kategorie['Class'] = "selectedNav";
				if (in_array($kategorie["Bezeichnung"], $BelegteBereiche))
				{
					$kategorie['Color'] = "ueberschrift";
				}
			} else
			{
				$kategorie['Color'] = "dunkleZeile";
				$kategorie['Class'] = "normalNav";
				if (in_array($kategorie["Bezeichnung"], $BelegteBereiche))
				{
					$kategorie['Color'] = "ueberschrift";
				}
			}
			$Kategorien[] = $kategorie;
		}
		$Smarty->assign('Artikelurl', get_url(get_cmodule_id('cvjmartikel')));
		$Smarty->assign('Kategorien', $Kategorien);
		$Smarty->assign('default_background', $design["default_background"]);
		// Template
		$_SESSION['Buchung'] = $_REQUEST['Buchung_Nr'];
		$_SESSION['Tag'] = $Tag;
		$_SESSION['Art'] = $docinput['Art'];
		$_SESSION['docinput[Page]'] = $docinput['Page'];
		$_SESSION['docinput[Art]'] = $docinput['Art'];
		$Smarty->assign('Artikelart', $Artikelarten[$docinput['Art']]);
		$Tage = array ();
		for ($i = 0; $i < $_SESSION['ANZAHLTAGE']; $i++)
		{
			$Tage[] = strtotime('+'.$i.' day', $Tag);
		}
		$Smarty->assign('Tage', $Tage);

		$docinput['Artikelnummern'] = '';
		for ($i = 0; $i < $_SESSION['ANZAHLTAGE']; $i++)
		{
			$Schlafplaetze[strtotime('+'.$i.'days',$Tag)] = 0;
		}
		$Artikel = array();
		ArtikelAnzeigen($docinput["Art"], $Tag, $docinput["Page"]);
		$Smarty->assign('Artikel', $Artikel);
		if ($docinput['Art'] == CVJMART_ORT && isset ($_REQUEST['Buchung_Nr']) && $_REQUEST['Buchung_Nr'] > 0)
		{
			$Smarty->assign('Schlafplaetze', $Schlafplaetze);
		}
		// Komma am Ende abschneiden
		if (strlen($docinput["Artikelnummern"]) > 0)
		{
			$docinput["Artikelnummern"] = substr($docinput["Artikelnummern"], 0, strlen($docinput["Artikelnummern"]) - 1);
		}
			
		$Smarty->assign('docinput', $docinput);
		$Smarty->assign('id', $id);
		$Smarty->assign('params', 'Buchung_Nr='.$Buchung->Buchung_Nr);
		$Smarty->assign('Tag', $Tag);
		$Smarty->assign('TageVor', strtotime('+5 days', $Tag));
		$Smarty->assign('TageZurueck', strtotime('-5 days', $Tag));
		$Smarty->assign('AnzeigeEnde', strtotime('+'.$_SESSION['ANZAHLTAGE'].' days',$Tag));
		$Smarty->assign('menge',  $Menge);
		$buchung['Fertig'] = $Buchung->isFertig();
		$Smarty->assign('buchung', $buchung);
		$Smarty->assign('Buchungurl', get_url(get_cmodule_id('cvjmbuchung')));
		setlocale(LC_ALL, "de_DE@euro", "de_DE", "de", "ge");
		$Smarty->display('Buchungsuebersicht.tpl');
	}
} // wenn nicht Editor
?>
