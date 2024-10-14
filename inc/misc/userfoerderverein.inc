<?php
load_lang('user');
global $profile;
if ( SESSION_STATUS == "ok" && is_numeric(SESSION_DBID)) {
  if(isset($docinput['update_user']) && $docinput['update_user'] == 'true' && isset($_REQUEST["dbdata"])) {
    // Datenbank aktualisieren
    $dbdata = $_REQUEST["dbdata"];
    $sql = "UPDATE fv_mitglieder SET ";
    foreach ($dbdata as $key => $value) {
      $value = addslashes(str_replace('"', "", stripslashes($value)));  // " wegmachen wegen HTML
      $sql .= $key."='".transform($value,"clean")."',";
    }
    $sql = substr($sql, 0, strlen($sql)-1); // letztes Komma wegschneiden
    $sql .= " WHERE awf_id = ".SESSION_DBID;
    //echo $sql;
    if ( ! sql_query($sql) ) echo "Update-Datenbankfehler: ".sql_error();
  }
  $sql = "SELECT * FROM fv_mitglieder WHERE awf_id = ".SESSION_DBID;
  $qu = sql_query($sql);
  if ( sql_num_rows($qu) == 0 ) {
    // Neuen User anlegen
    sql_free_result($qu);
    $sql2 = "INSERT INTO fv_mitglieder (awf_id, Name, Email_p) VALUES (".SESSION_DBID.",'".$profile["nickname"]."','".SESSION_EMAIL."')";
    if ( ! sql_query($sql2) ) echo "Einf&uuml;gefehler: ".sql_error();
    $qu = sql_query($sql);
  }
  if ( $row = sql_fetch_array($qu) ) {
    echo $design['table_begin'];
    echo '<tr bgcolor="'. $design['table_row_dark'] . '"><td>';
    echo '<b>Im System gespeicherte Daten:</b></td></tr>';
    echo '<tr bgcolor="'.$design['table_row_bright'].'"><td>';
    echo 'Name<br />';
    echo '<input type="text" name="dbdata[Name]" value="'.$row['Name'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Vorname<br />';
    echo '<input type="text" name="dbdata[Vorname]" value="'.$row['Vorname'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Anrede1<br />';
    echo '<input type="text" name="dbdata[Anrede1]" value="'.$row['Anrede1'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Anrede2<br />';
    echo '<input type="text" name="dbdata[Anrede2]" value="'.$row['Anrede2'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Zusatz<br />';
    echo '<input type="text" name="dbdata[Zusatz]" value="'.$row['Zusatz'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Strasse<br />';
    echo '<input type="text" name="dbdata[Strasse]" value="'.$row['Strasse'].'" size="60" maxlength="60" />';
    echo '<br />';

    echo 'PLZ&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ort<br />';
    echo '<input type="text" name="dbdata[PLZ]" value="'.$row['PLZ'].'" size="10" maxlength="10" />';
    echo '<input type="text" name="dbdata[Ort]" value="'.$row['Ort'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Telefon privat &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Telefon dienstlich<br />';
    echo '<input type="text" name="dbdata[Telefon_p]" value="'.$row['Telefon_p'].'" size="20" maxlength="20" />';
    echo '<input type="text" name="dbdata[Telefon_d]" value="'.$row['Telefon_d'].'" size="20" maxlength="20" />';
    echo '<br />';
    echo 'Email privat<br />';
    echo '<input type="text" name="dbdata[Email_p]" value="'.$row['Email_p'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Email dienstlich<br />';
    echo '<input type="text" name="dbdata[Email_d]" value="'.$row['Email_d'].'" size="50" maxlength="50" />';
    echo '<br />';
    echo 'Geburtsdatum (bitte im Format: Jahr-Monat-Tag)<br />';
    echo '<input type="date" name="dbdata[Gebdatum]" value="'.$row['Gebdatum'].'" size="10" maxlength="10" />';
    echo 'jjjj-mm-tt<br />';
    echo 'Beruf, Erfahrungen, besondere F&auml;higkeiten<br />';
    echo '<textarea name="dbdata[Erfahrung]" cols="50" rows="4">';
    echo $row['Erfahrung'] ;
    echo '</textarea>';

    echo '<br />';
          if ( $row['Mitglied'] == "J" or $row['Mitglied'] == "j" ) {
          echo '<hr>';
          echo '<b>Im System gespeicherte Mitgliedsdaten:</b><br />';
          echo 'J&auml;hrlicher Beitrag in Euro&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ';
          echo '<input type="text" name="dbdata[Beitrag]" value="'.$row['Beitrag'].'" size="12" maxlength="12" />';
          echo '<br />';
          echo 'Zahlungsart:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
          echo '<input type="text" name="dbdata[Zahlungsart]" value="'.$row['Zahlungsart'].'" size="20" maxlength="20" />';
          echo '<br />';
          echo 'Bankname<br />';
          echo '<input type="text" name="dbdata[Bankname]" value="'.$row['Bankname'].'" size="50" maxlength="50" />';
          echo '<br />';
          echo 'Bankleitzahl<br />';
          echo '<input type="text" name="dbdata[BLZ]" value="'.$row['BLZ'].'" size="14" maxlength="14" />';
          echo '<br />';
          echo 'Kontonummer<br />';
          echo '<input type="text" name="dbdata[KontoNr]" value="'.$row['KontoNr'].'" size="30" maxlength="30" />';
          echo '<br />';
          }
          // Online&uuml;berweisung
          if ( !$row['Bankkonto_Online'] == ""  ) {
          echo '<br /><hr><br /><b>Diese Daten haben wir f&uuml;r weitere Online&uuml;berweisungen f&uuml;r Sie hinterlegt:</b><br />';
          echo 'Bankname f&uuml;r die Online&uuml;berweisung<br />';
          echo '<input type="text" name="dbdata[Bankname_Online]" value="'.$row['Bankname_Online'].'" size="50" maxlength="50" />';
          echo '<br />';
          echo 'Kontonummer f&uuml;r die Online&uuml;berweisung<br />';
          echo '<input type="text" name="dbdata[Bankkonto_Online]" value="'.$row['Bankkonto_Online'].'" size="15" maxlength="15" />';
          echo '<br />';
          echo 'Bankleitzahl f&uuml;r die Online&uuml;berweisung<br />';
          echo '<input type="text" name="dbdata[Bankleitzahl_Online]" value="'.$row['Bankleitzahl_Online'].'" size="8" maxlength="8" />';
          echo '<br />';
          }
    echo '</td></tr>'.$design['table_end'];
    echo '<br /><br />';
  }
  else
    echo "Datenbankfehler: ".sql_error();
}
?>