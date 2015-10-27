<?php
/**
  * Spruchlistenauswahl
  * 2006 Christoph Griep
  */
session_start();
include ('atlantis.db.php');
include('character.class.php');

if (!session_is_registered('Charakter') || !$_SESSION['Magisch'])
{
	header('Location: http://' . $_SERVER['HTTP_HOST'] .dirname($_SERVER['PHP_SELF']). '/Atlantis-Step0.php');
}
$Charakter = unserialize($_SESSION['Charakter']);
if ( ! $Charakter->isMagisch() || isset($_REQUEST['Weiter']))
{
	header('Location: http://' . $_SERVER['HTTP_HOST'] .dirname($_SERVER['PHP_SELF']). '/Atlantis-Step2.php');
}
// Spruchlisten auswaehlen

if (isset ($_REQUEST['Spruchliste']) && is_numeric($_REQUEST['Spruchliste']))
{
	if (Count($Charakter->Spruchlisten) >= $Charakter->getKlasseSpruchlistenAnzahl() )
	{
		// Fehler: Keine freien Spruchlisten mehr
	} else
	{
		$query = mysql_query('SELECT * FROM T_Spezialisierungen ' .
		'WHERE Spezialisierung_id = ' . $_REQUEST['Spruchliste']);
		$sl = mysql_fetch_array($query);
		mysql_free_result($query);
		$Charakter->Spruchlisten[$_REQUEST['Spruchliste']] = $sl; 			
		$_SESSION['Charakter'] = serialize($Charakter);
	}
	header('Location: ' . $_SERVER['PHP_SELF']);
}
elseif (isset ($_REQUEST['Entfernen']) && is_numeric($_REQUEST['Entfernen']))
{
	unset($Charakter->Spruchlisten[$_REQUEST['Entfernen']]);
	$_SESSION['Charakter'] = serialize($Charakter);
	header('Location: ' . $_SERVER['PHP_SELF']);
} else
{
	if (!$query = mysql_query('SELECT * FROM T_Spezialisierungen WHERE Spruchliste AND F_Klasse_id=' .
		$Charakter->F_Klasse_id . ' ORDER BY Spezialisierung'))
		echo '<div class="Fehler">' . mysql_error() . '</div>';
    $verfuegbar = $Charakter->getKlasseSpruchlistenAnzahl()-Count($Charakter->Spruchlisten);
	if ( $verfuegbar > 0 && $verfuegbar < mysql_num_rows($query))
	{
		require 'Smarty/libs/Smarty.class.php';
		$smarty = new Smarty();
		$spruchliste_values = array ();
		$spruchliste_output = array ();
		$spruchliste_selected = '';
		$vspruchliste_values = array ();
		$vspruchliste_output = array ();
		$vspruchliste_selected = '';
		while ($row = mysql_fetch_array($query)) // Zeile holen
		{
			if (in_array($row['Spezialisierung_id'], array_keys($Charakter->Spruchlisten)))
			{
				$vspruchliste_output[] = $row['Spezialisierung'];
				$vspruchliste_values[] = $row['Spezialisierung_id'];
			} else
			{
				$spruchliste_output[] = $row['Spezialisierung'];
				$spruchliste_values[] = $row['Spezialisierung_id'];
			}
		}
		$smarty->assign('Character', $Charakter->alsFeld(false));
		$smarty->assign('PageTitle', 'Auswahl der Spruchlisten');
		$smarty->assign('mitAjax', true);
		$smarty->assign('Spruchliste_values', $spruchliste_values);
		$smarty->assign('Spruchliste_output', $spruchliste_output);
		$smarty->assign('Spruchliste_selected', '');
		$smarty->assign('vSpruchliste_values', $vspruchliste_values);
		$smarty->assign('vSpruchliste_output', $vspruchliste_output);
		$smarty->assign('vSpruchliste_selected', '');
		$smarty->assign('Spruchlistenanzahl', $verfuegbar);
		$smarty->display('Atlantis-Step1c.tpl');
	} else
	{
		if ($verfuegbar>=mysql_num_rows($query))
		{
			// alle Spruchlisten auswaehlen
			while ($liste = mysql_fetch_array($query))
			{
				$Charakter->Spruchlisten[$liste['Spezialisierung_id']] = $liste;
			}
			$_SESSION['Charakter'] = serialize($Charakter);
		}
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']).'/Atlantis-Step2.php');
	}
	mysql_free_result($query);
}
?>
