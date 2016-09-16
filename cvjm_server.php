<?php

/* Datenbank- und AWF-Funktionalität einbinden */
include ('inc/functions.inc');
include ('inc/licence.key');
include ('inc/sessions.inc');
include ('inc/caching.inc');
include ('inc/config.inc');

include_once('inc/misc/CVJM.inc');
include_once('inc/misc/cvjmBuchungstools.inc');
include_once('inc/misc/cvjmArtikeltools.inc');
include_once('inc/misc/cvjmKorrespondenz.inc');
require_once('inc/Smarty/Smarty.class.php');
require_once (INC_PATH.'cvjm/Adresse.class.php');
require_once (INC_PATH.'cvjm/Event.class.php');

/* prüfen ob eine Session für den Benutzer existiert */
if (!isset ($session_userid) || $session_userid == '')
{
	die('Unauthorisiert!');
}
// Benutzer-ID heraussuchen
define('SESSION_DBID', get_user_id($session_userid));
define('SESSION_STATUS','ok');
$profile = get_profile();

/**
 * Konvertiert einen JavaScript-Checked-Wert in einen PHP-Auswertbaren Boolean
 * @param String $value der JavaScript-Parameter
 * @return boolean true oder false
 */
function convertJavaScriptBool($value)
{
	if ( ! is_numeric($value))
	{
		if ( $value=='true')
		{
			$value = true;
		}
		else
		{
			$value = false;
		}
	}
	return $value;
}

/**
 *  Abrechnung: Erhöht oder senkt die Menge eines Artikels
 */
function erhoeheRechnungsMenge($rechnung_id, $artikel_nr, $Datum)
{
	return aendereRechnungsMenge($rechnung_id, $artikel_nr, $Datum, '+');
}
function senkeRechnungsMenge($rechnung_id, $artikel_nr, $Datum)
{
	return aendereRechnungsMenge($rechnung_id, $artikel_nr, $Datum, '-');
}

function aendereRechnungsMenge($rechnung_id, $artikel_nr, $Datum, $Richtung='+')
{
	$objResponse = new xajaxResponse();
	if (!is_numeric($rechnung_id) || !is_numeric($artikel_nr))
	return $objResponse;
	if ( $Richtung != '+' && $Richtung != '-')
	return $objResponse;
	// Eintragsmenge erhöhen
	$sql1 = 'UPDATE ' . TABLE_RECHNUNGSEINTRAEGE . ' SET Menge=Menge '.$Richtung.'1 WHERE ' .
	'F_Artikel_Nr = ' . $artikel_nr . ' AND F_Rechnung_id=' . $rechnung_id . ' AND Datum';
	if (strpos($Datum, '-') === false)
	$sql .= '='.$Datum;
	else
	{
		list ($anf, $end) = explode('-', $Datum);
		if ( is_numeric($anf) && is_numeric($end))
		$sql .= ' BETWEEN '.$anf.' AND '.$end;
	}
	if (!sql_query($sql1.$sql))
	$ergebnis = 'Fehler: ' . $sql1.$sql . ': ' . mysql_error();
	else
	{
		$query = sql_query('SELECT Menge FROM '.TABLE_RECHNUNGSEINTRAEGE.
		' WHERE F_Rechnung_id='.$rechnung_id.' AND F_Artikel_Nr='.$artikel_nr.
		' AND Datum '.$sql);
		$menge = sql_fetch_row($query);
		sql_free_result($query);
		$ergebnis = $menge[0];
	}
	$objResponse->addAssign('Menge'.$artikel_nr.'-'.$Datum, 'innerHTML', $ergebnis);
	return $objResponse;
}

/**
 * Buchungsübersicht:
 * Entfernt einen Artikel aus der Buchung und zeigt die entsprechende Zelle
 * ohne Häkchen an
 */
function entferneArtikel($buchung_nr, $artikel_id, $datum, $menge,$artikelnummern)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($buchung_nr) || ! is_numeric($artikel_id) || ! is_numeric($datum) || ! is_numeric($menge))
	{
		$objResponse->addAlert(utf8_encode('Fehler - ungültige Argumente.'));
	}
	else
	{
		$Buchung = new Buchung($buchung_nr);
		$Buchung->entbucheArtikel($artikel_id, $datum, $menge);
		$Smarty = new Smarty;
		$Smarty->template_dir = TEMPLATEPATH;
		$Artikel['id'] = $artikel_id;
		$buchung['Buchung_Nr'] = $buchung_nr;
		$Smarty->assign('buchung', $buchung);
		$Smarty->assign('artikel', $Artikel);
		$Smarty->assign('tagnr', $datum);
		$docinput['Artikelnummern'] = $artikelnummern;
		$Smarty->assign('docinput', $docinput);
		// Menge nach Art und Personenzahl feststellen
		$sql = 'SELECT Typ FROM ' . TABLE_ARTIKELARTEN . ' INNER JOIN '.TABLE_ARTIKEL.
		' ON Art_id=F_Art_id WHERE id=' . $artikel_id;
		$query = sql_query($sql);
		$Abrechnungstyp = sql_fetch_row($query);
		sql_free_result($query);
		$Abrechnungstyp = $Abrechnungstyp[0];
		if ($Abrechnungstyp != ABRECHNUNGSTYP_KOPF && $Abrechnungstyp != ABRECHNUNGSTYP_KOPFNACHT && $Abrechnungstyp != ABRECHNUNGSTYP_VERPFLEGUNG)
		{
			$Menge = 1;
		}
		else
		{
			$Menge= $Buchung->personenAnzahl();
		}
		$Smarty->assign('menge', $Menge);
		$content = utf8_encode($Smarty->fetch('Buchungsuebersicht_InaktiveZelle.tpl'));
		$objResponse->addAssign('A'.$artikel_id.'D'.$datum, 'innerHTML', $content);
		$objResponse->addAssign('Schlaf'.$datum, 'innerHTML', Max(0,$Buchung->BerechneAlleSchlafplaetze($datum,$artikelnummern)));
		$objResponse->addAssign('Fehleranzeige', 'innerHTML', '');
	}
	return $objResponse;
}
/**
 * Buchungsübersicht:
 * Bucht den entsprechenden Artikel hinzu und zeigt die Zelle entsprechend an
 */
