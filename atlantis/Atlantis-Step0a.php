<?php
session_start();
include ('atlantis.db.php');
include ('character.class.php');

if (!session_is_registered('Charakter')) // Name 
{
	// Zurück zur Namenseingabe
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step0.php');
} else
{
	$Charakter = unserialize($_SESSION['Charakter']);
	if (isset ($_REQUEST['Rasse']) && is_numeric($_REQUEST['Rasse']))
	{
		$Charakter->setRasse($_REQUEST['Rasse']);
		$_SESSION['Charakter'] = serialize($Charakter);
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step1.php');
	} else
	{
		require 'Smarty/libs/Smarty.class.php';
		$smarty = new Smarty;

		// Zugriffe auf die Datenbank
		if ($query = mysql_query('SELECT * FROM T_Rassen ORDER BY Rasse')) // Abfrage senden
		{
			$rassen_values = array ();
			$rassen_output = array ();
			$rassen_selected = '';
			while ($row = mysql_fetch_array($query)) // Zeile holen
			{
				$rassen_output[] = $row['Rasse'];
				$rassen_values[] = $row['Rasse_id'];
			}
		}
		mysql_free_result($query);
		$smarty->assign('Character', $Charakter->alsFeld(false));
		$smarty->assign('PageTitle', 'Rassenauswahl');
		$smarty->assign('mitAjax', true);
		$smarty->assign('rassen_values', $rassen_values);
		$smarty->assign('rassen_output', $rassen_output);
		$smarty->assign('rassen_selected', $Charakter->F_Rasse_id);
		$smarty->display('Atlantis-Step0a.tpl');
	}
} // Name ist vorhanden
?>
