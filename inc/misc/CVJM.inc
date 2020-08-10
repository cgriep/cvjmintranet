<?php
/* Hilfsfunktionen f&uuml;r CVJM */

load_lang('CVJM');

// Bezeichnung der Mitteilung aus der Putzmaske
define('CVJMPUTZMITTEILUNG','Mitteilung der Putzkräfte');
define('CVJMSCHLUESSELMITTEILUNG', 'Besondere Schlüssel ausgegeben');

// Endung der Korrespondenzdateien
define('CVJM_ENDUNG', '.odt');
define('CVJM_ENDUNG_PDF', '.pdf');

// Name des Verzeichnisses, in dem Anhänge zu Adressen gelagert werden
define('CVJM_ADRESSEN_DIR', 'Adressen');

// Name des Verzeichnisses, in dem Vorlagen gelagert werden
define('CVJM_VORLAGEN_DIR', 'Vorlagen');

// Kategorien für Artikel -> cvjm_Artikelarten
define('CVJMART_ORT', 5);
define('CVJMART_VERPFLEGUNG', 2);
define('CVJMART_PROGRAMM', 3);
define('CVJMART_VERLEIH', 9);
define('CVJMART_PAUSCHALEKOPF', 12);
define('CVJMART_PAUSCHALENACHT', 7);
define('CVJMART_PROGRAMMPAKET', 13);
define('CVJMART_PAUSCHALEKOPFNACHT', 15); 
define('CVJMART_UNBEKANNT', 1);
define('CVJMART_VERPFLEGUNGPAUSCHAL', 17);

// Artikelnummer von 'Selbstverpflegung
define('CVJM_SELBSTVERPFLEGUNG', '350');
// Artikelnummer von Pauschale 'Vegetarier'
define('CVJM_VEGETARIER', '391');
// Artikelnummer von Pauschale 'Schweinefleischlos'
define('CVJM_SCHWEINEFLEISCHLOS', '1496');
// Artikelnummer von Pauschale 'ohne Hilfe' in der Küche
define('CVJM_OHNEMITHILFE', '390');
// Artikelnummer von Pauschale 'allgemein' in der Küche
define('CVJM_PAUSCHALE', '501');

// Name der Buchungsgruppe, die e-Mails bei Bemerkungen bekommt.
define('CVJM_VERWALTUNGSGRUPPE', 'group_3');
define('CVJM_BUCHUNGSGRUPPE', 'group_20');
define('CVJM_ANGEBOTSGRUPPE', 'group_9');
define('CVJM_ANREISEGRUPPE', 'group_24');
define('CVJM_ABNAHMEGRUPPE', 'group_25');
define('CVJM_AUSSENTEAM', 'group_8');

// Namen der Tabellen
define('TABLE_AUFTRAG', 'cvjm_Auftraege');
define('TABLE_ARTIKEL', 'cvjm_Artikel');
define('TABLE_ARTIKELBEZIEHUNGEN', 'cvjm_Artikelbeziehungen');
define('TABLE_BUCHUNGEN', 'cvjm_Buchungen');
define('TABLE_BUCHUNGSEINTRAEGE', 'cvjm_Buchungseintraege');
define('TABLE_BUCHUNGPERSONEN','cvjm_BuchungPersonen');
define('TABLE_ADRESSEN', 'cvjm_Adressen');
define('TABLE_PREISE', 'cvjm_Preise');
define('TABLE_PREISLISTEN','cvjm_Preislisten');
define('TABLE_KATEGORIEN', 'cvjm_Kategorien');
define('TABLE_ADRESSEN_KATEGORIE','cvjm_Adressen_Kategorie');
define('TABLE_ADRESSEN_INSTITUTIONEN','cvjm_Adressen_Institutionen');
define('TABLE_BUCHUNGSBEMERKUNGEN','cvjm_Buchungsbemerkungen');
define('TABLE_ADRESSENBEMERKUNGEN','cvjm_Adressenbemerkungen');
define('TABLE_SCHLAFPLAETZE','cvjm_Schlafplaetze');
//define('TABLE_BAUMANSICHT','cvjm_Baumansicht');
define('TABLE_STUNDENZETTEL','cvjm_Stundenzettel');
define('TABLE_ANREDEN','cvjm_Anreden');
define('TABLE_FEIERTAGE', 'cvjm_Feiertage');
define('TABLE_ARTIKELARTEN', 'cvjm_Artikelarten');
define('TABLE_RECHNUNGEN', 'cvjm_Rechnungen');
define('TABLE_RECHNUNGSEINTRAEGE', 'cvjm_Rechnungseintraege');
define('TABLE_POSTLEITZAHLEN', 'cvjm_Postleitzahlen');
define('TABLE_MWST', 'cvjm_MWST');
define('TABLE_ALTERSGRUPPEN', 'cvjm_Altersgruppen');
define('TABLE_ARTIKELGRUPPEN', 'cvjm_Artikelgruppen');
define('TABLE_SCHLUESSEL', 'cvjm_Schluessel');
define('TABLE_SEMINARTEILNEHMER', 'cvjm_Seminarteilnehmer');
define('TABLE_SEMINARE', 'cvjm_Seminardaten');
define('TABLE_SEMINARPREISE', 'cvjm_Seminarpreise');

