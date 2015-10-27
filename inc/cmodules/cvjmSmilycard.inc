<?php

include (INC_PATH . 'cvjm/Buchung.class.php');
include (INC_PATH . 'misc/CVJM.inc');

require (INC_PATH.'Smarty/Smarty.class.php');


if ( is_numeric($_REQUEST['Buchung_Nr']))
{
	$Buchung = new Buchung($_REQUEST['Buchung_Nr']);
	if ( isset($_REQUEST['Smilycard']) && is_array($_REQUEST['Smilycard']))
	{
		// Speichern
		$Buchung->saveSmilycard($_REQUEST['Smilycard']);
	}
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('Buchung', $Buchung);
	$smilycard = $Buchung->getSmilycard();
	$Smarty->assign('Werte', array(1,2,3,4));
	$Smarty->assign('Unterbringung', array('JB'=>'Jugendbereich', 'FB'=>'Familienbereich', 'ZB'=>'Zeltplatz'));
	$Smarty->assign('Programm', array(
		'N'=>'Naturforscher', 
		'H'=>'Häuptling Bunte Feder', 
		'E'=>'Das Spectaculum',
		'A'=>'Andere <input type="text" name="Smilycard[ProgrammAndere]" value="'.$smilycard['ProgrammAndere'].'" />'));
	$Smarty->assign('Herkunft', array(
		'P'=>'Prospektmaterial', 
		'C'=>'CVJM Publikation: <input type="text" name="Smilycard[HerkunftPublikation]" value="'.$smilycard['HerkunftPublikation'].'" />', 
		'E'=>'evangelische Familienerholung',
		'F'=>'Freunde/Bekannte/Kollegen',
		'I'=>'Internet',
		'K'=>'Wir waren schon einmal hier',
		'S'=>'Sonstige: <input type="text" name="Smilycard[HerkunftSonstige]" value="'.$smilycard['HerkunftSonstige'].'" />'));
	$Smarty->assign('id', $id);
	$Smarty->assign('Adressebearbeiten', get_url(get_cmodule_id('cvjmadressen')));
	$Smarty->display('Smilycard.tpl');
}
elseif ( $profile['editor'] != '1')
{
	throw new Exception('Keine Buchungsnummer angegeben!');
}

?>
