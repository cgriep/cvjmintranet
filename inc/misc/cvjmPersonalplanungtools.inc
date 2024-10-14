<?php

function show_users($name, $gruppe, $aktiv = '')
{
  if ( $aktiv == '' || is_numeric($aktiv) )
  {
    echo '<select name="docinput['.$name.']">';
    echo '<option value="" ';
    if ( $aktiv == '' ) echo 'selected="selected"';
    echo '>-nicht festgelegt-';
    echo '</option>';
    $qres = sql_query('SELECT U.user_id, U.value FROM '.TABLE_USERDATA.' AS U LEFT JOIN '.
      TABLE_USERDATA." AS U2 ON U.user_id=U2.user_id WHERE U2.name='$gruppe' AND U2.value=1 ".
      " AND U.name='nickname' ORDER BY U.value");
    while ( $user = sql_fetch_array($qres) )
    {
      echo '<option value="'.$user['user_id'].'" ';
      if ( $aktiv == $user['user_id'] )
        echo 'selected="selected"';
      echo '>'.$user['value']; //get_user_nickname($user["user_id"]);
      echo '</option>';
    }
    sql_free_result($qres);
    echo '</select>';
  }
  else
  {
  	// Eintrag ist bereits abgehakt
    echo '<input type="text" name="docinput['.$name.']" value="'.
      $aktiv.'" readonly="readonly" />';
    global $profile;
    global $docinput;
    global $id;
    if ( $profile[CVJM_VERWALTUNGSGRUPPE] == 1 )
    {
    	echo ' <a href="'.get_url($id,
    	  array('docinput[Buchung_Nr]'=>$docinput['Buchung_Nr'],
    	        'docinput[Entfernen]'=>$name)).
          '">Entfernen</a>';
    }
  }
}

function speichereSchluessel($docinput)
{
  // speichert die in docinput[Ort] abgelegten Schlüsselanzahlen
  if ( is_array($docinput['Ort']) )
  {
    foreach ($docinput['Ort'] as $key => $value )
    {
      if ( is_numeric($key) )
      {
        if ( trim($value) == '' ||$value == 0 )
        {
          if (! sql_query('DELETE FROM '.TABLE_SCHLUESSEL.' WHERE F_Buchung_Nr='.
            $docinput['Buchung_Nr'].
            ' AND F_Artikel_Nr='.$key)) echo sql_error();
        }
        elseif ( ! is_numeric($value) )
          echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler beim Schlüsseleintragen: Keine Zahl!</div>';
        else
        {
          if ( ! sql_query('INSERT INTO '.TABLE_SCHLUESSEL.
            ' (F_Buchung_Nr,F_Artikel_Nr,Anzahl,Ausgeber) VALUES ('.$docinput['Buchung_Nr'].','.
            $key.','.$value.','.SESSION_DBID.')') )
            {
              if (! sql_query('UPDATE '.TABLE_SCHLUESSEL.' SET Anzahl='.$value.
                ' WHERE F_Buchung_Nr='.$docinput['Buchung_Nr'].' AND F_Artikel_Nr='.$key))
                  echo sql_error();
            }
        }
      }
    }
    echo "<strong>&gt;&gt;&gt; Schlüsseldaten gespeichert.</strong><br />";
    sql_query('UPDATE '.TABLE_BUCHUNGEN." SET Logtext=CONCAT('".date('d.m.Y H:i').' Schlüssel verwaltet '.
              get_user_nickname()."\n',Logtext)".
            ' WHERE Buchung_Nr = '.$docinput['Buchung_Nr']);
  }
  if ( isset($docinput['Sonstige']['id']) && is_numeric($docinput['Sonstige']['id']) )
  {
    // Schlüsselbemerkung speichern
    if ( $docinput['Sonstige']['id'] != -1 )
    {
      // prüfen ob geändert
      $sql = 'SELECT Hinweis FROM '.TABLE_BUCHUNGSBEMERKUNGEN.' WHERE Bemerkung_id='.
        $docinput['Sonstige']['id'];
      $query = sql_query($sql);
      $row = sql_fetch_row($query);
      sql_free_result($query);
      if ( $row[0] != trim(sql_real_escape_string($docinput['Sonstige']['Bemerkung'])) )
      {
        if ( trim($docinput['Sonstige']['Bemerkung']) == '' )
          // Bemerkung löschen
          sql_query('DELETE FROM '.TABLE_BUCHUNGSBEMERKUNGEN.' WHERE Bemerkung_id='.$docinput['Sonstige']['id']);
        else
        {
          $sql = 'UPDATE '.TABLE_BUCHUNGSBEMERKUNGEN." SET Hinweis='".trim(
            sql_real_escape_string($docinput['Sonstige']['Bemerkung'])).
            "', Logtext=CONCAT('".date('d.m.Y H:i').' geändert '.
            get_user_nickname()."\n',Logtext) WHERE Bemerkung_id = ".$docinput['Sonstige']['id'];
          if ( ! sql_query($sql) ) echo sql_error();
        }
      }
    }
    elseif ( trim($docinput['Sonstige']['Bemerkung']) != '' )
    {
      // Neue Bemerkung anfügen
      if ( !sql_query('INSERT INTO '.TABLE_BUCHUNGSBEMERKUNGEN.' (F_Buchung_Nr,Bemerkung,Hinweis,Logtext) VALUES ('.
            $docinput['Buchung_Nr'].",'".CVJMSCHLUESSELMITTEILUNG."','".
            sql_real_escape_string($docinput['Sonstige']['Bemerkung'])."','".date('d.m.Y H:i').
            ' erstellt '.get_user_nickname()."')"))
            echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler beim Erzeugen der Bemerkung.</div>';
    }
  }
}

