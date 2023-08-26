<?php


require_once ('inc/xajax/xajax.inc.php');

/* Datenbank- und AWF-Funktionalit�t einbinden */
include ('inc/functions.inc');
include ('inc/licence.key');
include ('inc/sessions.inc');
include ('inc/caching.inc');
include ('inc/config.inc');
include ('inc/misc/drow.inc');

/* pr�fen ob eine Session f�r den Benutzer existiert */
if (!isset ($session_userid) || $session_userid == '')
{
	die('Unauthorisiert!');
}
// Benutzer-ID heraussuchen
define('SESSION_DBID', get_user_id($session_userid));
define('SESSION_STATUS','ok');
$profile = get_profile();

$xajax = new xajax('drow_server.php');
$xajax->registerFunction('sichereGunststufe');
$xajax->registerFunction('zeigeGunststufe');
$xajax->registerFunction('zeigeGunst');
$xajax->registerFunction('zeigeFertigkeit');
$xajax->registerFunction('sichereName');
$xajax->registerFunction('sichereGunstpunkte');
$xajax->registerFunction('sichereRasse');
$xajax->processRequests();

function sichereGunstpunkte($charakter_id, $punkte)
{
	global $profile;
	$objResponse = new xajaxResponse();
	if (!is_numeric($charakter_id) || ! is_numeric($punkte) )
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente: '.$charakter_id.' Punkte: '.$punkte));
		return $objResponse;
	}
	if ( $profile['SteinsbergSL'] != 1 )
	{
		$objResponse->addAlert(utf8_encode('Fehler - nur SL darf Punkte speichern: '.$charakter_id));
		return $objResponse;
	}
	// Charakter holen
	$charakter = array();
	$charakter['Gunstpunkte'] = $punkte;
	try {
		save_charakter($charakter_id, $charakter);
	}
	catch ( Exception $e )
	{
		$objResponse->addAlert(utf8_encode($e->getMessage()));
	}
	$Gunst = bestimmeCharGunst($charakter_id);
	$objResponse->addAssign('Gunstgesamt', 'innerHTML', $Gunst);
	return $objResponse;
}
function sichereRasse($charakter_id, $rasse)
{
	global $profile;
	$objResponse = new xajaxResponse();
	if (!is_numeric($charakter_id) || ! is_numeric($rasse) )
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente: '.$charakter_id.' Rasse: '.$rasse));
		return $objResponse;
	}
	// Charakter holen
	$charakter = array();
	$charakter['F_Rasse_id'] = $rasse;
	if ( $rasse != 1 ) 
	{
		$charakter['Adlig'] = 0; // nur Drow k�nnen adlig sein
		$objResponse->addAssign('charakter[Adlig]', 'checked', '');
		$objResponse->addAssign('charakter[Adlig]', 'disabled', 'disabled');
	}
	else
	{
		$objResponse->addAssign('charakter[Adlig]', 'disabled', '');
	}
	try {
		save_charakter($charakter_id, $charakter);
	}
	catch ( Exception $e )
	{
		$objResponse->addAlert(utf8_encode($e->getMessage()));
	}
	$Gunst = bestimmeCharGunst($charakter_id);
	$objResponse->addAssign('Gunstgesamt', 'innerHTML', $Gunst);
	return $objResponse;
}
function sichereGunststufe($charakter_id, $gunststufe)
{
	global $profile;
	$objResponse = new xajaxResponse();
	if (!is_numeric($charakter_id) || ! is_numeric($gunststufe) )
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente: '.$charakter_id.' Gusntstufe: '.$gunststufe));
		return $objResponse;
	}
	// Charakter holen
	$charakter = array();
	$charakter['F_Gunststufe_id'] = $gunststufe;
	try {
		save_charakter($charakter_id, $charakter);
	}
	catch ( Exception $e )
	{
		$objResponse->addAlert(utf8_encode($e->getMessage()));
	}
	$Gunst = bestimmeCharGunst($charakter_id);
	$objResponse->addAssign('Gunstgesamt', 'innerHTML', $Gunst);
	return $objResponse;
}

