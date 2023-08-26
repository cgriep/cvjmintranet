<?php

/* Zusatzdaten zu Usern anzeigen */

if ( $profile['editor'] != 1 ) {
  if ( isset($_REQUEST["User"]) && is_numeric($_REQUEST["User"])) {
    $prof = get_profile($_REQUEST["User"]);
    if ( isset($_REQUEST["Update"]) ) {
      $daten = $_REQUEST["daten"];
      $entf = $_REQUEST["Entfernen"];
      foreach ($daten as $key => $value)
      {
        echo "$key ".$daten[$key]." ".$prof[$key]." ".$entf[$key]."<br />";
        if ( $entf[$key] == "v" )
          remove_profile($key, $_REQUEST["User"]);
        elseif ( $prof[$key] != $daten[$key] )
          set_profile($key, $daten[$key], $_REQUEST["User"]);
      }
      if ( $_REQUEST["Feld"] != "" && $_REQUEST["Wert"] != "" )
        set_profile($_REQUEST["Feld"], $_REQUEST["Wert"], $_REQUEST["User"]);
      $prof = get_profile($_REQUEST["User"]);
    }
    // Einzelnen User bearbeiten
    echo '<table border="1"><tr><td>';
    echo '<form action="'.get_url($id).'" method="post">';
    echo '<input type="hidden" name="User" value="'.$_REQUEST["User"].'">';
    echo '<table border="0" cellspacing="0">';
    echo '<tr><th colspan="3">'.get_user_nickname($_REQUEST["User"]).'</th></tr>';
    echo '<tr><th>Feld</th><th>Wert</th><th>Feld<br />entfernen</th></tr>';
    foreach ($prof as $key => $value) {
      echo '<tr><td>'.$key.'</td><td><input type="Text" name="daten['.$key.']" value="';
      echo $value.'"></td><td align="center">';
      echo '<input type="Checkbox" name="Entfernen['.$key.']" value="v">';
      echo '</td></tr>';
    }
    echo '<tr><td><input type="text" name="Feld" value=""></td>';
    echo '<td><input type="Text" name="Wert" size="" maxlength=""></td><td></td></tr>';
    echo '<tr><td colspan="3"><input type="Submit" name="Update" value="Update"></td></tr>';
    echo $design["table_end"]."</form></td></tr></table><br />";
  }
  elseif ( isset($_REQUEST["Marker"]) ) {
    // mehrere User bearbeiten
  }
  echo '<br /><table border="1"><tr><td><form action="'.get_url($id).'" method="post">';
  echo '<table border="0" cellspacing="0">';
  echo '<tr><th>Name</th><th></th></tr>';
  $qres = sql_query ("SELECT id FROM ".TABLE_USERS);
  while($row = sql_fetch_row($qres)) {
    echo '<tr><td>';
    echo '<a href="?id='.$id.'&User='.$row[0].'">'.get_user_nickname($row[0])."</a>";
    echo '</td><td>';
    echo '<input type="Checkbox" name="Marker[]" value="'.$row[0].'">';
    echo '</td></tr>';
    echo '<td>';
  }
  echo $design['table_end'];
  sql_free_result($qres);
  echo '<br /><input type="Submit" value="Feld hinzuf�gen">';
  echo '</form><br /></td></tr></table>';
}
?>