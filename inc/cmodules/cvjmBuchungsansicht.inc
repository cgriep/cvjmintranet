<?php

/* Modul Buchungsansicht */
/* Parameter:

AWF-Name: cvjmbuchungsansicht

*/

include(INC_PATH.'misc/CVJM.inc');
include(INC_PATH.'misc/cvjmBuchungstools.inc');
include(INC_PATH.'misc/cvjmBuchungstabelle.inc');
include(INC_PATH.'misc/cvjmArtikeltools.inc');
include(INC_PATH.'misc/Kalenderjs.inc');

unset($_SESSION['LetzteRechnung']);

if ( $profile['editor'] != 1 )
{
	if ( !isset($profile['Tage'.$id]))
    {
    	set_profile('Tage'.$id,15);
    	$profile['Tage'.$id] = 15;
    }
	if ( isset($_REQUEST['Search'] ))
    {
      if ( ! is_numeric($docinput['Tage'])) 
      {
      $docinput['Tage'] = 10;
      }
      set_profile('Tage'.$id,$docinput['Tage']);
      $profile['Tage'.$id] = $docinput['Tage'];
    }
    $Where = '';
    if ( isset($_REQUEST['Search']) && trim($_REQUEST['Search']) != '' )
    {
      $Datum = convertDatumToTimestamp($_REQUEST['Search']);
      if ( $Datum != 0 )
      {
        $Where = '(Von <= '.strtotime('+'.$profile['Tage'.$id].' days',$Datum).
          ' AND Bis >= '.$Datum.')';
      }
      else
      {
      	if ( is_numeric($_REQUEST['Search']))
      	{
      		// nach Buchungsnummer suchen
			$Where = 'Buchung_Nr='.$_REQUEST['Search'];
      	}
      	else
      	{
      		// nach Kunde suchen
			$Where = 'Name LIKE "%'.mysql_real_escape_string($_REQUEST['Search']).'%"';
      	}
      }
    }
    else
    {
      $dat = strtotime('+'.$profile['Tage'.$id].' days',
          mktime(0,0,0,date('m',time()),date('d',time()),date('Y',time())));
      $Where = 'Von <= '.$dat.' AND Bis >= '.
        mktime(0,0,0,date('m',time()),date('d',time()),date('Y',time()));
    }
    BuchungstabelleAnzeigen($Where);
} // wenn nicht editor

?>