function sichereName($charakter_id, $name, $id)
{
	global $profile;
	$objResponse = new xajaxResponse();
	if (!is_numeric($charakter_id) || trim($name) == '' || ! is_numeric($id))
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente: '.$charakter_id.' Name: '.$name));
		return $objResponse;
	}
	// Charakter holen
	$charakter = array();
	$charakter['Name'] = $name;
	try {
		save_charakter($charakter_id, $charakter);
		// TODO - Flavour 1 = deutsch 
        add_nodedata($id, 'title', transform(trim($name),'text'), 1);
	}
	catch ( Exception $e )
	{
		$objResponse->addAlert(utf8_encode($e->getMessage()));
	}
	return $objResponse;
}

function zeigeGunst($charakter_id, $adel, $position)
{
	global $profile;
	$objResponse = new xajaxResponse();
	if (!is_numeric($charakter_id) || ! in_array($adel, array('true', 'false')) || ! is_numeric($position)  )
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente.'.$charakter_id));
		return $objResponse;
	}
	// Charakter holen
	$charakter = array();
	if ( $adel == 'true' )
	{
		$charakter['Adlig'] = 1;
	}
	else
	{
		$charakter['Adlig'] = 0;
	}
	
	$charakter['F_Position_id'] = $position;
	try {
		save_charakter($charakter_id, $charakter);
	}
	catch ( Exception $e )
	{
		$objResponse->addAlert(utf8_encode($e->getMessage()));
	}
	$Gunst = bestimmeCharGunst($charakter_id);
	
	$objResponse->addAssign('Gunstgesamt', 'innerHTML', $Gunst);
	return $objResponse;
}
function zeigeFertigkeit($charakter_id, $fertigkeit_id)
{
	$objResponse = new xajaxResponse();
	if (!is_numeric($fertigkeit_id) || ! is_numeric($charakter_id))
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente.'.$fertigkeit_id));
		return $objResponse;
	}
	// Fertigkeit holen
	$charakter = get_charakter($charakter_id);
	$sql = 'SELECT * FROM T_Eigenschaften INNER JOIN T_Gunststufen ON '.$charakter['Klasse'].'_Gunststufe=Gunststufe_id WHERE Eigenschaft_id='.$fertigkeit_id;
	if ( ! $query = sql_query($sql) ) 
	{
		$objResponse->addAlert(utf8_encode('Fehler:'. $sql.'/'.sql_error()));	
	}
	else
	{
		$row = sql_fetch_array($query);
		sql_free_result($query);
		$objResponse->addAssign('Fertigkeitsbeschreibung', 'innerHTML', utf8_encode($row['Effekt']));
		$objResponse->addAssign('Fertigkeitsgunststufe', 'innerHTML', 'Min.Gunststufe: '.utf8_encode($row[$charakter['Klasse']]).
		'<br />Min.Gunstpunkte: '.$row['Minimum_Gunstpunkte']);
	}
	return $objResponse;
}
function zeigeGunststufe($charakter_id, $klasse)
{
	global $profile;
	$objResponse = new xajaxResponse();
	if (!is_numeric($charakter_id) || ! in_array($klasse, array('Krieger', 'Kleriker', 'Magier')) )
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente.'.$klasse.'/'.$charakter_id));
		return $objResponse;
	}
	$charakter = array();
	$charakter['Klasse'] = $klasse;
	try {
		save_charakter($charakter_id, $charakter);
	}
	catch ( Exception $e )
	{
		$objResponse->addAlert(utf8_encode($e->getMessage()));
	}
	// Charakter holen
	$charakter = get_charakter($charakter_id);
	if ( $profile['SteinsbergSL'] == 1 )
	{
		$ergebnis = '<select name="charakter[F_Gunststufe_id]" id="charakter[F_Gunststufe_id]" 
		onChange="xajax_sichereGunststufe('.$charakter_id.',document.getElementById(\'charakter[F_Gunststufe_id]\').value)">';
		$sql = 'SELECT '.$klasse.', Gunststufe_id FROM T_Gunststufen ORDER BY Gunststufe_id';
		$query = sql_query($sql);
		while ( $row = sql_fetch_array($query))
		{
			$ergebnis .= '<option value="'.$row['Gunststufe_id'].'" ';
			if ( $charakter['F_Gunststufe_id'] == $row['Gunststufe_id']) $ergebnis .= 'selected="selected"';
			$ergebnis .= '>'.$row[$klasse].'</option>';
		}
		sql_free_result($query);
		$ergebnis .= '</select>';
	}
	else
	{
		$ergebnis = get_Gunststufe($charakter['F_Gunststufe_id'], $klasse);
	}
	$objResponse->addAssign('Gunststufe', 'innerHTML', utf8_encode($ergebnis));
	
	// Fertigkeiten anpassen
	$query = sql_query('SELECT Hausep FROM T_Haeuser WHERE Haus_id='.$charakter['F_Haus_id']);
	$punkte = sql_fetch_row($query);
	$hausep = $punkte[0];
	sql_free_result($query);			
			 
	if ( $charakter['EP']>0 || substr($charakter['Erstellt'],0,4) == '0000' && $hausep > 0)
	{
		$summe = $charakter['EP'];
		if ( substr($charakter['Erstellt'],0,4) == '0000' ) 
		{
			$summe += $hausep;
		}
		// Fertigkeiten hinzuf�gen
		$sql = 'SELECT Eigenschaftsname, Eigenschaft_id, Erfahrungspunkte, Art FROM T_Eigenschaften WHERE Erfahrungspunkte<='.$summe.' AND '.$charakter['Klasse'].'_Gunststufe>0 AND '.
		$charakter['Klasse'].'_Gunststufe<='.$charakter['F_Gunststufe_id'].
		//' AND Minimum_Gunstpunkte<='.$Gunst.
		//' AND Eigenschaft_id NOT IN (SELECT F_Eigenschaft_id FROM '.
		//'T_Charakter_Eigenschaften WHERE F_Charakter_id='.$charakter['Charakter_id'].') 
		' ORDER BY Eigenschaftsname';
		$query = sql_query($sql);
		if ( sql_num_rows($query) > 0 )
		{
			$ergebnis = '<select name="Fertigkeit" id="Fertigkeit" onChange="xajax_zeigeFertigkeit('.$charakter['Charakter_id'].
			',document.getElementById(\'Fertigkeit\').value)">';
			while ($row = sql_fetch_array($query))
			{
				$ergebnis .= '<option value="'.$row['Eigenschaft_id'].'">'.$row['Eigenschaftsname'].' ('.$row['Art'].'/'.$row['Erfahrungspunkte'].')</option>';
			}
			$ergebnis .= '</select>';	
		}
		else
		{
			$ergebnis = 'Es gibt keine Fertigkeiten die der Charakter erwerben k�nnte.';
		}
		sql_free_result($query);
		$objResponse->addAssign('Fertigkeiten', 'innerHTML', utf8_encode($ergebnis));
		$objResponse->addAssign('Fertigkeitsbeschreibung', 'innerHTML', '');
		$objResponse->addAssign('Fertigkeitsgunststufe', 'innerHTML', '');
		

	}
	return $objResponse;
}
/*
function entferneArtikel($buchung_nr, $artikel_id, $datum, $menge,$artikelnummern)
{
	$objResponse = new xajaxResponse();
	if ( ! is_numeric($buchung_nr) || ! is_numeric($artikel_id) || ! is_numeric($datum) || ! is_numeric($menge))
	{
		$objResponse->addAlert(utf8_encode('Fehler - ung�ltige Argumente.'));
	}
	else
	{
		$content = utf8_encode($Smarty->fetch('Buchungsuebersicht_InaktiveZelle.tpl'));
		$objResponse->addAssign('A'.$artikel_id.'D'.$datum, 'innerHTML', $content);
		$objResponse->addAssign('Schlaf'.$datum, 'innerHTML', Max(0,$Buchung->BerechneAlleSchlafplaetze($datum,$artikelnummern)));
		$objResponse->addAssign('Fehleranzeige', 'innerHTML', '');
	}
	return $objResponse;
}
*/

?>