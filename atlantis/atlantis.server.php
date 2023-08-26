<?php
/*
 * Created on 12.11.2006
 * Serverkomponente f�r die AJAX-Anfragen der Templates
 *
 * Christoph Griep
 */
session_start();
require('atlantis.db.php');
require 'Smarty/libs/Smarty.class.php';

function zeigeFertigkeit($fertigkeit_id)
{	
	$ergebnis = '(keine Beschreibung vorhanden)';
	
	$ids = explode(',',$fertigkeit_id);
	if ( ! is_array($ids))
	  $ids[0] = $ids;
	if ( is_numeric($ids[0]))
	{
	  $query = sql_query('SELECT Fertigkeit, Beschreibung FROM T_Fertigkeiten ' .
			  'WHERE Fertigkeit_id='.$ids[0]);
	  if( $beschreibung = sql_fetch_row($query))
	  {		
		if ( $beschreibung[1] != '')
		  $ergebnis = nl2br($beschreibung[1]);
		$ergebnis = '<h1>'.$beschreibung[0].'</h1>'.$ergebnis;
  	  }
	  sql_free_result($query);
	}
	$ergebnis = utf8_encode($ergebnis);
	$objResponse = new xajaxResponse();
	$objResponse->addAssign("Beschreibung", "innerHTML", $ergebnis);
	return $objResponse;
} 

function zeigeRasse($rassen_id)
{	
	$ergebnis = '(keine Beschreibung vorhanden)';
	$bild = '(kein Bild vorhanden)';	
	if ( is_numeric($rassen_id))
	{
	  $smarty = new Smarty;			  
	  $query = sql_query('SELECT Rasse, Bild, Beschreibung, Rasse FROM T_Rassen ' .
			  'WHERE Rasse_id='.$rassen_id);
	  if( $beschreibung = sql_fetch_array($query))
	  {
	  	if ( $beschreibung['Beschreibung'] != '')		
		  $ergebnis = $beschreibung['Beschreibung'];
		else
		  $ergebnis = '(keine Beschreibung vorhanden)';
		if ( $beschreibung['Bild'] != '')		
		  $bild = '<img src="zeigeBild.php?Rasse='.$rassen_id.'&Groesse=-240" title="'.
		    $beschreibung['Rasse'].'" alt="'.$beschreibung['Rasse'].'" />';		
        $smarty->assign('Rasse', utf8_encode($beschreibung['Rasse']));   	  
  	  }
	  sql_free_result($query);
	  // Rassenbesonderheiten anzeigen 
	  $besonderheiten = array();
	  $query = sql_query('SELECT * FROM T_RassenFertigkeiten INNER JOIN T_Fertigkeiten ' .
	  		'ON F_Fertigkeit_id=Fertigkeit_id WHERE F_Rasse_id='.$rassen_id);
	  while ( $b = sql_fetch_array($query))
	  {
	  	foreach ( $b as $key => $value)
	  	$bb[$key] = utf8_encode($value);
	  	if ( $bb['Beschreibung'] == '') $bb['Beschreibung'] = '(keine Beschreibung vorhanden)';
	  	$besonderheiten[] = $bb;
	  }
	  sql_free_result($query);	  
	  $smarty->assign('Beschreibung', utf8_encode($ergebnis));
	  $smarty->assign('Besonderheiten', $besonderheiten);
	  $ergebnis = $smarty->fetch('rasse.tpl');
	}
	$objResponse = new xajaxResponse();
	$objResponse->addAssign("Beschreibung", "innerHTML", $ergebnis);
	$objResponse->addAssign("Bild", "innerHTML", $bild);
	return $objResponse;
} 

