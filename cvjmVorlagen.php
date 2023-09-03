<?php

/**
  * cvjmVorlage.php
  * Erzeugt nach einer Vorlage eine OpenOffice-Datei mit den Daten
  * Parameter:
  * db - Die Datenbank ohne cvjm_
  * id - Die ID-Nummer des gesuchten Datensatzes
  * Vorlage - Dateiname der Vorlage (muss im Vorlagen-Verzeichnis liegen, Unterverzeichnis
  *   entsprechend dem Datenbanknamen
*/

//set_magic_quotes_runtime(0);

DEFINE ('OHNE_BERECHNUNG', 'o.B.');

include('inc/functions.inc');
include('inc/licence.key');
include('inc/sessions.inc');
include('inc/caching.inc');
require('inc/database.inc');
// Read all constants from database
$qresult = sql_query ("SELECT name, value FROM awf_setup");
while ($row = sql_fetch_row($qresult)) {
        define(strtoupper($row[0]),$row[1]);
        }
sql_free_result($qresult);

include_once('inc/misc/CVJM.inc');
include('inc/misc/cvjmBuchungstools.inc');
include('inc/misc/cvjmArtikeltools.inc');
include('inc/cvjm/Artikel.class.php');

function Preis($wert, $mitOb = true)
{
  $wert = str_replace(',','.',$wert);
  if ( ! is_numeric($wert) ) $wert = 0;
  if ( $wert == 0 )
    if ( $mitOb )
      return OHNE_BERECHNUNG;
    else
      return '0,00';
  else
    return number_format($wert, 2, ',', ''); // Euro-Zeichen
}

function PreisToZahl($preis)
{
  if ($preis == OHNE_BERECHNUNG ) $preis = 0;
  $preis = str_replace(',','.',$preis);
  return $preis;
}

function AddPreis($preis1, $preis2, $mitOb = true)
{
  if ($preis1 == OHNE_BERECHNUNG ) $preis1 = 0;
  if ($preis2 == OHNE_BERECHNUNG ) $preis2 = 0;
  $preis1 = str_replace(',','.',$preis1);
  $preis2 = str_replace(',','.',$preis2);
  return Preis($preis1+$preis2, $mitOb);
}

function sortiereDatenNachDatum($daten)
{
  $felder = array('ARTIKELNR', 'ART', 'BEZEICHNUNG', 'MENGE', 'DATUM', 'PREIS', 'MWST',
                  'MWSTSUMME', 'EINHEIT', 'RABATT', 'GESAMT');
  $vorspann = array('Z'); //, 'V', 'Z');
  foreach ( $vorspann as $x => $zeichen )
  {  	
  	$tempdaten = array();
  	$tdaten = array();
    $keypositionen = array();
	// alle Daten zwischensichern  
    foreach($felder as $f => $feld)
    {
    	$tdaten[$zeichen.$feld] = $daten[$zeichen.$feld];
    	$daten[$zeichen.$feld] = array(); // inhalte leeren
    }    
    foreach ($daten['SORT'.$zeichen.'DATUM'] as $key => $art)
    {
       if ( $art == 0 )
       {
       		// neue Überschrift - bisherige Daten sortieren
    		array_multisort($tempdaten, SORT_NUMERIC, $keypositionen);
				foreach($felder as $f => $feld)
    		{ 
				// neue Inhalte in neuer Reihenfolge eintragen 
     			foreach ( $keypositionen as $pos)
        		{
        			$daten[$zeichen.$feld][] = $tdaten[$zeichen.$feld][$pos];
        		}
    		}
    		// aktuelle Überschrift einfügen
    		foreach($felder as $f => $feld)
    		{ 
				  $daten[$zeichen.$feld][] = $tdaten[$zeichen.$feld][$key];
    		}
    		$tempdaten = array();
    		$keypositionen = array();   	  
       }
       else 
       {
       		// Daten übertragen
       		$tempdaten[] = $daten['SORT'.$zeichen.'DATUM'][$key];       
       		$keypositionen[] = $key;
       }
    }
    if ( count($tempdaten) > 0 )
    {
    	array_multisort($tempdaten, SORT_NUMERIC, $keypositionen);
    	foreach($felder as $f => $feld)
    	{ 
				foreach ($keypositionen as $pos)
				{
					$daten[$zeichen.$feld][] = $tdaten[$zeichen.$feld][$pos];
				}
    	}
    }    
  }
  return $daten;
}

function Kuechenzettel($Buchung_Nr, $daten)
{
  global $attribute;
  $sql = 'SELECT F_Buchung_Nr, F_Artikel_Nr, Datum, Menge, Bezeichnung, '.
      'Position FROM '.TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.
      TABLE_ARTIKEL.' ON id=F_Artikel_Nr WHERE F_Art_id='.CVJMART_VERPFLEGUNG.
        " AND F_Buchung_Nr=$Buchung_Nr AND NOT Unberechnet ORDER BY Datum";
  if ( ! $Eintraege = sql_query($sql) ) echo "Fehler $sql: ".sql_error();
  $Artikel = array();
  $Essen = array();
  $Buchung = array();
  while ( $Eintrag = sql_fetch_array($Eintraege) )
  {
      $Artikel[$Eintrag['F_Artikel_Nr']] = $Eintrag['Bezeichnung'];
      $ArtikelPos[$Eintrag['F_Artikel_Nr']] = $Eintrag['Position'];
      $ArtikelNr[$Eintrag['F_Artikel_Nr']] = $Eintrag['F_Artikel_Nr'];
      $Essen[$Eintrag['Datum']][$Eintrag['F_Artikel_Nr']] = $Eintrag['Menge'];
  }
  sql_free_result($Eintraege);
  $daten['ESSEN'][0][0] = 'Datum';
  if ( Count($Artikel) > 0 )
  {
      $Nr = 1;
      $daten['SPEISERAUM'] = getOrtsname($daten['F_SPEISERAUM_ID'],0);
      // Sortieren nach Position
      array_multisort($ArtikelPos, SORT_ASC, SORT_NUMERIC, $Artikel, $ArtikelNr);
      foreach($Artikel as $key => $value) 
      {
        $daten['ESSEN'][0][$Nr] = $value;
        $attribute['ESSEN'][0][$Nr] = 'b';
        $x = 1;
        foreach ( $Essen as $datum => $menge )
        {
          $daten['ESSEN'][$x][0] = date('d.m.',$datum);
          $attribute['ESSEN'][$x][0] = 'i';
          if ( isset($menge[$ArtikelNr[$key]]) )
          {
            $daten['ESSEN'][$x][$Nr] = $menge[$ArtikelNr[$key]];
          }
          else
            $daten['ESSEN'][$x][$Nr] = '';
          $x++;
        }
        $Nr++;
      }
  }
  return $daten;
}

function InstitutionErsetzen($daten)
{
  global $Anredearten;
  // Institution überprüfen falls Buchung!
  if ( isset($daten['F_INSTITUTION']) && is_numeric($daten['F_INSTITUTION']) &&
       $daten['F_INSTITUTION']> 0 )
  {
    // statt Kundendaten die Daten der Institution übernehmen
    $query = sql_query('SELECT * FROM '.TABLE_ADRESSEN.' WHERE Adressen_id='.$daten['F_INSTITUTION']);
    if ( $adresse = sql_fetch_array($query) )
    {
      $daten['INSTITUTION'] = $adresse['Name'];
      $daten['ZUSATZ'] = $adresse['Zusatz'];
      $daten['STRASSE'] = $adresse['Strasse'];
      $daten['PLZ'] = $adresse['PLZ'];
      $daten['LAND'] = $adresse['Land'];
      $daten['ORT'] = $adresse['Ort'];
      $daten['TELEFON1'] = $adresse['Telefon1'];
      $daten['TELEFON2'] = $adresse['Telefon2'];
      $daten['FAX'] = $adresse['Fax'];
      $daten['I_NAME'] = $adresse['Name'];
      $daten['I_ANREDE'] = $Anredearten[$adresse['F_Anrede_id']];
      $daten['I_VORNAME'] = $adresse['Vorname'];
      $daten['I_TITEL'] = $adresse['Titel'];
      $daten['I_ZUSATZ'] = $adresse['Zusatz'];
      $daten['I_STRASSE'] = $adresse['Strasse'];
      $daten['I_PLZ'] = $adresse['PLZ'];
      $daten['I_LAND'] = $adresse['Land'];
      $daten['I_ORT'] = $adresse['Ort'];
      $daten['I_TELEFON1'] = $adresse['Telefon1'];
      $daten['I_EMAIL'] = $adresse['Email'];
      $daten['I_TELEFON2'] = $adresse['Telefon2'];
      $daten['I_FAX'] = $adresse['Fax'];
      $daten['VOLLNAME'] = $daten['ANREDE'].' ';
      if ( $daten['TITEL'] != '' )
        $daten['VOLLNAME'] .= $daten['TITEL'].' ';
      $daten['VOLLNAME'] .= $daten['NAME'];
    }
    sql_free_result($query);
  }
  else
  {
      $daten['VOLLNAME'] = $daten['ANREDE'].' ';
      if ( $daten['TITEL'] != '' )
        $daten['VOLLNAME'] .= $daten['TITEL'].' ';
      $daten['VOLLNAME'] .= $daten['VORNAME'].' '.$daten['NAME'];
      $daten['INSTITUTION'] = '';
      $daten['I_NAME'] = '';
      $daten['I_VORNAME'] = '';
      $daten['I_TITEL'] = '';
      $daten['I_ZUSATZ'] = '';
      $daten['I_STRASSE'] = '';
      $daten['I_PLZ'] = '';
      $daten['I_LAND'] = '';
      $daten['I_ORT'] = '';
      $daten['I_TELEFON1'] = '';
      $daten['I_TELEFON2'] = '';
      $daten['I_FAX'] = '';
      $daten['I_EMAIL'] = '';
  }
  return $daten;
}

