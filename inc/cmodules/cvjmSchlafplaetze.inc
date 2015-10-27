<?php
/*

cvjmSchlafplaetze
AWF-Name: cvjmschlafplaetze
entfällt sobald die Schlafplätze komplett in die Artikel integriert sind
*/

if ( $profile["editor"] != 1 )
{
  include(INC_PATH."misc/CVJM.inc");
  if ( $document['title'] != '')
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  if ( isset($docinput["SP"]) )
  {
    // Speichern
    while ( list ($key, $value) = each($docinput["SP"]) )
    {
      if (is_numeric($value) )
      {
        if ( $value < 0 )
          sql_query("DELETE FROM ".TABLE_SCHLAFPLAETZE." WHERE Artikel_Nr =".$key);
        else
        {
          $sql = "UPDATE ".TABLE_SCHLAFPLAETZE." SET Schlafplaetze=".$value.
            " WHERE Artikel_Nr=".$key;
          if ( ! sql_query($sql) )
            sql_query("INSERT INTO ".TABLE_SCHLAFPLAETZE." (Artikel_Nr, Schlafplaetze) ".
              "VALUES (".$key.",".$value.")");
        }
      }
      else
          echo '&gt;&gt;&gt; Unzulässiger Wert $value - Zahl erwartet.<br />';
    }
  }
  $sql = "SELECT * FROM ".TABLE_ARTIKEL." WHERE F_Art_id=".CVJMART_ORT.
    " ORDER BY Position";
  if ( ! $query = sql_query($sql)) echo mysql_error();
  echo '<table>';
  echo '<form action="'.get_url($id).'" method="post">';
  echo '<tr><td colspan="2"><input type="Submit" value="Speichern"></td></tr>';
  echo '<tr><td>Schlafplätze</td><td>Bezeichnung</td></tr>';
  $Color = $design["table_row_bright"];
  while ( $artikel = sql_fetch_array($query) )
  {
    echo '<tr bgcolor="'.$Color.'">';
    echo '<td>';
    echo '<input type="Text" name="docinput[SP]['.$artikel["id"].']" value="';
    $sq = sql_query("SELECT Schlafplaetze FROM ".TABLE_SCHLAFPLAETZE.
      " WHERE Artikel_Nr=".$artikel["id"]);
    if ( $schlafplatz = sql_fetch_row($sq) )
      echo $schlafplatz[0];
    else
      echo "-1";
    sql_free_result($sq);
    echo '" size="3" maxlength="3" />';
    echo '</td>';
    echo '<td>'.$artikel["Bezeichnung"].'</td>';
    echo "</tr>\n";
    if ( $Color == $design["table_row_bright"] )
      $Color = $design["table_row_dark"];
    else
      $Color = $design["table_row_bright"];
  }
  sql_free_result($query);
  echo '<tr><td colspan="2"><input type="Submit" value="Speichern"></td></tr>';
  echo '</form>';
  echo '</table>';
  echo "Geben Sie -1 ein wenn an einem Ort keine Schlafplätze zur Verfügung stehen! 0 wird beim Zeltplatz verwendet.";
}

?>