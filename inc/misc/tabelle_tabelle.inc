<?php

    echo $design['table_begin'];
    echo '<tr bgcolor="' . $design['table_header'] . '">';
    if ( isset($_REQUEST["suche"]) )
      $Such = "&suche=".$_REQUEST["suche"];
    while ($Feld)
    {
      if ( strpos($Feld, ":") === false )
          $Feldbez = $Feld;
        else {
          $Feldbez = substr($Feld, strpos($Feld, ":")+1);
          $Feld = substr($Feld, 0, strpos($Feld, ":"));
      }
      if ( strpos($Feld,">") === false )
      {
        $Feldnamen .= $Feld . ",";
        if ( $suche != "" ) $Suchkriterien .= $Feld . " LIKE '%" . $suche . "%' OR ";
        if ( strpos($Feld, ".") > 0 ) $Feld = substr($Feld,strpos($Feld,".")+1);
        echo '<th align="left">';
        echo '<a href="' . get_url($id, "Sort=$Feld$Such") . '">';
        echo $Feldbez;
        echo "</a>";
        echo "</th>";
      }
      else
      {
        $Feld = substr($Feld, 1);
        $Len = substr($Feld, 0, strpos($Feld, ">"));
        $Feld = substr(substr($Feld, strpos($Feld, ">")),1);
        $Feldname .= $Feld . ",";
        if ( $suche != "" ) $Suchkriterien .= $Feld . " LIKE '%" . $suche . "%' OR ";
        if ( strpos($Feld, ".") > 0 ) $Feld = substr($Feld,strpos($Feld,".")+1);
        echo "<th align=\"left\" width=\"$Len%\">" . $Feldbez. "</th>";
      }
      if ( in_array($Summe, $Feld) )
      {
        $A_Summe[$Feldanzahl] = 0;
      }
      $Feld = strtok("|");
      $Feldanzahl++;
    }
    if ( strpos($Feldname, $document['IDFeld'].",") === false && $document['IDFeld'] != "" )
      $Feldnamen .= $document['IDFeld'];
    else
      // abschlie&szlig;endes , wegschneiden
      $Feldnamen = substr($Feldnamen, 0, strlen($Feldnamen)-1);
    if ( $Suchkriterien != "" ) {
      $Suchkriterien = substr($Suchkriterien, 0, strlen($Suchkriterien)-3);
      if ( $document["Auswahlkriterien"] != "" ) $Auswahlkriterien .= " AND ";
      $Auswahlkriterien .= "(".$Suchkriterien.")";
    }
    echo "</tr>";
    // Sortierkriterium angegeben?
    $Sortierfeld = implode(",",explode("|",$document['Sortierfelder']));
    if ( isset($_REQUEST["Sort"]) )
      $Sortierfeld = $_REQUEST["Sort"];
    if ( $Sortierfeld != "" ) $Sortierfeld = "ORDER BY $Sortierfeld";
    $sql = "select $Feldnamen from " . $document['Tabelle']." $Auswahlkriterien $Sortierfeld";