function fuegeArtikelHinzu($buchung_nr, $artikel_id, $datum, $artikelnummern)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($buchung_nr) || ! is_numeric($artikel_id) || ! is_numeric($datum) )
	{
		$objResponse->addAlert(utf8_encode('Fehler - ungültige Argumente.'));
	}
	else
	{
		$Buchung = new Buchung($buchung_nr);
		$Eintrag['Menge'] = $Buchung->standardMengeBeiArtikel($artikel_id);
		$Buchung->bucheArtikel($artikel_id, $datum, $Eintrag['Menge']);
		$Smarty = new Smarty;
		$Smarty->template_dir = TEMPLATEPATH;
		$Artikel['id'] = $artikel_id;
		$Smarty->assign('artikel', $Artikel);
		$Eintrag['Datum'] = $datum;
		$Eintrag['Farbe'] = '';
		$Smarty->assign('eintrag', $Eintrag);
		$buchung['Buchung_Nr'] = $buchung_nr;
		$Smarty->assign('buchung', $buchung);
		$Smarty->assign('mitZeit', false);
		$docinput['Artikelnummern'] = $artikelnummern;
		$Smarty->assign('docinput', $docinput);
		$Smarty->assign('Programmpaket', false);
		$content = utf8_encode($Smarty->fetch('Buchungsuebersicht_AktiveZelle.tpl'));
		$objResponse->addAssign('Schlaf'.$datum, 'innerHTML', Max(0,$Buchung->BerechneAlleSchlafplaetze($datum, $artikelnummern)));
		$objResponse->addAssign('A'.$artikel_id.'D'.$datum, 'innerHTML', $content);
		$s = '';
		$l = $Buchung->getFehlerliste();
		foreach ($l as $feld)
		{
			$s .= 'Artikel '.$feld['Bezeichnung'].' doppelt gebucht am '.date('d.m.Y',$feld['Datum']);
			if ( date('H:i',$datum) != '00:00' )
			{
				$s .= ' '.date('H:i',$datum);
			}
			$s .= ' von Buchung <a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$feld['Buchung_Nr']).
			'">'.$feld['Buchung_Nr'].'</a> ('.$feld['Menge'].' mal)<br />';
		}
		$objResponse->addAssign('Fehleranzeige', 'innerHTML', $s);
	}
	return $objResponse;
}

function gesamteReise($buchung_nr, $artikel_id, $art, $setzen,$artikelnummern, $objResponse=null)
{
	if ( $objResponse==null)
	{
		$objResponse = new xajaxResponse();
	}
	if ( ! is_numeric($buchung_nr) || ! is_numeric($artikel_id) || ! is_numeric($art))
	{
		$objResponse->addAlert(utf8_encode('Fehler - ungültige Argumente.'));
	}
	else
	{
		$Smarty = new Smarty;
		$Smarty->template_dir = TEMPLATEPATH;
		$Buchung = new Buchung($buchung_nr);
		$tag = $Buchung->Von;
		$Artikel['id'] = $artikel_id;
		$Smarty->assign('artikel', $Artikel);
		// Problem: Anpassen wenn Essen etc.
		$Eintrag['Menge'] = $Buchung->standardMengeBeiArtikel($artikel_id);
		$Eintrag['Farbe'] = '';
		$Smarty->assign('buchung', array('Buchung_Nr'=> $Buchung->Buchung_Nr));
		$Smarty->assign('mitZeit', false);
		$docinput['Page'] = $art;
		$docinput['Artikelnummern'] = $artikelnummern;
		$Smarty->assign('docinput', $docinput);
		$Smarty->assign('Programmpaket', false);
		while ( $tag <= $Buchung->Bis)
		{
			$Eintrag['Datum'] = $tag;
			$Smarty->assign('eintrag', $Eintrag);
			$Smarty->assign('tagnr', $tag);
			if ( $setzen == "true" )
			{
				$Buchung->bucheArtikel($artikel_id, $tag, $Eintrag['Menge']);
				$content = $Smarty->fetch('Buchungsuebersicht_AktiveZelle.tpl');
			}
			else
			{
				$Buchung->entbucheArtikel($artikel_id, $tag, 0); // alle löschen
				$content = $Smarty->fetch('Buchungsuebersicht_InaktiveZelle.tpl');
			}
			$content = utf8_encode($content);
			$objResponse->addAssign('A'.$artikel_id.'D'.$tag, 'innerHTML', $content);
			$objResponse->addAssign('Schlaf'.$tag, 'innerHTML', Max(0, $Buchung->BerechneAlleSchlafplaetze($tag, $artikelnummern)));
			$tag = strtotime('+1day',$tag);
		}
	}
	return $objResponse;
}
function gesamterBereich($buchung_nr, $artikelnummern, $art, $setzen)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($buchung_nr) || ! is_numeric($art))
	{
		$objResponse->addAlert(utf8_encode('Fehler - ungültige Argumente.'));
	}
	else
	{
		$artikelnummern = explode(',',$artikelnummern);
		$x = array();
		foreach ($artikelnummern as $artikel_id)
		{
			gesamteReise($buchung_nr, $artikel_id, $art, $setzen,$artikelnummern, $objResponse);
		}
	}
	return $objResponse;
}
function aktualisiereKorrespondenzListe($id, $Adressen_id, $id_nr)
{
	global $DokArten;
	$objResponse = new xajaxResponse();
	if ( $id_nr[0] == 'B')
	{
		$Buchung_Nr = substr($id_nr, 1);
		$Buchungsvorlagen = holeVorlagen('Buchungen');
		$Buchungsvorlagen = array_merge($Buchungsvorlagen, holeVorlagen('Kuechenzettel'));
		$korrespondenz['Buchung_Nr'] = $Buchung_Nr;
	}
	elseif ($id_nr[0] == 'R')
	{
		$Rechnung_Nr = substr($id_nr, 1);
		// Rechnung holen
		$query = sql_query('SELECT * FROM ' . TABLE_RECHNUNGEN.' WHERE Rechnung_Nr = ' . $Rechnung_Nr);
		$rechnung = mysql_fetch_array($query);
		mysql_free_result($query);
		$Buchungsvorlagen = holeVorlagen('Fakturierung/' .$DokArten[$rechnung['Rechnung']], 'Rechnung');
		$Buchung_Nr = $rechnung['F_Buchung_Nr'];
		$korrespondenz['Buchung_Nr'] = $Buchung_Nr;
		$korrespondenz['Rechnung_id'] = $Rechnung_id;
	}
	$korrespondenz['Adressen_id']=$Adressen_id;
	$content = utf8_encode(holeKorrespondenzListe($id, $Buchungsvorlagen, $korrespondenz, $params, true));
	return $objResponse;
}
/**
 * Korrespondenz - löscht eine Datei
 */
