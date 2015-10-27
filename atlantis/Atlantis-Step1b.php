<?php


/**
 * Auswahl Totem, Element etc. 
 * 2006 Christoph Griep
 */
session_start();
include ('atlantis.db.php');
include ('character.class.php');

if (!session_is_registered('Charakter') || !session_is_registered('Magisch') || !$_SESSION['Magisch'])
{	
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1.php');
} else
{
	$Charakter = unserialize($_SESSION['Charakter']);
	if ($Charakter->F_Klasse_id == NULL)
	{
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1a.php');
	} else
	{
		// Totem, Element etc. auswaehlen soweit notwendig
		$Art = '';
		if (isset ($_REQUEST['Wahl']) && is_numeric($_REQUEST['Wahl']))
		{
			$Charakter->F_MagischeBesonderheit_id = $_REQUEST['Wahl'];
			$_SESSION['Charakter'] = serialize($Charakter);
			header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1c.php');
		} else
		{
			$mb = $Charakter->getMagischeBesonderheiten();
			// Nur wenn Auswahlmöglichkeiten vorhanden 
			if (Count($mb) > 0)
			{
				require 'Smarty/libs/Smarty.class.php';
				$smarty = new Smarty;
				$smarty->assign('Character', $Charakter->alsFeld(false));
				$smarty->assign('PageTitle', 'Auswahl der magischen Ausrichtung');
				$smarty->assign('mitAjax', false);
				$smarty->assign('Fertigkeit_values', array_keys($mb));
				$smarty->assign('Fertigkeit_output', array_values($mb));
				$smarty->assign('Fertigkeit_selected', $Charakter->F_MagischeBesonderheit_id);
				$smarty->display('Atlantis-Step1b.tpl');
			} else
			{
				// Keine Auswahl nötig. Weiter zur nächsten Seite
				if (strpos($_SERVER['HTTP_REFERER'], 'Step1c') === false)
				{
					header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1c.php');
				} else
				{

					// zurück zur Wahl
					header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1a.php');
				}
			}
		}
	}
}
?>