// Typ von Abrechnungen
DEFINE('ABRECHNUNGSTYP_NORMAL', 0);
DEFINE('ABRECHNUNGSTYP_KOPF', 1);
DEFINE('ABRECHNUNGSTYP_NACHT', 2);
DEFINE('ABRECHNUNGSTYP_KOPFNACHT', 3);
DEFINE('ABRECHNUNGSTYP_VERPFLEGUNG', 4);
DEFINE('ABRECHNUNGSTYP_VERPFLEGUNGPAUSCHAL', 5);
DEFINE('ABRECHNUNGSTYP_VERPFLEGUNGNACHT', 6);
DEFINE('ABRECHNUNGSTYP_PAUSCHAL', 7);
$Abrechnungstypen[ABRECHNUNGSTYP_NORMAL] = 'Normal';
$Abrechnungstypen[ABRECHNUNGSTYP_NACHT] = 'pro Nacht';
$Abrechnungstypen[ABRECHNUNGSTYP_KOPF] = 'pro Kopf';
$Abrechnungstypen[ABRECHNUNGSTYP_KOPFNACHT] = 'pro Kopf und Nacht';
$Abrechnungstypen[ABRECHNUNGSTYP_VERPFLEGUNG] = 'Verpflegung (Altersgruppen)';
$Abrechnungstypen[ABRECHNUNGSTYP_VERPFLEGUNGPAUSCHAL] = 'Verpflegung (Altergruppen) pauschal';
$Abrechnungstypen[ABRECHNUNGSTYP_VERPFLEGUNGNACHT] = 'Verpflegung (Altergruppen)/pro Nacht pauschal';
$Abrechnungstypen[ABRECHNUNGSTYP_PAUSCHAL] = 'pauschal (zeitunabhängig)';
  // Verpflegung mit Altersgruppen und als Pauschale (egal wann gebucht)

// Status der Buchung
DEFINE('BUCHUNG_FERTIG', 30);
DEFINE('BUCHUNG_VORRESERVIERUNG', 2);
DEFINE('BUCHUNG_GELOESCHT', 31);
DEFINE('BUCHUNG_RESERVIERUNG', 10);
DEFINE('BUCHUNG_STORNIERT', 20);
DEFINE('BUCHUNG_STORNIERTABGERECHNET', 35);
DEFINE('BUCHUNG_INTERN', 40);
$Buchungsstatus[0] = 'Vorreservierung';
$Buchungsstatus[1] = '1. Nachfrage';
$Buchungsstatus[BUCHUNG_VORRESERVIERUNG] = '2. Nachfrage';  // < 2 ist vorreserviert
$Buchungsstatus[BUCHUNG_GELOESCHT] = 'Vorreservierung gelöscht';
$Buchungsstatus[BUCHUNG_RESERVIERUNG] = 'Reservierung';
$Buchungsstatus[BUCHUNG_FERTIG] = 'Abgerechnet'; // > 20 ist abgerechnet
$Buchungsstatus[BUCHUNG_STORNIERT] = 'Storniert';
$Buchungsstatus[BUCHUNG_STORNIERTABGERECHNET] = 'Storniert, abgerechnet';
$Buchungsstatus[BUCHUNG_INTERN] = 'Intern';

// Dokumentarten für Vorlagen
// Bei Änderung in cvjmAbrechnung und cvjmWiedervorlagen anpassen
$DokArten[0] = 'Angebot';
$DokArten[1] = 'Rechnung';
//$DokArten[2] = 'Vorreservierung';
$DokArten[3] = 'Bestätigung';

