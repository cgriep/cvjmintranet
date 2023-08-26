<?php

/**
 * Auswahl magisch / unmagisch
 * 
 * 2006 Christoph Griep
 */
session_start();
include ('atlantis.db.php');
include ('character.class.php');

if (session_is_registered('Charakter') && session_is_registered('Magisch') && $_SESSION['Magisch'])
{
	$Charakter = unserialize($_SESSION['Charakter']);
	if (isset ($_REQUEST['Klasse']) && is_numeric($_REQUEST['Klasse']))
	{
		// evtl. vorhandene Spruchlisten l�schen
		$Charakter->Spruchlisten = array ();
		$Charakter->F_Klasse_id = $_REQUEST['Klasse'];
		$_SESSION['Charakter'] = serialize($Charakter);		
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1b.php');
	} else
	{
		// Neues Template anzeigen	
		require 'Smarty/libs/Smarty.class.php';
		$smarty = new Smarty;

		if ($query = sql_query('SELECT * FROM T_Spezialisierungsklassen ' .
			'WHERE Magisch AND Auswaehlbar ORDER BY Klasse'))
		{
			$klassen_values = array ();
			$klassen_output = array ();
			while ($row = sql_fetch_array($query)) // Zeile holen
			{
				$klassen_output[] = $row['Klasse'];
				$klassen_values[] = $row['Klasse_id'];
			}
		} else
			die('Fehler bei DB:' .	sql_error());
		sql_free_result($query);
		$smarty->assign('Character', $Charakter->alsFeld(false));
		$smarty->assign('PageTitle', 'Auswahl der magischen Klasse');
		$smarty->assign('mitAjax', true);
		$smarty->assign('Klassen_values', $klassen_values);
		$smarty->assign('Klassen_output', $klassen_output);
		$smarty->assign('Klassen_selected', $Charakter->F_Klasse_id);		
		$smarty->display('Atlantis-Step1a.tpl');
	}
} else
{
	// Fehler: Dieses Skript ist nur f�r magische Charaktere gedacht     
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step2.php');
}
?>