function fuelleSchlafplaetze($daten = array() )
{
  // Berechnet die vorhandenen Schlafmöglichkeiten
  // muss nach DatenZusammenfassen aufgerufen werden
  global $Artikelarten;
  global $attribute;
  $daten['UNTERBRINGUNGART'] = array();
  $daten['UNTERBRINGUNGANZAHL'] = array();
  $daten['UNTERBRINGUNGPLAETZE'] = array();
  $daten['UNTERBRINGUNGPLAETZEANZAHL'] = array();
  $daten['UNTERBRINGUNGGESAMT'] = 0;
  $daten['UNTERBRINGUNGVONBIS'] = array();
//  $daten['SOLLANZAHLUNG'] = 0;
  $daten['ORTSKOSTEN'] = 0;
  if ( ! is_array($daten['ZARTIKELNR']) ) return $daten;

  $merkartikel = array();
  foreach ( $daten['ZARTIKELNR'] as $key => $artikelnr )
  {
    if ( is_numeric($artikelnr) )
    {
      if ( $daten['ZART'][$key] == $Artikelarten[CVJMART_ORT] &&
        $daten['ZGESAMT'] != OHNE_BERECHNUNG )
        {
          $daten['ORTSKOSTEN'] = AddPreis($daten['ORTSKOSTEN'],$daten['ZGESAMT'][$key], false);
        }
      $a = new Artikel($artikelnr);
      $sp = $a->BerechneSchlafplaetze();
    }
    else
      $sp = -1;
    if ( $sp >= 0 )
    {
        // Oberkategorie feststellen
        $bereichid = holeArtikelParentID($artikelnr);
        if ( ! isset($daten['UNTERBRINGUNGART'][$bereichid]) ||
             ! is_numeric($merkartikel[$bereichid]) ||
             $daten['VPREIS'][$merkartikel[$bereichid]] != $daten['ZPREIS'][$key] ||
             $daten['VDATUM'][$merkartikel[$bereichid]] != $daten['ZDATUM'][$key] ) // neu: nur bei gleichem Datum zusammenfassen
        {
          $bereichname = getOrtsname($bereichid);
          $daten['UNTERBRINGUNGART'][$bereichid] = $bereichname;
          if ( $sp != 0 )
            $daten['UNTERBRINGUNGPLAETZE'][$bereichid] = $sp;
          else
            $daten['UNTERBRINGUNGPLAETZE'][$bereichid] = '';
          $daten['UNTERBRINGUNGANZAHL'][$bereichid] = 0;
          $daten['UNTERBRINGUNGPLAETZEANZAHL'][$bereichid] = 0;
          $daten['VBEZEICHNUNG'][] = $bereichname;
          $daten['VDATUM'][] = $daten['ZDATUM'][$key];
          $daten['SORTVDATUM'][] = $daten['SORTZDATUM'][$key];
          $daten['VPREIS'][] = $daten['ZPREIS'][$key];
          $daten['VMWST'][] = $daten['ZMWST'][$key];
          if ( $daten['ZDAUER'][$key] > 0 )
            if ( $daten['ZUMENGE'][$key] != 1 )
              $daten['VEINHEIT'][] = $daten['ZDAUER'][$key].' h x '.$daten['ZUMENGE'][$key].
                ' '.$daten['ZUEINHEIT'][$key];
            else
              $daten['VEINHEIT'][] = $daten['ZDAUER'][$key].' h '.$daten['ZUEINHEIT'][$key];
          elseif ( $daten['ZUMENGE'][$key] > 1 )
            $daten['VEINHEIT'][] = ' x '.$daten['ZUMENGE'][$key].' '.$daten['ZUEINHEIT'][$key];
          else
            $daten['VEINHEIT'][] = $daten['ZUEINHEIT'][$key];
          $daten['VUMENGE'][] = $daten['ZUMENGE'][$key];
          $daten['VMENGE'][] = $daten['ZMENGE'][$key];
          $daten['VRABATT'][] = $daten['ZRABATT'][$key];
          $daten['VGESAMT'][] = $daten['ZGESAMT'][$key];
          $daten['VMWSTSUMME'][] = $daten['ZMWSTSUMME'][$key];
          $daten['VART'][] = $daten['ZART'][$key];
          $daten['VARTIKELNR'][] = $daten['ZARTIKELNR'][$key];
          if ( isset($attribute['ZBEZEICHNUNG'][$key]))
            $attribute['VBEZEICHNUNG'][] = $attribute['ZBEZEICHNUNG'][$key];
          else
            $attribute['VBEZEICHNUNG'][] = '';          
          // Position dieses Schlüssels merken
          $merkartikel[$bereichid] = Count($daten['VARTIKELNR'])-1;
          //echo $bereichname.'//'.$merkartikel[$bereichid];
        }
        else
        {
          // Artikel schon da - Zusammenfassen
          $daten['VMENGE'][$merkartikel[$bereichid]] += $daten['ZMENGE'][$key];
          if ( $daten['ZDAUER'][$key] > 0 )
            if ( $daten['ZUMENGE'][$key] != 1 )
              $daten['VEINHEIT'][$merkartikel[$bereichid]] = $daten['ZDAUER'][$key].
                ' h x '.$daten['ZUMENGE'][$key].
                ' '.$daten['ZUEINHEIT'][$key];
            else
              $daten['VEINHEIT'][$merkartikel[$bereichid]] = $daten['ZDAUER'][$key].' h '.
              $daten['ZUEINHEIT'][$key];
          elseif ( $daten['ZUMENGE'][$key] > 1 )
            $daten['VEINHEIT'][$merkartikel[$bereichid]] = ' x '.$daten['ZUMENGE'][$key].
            ' '.$daten['ZUEINHEIT'][$key];
          else
            $daten['VEINHEIT'][$merkartikel[$bereichid]] = $daten['ZUEINHEIT'][$key];
          $daten['VGESAMT'][$merkartikel[$bereichid]] =
            AddPreis($daten['VGESAMT'][$merkartikel[$bereichid]],$daten['ZGESAMT'][$key]);
          if ( ! isset($daten['VMWSTSUMME'][$merkartikel[$bereichid]]))
            $daten['VMWSTSUMME'][$merkartikel[$bereichid]] = $daten['ZMWSTSUMME'][$key];
          else
            $daten['VMWSTSUMME'][$merkartikel[$bereichid]] =
            AddPreis($daten['VMWSTSUMME'][$merkartikel[$bereichid]],$daten['ZMWSTSUMME'][$key]);
        }
        $daten['UNTERBRINGUNGANZAHL'][$bereichid] += 1;
        $daten['UNTERBRINGUNGPLAETZEANZAHL'][$bereichid] += $sp;
        $daten['UNTERBRINGUNGGESAMT'] += $sp;
        if ( !isset($daten['UNTERBRINGUNGVONBIS'][$bereichid]) ||
           $daten['ZDATUM'][$key] > $daten['UNTERBRINGUNGVONBIS'][$bereichid] )
            $daten['UNTERBRINGUNGVONBIS'][$bereichid] = $daten['ZDATUM'][$key];
    }
    else
    {
        // irgendwas was kein Schlafort ist
        $daten['VBEZEICHNUNG'][] = $daten['ZBEZEICHNUNG'][$key];
        $daten['VDATUM'][] = $daten['ZDATUM'][$key];
        $daten['SORTVDATUM'][] = $daten['SORTZDATUM'][$key];
        $daten['VPREIS'][] = $daten['ZPREIS'][$key];
        $daten['VMWST'][] = $daten['ZMWST'][$key];
        $daten['VEINHEIT'][] = $daten['ZEINHEIT'][$key];
        $daten['VUMENGE'][] = $daten['ZUMENGE'][$key];
        $daten['VMENGE'][] = $daten['ZMENGE'][$key];
        $daten['VRABATT'][] = $daten['ZRABATT'][$key];
        $daten['VGESAMT'][] = $daten['ZGESAMT'][$key];
        $daten['VMWSTSUMME'][] = $daten['ZMWSTSUMME'][$key];
        $daten['VART'][] = $daten['ZART'][$key];
        $daten['VARTIKELNR'][] = $daten['ZARTIKELNR'][$key];
        if ( isset($attribute['ZBEZEICHNUNG'][$key]))
          $attribute['VBEZEICHNUNG'][] = $attribute['ZBEZEICHNUNG'][$key];
        else
          $attribute['VBEZEICHNUNG'][] = '';
        // Sonderbehandlung Orte: Aufführung bei Unterbringung
        if ( $daten['ZART'][$key] == $Artikelarten[CVJMART_ORT] )
        {
          $daten['UNTERBRINGUNGANZAHL'][$artikelnr] = 1;
          if ( ! isset($daten['UNTERBRINGUNGVONBIS'][$artikelnr]))
            $daten['UNTERBRINGUNGVONBIS'][$artikelnr] = $daten['ZDATUM'][$key];
          elseif ( $daten['ZDATUM'][$key] > $daten['UNTERBRINGUNGVONBIS'][$artikelnr] )
            $daten['UNTERBRINGUNGVONBIS'][$artikelnr] = $daten['ZDATUM'][$key];
          $daten['UNTERBRINGUNGART'][$artikelnr] = $daten['ZBEZEICHNUNG'][$key];
          $daten['UNTERBRINGUNGPLAETZE'][$artikelnr] = '';
          $daten['UNTERBRINGUNGPLAETZEANZAHL'][$artikelnr] = '';
        }
    }
  }
  $daten['ORTSKOSTEN'] = Preis($daten['ORTSKOSTEN']);
  return $daten;
}

