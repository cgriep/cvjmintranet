<?php

/**
 *  cvjmBelegungsExport.php
 *  Exportiert Daten zur Belegung auf Hütten/Saal-Ebene in ein JSON Format für die Automatisierung
 *
 */

require('inc/database.inc');
require('inc/db_tables.inc');

// Read all constants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
while ($row = sql_fetch_row($qresult)) {
	define(strtoupper($row[0]),$row[1]);
	}
sql_free_result($qresult);

include('inc/functions.inc');
include('inc/licence.key');
include('inc/sessions.inc');
include(INC_PATH.'misc/CVJM.inc');
include(INC_PATH.'misc/cvjmBuchungstools.inc');
include(INC_PATH.'misc/cvjmArtikeltools.inc');
include_once(INC_PATH.'cvjm/Artikel.class.php');

//if ( $session_userid == '' )
//  die('Unauthorisiert!');

header('Content-type: application/json');
header('Content-Disposition: attachment; filename=belegung.json');

$orte = array(
  170, // Saal HH
  154, // DZ 1
  155,
  156,
  157, // DZ 4
  39,
  40, // DZ 6
  29, // MZH
  42, // WBH
  1, // BH 1
  2,
  18,
  21,
  22, // BH 5
  15,
  17, // BH 7
  23, // Heuhaus
  25, // KZH
  99, // Saal AHH
  61, // FH 1
  62,
  63,
  64,
  65,
  66, // FH 6
  67, // FH 9
  68, // FH 7
  69,
  70, // FH 10
  60, // GemHs
  258, // Saal Zwig
  259,
  260,
  261,
  308, // Tavernenhaus
  696, // Pizzahütte
  252 // SV Küche
);
/*

Tag - erster Tag der Anzeige
Tage - Anzahl der anzuzeigenden Tage
docinput[Kategorie][] - Kategorien, die angezeigt werden sollen
*/
$Tag = new \DateTime("midnight");

$sql = 'SELECT id, Bezeichnung FROM '.TABLE_ARTIKEL.' WHERE id in ('.implode(',',$orte).')';     
$qu = sql_query($sql);
$Artikel = array();
while ($entry = sql_fetch_row($qu))
{
  $artikel[$entry[0]] = array('Ort' => $entry[1], 'Gestern' => 0, 'Heute' => 0, 'Morgen'=>0);  
}
$artikel['Stand'] = new DateTime();
sql_free_result($qu);

for ( $i = 0; $i <= 2; $i++ )
{      
  $datum = strtotime(($i-1).' day',$Tag->getTimestamp());
  $artikel[$i==0? 'Gestern' : ($i==1?'Heute':'Morgen')] = date('d-m-Y', $datum);
    $sql = 'SELECT F_Artikel_Nr FROM '.TABLE_BUCHUNGEN.
         ' INNER JOIN '.TABLE_BUCHUNGSEINTRAEGE.
        ' ON F_Buchung_Nr=Buchung_Nr '.
        'WHERE F_Artikel_Nr IN ('.implode(',',$orte).') AND BStatus IN (0,'.BUCHUNG_FERTIG.','.
         BUCHUNG_VORRESERVIERUNG.','.BUCHUNG_RESERVIERUNG.','.BUCHUNG_INTERN.') '.
        " AND Datum=$datum AND Bis>".strtotime('+23 hour',$datum); 
    
    if (!$buchungen = sql_query($sql)) echo sql_error();
    while ( $buchung = sql_fetch_row($buchungen))
    {      
      $artikel[$buchung[0]][$i==0? 'Gestern' : ($i==1?'Heute':'Morgen')]++;
    }
    sql_free_result($buchungen);
   
}
echo json_encode($artikel, JSON_PRETTY_PRINT | JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);

?>


