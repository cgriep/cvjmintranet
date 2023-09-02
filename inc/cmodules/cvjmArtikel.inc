<?php

/**
 *
 * cvjmArtikel
 *
 * AWF-Name: cvjmartikel
 *
 */
function zeigeArtikelTabelle($tabelle, $ueberschrift)
{
	global $docinput;
	global $design;
	global $id;
	if ( $tabelle == TABLE_ARTIKELBEZIEHUNGEN )
	{
		$tabellennummer = 1;
	}
	if ( $tabelle == TABLE_ARTIKELGRUPPEN )
	{
		$tabellennummer = 2;
	}

	if ( ! isset($tabellennummer ) ) die('Keine Tabellennummer!');

	if ( isset($docinput['UTable']) && $docinput['UTable'] == $tabellennummer )
	{
		if ( isset($docinput['DArtikel_Nr']) && is_numeric($docinput['DArtikel_Nr']) )
		{
			// Unterartikel löschen
			sql_query('DELETE FROM '.$tabelle.' WHERE F_Artikel_id = '.
			$docinput['Artikel_Nr'].' AND F_Unterartikel_id = '.$docinput['DArtikel_Nr']);
		}
		if ( isset($docinput['Menge']) && is_array($docinput['Menge']) )
		{
			foreach ( $docinput['Menge'] as $key => $menge )
			{
				if ( is_numeric($key) )
				{
					$menge = str_replace(',','.',$menge);
					$dauer = $docinput['Dauer'][$key];
					$dauer = str_replace(',','.',$dauer);
					if ( ! is_numeric($dauer) ) $dauer = 0;
					$beginn = $docinput['BeginnTag'][$key];
					if ( ! is_numeric($beginn) ) $beginn = 0;
					if ( $beginn > 0 ) $beginn -= 1;
					$beginnT = $beginn*86400;
					if ( strpos($docinput['BeginnStunde'][$key],':') === false)
					$docinput['BeginnStunde'][$key] = substr($docinput['BeginnStunde'][$key],0,2).':'.
					substr($docinput['BeginnStunde'][$key],2);
					$zeit = explode(':',$docinput['BeginnStunde'][$key]);
					if ( ! is_numeric($zeit[0]) ) $zeit[0] = 0;
					if ( ! is_numeric($zeit[1]) ) $zeit[1] = 0;
					$beginn = $beginnT+$zeit[0]*3600+$zeit[1]*60;
					if ( $beginn < 0 ) $beginn = 0;
					if ( is_numeric($menge) && is_numeric($dauer) && is_numeric($beginn) )
					{
						if ( !isset($docinput["Edit$tabelle"]) )
						{
							// Menge speichern
							if (! sql_query("UPDATE $tabelle SET Menge=$menge, Beginn=$beginn, Dauer=$dauer ".
							'WHERE F_Artikel_id='.
							$docinput['Artikel_Nr']." AND F_Unterartikel_id=$key"))
							echo "&gt;&gt;&gt;Fehler beim Update: ".sql_error().'<br />';
						}
						else
						{
							// Unterartikel hinzufügen
							if (! sql_query('INSERT INTO '.$tabelle.
							' (F_Artikel_id, F_Unterartikel_id,Menge,Dauer,Beginn) '.
							'VALUES ('.$docinput['Artikel_Nr'].",$key,$menge,$dauer,$beginn)"))
							echo 'Fehler beim Einfügen: '.sql_error();
						}
					}
				}
			}
		}
	}
	echo '<a id="'.$tabelle.'" name="'.$tabelle.'"></a><table class="volleTabelle" cellspacing="0">';
	echo '<tr class="ueberschrift"><th colspan="5">';
	echo $ueberschrift;
	echo '</th></tr>';
	$query = sql_query("SELECT * FROM $tabelle INNER JOIN ".TABLE_ARTIKEL.
	' ON F_Unterartikel_id = id WHERE F_Artikel_id = '.$docinput['Artikel_Nr'].
	' ORDER BY Bezeichnung');
	if ( sql_num_rows($query) > 0 || $docinput["Edit$tabelle"] == 2)
	{
		if ( isset($docinput["Edit$tabelle"]) )
		{
			echo '<form action="'.get_url($id).'#'.$tabelle.'" method="post">';
			echo '<input type="hidden" name="docinput[UTable]" value="'.$tabellennummer.'" />';
			echo '<input type="hidden" name="docinput[Artikel_Nr]" value="'.$docinput["Artikel_Nr"].'" />';
		}
		echo '<tr><td align="center"><strong>Menge</strong></td><td width="70%"><strong>Bezeichnung</strong>';
		echo '</td><td align="center"><strong>Dauer</strong></td><td><strong>Beginn</strong></td><td></td></tr>';
		$Color = "helleZeile";
		$Artikelid = array();
		while ( $unterartikel = sql_fetch_array($query) )
		{
			echo '<tr class="'.$Color.'"><td align="center">';
			if ( $docinput["Edit$tabelle"] == 1 )
			echo '<input type="Text" name="docinput[Menge]['.$unterartikel["id"].
			']" value="';
			echo number_format($unterartikel["Menge"],2);
			if ( $docinput["Edit$tabelle"] == 1 )
			echo '" size="3" maxlength="5" />';
			echo '</td>';
			echo '<td>';
			if ( $unterartikel['Beschreibung'] != '' )
			{
				echo '<span onMouseOver="return overlib(\''.
				htmlentities($unterartikel['Beschreibung'])."',CAPTION," .
				"'Artikel ".$unterartikel['Bezeichnung']."');";
				echo '" onMouseOut="return nd();" >';
			}
			echo '<a href="'.get_url($id,"docinput[Artikel_Nr]=".
			$unterartikel['id']).'">'.stripslashes($unterartikel['Bezeichnung']);
			echo '</a>';
			if ( $unterartikel['Beschreibung'] != '' ) echo '</span>';
			echo "</td>\n";
			echo '<td align="center">';
			if ( $docinput["Edit$tabelle"] == 1 )
			echo '<input type="Text" name="docinput[Dauer]['.$unterartikel["id"].
			']" value="';
			if ( $unterartikel["Dauer"] != 0 || $docinput["Edit$tabelle"] == 1)
			echo $unterartikel["Dauer"];
			if ( $docinput["Edit$tabelle"] == 1 )
			echo '" size="3" maxlength="3" />';
			echo '</td><td>';
			if ( $unterartikel["Beginn"] != 0 || $docinput["Edit$tabelle"] == 1 )
			{
				$tage = floor($unterartikel["Beginn"] / 86400);
				$stunden = $unterartikel["Beginn"]-$tage*86400-3600;
				if ( $unterartikel["Beginn"] == 0 )
				{
					$stunden = 0;
				}
				if ( $docinput["Edit$tabelle"] == 1 )
				{
					echo '<input type="Text" name="docinput[BeginnTag]['.$unterartikel["id"].
					']" value="'.($tage+1);
					echo '" size="1" maxlength="1" />. Tag ';
					echo '<input type="Text" name="docinput[BeginnStunde]['.$unterartikel["id"].
					']" value="'.date("H:i",$stunden);
					echo '" size="5" maxlength="5" />';
				}
				else
				echo ($tage+1).". Tag ".date("H:i",$stunden);
			}
			echo '</td>';
			echo '<td>';
			$Artikelid[] = $unterartikel["id"];
			echo '<a href="'.get_url($id,array("docinput[Artikel_Nr]"=>$docinput["Artikel_Nr"],
			"docinput[DArtikel_Nr]"=>$unterartikel["id"],"docinput[UTable]"=>$tabellennummer));
			echo '#'.$tabelle.'" onClick="javascript:return window.confirm(\'Unterartikel '.
			stripslashes($unterartikel["Bezeichnung"]);
			echo ' wirklich löschen?\');"><img src="/img/small_edit/delete.gif" border="0"/>';
			echo '</td></tr>';
			if ( $Color == "helleZeile")
			$Color = "dunkleZeile";
			else
			$Color = "helleZeile";
		}
		if ( $docinput["Edit$tabelle"] == 2 )
		{
			if ( isset($docinput["Search".$tabelle]) &&
			trim($docinput["Search".$tabelle]) != "" )
			{
				// TODO: Subselect: Die vorhandenen Unterartikel ausschließen
				$uquery = sql_query("SELECT Bezeichnung, id FROM ".TABLE_ARTIKEL.
				" WHERE id <> ".$docinput["Artikel_Nr"]." AND Bezeichnung LIKE '%".
				addslashes($docinput["Search$tabelle"])."%'".
				"ORDER BY Bezeichnung");
				while ( $unterartikel = sql_fetch_array($uquery) )
				{
					// schon vorhandene ausnehmen
					if ( ! in_array($unterartikel["id"],$Artikelid) )
					{
						echo '<tr><td>';
						echo '<input type="Text" name="docinput[Menge]['.$unterartikel["id"].
						']" value="" size="3" maxlength="5" />';
						echo '</td><td>';
						echo stripslashes($unterartikel["Bezeichnung"]);
						echo '</td><td align="right">';
						echo '<input type="Text" name="docinput[Dauer]['.$unterartikel["id"].
						']" value="';
						echo '" size="3" maxlength="3" />';
						echo '</td><td>';
						echo '<input type="Text" name="docinput[BeginnTag]['.$unterartikel["id"].
						']" value="" size="1" maxlength="1" />. Tag ';
						echo '<input type="Text" name="docinput[BeginnStunde]['.$unterartikel["id"].
						']" value="" size="5" maxlength="5" />';
						echo "</td></tr>\n";
					}
				}
			}
			echo '<tr><td colspan="2">';
			echo 'Artikel suchen: <input type="Text" name="docinput[Search'.$tabelle.']" size="20" /> ';
			echo '</td></tr>';
			echo '<tr><td colspan="3" class="zentriert">';
			echo '<input type="hidden" name="docinput[Edit'.$tabelle.']" value="'.
			$docinput["Edit$tabelle"].'" />';
			echo '<input type="Submit" name="docinput[Speichern'.$tabelle.
			']" value="';
			if ( isset($docinput["Search$tabelle"]) )
			echo 'Hinzufügen/';
			echo 'Artikel suchen" /></td></tr>';
		}
		if ( $docinput["Edit$tabelle"] == 1 )
		{
			echo '<tr><td colspan="3" class="zentriert">';
			echo '<input type="Submit" name="docinput[Speichern'.$tabelle.
			']" value="Mengen speichern" /></td></tr>';
		}
		if ( isset($docinput["Edit$tabelle"]) )
		{
			echo '<tr><td colspan="3">Geben Sie als Menge "0" an, um ';
			if ( $tabelle == TABLE_ARTIKELGRUPPEN )
			echo 'eine einmalige Pauschale zu kennzeichnen.';
			elseif ( $tabelle == TABLE_ARTIKELBEZIEHUNGEN )
			echo 'eine beliebige Anzahl nicht berechnen zu lassen.';
			echo "</td></tr>\n";
			echo '</form>';
		}
		if ( sql_num_rows($query) > 0 )
		echo '<tr><td colspan="3" class="zentriert">[ <a href="'.get_url($id,
		array("docinput[Artikel_Nr]"=>$docinput["Artikel_Nr"],"docinput[Edit$tabelle]"=>1)).
		'#'.$tabelle.'">Bearbeiten</a> ]</td></tr>';
	}
	else
	{
		echo '<tr><td colspan="3" class="zentriert">Zu diesem Artikel geh�ren keine weiteren Artikel</td></tr>';
	}
	if ( $docinput["Edit$tabelle"] != 2 )
	echo '<tr><td colspan="3" class="zentriert">[ <a href="'.get_url($id,
	array("docinput[Artikel_Nr]"=>$docinput["Artikel_Nr"],"docinput[Edit$tabelle]"=>2)).
	'#'.$tabelle.'">Hinzufügen</a> ]</td></tr>';
	echo '</table>';
	sql_free_result($query);
}

