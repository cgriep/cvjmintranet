<?php
/**
 *
 *Adressen
 *
 *Parameter:
 *Search - Wird in den Feldern Name, Vorname, Straße, Ort gesucht
 *Sort - Feld, nachdem sortiert wird
 *Seite - Anzuzeigende Seite des Ergebnisses
 *docinput[AnzahlProSeite] - Setzt die pro Seite anzuzeigenden Datens&auml;tze (Userspezifisch)
 *Bearbeiten - Nummer der Adresse (Adressen_id), die bearbeitet werden soll
 *docinput[Kategorie] - Nummer der Kategorie (Kategorie_id) die angezeigt werden soll
 *docinput[Institution] - Nummer der Institution (Adressen_id) deren Ansprechpartner
 *		       angezeigt werden sollen
 *docinput[Zugehoerig] - Nummer der Adresse (Adressen_id) deren Institutionen angezeigt
 *		       werden sollen
 *docinput[Edit] - Editieren der Kategoriezugehörigkeiten (1), Zugehoerig (2) oder
 *                 Institutionen(3), Versandmarker (4)
 *docinput[Adressen]  (Adressen_id die in die Kategorie gesetzt werden sollen
 *docinput[AdressenNr] bei der Zuordnung von Kategorien (letzteres alle Nr. in der Ansicht)
 *KategorieNeu - Neue Kategorie hinzuf&uuml;gen (-1) oder bearbeiten, dann Kategorie_id
 *Delete - Adressennummer: Anfrage ob gel&ouml;scht werden soll
 *DelOk (Adressennummer) und DelMOk (minus Adressennummer) - L&ouml;schen einer Adresse
 *Kopie : Erstellt eine Kopie des Datensatzes (Kat=O, oberhalb des Originals =U Unterhalb)
 *
 *Optionen:
 *Personal - 1: Nur Kategorie entsprechend des angemeldeten Nickname wird angezeigt
 *Personal - 0: Alle Kategorien werden angezeigt, Auswahl ist m&ouml;glich
 *
 *AWF
 *Modulname: cvjmAdressen
 *Benutzte Module: cvjmbuchung
 *
 *User-Profil:
 *AdressauswahlKategorie - merkt sich die letzte gew&auml;hlte Kategorie
 *AdressenAnzahlProSeite - Anzahl der pro Seite angezeigten Adressen
 *
 *Tabellen:
 *cvjm_Kategorien
 *cvjm_Adressen
 *cvjm_Adressen_Kategorie
 *cvjm_Adressenbemerkungen
 *cvjm_Adressen_Institutionen
 *
 */

