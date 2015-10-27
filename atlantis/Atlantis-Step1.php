<?php

/*
 * Created on 15.11.2006
 *
 * Christoph Griep
 */
// Alte Daten löschen
session_start();

if (!session_is_registered('Charakter'))
{
	// Fehler - kein Charakter
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step0.php');
} else
{
	include ('atlantis.db.php');
	include ('character.class.php');

	$Charakter = unserialize($_SESSION['Charakter']);
	// Daten speichern
	if (isset ($_REQUEST['Magisch']) && is_numeric($_REQUEST['Magisch']))
	{
		$Charakter->Spruchlisten = array ();
		$_SESSION['Charakter'] = serialize($Charakter);
		if ($_REQUEST['Magisch'] == 1)
		{
			$_SESSION['Magisch'] = true;
			header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1a.php');
		} else
		{
			$_SESSION['Magisch'] = false;
			header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step2.php');
		}
	} else
	{
		require 'Smarty/libs/Smarty.class.php';		
		$smarty = new Smarty;

		$smarty->assign('Character', $Charakter->alsFeld(false));
		$smarty->assign('PageTitle', 'Magischer oder weltlicher Charakter?');
		$smarty->assign('Fertigkeit_values', array (
			0,
			1
		));
		$smarty->assign('Fertigkeit_values', array (
			0,
			1
		));
		$smarty->assign('Fertigkeit_output', array (
			'Weltlich',
			'Magisch'
		));
		if (!session_is_registered('Magisch'))
		{
			$_SESSION['Magisch'] = false;
		}
		$smarty->assign('Fertigkeit_selected', $_SESSION['Magisch']);
		$smarty->assign('mitAjax', false);
		$smarty->display('Atlantis-Step1.tpl');
	}
}
?>
