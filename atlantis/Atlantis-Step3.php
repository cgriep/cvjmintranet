<?php
/**
 * 2006 Christoph Griep
 */
session_start();
if (!session_is_registered('Charakter_id'))
{
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']).'/Atlantis-Auswahl.php');
}
elseif (!session_is_registered('AnzeigeFertigkeit'))
{
	// Fehler - zurück zum Charakter
	header('Location: http://' . $_SERVER['HTTP_HOST'] .dirname($_SERVER['PHP_SELF']). '/Atlantis-Step3a.php');
} else
{
	include ('atlantis.db.php');
	include ('character.class.php');
	$Meldung = '';
	$Fehler = '';
	$char = new Charakter();
	$char->loadFromDatabase($_SESSION['Charakter_id']);
	// TODO: Schummelschalter entfernen
	if (isset ($_REQUEST['MehrPunkte']))
	{
		$char->Punkte += 50;
		$char->saveInDatabase();
	}	
	if (isset ($_REQUEST['Fertigkeit']) && is_numeric($_REQUEST['Fertigkeit']))
	{
			$Fehler = $char->fertigkeitHinzu($_REQUEST['Fertigkeit'], $_SESSION['AnzeigeFertigkeit']);
			$char->saveInDatabase();
	}
	elseif (isset ($_REQUEST['Entfernen']) && is_numeric($_REQUEST['Entfernen']))
	{
		$Fehler = $char->fertigkeitEntfernen($_REQUEST['Entfernen'], $_SESSION['AnzeigeFertigkeit']);
		$char->saveInDatabase();		
	}
	// nun sind alle Daten aktuell

	require 'Smarty/libs/Smarty.class.php';
	$smarty = new Smarty;
	$smarty->assign('Character', $char->alsFeld(true));
	$smarty->assign('PageTitle', 'Auswahl der Fertigkeiten');
	$smarty->assign('mitAjax', true);
	// Zugriffe auf die Datenbank
	
	$fertigkeiten = $char->bestimmeMoeglicheFertigkeiten($_SESSION['AnzeigeFertigkeit']);
	// Name der Fertigkeitsklasse bestimmen
	if ( isset($fertigkeiten[0]))
	  $Spezialisierungsname = $fertigkeiten[0]['Spezialisierung'];
	 else
	 $Spezialisierungsname = '';
	$fertigkeit_values = array_keys($fertigkeiten);
	$fertigkeit_output = array();
	foreach  ($fertigkeiten as $key => $f)
	{
		$fertigkeit_output[] = $f['Fertigkeit'] . ' (' . $f['Rang'] . ', ' . 
		($char->holeFertigkeitPunkte($f['Fertigkeit_id'], $f['Spezialisierung_id'])) . ')';
	}

	$fert = $char->bestimmeVorhandeneFertigkeiten($_SESSION['AnzeigeFertigkeit']);
	$vfertigkeit_values = array_keys($fert);
	$vfertigkeit_output = array();
	foreach  ($fert as $key => $f)
	{
		$vfertigkeit_output[] = $f['Fertigkeit'] . ' (' . $f['Rang'] . ', ' . 
		($char->holeFertigkeitPunkte($f['Fertigkeit_id'], $f['Spezialisierung_id'])) . ')';
	}
	
	$smarty->assign('Spezialisierung', $Spezialisierungsname);
	$smarty->assign_by_ref('Charakter', $char);
	$smarty->assign('fertigkeit_values', $fertigkeit_values);
	$smarty->assign('fertigkeit_output', $fertigkeit_output);
	$smarty->assign('fertigkeit_selected', '');
	$smarty->assign('vfertigkeit_values', $vfertigkeit_values);
	$smarty->assign('vfertigkeit_output', $vfertigkeit_output);
	$smarty->assign('vfertigkeit_selected', '');

	//$smarty->assign('PunkteHinweis', $Punkte);
	$smarty->assign('FehlerHinweis', $Fehler);
	$smarty->display('Atlantis-Step3.tpl');
}
?>
