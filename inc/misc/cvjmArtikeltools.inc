<?php

/**
 * cvjmArtikeltools.inc
 * Hilfsfunktionen zu Artikeln
*/

function holeArtikelID($Artikelname)
{
  $query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE Bezeichnung="'.$Artikelname.'"');
  if ( $row = sql_fetch_row($query) )
    $id = $row[0];
  else
    $id = -1;
  sql_free_result($query);
  return $id;
}


// Sucht einen Artikel mit Zusatz
// gut f�r Altersangaben (Fr�hst�ck -> Fr�hst�ck 6-8 Jahre) etc.
function holeUnterArtikelID($Artikelid, $Zusatz)
{
  $sql = 'SELECT T1.id FROM '.TABLE_ARTIKEL.' AS T1 INNER JOIN '.TABLE_ARTIKEL.
    " AS T2 ON T1.Bezeichnung=CONCAT(T2.Bezeichnung,' $Zusatz') WHERE T2.id=$Artikelid";
  if ( ! $query = sql_query($sql) ) echo "Fehler $sql: ".sql_error();
  if ( $row = sql_fetch_row($query) )
  {
    $id = $row[0];
  }
  else
    $id = -1;
  sql_free_result($query);
  return $id;
}


function holeArtikelParent($Art, $Position, $Einruecken)
{
  $sql = 'SELECT MAX(Position) FROM '.TABLE_ARTIKEL;
  $sql .= " WHERE Position<$Position AND Einruecken<$Einruecken AND F_Art_id=$Art";
  if ( ! $query = sql_query($sql)) echo "HAP $sql:".sql_error();
  if ( $zeile = sql_fetch_row($query))
    $zeile = $zeile[0];
  sql_free_result($query);
  if ( ! is_numeric($zeile)) $zeile = -1;
  if ( ! $query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.
    " WHERE Position=$zeile AND F_Art_id=$Art") ) echo "HAP2: ".sql_error();
  if ( ! $art = sql_fetch_row($query)) $art[0] = -1;
  sql_free_result($query);
  return $art[0];
}
function holeArtikelParentID($id)
{
  if ( ! $query = sql_query('SELECT * FROM '.TABLE_ARTIKEL.' WHERE id='.$id))
    echo 'Fehler: '.sql_error();
  if ( $art = sql_fetch_array($query))
    $art = holeArtikelParent($art['F_Art_id'],$art['Position'],$art['Einruecken']);
  else
    $art = -1;
  sql_free_result($query);
  return $art;
}

function holeArtikelMasterParent($Art, $Position, $Einruecken)
{
  $sql = 'SELECT MAX(Position) FROM '.TABLE_ARTIKEL;
  $sql .= " WHERE Position<=$Position AND Einruecken=0 AND F_Art_id=$Art";
  if ( ! $query = sql_query($sql)) echo "HAMP $sql: ".sql_error();
  if ( ! $zeile = sql_fetch_row($query)) $zeile[0] = -1;
  sql_free_result($query);
  $zeile = $zeile[0];
  $query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.
    " WHERE Position=$zeile AND F_Art_id=$Art");
  if ( ! $art = sql_fetch_row($query)) $art[0] = -1;
  sql_free_result($query);
  return $art[0];
}

// Zeigt die Artikel einer bestimmten Art / Buchung
function ZeigeArtikel($Art, $Buchung_Nr)
{
  $Bereiche = array();
  $sql = 'SELECT DISTINCT id, Bezeichnung FROM '.TABLE_BUCHUNGSEINTRAEGE.
    ' INNER JOIN '.TABLE_ARTIKEL.
    ' ON F_Artikel_Nr = id WHERE F_Buchung_Nr = '.$Buchung_Nr.
    ' AND F_Art_id ='.$Art;
  if ( ! $query = sql_query($sql)) echo 'Fehler ZeigeArtikel: '.sql_error();
  while ( $artikel = sql_fetch_array($query) )
  {
    $Bereiche[] = $artikel['Bezeichnung'];
  }
  sql_free_result($query);
  return implode(', ',$Bereiche);
}