// Alterswerte
$Alterswerte[1] = 'unter 2';
$Alterswerte[2] = '2-6';
$Alterswerte[3] = '7-15';
$Alterswerte[4] = '16-18';
$Alterswerte[5] = '19-27';
$Alterswerte[6] = 'ab 28';

$Steuerarten[0] = 'befreit';
$Steuerarten[5] = 'Zweckbetrieb 5%';
$Steuerarten[7] = 'Zweckbetrieb 7%';
$Steuerarten[16]= 'Geschäftsbetrieb 16%';
$Steuerarten[19]= 'Geschäftsbetrieb 19%';
$SteuerKurz[0] = 'F';
$SteuerKurz[5] = 'Z';
$SteuerKurz[7] = 'Z7';
$SteuerKurz[16] = 'G16';
$SteuerKurz[19] = 'G';


// Altergruppen für die Altersangabe
$query = sql_query('SELECT * FROM '.TABLE_ALTERSGRUPPEN.' ORDER BY Reihenfolge');
while ( $anrede = sql_fetch_array($query) )
{
  $Altersgruppen[$anrede['Altersgruppe_id']] = $anrede['Altersgruppe'];
  $AltersgruppeR[] = $anrede['Altersgruppe_id'];
}
sql_free_result($query);

// Die Wochentage
$Wochentage[0] = 'Sonntag';
$Wochentage[1] = 'Montag';
$Wochentage[2] = 'Dienstag';
$Wochentage[3] = 'Mittwoch';
$Wochentage[4] = 'Donnerstag';
$Wochentage[5] = 'Freitag';
$Wochentage[6] = 'Samstag';

// Farben für die Belegungsplanung
$Belegungsfarbe[0] = '#F4E6C3';
$Belegungsfarbe[1] = '#F2F5C9';
$Belegungsfarbe[2] = '#DCC9F5';
$Belegungsfarbe[3] = '#F5DBC9';
$Belegungsfarbe[4] = '#CAF5C9';
$Belegungsfarbe[5] = '#C9F5EC';
$Belegungsfarbe[6] = '#C9E7F5';
$Belegungsfarbe[7] = '#C9D6F5';
$Belegungsfarbe[8] = '#CAC9F5';
$Belegungsfarbe[9] = '#EDC9DF';
$Belegungsfarbe[10] = '#F5C9DF';
$Belegungsfarbe[11] = '#F5E2C9';
$Belegungsfarbe[12] = '#408080';
$Belegungsfarbe[13] = '#4080FF';
$Belegungsfarbe[14] = '#8080C8';
$Belegungsfarbe[15] = '#C880C8';
$Belegungsfarbe[16] = '#C88080';
$Belegungsfarbe[17] = '#008080';
$Belegungsfarbe[18] = '#4080C8';
$Belegungsfarbe[19] = '#808000';
$Belegungsfarbe[20] = '#806480';
$Belegungsfarbe[21] = '#648080';
$Belegungsfarbe[22] = '#808064';
$Belegungsfarbe[23] = '#648080';
$Belegungsfarbe[24] = '#64C8FF';
$Belegungsfarbe[25] = '#646400';
$Belegungsfarbe[26] = '#64C8C8';
$Belegungsfarbe[27] = '#C864C8';
$Belegungsfarbe[28] = '#C8C864';
$Belegungsfarbe[29] = '#006464';
$Belegungsfarbe[30] = '#640064';

// Anredearten
// TODO: noch aus CVJMVorlagen und Abrechnung entfernen
/**
 * bestimmt den Abrechnungstyp zu einer Artikelart
 * @param int $Artikelart
 * @return int der Abrechnungstyp
 */
function getAbrechnungstyp($Artikelart)
{
	if ( is_numeric($Artikelart))
	{
	$query = sql_query("SELECT Typ FROM " . TABLE_ARTIKELARTEN . " WHERE Art_id=" . $Artikelart);
	$Abrechnungstyp = sql_fetch_row($query);
	sql_free_result($query);
	return $Abrechnungstyp[0];
	}
	else
	{
		throw new Exception('getAbrechnungstyp: Artikelart nicht numerisch');
	}
}