function loescheKorrespondenz($id, $Adressen_id, $dokument, $id_nr = -1)
{
	global $DokArten;
	$objResponse = new xajaxResponse();
	// Korrespondenz-Datei löschen
	if ( !is_numeric($id_nr) && strlen($id_nr) > 0)
	{
		if ( $id_nr[0] == 'B')
		{
			$Buchung_Nr = substr($id_nr, 1);
			$Buchungsvorlagen = holeVorlagen('Buchungen');
			$Buchungsvorlagen = array_merge($Buchungsvorlagen, holeVorlagen('Kuechenzettel'));
			$korrespondenz['Buchung_Nr'] = $Buchung_Nr;
		}
		elseif ($id_nr[0] == 'R')
		{
			$Rechnung_Nr = substr($id_nr, 1);
			// Rechnung holen
			$query = sql_query('SELECT * FROM ' . TABLE_RECHNUNGEN.' WHERE Rechnung_id = ' . $Rechnung_Nr);
			$rechnung = mysql_fetch_array($query);
			mysql_free_result($query);
			$Buchungsvorlagen = holeVorlagen('Fakturierung/' .$DokArten[$rechnung['Rechnung']], 'Rechnung');
			$Buchung_Nr = $rechnung['F_Buchung_Nr'];
			$korrespondenz['Buchung_Nr'] = $Buchung_Nr;
			$korrespondenz['Rechnung_id'] = $Rechnung_Nr;
		}
	}
	elseif ( is_numeric($Adressen_id) && $Adressen_id > 0)
	{
		// Nur Adresse angegeben
		$Dateiname = getAdressenAnhangLink($Adressen_id, "*", CVJM_ENDUNG, "*");
		$DateinamePDF = getAdressenAnhangLink($Adressen_id, "*", CVJM_ENDUNG_PDF, "*");
		$Buchungsvorlagen = array_merge(glob($Dateiname), glob($DateinamePDF));
	}
	// Adresse
	$korrespondenz['Adressen_id']=$Adressen_id;
	$datei = utf8_decode($dokument);
	if ( ! unlink($datei) )
	{
		$objResponse->addAlert(utf8_encode('Datei '.$datei.' konnte nicht gelöscht werden.'));
	}
	else
	{
		//$objResponse->addAlert(utf8_encode('Datei '.$datei.' wurde gelöscht.'));
		if ( $Buchung_Nr > 0 )
		{
			global $session_userid;
			$qresult = sql_query("SELECT id FROM ".TABLE_USERS." WHERE email='".mysql_real_escape_string($session_userid)."'");
			$idresult = mysql_fetch_row($qresult);
			$idresult = $idresult[0];
			mysql_free_result($qresult);
			$query = mysql_query('SELECT value FROM '.TABLE_USERDATA.' WHERE name="nickname" AND user_id='.$idresult);
			$user = mysql_fetch_array($query);
			mysql_free_result($query);
			$nickname = $user['value'];
			// Profil laden wegen Nickname
			if ( ! sql_query('UPDATE '.TABLE_BUCHUNGEN." SET Logtext=CONCAT('".date('d.m.Y H:i').
			' Korrespondenz '.mysql_real_escape_string(utf8_decode($dokument)).' gelöscht '.
			$nickname."\n',Logtext) ".
			' WHERE Buchung_Nr='.$Buchung_Nr))
			$objResponse->addAlert(utf8_encode('Datenbankfehler beim Löschen: '.mysql_error()));
			$query = mysql_query('SELECT Logtext FROM '.TABLE_BUCHUNGEN.' WHERE Buchung_Nr='.$Buchung_Nr);
			$buchung = mysql_fetch_array($query);
			mysql_free_result($query);
			$objResponse->addAssign('Historie', 'innerHTML', utf8_encode(nl2br($buchung['Logtext'])));
		}
	}
	$content = utf8_encode(holeKorrespondenzListe($id, $Buchungsvorlagen, $korrespondenz, $params, true));
	$objResponse->addAssign('Korrespondenzdiv', 'innerHTML', $content);
	return $objResponse;
}
/**
 * aktualisiert die Liste der Korrespondenz 
 *
 * @param int $id
 * @param int $Adressen_id verwendet wenn nicht Rechnung oder Buchung 
 * @param string $id_nr B<NR> or R<NR> for Buchung / Rechnung, leer sonst
 */
