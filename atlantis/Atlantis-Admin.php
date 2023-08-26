<?php


/**
 * Administrationsoberfl�che für die Atlantis-Daten
 * 2006 Christoph Griep
 */
include ('atlantis.db.php');
$template = 'Liste';

include('atlantis.export.php');

$feld = array();
if ($sql != '')
{
	if(!$query = sql_query($sql)) 
	{
	  echo 'Fehler: '.$sql.'/'.sql_error();	
	}
	while ($zeile = sql_fetch_array($query))
	{
		$z = array();		
		foreach ( $zeile as $key => $value )
		{
			if ( !is_numeric($key) && $key != $gruppe )
			{
				// Bilder berücksichtigen 
			    if ( $key == 'Bild' && $id != '')
			    {
			    	$z[$key] = '<a href="zeigeBild.php?'.$id.'='.$zeile[$id.'_id'].
                       '" target="_blank"><img src="zeigeBild.php?'.$id.'='.
                       $zeile[$id.'_id'].'" width="100px"/></a>';
			    }
			    else
			      $z[$key] = $value;
			}
		} 
		if ( $gruppe != '')
		  $feld[$zeile[$gruppe]][] = $z;
		else
		  $feld[$action][] = $z;
	}
	sql_free_result($query);
}

require 'Smarty/libs/Smarty.class.php';

$smarty = new Smarty;

$smarty->assign('PageTitle', 'Listen');
$smarty->assign('mitAjax', false);
$smarty->assign('Hinweis',$hinweis);
$smarty->assign('zeilen', $feld);

$smarty->display($template . '.tpl');
?>
