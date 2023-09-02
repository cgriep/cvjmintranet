<?php

/**
 * 2006
 * 
 * Christoph Griep
 */
session_start();
if (!session_is_registered('Charakter_id'))
{
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Auswahl.php');
} else
{
	include ('atlantis.db.php');
	include ('character.class.php');
	$Meldung = '';
	$Fehler = '';
	$char = new Charakter();
	$char->loadFromDatabase($_SESSION['Charakter_id']);

	if (!$char->isUebernatuerlich() || $char->getAktivierungspunkte() <= 0)
	{
		// Keine Vorteile zur Auswahl vorhanden
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Name.php');
	}
	if (isset ($_REQUEST['Fertigkeit']) && is_numeric($_REQUEST['Fertigkeit']))
	{
		if (isset ($char->Fertigkeiten[$_REQUEST['Fertigkeit']]))
		{
			$char->Fertigkeiten[$_REQUEST['Fertigkeit']]['Stufe']++;
			$char->saveInDatabase();
		} else
			$Fehler = 'Diesen Vorteil gibt es nicht!';
	}
	elseif (isset ($_REQUEST['Entfernen']) && is_numeric($_REQUEST['Entfernen']))
	{
		if (isset ($char->Fertigkeiten[$_REQUEST['Entfernen']]) && $char->Fertigkeiten[$_REQUEST['Entfernen']]['Stufe'] > 0)
		{
			$char->Fertigkeiten[$_REQUEST['Entfernen']]['Stufe']--;
			$char->saveInDatabase();
		} else
			$Fehler = 'Diesen Vorteil gibt es nicht!';
	}
	// nun sind alle Daten aktuell

	require 'Smarty/libs/Smarty.class.php';
	$smarty = new Smarty;
	$smarty->assign('Character', $char->alsFeld(true));
	$smarty->assign('PageTitle', 'Aktivierung der �bernat�rlichen Vorteile');
	$smarty->assign('mitAjax', true);
	// Zugriffe auf die Datenbank

	$Punkte = $char->getAktivierungspunkte();
	$fertigkeit_values = array ();
	$fertigkeit_output = array ();
	$vfertigkeit_values = array ();
	$vfertigkeit_output = array ();

	foreach ($char->Fertigkeiten as $key => $f)
	{
		if ($f['Art'] == 'Uv')
		{
			if ($f['Stufe'] == 0)
			{
				if ($f['Kosten1'] >= 0 && $f['Kosten1'] <= $Punkte)
				{
					$fertigkeit_values[] = $key;
					$fertigkeit_output[] = $f['Fertigkeit'] . ' (' . $f['Kosten1'] . ')';
				}
			} else
			{
				$vfertigkeit_values[] = $key;
				$vfertigkeit_output[] = $f['Fertigkeit'];
			}
		}
	}
	$smarty->assignbyref('Charakter', $char);
	$smarty->assign('fertigkeit_values', $fertigkeit_values);
	$smarty->assign('fertigkeit_output', $fertigkeit_output);
	$smarty->assign('fertigkeit_selected', '');
	$smarty->assign('vfertigkeit_values', $vfertigkeit_values);
	$smarty->assign('vfertigkeit_output', $vfertigkeit_output);
	$smarty->assign('vfertigkeit_selected', '');

	//$smarty->assign('PunkteHinweis', $Punkte);
	$smarty->assign('FehlerHinweis', $Fehler);
	$smarty->display('Atlantis-Step4.tpl');
}
?>