function aktualisiereKorrespondenz($id, $Adressen_id, $id_nr = -1)
{
	global $DokArten;
	$objResponse = new xajaxResponse();
	// Korrespondenz-Datei löschen
	if ( !is_numeric($id_nr) && strlen($id_nr) > 0)
	{
		if ( $id_nr[0] == 'B')
		{
			$Buchung_Nr = substr($id_nr, 1);
			$Buchungsvorlagen = holeVorlagen('Buchungen');
			$Buchungsvorlagen = array_merge($Buchungsvorlagen, holeVorlagen('Kuechenzettel'));
			$korrespondenz['Buchung_Nr'] = $Buchung_Nr;
		}
		elseif ($id_nr[0] == 'R')
		{
			$Rechnung_Nr = substr($id_nr, 1);
			// Rechnung holen
			$query = sql_query('SELECT * FROM ' . TABLE_RECHNUNGEN.' WHERE Rechnung_id = ' . $Rechnung_Nr);
			$rechnung = mysql_fetch_array($query);
			mysql_free_result($query);
			$Buchungsvorlagen = holeVorlagen('Fakturierung/' .$DokArten[$rechnung['Rechnung']], 'Rechnung');
			$Buchung_Nr = $rechnung['F_Buchung_Nr'];
			$korrespondenz['Buchung_Nr'] = $Buchung_Nr;
			$korrespondenz['Rechnung_id'] = $Rechnung_id;
		}
	}
	elseif ( is_numeric($Adressen_id) && $Adressen_id > 0)
	{
		// Nur Adresse angegeben
		$Dateiname = getAdressenAnhangLink($Adressen_id, "*", CVJM_ENDUNG, "*");
		$DateinamePDF = getAdressenAnhangLink($Adressen_id, "*", CVJM_ENDUNG_PDF, "*");
		$Buchungsvorlagen = array_merge(glob($Dateiname), glob($DateinamePDF));
	}
	// Adresse
	$korrespondenz['Adressen_id']=$Adressen_id;
	$content = utf8_encode(holeKorrespondenzListe($id, $Buchungsvorlagen, $korrespondenz, $params, true));
	$objResponse->addAssign('Korrespondenzdiv', 'innerHTML', $content);
	return $objResponse;
}
function setzeAdressKategorie($Adressen_id, $Kategorie_id, $set = true)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($Adressen_id) || ! is_numeric($Kategorie_id))
	{
		$objResponse->addAlert('Fehler- keine numerischen Argumente');
	}
	else
	{
		$set = convertJavaScriptBool($set);
		if ( $set )
		{
			sql_query('INSERT INTO '.TABLE_ADRESSEN_KATEGORIE.' (F_Adressen_id,F_Kategorie_id) VALUES ('.$Adressen_id.','.$Kategorie_id.')');
		}
		else
		{
			sql_query('DELETE FROM '.TABLE_ADRESSEN_KATEGORIE.' WHERE F_Adressen_id='.$Adressen_id.' AND F_Kategorie_id='.$Kategorie_id);
		}
		// Anzahlen korrigieren
		$query = mysql_query('SELECT Count(*) FROM '.TABLE_ADRESSEN_KATEGORIE.' WHERE F_Kategorie_id='.$Kategorie_id);
		$anzahl = mysql_fetch_row($query);
		$anzahl = $anzahl[0];
		mysql_free_result($query);
		$objResponse->addAssign('K'.$Kategorie_id, 'innerHTML', $anzahl);
	}
	return $objResponse;
}

function zeigeKategorien($Adressen_id, $edit = false)
{
	$objResponse = new xajaxResponse();
	$Smarty = new Smarty();
	$Smarty->template_dir = TEMPLATEPATH;
	$Adresse = new Adresse($Adressen_id);
	$edit = convertJavaScriptBool($edit);
	$Smarty->assign_by_ref('Adresse', $Adresse);
	if ( $edit )
	{
		$content = $Smarty->fetch('Adressen_Kategorien_Edit.tpl');
		$objResponse->addAssign('KategorienLink', 'innerHTML', 'Kategoriebearbeitung beenden');
		$objResponse->addEvent('KategorienLink', 'onclick', 'xajax_zeigeKategorien('.$Adressen_id.',false);return false;');
	}
	else
	{
		$content = $Smarty->fetch('Adressen_Kategorien_Liste.tpl');
		$objResponse->addAssign('KategorienLink', 'innerHTML', 'Kategorien festlegen');
		$objResponse->addEvent('KategorienLink', 'onclick', 'xajax_zeigeKategorien('.$Adressen_id.',true);return false;');
	}
	$content = utf8_encode($content);
	$objResponse->addAssign('Kategorien', 'innerHTML', $content);
	return $objResponse;
}

