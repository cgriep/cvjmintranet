<?php


require_once ('inc/xajax/xajax.inc.php');

/* Datenbank- und AWF-Funktionalität einbinden */
include ('inc/functions.inc');
include ('inc/licence.key');
include ('inc/sessions.inc');
include ('inc/caching.inc');
include ('inc/config.inc');

/* prüfen ob eine Session für den Benutzer existiert */
if (!isset ($session_userid) || $session_userid == '')
{
	die('Unauthorisiert!');
}
// Benutzer-ID heraussuchen
define('SESSION_DBID', get_user_id($session_userid));
define('SESSION_STATUS','ok');

$xajax = new xajax('steinsberg_server.php');
$xajax->registerFunction('zeigeTeilnahmeArt');
$xajax->processRequests();

function zeigeTeilnahmeArt($node_id, $art)
{
	// $profile = get_profile();
	$docinfo = get_node($node_id, 1, false); // 1 - flavour , ggf. anpassen
	$objResponse = new xajaxResponse();
	$options = '';
	$test = "";
	foreach (  explode("\n",$docinfo['Bereiche']) as $Bereich)
	{
		// Nur Optionen anzeigen die für die entsprechende Teilnahmeart verfügbar sind.
		$option =  explode(';',$Bereich);
		if ( !isset($option[1]) || trim($option[1]) == $art || ($art != 'SC' && $art != 'NSC')) 
		{
			$options .= '<option>'.$option[0].'</option>';
		}
	}
	$objResponse->addAssign('Teilnahmeart', 'innerHTML', utf8_encode($options));
	return $objResponse;
}
?>