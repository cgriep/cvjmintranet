<?php

function BuchungstabelleAnzeigen($Where, $nurAnsicht = true, $WhereSeminar = '')
{
	require_once(INC_PATH.'Smarty/Smarty.class.php');
	require_once(INC_PATH . 'cvjm/Buchung.class.php');
	$Smarty = new Smarty;
	$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
	global $profile;
	global $id;
	global $Altersgruppen;
	global $docinput;
	global $document;
	// Liste der vorhandenen Buchungen
	// Übersicht über die aktuellen Belegungen
	$Status = 'BStatus = -1'; // zunächst nichts anzeigen
	if ( isset($_REQUEST['Search'] ))
	{
		if ( isset($docinput['Reservierungen']) )
		{
		set_profile('BuchungRes', 1);
		}
		else
		{
		remove_profile('BuchungRes');
		}
		if ( isset($docinput['Stornierungen']) )
		{
		set_profile('BuchungSto', 1);
		}
		else
		{
		remove_profile('BuchungSto');
		}
		if ( isset($docinput['Abgerechnet']) )
		{
		set_profile('BuchungAb',1);
		}
		else
		{
		remove_profile('BuchungAb');
		}
		if ( isset($docinput['Vorreservierungen']) )
		{
		set_profile('BuchungVor',1);
		}
		else
		{
		remove_profile('BuchungVor');
		}
	}
	$stat = array();
	if ( $profile['BuchungRes'] == 1 ) $stat[] = BUCHUNG_RESERVIERUNG;
	//$Status .= ' OR BStatus = '.BUCHUNG_RESERVIERUNG;
	if ( $profile['BuchungAb'] == 1 ) 
	{
		$stat[] = BUCHUNG_FERTIG;
		$stat[] = BUCHUNG_STORNIERTABGERECHNET;
		$stat[] = BUCHUNG_INTERN;
	//$Status .= ' OR BStatus = '.BUCHUNG_FERTIG.
	//' OR BStatus = '.BUCHUNG_STORNIERTABGERECHNET.' OR BStatus = '.BUCHUNG_INTERN;
	}
	if ( $profile['BuchungVor'] == 1 ) 
	{
		$stat[] = BUCHUNG_VORRESERVIERUNG;
		$stat[] = 0;
		$stat[] = 1;
		//$Status .= ' OR (BStatus <= '.BUCHUNG_VORRESERVIERUNG.')';
	}
	if ( $profile['BuchungSto'] == 1 ) 
	{
		$stat[] = BUCHUNG_GELOESCHT;
		$stat[] = BUCHUNG_STORNIERT;
		//$Status .= ' OR BStatus = '.BUCHUNG_GELOESCHT.' OR BStatus='.BUCHUNG_STORNIERT;
	}
	$Where = 'WHERE ('.$Where.')';
	if ( Count($stat) > 0 ) //$Status != '' ) 
	{
		$Status = 'BStatus IN ('.implode($stat,',').')';
		$Where .= 'AND ('.$Status.')';
		if ( $WhereSeminar != '' ) $WhereSeminar .= ' AND ('.$Status.')';
	}	
	if ( is_numeric($docinput['Altersgruppe']) && $docinput['Altersgruppe'] != -1 )
	{
		$Where .= ' AND Altersgruppe='.$docinput['Altersgruppe'];
	}
	if ( ! $nurAnsicht )
	{
		if ( ! $query = sql_query('SELECT Count(*) FROM '.TABLE_BUCHUNGEN.
		' INNER JOIN '.TABLE_ADRESSEN." ON Adressen_id=F_Adressen_id $Where")) echo mysql_error();
		$anz = sql_fetch_row($query);
		$Gesamtanzahl = $anz[0];
		$AnzahlProSeite = 15;
		sql_free_result($query);
	}
	else
	{
		$AnzahlProSeite = 100;
		$Gesamtanzahl = 100;
		$Seite = 1;
	}
	if ( isset($_REQUEST['Seite']) && is_numeric($_REQUEST['Seite']) )
	{
		$Seite = $_REQUEST['Seite'];
	}
	else
	{
		$Seite = 1;
	}
	$Smarty->assign('title', transform($document['title'], $document['datatype']['title']));

	$Smarty->assign('id', $id);
	if ( isset($_REQUEST['Search']) )
	{
		$Smarty->assign('Search', $_REQUEST['Search']);
	}
	elseif ( $nurAnsicht )
	{
		$Smarty->assign('Search', date('d.m.Y'));
	}
	$Smarty->assign('Tage', $profile["Tage".$id]);
	$Optionen = array();
	$Optionen['Vorreservierungen'] = $profile['BuchungVor'] == 1;
	$Optionen['Reservierungen'] = $profile['BuchungRes'] == 1;
	$Optionen['Stornierungen'] = $profile['BuchungSto'] == 1;
	$Optionen['Abgerechnet'] = $profile['BuchungAb'] == 1;
	$params = '';
	foreach ( $Optionen as $key => $value )
	{
		if ( $value )
		{
			$params .= '&docinput['.$key.']=v';
		}
	}
	$Smarty->assign('params', $params);
	$Smarty->assign('Optionen', $Optionen);
	$Smarty->assign('nurAnsicht', $nurAnsicht);
	$Smarty->assign('Seiten', $Gesamtanzahl/$AnzahlProSeite);
	$Smarty->assign('Seite', $Seite);
	$sql = 'SELECT Buchung_Nr, Von FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_ADRESSEN.' ON F_Adressen_id=Adressen_id '.$Where;
	//Seminar ergänzen
	if ( $WhereSeminar != '')
	{
		$sql .= ' UNION SELECT Buchung_Nr, Von FROM '.TABLE_BUCHUNGEN.' INNER JOIN ('.TABLE_SEMINARTEILNEHMER.
		' INNER JOIN '.TABLE_ADRESSEN.' ON '.TABLE_SEMINARTEILNEHMER.
		'.F_Adressen_id=Adressen_id) ON F_Buchung_Nr=Buchung_Nr WHERE '.$WhereSeminar;
	}
	$sql .= ' ORDER BY Von '. sql_limit($AnzahlProSeite,($Seite-1)*$AnzahlProSeite);
	if ( ! $query = sql_query($sql))
	{
		throw new Exception('Buchungstabelle Anzeigen: '.$sql.'/'.mysql_error());
	}
	$Buchungen = array();
	while ( $buchung = sql_fetch_array($query) )
	{
		$Buchung = new Buchung($buchung['Buchung_Nr']);		
		$Buchungen[] = $Buchung;
	}
	sql_free_result($query);
	$Smarty->assign('Buchungen', $Buchungen);
	$b = new Buchung();
	$Smarty->assign('Altersgruppen', $b->getAltersgruppen());
	setlocale(LC_ALL, 'de_DE@euro', 'de_DE', 'de', 'ge');
	$Smarty->display('Buchung_Tabelle.tpl');
}


?>