function fuelleArtikel($daten = array() )
{
  global $attribute;
  if ( ! is_array($daten) ) return $daten;
  $daten['ARTIKELBEZEICHNUNG'] = array();
  $art = '';
  foreach ( $daten['BEZEICHNUNG'] as $key => $bezeichnung )
  {
    if ( $art != $daten['ART'][$key] )
    {
      $art = $daten['ART'][$key];
      $daten['ARTIKELBEZEICHNUNG'][] = $art;
      $daten['ARTIKELPREIS'][] = '';
      $daten['ARTIKELPREISH'][] = '';
      $daten['ARTIKELEINHEIT'][] = '';
      $attribute['ARTIKELBEZEICHNUNG'][] = 'b';
    }
    $daten['ARTIKELBEZEICHNUNG'][] = $bezeichnung;
    $daten['ARTIKELEINHEIT'][] = $daten['EINHEIT'][$key];
    $daten['ARTIKELPREIS'][] = $daten['PREIS'][$key];
    $daten['ARTIKELPREISH'][] = $daten['PREISSTUNDE'][$key];
    $attribute['ARTIKELBEZEICHNUNG'][] = '';
  }
  return $daten;
}

function fuelleProgramme($daten = array() )
{
  // Berechnet die vorhandenen Programme
  // muss nach DatenZusammenfassen aufgerufen werden
  global $Artikelarten;
  $daten['PROGRAMMDATUM'] = array();
  $daten['PROGRAMMUHRZEIT'] = array();
  $daten['PROGRAMMINHALT'] = array();
  $daten['PROGRAMMKOSTEN'] = 0;
  if ( ! is_array($daten['ZARTIKELNR']) ) return $daten;

  $programm1 = array();
  $programm2 = array();
  foreach ( $daten['ZARTIKELNR'] as $key => $artikelnr )
  {
    if ( is_numeric($artikelnr) )
    {
      if ( $daten['ZART'][$key] == $Artikelarten[CVJMART_PROGRAMM] )
      {
        $daten['PROGRAMMKOSTEN'] = AddPreis($daten['PROGRAMMKOSTEN'], $daten['ZGESAMT'][$key]);
        // Oberkategorie feststellen
        $bereichid = holeArtikelParentID($artikelnr);
        $programm = getOrtsname($bereichid);
        $datum = explode(' ',$daten['ZDATUM'][$key]);
        $tag = explode('.',$datum[0]);
        $zeit = explode(':',$datum[1]);
        $programm1[] = mktime($zeit[0],$zeit[1],0,$tag[1],$tag[0],$tag[2]);
        $programm2[] = $programm.': '.$daten['ZBEZEICHNUNG'][$key];
      }
    }
  }
  array_multisort($programm1, SORT_NUMERIC, $programm2);
  foreach ( $programm1 as $key => $value )
  {
    $daten['PROGRAMMDATUM'][] = date('d.m.Y',$value);
    if ( date('H:i',$value) != '00:00')
      $daten['PROGRAMMUHRZEIT'][] = date('H:i',$value);
    else
      $daten['PROGRAMMUHRZEIT'][] = '';
    $daten['PROGRAMMINHALT'][] = $programm2[$key];
  }
  $daten['PROGRAMMKOSTEN'] = Preis($daten['PROGRAMMKOSTEN']);
  return $daten;
}

function berechneGesamtsumme($daten = array() )
{
  $daten['GESAMTPREIS'] = 0;
  $daten['GESAMTMWST'] = 0;
  $daten['EINZELMWST'] = array();
  $daten['EINZELMWSTSUMME'] = array();
  $werte = array();
  foreach ( $daten['ZGESAMT'] as $key => $value )
  {
    //if ( is_numeric($daten['ZGESAMT'][$key]) )
    {
      // Preis rückgüngig machen
      $daten['GESAMTPREIS'] = AddPreis($daten['GESAMTPREIS'],$daten['ZGESAMT'][$key],false);
      $daten['GESAMTMWST'] = AddPreis($daten['GESAMTMWST'],$daten['ZMWSTSUMME'][$key],false);
      if ( ! isset($werte[$daten['ZMWST'][$key]]) )
        $werte[$daten['ZMWST'][$key]] = $daten['ZMWSTSUMME'][$key];
      else
        $werte[$daten['ZMWST'][$key]] = AddPreis(
          $werte[$daten['ZMWST'][$key]],
          $daten['ZMWSTSUMME'][$key]);
    }
  }
  foreach ( $werte as $satz => $wert )
  {
    if ( $satz != 0 )
    {
      $daten['EINZELMWST'][] = $satz;
      $daten['EINZELMWSTSUMME'][] = $wert;
    }
  }
  $daten['GESAMTNETTO'] = AddPreis($daten['GESAMTPREIS'],$daten['GESAMTMWST'], false);
  if ( isset($daten['VORHANZAHLUNG']) && isset($daten['BARZAHLUNG']) )
  {
    if (! is_numeric($daten['VORHANZAHLUNG']) ) $daten['VORHANZAHLUNG'] = 0;
    if (! is_numeric($daten['BARZAHLUNG']) ) $daten['BARZAHLUNG'] = 0;
    $daten['RESTSUMMEBRUTTO'] = AddPreis($daten['GESAMTPREIS'], '-'.$daten['VORHANZAHLUNG']);
    $daten['RESTSUMMEBRUTTO'] = AddPreis($daten['RESTSUMMEBRUTTO'],'-'.$daten['BARZAHLUNG'],false);
    $daten['RESTSUMME'] = AddPreis($daten['GESAMTNETTO'],'-'.$daten['VORHANZAHLUNG']);
    $daten['RESTSUMME'] = AddPreis($daten['RESTSUMME'], '-'.$daten['BARZAHLUNG'],false);
    $daten['RESTSUMMEBRUTTO3'] = Preis(str_replace(',','.',$daten['RESTSUMMEBRUTTO'])*1.03,false);
    $daten['RESTSUMMEBRUTTO5'] = Preis(str_replace(',','.',$daten['RESTSUMMEBRUTTO'])*1.05,false);
    $daten['RESTSUMMEBRUTTO7'] = Preis(str_replace(',','.',$daten['RESTSUMMEBRUTTO'])*1.07,false);
    $daten['SKONTO3'] = Preis(str_replace(',','.',$daten['RESTSUMMEBRUTTO'])*0.03,false);
    $daten['SKONTO5'] = Preis(str_replace(',','.',$daten['RESTSUMMEBRUTTO'])*0.05,false);
    $daten['SKONTO7'] = Preis(str_replace(',','.',$daten['RESTSUMMEBRUTTO'])*0.07,false);

    $daten['RESTSUMME3'] = Preis(str_replace(',','.',$daten['RESTSUMME'])*1.03,false);
    $daten['RESTSUMME5'] = Preis(str_replace(',','.',$daten['RESTSUMME'])*1.05,false);
    $daten['RESTSUMME7'] = Preis(str_replace(',','.',$daten['RESTSUMME'])*1.07,false);
    $daten['SKONTOS3'] = Preis(str_replace(',','.',$daten['RESTSUMME'])*0.03,false);
    $daten['SKONTOS5'] = Preis(str_replace(',','.',$daten['RESTSUMME'])*0.05,false);
    $daten['SKONTOS7'] = Preis(str_replace(',','.',$daten['RESTSUMME'])*0.07,false);
    if ( $daten['ZAHLUNGSZIEL'] == 1 )
      $daten['ZAHLUNGSZIEL'] = 'Zahlbar innerhalb von 7 Tagen rein Restsumme, '.
        'danach zahlbar bis zum '.$daten['ZAHLDATUM'].' '.$daten['RESTSUMME5'].' Euro.';
    else
      $daten['ZAHLUNGSZIEL'] = '';
    $daten['VORHANZAHLUNG'] = Preis($daten['VORHANZAHLUNG'],false);
    $daten['BARZAHLUNG'] = Preis($daten['BARZAHLUNG'],false);
    $daten['RESTSUMME'] = Preis($daten['RESTSUMME'],false);
    $daten['RESTSUMMEBRUTTO'] = Preis($daten['RESTSUMMEBRUTTO'],false);
  }
  $daten['GESAMTNETTO'] = Preis($daten['GESAMTNETTO'],false);
  $daten['GESAMTPREIS'] = Preis($daten['GESAMTPREIS'],false);
  $daten['GESAMTMWST'] = Preis($daten['GESAMTMWST'],false);
  return $daten;
}

