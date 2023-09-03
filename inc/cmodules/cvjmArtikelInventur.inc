<?php

/**
 *
 * cvjmArtikelInventur
 *
 * AWF-Name: cvjmartikelinventur
 *
 */

if ( $profile['editor']!= 1 )
{
	include(INC_PATH.'misc/CVJM.inc');
	include(INC_PATH.'cvjm/Artikel.class.php');	
	include_once (INC_PATH . 'cvjm_ajax.php');
	$xajax->printJavascript('./javascript');
	
	// Speichern der Einträge
	
	
	require_once(INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('title', transform($document['title'], $document['datatype']['title']));
	$Smarty->assign('id', $id);
	// Alle Artikel auflisten
	//$Smarty->assign()
	// Alle Orte auflisten
	$Smarty->assignbyref('Orte',Artikel::getOrte());
	if ( isset($docinput['Ort']) && $docinput['Ort'] != '')
	{
		// Ort heraussuchen
		$Artikel = new Artikel($docinput['Ort']);
		$Smarty->assignbyref('Ort', $Artikel);
		$Liste = array();
		$query = sql_query('SELECT * FROM '.'cvjm_Artikelorte'.' WHERE F_Ort_id='.$Artikel->Artikel_id);
		while ( $menge = sql_fetch_array($query))
		{
			$a = new Artikel($menge['F_Artikel_id']);
			// Sonderfall: Menge hinzufügen
			$a->Menge = $menge['Menge'];
			$Liste[] = $a;
			
		}
		sql_free_result($query);		
		$Smarty->assignbyref('Artikelliste', $Liste);
	}	
	$Smarty->display('Artikel_Inventur.tpl');
} // wenn nicht Editor
?>
