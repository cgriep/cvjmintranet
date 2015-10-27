<?php
/*
 * Created on 15.11.2006
 *
 * Christoph Griep
 */
session_start();
require 'Smarty/libs/Smarty.class.php';
include('atlantis.db.php');

if ( isset($_REQUEST['Del']) && is_numeric($_REQUEST['Del']))
{
	mysql_query('DELETE FROM T_CharakterFertigkeiten WHERE F_Charakter_id='.$_REQUEST['Del']);
	mysql_query('DELETE FROM T_CharakterSpruchlisten WHERE F_Charakter_id='.$_REQUEST['Del']);
	mysql_query('DELETE FROM T_Charaktere WHERE Charakter_id='.$_REQUEST['Del']);
}

$Charaktere = array();
if (! $query= mysql_query('SELECT Charakter_id, Charaktername FROM T_Charaktere ' .
		'ORDER BY Charaktername')) echo mysql_error();
while ( $char = mysql_fetch_array($query) )
{
	$Charaktere[$char['Charakter_id']] = $char['Charaktername'];	
}
mysql_free_result($query);

$smarty = new Smarty;
$smarty->assign('Charaktere',$Charaktere);
$smarty->assign('PageTitle', 'Charakter auswählen');
$smarty->assign('mitAjax', true);
$smarty->display('Atlantis-Auswahl.tpl');

?>