/*
function BerechneEinzelnSchlafplaetze($Ortid)
{
  $query = sql_query('SELECT Schlafplaetze FROM '.TABLE_SCHLAFPLAETZE.
    ' WHERE Artikel_Nr = '.$Ortid);
  if ( $row = sql_fetch_row($query) )
    $Plaetze = $row[0];
  else
    $Plaetze = -1;
  sql_free_result($query);
  return $Plaetze;
}

// Ergibt die Schlafpl�tze eines Ortes, -1 wenn keine
function BerechneSchlafplaetze($Ortid)
{
  $Plaetze = BerechneEinzelnSchlafplaetze($Ortid);
  if ( ! $query = sql_query('SELECT id, Position, Einruecken,F_Art_id FROM '.
    TABLE_ARTIKEL.' WHERE id = '.$Ortid)) echo sql_error();
  if ( ! $artikel = sql_fetch_array($query))
  {
    echo "Fehler: $Ortid nicht gefunden.";
    return $Plaetze;
  }
  sql_free_result($query);
  if ( ! $query = sql_query('SELECT Position FROM '.TABLE_ARTIKEL.
    ' WHERE Position>'.$artikel['Position'].' AND Einruecken<='.$artikel['Einruecken'].
    ' AND F_Art_id='.$artikel['F_Art_id'].' ORDER BY Position LIMIT 1')) echo sql_error();
  if ( ! $artikel2 = sql_fetch_array($query))
    $artikel2['Position'] = 999999999;
  sql_free_result($query);
  $query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE Position>'.$artikel['Position'].
    ' AND F_Art_id='.$artikel['F_Art_id'].' AND Position<'.$artikel2['Position']) ;
  while ( $Ort = sql_fetch_row($query))
  {
    $p = BerechneEinzelnSchlafplaetze($Ort[0]);
    if ( $p >= 0 )
    {
      if ( $Plaetze < 0 ) $Plaetze = 0;
      $Plaetze += $p;
    }
  }
  sql_free_result($query);
  return $Plaetze;
}

// ergibt die Gesamtanzahl der Schlafpl�tze (0 ... )
function BerechneAlleSchlafplaetze($Buchungid, $datum=0, $auswahlID='')
{
  $sql = 'SELECT DISTINCT F_Artikel_Nr FROM '.TABLE_BUCHUNGSEINTRAEGE;
  $sql .= ' WHERE F_Buchung_Nr= '.$Buchungid;
  if ($datum > 0) $sql .= ' AND Datum='.$datum;
  if ( $auswahlID != '' ) $sql .= ' AND F_Artikel_Nr IN ('.$auswahlID.')';  
  $query = sql_query($sql);
  $Plaetze = -1;
  while ( $row = sql_fetch_row($query) )
  {
    $p = BerechneSchlafplaetze($row[0]);
    if ( $p >= 0 )
    {
      if ( $Plaetze < 0 ) $Plaetze = 0;
      $Plaetze += $p;
    }
  }
  sql_free_result($query);
  return $Plaetze;
}
*/
function holePreis($id, $Preisliste, $Stunde = false)
{
  if ( ! is_numeric($id) || ! is_numeric($Preisliste) ) return 0;
  if ( ! $query = sql_query('SELECT Preis, PreisStunde FROM '.TABLE_PREISE.
    ' WHERE F_Preisliste_id = '.$Preisliste.' AND Artikel_Nr = '.$id) ) echo 'Fehler '.sql_error();
  if ( ! $preis = sql_fetch_row($query) )
  {
    // parent pr�fen
    $parent= holeArtikelParentID($id);
    if ( $parent > 0 )
      $erg = holePreis($parent, $Preisliste, $Stunde);
    else
      $erg = 0;
  }
  else
  {
    if ( $Stunde )
      $erg =$preis[1];
    else
      $erg =$preis[0];
  }
  if ( ! is_numeric($erg) ) $erg = 0;
  sql_free_result($query);
  return $erg;
}

function holeOberArtikel($artikel_id, $buchung, $tabelle = TABLE_ARTIKELBEZIEHUNGEN)
{
  $sql = "SELECT $tabelle.F_Artikel_id FROM $tabelle ".
    ' INNER JOIN '.TABLE_BUCHUNGSEINTRAEGE.' ON F_Artikel_id=F_Artikel_Nr '.
    " WHERE F_Unterartikel_id=$artikel_id AND F_Buchung_Nr=".$buchung['Buchung_Nr'];
  if ( ! $q = sql_query($sql)) echo "Fehler $sql: ".sql_error();
  $vorhanden = -1;
  if ( $da = sql_fetch_row($q) )
  {
    // Wenn ein Oberartikel vorhanden ist, ist der Preis 0
    $vorhanden = $da[0];
  }
  sql_free_result($q);
  return $vorhanden;
}

