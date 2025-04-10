<?php
/*

Modul BuchungsÜbersicht

AWF-Name: cvjmbuchungszusammenfassung

Parameter:
Tag - erster Tag der Anzeige
Tage - Anzahl der anzuzeigenden Tage
docinput[Kategorie][] - Kategorien, die angezeigt werden sollen
*/

if ( $profile['editor'] != 1 ) {

include(INC_PATH.'misc/CVJM.inc');
include(INC_PATH.'misc/cvjmBuchungstools.inc');
include(INC_PATH.'misc/cvjmArtikeltools.inc');
include_once(INC_PATH.'cvjm/Artikel.class.php');

echo '<style type="text/css">
<!--
  .linksLinie {
    border-right-color: black;
    border-right-width: 2pt;			
    border-right-style: solid;
	}
  }
-->
</style>';

// $art gibt die Position eines Unterartikels an.
function ArtikelAnzeigen($art, $Tag)
{
  
  // Zeigt alle Orte als Baum an
  $sql = 'SELECT id FROM '.TABLE_ARTIKEL.      
    ' WHERE F_Art_id='.CVJMART_ORT.' AND Anzeigen AND Einruecken>0 AND Position BETWEEN '.
     $art['Position'].' AND '.findeArtikelPos($art, "r");   
  
  $qu = sql_query($sql);      
  $Gesamtanzahl = 0;
  $ids = array();
  while ( $row = sql_fetch_array($qu) )
  {
  	// Nur Schlaforte berücksichtigen
	$a = new Artikel($row['id']); 
  	if ( $a->BerechneSchlafplaetze() > -1)
  	{
  	  $ids[] = $row['id'];
  	  $Gesamtanzahl++;
  	}
  } 
  sql_free_result($qu);
  for ( $i = $Tag; $i < strtotime('+1 month', $Tag); $i=strtotime('+1 day',$i))
  {
    $Orte[$i] = 0;
    $sql = 'SELECT Count(*) FROM '.TABLE_BUCHUNGEN.
         ' INNER JOIN '.TABLE_BUCHUNGSEINTRAEGE.
        ' ON F_Buchung_Nr=Buchung_Nr '.
        'WHERE F_Artikel_Nr IN ('.implode(',',$ids).') AND BStatus IN (0,'.BUCHUNG_FERTIG.','.
         BUCHUNG_VORRESERVIERUNG.','.BUCHUNG_RESERVIERUNG.','.BUCHUNG_INTERN.') '.
        " AND Datum=$i AND Bis>".strtotime('+23 hour',$i); // TODO         
    
    if (!$buchungen = sql_query($sql)) echo sql_error();
    while ( $buchung = sql_fetch_row($buchungen))
    {      
        $Orte[$i] = $buchung[0]/$Gesamtanzahl;
        //print date('d.m.Y',$i).' '.$buchung[0].' von '.$Gesamtanzahl.'<br >';
    }
    sql_free_result($buchungen);
  } 
  // Seminare suchen
  for ( $i = $Tag; $i < strtotime('+1 month', $Tag); $i=strtotime('+1 day',$i))
  {    
  	$sql = 'SELECT DISTINCT MaxTeilnehmer, '.TABLE_SEMINARTEILNEHMER.'.F_Adressen_id FROM '.TABLE_BUCHUNGEN.
  	' INNER JOIN '.TABLE_BUCHUNGSEINTRAEGE.' ON '.TABLE_BUCHUNGSEINTRAEGE.'.F_Buchung_Nr=Buchung_Nr '.
  	' INNER JOIN '.TABLE_SEMINARE.' ON '.TABLE_SEMINARE.'.F_Buchung_Nr=Buchung_Nr '.
  	' LEFT JOIN '.TABLE_SEMINARTEILNEHMER.' ON '.TABLE_SEMINARTEILNEHMER.'.F_Buchung_Nr=Buchung_Nr '.
        'WHERE F_Artikel_Nr IN ('.implode(',',$ids).') AND Seminar AND BStatus IN (0,'.BUCHUNG_FERTIG.','.
         BUCHUNG_VORRESERVIERUNG.','.BUCHUNG_RESERVIERUNG.','.BUCHUNG_INTERN.') '.
        ' AND Datum='.$i.' AND Bis>'.strtotime('+23 hour',$i);
    if (!$buchungen = sql_query($sql)) echo sql_error();
    if ( $buchung = sql_fetch_row($buchungen))
    {     
    	if ( $Orte[$i] == 0 )
    	{	
    		if ( ! is_null($buchung[1])) 
    		  $Orte[$i] = -sql_num_rows($buchungen)/$buchung[0];
        	else
          	  $Orte[$i] = -1;
    	}
    }
    sql_free_result($buchungen);
  }   
  return $Orte;  
}

  $Tag = mktime(0,0,0,date('m'),1,date('Y'));
  if ( $document['title'] != '')
  {
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  }
  echo '<a id="Oben" name="Oben"></a>';
  $monat = 0;
  // Buffer vorbereiten
  ob_start();
  echo 'Stand: '.date('d.m.Y').'<br />';
  while ( $monat < 12 )
  {
    $Tag = mktime(0,0,0,date('m',$Tag),1,date('Y',$Tag));
    echo '<div style="border-style:solid;border-color:black;border-width:thin;">';
    echo '<h2>'.date('m-Y',$Tag).'</h2>';
    echo '<table class="volleTabelle" cellspacing="0" cellpadding="1" border="0" style="border-collapse:collapse;">';
    echo '<tr class="ueberschrift"><td></td>';
    $Ende = strtotime('+1 month -1day',$Tag);
    for ( $i = $Tag; $i <= $Ende; $i=strtotime('+1 day',$i) )
    {
      echo '<td class="zentriert ';
      // Am Sonntag eine Linie machen
      if ( date('w',$i) == 0 )
        echo 'linksLinie';
      echo '">';
      switch ( date('w', $i) )
      {
        case 0: echo '<strong>So</strong>';
          break;
        case 1: echo 'Mo';
          break;
        case 2: echo 'Di';
          break;
        case 3: echo 'Mi';
          break;
        case 4: echo 'Do';
          break;
        case 5: echo 'Fr';
          break;
        case 6: echo 'Sa';
          break;
      }
      echo '<br />'.date('d.',$i);
      echo '</td>';
    }
    echo "</tr>\n";
    foreach ( array(3, 8 ,9)  as $kategorie ) // JB, FB, Zeltplatz
    {
      $query = sql_query('SELECT * FROM '.TABLE_ARTIKEL.' WHERE id='.$kategorie);
  	  $artikel = sql_fetch_array($query);
  	  sql_free_result($query);
      echo '<tr><td>'.$artikel['Bezeichnung'].'</td>';
      $Orte = ArtikelAnzeigen($artikel, $Tag);
      for ( $i = $Tag; $i <= $Ende; $i = strtotime('+1 day',$i) )
      {
        echo '<td ';
        // Am Sonntag eine Linie machen
        if ( date('w',$i) == 0 )
          echo ' class="linksLinie" ';
        if ( isset($Orte[$i]))
        {
      	  // Wenn Zeltplatz dann direkt belegt
      	  if ($Orte[$i]>= 0.7  || ($kategorie == 9 && $Orte[$i] > 0) )
        	  echo 'bgcolor="red">BEL';
      	  elseif ( $Orte[$i] > 0.3)
            echo 'bgcolor="yellow">TEIL';
          elseif ( $Orte[$i] > 0 )
            echo 'bgcolor="#C0FF00">TEIL';
        }
        if ( ! isset($Orte[$i]) || $Orte[$i] == 0 )
        {
        	echo 'bgcolor="green">FREI';
        }
        else
        {
        	if ( $Orte[$i] < 0 ) // Seminar
        	{        	  
        	  if ( Abs($Orte[$i]) > 0.6)
                echo 'bgcolor="yellow">';
              elseif ( Abs($Orte[$i]) > 0.3 )
                echo 'bgcolor="#C0FF00">';
              else
                echo 'bgcolor="green">';
              echo 'SPEZ';
			}			
        }        
        echo '</td>';
      }    
      echo '</tr>';
    }
    echo $design['table_end'];
    echo '</div>';
    $Tag = strtotime('+1 day',$Ende);
    flush();
    $monat++;
  } // Monatszählung
  $content = ob_get_contents();
  // Content speichern
  if ( ! $handle = fopen ("/var/www/httpdocs/files/belegung.html", "w") ) 
  {
  	echo 'Fehler beim öffnen der Datei!';
  }
  else
  {
  	if (!fwrite($handle, $content)) { echo 'Datei nicht schreibbar!'; }
  }
  fclose($handle);
  ob_end_flush();
} // wenn nicht Editor
?>
