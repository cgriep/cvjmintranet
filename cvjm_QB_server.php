<?php

include('inc/functions.inc');
include('inc/licence.key');
include('inc/sessions.inc');
include('inc/caching.inc');
require('inc/database.inc');
// Read all constants from database
$qresult = sql_query ('SELECT name, value FROM awf_setup');
while ($row = sql_fetch_row($qresult)) {
        define(strtoupper($row[0]),$row[1]);
        }
sql_free_result($qresult);

include_once('inc/misc/CVJM.inc');
include('inc/cvjm/Artikel.class.php');
require('inc/cvjm/Buchung.class.php');

$query = sql_query('SELECT Buchung_Nr FROM cvjm_Buchungen WHERE Bis BETWEEN '.strtotime('-10 days').' AND '.strtotime('-1 day'));
while ( $bnr = sql_fetch_array($query))
{
	$Buchung = new Buchung($bnr['Buchung_Nr']);
	echo $Buchung->Buchung_Nr.'<br />';
}
if ( sql_num_rows($query) == 0)
{
	echo 'Keine Buchungen gefunden.';
}
sql_free_result($query);

?>