<!--  Template Buchung Schlüsselliste -->
{popup_init src="/javascript/overlib.js"}

<table>
<tr class="ueberschrift"><th colspan="5">ausgegebene Schlüssel
 {if $Buchung->schluesselAnzahl() > 0} (bisher {$Buchung->schluesselAnzahl()} ausgegeben){/if}
<tr>
{foreach item=

<input type="text" name="docinput[Ort][  
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