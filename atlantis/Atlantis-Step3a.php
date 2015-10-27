<?php
/**
 * 2006
 * Christoph Griep
 */
session_start();

if ( ! session_is_registered('Charakter_id'))
{
	header('Location: http://' . $_SERVER['HTTP_HOST'] .dirname($_SERVER['PHP_SELF']). '/Atlantis-Auswahl.php');
}
elseif ( isset ($_REQUEST['Klasse']) && is_numeric($_REQUEST['Klasse']))
{
	$_SESSION['AnzeigeFertigkeit'] = $_REQUEST['Klasse'];
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']).'/Atlantis-Step3.php');
} else
{
	include ('atlantis.db.php');	
	include ('character.class.php');
	$char = new Charakter();
	$char->loadFromDatabase($_SESSION['Charakter_id']);	
	
	require 'Smarty/libs/Smarty.class.php';
	$smarty = new Smarty;
	$smarty->assign('Character', $char->alsFeld(true));
	$smarty->assign('PageTitle', 'Auswahl der Fertigkeitenklasse');
	$smarty->assign('mitAjax', true);
	$smarty->assign('Spruchlisten', '');
	// Zugriffe auf die Datenbank
	$Meldung = '';
	
	$s = $char->bestimmeMoeglicheSpezialisierungsklassen();
	
	$fertigkeit_values = array_keys($s);
	$fertigkeit_output = array_values($s);
	
	$smarty->assign('fertigkeit_values', $fertigkeit_values);
	$smarty->assign('fertigkeit_output', $fertigkeit_output);
	$smarty->assign('fertigkeit_selected', $fertigkeit_values[0]);

	$smarty->display('Atlantis-Step3a.tpl');
}
?>