function fuelleDaten($sql, $daten = array() )
{
  global $Anredearten;
  global $Altersgruppen;
  global $Buchungsstatus;
  $d = strtotime('+14 days');
  if ( date('w',$d) == 0 ) $d = strtotime('+15 days');
  if ( date('w',$d) == 6 ) $d = strtotime('+16 days');
  $daten['ERSTELLDATUM'] = date('d.m.Y');
  $daten['NEUDATUM'] = date('d.m.Y', $d);
  $daten['GESAMTPREIS'] = 0;
  $daten['GESAMTMWST'] = 0;
  $daten['GESAMTRABATT'] = 0;
  if ( ! $query = sql_query($sql) )
    die('Fehler ('.$sql.'): '.sql_error());
  // mehr als ein Datensatz - in Felder!
  while ( $ds = sql_fetch_array($query) )
  {
    foreach ( $ds as $key => $value )
    {
      // besondere Felder füllen
      if ( ! is_numeric($key) )
      switch ( $key )
      {        
      	case 'Rechnungsdatum':
          $daten['ZAHLDATUM'] = date('d.m.Y',strtotime('+14 day',$value));
        case 'Von':
        case 'Bis':
          if ( $key == 'Von' || $key == 'Bis' ) 
            $daten['UEBERNACHTUNGEN'] = berechneUebernachtungen($ds);
        case 'ReVon':
        case 'ReBis':
          if ( $key == 'ReVon' || $key == 'ReBis' )
            $daten['UEBERNACHTUNGENBERECHNET'] = berechneUebernachtungen($ds, 'ReVon', 'ReBis');
        case 'SeminarGeburtsdatum':
          if ( $key == 'SeminarGeburtsdatum' )
          {
          	if ( $value != 0)
              $daten['SEMINARALTER'][] = floor((time()-$value)/31536000);
            else
              $daten['SEMINARALTER'][] = 'unbekannt';
          }
        case 'Geburtsdatum':
          if ( $key == 'Geburtsdatum' && $value != 0)
            $daten['ALTER'] = floor((time()-$value)/31536000);
        case 'Eingang':
        case 'Datum':
        case 'Rechnungstellung':
        case 'Abnahme':
        case 'Anmeldeschluss':
        case 'GueltigAb':
          if ( $value != 0 )
          {
            $dat = date('d.m.y', $value);
            if ( date('H:i',$value) != '00:00' )
            {
              $dat .= ' '.date('H:i',$value);
            }
          }
          else
            $dat = '';
          if ( sql_num_rows($query) > 1 )
          {
            if ( isset($daten[strtoupper($key)]) && ! is_array($daten[strtoupper($key)]) )
            {
              $daten[strtoupper($key)] = array();
              $daten['SORT'.strtoupper($key)] = array();
            }
            $daten[strtoupper($key)][] = $dat;
            $daten['SORT'.strtoupper($key)][] = $value;
          }
          else
          {
            $daten[strtoupper($key)] = $dat;
            $daten['SORT'.strtoupper($key)] = $value;
          }
          break;
        case 'Ankunftszeit':
        case 'Abfahrtszeit':
          $zeit = '';
          if ( $value != 0 ) $zeit = date('H:i', $value);
          if ( sql_num_rows($query) > 1 )
            $daten[strtoupper($key)][] = $zeit;
          else
            $daten[strtoupper($key)] = $zeit;
          break;
        case 'Anrede':
        case 'F_Anrede_id':
          if ( is_numeric($value) ) $value = $Anredearten[$value];
          if ( $value == 'Frau' || $value == 'Liebe' )
          {
            $Anrede = 'Sehr geehrte Frau';
            $dasR = '';
          }
          elseif ( $value == 'Herr' || $value == 'Lieber' )
          {
            $dasR = 'r';
            $Anrede = 'Sehr geehrter Herr';
          }
          else
          {
            $dasR = '';
            $Anrede = 'Sehr geehrte Damen und Herren';
          }
          if ( sql_num_rows($query) > 1 )
          {
            $daten['ANREDE'][] = $value;
            $daten['ANREDE-V'][] = $Anrede;
            $daten['ANREDE-R'][] = $dasR;
          }
          else
          {
            $daten['ANREDE'] = $value;
            $daten['ANREDE-V'] = $Anrede;
            $daten['ANREDE-R'] = $dasR;
          }
          break;
        case 'Logtext':
          $pos = strrpos($value,"\n");
          if ( $pos === false ) $pos = -1;
            $daten['ANFRAGEDATUM'] = substr($value,$pos+1,10);
          break;
        case 'Rechnung':
          $daten['RECHNUNG'] = $value;
          switch ( $value )
          {
            case 1:
              $daten['BELEGART'] = 'Rechnung';
              $daten['DOKUMENTNR'] = 'R-'.$ds['Rechnungsnummer'];
              break;
            case 2:
              $daten['BELEGART'] = 'Vorreservierung';
              $daten['DOKUMENTNR'] = 'V-'.$ds['Rechnung_id'];
              break;
            case 3:
              $daten['BELEGART'] = 'Reservierungsbestätigung';
              $daten['DOKUMENTNR'] = 'B-'.$ds['Rechnung_id'];
              break;
            default:
              $daten['BELEGART'] = 'Angebot';
              $daten['DOKUMENTNR'] = 'A-'.$ds['Rechnung_id'];
          }
          break;
        case 'F_Kunden_Nr':
          if ( sql_num_rows($query) > 1 )
          {
            $daten['F_KUNDEN_NR'][] = $value;
            if ( ! isset($daten['KUNDEN_NR'][Count($daten['F_KUNDEN_NR'])-1]))
              $daten['KUNDEN_NR'][] = $value;
          }
          else
          {
            if ( ! isset($daten['KUNDEN_NR']) ) $daten['KUNDEN_NR'] = $value;
            $daten['F_KUNDEN_NR'] = $value;
          }
          break;
        case 'F_Buchung_Nr':
          if ( sql_num_rows($query) > 1 )
          {
            $daten['F_BUCHUNG_NR'][] = $value;
            if ( is_array($daten['BUCHUNG_NR']) || ! isset($daten['BUCHUNG_NR']) )
              $daten['BUCHUNG_NR'][] = $value;
          }
          else
          {
            $daten['F_BUCHUNG_NR'] = $value;
            $daten['BUCHUNG_NR'] = $value;
          }
          break;
        case 'Status':
          if ( sql_num_rows($query) > 1 )
            $daten['STATUS'][] = $Buchungsstatus[$value];
          else
            $daten['STATUS'] = $Buchungsstatus[$value];
          break;
        case 'Altergruppe':
          if ( sql_num_rows($query) > 1 )
            $daten['ALTERSGRUPPE'][] = $Altersgruppen[$value];
          else
            $daten['ALTERSGRUPPE'] = $Altersgruppen[$value];
          break;
        case 'PreisStunde':
           if ( sql_num_rows($query) > 1 )
             $daten['PREISSTUNDE'][] = Preis($ds['PreisStunde']);
           else
             $daten['PREISSTUNDE'] = Preis($ds['PreisStunde']);
           break;
        case 'F_Preis':
        case 'Preis':
          if ( (! isset($ds['F_Preis']) || is_null($ds['F_Preis'])) &&
               isset($ds['F_Artikel_Nr']) && is_numeric($ds['F_Artikel_Nr']) && is_numeric($ds['F_Preisliste_id']) &&
               ! isset($ds['PreisStunde']) )
            $Preis = holePreis($ds['F_Artikel_Nr'], $ds['F_Preisliste_id']);
          else
            $Preis = $ds[$key];
          if ( sql_num_rows($query) > 1 )
          {
            $daten[strtoupper($key)][] = Preis($Preis);
            if ( ! isset($ds['Preis']) )
              $daten['PREIS'][] = Preis($Preis);
            if ( isset($ds['Menge']) && isset($ds['Rabatt']) )
            {
              	$derPreis = berechnePreis($Preis,$ds['Menge'],$ds['Dauer'],$ds['Rabatt']);
          		$daten['GESAMTRABATT'] = AddPreis($daten['GESAMTRABATT'],
            		berechnePreis($Preis,$ds['Menge'],$ds['Dauer'])*$ds['Rabatt']/100);
            	$daten['GESAMT'][] = Preis($derPreis);
              // TODO
              $daten['MWSTSUMME'][] = Preis(berechnePreis($Preis,$ds['Menge'],$ds['Dauer'],
                 $ds['Rabatt'])*$ds['MWSt']/100);
              $daten['GESAMTMWST'] += sprintf('%01.2f',berechnePreis($Preis,$ds['Menge'],
                $ds['Dauer'],$ds['Rabatt'])*$ds['MWSt']/100,2);
              $daten['GESAMTPREIS'] += sprintf('%01.2f',berechnePreis($Preis,$ds['Menge'],
                $ds['Dauer'],$ds['Rabatt']),2);
            }
          }
          else
          {
            $daten[strtoupper($key)] = Preis($Preis);
            if ( ! isset($ds['Preis']) )
              $daten['PREIS'] = Preis($Preis);
            if ( isset($ds['Menge']) && isset($ds['Rabatt']) && isset($ds['Dauer']) )
            {
            	$derPreis = berechnePreis($Preis,$ds['Menge'],$ds['Dauer'],$ds['Rabatt']);
            	$daten['GESAMT'] = $derPreis;
              // TODO
              $daten['MWSTSUMME'] = Preis($daten['GESAMT']*$ds['MWSt']/100);
              $daten['GESAMTPREIS'] += $daten['GESAMT'];
              $daten['GESAMTMWST'] += sprintf('%01.2f',$daten['GESAMT']*$ds['MWSt']/100);
              $daten['GESAMT'] = Preis($daten['GESAMT']);
            }
          }
          break;
        case 'F_Bezeichnung':
        case 'Bezeichnung':
          if ( isset($ds['F_Bezeichnung']) && ! is_null($ds['F_Bezeichnung']) &&
               $ds['F_Bezeichnung'] != '' )
          {
            if ( sql_num_rows($query) > 1 )
              $daten[strtoupper($key)][] = $ds['F_Bezeichnung'];
            else
              $daten[strtoupper($key)] = $ds['F_Bezeichnung'];
          }
          else
            if ( sql_num_rows($query) > 1 )
              $daten[strtoupper($key)][] = $ds['Bezeichnung'];
            else
              $daten[strtoupper($key)] = $ds['Bezeichnung'];
          break;
        case 'Hinweise':
		case 'Vereinbarungen':
        	if ( $value != '' ) $daten[strtoupper($key).'DA'] = '1';
        default:
          // New-Line in Zeilenumbruch umwandeln
          if ( sql_num_rows($query) > 1 )
          {
            if ( isset($daten[strtoupper($key)]) && ! is_array($daten[strtoupper($key)]) )
              $daten[strtoupper($key)] = array();
            $daten[strtoupper($key)][] = $value;
          }
          else
          {
            $daten[strtoupper($key)] = $value;
          }
      } // switch
      $daten['DATUMAKTUELL'] = date('d.m.Y');
    } // foreach
    // Privat-Daten übernehmen (falls noch eine Institution kommt)
    if ( isset($daten['NAME']) )
    {
      $daten['P_ZUSATZ'] = $daten['ZUSATZ'];
      $daten['P_NAME'] = $daten['NAME'];
      $daten['P_ANREDE'] = $daten['ANREDE'];
      $daten['P_TITEL'] = $daten['TITEL'];
      if ( isset($daten['EMAIL'])) $daten['P_EMAIL'] = $daten['EMAIL'];
      $daten['P_VORNAME'] = $daten['VORNAME'];
      $daten['P_STRASSE'] = $daten['STRASSE'];
      $daten['P_PLZ'] = $daten['PLZ'];
      if ( isset($daten['LAND'])) $daten['P_LAND'] = $daten['LAND'];
      $daten['P_ORT'] = $daten['ORT'];
      if ( isset($daten['TELEFON1'])) $daten['P_TELEFON1'] = $daten['TELEFON1'];
      if ( isset($daten['TELEFON2'])) $daten['P_TELEFON2'] = $daten['TELEFON2'];
      if ( isset($daten['FAX'])) $daten['P_FAX'] = $daten['FAX'];
    }
    if ( isset($daten['PERSONEN1']))
    {
      $daten['PERSONENGESAMT'] = Personenanzahl($ds);
      $daten['PERSONENGESAMTM'] = 0;      
      for ( $i = 0; $i < 7; $i++ ) 
      {
        if ( isset($daten['PERSONEN'.$i]))   
        	$daten['PERSONENGESAMTM'] += $daten['PERSONEN'.$i];
      }
      $daten['PERSONENGESAMTW'] = 0;
      for ( $i = 0; $i < 7; $i++ )
      {
         if ( isset($daten['PERSONEN'.$i.'W']))
           $daten['PERSONENGESAMTW'] += $daten['PERSONEN'.$i.'W'];
      }
      $daten['BETREUERANZAHL'] = $daten['BETREUERANZAHLW'] + $daten['BETREUERANZAHLM'];
      $daten['TEILNEHMERGESAMT'] = $daten['PERSONENGESAMT'] - $daten['BETREUERANZAHL'];
      $daten['TEILNEHMERGESAMTW'] = $daten['PERSONENGESAMTW'] - $daten['BETREUERANZAHLW'];
      $daten['TEILNEHMERGESAMTM'] = $daten['PERSONENGESAMTM'] - $daten['BETREUERANZAHLM'];
    }
  } // while
  sql_free_result($query);
  // Wenn der Preis null ist gibt es auch keinen Rabatt!
  if ( isset($daten['RABATT']) && is_array($daten['RABATT']) )
  {
    foreach($daten['RABATT'] as $key => $value )
      if ( $daten['PREIS'][$key] == 0 || ! is_numeric($daten['RABATT'][$key]) )
        $daten['RABATT'][$key] = 0;
  }
  return $daten;
}

