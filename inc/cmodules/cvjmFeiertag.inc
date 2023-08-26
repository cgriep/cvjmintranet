<?php

/*

Modul cvjmfeiertag
AWF-Name: cvjmFeiertag.inc

Verwaltung von Feiertagen

*/

if ( $profile["editor"] != "1" )
{
  if ( $document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  include(INC_PATH."misc/CVJM.inc");
  include(INC_PATH."misc/Kalenderjs.inc");
  KalenderScriptEinfuegen();
  if ( isset($docinput["Datum"]) )
  {
    // Speichern
    $docinput["Datum"] = convertDatumToTimestamp($docinput["Datum"]);
    if ( isset($docinput["Feiertag"]) && is_numeric($docinput["Feiertag"]) )
      $sql = "UPDATE ".TABLE_FEIERTAGE." SET Bezeichnung='".
        $docinput["Bezeichnung"]."', Datum=".$docinput["Datum"].
        " WHERE id=".$docinput["Feiertag"];
    else
      $sql = "INSERT INTO ".TABLE_FEIERTAGE." (Bezeichnung, Datum) VALUES ('".
        $docinput["Bezeichnung"]."',".$docinput["Datum"].")";
    if ( sql_query($sql) )
      echo "&gt;&gt;&gt; Daten gespeichert.<br />";
    else
      echo "Fehler: ".sql_error();
  }
  if ( isset($docinput["Feiertag"]) && is_numeric($docinput["Feiertag"]) )
  {
    $query = sql_query("SELECT * FROM ".TABLE_FEIERTAGE." WHERE id = ".$docinput["Feiertag"]);
    if ( $Feiertag = sql_fetch_array($query) )
    {
    }
    sql_free_result($query);
  }
  echo '<form action="'.get_url($id).'" method="post" name="TagForm">';
  if ( isset($Feiertag["id"]) )
  {
    echo '<input type="hidden" name="docinput[Feiertag]" value="';
    echo $Feiertag["id"].'" />';
  }
  echo '<table>';
  echo '<tr><td>Datum</td><td><input type="Text" name="docinput[Datum]" value="';
  echo date("d.m.Y",$Feiertag["Datum"]).'" size="10" maxlength="10"';
  KalenderEvents("TagForm","docinput[Datum]");
  echo ' /></td></tr>';
  echo '<tr><td>Bezeichnung</td><td><input type="Text" name="docinput[Bezeichnung]" value="';
  echo $Feiertag["Bezeichnung"].'" size="30" maxlength="30" /></td></tr>';
  echo '<tr><td colspan="2"><input type="Submit" value="Speichern" /></td></tr>';
  echo '</table>';
  echo '</form>';
  echo '<hr />';
  echo $design["table_begin"];
  echo '<tr><th>Datum</th><th>Bezeichnung</th></tr>';
  $query = sql_query("SELECT * FROM ".TABLE_FEIERTAGE." ORDER BY Datum DESC");
  $Color = $design["table_row_bright"];
  while ( $tag = sql_fetch_array($query) )
  {
    echo '<tr bgcolor="'.$Color.'"><td>';
    echo date("d.m.Y",$tag["Datum"]);
    echo '</td><td><a href="'.get_url($id,"docinput[Feiertag]=".$tag["id"]).'">';
    echo stripslashes($tag["Bezeichnung"])."</a>";
    echo "</td></tr>\n";
    if ( $Color == $design["table_row_bright"] )
        $Color = $design["table_row_dark"];
    else
        $Color = $design["table_row_bright"];
  }
  sql_free_result($query);
  echo $design["table_end"];
}

?>