define('SCHLUESSELSPALTENANZAHL', 5);

function ZeigeSchluesselliste($Buchung)
{
  global $design;
  if ( ! is_array($Buchung) ) die('Fehler in ZeigeSchluesselliste: Kein Buchungsfeld');
  // Schlüssel zur Auswahl für die Buchung anzeigen
  $sql = 'SELECT DISTINCT id, Bezeichnung FROM '.TABLE_ARTIKEL.' WHERE F_Art_id='.CVJMART_ORT.
    ' AND Anzeigen ORDER BY Position';
  $query = sql_query($sql);
  $sql2 = 'SELECT Count(*) FROM '.TABLE_BUCHUNGSEINTRAEGE.' WHERE F_Buchung_Nr='.
    $Buchung['Buchung_Nr'].' AND F_Artikel_Nr=';
  echo $design['table_begin'];
  $nr = 0;
  echo '<tr class="ueberschrift"><th colspan="'.SCHLUESSELSPALTENANZAHL.
    '">ausgegebene Schlüssel';
  // bisherige Anzahl anzeigen!
  $query2 = sql_query('SELECT SUM(Anzahl) FROM '.TABLE_SCHLUESSEL.' WHERE F_Buchung_Nr='.
    $Buchung['Buchung_Nr']);
  $anz = sql_fetch_row($query2);
  sql_free_result($query2);
  $anz = $anz[0];
  if ( $anz > 0 ) echo " (bisher $anz ausgegeben)";
  echo '<tr>';
  $tabelle = array();
  $zeile = 1;
  $spalte = 1;
  $Nr = 1;
  while ( $ort = sql_fetch_array($query) )
  {
    $query2 = sql_query($sql2.$ort['id']);
    $row = sql_fetch_row($query2);
    sql_free_result($query2);
    $tabelle[$zeile][$spalte] = '<input type="text" name="docinput[Ort]['.$ort["id"].']" value="';
    $query2 = sql_query("SELECT * FROM ".TABLE_SCHLUESSEL." WHERE F_Buchung_Nr=".
      $Buchung["Buchung_Nr"]." AND F_Artikel_Nr=".$ort["id"]);
    if ( $schluessel = sql_fetch_array($query2) )
    {
      $tabelle[$zeile][$spalte] .= $schluessel["Anzahl"];
      $tabelle[$zeile][$spalte] .= '" title="zuletzt geändert durch '.
         get_user_nickname($schluessel["Ausgeber"])." am ".
         $schluessel["Ausgegeben"];
    }
    sql_free_result($query2);
    $tabelle[$zeile][$spalte] .= '" tabindex="'.$Nr;
    $Nr++;
    $tabelle[$zeile][$spalte] .= '" size="1" maxlength="1"/>';
    if ( $row[0] > 0 )
      $tabelle[$zeile][$spalte] .= '<strong>';
    $tabelle[$zeile][$spalte] .= $ort["Bezeichnung"];
    if ( $row[0] > 0 )
      $tabelle[$zeile][$spalte] .= '</strong>';
    $zeile++;
    if ( $zeile > ceil(sql_num_rows($query) / SCHLUESSELSPALTENANZAHL) )
    {
      $zeile = 1;
      $spalte++;
    }
  }
  $zeile = 1;
  while ( isset($tabelle[$zeile][1]) )
  {
    echo '<tr>';
    for ( $spalte = 1; $spalte <= SCHLUESSELSPALTENANZAHL; $spalte++ )
      if ( isset($tabelle[$zeile][$spalte]))
        echo '<td>'.$tabelle[$zeile][$spalte].'</td>';
    echo '</tr>';
    $zeile++;
  }
  echo '</table>';
  sql_free_result($query);
  echo '<div class="ueberschriftzeile">Besondere Schlüssel</div>';
  echo '<textarea name="docinput[Sonstige][Bemerkung]" cols="60" rows="5">';
  $query2 = sql_query("SELECT * FROM ".TABLE_BUCHUNGSBEMERKUNGEN.
     " WHERE F_Buchung_Nr=".$Buchung["Buchung_Nr"]." AND Bemerkung ='".
     CVJMSCHLUESSELMITTEILUNG."'");
  if ( $bem = sql_fetch_array($query2) )
  {
     echo stripslashes($bem["Hinweis"]);
  }
  else
    $bem["Bemerkung_id"] = -1;
  sql_free_result($query2);
  echo '</textarea>';
  echo '<input type="hidden" name="docinput[Sonstige][id]" value="'.$bem["Bemerkung_id"].
    '" />';
  echo '<input type="hidden" name="docinput[Buchung_Nr]" value="'.$Buchung["Buchung_Nr"].'" />';
}