function DatenZusammenfassen($daten)
{
  global $attribute;
  global $Artikelarten;
  // Zusammenfassung Endbearbeitung
  if ( isset($daten['MENGE']) && isset($daten['DATUM']) && isset($daten['ART']) &&
       (isset($daten['BEZEICHNUNG']) || isset($daten['F_BEZEICHNUNG']))
       && is_array($daten['DATUM']) )
  {
    // Zusammenfassung erzeugen
    $LetzteMenge = 0;
    $LetztesDatum = 0;
    $Rabatt = 0;
    $MWSt = 0;
    $Menge = 0;
    $Preis = 0;
    $Dauer = 0;
    $Startdatumzahl = 0;
    $LetzterArtikel = '';
    $Artikelnr = 0;
    $LetzteArt = '';
    $Einheit = '';
    $VonDatum = 0;
    $BisDatum = 0;
    $Gesamtpreis = 0;
    foreach ( $daten['DATUM'] as $key => $value ) // die Datumszahl verwenden
    {
      if ( ! isset($daten['BEZEICHNUNG'][$key]))
        $daten['BEZEICHNUNG'][$key] = $daten['F_BEZEICHNUNG'][$key];
      if ( ! isset($daten['PREIS'][$key]))
        $daten['PREIS'][$key] = $daten['F_PREIS'][$key];
      $datum = explode('.',$BisDatum);
      if ( Count($datum) == 3 )
      {
        if ( strpos($datum[2]," ") !== false )
        {
           $datum[2] = substr($datum[2], 0, strpos($datum[2]," "));
        }
        $datum = mktime(0,0,0,$datum[1],$datum[0],$datum[2]);
      }
      else
        $datum = 0;
      $ndatum = explode('.',$daten['DATUM'][$key]);
      if ( strpos($ndatum[2]," ") !== false )
      {
        $ndatum[2] = substr($ndatum[2], 0, strpos($ndatum[2]," "));
      
      }
      $ndatum = mktime(0,0,0,$ndatum[1],$ndatum[0],$ndatum[2]);
      // Bei Orten: Erlaube das Zusammenfassen bei unterschiedlichen Mengen
      // damit '0' für die Übernachtungszählung berücksichtigt wird
      if ( $LetzterArtikel == $daten['BEZEICHNUNG'][$key] &&
           strtotime('-1 day',$ndatum) == $datum &&
           ($LetzteMenge == $daten['MENGE'][$key] || $LetzteMenge == 0 ) &&
           $Preis == $daten['PREIS'][$key] &&
           $Einheit == $daten['EINHEIT'][$key] &&
           $Dauer == $daten['DAUER'][$key] &&
           $Rabatt == $daten['RABATT'][$key] )
      {
        $BisDatum = $value;
        if ( is_numeric($daten['MENGE'][$key]) )
          $Menge += $daten['MENGE'][$key];
        if ( $LetzteMenge == 0 )
        {
          // Sonderfall: Wenn Menge = 0 dann speichern der 'nicht-0' Eintragsdaten
          $LetzteMenge = $daten['MENGE'][$key];
        }
      }
      else
      {
        if ( $VonDatum != 0 )
        {
          // Veränderung - bisherigen Stand abspeichern
          if ( $VonDatum != $BisDatum )
          {
            $d1 = explode('.',$VonDatum);
            $d2 = explode('.',$BisDatum);
            $VonDatum = $d1[0].'.'.$d1[1].'. bis '.$d2[0].'.'.$d2[1].'.'.$d2[2];
          }
          else
          {
            $d1 = explode('.',$VonDatum);
            $VonDatum = $d1[0].'.'.$d1[1].'.'.$d1[2];
          }
          $daten['ZBEZEICHNUNG'][] = $LetzterArtikel;
          $daten['ZDATUM'][] = $VonDatum;
          $daten['ZPREIS'][] = Preis($Preis);
          $daten['ZMWST'][] = $MWSt;
          if ( $LetzteMenge != 0 )
            $einzelmenge = number_format($Menge/$LetzteMenge,0);
          else
            $einzelmenge = 0;
          // Prüfen: Bei Essen und Unterbringung Einheit ändern:
          // Anzahl/Tag*'Personen'
          if ( ($LetzteArt == $Artikelarten[CVJMART_VERPFLEGUNG] ||
               $LetzteArt == $Artikelarten[CVJMART_PAUSCHALEKOPF] ||
               $LetzteArt == $Artikelarten[CVJMART_PAUSCHALEKOPFNACHT] ||
               $LetzteArt == $Artikelarten[CVJMART_VERPFLEGUNGPAUSCHAL] 
               )
               && $Artikelnr != CVJM_SELBSTVERPFLEGUNG )
          {
            if ( $LetzteMenge > 1 ) $dieEinheit = 'Personen'; else $dieEinheit = 'Person';
            if ( $einzelmenge <= 1 )
            {
              $daten['ZMENGE'][] = $LetzteMenge;
              $daten['ZUMENGE'][] = $einzelmenge;
              if ( $Dauer != 0 )
              {
                $daten['ZEINHEIT'][] = $Dauer.' h '.$dieEinheit;
              }
              else
              {
                $daten['ZEINHEIT'][] = $dieEinheit;
              }
            }
            else
            {
              $daten['ZMENGE'][] = $einzelmenge;
              $daten['ZUMENGE'][] = $LetzteMenge;
              if ( $Dauer != 0 )
              {
                $daten['ZEINHEIT'][] = $Dauer.' h x '.$LetzteMenge.' '.$dieEinheit;
              }
              else
              {
                $daten['ZEINHEIT'][] = ' x '.$LetzteMenge.' '.$dieEinheit;
              }
            }
            $daten['ZUEINHEIT'][] = $dieEinheit;
          }
          else
          {
            if ( $einzelmenge == 1 )
            {
              $daten['ZMENGE'][] = $LetzteMenge;
              $daten['ZUMENGE'][] = 1;
              if ( $Dauer != 0 )
                $daten['ZEINHEIT'][] = $Dauer.' h '.$Einheit;
              else
                $daten['ZEINHEIT'][] = $Einheit;
            }
            else
            {
              $daten['ZUMENGE'][] = $einzelmenge;
              $daten['ZMENGE'][] = $LetzteMenge; // war Menge
              if ( $LetzteMenge != 0 )
              {
                if ( $Dauer != 0 )
                  $daten['ZEINHEIT'][] = $Dauer.' h x '.$einzelmenge.' '.$Einheit;
                else
                  $daten['ZEINHEIT'][] = ' x '.$einzelmenge.' '.$Einheit;
              }
              else
                $daten['ZEINHEIT'][] = $Einheit;
            }
            $daten['ZUEINHEIT'][] = $Einheit;
          }
          $daten['SORTZDATUM'][] = $Startdatumzahl;
          $daten['ZRABATT'][] = $Rabatt;
          $daten['ZDAUER'][] = $Dauer;
          $daten['ZGESAMT'][] = Preis(berechnePreis(PreisToZahl($Preis),$Menge,$Dauer,$Rabatt));
          $daten['ZMWSTSUMME'][] = Preis(berechnePreis(
                                         PreisToZahl($Preis),$Menge,$Dauer,$Rabatt)*$MWSt/100);
          $daten['ZART'][] = $LetzteArt;
          $daten['ZARTIKELNR'][] = $Artikelnr;
          $attribute['ZBEZEICHNUNG'][] = '';
        }
        // Neuer Anfang
        if ( $LetzteArt != $daten['ART'][$key])
        {
          // Art-Überschrift
          $LetzteArt = $daten['ART'][$key];
          $daten['SORTZDATUM'][] = 0;
          $daten['ZDATUM'][] = '';
          $daten['ZPREIS'][] = '';
          $daten['ZMWST'][] = '0';
          $daten['ZMWSTSUMME'][] = '0';
          $daten['ZEINHEIT'][] = '';
          $daten['ZUEINHEIT'][] = '';
          $daten['ZRABATT'][] = '0';
          $daten['ZGESAMT'][] = '';
          $daten['ZMENGE'][] = '';
          $daten['ZUMENGE'][] = '';
          $daten['ZART'][] = '';
          $daten['ZDAUER'][] = '';
          $daten['ZBEZEICHNUNG'][] = $LetzteArt;
          $daten['ZARTIKELNR'][] = '';
          // Überschriften sollen fett gedruckt werden
          $attribute['ZBEZEICHNUNG'][] = 'b';
        }
        $Startdatumzahl = $daten['SORTDATUM'][$key];
        $VonDatum = $value;
        $BisDatum = $VonDatum;
        $Einheit = $daten['EINHEIT'][$key];
        $Artikelnr = $daten['F_ARTIKEL_NR'][$key];
        $Preis = $daten['PREIS'][$key];
        $Dauer = $daten['DAUER'][$key];
        $PreisZahl = str_replace(',','.',$Preis);
        if ( ! is_numeric($PreisZahl) ) $PreisZahl = 0;
        $MWSt = $daten['MWST'][$key];
        $Rabatt = $daten['RABATT'][$key];
        $LetzterArtikel = $daten['BEZEICHNUNG'][$key];
        $LetzteMenge = $daten['MENGE'][$key];
        $Menge = $LetzteMenge;
      }
    } // foreach
    reset($daten['DATUM']);
    // änderungen des letzten Eintrages abspeichern
    if ( $VonDatum != $BisDatum )
    {
      $d1 = explode('.',$VonDatum);
      $d2 = explode('.',$BisDatum);
      $VonDatum = $d1[0].'.'.$d1[1].'. bis '.$d2[0].'.'.$d2[1].'.'.$d2[2];
    }
    else
    {
      $d1 = explode('.',$VonDatum);
      $VonDatum = $d1[0].'.'.$d1[1].'.';
    }
    $daten['ZBEZEICHNUNG'][] = $LetzterArtikel;
    $daten['ZDATUM'][] = $VonDatum;
    $daten['ZPREIS'][] = Preis($PreisZahl);
    $daten['ZMWST'][] = $MWSt;
    $daten['ZDAUER'][] = $Dauer;
    if ( $LetzteMenge != 0 )
            $einzelmenge = number_format($Menge/$LetzteMenge,0);
    else
            $einzelMenge = 0;
    // Prüfen: Bei Essen und Unterbringung Einheit ändern:
    // Anzahl/Tag*'Personen'
    if ( ($LetzteArt == $Artikelarten[CVJMART_VERPFLEGUNG] ||
               $LetzteArt == $Artikelarten[CVJMART_PAUSCHALEKOPF] ||
               $LetzteArt == $Artikelarten[CVJMART_PAUSCHALEKOPFNACHT] ||
               $LetzteArt == $Artikelarten[CVJMART_VERPFLEGUNGPAUSCHAL] 
               //$LetzteArt == $Artikelarten[CVJMART_VERPFLEGUNGNACHT]
               )
               && $Artikelnr != CVJM_SELBSTVERPFLEGUNG )
    {
      if ( $LetzteMenge > 1 ) $dieEinheit = 'Personen'; else $dieEinheit = 'Person';
      if ( $einzelmenge == 1 )
      {
              $daten['ZMENGE'][] = $LetzteMenge;
              $daten['ZUMENGE'][] = 1;
              if ( $Dauer != 0 )
              {
                $daten['ZEINHEIT'][] = $Dauer.' h '.$dieEinheit;
              }
              else
              {
                $daten['ZEINHEIT'][] = $dieEinheit;
              }
      }
      else
      {
              $daten['ZMENGE'][] = $einzelmenge;
              $daten['ZUMENGE'][] = $LetzteMenge;
              if ( $Dauer != 0 )
              {
                $daten['ZEINHEIT'][] = $Dauer.' h x '.$LetzteMenge.' '.$dieEinheit;
              }
              else
              {
                $daten['ZEINHEIT'][] = ' x '.$LetzteMenge.' '.$dieEinheit;
              }
      }
      $daten['ZUEINHEIT'][] = $dieEinheit;
    }
    else
    {
            if ( $einzelmenge == 1 )
            {
              $daten['ZMENGE'][] = $LetzteMenge;
              $daten['ZUMENGE'][] = 1;
              if ( $Dauer != 0 )
                $daten['ZEINHEIT'][] = $Dauer.' h '.$Einheit;
              else
                $daten['ZEINHEIT'][] = $Einheit;
            }
            else
            {
              $daten['ZUMENGE'][] = $einzelmenge;
              $daten['ZMENGE'][] = $LetzteMenge; // war Menge
              if ( $LetzteMenge != 0 )
              {
                if ( $Dauer != 0 )
                  $daten['ZEINHEIT'][] = $Dauer.' h x '.$einzelmenge.' '.$Einheit;
                else
                  $daten['ZEINHEIT'][] = ' x '.$einzelmenge.' '.$Einheit;
              }
              else
                $daten['ZEINHEIT'][] = $Einheit;
            }
            $daten['ZUEINHEIT'][] = $Einheit;
    }
    $daten['SORTZDATUM'][] = $Startdatumzahl;
    $daten['ZMWSTSUMME'][] = Preis(berechnePreis($PreisZahl,$Menge,$Dauer,$Rabatt)*$MWSt/100);
    $daten['ZRABATT'][] = $Rabatt;
    $daten['ZGESAMT'][] = Preis(berechnePreis($PreisZahl,$Menge,$Dauer,$Rabatt));
    $daten['ZART'][] = $LetzteArt;
    $daten['ZARTIKELNR'][] = $Artikelnr;
  } // Zusammenfassung
  else
  {
    // ein einzelner Posten
    $daten['ZGESAMT'][] = $daten['GESAMT'];
    $daten['ZART'][] = $daten['ART'];
    $daten['SORTZDATUM'][] = 0;
    $daten['ZMWSTSUMME'][] = $daten['MWSTSUMME'];
    $daten['ZRABATT'][] = $daten['RABATT'];
    $daten['ZART'][] = $daten['ART'];
    $daten['ZARTIKELNR'][] = $daten['F_ARTIKEL_NR'];    // ARTIKELNR
    $daten['ZDATUM'][] = $daten['DATUM'];
    $daten['ZPREIS'][] = $daten['PREIS'];
    $daten['ZMWST'][] = $daten['MWST'];
    $daten['ZEINHEIT'][] = $daten['EINHEIT'];
    $daten['ZUEINHEIT'][] = $daten['EINHEIT'];
    $daten['ZMENGE'][] = $daten['MENGE'];
    $daten['ZUMENGE'][] = $daten['MENGE'];
    $daten['ZDAUER'][] = $daten['DAUER'];
    $daten['ZBEZEICHNUNG'][] = $daten['F_BEZEICHNUNG'];
  }
  return $daten;
}

