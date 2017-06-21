<?php

/*
   AWF-Name: cvjmstatistik
   cvjmStatistik.inc
*/

define('CVJM_ZELTBEREICH', 'ZB');

function calculateBelegung($uebernachtungen, $sql)
{
  $query = sql_query($sql);
  $plaetze = array();
  while ( $eintrag = sql_fetch_array($query) )
  {
    $a = new Artikel($eintrag['F_Artikel_Nr']);
  	$schlafplaetze = $a->BerechneSchlafplaetze();
    if ( $schlafplaetze >= 0 )
    {
      if ( $schlafplaetze > 0 )
      {
        // Hütten-Übernachtung
        $bereich = getOrtsname(holeArtikelMasterParent($eintrag['F_Art_id'],
          $eintrag['Position'],$eintrag['Einruecken']));
      }
      else
        $bereich = CVJM_ZELTBEREICH;
      $eintrag['Datum'] = strtotime(date('Y-m-d', $eintrag['Datum']));
      if ( ! isset($plaetze[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich]) )
      {
        $plaetze[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich] = 0;
        $preise[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich] = 0;
      }
      $plaetze[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich] += $schlafplaetze;
      $preise[$eintrag['Buchung_Nr']][$eintrag['Datum']][$bereich] +=
           berechnePreis($eintrag['Preis'], $eintrag['Menge'], $eintrag['Dauer'], $eintrag['Rabatt']);
      //echo $eintrag['Buchung_Nr'].': Bereich $bereich mit $schlafplaetze<br />';
      $leute[$eintrag['Buchung_Nr']] = Personenanzahl($eintrag);
      $buchung[$eintrag['Buchung_Nr']]['Bis'] = $eintrag['Bis'];
      $buchung[$eintrag['Buchung_Nr']]['Von'] = $eintrag['Von'];
       //print gmdate("Y-m-d\TH:i:s\Z", $eintrag['Datum']) . ' // ' . $personen. ' // '.$buchung_nr . ' // ' . $bereich.'<br />'; 
      
    }
  }
  sql_free_result($query);  
  // Nun die Auslastung der Bereiche berechnen
  foreach ( $leute as $buchung_nr => $anzahl )
  {
    // Datum durchgehen
    foreach ( $plaetze[$buchung_nr] as $datum => $bereiche )
    {
      $anz = $anzahl;
      $zda = false;
      foreach ( $bereiche as $bereich => $schlafplaetze )
      {
        //echo 'Buchung $buchung_nr: Bereich $bereich mit $schlafplaetze vorhanden<br />';
        // Initialisierung
        if ( ! isset($uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]) )
        {
          //echo date('d.m.Y',$datum).'/';
          $uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)] = array();
          $uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Anzahl'] = 0;
          $uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Einnahme'] = 0;
          $uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Buchungen'] = array();
        }
        if ( $schlafplaetze > 0 )
        {
          if ( $anz <= $schlafplaetze )
          {
            $personen = $anz;
            $anz = 0;
          }
          elseif ( $anz > $schlafplaetze )
          {
            $personen = $schlafplaetze;
            $anz -= $schlafplaetze;
          }
          $uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Anzahl'] += $personen;
          $uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Einnahme'] +=
            $preise[$bereich][date('Y',$datum)][date('m',$datum)];
          //echo 'Buchung $buchung_nr: $personen in Bereich $bereich<br />';
        }
        else
          $zda = true;
        if ( ! in_array($buchung_nr, $uebernachtungen[$bereich][date('Y',
          $datum)][date('m',$datum)]['Buchungen']) )
          $uebernachtungen[$bereich][date('Y',$datum)][date('m',$datum)]['Buchungen'][] =
            $buchung_nr;
      }
      if ( $anz > 0 )
      {
        if ( $zda )
        {
          $uebernachtungen[CVJM_ZELTBEREICH][date('Y',$datum)][date('m',$datum)]['Anzahl'] += $anz;
          $uebernachtungen[CVJM_ZELTBEREICH][date('Y',$datum)][date('m',$datum)]['Einnahme'] +=
            $preise[CVJM_ZELTBEREICH][date('Y',$datum)][date('m',$datum)];
          //echo 'Buchung $buchung_nr: $anz im Zeltbereich<br />';
        }
        else
        {
          // fuer Vor- und Nachhut - Maximalanzahl Bettenanzahl unabhängig von der Anzahl der Personen
          // in diesem Fall keine nicht zuordbaren Werte

          if ($buchung[$buchung_nr]['Bis'] >= $datum && $buchung[$buchung_nr]['Von'] < $datum ) 
          {
            echo "Nicht zuordbare Personenzahl: $anz in Buchung $buchung_nr am ".
              date('d.m.Y',$datum).'<br />';
            if ( ! isset($uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]) )
            {
              $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)] = array();
              $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Anzahl'] = 0;
              $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Einnahme'] = 0;
              $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Buchungen'] = array();
            }
            if ( ! in_array($buchung_nr, $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Buchungen']) )
              $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Buchungen'][] = $buchung_nr;
            $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Anzahl'] += $anz;
            $uebernachtungen['unbekannt'][date('Y',$datum)][date('m',$datum)]['Einnahme'] +=
              $preise['unbekannt'][date('Y',$datum)][date('m',$datum)];
          }
        }
      }
    }
  }
  return $uebernachtungen;
}