function zeigeSeminarKorrespondenz($Buchung_Nr, $Adressen_id, $zeigen = true)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($Buchung_Nr) || ! is_numeric($Adressen_id))
	{
		$objResponse->addAlert('Falsche Parameter!');
	}
	else
	{

		if ( convertJavaScriptBool($zeigen) )
		{
			$Vorlagen = holeVorlagen('Seminare');

			$content = holeKorrespondenzliste(0, $Vorlagen,
			array('Buchung_Nr'=>$Buchung_Nr, 'Adressen_id'=>$Adressen_id),
			array(), false);
			$objResponse->addEvent('KorrLink'.$Adressen_id, 'onclick', 'xajax_zeigeSeminarKorrespondenz('.
			$Buchung_Nr.','.$Adressen_id.',false);');
			$objResponse->addAssign('KorrLink'.$Adressen_id, 'innerHTML', 'W');
		}
		else
		{
			$content = '';
			$objResponse->addEvent('KorrLink'.$Adressen_id, 'onclick', 'xajax_zeigeSeminarKorrespondenz('.
			$Buchung_Nr.','.$Adressen_id.',true);');
			$objResponse->addAssign('KorrLink'.$Adressen_id, 'innerHTML', 'K');
		}
		$objResponse->addAssign('Korrespondenz'.$Adressen_id, 'innerHTML', utf8_encode($content));
	}
	return $objResponse;

}

/**
 * Sendet eine Mail an einen Kunden. Diese enthält als Anhang die angegebenen
 * Dateien
 * @param int $Adressen_id die ID des Kunden
 * @param String $id_nr die ID der Buchung oder Rechnung (Prefix B oder R, leer wenn nicht vorhanden
 * @param String $Betreff der Betreff der Mail, wird ggf. um die Buchungsnummer ergänzt
 * @param String $Text der Inhalt der Mail
 * @param array $dateien die Dateinamen der anzuhängenden Dateien, diese werden mit ; getrennt
 */
function sendeKorrespondenzMail($Adressen_id, $id_nr, $Betreff, $Text, $dateien, $mailadresse = '')
{
	$objResponse = new xajaxResponse();
	if ( !is_numeric($id_nr) && strlen($id_nr) > 0)
	{
		if ( $id_nr[0] == 'B')
		{
			$Buchung_Nr = substr($id_nr, 1);
		}
		elseif ($id_nr[0] == 'R')
		{
			$Rechnung_Nr = substr($id_nr, 1);
			// Rechnung holen
			$query = sql_query('SELECT * FROM ' . TABLE_RECHNUNGEN.' WHERE Rechnung_Nr = ' . $Rechnung_Nr);
			$rechnung = mysql_fetch_array($query);
			mysql_free_result($query);
			$Buchung_Nr = $rechnung['F_Buchung_Nr'];
				
		}
		try{
			$Buchung = new Buchung($Buchung_Nr);
			$Adresse = $Buchung->Adresse;
			$Betreff = 'Buchung '.$Buchung_Nr.' '.$Betreff;
		}
		catch ( Exception $e )
		{
			$objResponse->addAlert(utf8_encode('Ungültige Referenz!'));
		}
	}
	else
	{
		try {
			$Adresse = new Adresse($Adressen_id);
			if ( $Adresse->Kunden_Nr != '' ) 
			{
				$Betreff = 'Kundennummer '.$Adresse->Kunden_Nr.' '.$Betreff;
			}
		}
		catch ( Exception $e )
		{
			$objResponse->addAlert(utf8_encode('Ungültige Adressen-ID'));
		}
	}
	if ( isset($Adresse))
	{
		if ( $Adresse->Email != '' || $mailadresse != '')
		{
			require('php/Mail.php');
			require_once('php/Mail/mime.php');
			require_once('php/MIME/Type.php');
			try{
				$mime = new Mail_mime("\n");
				$mime->setTXTBody(utf8_decode($Text));
				foreach ( $dateien as $datei)
				{
					$datei = utf8_decode($datei);
					$p = pathinfo($datei);
					switch (strtolower($p['extension']))
					{
						case 'pdf':
							$type= 'application/pdf';
							break;
						case 'sxw':
							$type= 'application/vnd.sun.xml.writer';
							break;
						case 'odt':
							$type= 'application/vnd.oasis.opendocument.text';
							break;
						default:
							$type= 'application/octet-stream';

					}
					$mime->addAttachment($datei, $type);
				}
				//do not ever try to call these lines in reverse order
				$body = $mime->get();
				$hdrs = array('From'=>'buchhaltung@cvjm-feriendorf.de', 
				'Bcc'=>'info@cvjm-feriendorf.de', // get_user_email()
				'Subject'=>utf8_decode($Betreff));
				$hdrs = $mime->headers($hdrs);

				$mail =& Mail::factory('mail');
				if ( $mailadresse == '' ) 
					$mailadresse = $Adresse->Email;
				if ( $mail->send($mailadresse, $hdrs, $body) === true )
				{
					$objResponse->addAlert('Die Korrespondenz '.implode(',',$dateien).
					' wurde per Mail an '.$mailadresse.' versendet.');
					if ( isset($Buchung))
					{
						$Buchung->logAction('Korrespondenz '.mysql_real_escape_string(utf8_decode(implode(',',$dateien))).
							' per Mail mit Betreff '.utf8_decode($Betreff).' an '.
							mysql_real_escape_string($mailadresse).' gesendet');
							// TODO Korrektur URL für Verlinkung übergeben
						$objResponse->addAssign('Historie', 'innerHTML', utf8_encode(nl2br($Buchung->holeHistory(''))));
						// Speichern der Mail-Datei bei der Buchung
						$mail = 'An: '.$mailadresse."\n";
						$mail.= 'Betreff: '.$Betreff."\n";
						$mail.= $Text."\n";
						$mail.= 'Anhang: '.implode(',',$dateien);
						$datei  = fopen($Buchung->Adresse->getAdressenAnhangLink($Buchung->Buchung_Nr, '.txt','Mail',date('dmyHi')),'w');
                        fwrite($datei, $mail);
						fclose($datei);
					}
				}
				else
				{
					$objResponse->addAlert('Beim Versenden ist ein Fehler aufgetreten!');
				}
			}
			catch ( Exception $e)
			{
				$objResponse->addAlert('Fehler:'.$e->getMessage());
			}
		}
		else
		{
			$objResponse->addAlert('Der Kunde hat keine E-Mail-Adresse angegeben!');
		}
	}
	else
	{
		$objResponse->addAlert('Konnte keine korrekte ID erkennen.');
	}
	return $objResponse;
}