if ( ! isset($_REQUEST['db']) )
  die('Keine Datenbank angegeben!');
if ( ! isset($_REQUEST['id']) || ! is_numeric($_REQUEST['id']) )
  die('Keine id-Nummer angegeben!');
if ( ! isset($_REQUEST['Vorlage']) || $_REQUEST['Vorlage'] == '' )
  die('Keine Vorlage angegeben!');
if ( strpos($_REQUEST['db'],'/')!== false && strpos($_REQUEST['db'],'/') == 0 )
  $_REQUEST['db'] = substr($_REQUEST['db'],1);

$attribute = array();
if ( !isset($session_userid) || $session_userid == '' )
  die('Unauthorisiert!');
$Artikelarten = holeArtikelarten();
$Anredearten = holeAnredearten();
$Nummer = '';
$Datenbank = $_REQUEST['db'];
$Vorlage = basename($_REQUEST['Vorlage']);
$id = $_REQUEST['id'];
$username= $_REQUEST['User'];
$daten = array();
$Nr = '';
// TODO: Institutionen als Felder!
switch ($Datenbank )
{
  case 'Buchungen':
    $sql = 'SELECT * FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_ADRESSEN;
    $sql .= " ON Adressen_id=F_Adressen_id WHERE Buchung_Nr=$id LIMIT 1";
    $Nummer = 'B'.$id;
    $daten = fuelleDaten($sql);
    // Eintraege hinzufuegen
    $sql = 'SELECT F_Artikel_Nr, F_Preis,F_Bezeichnung, Rabatt, Menge, Datum, '.
      'Bezeichnung, F_Preisliste_id, Art, Einheit, Dauer, MWST as MWSt FROM (('.
      TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.
      TABLE_BUCHUNGEN.' ON F_Buchung_Nr=Buchung_Nr) INNER JOIN ('.TABLE_ARTIKEL.
       ' INNER JOIN '.TABLE_MWST.' ON F_MWST=MWST_id) '. 
        ' ON F_Artikel_Nr=id) INNER JOIN '.TABLE_ARTIKELARTEN.
        " ON F_Art_id=Art_id WHERE F_Buchung_Nr=$id AND NOT Unberechnet ORDER BY Art, Bezeichnung, Datum";
    $daten = array_merge($daten, fuelleDaten($sql));
    $query = sql_query('SELECT Adressen_id FROM '.TABLE_ADRESSEN.' INNER JOIN '.
      TABLE_BUCHUNGEN." ON F_Adressen_id=Adressen_id WHERE Buchung_Nr=$id LIMIT 1");
    $Adressenid = sql_fetch_row($query);
    sql_free_result($query);
    $daten = DatenZusammenfassen($daten);
    $daten = fuelleSchlafplaetze($daten);
    $daten = fuelleProgramme($daten);
    $daten = InstitutionErsetzen($daten);
    $Adressenid = $Adressenid[0];
    sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Logtext=CONCAT("'.date('d.m.Y H:i').
      ' '.$Vorlage.' erzeugt '.$username.'\n",Logtext) WHERE Buchung_Nr = '.
        $id);
    
    break;
  case 'Rechnung':
    $sql = 'SELECT * FROM '.TABLE_RECHNUNGEN." WHERE Rechnung_id=$id LIMIT 1";
    $Nummer = 'R'.$id;
    $daten = fuelleDaten($sql);
    $sql = 'SELECT * FROM '.TABLE_BUCHUNGEN.' WHERE Buchung_Nr='.$daten['F_BUCHUNG_NR'];
    $daten = fuelleDaten($sql, $daten);
    $sql = 'SELECT Adressen_id FROM '.TABLE_ADRESSEN.' WHERE Adressen_id='.$daten['F_ADRESSEN_ID'];
    if (! $query = sql_query($sql)) die( sql_error());
    if ( $adresse = sql_fetch_row($query) )
    {
      $Adressenid = $adresse[0];
      $daten['ADRESSEN_ID'] = $Adressenid;
    }
    sql_free_result($query);
    $sql = 'SELECT * FROM '.TABLE_RECHNUNGSEINTRAEGE.
      " WHERE F_Rechnung_id=$id ORDER BY Art, F_Bezeichnung, Datum";
    $daten = fuelleDaten($sql, $daten);
    $daten = DatenZusammenfassen($daten);
    $daten = fuelleSchlafplaetze($daten);
    $daten = fuelleProgramme($daten);
    $daten = berechneGesamtsumme($daten);
    $daten = InstitutionErsetzen($daten);
    $daten = sortiereDatenNachDatum($daten); 
     sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Logtext=CONCAT("'.date('d.m.Y H:i').
      ' '.$Vorlage.' erzeugt '.$username.'\n",Logtext) WHERE Buchung_Nr = '.
        $daten['F_BUCHUNG_NR']);
    $Nr = '-'.$daten['BUCHUNG_NR'];
    if ( $daten['RECHNUNGSNUMMER'] > 0 ) $Nr .= '-'.$daten['RECHNUNGSNUMMER'];
    break;
  case 'Adressen':
    $sql = 'SELECT * FROM '.TABLE_ADRESSEN." WHERE Adressen_id=$id LIMIT 1";
    $daten = fuelleDaten($sql);
    if ( isset($_REQUEST['InstId']) && is_numeric($_REQUEST['InstId']) )
    {
      $daten['F_INSTITUTION'] = $_REQUEST['InstId'];
      $daten = InstitutionErsetzen($daten);
    }
    $Nummer = $id;
    $Adressenid = $daten['ADRESSEN_ID'];
    break;
  case 'ArtikelPreislisten':
    $sql = 'SELECT Bezeichnung AS Preisliste, GueltigAb FROM '.TABLE_PREISLISTEN.
      ' WHERE Preisliste_id='.$id;
    $daten = fuelleDaten($sql);
    $Where = '';
    if ( isset($_REQUEST['Arten']) && trim($_REQUEST['Arten']) != '') 
    {
    	$Where = ' AND F_Art_id IN ('.$_REQUEST['Arten'].')';
    }
    $sql = 'SELECT * FROM ('.TABLE_ARTIKEL.' INNER JOIN '.TABLE_PREISE.
      ' ON id=Artikel_Nr) INNER JOIN '.TABLE_ARTIKELARTEN.' ON F_Art_id=Art_id '.
      ' WHERE F_Preisliste_id='.$id.$Where.
      ' ORDER BY Art, Bezeichnung';
    $daten = fuelleDaten($sql, $daten);
    $daten = fuelleArtikel($daten);
    break;
  case 'Kuechenzettel':
    $sql = 'SELECT * FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_ADRESSEN;
    $sql .= " ON Adressen_id=F_Adressen_id WHERE Buchung_Nr=$id LIMIT 1";

    $daten = fuelleDaten($sql);
    $Nummer = 'B'.$id;
    $daten = Kuechenzettel($id, $daten);
    $Adressenid = $daten['F_ADRESSEN_ID'];
    $daten['ADRESSEN_ID'] = $Adressenid;
    sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Logtext=CONCAT("'.date('d.m.Y H:i').
      ' Küchenzettel erzeugt '.$username.'\n",Logtext) WHERE Buchung_Nr = '.$id);
  break;
  case 'Seminar':
      // Seminardaten des Seminars mit gegebener id
      $sql = 'SELECT * FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_SEMINARE;      
      $sql .= ' ON '.TABLE_SEMINARE.'.F_Buchung_Nr=Buchung_Nr '; 
      $sql .= 'WHERE Buchung_Nr='.$id;
      $Nummer = 'B'.$id;
      $daten = fuelleDaten($sql);
      $sql = 'SELECT * FROM '.TABLE_SEMINARPREISE.' WHERE F_Buchung_Nr='.$id;     
      unset($daten['F_BUCHUNG_NR']);
      $daten = fuelleDaten($sql, $daten);      
      unset($daten['F_BUCHUNG_NR']);
      $Adressenid = '';
      unset($daten['F_ADRESSEN_ID']);      
      sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Logtext=CONCAT("'.date('d.m.Y H:i').
      ' '.$Vorlage.' erzeugt '.$username.'\n",Logtext) WHERE Buchung_Nr = '.$id);
      break;
  case 'Seminare':
    if ( isset($_REQUEST['InstId']) && is_numeric($_REQUEST['InstId']) )
    {
      $Adressenid = $_REQUEST['InstId'];
      $sql = 'SELECT * FROM (('.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_SEMINARE;
      $sql .= ' ON '.TABLE_SEMINARE.'.F_Buchung_Nr=Buchung_Nr) INNER JOIN ';
      $sql .= TABLE_SEMINARTEILNEHMER.' ON '.TABLE_SEMINARTEILNEHMER;
      $sql .= '.F_Buchung_Nr=Buchung_Nr) INNER JOIN '.TABLE_ADRESSEN;
      $sql .= ' ON Adressen_id='.TABLE_SEMINARTEILNEHMER.'.F_Adressen_id ';
      $sql .= "WHERE Buchung_Nr=$id AND Adressen_id=$Adressenid LIMIT 1";
      $Nummer = 'B'.$id;
      $daten = fuelleDaten($sql);
      if ( $daten['SEMINARGRUPPE'] != 0 )
      {
        $sql = 'SELECT CONCAT(Name,", ",Vorname) AS SeminarName, SeminarPreis, ' .
      		 'Geburtsdatum AS SeminarGeburtsdatum ' .
      		' FROM '.TABLE_SEMINARTEILNEHMER.' INNER JOIN '.TABLE_ADRESSEN.
              ' ON F_Adressen_id=Adressen_id WHERE F_Buchung_Nr='.$id.
              ' AND SeminarGruppe='.$daten['SEMINARGRUPPE'];          
        unset($daten['SEMINARPREIS']);
        unset($daten['SEMINARGRUPPE']);
        $daten = fuelleDaten($sql, $daten);
      }
      $sql = 'SELECT * FROM '.TABLE_SEMINARPREISE.' WHERE F_Buchung_Nr='.$id;     
      unset($daten['F_BUCHUNG_NR']);
      $daten = fuelleDaten($sql, $daten);      
      sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Logtext=CONCAT("'.date('d.m.Y H:i').
      ' '.$Vorlage.' erzeugt '.$username.'\n",Logtext) WHERE Buchung_Nr = '.$id);
    }
    else
    {
      // Teilnehmerliste des Seminars mit gegebener id
      $sql = 'SELECT * FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_SEMINARE;
      $sql .= ' ON '.TABLE_SEMINARE.'.F_Buchung_Nr=Buchung_Nr ';
      $sql .= 'WHERE Buchung_Nr='.$id.' LIMIT 1';
      $Nummer = 'B'.$id;
      $daten = fuelleDaten($sql);
      unset($daten['F_BUCHUNG_NR']);
      $Adressenid = '';
      unset($daten['F_ADRESSEN_ID']);
      $sql = 'SELECT *, Geburtsdatum AS SeminarGeburtsdatum FROM '.
          TABLE_SEMINARTEILNEHMER.' INNER JOIN '.TABLE_ADRESSEN;
      $sql .= ' ON Adressen_id=F_Adressen_id ';
      $sql .= 'WHERE F_Buchung_Nr='.$id;
      $sql .= ' ORDER BY Name, Vorname';
      $daten = fuelleDaten($sql, $daten);
      sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Logtext=CONCAT("'.date('d.m.Y H:i').
      ' '.$Vorlage.' erzeugt '.$username.'\n",Logtext) WHERE Buchung_Nr = '.$id);
    }
  break;
} // switch

