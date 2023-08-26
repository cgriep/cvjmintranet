<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.06.2002

*/

if ( $profile['editor'] == 1 ) {
  echo "<br /><br />";
  create_editor_input("Listenfelder (| getrennt)","Felder", "text_input");
  echo "<br /><br />";
  create_editor_input("Auswahlkriterien ([!]Feldname:Wert| getrennt)","Auswahlkriterien", "text_input");
  echo "<br /><br />";
  create_editor_input("Markierungsfeld","Markierungsfeld", "text_input");
  echo "<br /><br />";
  create_editor_input("Markierungswert","Markierungswert", "text_input");
  echo "<br /><br />";
  create_editor_input("Sortierungsfeld","Sort", "text_input");
}
else
{
  // Liste anzeigen !
  $Summe = "";
  $MarkierungsFeld = $document["Markierungsfeld"];
  $MarkierungsWert = $document["Markierungswert"];
  $Auswahlkriterien = $document["Auswahlkriterien"];
  $Felder = $document["Felder"];
  $Sort = $document["Sort"];
  /* Feldangaben:
    - nicht anzeigen
    # wird durch Veranstaltungsnummer ersetzt
   : trennt Bezeichnung und Feldnamen (Bemerkung:VBem#)
  */
  if ( isset($_REQUEST['Sort']) ) $Sort = $_REQUEST['Sort'];
  $Feld =  strtok($Felder, "|");
  $Feldanzahl = 0;
  while ($Feld )
  {
    // ausgeblendete Felder
    if ( strpos($Feld, "-") === false ) $Feldanzahl++;
    $Feld = strtok("|");
  }
  echo $design['table_begin'];
  echo '<tr bgcolor="' . $design['table_row_dark'].'"><th colspan="'.$Feldanzahl.
      '">'.transform($document['title'],"text")."<br />".
        transform($document["body"],$document['datatype']['body']);
  echo '</th></tr><tr bgcolor="'.$design['table_row_dark'].'">';
  $Feld =  strtok($Felder, "|");
  while ($Feld )
  {
    $Bez = $Feld;
    if ( ! (strpos($Feld, ":") === false) ) {
      $Bez = substr($Feld, strpos($Feld, ":")+1);
      $Feld = substr($Feld, 0, strpos($Feld, ":"));
    }
    if ( strpos($Feld, "-") === false ) echo "<td>$Bez</td>";
    $Feld = strtok("|");
  }
  echo '</tr>';
  $alledaten = array();
  $query = "SELECT DISTINCT user_id FROM ".TABLE_USERDATA;
  $query = sql_query($query);
  while ( $row = sql_fetch_row($query))
  {
    $alledaten[$row[0]] = get_profile($row[0], false);
    $alledaten[$row[0]]['EMail'] = url_to_link(get_user_email($row[0], false));
    $alledaten[$row[0]]['ID'] = $row[0];
    $Auswahl = strtok($Auswahlkriterien, "|");
    while ( $Auswahl && isset($alledaten[$row[0]]) )
    {
      $Kriterium = substr($Auswahl,strpos($Auswahl,":")+1);
      $Auswahl = substr($Auswahl, 0, strpos($Auswahl, ":"));
      if ( (substr($Kriterium,0,1) == "!" && $alledaten[$row[0]][$Auswahl] == substr($Kriterium,1) ) ||
        (substr($Kriterium,0,1) != "!" && $alledaten[$row[0]][$Auswahl] != $Kriterium) )
        unset($alledaten[$row[0]]);
      $Auswahl = strtok("|");
    }
  }
  sql_free_result($query);
  // Charakterdaten hinzuf&uuml;gen
  // Suchen nach VChar#
  // merge der Daten
  global $flavour;
  // Sortieren des Feldes
  $sortarray = array();
  foreach ($alledaten as $key => $value) 
  {
    $sortarray[] = $value[$Sort];
  }
  array_multisort($sortarray, SORT_ASC, $alledaten);
  $Summenwert = 0;
  foreach ( $alledaten as $key => $value)
  {
    $Feld = strtok($Felder, "|");
    echo '<tr';
    if ( $MarkierungsFeld != "" )
      if ( (substr($MarkierungsWert, 0,1) == "!" && $value[$MarkierungsFeld] != substr($MarkierungsWert,1) ) ||
        (substr($MarkierungsWert, 0,1) != "!" && $value[$MarkierungsFeld] == $MarkierungsWert ))
        echo ' bgcolor="'.$design['table_row_bright'].'"';
    echo ">";
    while ( $Feld ) {
      if ( ! (strpos($Feld, ":") === false) )
        $Feld = substr($Feld, 0, strpos($Feld, ":"));
      echo "<td";
      if ( $Feld == $Summe ) echo ' align="right"';
      echo '>';
      if ( $Feld == substr($Felder, 0, strlen($Feld)) )
        echo $value[$Feld]; // evtl. Bearbeiten
      else
        echo $value[$Feld];
      if ( $Feld == $Summe && is_numeric($value[$Feld]))
        $Summenwert += $value[$Feld];
      echo "</td>";
      $Feld = strtok("|");
    }
    echo "</tr>";
  }
  if ( $Summe != "" ) {
    $Feld = strtok($Felder, "|");
    echo '<tr bgcolor="'.$design['table_row_dark'].'">';
    while ( $Feld ) {
      echo '<td align="right">';
      if ( ! (strpos($Feld, ":") === false) )
        $Feld = substr($Feld, 0, strpos($Feld, ":"));
      if ( $Feld == $Summe ) echo $Summenwert;
      echo '</td>';
      $Feld = strtok("|");
    }
    echo '</tr>';
  }
  echo $design['table_end'];
  echo "<b>".count($alledaten)." Eintr&auml;ge</b><br />";
}
?>