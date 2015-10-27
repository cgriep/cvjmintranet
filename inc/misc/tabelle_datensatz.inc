<?php
  // Sortierfeld anzeigen
  echo '<form action="?id='.$id.'" method="post">';
  echo LANG_SORTIEREN_NACH;
  echo '  <select name="Sort">';
  while ($Feld)
  {
    if ( strpos($Feld, ":") === false )
      $Feldbez = $Feld;
    else {
      $Feldbez = substr($Feld, strpos($Feld, ":")+1);
      $Feld = substr($Feld, 0,strpos($Feld, ":"));
    }
    echo '<option value="'.$Feld.'">';
    if ( strpos($Feld, ".") > 0 ) $Feld = substr($Feld,strpos($Feld,".")+1);
    echo $Feld.'</option>';
    $Feld = strtok("|");
  }
  echo '</select>';
  echo '<input type="submit" value="'.LANG_SORT.'" />';
  if ( isset($_REQUEST["suche"]) )
    echo '<input type="hidden" name="suche" value="'.$_REQUEST["suche"].'">';
  echo '</form>';
  $Feld =  strtok($document['Felder'], "|");
  echo $design['table_begin'];
  while ($Feld)
  {
    if ( strpos($Feld, ":") === false ) {}
    else
      $Feld = substr($Feld, 0,strpos($Feld, ":"));
    if ( strpos($Feld,">") === false )
    {
      $Feldnamen .= $Feld . ",";
      if ( $suche != "" ) $Suchkriterien .= $Feld . " LIKE '%" . $suche . "%' OR ";
    }
    else
    {
      $Feld = substr($Feld, 1);
      $Len = substr($Feld, 0, strpos($Feld, ">"));
      $Feld = substr(substr($Feld, strpos($Feld, ">")),1);
      if ( $suche != "" ) $Suchkriterien .= $Feld . " LIKE '%" . $suche . "%' OR ";
      $Feldname .= $Feld . ",";
    }
    $Feld = strtok("|");
    $Feldanzahl++;
  }
  if ( strpos($Feldname, $document['IDFeld'].",") === false && $document['IDFeld'] != "" )
    $Feldnamen .= $document['IDFeld'];
  else
    // abschlie&szlig;endes , wegschneiden
    $Feldnamen = substr($Feldnamen, 0, strlen($Feldnamen)-1);
  // Abschlie&szlig;endes OR abschneiden
  if ( $Suchkriterien != "" ) {
    $Suchkriterien = substr($Suchkriterien, 0, strlen($Suchkriterien)-3);
    if ( $document["Auswahlkriterien"] != "" ) $Auswahlkriterien .= " AND ";
    $Auswahlkriterien .= "(".$Suchkriterien.")";
  }
  // Sortierkriterium angegeben?
  $Sortierfeld = implode(",",explode("|",$document['Sortierfelder']));
  if ( isset($_REQUEST["Sort"]) )
     $Sortierfeld = $_REQUEST["Sort"];
  if ( $Sortierfeld != "" ) $Sortierfeld = "ORDER BY $Sortierfeld";
  $sql = "select $Feldnamen from " . $document['Tabelle']." $Auswahlkriterien $Sortierfeld";
  //echo $sql;
  if ( ! $query = sql_query("SELECT Count(*) FROM ".$document['Tabelle']." $Auswahlkriterien") ) die(mysql_error());
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
    $farbe = $design['table_row_bright'];
    while ( $data = mysql_fetch_array($query))
    {
      $Feld = strtok($document['Felder'], "|");
      $Feldanzahl = 0;
      while ( $Feld )
      {
        if ( strpos($Feld, ":") === false )
          $Feldbez = $Feld;
        else {
          $Feldbez = substr($Feld, strpos($Feld, ":")+1);
          $Feld = substr($Feld, 0, strpos($Feld, ":"));
        }
        // Abstandszeile
        if ( $Feldanzahl == 0 ) echo '<tr><td colspan="2">&nbsp;</td></tr>';
        echo '<tr bgcolor="';
        if ( $Feldanzahl == 0 ) {
          if ( $farbe == $design['table_row_dark'] )
            $farbe = $design['table_row_bright'];
          else
            $farbe = $design['table_row_dark'];
        }
        echo $farbe;
        echo '">';
        if ( strpos($Feld, ".") > 0 ) $Feld = substr($Feld,strpos($Feld,".")+1);
        echo '<td>'.$Feldbez.'</td><td>';
        if ( strpos($Feld, ">" ) === false && strpos($Feld, "*") === false )
        {
          if ( mysql_field_type($query, $Feldanzahl) == "int" || mysql_field_type($query, $Feldanzahl) == "real")
          {
            if ( mysql_field_len($query, $Feldanzahl) == 1)
            {
              // Bool-Wert
              if ($data[$Feld] == 1)
                echo "Ja";
              else
                echo "Nein";
            }
            else
            {
              // Zahl
              echo $data[$Feld];
            }
          }
          elseif ( mysql_field_type($query, $Feldanzahl) == "datetime" )
          {
            // Datumsfeld
            echo date("d.m.Y H:i",strtotime($data[$Feld]));
          }
          elseif ( mysql_field_type($query, $Feldanzahl) == "timestamp" )
          {
            // Datumsfeld
            echo substr($data[$Feld], 8, 2) . ".";
            echo substr($data[$Feld], 5, 2);
            echo  "." . substr($data[$Feld],0,4); //timestamp_to_string($data[$Feld]);
            echo " " . substr($data[$Feld], 11, 2) . ":" . substr($data[$Feld], 14, 2);;
          }
          else
          {
            if ( is_numeric($document['EditDokuNr']) && $Feldanzahl == 0 )
              echo '<a href="' .
               get_url($document['EditDokuNr'], $document['IDFeld'] . "=" .
               $data[$document['IDFeld']]) . '">';
            if ( $data[$Feld] == "" )
              echo "(leer)";
            else {
              echo transform($data[$Feld], "usertext");
            }
            if ( $profile[$document['NewDS']] == '1' && is_numeric($document['EditDokuNr']) && $Feldanzahl == 0) echo '</a>';
          }
        }
        if ( $Feld == $document["LinkFeld"] )
          echo ' <a href="'.$document["LinkURL"].$data[$Feld].'">Weitere Info</a>';
        echo '</td></tr>';
        $Feldanzahl++;
        $Feld = strtok("|");
      }
    }
  }
  echo $design['table_end'];
  sql_free_result($query);
  if ( $Gesamtanzahl > $Offset+$Anzahl || $Offset > 0 ) {
    echo LANG_PAGE." ";
    for ( $i = 0; $i < $Gesamtanzahl; $i+= $Anzahl)
      if ( $i != $Offset )
        echo '<a href="?id='.$id.'&Offset='.$i.'">'.($i/$Anzahl+1).'</a> ';
  }
?>