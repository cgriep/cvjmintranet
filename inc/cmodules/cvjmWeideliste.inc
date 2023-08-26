<?php
/**
 * cvjmWeidelisten
 * Weideliste für CVJM Feriendorf Herbstein
 * 
 */

require (INC_PATH.'Smarty/Smarty.class.php');
$Smarty = new Smarty;
$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';

if ( isset($_REQUEST['Weideliste']) )
{
	// Speichern
	$sql = 'INSERT INTO cvjm_Weideliste (Datum, Ort, Tierart, Bemerkung, Erstellt) VALUES (';
	$sql .= '\''.date('Y-m-d',strtotime($_REQUEST['Weideliste']['Datum'])).'\',\''.
	sql_real_escape_string($_REQUEST['Weideliste']['Ort']).'\',\''.
	sql_real_escape_string($_REQUEST['Weideliste']['Tierart']).'\',\''.
	sql_real_escape_string($_REQUEST['Weideliste']['Bemerkung']).'\',NOW())';
	if ( ! sql_query($sql))
	{
		print sql_error();
	}
}

	$startdatum = strtotime('-30 days', strtotime(date('Y-m-d')));
	if ( isset($_REQUEST['ShowDatum']))
	{
		$startdatum = strtotime($_REQUEST['ShowDatum']);
	}
	$sql = 'SELECT * FROM cvjm_Weideliste WHERE Datum BETWEEN \''.date('Y-m-d',$startdatum).
	'\' AND \''.
	date('Y-m-d',strtotime('+30 day', $startdatum)).'\' ORDER BY Tierart, Datum';
	$query = sql_query($sql);
	$weideliste = array();
	$row = array('Datum'=>'', 'Tierart'=>'', 'Ort'=>'', 'Bemerkung'=>'', 'Erstellt'=>'');
	for ($i=0; $i<30;$i++)
	{
		$weideliste[strtotime('+'.$i.' days', $startdatum)] = $row;
	}
	while ( $row = sql_fetch_array($query))
	{
		if ( $weideliste[strtotime($row['Datum'])]['Tierart'] == '' )
		{
			$weideliste[strtotime($row['Datum'])] = $row;
		}
		else
		{
			// schon was da 
			foreach ( $row as $k=>$z)
			{
				if ( $k != 'Datum') 
				{
					$weideliste[strtotime($row['Datum'])][$k] .= "\n".$z;
				}
			}
		}
	}
	sql_free_result($query);
	// template anzeigen
	$Smarty->assign('Weideliste', $weideliste);
	$Smarty->assign('id', $id);
	$Smarty->assign('Datum', date('d.m.Y'));
	$Smarty->assign('AbDatum', date('d.m.Y', $startdatum));
	
	$Smarty->assign('Tiere', array('Schafe', 'Ziegen', 'Ponys'));
	$Smarty->assign('Orte', array(
		'Röderwiesen (Zeltplatz)',
		'Struth JB (Hauptwiese)',
		'Struth JB (oberhalb BH)',
		'Struth FB (Hauptwiese)',
		'Struth FB_2 (zwischen FH1 und Röderwiesen)',
		'Struth FB_3 (oberhalb FH6-10)',
		'Streuobstwiese',
		'Ellersbach 1 (andere Seite)',
		'Ellersbach 2 (andere Seite)',
		'Ellersbach 3 (Parkplatzwiese)',
		'Ellersbach 4 (Gehege)'
	));
	$Smarty->display('Weideliste.tpl');

?>
