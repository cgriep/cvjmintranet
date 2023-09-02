<?php

/* Modul Auftrag

AWF-Name: auftrag
Parameter:
Delete - Löscht Auftrag mit id

Auftrag - id des Auftrages
Titel, Betroffen, Status, Prioritaet, Ort - Speichert einen Auftrag
newauf - Neuer Auftrag

docinput[VTitel] - Vorgabe für Titel

*/
include(INC_PATH.'misc/CVJM.inc');
include(INC_PATH.'misc/cvjmArtikeltools.inc');
include(INC_PATH.'misc/cvjmAuftragstools.inc');
include(INC_PATH.'cvjm/EventFactory.class.php');
require_once('INC_PATH.'Smarty/Smarty.class.php');
$Smarty = new Smarty;
$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';

$BenutzerURL = '';
$Benutzer = SESSION_DBID;
if ( isset($profile[CVJM_VERWALTUNGSGRUPPE]))
{
	if ( isset($docinput['Benutzer']) && is_numeric($docinput['Benutzer']))
	{
		$Benutzer = $docinput['Benutzer'];
		$BenutzerURL = '&docinput[Benutzer]='.$Benutzer;
	}
}

/*
 * Löschen eines Auftrags, nur der Autor darf das
 */
if ( is_numeric($_REQUEST['Delete']) )
{
	$event = EventFactory::getEvent($_REQUEST['Delete']);
	if ( $event->istAutor(SESSION_DBID) )
	{
		$event->loeschen();
		echo '<div class="Hinweis">Auftrag '.$_REQUEST['Delete'].' wurde gelöscht.</div>';
	}
}
/*
 * Speichern eines Auftrages
 */
if ( isset($_REQUEST['Titel']) && is_numeric($_REQUEST['Auftrag']))
{
	$Auftrag = new Auftrag($_REQUEST['Auftrag']);
	$Auftrag->Status = $_REQUEST['Status'];
	$Auftrag->Titel = $_REQUEST['Titel'];
	$Auftrag->Beschreibung = $_REQUEST['Beschreibung'];
	$Auftrag->Prioritaet = $_REQUEST['Prioritaet'];
	$Auftrag->Bemerkung = $_REQUEST['Bemerkung'];
	$Auftrag->Ort = $_REQUEST['Ort'];
	$Auftrag->Betroffene = array(); 
	/*
	 * falls nur ein Feld von Betroffenen übergeben wird, dann umwandeln mit
	 * Standardwerten
	 */
	if (isset($_REQUEST['Betroffen']) && is_array($_REQUEST['Betroffen']))
	{
		foreach ( $_REQUEST['Betroffen'] as $value)
		{
				$betroffener = array();
				$betroffener['F_Betroffener_id'] = $value;
				$betroffener['Erinnerungsstatus'] = 0;
				$betroffener['Bestaetigungsstatus'] = EVENT_BESTAETIGUNG_KEINE;
				if ( is_numeric($value) )
				  $betroffener['Betroffener_Art'] = EVENT_BETROFFENER_USER;
				else
				  $betroffener['Betroffener_Art'] = EVENT_BETROFFENER_GRUPPE;
				$betroffener['Betroffener'] = ''; // Name 
				$Auftrag->Betroffene[] = $betroffener;
		}
	}
	if ( count($Auftrag->Betroffene) == 0)
	{
		$Auftrag->Betroffene = array();
		$Auftrag->Betroffene[] = $Benutzer;
	}
	$Auftrag->Save();	
}
// Abfrage, ob der Status eines Auftrages geändert werden soll
if ( isset($_REQUEST['Auftrag']) && isset($_REQUEST['NewStatus']) &&
is_numeric($_REQUEST['Auftrag']) && is_numeric($_REQUEST['NewStatus']) ) {
	$Auftrag = new Auftrag($_REQUEST['Auftrag']);
	$Auftrag->neuerStatus($_REQUEST['NewStatus']);
}

$Smarty->assign('BenutzerID', $Benutzer);
$Smarty->assign('Benutzer', get_user_nickname($Benutzer));
$Smarty->assign('BernutzerURL', $BenutzerURL);
$Smarty->assign('id', $id);

// wenn kein einzelnen Auftrag angezeigt wird, dann Tabelle anzeigen
if ( ! ((isset($_REQUEST['EditAuftrag']) && is_numeric($_REQUEST['EditAuftrag'])) ||
isset($_REQUEST['newauf'])))
{
	//remove_profile('AuftragUnterbrochen');
	//unset($profile['AuftragUnterbrochen']);

	if ( ($_REQUEST['Alle'] == 1 || $profile['AlleAuftraege'] == 1) && $_REQUEST['Alle'] != '0' ) {
		$Alle = true;
		if ( !isset($profile['AlleAuftraege']) ) set_profile('AlleAuftraege', 1);
	}
	else {
		$Alle = false;
		if ( $profile['AlleAuftraege'] == 1 ) remove_profile('AlleAuftraege');
	}
	if ( is_numeric($_REQUEST['SearchOrt'] ) ) {
		$Ort = $_REQUEST['SearchOrt'];
	}
	else
	{
		$Ort = -1;
	}

	$Smarty->assignbyref('Profil', $profile);
	if ( ! isset($profile['AlleAuftraege']) )
	{
		$Smarty->assign('Alle', '1');
		$Smarty->assign('AlleText', 'auch ');
	}
	else
	{
		$Smarty->assign('Alle', '0');
		$Smarty->assign('AlleText', 'nur un');
	}

	if ( !isset($_REQUEST['MyWork']) ) {
		$Betroffen = SESSION_DBID;
	}
	else
	{
		$Betroffen = $_REQUEST['MyWork'];
	}
	if ( ! isset($_REQUEST['Sort']))
	{
		$_REQUEST['Sort'] = 'Prioritaet,Datum DESC';
	}
	$Auftraege = Auftrag::searchAuftraege($Benutzer, $Alle, $Ort, $_REQUEST['Sort']);
	$Smarty->assignbyref('Auftraege', $Auftraege);
	$Smarty->assignbyref('Statusse', Event::$Statusse);
	if ( isset($profile[CVJM_VERWALTUNGSGRUPPE]) )
	{
		// Berechtigter Nutzer: Anzeige der User
		$query = 'SELECT id FROM '.TABLE_USERS.' WHERE valid';
		$query = sql_query($query);
		while ( $row = sql_fetch_row($query))
		{
			$user[$row[0]] = get_user_nickname($row[0]);
		}
		sql_free_result($query);
		$u = $user;
		asort($u);
		$Smarty->assignbyref('User', $u);
	}
	$Smarty->display('Auftragsliste.tpl');
}
else
// einen speziellen Auftrag anzeigen
if ( is_numeric($_REQUEST['EditAuftrag']) || isset($_REQUEST['newauf']))
{
	if ( is_numeric($_REQUEST['EditAuftrag']))
	{
		$Auftrag = new Auftrag($_REQUEST['EditAuftrag']);
	}
	else
	{
		$Auftrag = new Auftrag();
	}
	OrtsAuswahlScriptEinfuegen();
	
	$Smarty->assignbyref('Auftrag', $Auftrag);
	$Smarty->display('Auftrag_Daten.tpl');
}

?>
