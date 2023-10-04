<?php
/**
 *
 *Dateiname: cvjmAdressenKategorien.inc
 *AWF-Name: cvjmadressenkategorien
 *
 *Parameter:
 * int Adressen_id die Id der Adresse die bearbeitet werden soll
 *
 */

if ( $profile['editor'] != 1 )
{
	if ( isset($_REQUEST['Adressen_id'] ) && is_numeric($_REQUEST['Adressen_id']))
	{		
		include(INC_PATH.'misc/CVJM.inc');
		require_once (INC_PATH.'cvjm/Adresse.class.php');
		include (INC_PATH . 'cvjm_ajax.php');
   	    $xajax->printJavascript('./javascript');

		$Adresse = new Adresse($_REQUEST['Adressen_id']);
		require_once (INC_PATH.'Smarty/Smarty.class.php');
		$Smarty = new Smarty();
		$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
		$Smarty->assign('title', transform($document['title'], $document['datatype']['title']));
		$Smarty->assign('Adresse', $Adresse);

		if ( ! $query = sql_query('SELECT Kategorie_id, Kategorie, Count(F_Kategorie_id) AS Anz, '.
		'Sum(F_Adressen_id='.$Adresse->Adresse['Adressen_id'].') AS Da FROM '.
		TABLE_KATEGORIEN.' LEFT JOIN '.TABLE_ADRESSEN_KATEGORIE.
		' ON Kategorie_id=F_Kategorie_id GROUP BY Kategorie, Kategorie_id ORDER BY Kategorie'))
		{
			throw new Exception('Fehler beim Laden der Kategorien: '.sql_error());
		}
		$Smarty->assign('id', $id);
		$ret = get_cmodule_id('cvjmadressen');
		// Rücksprungadresse, standardmäßig zurück zu den Adressstammdaten
		if ( isset($_REQUEST['Return']) && is_numeric($_REQUEST['Return']) )
		{
			$ret = $_REQUEST['Return'];
		}
		$Smarty->assign('ret', $ret);
		$Tabelle = array();
		while ( $kategorie = sql_fetch_array($query) )
		{
			$Tabelle[] = $kategorie;
		}
		$haelfte = ceil(Count($Tabelle) / 2);
		// Umsortieren für zweispaltige Anzeige
		$Kategorien = array();
		for ( $i = 0; $i <= $haelfte; $i++)
		{
			$Kategorien[] = $Tabelle[$i];
			if ( $i+$haelfte < Count($Tabelle) )
			{
				$Nr = $i + $haelfte;
				$Kategorien[] = $Tabelle[$Nr];
			}
		}
		sql_free_result($query);
		$Smarty->assign('Kategorien', $Kategorien);
		$Smarty->assign('Adressenurl', get_url(get_cmodule_id('cvjmadressen')));
		$Smarty->display('Adresskategorien.tpl');
	}
	else
	{
		echo '<div class="SchwererFehler">Es wurde keine Adressenid übergeben!</div>';
	}
}

?>