// gibt die ID des (ersten) Oberartikels zur�ck wenn in der Buchung vorhanden
function pruefeAlleOberArtikel($artikel_id, $buchung, $tabelle = TABLE_ARTIKELBEZIEHUNGEN)
{
  $sql = 'SELECT F_Artikel_id FROM '.TABLE_ARTIKELBEZIEHUNGEN.
    " WHERE F_Unterartikel_id=$artikel_id";
  if ( ! $q = sql_query($sql)) echo "Fehler $sql: ".sql_error();
  $vorhanden = holeOberArtikel($artikel_id, $buchung, $tabelle);
  while ( ($da = sql_fetch_row($q)) && $vorhanden == -1)
  {
    $vorhanden = pruefeAlleOberArtikel($da[0], $buchung, $tabelle);
  }
  sql_free_result($q);
  return $vorhanden;
}

function OberArtikelvorhanden($artikel_id, $buchung)
{
  return (holeOberArtikel($artikel_id, $buchung) != -1);
}

function berechnePreis($einzelpreis, $menge, $dauer=0, $rabatt=0)
{
  if ( $dauer == 0 ) $dauer = 1;
  if ( $rabatt == 0 )
    $rabatt = 1;
  else
    $rabatt = (100-$rabatt)/100;
  return $einzelpreis*$menge*$dauer*$rabatt;
}

// sucht einen Preis. Ist ein Oberartikel in der Buchung vorhanden,
// so wird nur dieser berechnet.
// TODO: Mehrfacheinbindungen
function holeEndPreis($artikel_id, $buchung, $Stunde = false)
{
  if ( pruefeAlleOberArtikel($artikel_id, $buchung) == -1 )
  //! OberArtikelvorhanden($artikel_id, $buchung) )
  {
    return holePreis($artikel_id, $buchung['F_Preisliste_id'], $Stunde);
  }
  else
    return 0;
}
/**
 * Sucht Artikelnachfolger in Bezug auf die Position eines andere Artikels.
 * Es geht also um die Positionierung im Artikelbaum. Die Einr�cktiefe bleibt
 * dabei gleich.
 * Es wird der Artikel gefunden, der vor bzw. nach dem Artikel liegt.
 * @param $Artikel der Artikel auf den sich bezogen wird (kompletter Datensatz)
 * @Richtung h (hoch), r (runter) oder egal: Nachfolger des Nachfolgers (zweimal r)
 */
function findeArtikelPos($Artikel, $Richtung)
{
  // Sucht
  if ( $Richtung == 'h' )
  {
    // der Vorg�nger
    $query = sql_query('SELECT MAX(Position) FROM '.TABLE_ARTIKEL.
     ' WHERE Einruecken<='.$Artikel['Einruecken'].' AND Position < '.$Artikel['Position'].
      ' AND F_Art_id='.$Artikel['F_Art_id']);
    if ( ! $Pos = sql_fetch_row($query) )
    {
      $Pos[0] = $Artikel['Position']-1;
    }
    if ( ! is_numeric($Pos[0]) ) $Pos[0] = 0;
  }
  else if ( $Richtung == 'r' )
  {  // der nachfolgende
    $sql = 'SELECT MIN(Position) FROM '.TABLE_ARTIKEL.
     ' WHERE Einruecken<='.$Artikel['Einruecken'].' AND Position > '.$Artikel['Position'].
      ' AND F_Art_id='.$Artikel['F_Art_id'];
    if ( ! $query = sql_query($sql)) echo sql_error();
    if ( ! $Pos = sql_fetch_row($query) )
    {
      $Pos[0] = $Artikel['Position']+1;
    }
    if ( ! is_numeric($Pos[0]) )
    {
      sql_free_result($query);
      $query = sql_query('SELECT MAX(Position) FROM '.TABLE_ARTIKEL.
        ' WHERE F_Art_id='.$Artikel['F_Art_id']);
      $Pos = sql_fetch_row($query);
      $Pos[0] = $Pos[0]+1;
    }
  }
  else
  {  // der Nachfolger des Nachfolgers
    $sql = 'SELECT MIN(Position) FROM '.TABLE_ARTIKEL.
      ' WHERE F_Art_id='.$Artikel['F_Art_id'].' AND Einruecken<='.
      $Artikel['Einruecken'].' AND Position > '.
       findeArtikelPos($Artikel, 'r');
    if ( ! $query = sql_query($sql)) echo 'FAP '.$sql.': '.sql_error();
    if ( ! $Pos = sql_fetch_row($query) )
    {
      $Pos[0] = $Artikel['Position']+1;
    }
    if ( ! is_numeric($Pos[0]) ) {
      sql_free_result($query);
      $query = sql_query('SELECT MAX(Position) FROM '.TABLE_ARTIKEL.
        ' WHERE F_Art_id='.$Artikel['F_Art_id']);
      $Pos = sql_fetch_row($query);
      $Pos[0] = $Pos[0]+1;
    }
  }
  sql_free_result($query);
  return $Pos[0];
}
?>