/**
 *  Zeigt den INHALT der Personalplanung als Tabelle
 *  Tabellenrumpf muss vorher vorhanden sein
 *  Buchungsnummer wird in docinput[Buchung_Nr] übergeben
 *  Formular und Submit-Button muss ebenfalls selbst gemqacht werden!
 */
function ZeigePersonalplanung($Buchung, $FormName ='')
{
   global $design;
  
    echo '<input type="hidden" name="docinput[Buchung_Nr]" value="'.$Buchung["Buchung_Nr"];
    echo '" />';
    echo '<table class="volleTabelle">';
    echo '<tr class="ueberschrift"><th colspan="2">Verantwortliche</th></tr>';
    echo '<tr class="dunkleZeile"><td>Anreise</td>';
    echo '<td>';
    show_users("Anreisename", CVJM_ANREISEGRUPPE, $Buchung["Anreisename"]);
    echo '</td></tr>';
    echo '<tr class="dunkleZeile"><td>Anreisedatum/-zeit</td>';
    echo '<td><input type="Text" name="docinput[Anreisedatum]" id="ADatum" value="';
    echo date("d.m.Y",$Buchung["Von"])." ";
    if ( $Buchung["Ankunftszeit"] != 0 ) echo date("H:i",$Buchung["Ankunftszeit"]);    
    echo '" size="16" maxlength="16" ';
    KalenderEvents($FormName, 'ADatum');
    echo '/> (tt.mm.jjjj ss:mm)</td></tr>';
    echo '<tr class="helleZeile"><td>Abnahme</td>';
    echo '<td>';
    show_users("Abnahmename", CVJM_ABNAHMEGRUPPE, $Buchung["Abnahmename"]);
    echo '</td></tr>';
    echo '<tr class="helleZeile"><td>Abnahmedatum/-zeit</td>';
    echo '<td><input type="Text" name="docinput[Abnahme]" id="AbDatum" value="';
    if ( $Buchung["Abnahme"] != 0 )
    {
      echo date("d.m.Y",$Buchung["Abnahme"]);
      if ( date("H:i",$Buchung["Abnahme"]) != "00:00" )
        echo date(" H:i",$Buchung["Abnahme"]);
    }
    echo '" size="16" maxlength="16" ';
    KalenderEvents($FormName, 'AbDatum');
    echo '/> (tt.mm.jjjj ss:mm)</td></tr>';
    echo '<tr class="helleZeile"><td>Abreisedatum/-zeit</td>';
    echo '<td><input type="Text" name="docinput[Abreisedatum]" id="AbrDatum" value="';
    echo date("d.m.Y",$Buchung["Bis"])." ";
    if ( $Buchung["Abfahrtszeit"] != 0 ) echo date("H:i",$Buchung["Abfahrtszeit"]);
    echo '" size="16" maxlength="16" ';
    KalenderEvents($FormName, 'AbrDatum');
    echo '/> (tt.mm.jjjj ss:mm)</td></tr>';
    echo '<tr class="dunkleZeile"><td>Rechnungsstellung</td>';
    echo '<td>';
    show_users("Rechnungname", CVJM_VERWALTUNGSGRUPPE, $Buchung["Rechnungname"]);
    echo '</td></tr>';
    echo '<tr class="dunkleZeile"><td>Rechnungsstellung Datum</td>';
    echo '<td><input type="Text" name="docinput[Rechnungsstellung]" id="RDatum" value="';
    if ( $Buchung["Rechnungsstellung"] != 0 )
    {
      echo date("d.m.Y",$Buchung["Rechnungsstellung"]);
      if ( date("H:i",$Buchung["Rechnungsstellung"]) != "00:00" )
        echo date(" H:i",$Buchung["Rechnungsstellung"]);
    }
    echo '" size="16" maxlength="16" ';
    KalenderEvents($FormName, 'RDatum');
    echo '/> (tt.mm.jjjj ss:mm)</td></tr>';
    echo '</table>';
}

