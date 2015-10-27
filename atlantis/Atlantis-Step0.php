<?php
/*
 * Namensauswahl für einen neuen Charakter
 * Created on 15.11.2006
 * Christoph Griep
 */
session_start();
include ('atlantis.db.php');
include ('character.class.php');

if (isset ($_REQUEST['Neu']))
{
	// Alte Daten löschen
	session_destroy();
}
if ( session_is_registered('Charakter'))
{
	$Charakter = unserialize($_SESSION['Charakter']);
}
else
{
	$Charakter = new Charakter();
}
if (isset ($_REQUEST['Name']) && trim($_REQUEST['Name'])!='')
{ 
    
	$Charakter->Charaktername = $_REQUEST['Name'];
	$_SESSION['Charakter'] = serialize($Charakter);
	//$_SESSION['Name'] = $_REQUEST['Name'];
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']).'/Atlantis-Step0a.php');
} else
{
	require 'Smarty/libs/Smarty.class.php';
	
	$smarty = new Smarty;

    if ( isset($_REQUEST['Name']))
	{
		$smarty->assign('Fehler', 'Sie müssen einen Namen angeben!'); 
	}
	
	$smarty->assign('PageTitle', 'Namensauswahl für neuen Charakter');
	$smarty->assign('mitAjax', false);
	$smarty->assign('Name', $Charakter->Charaktername);
	$smarty->display('Atlantis-Step0.tpl');
} // kein Name übergeben
?>