//    echo $sql;
    $query = sql_query("SELECT Count(*) FROM ".$document['Tabelle']." $Auswahlkriterien");
    $row = sql_fetch_row($query);
    sql_free_result($query);
    $Gesamtanzahl = $row[0];
    $Anzahl = $_REQUEST["Anzahl"];
    $Offset = $_REQUEST["Offset"];
    if ( ! is_numeric($Anzahl) ) $Anzahl = $document["Anzahl"];
    if ( ! is_numeric($Anzahl) ) $Anzahl = 100;
    if ( ! is_numeric($Offset) ) $Offset = 0;
    $sql .= " ".sql_limit($Anzahl, $Offset);
    if ( $query = sql_query($sql))
    {
    // die Funktion getswitchcolor
    $colorswitch = 0;
    include(INC_PATH.'misc/forum_functions.inc');
    while ( $data = sql_fetch_array($query))
    {
      echo '<tr bgcolor = "';
      if ( isset($document['Markierung']) && $data[$document['Markierung']] == $document['Markierbedingung']
        && $document['Markierung'] != "" )
        echo $document['Markierungsfarbe'];
      else
        $colorswitch = getswitchcolor();
      echo '" ';
      echo ">";
      $Feld = strtok($document['Felder'], "|");
      $Feldanzahl = 0;
      while ( $Feld )
      {
        if ( strpos($Feld, ":") === false ) {}
        else
          $Feld = substr($Feld, 0, strpos($Feld, ":"));
        if ( in_array($Summe, $Feld) ) $A_Summe[$Feldanzahl] += $data[$Feld];
        if ( strpos($Feld, ".") > 0 ) $Feld = substr($Feld,strpos($Feld,".")+1);
        echo '<td ';
        if ( strpos($Feld, ">" ) === false && strpos($Feld, "*") === false )
        {
          if ( $Feld == $document["LinkFeld"] )
          {
            $Link = '<a href="'.$document["LinkURL"].$data[$Feld].'">';
            $LinkEnde = "</a>";
          }
          else
          {
            $Link = "";
            $LinkEnde = "";
          }
          if ( sql_field_type($query, $Feldanzahl) == "int" || sql_field_type($query, $Feldanzahl) == "real")
          {
            if ( sql_field_len($query, $Feldanzahl) == 1)
            {
              // Bool-Wert
              echo ' align="center">';
              if ($data[$Feld] == 1)
                echo "Ja";
              else
                echo "Nein";
            }
            else
            {
              // Zahl
              echo ' align="right">' .$Link. $data[$Feld].$LinkEnde;
            }
          }
          elseif ( sql_field_type($query, $Feldanzahl) == "datetime" )
          {
            // Datumsfeld
            echo ">" . date("d.m.Y H:i",strtotime($data[$Feld]));
          }
          elseif ( sql_field_type($query, $Feldanzahl) == "timestamp" )
          {
            // Datumsfeld
            echo ">$Link";
            echo substr($data[$Feld], 8, 2).".";
            echo substr($data[$Feld], 5, 2);
            echo  "." . substr($data[$Feld],0,4); //timestamp_to_string($data[$Feld]);
            echo " " . substr($data[$Feld], 11, 2) . ":" . substr($data[$Feld], 14, 2);
            echo $LinkEnde;
          }
          else
          {
            echo ">";
            if ( is_numeric($document['EditDokuNr']) && $Feldanzahl == 0 )
            {
              $Link = "";
              $LinkEnde = "";
              echo '<a href="' .
               get_url($document['EditDokuNr'], $document['IDFeld'] . "=" .
               $data[$document['IDFeld']]) . '">';
            }
            echo $Link;
            if ( $data[$Feld] == "" )
              echo "(leer)";
            else {
              echo transform($data[$Feld], "usertext");
            }
            if ( $profile[$document['NewDS']] == '1' &&
                 is_numeric($document['EditDokuNr']) &&
                 $Feldanzahl == 0) echo '</a>';
            echo $LinkEnde;
          }
        }
        else
        {
          echo ">"; // >- Feld (nur Überschrift
        }
        echo '</td>';
        $Feldanzahl++;
        $Feld = strtok("|");
      }
    }
    echo "</tr>";
  }
  sql_free_result($query);
  if ( count($A_Summe) > 0 )
  {
    echo "<tr>";
    $Anfang = 0;
    for ($i = $Anfang ; $i <= $Feldanzahl; $i++)
    {
      echo "<td";
      if ( isset($A_Summe[$i]) )
        echo ' align="right"><b>'.$A_Summe[$i].'</b>';
      else
        echo ">";
      echo "</td>";
    }
    echo "</tr>";
  }
  echo $design['table_end'];
  if ( $Gesamtanzahl > $Offset+$Anzahl || $Offset > 0 ) {
    echo LANG_PAGE." ";
    for ( $i = 0; $i < $Gesamtanzahl; $i+= $Anzahl)
      if ( $i != $Offset )
        echo '<a href="?id='.$id.'&Offset='.$i.'">'.($i/$Anzahl+1).'</a> ';
  }

?>