function holeAnredearten()
{
  $query = sql_query('SELECT * FROM '.TABLE_ANREDEN);
  while ( $anrede = sql_fetch_array($query) )
    $Anredearten[$anrede['Anrede_id']] = $anrede['Anrede'];
  sql_free_result($query);
  return $Anredearten;
}
/**
 * macht aus einem assoziativen Feld eine Parameterliste für eine URL, mit & getrennt
 * @param String $feld ein assoziatives Feld mit Schlüsseln und Werten 
 * @return String die Parameter als Key=Wert-Liste
 */
function feldToParams($feld)
{
	$pre = '';
	$params = array(); 
	foreach ($feld as $key => $value)
	{
		$params[]=urlencode($key).'='.urlencode($value);
	}
	return implode('&', $params);
}
function zeigeDatumUndUhrzeit($timestamp)
{
  echo date('d.m.Y',$timestamp);
  if ( date('H:i',$timestamp) != '00:00' )
    echo date(' H:i',$timestamp);
}

function holeArtikelarten()
{
  $Artikelarten = array();
  if ( ! $query = sql_query('SELECT * FROM '.TABLE_ARTIKELARTEN.' ORDER BY Art'))
   echo mysql_error();
  while ( $art = sql_fetch_array($query) )
  {
    $Artikelarten[$art['Art_id']] = $art['Art'];
  }
  sql_free_result($query);
  return $Artikelarten;
}
function holeMWST()
{
  $MWST = array();
  if ( ! $query = sql_query('SELECT * FROM '.TABLE_MWST))
   echo mysql_error();
  while ( $art = sql_fetch_array($query) )
    $MWST[$art['MWST_id']] = $art['MWST'];
  sql_free_result($query);
  return $MWST;
}

function OrtsAuswahlScriptEinfuegen()
{
  echo '<script language="javascript">';
  echo 'function OrtsAuswahl(id)';
  echo '{';
  echo '  F1 = window.open("';
  echo get_url(get_cmodule_id("cvjmorte"),'Art='.CVJMART_ORT.'&Wahl=');
  // docinput[design]=Printable&
  echo '"+id,';
  echo '  "print","width=600,height=400,dependent=yes,toolbar=yes,menubar=yes,';
  echo 'scrollbars=yes,resizable=yes,status=yes,location=no");';
  echo '}';
  echo '</script>';
  echo '<noscript>';
  echo 'JavaScript ist deaktiviert. Keine Ortsauswahl möglich.';
  echo '</noscript>';
}
function OrtsName($oid = -1, $mitHTML = true)
{
  global $id;
  global $DerOrt;
  $Ergebnis = '-CVJM Feriendorf-';
  if ( is_numeric($oid) ) {
    if ( $oid == -1 )
      return '';
    else
    {
      $qu = sql_query('SELECT Einruecken, Position, F_Art_id, Bezeichnung FROM '.TABLE_ARTIKEL.
           ' WHERE id = '.$oid);
      $row = sql_fetch_array($qu);
      sql_free_result($qu);
      if ( $DerOrt == '') $DerOrt = $row['Bezeichnung'];
      $Ergebnis = Ortsname(holeArtikelParent($row['F_Art_id'],
        $row['Position'], $row['Einruecken']), $mitHTML);
      if ( $Ergebnis != '' ) $Ergebnis .= ' &gt; ';
      if ( $mitHTML )
        $Ergebnis .= '<a href="'.get_url($id,'Ort='.$oid).'">';
      $Ergebnis .= $row['Bezeichnung'];
      if ( $mitHTML) $Ergebnis .= '</a>';
    }
  }
  return $Ergebnis;
}

// Gibt einen Artikelnamen zurück
// Wenn ebene > 0 dann auch die nächst höhere Ebene, sofern nicht oberste Ebene
// TODO: Mehr als eine Ebene funktioniert noch nicht
function getOrtsname($Ortid, $Ebenen=0)
{
  $erg = '-ohne Angabe-';
  if ( is_numeric($Ortid) && $Ortid >= 0) {
    /*$qu = sql_query("SELECT A1.Bezeichnung AS Bezeichnung, A2.parent AS parent, ".
      "A2.Bezeichnung AS Oberbezeichnung FROM ".TABLE_ARTIKEL." AS A1 LEFT JOIN ".
      TABLE_ARTIKEL." AS A2 ON A1.parent = A2.id WHERE A1.id = $Ortid");*/
    $qu = sql_query("SELECT * FROM ".TABLE_ARTIKEL." WHERE id=$Ortid");
    if ( $row = sql_fetch_array($qu) )
    {
      $erg = $row["Bezeichnung"];
      if ( $Ebenen > 0 )
      {
        $parent = holeArtikelParent($row["F_Art_id"],$row["Position"],$row["Einruecken"]);
        if ( $parent > 0 )
          $erg = getOrtsname($parent,$Ebenen-1)." &gt; ".$erg;
      }
    }
    sql_free_result($qu);
  }
//  if ( $Ebenen > 0 && $row["parent"] > 0 )
//    return getOrtsname($row["parent"], $Ebenen-1).">".$erg;
//  else
    return $erg;
}