/**
 * @param int $Adressen_id die ID der Adresse
 * @param int $Art die Art (1 - 4)
 * @param boolean $wert true zum Setzen, false zum Entfernen
 * @param int Zuordnung ID die Zugeordnet werden soll
 */
function aendereZuordnung($Adressen_id, $art, $wert, $Zuordnung)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($Adressen_id) || !is_numeric($Zuordnung))
	{
		$objResponse->addAlert(utf8_encode('Ungültige Adressen_id!'));
	}
	else
	{
		$wert = convertJavaScriptBool($wert);
		$a = new Adresse($Adressen_id);
		// Kategorie
		if ($art == 1 )
		{
			$a->setKategorie($Zuordnung, $wert);
		}
		elseif ($art == 2 )
		{
			$a->setAnsprechpartner($Zuordnung, $wert);
		}
		elseif ($art == 3 )
		{
			$a->setInstitution($Zuordnung, $wert);
		}
		elseif ($art == 4)
		{
			$a->setVersandmarker($wert);
		}
	}
	return $objResponse;
}

function sucheArtikel($suche, $Buchung_Nr, $Spalten, $id, $Rechnung_id=-1)
{
	$objResponse = new xajaxResponse();
	$Buchung = new Buchung($Buchung_Nr);
	$Smarty = new Smarty();
	$Smarty->template_dir = TEMPLATEPATH;
	$Smarty->assign('spalten', $Spalten);
	$Smarty->assign('id', $id);
	$Smarty->assign_by_ref('Buchung', $Buchung);
	$docinput = array();
	$docinput['ArtikelSearch']=utf8_decode($suche);
	if ( $Rechnung_id >0)
	{
		$docinput['Rechnung_id'] = $Rechnung_id;
	}
	$Smarty->assign('docinput', $docinput);
	$content = $Smarty->fetch('Buchung_Artikel_hinzu.tpl');
	$objResponse->addAssign('Artikel_Hinzu', 'innerHTML', utf8_encode($content));
	return $objResponse;
}
/**
 * Verschiebt einen Artikel im Artikelbaum
 * @param int $id die ID der Seite
 * @param int $Artikel_id die ID des Artikels der verschoben werden soll
 * @param char $Richtung die Richtung der Verschiebung (u,d,l,r)
 */
function moveArtikel($id, $Artikel_id, $Richtung)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($Artikel_id)  || ! is_numeric($id))
	{
		$objResponse->addAlert(utf8_encode('Ungültige Artikel-id'));
	}
	else
	{
		$Artikel = new Artikel($Artikel_id);
		$Artikel->bewegen($Richtung);
		$Smarty = new Smarty();
		$Smarty->template_dir = TEMPLATEPATH;
		$Smarty->assign('id', $id);
		$Smarty->assign_by_ref('Artikel', $Artikel);
		$content = $Smarty->fetch('Artikel_Baumliste.tpl');
		$objResponse->addAssign('Artikelbaumliste', 'innerHTML', utf8_encode($content));
	}
	return $objResponse;
}
/**
 * ändert die Anzahl der Einträge in der Artikelbaumliste
 * @param int $id die ID der Seite
 * @param int $Artikel_id die ID des Artikels der gerade angezeigt wird
 * @param int/String $Anzahl die Anzahl oder "Alle" die angezeigt werden soll
 */
function aendereArtikelAnzahl($id, $Artikel_id, $Anzahl)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($Artikel_id) || ! is_numeric($id))
	{
		$objResponse->addAlert(utf8_encode('Ungültige id!'));
	}
	else
	{
		$_SESSION['ArtikelAnzahl'] = $Anzahl;
		// TODO: Im Profil speichern
		$Artikel = new Artikel($Artikel_id);
		$Smarty = new Smarty();
		$Smarty->template_dir = TEMPLATEPATH;
		$Smarty->assign_by_ref('Artikel', $Artikel);
		$Smarty->assign('id', $id);
		$content = $Smarty->fetch('Artikel_Baumliste.tpl');
		$objResponse->addAssign('Artikelbaumliste', 'innerHTML', utf8_encode($content));
	}
	return $objResponse;
}
/**
 * zeigt zu einem gegebenen Barcode den passenden Artikel an. Als Anzeige erfolgt im Feld "Artikel" die
 * Ausgabe der Bezeichnung des Artikels
 *
 * @param String $Barcode der Barcode des Artikels
 */
