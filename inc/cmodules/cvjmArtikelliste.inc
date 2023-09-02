<?php
/**
  * cvjmArtikelliste
  * AWF-Name: cvjmartikelliste
*
*/
include(INC_PATH.'cvjm/Artikel.class.php');
include(INC_PATH.'Smarty/Smarty.class.php');

if ( $profile["editor"] != 1 )
{
  $Smarty = new Smarty();
  $Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
  $Smarty->assign('title', transform($document['title'],$document['datatype']['title']));
  $Smarty->assign('id', $id);
  
  include(INC_PATH."misc/CVJM.inc");
  $preislisten = sql_query("SELECT * FROM ".TABLE_PREISLISTEN.
    " WHERE NOT Inaktiv ORDER BY GueltigAb DESC, Bezeichnung");
  $selected = false;
  $Preislisten = array();
  while ( $preisliste = sql_fetch_array($preislisten) )
  {
    if ( $preisliste['Standard'] )
    {
    	
    	$Smarty->assign('Preisliste_id', $preisliste['Preisliste_id']);
    }
    $Preislisten[$preisliste["Preisliste_id"]] = $preisliste['Bezeichnung'].' ('.
      date('d.m.Y', $preisliste['GueltigAb']).')';
  }
  $Smarty->assignbyref('Preislisten', $Preislisten);
  sql_free_result($preislisten);
  if ( isset($docinput["Preisliste"]) && is_numeric($docinput["Preisliste"]) )
  {
  	if ( isset($docinput['Art']))
  	{
  		$Where = ' AND F_Art_id IN ('.implode(',',$docinput['Art']).')';
  		$Smarty->assign('Arten', implode(',',$docinput['Art']));
  	}
  	else
  	{
  		$Where = '';
  		$Smarty->assign('Arten', '');
  	}
  	$Smarty->assign('Preisliste_id', $docinput['Preisliste']);
  	$Smarty->assign('username', get_user_nickname());
  	$sql = "SELECT id FROM (".TABLE_ARTIKEL." INNER JOIN ".TABLE_PREISE.
      " ON id=Artikel_Nr) INNER JOIN ".TABLE_ARTIKELARTEN." ON F_Art_id=Art_id ".
      " WHERE F_Preisliste_id=".$docinput["Preisliste"].$Where.
      " ORDER BY Art, Bezeichnung";
   if (!$query = sql_query($sql)) echo sql_error();
    $Artikel = array();
    while ( $artikel = sql_fetch_array($query) )
    {
      $Artikel[] = new Artikel($artikel['id']);
    }
    $Smarty->assignbyref('Artikel', $Artikel);
    sql_free_result($query);
    $Buchungsvorlagen = glob(CVJM_VORLAGEN_DIR."/ArtikelPreislisten/*".CVJM_ENDUNG);
    $anz = 0;
    $Vorlagen = array();
    foreach ($Buchungsvorlagen as $filename)
    {
      $Vorlagen[] = basename($filename, CVJM_ENDUNG);
    }
    $Smarty->assign('Vorlagen', $Vorlagen);
	
  }
  $Smarty->display('Artikel_Preisliste.tpl');
}
?>