if ($profile['editor'] != '1')
{
	include (INC_PATH . 'misc/CVJM.inc');
	require_once (INC_PATH.'Smarty/Smarty.class.php');
	include_once (INC_PATH . 'cvjm/Adresse.class.php');
	include_once (INC_PATH . 'cvjm_ajax.php');
	$xajax->printJavascript('./javascript');

	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('id', $id);

	if (isset ($_REQUEST['Sem']))
	{
		$_SESSION['Sem'] = $_REQUEST['Sem'];
	}
	if (isset ($_REQUEST['Versand']))
	{
		$Versandmarker = $_REQUEST['Versand'];
		$docinput['Edit'] = 4;
		set_profile('Adressen_Versandmarker', $Versandmarker);
	} else
	{
		$Versandmarker = $profile['Adressen_Versandmarker'];
	}
	if ($document['Personal'] == 1 && isset ($_REQUEST['Bearbeiten']))
	{
		// hack: Damit nicht immer im Persönlichen id der unpersönlichen Seite setzen
		if (!$qres = sql_query('SELECT ' . TABLE_NODES . '.id FROM ' . TABLE_NODES . ' INNER JOIN ' . TABLE_NODEDATA .
		' ON node_id=' . TABLE_NODES . ".id WHERE value<>'1' AND name='Personal' AND type_id = " .
		$doctypes['cvjmadressen']))
		{
			echo sql_error();
		}
		if ($node = sql_fetch_row($qres))
		{
			$id = $node[0];
		}
		sql_free_result($qres);
	}
	if (isset ($_REQUEST['DelOk']) && isset ($_REQUEST['DelMOk']))
	{
		// Löschen einer Adresse
		if (is_numeric($_REQUEST['DelMOk']))
		{
			if ($_REQUEST['DelOk'] == - $_REQUEST['DelMOk'])
			{
				$Adresse = new Adresse($_REQUEST['DelOk']);
				$Adresse->loeschen();
				echo '<div class="Erledigt">&gt;&gt;&gt; Adresse ' . $_REQUEST['DelOk'] . ' gelöscht.</div>';
			}
		}
	}
	if (isset ($docinput['KategorieName']) && trim($docinput['KategorieName']) != '')
	{
		if (isset ($docinput['Kategorie']) && is_numeric($docinput['Kategorie']))
		{
			if (sql_query('UPDATE ' . TABLE_KATEGORIEN . " SET Kategorie='" .
			sql_real_escape_string($docinput['KategorieName']) .
			"' WHERE Kategorie_id=" . $docinput['Kategorie']))
			{
				echo '<div class="Erledigt">&gt;&gt;&gt; Kategorie umbenannt.</div>';
			} else
			{
				echo '<div class="SchwererFehler">Fehler beim Umbenennen der Kategorie.' . sql_error() . '</div>';
			}
		}
		else
		{
			if (sql_query('INSERT INTO ' . TABLE_KATEGORIEN . " (Kategorie) VALUES ('" .
			sql_real_escape_string($docinput['KategorieName']) . "')"))
			{
				echo '<div class="Erledigt">&gt;&gt;&gt; Kategorie hinzugefügt.</div>';
				$docinput['Kategorie'] = sql_insert_id();
			} else
			{
				echo '<div class="SchwererFehler">Fehler beim Hinzufügen der Kategorie.' . sql_error() . "</div>";
			}
		}
	}
	if (isset ($docinput['Edit']) && $docinput['Edit'] >= 5 && $docinput['Edit'] <= 7)
	{
		// Alle Versandmarker löschen
		if (!isset ($docinput['Kategorie']))
		{
			switch ($docinput['Edit'])
			{
				case 5 : // alle Löschen
					Adresse::removeAllVersandmarker();
					break;
				case 6 : // Institutionen Löschen
					Adresse::removeAllVersandmarkerInstitutionen();
					break;
				case 7 : // Ansprechpartner löschen
					Adresse::removeAllVersandmarkerAnsprechpartner();
					break;
			} // Switch
		}
		else
		{
			Adresse::setVersandmarkerKategorie($docinput['Kategorie']);
		}
		$docinput['Edit'] = 4;
		unset ($docinput['Kategorie']); // Löschen damit nicht nur die Kategorie angezeigt wird
	}

	// Neuen Datensatz als Kopie erstellen. Die Kopie wird direkt bearbeitet
	if (isset ($_REQUEST['Kopie']) && is_numeric($_REQUEST['Kopie']))
	{
		$Adresse = new Adresse($_REQUEST['Kopie']);
		if ( ! isset($_REQUEST['Kat'])) { $_REQUEST['Kat'] == '';}
		$Adresse->erstelleKopie($_REQUEST['Kat']);
	}
	elseif (isset ($_REQUEST['Bearbeiten']))
	{
		$Adresse = new Adresse($_REQUEST['Bearbeiten']);
	}
	if ( isset($Adresse))
	{
		// Stammdaten speichern oder hinzufügen		
		if ( isset($_REQUEST['KuNr']) )
		{
			$Adresse->getKundenNummer();					
		}
		if (isset ($_REQUEST['Save']))
		{
			if ($docinput['Geburtsdatum'] != '')
			{
				$docinput['Geburtsdatum'] = convertDatumToTimestamp($docinput['Geburtsdatum']);
			}
			if (isset ($docinput['ImmerPerEMail']) && trim($docinput['Email']) != '')
			{
				$docinput['ImmerPerEMail'] = 1;
			}
			else
			{
				$docinput['ImmerPerEMail'] = 0;
			}
			$Adresse->uebertrageFelder($docinput);
			$Adresse->save();
		}
		// Bemerkungs-Anhang löschen
		if (isset ($_REQUEST['DelAnhang']) && isset ($_REQUEST['AddBem']))
		{
			$Adresse->entferneAnhang($_REQUEST['AddBem']);
		}
		// Bemerkungen speichern oder hinzufügen
		if (isset ($_REQUEST["BemSave"]))
		{
			$Datum = convertDatumToTimestamp($docinput["Datum"]);
			if (strpos($docinput["Wiedervorlage"], ".") === false)
			{
				if (is_numeric($docinput["Wiedervorlage"]))
				{
					$Wiedervorlage = strtotime("+" . $docinput["Wiedervorlage"] . " day");
				}
				else
				{
					$Wiedervorlage = 0;
				}
			}
			else
			{
				$Wiedervorlage = convertDatumToTimestamp($docinput["Wiedervorlage"]);
			}
			if (isset ($docinput["Bemerkung_id"]) && is_numeric($docinput["Bemerkung_id"]))
			{
				// Korrektur
				$sql = "UPDATE " . TABLE_ADRESSENBEMERKUNGEN . " SET Bemerkung='" .
				sql_real_escape_string($docinput["Bemerkung"]) . "\ngeändert " . date("d.m.Y H:i") . " " .
				get_user_nickname() . "',Datum=$Datum,Wiedervorlage=$Wiedervorlage," .
				"Anhang='" . $_FILES["Anhang"]["name"] . "' WHERE Bemerkung_id=" . $docinput["Bemerkung_id"];
			} else
			{
				// Neue Bemerkung
				$sql = "INSERT INTO " . TABLE_ADRESSENBEMERKUNGEN .
				" (F_Adressen_id,Bemerkung,Datum,Wiedervorlage,Anhang) VALUES (" .
				$_REQUEST["Bearbeiten"] . ",'" .
				sql_real_escape_string($docinput["Bemerkung"]) . "\nerzeugt " . date("d.m.Y H:i") . " " .
				get_user_nickname() . "'," . $Datum . "," .
				$Wiedervorlage . ",'" . $_FILES["Anhang"]["name"] . "')";
			}
			if (!sql_query($sql))
			{
				echo sql_error();
			}
			if (!is_numeric($docinput["Bemerkung_id"]))
			{
				$docinput["Bemerkung_id"] = sql_insert_id();
			}
			if ($_FILES["Anhang"]["name"] != "")
			{
				// Upload eines Anhangs
				if (!is_dir(CVJM_ADRESSEN_DIR . "/" . $_REQUEST["Bearbeiten"]))
				mkdirs(CVJM_ADRESSEN_DIR .
				"/" . $_REQUEST["Bearbeiten"], CONFIG_CREATE_MASK);
				$ext = pathinfo($_FILES["Anhang"]["name"]);
				move_uploaded_file($_FILES["Anhang"]["tmp_name"], CVJM_ADRESSEN_DIR . "/" .
				$_REQUEST["Bearbeiten"] . "/Bemerkung" . $docinput["Bemerkung_id"] . "." . $ext["extension"]);
			}

		}
		// Adressbearbeitungsmodus
		if (isset ($_REQUEST["Plain"]))
		{
			unset ($profile[$document["SpeicherRecht"]]);
		}
		/* Beginn Template */
		$Smarty->assign('Speichern', $profile[$document['SpeicherRecht']] == '1');
		$Smarty->assign('Buchungurl', get_url(get_cmodule_id("cvjmbuchung")));
		$Smarty->assign_by_ref('Adresse', $Adresse);
		if (!$Adresse->isNeu())
		{
			include (INC_PATH . "misc/cvjmKorrespondenz.inc");
			ob_start();
			$Vorlagen = ZeigeVorlagen($Adresse->Adressen_id, 'Adressen');
			$Smarty->assign_by_ref('Vorlagenliste',holeVorlagen('Adressen'));
			$Smarty->assign('Vorlagen', ob_get_contents());
			ob_end_clean();

			$Smarty->assign('Kategorienurl', get_url(get_cmodule_id("cvjmadressenkategorien")));

			if (isset ($_REQUEST['AddBem']))
			{
				if ($_REQUEST["AddBem"] >= 0)
				{
					$query = sql_query("SELECT * FROM " . TABLE_ADRESSENBEMERKUNGEN . " WHERE Bemerkung_id = " . $_REQUEST["AddBem"]);
					if (!$Bemerkung = sql_fetch_array($query))
					unset ($Bemerkung);
					sql_free_result($query);
				}
				if (!isset ($Bemerkung))
				{
					$Bemerkung = array ();
					$Bemerkung['Datum'] = time();
					$Bemerkung['Wiedervorlage'] = 0;
					$Bemerkung['Anhang'] = '';
				}
				elseif ($Bemerkung['Anhang'] != '')
				{
					$ext = pathinfo($Bemerkung['Anhang']);
					$Bemerkung['Anhanglink'] = $Adresse->getAdressenAnhangLink($Bemerkung['Bemerkung_id'], $ext['extension'], 'Bemerkung');
				}
				$Smarty->assign('Bemerkung', $Bemerkung);
			}

			$Smarty->assign('Buchungsuebersichturl', get_url(get_cmodule_id('cvjmbuchungsuebersicht')));
			$Smarty->assign('Buchungsbemerkungenurl', get_url(get_cmodule_id('cvjmbuchungsbemerkungen')));

			//$Dateiname = $Adresse->getAdressenAnhangLink(-1, '*', '*');
			$Vorlagen = array('*'); //glob($Dateiname);
			ob_start();
			ZeigeKorrespondenz(array('Adressen_id'=>$Adresse->Adressen_id), $Vorlagen, array (
			"Bearbeiten" => $Adresse->Adressen_id
			));
			$Smarty->assign('Korrespondenz', ob_get_contents());
			ob_end_clean();
			$Smarty->assign('Artikelurl', get_url(get_cmodule_id('cvjmartikel')));
		} // Adresse_id > 0
		setlocale(LC_ALL, "de_DE@euro", "de_DE", "de", "ge");
		$Smarty->display('Adressen_Daten.tpl');
	} // Adressendatensatz anzeigen
	elseif (isset ($_REQUEST["Delete"]) && is_numeric($_REQUEST["Delete"]))
	{
		$Adresse = new Adresse($_REQUEST['Delete']);
		echo '<h2>Kontakt</h2>';
		echo '<table>';
		echo "<tr><td>Name</td><td><strong>" . $Adresse->Name . "</strong></td></tr>";
		echo "<tr><td>Zusatz</td><td><strong>" . $Adresse->Zusatz . '</strong></td></tr>';
		echo "<tr><td>Vorname</td><td><strong>" . $Adresse->Vorname . '</strong></td></tr>';
		echo '<tr><td colspan="2"><font size="+1">Wollen Sie den Kontakt wirklich inklusive aller Informationen l&ouml;schen?</font></td></tr>';
		echo '<tr><td>[ <a href="' . get_url($id, "Bearbeiten=" . $_REQUEST["Delete"]) .
		'">Nein</a> ]</td><td>[ <a href="' . get_url($id, "DelOk=" . $_REQUEST["Delete"] . "&DelMOk=-" .
		$_REQUEST["Delete"]) . '">Ja</a> ]</td></tr>';
		echo '</table>';
	}
	elseif (isset ($_REQUEST["KategorieNeu"]))
	{
		// Neue Kategorie hinzufügen / bearbeiten
		echo '<form action="' . get_url($id) . '" method="post" >';
		echo '<label for="Kategorie">Kategorie</label> <input id="Kategorie" ';
		if ($_REQUEST["KategorieNeu"] >= 0)
		{
			echo 'value ="';
			$query = sql_query("SELECT Kategorie FROM " . TABLE_KATEGORIEN . " WHERE Kategorie_id = " .
			$_REQUEST["KategorieNeu"]);
			$kategorie = sql_fetch_row($query);
			sql_free_result($query);
			echo stripslashes($kategorie[0]) . '"';
		}
		echo 'type="text" name="docinput[KategorieName]" size="25" maxlength="25">';
		if ($_REQUEST["KategorieNeu"] < 0)
		echo '<input type="Submit" value="hinzuf&uuml;gen" />';
		else
		{
			echo '<input type="hidden" name="docinput[Kategorie]" value="' . $_REQUEST["KategorieNeu"] . '" />';
			echo '<input type="Submit" value="umbenennen" />';
		}
		echo '</form>';
		echo '[ <a href="' . get_url($id) . '">Abbrechen</a> ]';
	} else
	{
		// Liste der Adressen aufbauen
		$Smarty->assign('title', transform($document['title'], $document['datatype']['title']));
		$Smarty->assign_by_ref('document', $document);
		$Smarty->assign('Speichern', $profile[$document['SpeicherRecht']] == '1');

		if ($document['Personal'] == 1 && !isset ($_REQUEST['Bearbeiten']))
		{
			$docinput['KategorieName'] = get_user_nickname();
			$query = sql_query('SELECT Kategorie_id FROM ' . TABLE_KATEGORIEN . ' WHERE Kategorie ="' .
			get_user_nickname() . '"');
			if (!$kategorie = sql_fetch_row($query))
			{
				$docinput['Kategorie'] = -1;
				$docinput['KategorieName'] = 'Alle Adressen';
			} else
			$docinput['Kategorie'] = $kategorie[0];
			sql_free_result($query);
		} else
		{
			if (!isset ($docinput['Edit']) || $docinput['Edit'] != 4)
			{
				if (!isset ($docinput['Kategorie']))
				$docinput['Kategorie'] = $profile['AdressauswahlKategorie'];
				else
				set_profile('AdressauswahlKategorie', $docinput['Kategorie']);
			}
			$query = sql_query('SELECT * FROM ' . TABLE_KATEGORIEN . ' ORDER BY Kategorie');
			$Kategorien = array();
			$Kategorien[-1] ='-Alle-';
			while ($kategorie = sql_fetch_array($query))
			{
				$Kategorien[$kategorie['Kategorie_id']] = $kategorie['Kategorie'];
				if (isset ($docinput['Kategorie']) && $docinput['Kategorie'] == $kategorie['Kategorie_id'])
				{
					$docinput['KategorieName'] = $kategorie['Kategorie'];
				}
			}
			sql_free_result($query);
			$Smarty->assign_by_ref('Kategorien', $Kategorien);
		}
		$params = array ();
		if (isset ($docinput['Zugehoerig']) && is_numeric($docinput['Zugehoerig']))
		{
			$params['docinput[Zugehoerig]'] = $docinput['Zugehoerig'];
		}
		if (isset ($docinput['Kategorie']) && is_numeric($docinput['Kategorie']))
		{
			$params['docinput[Kategorie]'] = $docinput['Kategorie'];
		}
		if (isset ($docinput['Institution']) && is_numeric($docinput['Institution']))
		{
			$params['docinput[Institution]'] = $docinput['Institution'];
		}
		if (isset ($docinput['Edit']) && is_numeric($docinput['Edit']))
		{
			$params['docinput[Edit]'] = $docinput['Edit'];
		}
		if (isset ($_REQUEST['Sort']) && $_REQUEST['Sort'] != '')
		{
			$params['Sort'] = $_REQUEST['Sort'];
		}
		if (isset ($_REQUEST['Search']) && $_REQUEST['Search'] != '')
		{
			$params['Search'] = $_REQUEST['Search'];
		}
		if (is_numeric($docinput["Institution"]) || is_numeric($docinput["Zugehoerig"]))
		{
			if (is_numeric($docinput["Institution"]))
			{
				$adresse = new Adresse($docinput["Institution"]);
				$docinput["KategorieName"] = "Ansprechpartner von ";
			} else
			{
				$adresse = new Adresse($docinput["Zugehoerig"]);
				$docinput["KategorieName"] = "Institutionen von ";
			}
			$docinput["KategorieName"] .= $adresse->Vorname . " " . $adresse->Name;
			$stammdatenlink = '';
			if ($adresse->Kunden_Nr != NULL && $adresse->Kunden_Nr > 0)
			{
				$docinput["KategorieName"] .= " (" . $adresse->Kunden_Nr . ")";
				$stammdatenlink = " (" . $adresse->Kunden_Nr . ")";
			}
			$stammdatenlink = $adresse->Vorname . " " . $adresse->Name . $stammdatenlink;
			$stammdatenlink = '<a href="' . get_url($id, 'Bearbeiten=' . $adresse->Adressen_id) .
			'">Zu den Adressstammdaten von ' . $stammdatenlink . '</a><br />';

			$Smarty->assign('stammdatenlink', $stammdatenlink);
		}
		if (isset ($_REQUEST['Search']))
		{
			$Search = trim(sql_real_escape_string($_REQUEST['Search']));
			if ($Search == '')
			{
				$Search = 'NOT Geloescht';
			}
			else
			{ 
				// Suchstring aufbereiten
				if (strpos($Search, ':') !== false)
				{
					// Filtern, damit nach einzelnen Feldern gesucht werden kann.
					$suchen = explode(' ', $Search);
					$normalsuche = array ();
					$spezialsuche = array ();
					foreach ($suchen as $suche)
					{
						if (strpos($suche, ':') !== false)
						{
							$Felder = array (
							'Name',
							'PLZ',
							'Ort',
							'Strasse',
							'Vorname'
							);
							list ($feld, $wert) = explode(':', $suche);
							if (in_array($feld, $Felder))
							$spezialsuche[] = $feld . " LIKE '%" . $wert . "%'";
							else
							$normalsuche[] = $suche;
						} else
						$normalsuche[] = $suche;
					}
					$suchstring = 'NOT Geloescht ';
					if (Count($spezialsuche) > 0)
					$suchstring .= 'AND (' . implode(' AND ', $spezialsuche) . ') ';
					if (Count($normalsuche) > 0)
					{
						$Search = implode(' ', $normalsuche);
						$Search = "AND (Name REGEXP '$Search' OR Vorname REGEXP '$Search' OR Strasse " .
						"REGEXP '$Search' OR Ort REGEXP '$Search' OR Bemerkungen REGEXP '$Search'" .
						"OR Telefon1 REGEXP '$Search' OR Telefon2 REGEXP '$Search' OR " .
						"Kunden_Nr REGEXP '$Search' OR " .
						"Email REGEXP '$Search' OR PLZ REGEXP '$Search' OR Zusatz REGEXP '$Search')";
						$suchstring .= $Search;
					}
					$Search = $suchstring;
				} else
				$Search = "NOT Geloescht AND (Name REGEXP '$Search' OR Vorname REGEXP '$Search' OR Strasse " .
				"REGEXP '$Search' OR Ort REGEXP '$Search' OR Bemerkungen REGEXP '$Search'" .
				"OR Telefon1 REGEXP '$Search' OR Telefon2 REGEXP '$Search' OR " .
				"Kunden_Nr REGEXP '$Search' OR " .
				"Email REGEXP '$Search' OR PLZ REGEXP '$Search' OR Zusatz REGEXP '$Search')";
			}
		} else
		{
			$Search = 'NOT Geloescht';
		}
		if ($docinput['Edit'] == 4 && $Versandmarker)
		{
			$Search .= ' AND Versandmarker';
		}
		if ( $docinput['proJahr'] == 'v')
			{				
				$Search .= ' AND Stand >= DATE_SUB(NOW(), INTERVAL 3 YEAR)';				
			}
		if ( is_numeric($docinput['AnzBuchungen']) )
		{
				// mindestens x Buchungen 
				$Search .= ' AND Adressen_id IN (SELECT F_Adressen_id FROM cvjm_Buchungen GROUP BY F_Adressen_id HAVING Count(F_Adressen_id) >= ';
				$Search .= $docinput['AnzBuchungen'].')';				
		}		
		if (isset ($_REQUEST['Seite']) && is_numeric($_REQUEST['Seite']))
		{
			$Seite = $_REQUEST['Seite'];
		}
		else
		{
			$Seite = $profile["Adressenseite$id"];
		}
		set_profile("Adressenseite$id", $Seite);

		$Listen = array (
		'Allgemein:Name|Zusatz|Vorname|Strasse|PLZ|Ort'
		);
		if (trim($document['Listen']) != '')
		$Listen = array_merge($Listen, explode("\n", $document['Listen']));
		if (!isset ($docinput['Page']))
		{
			$docinput['Page'] = 0;
		}
		// Reiter aufbauen
		$Anzeigeliste = array();
		for ($i = 0; $i < Count($Listen); $i++)
		{
			list ($Listenname, $Felder) = explode(':', $Listen[$i]);
			if ($docinput['Page'] == $i)
			{
				$Color = $design['table_row_bright'];
				$Class = 'selectedNav';
				$DBFelder = explode('|', $Felder);
			} else
			{
				$Color = $design['table_row_dark'];
				$Class = 'normalNav';
			}
			$Liste['Listenname'] = $Listenname;
			$Liste['Class'] = $Class;
			$Liste['Color'] = $Color;
			$Anzeigeliste[] = $Liste;
		}
		$Smarty->assign_by_ref('Listen', $Anzeigeliste);
		if (isset ($_REQUEST['Sort']) && trim($_REQUEST['Sort']) != '')
		{
			$Sort = sql_real_escape_string($_REQUEST['Sort']);
		}
		else
		{
			$Sort = 'Name,Vorname';
		}
		if (is_numeric($docinput['Institution']))
		{
			if (isset ($docinput['Edit']))
			{
			$sql = 'SELECT Adressen_id FROM ' . TABLE_ADRESSEN . " WHERE ($Search) AND Adressen_id <> " .
			$docinput['Institution'] . ' ORDER BY '.$Sort;
			}
			else
			{
			$sql = "SELECT DISTINCT Adressen_id FROM " . TABLE_ADRESSEN . " LEFT JOIN " . TABLE_ADRESSEN_INSTITUTIONEN .
			" ON Adressen_id = F_UAdressen_id WHERE F_Adressen_id = " . $docinput["Institution"] .
			" AND ($Search) ORDER BY $Sort";
			}
		}
		elseif (is_numeric($docinput["Zugehoerig"]))
		{
			if (isset ($docinput["Edit"]))
			$sql = "SELECT Adressen_id FROM " . TABLE_ADRESSEN . " WHERE ($Search) ORDER BY $Sort";
			else
			$sql = "SELECT DISTINCT Adressen_id FROM " . TABLE_ADRESSEN . " LEFT JOIN " . TABLE_ADRESSEN_INSTITUTIONEN .
			" ON Adressen_id = F_Adressen_id WHERE F_UAdressen_id = " . $docinput["Zugehoerig"] .
			" AND ($Search) ORDER BY $Sort";
		}
		elseif ($docinput["Kategorie"] >= 0 && is_numeric($docinput["Kategorie"]))
		{
			if (isset ($docinput["Edit"]))
			$sql = "SELECT DISTINCT Adressen_id FROM " . TABLE_ADRESSEN . " LEFT JOIN " . TABLE_ADRESSEN_KATEGORIE .
			" ON Adressen_id = F_Adressen_id WHERE $Search ORDER BY $Sort";
			else
			$sql = "SELECT DISTINCT Adressen_id FROM " . TABLE_ADRESSEN . " LEFT JOIN " . TABLE_ADRESSEN_KATEGORIE .
			" ON Adressen_id = F_Adressen_id WHERE F_Kategorie_id = " . $docinput["Kategorie"] .
			" AND $Search ORDER BY $Sort";
		} else
		{
			$sql = 'SELECT Adressen_id FROM ' . TABLE_ADRESSEN . ' WHERE '.$Search.' ORDER BY '.$Sort;
		}
		if (!$query = sql_query($sql))
		{
			throw new Exception('Fehler beim Adressenanzeigen: '.sql_error());
		}
		$p = $params;
		unset ($p['Search']);
		$Smarty->assign('paramsOhneSearch', feldToParams($p));
		$p = $params;
		unset ($p['docinput[Zugehoerig]']);
		unset ($p['docinput[Institution]']);
		unset ($p['docinput[Edit]']);
		$Smarty->assign('paramsOhneAlles', implode('&', $p));
		$Smarty->assign('Versandmarker', $Versandmarker);
		// Seiten-Unterteilung festlegen
		$AnzahlProSeite = 15;
		if (is_numeric($profile['AdressenAnzahlProSeite']))
		{
			$AnzahlProSeite = $profile['AdressenAnzahlProSeite'];
		}
		if (isset ($docinput['AnzahlProSeite']) && is_numeric($docinput['AnzahlProSeite']))
		{
			$AnzahlProSeite = $docinput['AnzahlProSeite'];
			if ($AnzahlProSeite < 5)
			{
				$AnzahlProSeite = 5;
			}
			set_profile('AdressenAnzahlProSeite', $AnzahlProSeite);
		}
		$Gesamtanzahl = sql_num_rows($query);
		if (($Gesamtanzahl / $AnzahlProSeite) + 1 < $Seite || !is_numeric($Seite))
		{
			$Seite = 1;
		}
		$Smarty->assign('Gesamtanzahl', $Gesamtanzahl);
		if ($Gesamtanzahl > $AnzahlProSeite || $Seite > 1)
		{
			sql_free_result($query);
			// Adressen aus der Datenbank holen
			if (!$query = sql_query($sql . ' ' . sql_limit($AnzahlProSeite, ($Seite -1) * $AnzahlProSeite)))
			{
				echo sql_error();
			}
		}
		$Smarty->assign_by_ref('Felder', $DBFelder);
		while ($adresse = sql_fetch_array($query))
		{
			$Adressen[] = new Adresse($adresse['Adressen_id']);
		} // while
		sql_free_result($query);
		$Smarty->assign_by_ref('Adressen', $Adressen);
		$Smarty->assign('Seite', $Seite);
		$Smarty->assign('AnzahlProSeite', $AnzahlProSeite);
		$Smarty->assign('params', feldToParams($params));
		$Smarty->assign('Sort', $Sort);
		$Smarty->assign_by_ref('docinput', $docinput);
		$p = $params;
		unset ($p['docinput[Edit]']);
		$Smarty->assign('paramsOhneEdit', feldToParams($p));
		$Smarty->display('Adressen_Liste.tpl');
	} // Normaler Listenanzeigemodus
} // if not Editor
else
{
	echo '<br /><br />';
	create_editor_input('Recht für Veränderungen', 'SpeicherRecht', 'text_input');
	echo '<br /><br />';
	create_editor_input('Persönlich', 'Personal', 'onoff_input');
	echo '<br /><br />';
	create_editor_input('Listen (Format ist Listenname:Feld|Feld|Feld)', 'Listen', 'area_input');
	echo '<br /><br />';
}
?>
