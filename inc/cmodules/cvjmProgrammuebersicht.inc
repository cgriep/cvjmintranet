<?php


/**
Modul Programmübersicht

*/

if ($profile["editor"] != 1)
{

	include (INC_PATH . "misc/CVJM.inc");
	include (INC_PATH . "misc/cvjmArtikeltools.inc");
	include (INC_PATH . "misc/cvjmBuchungstools.inc");
	include (INC_PATH . "misc/Kalenderjs.inc");
	require_once(INC_PATH.'Smarty/Smarty.class.php');
	require_once(INC_PATH.'cvjm/Event.class.php');
	require_once(INC_PATH.'cvjm/Buchung.class.php');
	require_once(INC_PATH.'cvjm/Artikel.class.php');
  	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';	
  	$Smarty = new Smarty();
  	$Anzahl = 0;
	$Smarty->assign('title',transform($document['title'], $document['datatype']['title']));
	if (isset ($_REQUEST["Datum"]))
	{
		$Tag = convertDatumToTimestamp($_REQUEST["Datum"]);
	}
	else
	{
		$Tag = time();
	}
	// Datumsauswahl angeben
	$Smarty->assign('id', $id);
	$Smarty->assign('Tag', $Tag);	

	// Nur Module holen
	$sql = "SELECT Buchung_Nr, Datum, Dauer, F_Artikel_Nr, id FROM " . TABLE_BUCHUNGEN . ' INNER JOIN (' .
	TABLE_BUCHUNGSEINTRAEGE . ' INNER JOIN ' . TABLE_ARTIKEL .
	' ON F_Artikel_Nr=id) ON F_Buchung_Nr=Buchung_Nr ' .
	'WHERE F_Art_id=' . CVJMART_PROGRAMM;
	if (isset ($_REQUEST['Buchung_Nr']) && is_numeric($_REQUEST['Buchung_Nr']))
	{
		$sql .= ' AND Buchung_Nr=' . $_REQUEST['Buchung_Nr'];
	}
	else
	  $sql .= ' AND Datum > ' . $Tag;
	$sql .= ' ORDER BY Datum';
	$query = sql_query($sql);
	$zeilen = array();
	$Smarty->assign('artikelurl', get_url(get_cmodule_id("cvjmartikel")));
	$Smarty->assign('Buchungurl',get_url(get_cmodule_id('cvjmbuchung')));
	while ($row = sql_fetch_array($query))
	{
		$row['Artikel'] = new Artikel($row['F_Artikel_Nr']);
		$row['Buchung'] = new Buchung($row['Buchung_Nr']);
		
		// Buchung holen 
		$events = Event::searchBuchungEvent($row['Datum'], $row['Datum'],$row['Buchung_Nr'], $row['id']);
		$name = array();
		foreach ($events as $event)
		{
					foreach ($event->Betroffene as $nr => $betroffener)
					{
						$wer = $event->getBetroffenenName($nr);
						if ( $wer != '' && ! in_array($wer, $name) )
						{
							$name[] = $wer;
						}
					}	
		}		
		$row['zustaendig'] =implode(',',$name); 
		$zeilen[] = $row;
	}
	sql_free_result($query);
	$Smarty->assign_by_ref('zeilen', $zeilen);
	$Smarty->display('Programmuebersicht.tpl');

} // wenn nicht Editor
?>
