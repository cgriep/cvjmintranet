<?php
/*
 * Created on 25.01.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
 if ( $profile['editor'] != 1)
 {
 	require_once (INC_PATH.'Smarty/Smarty.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	$Smarty->assign('id', $id);
 	$Smarty->assign('title', transform($document['title'],$document['datatype']['title'])); 	
 	$query = sql_query('SELECT * FROM cvjm_Belegungsstatistik ORDER BY Bereich, Jahr, Datum');
 	$Werte =array();
 	$dieJahre = array();
 	$Maximal = array();
 	$Vergleich = array();
 	$JahrGesamt = array();
 	$maxDat = 0;
 	while ( $row = sql_fetch_array($query))
 	{
 		 $Werte[$row['Bereich']][$row['Datum']][$row['Jahr']]['Menge'] = $row['Menge'];
 		 $Werte[$row['Bereich']][$row['Datum']][$row['Jahr']]['Buchungen'] = $row['Buchungen'];
 		 $Werte[$row['Bereich']][$row['Datum']][$row['Jahr']]['Selbst'] = $row['SV'];
 		 if ( ! in_array($row['Jahr'], $dieJahre))
 		 {
 		   $dieJahre[] = $row['Jahr'];
 		 }
 		 $JahrGesamt[$row['Bereich']][$row['Jahr']] = $row['Menge'];
 		 $Vergleich[$row['Bereich']] = array(); 	
 		 $maxDat = $row['Datum'];	 
 	}
 	sql_free_result($query);
 	
 	// Vergleich - gibt den Werte jeden Jahres zum Vergleich mit dem aktuellen Wert des aktuellen Jahres
 	foreach ($Werte as $Bereich => $wert)
	{
		foreach ( $wert as $Dat => $Inhalt)
		{
			foreach ( $Inhalt as $Jahr => $was)
			{
				$datum = mktime(23,59,59,date('m',$maxDat),date('d',$maxDat),$Jahr); // Vergleichsdatum 
				if ( $Dat <= $datum )
				{ 
					$Vergleich[$Bereich][$Jahr] = $was['Menge'];				
				}				
			}
		}
	}
 	$Smarty->assign('Vergleichsdatum', $maxDat);
 	$Smarty->assign('JahrGesamt', $JahrGesamt);
 	$Smarty->assign('Werte', $Werte);
 	$Smarty->assign('dieJahre', $dieJahre);
 	
 	$Smarty->assign('Vergleich', $Vergleich);
 	$Smarty->display('Belegungshistorie.tpl');
 }
 
?>
