<?php
/*
 * Created on 16.05.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */

include('inc/functions.inc');
include('inc/licence.key');
// include('inc/sessions.inc');
include('inc/caching.inc');
require('inc/database.inc');
require('inc/db_tables.inc');

$query = mysql_query('SELECT * FROM cvjm_Erinnerung');
while ( $termin = mysql_fetch_array($query))
{
  $derTermin = get_nodedata ($termin['Termin_id'], 1);
  if ( isset($derTermin['Datum']) && is_numeric($derTermin['Datum']))
  {  	
  	$datum = $derTermin['Datum'];
  	switch ($derTermin['Turnus'])
  	{
  		case 'j':   		  
  		  $datum = mktime(0,0,0,date('m',$datum),date('d',$datum),date('Y'));
  		  if ( $datum < time() ) $datum = strtotime('+1 year',$datum);
  		  break;  		
  		case 'm': 
  		  if ( date('d') < date('d',$datum))
  		    $datum = mktime(0,0,0,date('m'),date('d',$datum),date('Y'));
  		  else
  		    $datum = strtotime('+1 month',mktime(0,0,0,date('m'),date('d',$datum),date('Y')));
  		  break;  		
  	}
  	$mdatum = strtotime(date('Y-m-d',strtotime('-'.$termin['Art'].' days',$datum)));
  	if ( time() > $mdatum )
  	{
 echo date('d.m.Y',$mdatum);
  		// Nachricht senden
  		$mailname = get_user_email($termin['User_id']);
  		mail($mailname,'[Erinnerung] '.transform($derTermin['title'],'clean'), 
  		  'am '.date('d.m.Y',$datum).' ist Termin "'.
  		  transform($derTermin['title'],'clean').'".'."\n".
  		  transform($derTermin['body'],'clean')."\nTermin-ID: ".$termin['Termin_id']);  		  
  		mysql_query('DELETE FROM cvjm_Erinnerung WHERE Erinnerung_id='.$termin['Erinnerung_id']); 
  	}  	
  }  
}
mysql_free_result($query);

?>