if ( $Datenbank == 'Rechnung' ) $Datenbank = 'Fakturierung/'.$DokArten[$daten['RECHNUNG']];

define('POO_TMP_PATH', "Vorlagen/".$Datenbank."/");
define('PCLZIP_INCLUDE_PATH','inc/misc/pclzip/');
define('ZIPLIB_INCLUDE_PATH','inc/misc/');
require('inc/misc/phpOpenOffice.php');
$doc = new phpOpenOffice();
$doc->loadDocument("Vorlagen/$Datenbank/$Vorlage");
$doc->insertStyles();

$doc->parse($daten, $attribute);
if ( $Datenbank != 'ArtikelPreislisten' && is_numeric($Adressenid) )
{
  // Preislisten werden nicht gespeichert
  $Pfadname = CVJM_ADRESSEN_DIR.'/'.$Adressenid.'/';
  $Dateiname = basename($Vorlage,CVJM_ENDUNG)."-$id$Nr-".date('dmyHi');
  if (!is_dir($Pfadname) )
    mkdirs($Pfadname, 0777);
  $doc->savefile($Pfadname.$Dateiname.CVJM_ENDUNG);
  // Neu: Weiterleitung auf Erstellung und Ausgabe der PDF-Datei
  if ( $Datenbank != 'Adressen')
  {
    // refresh parent window (list of correspondence )
	if ( $Nummer != '' )
	{
		$Nummer = '&'.$Nummer;
	}
	header('Location: https://' . $_SERVER['HTTP_HOST']. '/oopdf.php?Datei='.$Pfadname.$Dateiname);//.$Nummer);
  }
  else
  {
  	$doc->download($Dateiname);
  }
}
else
{
  if ( $Datenbank == 'Seminar' || $Datenbank == 'Seminare')
    $Dateiname = 'Seminar-'.$id;
  else
    $Dateiname = 'Preisliste-'.$id;
  $doc->download($Dateiname);
}
$doc->clean();
?>