function speicherePersonalplanung($docinput)
{
  // speichert die Personalplanung, wenn entsprechende Informationen in docinput
  // enthalten sind.
  if ( isset($docinput["Anreisedatum"]) && isset($docinput["Buchung_Nr"]) &&
       is_numeric($docinput["Buchung_Nr"]) )
  {
    // Speichern der Daten
    $sql = "UPDATE ".TABLE_BUCHUNGEN." SET ";
    $sql .= "Anreisename='".$docinput["Anreisename"]."',";
    //$sql .= "Abreisename='".$docinput["Abreisename"]."',";
    $zeit1 = convertDatumToTimestamp($docinput["Anreisedatum"]);
    $zeit2 = convertDatumToTimestamp($docinput["Abreisedatum"]);
    $abdatum = convertDatumToTimestamp($docinput["Abnahme"]);
    $Datum = convertDatumToTimestamp($docinput["Rechnungsstellung"]);
    $sql .= "Rechnungsstellung=".$Datum.",";
    $sql .= "Von=".strtotime(date("Y-m-d",$zeit1)).",";
    $sql .= "Ankunftszeit=".strtotime(date("H:i",$zeit1)).",";
    $sql .= "Bis=".strtotime(date("Y-m-d",$zeit2)).",";
    $sql .= "Abfahrtszeit=".strtotime(date("H:i",$zeit2)).",";
    $sql .= "Abnahmename='".$docinput["Abnahmename"]."',";
    $sql .= "Abnahme=".$abdatum.",";
    $sql .= "Rechnungname='".$docinput["Rechnungname"]."',";
    $sql .= 'Logtext=CONCAT("'.date("d.m.Y H:i").' Personaleinteilung '.get_user_nickname().'\n",LogText)';
    $sql .= " WHERE Buchung_Nr=".$docinput["Buchung_Nr"];
    if ( ! sql_query($sql) ) echo "Fehler: ".sql_error();
    echo "<strong>&gt;&gt;&gt; Personaldaten gespeichert.</strong><br />";
    // Mail an bestroffene senden
    if ( $zeit1 != 0 )
    {
      $Nachricht = "Personalplanung für Buchung ".$docinput["Buchung_Nr"].
        "\n\nAnreise am ".date("d.m.Y H:i",$zeit1)." : ";
      if ( is_numeric($docinput["Anreisename"]))
        $Nachricht .= get_user_nickname($docinput["Anreisename"]);
      else
        $Nachricht .= "-nicht festgelegt-";
    }
    if ( $zeit2 != 0 )
    {
      $Nachricht .= "\nAbnahme am ".date("d.m.Y H:i",$zeit2)." : ";
      if ( is_numeric($docinput["Abnahmename"]))
        $Nachricht .= get_user_nickname($docinput["Abnahmename"]);
      else
        $Nachricht .= "-nicht festgelegt-";
    }
    $zeit = convertDatumToTimestamp($docinput["Rechnungsstellung"]);
    if ( $zeit != 0 )
    {
      $Nachricht .= "\nRechnungsstellung am ".date("d.m.Y H:i",$zeit)." : ";
      if ( is_numeric($docinput["Rechnungname"]) )
        $Nachricht .= get_user_nickname($docinput["Rechnungname"]);
      else
        $Nachricht .= "-nicht festgelegt-";
    }
    if ( $zeit1 != 0 && is_numeric($docinput["Anreisename"]) )
       send_message (get_user_email($docinput["Anreisename"]),$Nachricht, SESSION_DBID,
      get_user_nickname(), '[Buchung '.$docinput["Buchung_Nr"].'] Personalplanung', 'email');
    if ( $zeit2 != 0 && is_numeric($docinput["Abreisename"]) )
       send_message (get_user_email($docinput["Abreisename"]),$Nachricht, SESSION_DBID,
      get_user_nickname(), '[Buchung '.$docinput["Buchung_Nr"].'] Personalplanung', 'email');
    if ( $zeit != 0 && is_numeric($docinput["Rechnungname"]) )
       send_message (get_user_email($docinput["Rechnungname"]),$Nachricht, SESSION_DBID,
      get_user_nickname(), '[Buchung '.$docinput["Buchung_Nr"].'] Personalplanung', 'email');
  }
  if ( isset($docinput["Entfernen"]) && isset($docinput["Buchung_Nr"]) &&
       is_numeric($docinput["Buchung_Nr"]) )
  {
  	$sql = 'UPDATE '.TABLE_BUCHUNGEN.' SET ';
    $sql .= $docinput['Entfernen'].'=NULL,';
    $sql .= 'Logtext=CONCAT("'.date("d.m.Y H:i").' '.$docinput['Entfernen'].
        ' entfernt von '.get_user_nickname().'\n",LogText)';
    $sql .= ' WHERE Buchung_Nr='.$docinput['Buchung_Nr'];
    if ( ! sql_query($sql) ) echo 'Fehler: '.sql_error();      
  }
}

?>