function zeigeArtikelNachBarcode($Barcode)
{
	$objResponse = new xajaxResponse();
	$Artikelfeld = Artikel::search($Barcode);
	if ( Count($Artikelfeld)> 0)
	{
		$objResponse->addAssign('Artikel', 'innerHTML', $Artikelfeld[0]->Bezeichnung);
	}
	else
	{
		$objResponse->addAssign('Artikel', 'innerHTML', '');
	}
	return $objResponse;
}

function schliessePersonenwahlDialog()
{
	$objResponse = new xajaxResponse();
	$objResponse->addRemove('Dialog');
	$objResponse->addAssign('Artikeltabelle','style.opacity','1.0');
	return $objResponse;
}
function zeigePersonenwahlDialog($Buchung_Nr, $Artikel_Nr, $Datum)
{
	global $groups;
	$objResponse = new xajaxResponse();
	$Artikel = new Artikel($Artikel_Nr);
	// Datum als XXX-XXX - auseinanderbrechen!
	$events = Event::searchBuchungEvent($Datum, $Datum, $Buchung_Nr, $Artikel_Nr);
	$markiertU = array();
	$markiertG = array();
	$extern = '';
	foreach ( $events as $event )
	{
		
		foreach ( $event->Betroffene as $nr => $betroffen)
		{			
			if ( $betroffen['Betroffener_Art'] == EVENT_BETROFFENER_USER)
			{
				$markiertU[] = $betroffen['F_Betroffener_id'];
			}
			elseif ($betroffen['Betroffener_Art'] == EVENT_BETROFFENER_EXTERN)
			{
				$extern .= $betroffen['Betroffener']."\n";
			}
			elseif ($betroffen['Betroffener_Art'] == EVENT_BETROFFENER_GRUPPE)
			{
				$markiertG[] = $betroffen['F_Betroffener_id'];
			}			
		}
	}
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH;
	$users = array();
	$query = 'SELECT id FROM '.TABLE_USERS.' WHERE valid';
	$query = sql_query($query);
	while ( $row = sql_fetch_row($query))
	{
		$users[$row[0]] = get_user_nickname($row[0]);
	}
	sql_free_result($query);
	asort($users);
	$Smarty->assign_by_ref('users', $users);
	init_groups();
	$Smarty->assign_by_ref('groups', $groups);
	$Smarty->assign('title', 'Zuständiger für '.$Artikel->Bezeichnung.' am '.date('d.m.Y',$Datum));
	$Smarty->assign('extern', $extern);
	$Smarty->assign('markiertU', $markiertU);
	$Smarty->assign('markiertG', $markiertG);
	$Smarty->assign_by_ref('Artikel', $Artikel);
	$dialog = $Smarty->fetch('Personenauswahlliste.tpl');
	$objResponse->addPrepend('Personenwahldialog','innerHTML',utf8_encode($dialog));
	$objResponse->addAssign('Artikeltabelle','style.opacity','0.6');
	return $objResponse;
}
/**
 * trägt die zuständige Person für einen Buchungseintrag ein. Wird keine Person angegeben, wird der Eintrag
 * gelöscht.
 * @param int $Buchung_Nr die Buchungsnummer
 * @param int $Artikel_Nr die Artikelnummer
 * @param int $Datum der Zeitpunkt (Datum+Zeit)
 * @param string $Person die einzutragende Person
 * @param int $id die ID des HTML-Feldes das geändert werden muss
 */