function zeigeSpruchliste($id)
{	
	$s = '(keine Eintr�ge vorhanden)';
	if ( is_numeric($id))
	{
	  $query = sql_query('SELECT Klasse, Spezialisierung, Meisterpunkte, Grossmeisterpunkte, ' .
	  		'Spruchliste, Allgemein FROM T_Spezialisierungen INNER JOIN T_Spezialisierungsklassen ' .
	  		'ON F_Klasse_id=Klasse_id ' .
			'WHERE Spezialisierung_id='.$id);
	  $name = sql_fetch_array($query);
	  sql_free_result($query);
	  $query = sql_query('SELECT Fertigkeit_id, Fertigkeit, Kosten, Rang, ' .
	  		  'T_Fertigkeiten.Beschreibung AS Beschreibung FROM T_Fertigkeiten ' .
			  'INNER JOIN (T_SpezialisierungenFertigkeiten INNER JOIN T_Raenge ' .
			  'ON Rang_id=F_Rang_id) ON F_Fertigkeit_id=Fertigkeit_id ' .
			  'WHERE F_Spezialisierung_id='.$id.' ORDER BY Rang_id, Kosten, Fertigkeit');
	  while( $beschreibung = sql_fetch_array($query))
	  {			
			if ($beschreibung['Beschreibung']=='')
			  $beschreibung['Beschreibung'] = '(ohne Beschreibung)';
			$sprueche[] = $beschreibung;
  	  }
  	  sql_free_result($query);
	  $smarty = new Smarty;		
	  $smarty->assign('Spezialisierung', $name);
	  $smarty->assign_by_ref('Sprueche', $sprueche);
	  $s = $smarty->fetch('spruchliste.tpl');
	}
	$s = utf8_encode($s);
	$objResponse = new xajaxResponse();
	$objResponse->addAssign("Beschreibung", "innerHTML", $s);
	return $objResponse;
} 


function zeigeKlasse($klassen_id)
{	
	$ergebnis = '(keine Beschreibung vorhanden)';
	$bild = '(kein Bild vorhanden)';	
	if ( is_numeric($klassen_id))
	{
	  $query = sql_query('SELECT Bild, Beschreibung, Klasse, Adeptenpunkte, ' .
	  		'Spruchlistenanzahl FROM T_Spezialisierungsklassen ' .
			  'WHERE Klasse_id='.$klassen_id);
	  if( $beschreibung = sql_fetch_array($query))
	  {
		$ergebnis = '<h1>'.$beschreibung['Klasse'].'</h1>';
		if ( $beschreibung['Beschreibung'] != '')		
		  $ergebnis = $beschreibung['Beschreibung'];
		$ergebnis .= '<p>Zum Adepten ben�tigt man in dieser Klasse '.$beschreibung['Adeptenpunkte'].' Punkte';
		if ( $beschreibung['Spruchlistenanzahl']>0)
		{
			$ergebnis .= ', man hat insgesamt '.$beschreibung['Spruchlistenanzahl'].' Spruchlisten zur Verf�gung';
		}
		$ergebnis .= '.</p>';
		if ( $beschreibung['Bild'] != '')		
		  $bild = '<img src="zeigeBild.php?Klasse='.$klassen_id.'&Groesse=-240" title="'.
		    $beschreibung['Klasse'].'" alt="'.$beschreibung['Klasse'].'" />';		
  	  }
	  sql_free_result($query);
	}
	$objResponse = new xajaxResponse();
	$objResponse->addAssign("Beschreibung", "innerHTML", utf8_encode($ergebnis));
	$objResponse->addAssign("Bild", "innerHTML", $bild);
	return $objResponse;
} 
 
/**
 * Zeigt die Daten des aktuellen Charakters in der Box "Daten" an.
 */
function zeigeDaten($id= NULL)
{
	include_once('character.class.php');
	$ergebnis = '(kein Charakter ausgew�hlt)';
	if (is_numeric($id))
	{
		$Charakter = new Charakter();
		if (!$Charakter->loadFromDatabase($id))
		 unset($Charakter);
	}
    elseif ( session_is_registered('Charakter_id'))
	{
		$Charakter = new Charakter();
		if (!$Charakter->loadFromDatabase($_SESSION['Charakter_id']))
		  unset($Charakter);
	}
	elseif ( session_is_registered('Charakter'))
	{
		$Charakter = unserialize($_SESSION['Charakter']);
	}
	if ( isset($Charakter))
	{
	  $smarty = new Smarty;		
	  $smarty->assign_by_ref('Character', $Charakter->alsFeld(true));
	  $ergebnis = $smarty->fetch('character.tpl');		
	}
	$objResponse = new xajaxResponse();
	$objResponse->addAssign('Daten', 'innerHTML', utf8_encode($ergebnis));
	return $objResponse;
} 
 
require('atlantisajax.php');
$xajax->processRequests();
 
?>