if ( $profile['editor'] != 1 )
{
  include(INC_PATH.'misc/CVJM.inc');
  include(INC_PATH.'misc/cvjmArtikeltools.inc');
  include(INC_PATH.'misc/cvjmBuchungstools.inc');
  include(INC_PATH.'cvjm/Artikel.class.php');
  $Artikelarten = Artikel::getArtikelarten();
  if ( $document['title'] != '')
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';

  if ( isset($docinput['Jahr']) && is_numeric($docinput['Jahr']))
  {
  	if ( $docinput['Jahr'] > 0 )
  	{
  		$Jahr = 'FROM_UNIXTIME(Datum,"%Y")='.$docinput['Jahr'];
  	}
  	else 
  	  $Jahr = '1';
  echo '<h2>Übernachtungen</h2>';
  $sql= 'SELECT '.TABLE_BUCHUNGEN.'.*, F_Artikel_Nr, Datum, F_Art_id, Position,Einruecken FROM (('.
      TABLE_RECHNUNGSEINTRAEGE.' INNER JOIN '.
      TABLE_RECHNUNGEN.' ON F_Rechnung_id=Rechnung_id) INNER JOIN '.TABLE_ARTIKEL.
      ' ON id=F_Artikel_Nr) INNER JOIN '.TABLE_BUCHUNGEN.
      ' ON Buchung_Nr=F_Buchung_Nr WHERE Rechnung=1 AND BStatus<='.BUCHUNG_FERTIG.
      ' AND BStatus <> '.BUCHUNG_STORNIERT.  // 2017-02-05 Vor- und Nachhut wird nicht berücksichtigt
      ' AND F_Art_id='.CVJMART_ORT.
      ' AND Menge > 0 AND NOT Storno AND '.$Jahr.' ORDER BY Datum';
  $uebernachtungen = array();
  $uebernachtungen = calculateBelegung($uebernachtungen, $sql);
  
  echo '<table style="text-align:right">';
  echo '<tr class="ueberschrift"><th>Jahr</th><th>Monat</th><th>Buchungen</th><th>Übernachtungen</th></tr>';
  $allgesamt = 0;
  foreach ( $uebernachtungen as $bereich => $jahre )
  {
    echo '<tr class="helleZeile"><td colspan="4" align="left">'.$bereich.'</td></tr>';
    $gesamt = 0;
    $buanz = 0;
    foreach ( $jahre as $jahr => $monate )
    {
      foreach ( $monate as $monat => $value )
      {
        echo '<tr><td>';
        echo $jahr;
        echo '</td><td>';
        echo $monat;
        echo '</td><td>';
        echo count($value['Buchungen']).':';
        $buanz += count($value['Buchungen']);
        foreach ( $value['Buchungen'] as $key => $nr )
          echo '<a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$nr).'">'.$nr."</a>, ";
        echo '</td><td>';
        echo $value['Anzahl'];
        $gesamt += $value['Anzahl'];
        echo '</td></tr>';
      }
    }
    $allgesamt += $gesamt;
    echo '<tr class="dunkleZeile">'.
      "<td>Gesamt</td><td></td><td>$buanz</td><td>$gesamt</td></tr>";
  }
  echo '<tr class="dunkleZeile"><td>Gesamt</td><td></td><td></td><td>'.
    "$allgesamt</td></tr>\n";
  echo '</table>';

  echo "<h2>Angebote</h2>";
  echo "<h3>Programmmodule</h3>";
  // Nur Rechnungen berücksichtigen
  if (! $query = sql_query("SELECT Datum, Menge, F_Artikel_Nr, Einruecken, Position, F_Preis, Rabatt FROM ((".
    TABLE_RECHNUNGSEINTRAEGE.
    ' INNER JOIN '.TABLE_RECHNUNGEN.' ON Rechnung_id=F_Rechnung_id) INNER JOIN '.
    TABLE_ARTIKEL.' ON F_Artikel_Nr=id) INNER JOIN '.TABLE_BUCHUNGEN.
    " ON F_Buchung_Nr=Buchung_Nr WHERE Art='".$Artikelarten[CVJMART_PROGRAMM].
    "' AND Rechnung=1 AND NOT Storno AND BStatus<=".BUCHUNG_FERTIG.
    ' AND BStatus <> '.BUCHUNG_STORNIERT.' AND '.$Jahr.
    ' GROUP BY YEAR(FROM_UNIXTIME(Datum)),MONTH(FROM_UNIXTIME(Datum)),F_Artikel_Nr'))
    echo mysql_error();
  $paket = array();
  while ( $eintrag = sql_fetch_array($query) )
  {
    $parent = holeArtikelParent(CVJMART_PROGRAMM, $eintrag['Position'], $eintrag['Einruecken']);
    $jahr = date('Y', $eintrag['Datum']);
    $monat = date('m',$eintrag['Datum']);
    if ( ! isset($paket[$parent][$jahr][$monat]) )
    {
      $paket[$parent][$jahr][$monat]['Buchungen'] = 0;
      $paket[$parent][$jahr][$monat]['Personen'] = 0;
      $paket[$parent][$jahr][$monat]['Einnahmen'] = 0;
    }
    $paket[$parent][$jahr][$monat]['Buchungen'] += 1;
    $paket[$parent][$jahr][$monat]['Personen'] += $eintrag['Menge'];
    $preis = berechnePreis($eintrag['F_Preis'], $eintrag['Menge'], 0, $eintrag['Rabatt']);
	// . entfernen, um Kommaproblem zu vermeiden 
    $paket[$parent][$jahr][$monat]['Einnahmen'] += str_replace('.','',$preis);
  }
  echo '<table style="text-align:right">';
  echo '<tr class="ueberschrift"><th>Jahr</th><th>Monat</th><th>Anzahl</th><th>Personen</th><th>Einnahme</th></tr>';
  $gesamt = 0;
  $personen = 0;
  foreach ( $paket as $artikelnr => $buchungen )
  {
    echo '<tr class="helleZeile"><td colspan="5" align="left">'.getOrtsname($artikelnr).'</td></tr>';
    foreach ( $buchungen as $year => $value )
    {
      foreach ( $value as $monat => $anzahl )
      {
        echo '<tr><td>';
        echo $year;
        echo '</td><td>';
        echo $monat;
        echo '</td><td>';
        echo $anzahl['Buchungen'];
        echo '</td><td>';
        echo $anzahl['Personen'];
        $personen += $anzahl['Personen'];
        echo '</td><td>';
        echo $anzahl['Einnahmen'];
        $gesamt += $anzahl['Einnahmen'];
        echo '</td></tr>';
      }
    }
  }
  sql_free_result($query);
  echo '<tr class="dunkleZeile"><td>Gesamt</td><td></td><td></td><td>'.$personen.'</td><td>'.
    $gesamt.'</td></tr>';
  echo "</table>\n";

  // Programmpakete
  echo "<h3>Programmpakete</h3>";
  if (! $query = sql_query("SELECT Datum, Menge, F_Artikel_Nr, Einruecken, Position, F_Preis, Rabatt, F_Buchung_Nr FROM ((".
    TABLE_RECHNUNGSEINTRAEGE.
    ' INNER JOIN '.TABLE_RECHNUNGEN.' ON Rechnung_id=F_Rechnung_id) INNER JOIN '.
    TABLE_ARTIKEL.' ON F_Artikel_Nr=id) INNER JOIN '.TABLE_BUCHUNGEN.
    " ON F_Buchung_Nr=Buchung_Nr WHERE Art='".$Artikelarten[CVJMART_PROGRAMMPAKET].
    "' AND Rechnung=1 AND NOT Storno AND BStatus<=".BUCHUNG_FERTIG.
    ' AND BStatus<>'.BUCHUNG_STORNIERT.' AND '.$Jahr.
    ' GROUP BY YEAR(FROM_UNIXTIME(Datum)),MONTH(FROM_UNIXTIME(Datum)),F_Artikel_Nr'))
    echo mysql_error();
  $paket = array();
  while ( $eintrag = sql_fetch_array($query) )
  {
    $parent = holeArtikelParent(CVJMART_PROGRAMMPAKET, $eintrag['Position'], $eintrag['Einruecken']);
    $jahr = date('Y', $eintrag['Datum']);
    $monat = date('m',$eintrag['Datum']);
    if ( ! isset($paket[$parent][$jahr][$monat]) )
    {
      $paket[$parent][$jahr][$monat]['Buchungen'] = 0;
      $paket[$parent][$jahr][$monat]['Personen'] = 0;
      $paket[$parent][$jahr][$monat]['Einnahmen'] = 0;
      $paket[$parent][$jahr][$monat]['Buchung'] = array();
    }
    if ( ! in_array($eintrag['F_Buchung_Nr'], $paket[$parent][$jahr][$monat]['Buchung']))
    $paket[$parent][$jahr][$monat]['Buchung'][] = $eintrag['F_Buchung_Nr'];
    $paket[$parent][$jahr][$monat]['Buchungen'] += 1;
    $paket[$parent][$jahr][$monat]['Personen'] += $eintrag['Menge'];
    $paket[$parent][$jahr][$monat]['Einnahmen'] +=
        berechnePreis($eintrag['F_Preis'], $eintrag['Menge'], 0, $eintrag['Rabatt']);
  }
  sql_free_result($query);
  echo '<table style="text-align:right">';
  echo '<tr class="ueberschrift"><th>Jahr</th><th>Monat</th><th>Anzahl</th><th>Personen</th><th>Einnahme</th><th>Buchung</th></tr>';
  $personen = 0;
  $gesamt = 0;
  foreach ( $paket as $artikelnr => $buchungen )
  {
    echo '<tr class="helleZeile"><td colspan="6" align="left">'.getOrtsname($artikelnr).'</td></tr>';
    foreach ( $buchungen as $year => $value )
    {
      foreach ( $value as $monat => $anzahl )
      {
        echo '<tr><td>';
        echo $year;
        echo '</td><td>';
        echo $monat;
        echo '</td><td>';
        echo Count($anzahl['Buchung']);
        echo '</td><td>';
        echo $anzahl['Personen'];
        $personen += $anzahl['Personen'];
        echo '</td><td>';
        echo $anzahl['Einnahmen'];
        $gesamt += $anzahl['Einnahmen'];
        echo '</td><td>';
        foreach ( $anzahl['Buchung'] as $BuchungNr)
        {
          echo '<a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$BuchungNr).'">'.$BuchungNr.'</a>, ';
        }
        echo '</td></tr>';
      }
    }
  }
  echo '<tr class="dunkleZeile"><td>Gesamt</td><td></td><td></td><td>'.$personen.'</td><td>'.
    $gesamt.'</td></tr>';
  echo '</table>';
  } // wenn Jahr übergeben wurde
  echo '<form action="" method="post">';
  echo 'Jahr ';
  $query = mysql_query('SELECT DISTINCT FROM_UNIXTIME(Datum,"%Y") FROM '.
     TABLE_RECHNUNGSEINTRAEGE.' ORDER BY Datum DESC');
  echo '<select name="docinput[Jahr]">';
  echo '<option value="-1">-alle-</option>';
  while ( $jahr = mysql_fetch_row($query))  
  {  	
  	 echo '<option ';
  	 if ( $jahr[0] == date('Y') ) echo 'selected="selected"';
     echo '>'.$jahr[0].'</option>'."\n";
  }
  echo '</select> ';
  mysql_free_result($query); 
  echo '<input type="submit" value="Anzeigen" />';
  echo '</form>';
}
?>