function tragePersonEin($Buchung_Nr, $Artikel_Nr, $Datum, $Person,$id)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($Buchung_Nr) || ! is_numeric($Artikel_Nr) )
	{
		$objResponse->addAlert(utf8_encode('Ungültige Parameter übergeben!'));
	}
	else
	{
		$Daten = array();
		$Fehler = false;
		if ( ! is_numeric($Datum))
		{
			// Datum in zwei Daten zerlegen
			$zeitraum = explode('-',$Datum);
			if ( Count($zeitraum)==2 && is_numeric($zeitraum[0]) && is_numeric($zeitraum[1]))
			{
				for ( $i=$zeitraum[0]; $i<= $zeitraum[1];$i=strtotime('+1 day',$i))
				{
					$Daten[] = $i;
				}
			}
			else
			{
				$Fehler = true;
			}
		}
		else
		{
			$Daten[]= $Datum;
		}
		// Neu: Event erstellen, Betroffene einrichten
		// prüfen ob ein Event existiert
		foreach ( $Daten as $Datum )
		{
			$text = '';
			$events = Event::searchBuchungEvent($Datum, $Datum, $Buchung_Nr, $Artikel_Nr);
			if ( Count($events) == 0)
			{
				// Neuen Event anlegen
				$event = new Event();
				$event->Titel = 'Buchung '.$Buchung_Nr;
				$event->Datum = $Datum;
				$event->Art = EVENT_ART_AUFTRAG;
				$event->Referenz = EVENT_REFERENZ_BUCHUNG.$Buchung_Nr.'/'.
				EVENT_REFERENZ_ARTIKEL.$Artikel_Nr.'/';
			}
			else
			{
			// wir nehmen nur den ersten Event
				$event = $events[0];
			}
			$event->Beschreibung = $Person['Beschreibung'];
			// alle Betroffenen entfernen
			$event->Betroffene = array();
			$text = '';
			foreach ( $Person['User'] as $user_id => $value)
			{
				$user = array();
				$user['F_Betroffener_id'] = $user_id;
				$user['Betroffener_Art'] = EVENT_BETROFFENER_USER;
				$user['Bestaetigungsstatus'] = EVENT_BESTAETIGUNG_ANGEFORDERT;
				$user['Betroffener'] = '';
				$event->Betroffene[] = $user;
				$text .= get_user_nickname($user_id).' ';
			}
			global $groups;
			init_groups();
			foreach ( $Person['Group'] as $user_id => $value)
			{
				$user = array();
				$user['F_Betroffener_id'] = $user_id;
				$user['Betroffener_Art'] = EVENT_BETROFFENER_GRUPPE;
				$user['Bestaetigungsstatus'] = EVENT_BESTAETIGUNG_KEINE;
				$user['Betroffener'] = '';
				$event->Betroffene[] = $user;
				$text .= $groups[$user_id].' ';
			}
			$extern = explode("\n", str_replace("\r",'',$Person['Externe']));
			foreach ( $extern as $value)
			{
				if ( trim($value) != '')
				{
				  $user = array();
				  $user['F_Betroffener_id'] = -1;
				  $user['Betroffener_Art'] = EVENT_BETROFFENER_EXTERN;
				  $user['Bestaetigungsstatus'] = EVENT_BESTAETIGUNG_KEINE;
				  $user['Betroffener'] = trim($value);
				  $event->Betroffene[] = $user;
				  $text .= $value.' ';
				}
			}
			// Event speichern
			if ( count($event->Betroffene) == 0)
			{
				$event->loeschen();
			}
			else 
			{
				$event->save();	
			}
		}
		/*
		foreach ( $Daten as $Datum )
		{
			if ( !mysql_query('INSERT INTO '.TABLE_BUCHUNGPERSONEN.' (F_Buchung_Nr,PersonenArt,F_Artikel_Nr,Datum,Status,Person) '.
			'VALUES ('.$Buchung_Nr.',"Person",'.$Artikel_Nr.','.$Datum.',"","'.mysql_real_escape_string($Person).
			'") ON DUPLICATE KEY UPDATE Person="'.mysql_real_escape_string(trim($Person)).'"'))
			{
				$Fehler = true;
			}
			// Leere Personen entfernen
			mysql_query('DELETE FROM '.TABLE_BUCHUNGPERSONEN.' WHERE Person=""');
		}
		*/
		if ( $Fehler )
		{
			$objResponse->addAlert('Fehler - Konnte Ereignis nicht eintragen.'.mysql_error());
		}
		else
		{
			if( $text!='')
			{
				$text = 'Zuständig: '.$text;
			}
			$objResponse->addAssign('Person'.$id,'innerHTML',utf8_encode($text));
		}

	}
	$objResponse->addRemove('Dialog');
	$objResponse->addAssign('Artikeltabelle','style.opacity','1.0');
	return $objResponse;
}
/**
 * erzeugt eine CVJM-EAN (nächste freie Nummer) und schreibt sie ins Feld
 * docinput[EAN]
 */
function createEAN()
{
	$objResponse = new xajaxResponse();
	$EAN = Artikel::findNextCVJMEAN();
	$objResponse->addAssign('EAN', 'value', $EAN);
	$objResponse->addAssign('Code', 'innerHTML',
	'<img src="/barcode.php?Anzeige='.$EAN.'" alt="Barcode" title="'.$EAN.'"/>');
	return $objResponse;
}

function showKundenAuswahl($Buchung_Nr)
{
	//  <input type="Text" name="docinput[KuNr]" value="{$Buchung->Adresse->Kunden_Nr}" size="6" maxlength="8" />
	$objResponse = new xajaxResponse();
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH;
	$Liste = utf8_encode($Smarty->fetch('Buchung_Kundenauswahl.tpl'));
	$objResponse->addAssign('KundeAendernFeld', 'innerHTML', $Liste);
	$objResponse->addAssign('KundeAendernFeld', 'style.visibility', '');
	$objResponse->addAssign('KundeAendern', 'style.visibility', 'hidden');
	return $objResponse;
}
function sucheKunden($Name)
{
	$objResponse = new xajaxResponse();
	$Kunden = array();
	$Name = trim(mysql_real_escape_string(utf8_decode($Name)));
	$query = mysql_query('SELECT Adressen_id FROM '.TABLE_ADRESSEN.' WHERE Kunden_Nr>0 AND (Name LIKE "%'.
	$Name.'" OR Vorname LIKE "%'.$Name.'%" OR Kunden_Nr="'.$Name.'") LIMIT 20');
	while ( $k = mysql_fetch_row($query))
	{
		$Kunden[] = new Adresse($k[0]);
	}
	mysql_free_result($query);
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH;
	$Smarty->assign('Kunden', $Kunden);
	$Liste = utf8_encode($Smarty->fetch('Buchung_Kundenauswahl.tpl'));
	$objResponse->addAssign('KundeAendernFeld', 'innerHTML', $Liste);
	return $objResponse;
}

function showInstitutionMail($Institution)
{
	$objResponse = new xajaxResponse();
	$Adresse = new Adresse($Institution);
	$Mail = '';
	if ( $Adresse->Email != '')
	{
		$Mail = '<a href="#Korrespondenz">Korrespondenz an '.$Adresse->Email.'</a>';
	}
	$objResponse->addAssign('Institutionsmail', 'innerHTML', $Mail);
	return $objResponse;

}
require ('inc/cvjm_ajax.php');
$xajax->processRequests();
?>
