<?php

/*
  cvjmArtikelarten.inc
  AWF-Name: cvjmartikelarten
  Speichert und ändert die Artikelarten.

*/

if ( $profile["editor"] != 1 )
{
  include(INC_PATH."misc/CVJM.inc");
  if ( $document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  if ( isset($docinput["DelArt"]) && is_numeric($docinput["DelArt"]) )
  {
    sql_query("DELETE FROM ".TABLE_ARTIKELARTEN." WHERE Art_id=".$docinput["DelArt"]);
  }
  if ( isset($docinput["Art"]) && is_array($docinput["Art"]) )
  {
     // Datensätze speichern
     foreach ( $docinput["Art"] as $key => $value )
     {
       if ( is_numeric($docinput["Typ"][$key]) && is_numeric($key) )
       {
         if ( $key == -1 && trim($value) != "" )
         {
           // Neue Art
           sql_query("INSERT INTO ".TABLE_ARTIKELARTEN." (Art, Typ) VALUES ('".
             addslashes($value)."',".$docinput["Typ"][$key].")");
         }
         else
         {
           // Update
           $sql = "UPDATE ".TABLE_ARTIKELARTEN." SET Art='".addslashes($value).
             "', Typ=".$docinput["Typ"][$key]." WHERE Art_id=".$key;
           sql_query($sql);
         }
       }
     }

  }
  if (!$query = sql_query("SELECT Art_id, Art, Typ, Count(id) AS Anz FROM ".TABLE_ARTIKELARTEN." LEFT JOIN ".
    TABLE_ARTIKEL." ON Art_id=F_Art_id GROUP BY Art, Art_id, Typ ORDER BY Art"))
     echo mysql_error();
  echo '<form action="'.get_url($id).'" method="post">';
  echo '<table width="100%">';
  echo '<tr bgcolor="'.$design["table_header"].
    '"><th></th><th>Artikelkategorie</th><th>Abrechnungsart</th><th>Anz</th></tr>';
  while ( $art = sql_fetch_array($query) )
  {
    echo '<tr><td>'.$art["Art_id"]."</td><td>";
    echo '<input type="Text" name="docinput[Art]['.$art["Art_id"].']" value="';
    echo htmlentities(stripslashes($art["Art"]));
    echo '" size="25" maxlength="25" />';
    echo '</td><td>';
    echo '<select name="docinput[Typ]['.$art["Art_id"].']">';
    foreach ( $Abrechnungstypen as $key => $value )
    {
      echo '<option value="'.$key.'" ';
      if ( $art["Typ"] == $key ) echo 'selected="selected"';
      echo '>'.$value.'</option>';
    }
    echo '</select><td>';
    if ( $art["Anz"] == 0 )
      echo '<a href="'.get_url($id,"docinput[DelArt]=".$art['Art_id']).'">Löschen</a>';
    else
      echo "(".$art["Anz"].")";
    echo "</td></tr>\n";
  }
  sql_free_result($query);
  echo '<tr><td>Neu:</td>';
  echo '<td><input type="Text" name="docinput[Art][-1]" size="25" maxlength="25" />';
  echo '</td><td>';
  echo '<select name="docinput[Typ][-1]">';
  reset($Abrechnungstypen);
  foreach ( $Abrechnungstypen as $key => $value )
    echo '<option value="'.$key.'">'.$value.'</option>';
  echo '</select></td></tr>';
  echo '</table>';
  echo '<input type="Submit" value="Speichern" />';
  echo '</form>';
}