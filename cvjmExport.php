<?php

/**
 *  cvjmExport.php
 *  Exportiert Daten in das CSV-Format.
 *  Parameter:
 *  db: Datenbankname ohne cvjm_
 *  Search: Suchparameter
 *
 */

if ( ! isset($_REQUEST['db']) )
{
  die('Keine Datenbank angegeben!');
}
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
require('inc/misc/CVJM.inc');

/**
 * Schreibt den Inhalt eines Datenbankfeldes aufbereitet in die CSV-Datei
 * Dabei werden verknüpfte Felder aufgelöst und Datumsangaben etc. leserlich
 * ausgegeben
 * @param String $feldname der Feldname der angezeigt werden soll (nach mysql)
 * @param String $feld der Inhalt des auzugebenden Feldes 
 * @return String die leserliche Ausgabe des Feldes   
 */
function schreibeInhalt($feldname, $feld)
{
	$s = $feld;
	switch ($feldname)
	{
		case 'Geburtsdatum':
			 $s = date('d.m.Y', $feld);
			 break;
	}
	return $s;
}
if ( $session_userid == '' )
  die('Unauthorisiert!');

$Datenbank = 'cvjm_'.$_REQUEST['db'];
if ( ! isset($_REQUEST['Search']))
{
	$_REQUEST['Search'] = '';
}
$Search = trim(sql_real_escape_string($_REQUEST['Search']));
if ( ! isset($_REQUEST['docinput']))
{
  $docinput = array();	
}
else
{
$docinput = $_REQUEST['docinput'];
}

switch ($Datenbank )
{
  case 'cvjm_Adressen':
     $Suchen = 'NOT Geloescht';
     if ( $Search != '' )
        $Suchen .= " AND (Name REGEXP '$Search' OR Vorname REGEXP '$Search' OR Strasse ".
          "REGEXP '$Search' OR Ort REGEXP '$Search' OR Bemerkungen REGEXP '$Search'".
          "OR Telefon1 REGEXP '$Search' OR Telefon2 REGEXP '$Search' OR ".
          "Kunden_Nr REGEXP '$Search' OR ".
          "Email REGEXP '$Search' OR PLZ REGEXP '$Search' OR Zusatz REGEXP '$Search')";
     $Datenbank = '('.$Datenbank.' INNER JOIN '.TABLE_ANREDEN.' ON F_Anrede_id=Anrede_id)';
     if ( isset($docinput['Kategorie']) && is_numeric($docinput['Kategorie']) && $docinput['Kategorie']>0)
     {
       $Datenbank .= ' INNER JOIN '.TABLE_ADRESSEN_KATEGORIE.' ON Adressen_id=F_Adressen_id';
       $Suchen .= ' AND F_Kategorie_id='.$docinput['Kategorie'];
     }
     if ( isset($docinput['Edit']) && $docinput['Edit'] == 4)
       $Suchen .= ' AND Versandmarker';
     break;
  case 'cvjm_Artikel':
    $Suchen = 1;
    if ( $Search != '' )
      $Suchen .= " AND Bezeichnung REGEXP '".$Search."'";
    if ( isset($docinput['Art_id']) && is_numeric($docinput['Art_id']))
      $Suchen .= ' AND F_Art_id='.$docinput['Art_id'];
    break;
  default:
    $Suchen = '1';
}
header('Content-type: application/octet-stream');
// Es wird downloaded.pdf benannt
header('Content-Disposition: attachment; filename='.$_REQUEST['db'].date('ymdHi').'.csv');
if ( ! $query = sql_query('SELECT * FROM '.$Datenbank.' WHERE '.$Suchen))
  die("Fehler (Datenbank $Datenbank, Suchen: $Suchen): ".sql_error());
  $anz = sql_num_fields($query);
for ( $i = 0; $i < $anz-1; $i++)
{
  echo '"'.addslashes(sql_field_name($query, $i)).'";';
}
echo '"'.addslashes(sql_field_name($query, $anz-1)).'"'."\n";
while ( $ds = sql_fetch_row($query) )
{
  $felder = array();
	for ( $i = 0; $i < $anz; $i++)
  {
    $felder[] = schreibeInhalt(sql_field_name($query, $i), $ds[$i]);
  }
  echo '"'.implode('";"',$felder).'"'."\n";
}
sql_free_result($query);

?>