<?php
/*
 * Created on 15.11.2006
 *
 * Christoph Griep
 */

session_start();
// Alte Daten löschen
session_destroy(); 

require 'Smarty/libs/Smarty.class.php';

$smarty = new Smarty;

$smarty->assign('PageTitle', 'Atlantissystem');
$smarty->assign('mitAjax', false);

$smarty->display('Atlantis-Intro.tpl');
 
?>