// TODO: Noch aus cvjm_server, cvjmKorrespondenz, cvjmWiedervorlage, cvjmAdressBemerkungentfernen
function getAdressenAnhangLink($Adressen_id, $Bemerkung_id, $ext=CVJM_ENDUNG, $Art="Bemerkung",$Zusatz="")
{
  if ( $Zusatz != "" ) $Zusatz = "-".$Zusatz;
  if ( substr($ext,0,1) != ".") $ext = ".".$ext;
  if ( is_numeric($Bemerkung_id) && $Bemerkung_id < 0 )
    return CVJM_ADRESSEN_DIR."/$Adressen_id/$Art$ext";
  else
    return CVJM_ADRESSEN_DIR."/$Adressen_id/$Art-$Bemerkung_id$Zusatz".$ext;
}

function holeOrt($plz)
{
  $ort = "-unbekannt-";
  if ( is_numeric($plz) )
  {
    $query = sql_query("SELECT Ort FROM ".TABLE_POSTLEITZAHLEN." WHERE PLZ=".$plz);
    if ( $ortr = sql_fetch_row($query))
      $ort = $ortr[0];
    sql_free_result($query);
  }
  return $ort;
}

function holePLZ($ort)
{
  $plz = "";
  if ( $ort != "" )
  {
    $query = sql_query("SELECT PLZ FROM ".TABLE_POSTLEITZAHLEN." WHERE Ort='".$ort."'");
    if ( $ortr = sql_fetch_row($query))
      $plz = $ortr[0];
    sql_free_result($query);
  }
  if (is_numeric($plz) )
    return sprintf("%05d",$plz);
  else
    return "";
}

// Konvertiert ein deutsches Datum DD.MM.YYYY HH:ii in einen Timestamp
// Das Datum kann eine Uhrzeit enthalten
// Alternativformat: 6stellige Zahl DDMMYY
function convertDatumToTimestamp($datum)
{
  $zeit = "";
  $datum = trim($datum);
  if ( is_numeric($datum) && (strlen($datum) == 6 || strlen($datum) == 8) )
  {
    $ts = strtotime(substr($datum,4)."-".substr($datum,2,2)."-".substr($datum,0,2));
    if (! checkdate(substr($datum,0,2),substr($datum,2,2),substr($datum,4)) )
      $ts = 0;
  }
  else
  {
    $d = explode(".",$datum);
    if ( Count($d) == 3 )
    {
      if ( strpos($d[2]," ") !== false )
      {
        // Uhrzeit ist angegeben
        list($d[2], $zeit) = explode(" ",$d[2]);
      }
      $datumstr = $d[2]."-".$d[1]."-".$d[0];
      if ( $zeit != "" )
        $datumstr .= " ".$zeit;
      $ts = strtotime($datumstr);
    }
    else
      $ts = 0;
  }
  return $ts;
}

function holeKundenNummer($Adressen_id)
{
  $kunr = sql_query("SELECT MAX(Kunden_Nr) FROM ".TABLE_ADRESSEN);
  $Kundennummer = sql_fetch_row($kunr);
  sql_free_result($kunr);
  if ( ! sql_query("UPDATE ".TABLE_ADRESSEN." SET Kunden_Nr=".($Kundennummer[0]+1).
     " WHERE Adressen_id = ".$Adressen_id))
    echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler beim Anlegen einer neuen Kundennummer!</div>';
  else
  {
    echo '<div class="Erledigt">&gt;&gt;&gt; Kundennummer ".($Kundennummer[0]+1)." wurde vergeben.</div>';
  }
  return $Kundennummer[0]+1;
}

?>
