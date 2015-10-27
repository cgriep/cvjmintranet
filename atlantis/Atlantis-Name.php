<?php
/*
 * Sichert die Charakterdaten aus der Session in der Datenbank
 * Zeigt alle Charaktere an, die in der Datenbank vorhanden sind 
 * 
 * Created on 15.11.2006
 * Christoph Griep
 */
session_start();
session_unregister('AnzeigeFertigkeit');
require 'Smarty/libs/Smarty.class.php';
include ('atlantis.db.php');
include ('character.class.php');
$Meldung = '';
if ( session_is_registered('Meldung'))
{
	$Meldung = $_SESSION['Meldung'];
	session_unregister('Meldung');
}
if (isset ($_REQUEST['Charakter_id']) && is_numeric($_REQUEST['Charakter_id']))
{
	$_SESSION['Charakter_id'] = $_REQUEST['Charakter_id'];
}
if (!session_is_registered('Charakter_id'))
{
	// Ein neu ersteller Charakter sollte in der Session gespeichert sein 
	if (session_is_registered('Charakter'))
	{
		// Charakter schon vorhanden
		$Charakter = unserialize($_SESSION['Charakter']);
		$Charakter->saveInDatabase();
		// Laden, damit alle Daten korrekt zugeordnet werden 
		$Charakter->loadFromDatabase($Charakter->Charakter_id);
		session_unset();
		$_SESSION['Charakter_id'] = $Charakter->Charakter_id;
		$Meldung = 'Charakter unter Nummer ' . $Charakter->Charakter_id . ' gespeichert.';
	} else
	{
		// Auswahl des Charakters !
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']).'/Atlantis-Auswahl.php');
	}
} else
{
	// id schon vorhanden - Charakter laden 
	$Charakter = new Charakter();
	if (!$Charakter->loadFromDatabase($_SESSION['Charakter_id']))
	{
		// Fehler
		session_unregister('Charakter_id');
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']).'/Atlantis-Auswahl.php');
	}
	// Bearbeitungsmodus für SL
	if ( isset($_REQUEST['Godmode']))
	{
		session_unregister('Charakter_id');
		$_SESSION['Charakter'] = serialize($Charakter);
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']).'/Atlantis-Step2.php');
	}
}
// Charakter in Datenbank speichern!
if (isset ($_REQUEST['DelBild']))
{
	$Charakter->Bild = '';
	$Charakter->saveInDatabase();
	$Meldung = 'Das Charakterbild wurde entfernt.';
}
if (isset ($_REQUEST['Name']))
{
	$Charakter->Charaktername = $_REQUEST['Name'];
	if (isset ($_REQUEST['Contage']) && is_numeric($_REQUEST['Contage']))
	{
		if (isset ($_REQUEST['Con']) && $_REQUEST['Con'] != '')
		{
			$Con = $_REQUEST['Con'];
			if (isset ($_REQUEST['Atlantis']))
				$Charakter->ContageErgaenzen($_REQUEST['Contage'], $Con, true);
			else
				$Charakter->ContageErgaenzen($_REQUEST['Contage'], $Con);
		}
	}
	$bild = file_get_contents($_FILES['Bild']['tmp_name']);
	if ($bild != '')
	{
		// Bild in jpeg konvertieren
		$dasBild = imagecreatefromstring($bild);
		ob_start();
		ImageJPEG($dasBild);
		$bild = ob_get_contents();
		ob_end_clean();
		$Charakter->Bild = $bild;
	}
	$Charakter->saveInDatabase();
	$_SESSION['Meldung'] = 'Die Charakterdaten wurden aktualisiert.';
	header('Location: http://' . $_SERVER['HTTP_HOST'].'/'.$_SERVER['PHP_SELF']);
} else
{

	$smarty = new Smarty;

	$smarty->assign('PageTitle', 'Charakterdaten');
	$smarty->assign('mitAjax', true);
	$smarty->assign_by_ref('Charakter', $Charakter);
	$smarty->assign('Character', $Charakter->alsFeld(true));
	$smarty->assign('Bild', '');
	if ($Charakter->Bild != '')
	{
		$smarty->assign('Bild', 'zeigeBild.php?Bild=1');
	}
	$smarty->assign('Meldung', $Meldung);
	$smarty->display('Atlantis-Name.tpl');
}
?>