if ( $profile['editor']!= 1 )
{
	include(INC_PATH.'misc/CVJM.inc');
	include(INC_PATH.'misc/cvjmArtikeltools.inc');
	include(INC_PATH.'cvjm/Artikel.class.php');
	include(INC_PATH.'cvjm/Adresse.class.php');
	include(INC_PATH.'cvjm_ajax.php');
	$xajax->printJavascript('./javascript');
	if ( isset($docinput['Kopie']) && is_numeric($docinput['Kopie']) )
	{
		$Artikel = new Artikel($docinput['Kopie']);
		$Artikel->erstelleKopie();
		$docinput['Artikel_Nr'] = $Artikel->Artikel_id;
	}
	if ( isset($docinput['F_Art_id']) && ! is_numeric($docinput['F_Art_id']) )
	{
		$docinput['F_Art_id'] = CVJMART_UNBEKANNT;
	}
	$Artikelarten = Artikel::getArtikelArten();
	if ( isset($_REQUEST['Baum']) && isset($docinput['F_Art_id']) )
	{
		// neu Nummerieren
		$query = sql_query('SELECT * FROM '.TABLE_ARTIKEL.' WHERE F_Art_id='.$docinput['F_Art_id'].' ORDER BY Position, Einruecken');
		$Position = 1;
		while ( $art = sql_fetch_array($query) )
		{
			sql_query('UPDATE '.TABLE_ARTIKEL." SET Position=$Position WHERE id=".$art['id']);
			$Position++;
		}
		sql_free_result($query);
		echo "&gt;&gt;&gt; $Position Positionen im Baum neu nummeriert.<br />";
	}
	if ( isset($docinput['Bezeichnung']) && isset($docinput['Einheit']) &&
	isset($docinput['Save']))
	{
		// prüfen, ob Artikel mit gleicher Bezeichnung schon vorhanden ist
		$query = sql_query('SELECT * FROM '.TABLE_ARTIKEL." WHERE Bezeichnung='".
		sql_real_escape_string($docinput['Bezeichnung'])."' AND id<>".$docinput['Artikel_Nr']);
		if ( sql_num_rows($query) > 0 )
		{
			echo '<span class="Fehler">Vorhandene Artikel mit gleicher Bezeichnung:</span><br />';

			while ( $artikel = sql_fetch_array($query) )
			{
				echo '<a href="'.get_url($id,'docinput[Artikel_Nr]='.$artikel['id']).'">'.
				stripslashes($artikel['Bezeichnung']).'</a><br />';
			}
			echo '<hr />';
		}
		sql_free_result($query);
		// Aktualisieren
		$Artikel = new Artikel($docinput['Artikel_Nr']);
		$docinput['Einkaufspreis'] = str_replace(',','.',$docinput['Einkaufspreis']);
		$docinput['LetztePruefung'] = convertDatumToTimestamp($docinput['LetztePruefung']);
		$Artikel->uebertrageFelder($docinput);
		$Artikel->save(isset($docinput['AstKopieren']));
		$docinput['Artikel_Nr'] = $Artikel->Artikel_id;
	}
	require_once(INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('title', transform($document['title'], $document['datatype']['title']));
	$Smarty->assign('id', $id);
	$Smarty->assignbyref('docinput', $docinput);
	if ( isset($docinput['Artikel_Nr']) && is_numeric($docinput['Artikel_Nr']))
	{
		$Artikel = new Artikel($docinput['Artikel_Nr']);
		if ( isset($docinput['F_Art_id']))
		{
			$Artikel->F_Art_id = $docinput['F_Art_id'];
		}
		if ( isset($docinput['Einruecken']))
		{
			$Artikel->Einruecken = $docinput['Einruecken'];
		}
		if ( isset($docinput['Position']))
		{
			$Artikel->Position = $docinput['Position'];
		}
		if ( isset($docinput['DelPreis']) && is_numeric($docinput['DelPreis']) )
		{
			// Preis von Preisliste entfernen
			$Artikel->entfernePreis($docinput['DelPreis']);
		}
		if ( isset($docinput['Richtung']) )
		{
			// Artikel im Baum bewegen
			$Artikel->bewegen($docinput['Richtung']);
		}
		$Smarty->assignbyref('Artikel', $Artikel);
		$Smarty->assign('Artikelarten_values', array_keys($Artikelarten));
		$Smarty->assign('Artikelarten_output', array_values($Artikelarten));

		$subject = 'Artikel/'.$Artikel->Artikel_id.'/';
		include(INC_PATH.'misc/cvjmDateien.inc');
		Dateiloeschen($subject, 'DelDatei');
		Dateihochladen($subject, 'Werbematerial');
		$handle=opendir($subject);
		while (false != ($file = readdir($handle))) {
			if(filetype($subject.$file) == 'file') {
				$datei['Groesse'] = floor(filesize($subject.$file) / 1024);
				$datei['URL'] = str_replace(' ', '%20',$subject.$file);
				$datei['Name'] = $file;
				$filelist[] = $datei;
			}
		}
		$Smarty->assign('Dateiliste', $filelist);
		$Lieferanten = array();
		$Lieferanten[-1] = '-- keiner angegeben --';
		$sql = 'SELECT Adressen_id, Name FROM (cvjm_Adressen INNER JOIN cvjm_Adressen_Kategorie ON '.
		'F_Adressen_id=Adressen_id) INNER JOIN `cvjm_Kategorien` ON '.
		"F_Kategorie_id=Kategorie_id WHERE Kategorie='Lieferant' ORDER BY Name";
		$query = sql_query($sql);
		while ( $lieferant = sql_fetch_array($query) )
		{
			$Lieferanten[$lieferant['Adressen_id']] = $lieferant['Name'];
			if ( $Artikel->F_Lieferant_id == $lieferant['Adressen_id'] )
			{
				$Smarty->assignbyref('Lieferant', new Adresse($lieferant['Adressen_id']));
				$Smarty->assign('Adressenurl', get_url(get_cmodule_id('cvjmadressen')));
			}
		}
		sql_free_result($query);
		$Smarty->assignbyref('Lieferanten_values', array_keys($Lieferanten));
		$Smarty->assignbyref('Lieferanten_output', array_values($Lieferanten));
		$artikel['Artikelart'] = $Artikelarten[$artikel['F_Art_id']];
		// Artikelgruppen verwalten
		if ( ! $Artikel->isNeu())
		{
			ob_start();
			zeigeArtikelTabelle(TABLE_ARTIKELGRUPPEN, 'Artikel, die mitgebucht und berechnet werden');

			// zugehörige Artikel
			zeigeArtikelTabelle(TABLE_ARTIKELBEZIEHUNGEN, 'Zugeh�rige Artikel (werden nicht berechnet)');

			$Smarty->assign('Tabellen', ob_get_contents());

			ob_end_clean();
		}
		// Preise anzeigen
		if ( isset($docinput['SavePreis']) )
		{
			// Preislistenpreis ändern
			foreach ( $docinput['Preis'] as $key => $value )
			{
				$value = str_replace(',','.',$value);
				$value2 = str_replace(',','.',$docinput['PreisStunde'][$key]);
				if (! is_numeric($value) ) $value = 0;
				if (! is_numeric($value2) ) $value2 = 0;
				if ( is_numeric($key) )
				{
					if ( $key < 0 && ($value != 0 || $value2 != 0) &&
					$docinput['Preisliste_id'] > 0 )
					{
						// Neuen Eintrag hinzufügen
						if ( ! sql_query('INSERT INTO '.TABLE_PREISE.
						' (Preis,Artikel_Nr,F_Preisliste_id,PreisStunde) VALUES ('.
						$value.','.$docinput['Artikel_Nr'].','.$docinput['Preisliste_id'].
						','.$value2.')')) echo 'EP:'.sql_error();
					}
					elseif ( ! sql_query('UPDATE '.TABLE_PREISE.
					" SET Preis=$value,PreisStunde=$value2 WHERE F_Preisliste_id=$key AND Artikel_Nr=".
					$docinput['Artikel_Nr']))
					echo 'Fehler bei Preisanpassung: '.sql_error();
				}
			}
		}
		$query = sql_query('SELECT * FROM '.TABLE_PREISLISTEN. ' WHERE NOT Inaktiv ORDER BY GueltigAb DESC, Bezeichnung');
		$Preisliste = -1;
		$Preislisten = array();
		while ( $preisliste = sql_fetch_array($query))
		{
			$Preislisten[$preisliste['Preisliste_id']] = $preisliste['Bezeichnung'];
			if ( $Preisliste < 0 && $preisliste['Standard'] )
			{
				$Preisliste = $preisliste['Preisliste_id'];
			}
		}
		sql_free_result($query);
		$Smarty->assign('Preisliste', $Preisliste);
		$Smarty->assign('Preislisten', $Preislisten);
		$Smarty->assign('Preisurl', get_url(get_cmodule_id('cvjmpreise')));

		// Artikelbaum Ausschnitt zeigen
		$params = array('docinput[Artikel_Nr]'=>$docinput['Artikel_Nr']);
		if ( isset($docinput['Anz']) && (is_numeric($docinput['Anz']) || $docinput['Anz'] == 'Alle') )
		{
			$Anz = $docinput['Anz'];
			set_profile('ArtikelAnzahlProSeite', $Anz);
		}
		else
		{
			$Anz = $profile['ArtikelAnzahlProSeite'];
		}
		if ( ! is_numeric($Anz) && $Anz != 'Alle' )
		{
			$Anz = 10;
		}
		$_SESSION['ArtikelAnzahl'] = $Anz;
		//$Smarty->assign('Anz', $Anz);
		$Smarty->assign('params', feldToParams($params));
		$Smarty->assignbyref('Artikelliste', $Artikel->baueArtikelBaumAuf($Anz));
		$Smarty->assignbyref('Pruefungsarten', $Artikel->Pruefungsarten());
		
		$Smarty->display('Artikel_Daten.tpl');
	}
	else
	{
		$params = array();
		// Tabelle mit vorhandenen Artikeln anzeigen
		if ( isset($_REQUEST['Sort']) && $_REQUEST['Sort'] != '' )
		{
			$_SESSION['Artikel_Sort'] = $_REQUEST['Sort'];
		}
		if ( isset($_REQUEST['Search']) && $_REQUEST['Search'] != '' )
		{
			$params['Search'] = $_REQUEST['Search'];
		}
		if ( isset($_REQUEST['Seite']) && is_numeric($_REQUEST['Seite']))
		{
			$Seite = $_REQUEST['Seite'];
		}
		elseif ( array_key_exists('Artikel_Seite', $_SESSION))
		{
			$Seite = $_SESSION['Artikel_Seite'];
		}
		else
		{
			$Seite = 1;
		}
		$_SESSION['Artikel_Seite'] = $Seite;
		//set_profile('Artikelseite'.$id, $Seite);
		$Smarty->assign('Seite', $Seite);
		$Smarty->assign('Artikelarten', $Artikelarten);
		$Smarty->assign('params', feldToParams($params));
		if ( isset($docinput['F_Art_id']) && is_numeric($docinput['F_Art_id']) )
		{
			$params['docinput[F_Art_id]'] = $docinput['F_Art_id'];
		}
		$Smarty->assign('params2', feldToParams($params));
		$Listen = array('Allgemein:Bezeichnung|Einheit|Art|MWST|Geringwertig|Anzeigen|Rabattfaehig');
		if ( trim($document['Listen']) != '' )
		{
			$Listen = array_merge($Listen, explode("\n",$document['Listen']));
		}
		if ( ! isset($docinput['Page']) )
		{
			$docinput['Page'] = 0;
		}
		$Smarty->assignbyref('docinput', $docinput);
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
		$Smarty->assignbyref('Listen', $Anzeigeliste);

		if ( isset($_REQUEST['Search']) )
		{
			$Search = trim(sql_real_escape_string($_REQUEST['Search']));
			if ( $Search == '' )
			{
				$Search = '1';
			}
			else
			{
				$Search = "Bezeichnung REGEXP '".$Search."' OR Barcode='".$Search."'";
			}
		}
		else
		{
			$Search = '1';
		}
		if ( array_key_exists('Artikel_Sort',$_SESSION) && trim($_SESSION['Artikel_Sort']) != '')
		{
			$Sort = sql_real_escape_string($_SESSION['Artikel_Sort']);
		}
		else
		{
			$Sort = 'Bezeichnung, F_Art_id';			
		}
		if ( isset($docinput['F_Art_id']) && is_numeric($docinput['F_Art_id']) )
		{
			$Search .= ' AND F_Art_id ='.$docinput['F_Art_id'];
			if ( ! array_key_exists('Artikel_Sort', $_SESSION) )
			{
				$Sort = 'Bezeichnung';
			}
		}
                $sql = 'SELECT id FROM '.TABLE_ARTIKEL.' WHERE '.$Search.' ORDER BY '.$Sort;
		if (! $query = sql_query($sql))
		{
			echo sql_error();
		}
		if ( $_REQUEST['Search'] != '' )
		{
			$p = $params;
			unset($p['Search']);
			$Smarty->assign('paramsOhneSearch', feldToParams($p));
		}
		// Seiten-Unterteilung festlegen
		$AnzahlProSeite = 15;
		if ( is_numeric($profile['ArtikelAnzahlProSeite']) )
		{
			$AnzahlProSeite = $profile['ArtikelAnzahlProSeite'];
		}
		if ( isset($docinput['AnzahlProSeite']) && is_numeric($docinput['AnzahlProSeite']))
		{
			$AnzahlProSeite = $docinput['AnzahlProSeite'];
			if ( $AnzahlProSeite < 5 )
			{
				$AnzahlProSeite = 5;
			}
			set_profile('ArtikelAnzahlProSeite', $AnzahlProSeite);
		}
		$Gesamtanzahl = sql_num_rows($query);
		if ( ($Gesamtanzahl/$AnzahlProSeite)+1 < $Seite || ! is_numeric($Seite))
		{
			$Seite = 1;
		}
		if ( $Gesamtanzahl > $AnzahlProSeite || $Seite > 1 )
		{
			sql_free_result($query);
			if ( ! $query = sql_query($sql.' '.sql_limit($AnzahlProSeite,($Seite-1)*$AnzahlProSeite)))
			{
				echo sql_error();
			}
		}
		$Smarty->assign('Seite', $Seite);
		$Smarty->assign('Gesamtanzahl', $Gesamtanzahl);
		$Smarty->assign('AnzahlProSeite', $AnzahlProSeite);
		$Smarty->assign('Felder', $DBFelder);
		while ( $artikel = sql_fetch_row($query) )
		{
			$Artikelliste[] = new Artikel($artikel[0]);
		} // while
		sql_free_result($query);
		$Smarty->assignbyref('Artikelliste',$Artikelliste);
		$Smarty->display('Artikel_Tabelle.tpl');
	}
} // wenn nicht Editor
else
{
	create_editor_input('Listen (Format ist Listenname:Feld|Feld|Feld)','Listen','area_input');
	echo '<br /><br />';
}
?>
