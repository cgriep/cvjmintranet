<?php

/* Modul, um eine Textdatei in eine Datenbank zu lesen

*/

if ( $profile['editor'] != "1" ) {
  if ( isset($_FILES['Listendatei']['tmp_name']) &&
       $_FILES['Listendatei']['tmp_name'] != '' )
  {
    $handle = fopen($_FILES['Listendatei']['tmp_name'],"r");
    // &Uuml;berschrift lesen
    $ueberzeile = fgets($handle, 4096);
    $felder = explode(";",$ueberzeile);
    while ( ! feof($handle) ) {
      $zeile = fgets($handle, 4096);
      $daten = explode(";",$zeile);
      reset($felder);
      $sql = "UPDATE Schulen SET ";
      while ( list($key, $value) = each($daten) ) {
        list($key, $v) = each($felder);
        $value = trim($value);
        $v = trim($v);
        if ( $v == "Bezirk" ) $Bezirk = $value;
        if ( $v == "Schule" ) $Schulname = $value;
        if ( $v == "SchulNr" ) $SchulNr = $value;
        if ( $v == "Art" ) $Schulart = $value;
        if ( $v == "Art" ) {
          if ( ! (strpos($value, "KTL") === false) ) $Nr = 0;
	 if ( ! (strpos($value, "KTL2") === false) ) $Nr = 1;
          if ( $value == "Koord" ) $Nr = -1;
        }
        if ( $v == "Nummer" ) {
          while ( strlen($value) < 3 ) $value = "0".$value;
          $eMail = "sucht$value@suchtprophylaxe-berlin.schule.de";
          $sql .= "eMail='$eMail',";
        }
        if ( $v == "Name" ) $Name = $value;
        if ( $v == "Vorname" ) {
          $Name = $value." ".$Name;
          $sql .= "Kontaktlehrer='".trim($Name)."',";
        }
      }
      $sql = substr($sql, 0, strlen($sql)-1);
      $sql .= " WHERE SchulNr = '$SchulNr' AND Nummer = $Nr";
      $qu = sql_query("SELECT * FROM Schulen WHERE SchulNr = '$SchulNr' AND Nummer = $Nr");
      if ( $row = sql_fetch_array($qu) ) {
        echo "vorhanden $SchulNr / $Nr : ".$row["eMail"]." - ".$row["Kontaktlehrer"]."<br />";
      }
      else {
        echo "Kein Datensatz vorhanden: $SchulNr / $Nr<br />";
        if ( Trim($Schulname) != "" )
        if ( ! sql_query("INSERT INTO Schulen (SchulNr, Nummer, Schulname, Bezirk, Schulart) " .
          " VALUES ('$SchulNr',$Nr, '$Schulname', '$Bezirk', '$Schulart')") )
          echo "<i>Hinzuf�gen misslungen</i>";
      }
      sql_free_result($qu);
      //echo $sql;
      //echo "<br />";
      if ( trim($Name) != "" )
      if ( !sql_query($sql) )
        echo "<b>misslungen</b> $SchulNr / $Nr <br />";
      else {
        //echo sql_affected_rows () ." geändert.<br />";
        $qu = sql_query("SELECT * FROM Schulen WHERE eMail = '$eMail'");
        if ( sql_num_rows($qu) == 0)
        {
          echo "<b>$eMail ($Schulname/$Bezirk) nicht gefunden</b><br />";
        }
        else {
          $row = sql_fetch_array($qu);
          echo "Eingetragen: ";
          if ( $row[Kontaktlehrer] <> $Name ) echo "<b>$row[eMail]: $row[Kontaktlehrer]- $Name / $Bezirk</b><br />";
          else
            echo "$row[Kontaktlehrer]- $Name / $Bezirk<br />";
        }
        sql_free_result($qu);
        // Gruppen eintragen
        $ktlid = get_user_id($eMail);
        if ( $ktlid > 0 ) {
          echo "User $ktlid $Name<br />";
          if ( $Nr >= 0 )  // Kontaktlehrer
            add_user_to_group($ktlid, 3 , false);  // Gruppe Kontaktlehrer
          else
            remove_user_from_group($ktlid, 3, false);
          if ( get_user_nickname($ktlid) == email_to_string(get_user_email($ktlid)) && trim($Name) != "" ) {
            set_profile("nickname", $Name, $ktlid);
            echo "Nickname eingetragen<br />";
          }
          remove_user_from_group($ktlid, 15, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 18, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 16, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 12, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 8, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 11, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 9, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 14, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 13, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 17, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 10, false);  // Gruppe Kontaktlehrer
          remove_user_from_group($ktlid, 7, false);  // Gruppe Kontaktlehrer
          echo "Bezirk $Bezirk<br />";
          switch ( $Bezirk ) {
            case 'Mitte':
              echo "$Bezirk";
              add_user_to_group($ktlid, 15, false);  // Gruppe Kontaktlehrer
              break;
            case 'Friedrichshain-Kreuzberg':
              echo "$Bezirk";
              add_user_to_group($ktlid, 18, false);  // Gruppe Kontaktlehrer
              break;
            case 'Pankow':
              echo "$Bezirk";
              add_user_to_group($ktlid, 16, false);  // Gruppe Kontaktlehrer
              break;
            case 'Charlottenburg-Wilmersdorf':
              echo "$Bezirk";
              add_user_to_group($ktlid, 12, false);  // Gruppe Kontaktlehrer
              break;
            case 'Spandau':
              echo "$Bezirk";
              add_user_to_group($ktlid, 8, false);  // Gruppe Kontaktlehrer
              break;
            case 'Steglitz-Zehlendorf':
              echo "$Bezirk";
              add_user_to_group($ktlid, 11, false);  // Gruppe Kontaktlehrer
              break;
            case 'Tempelhof-Sch�neberg':
              echo "$Bezirk";
              add_user_to_group($ktlid, 9, false);  // Gruppe Kontaktlehrer
              break;
            case 'Neuk�lln':
              echo "$Bezirk";
              add_user_to_group($ktlid, 14, false);  // Gruppe Kontaktlehrer
              break;
            case 'Treptow-K�penick':
              echo "$Bezirk";
              add_user_to_group($ktlid, 17, false);  // Gruppe Kontaktlehrer
              break;
            case 'Marzahn-Hellersdorf':
              echo "$Bezirk";
              add_user_to_group($ktlid, 10, false);  // Gruppe Kontaktlehrer
              break;
            case 'Lichtenberg':
              echo "$Bezirk";
              add_user_to_group($ktlid, 13, false);  // Gruppe Kontaktlehrer
              break;
            case 'Reinickendorf':
              echo "$Bezirk";
              add_user_to_group($ktlid, 7, false);  // Gruppe Kontaktlehrer
              break;
          }
        }
        else
          echo "User $eMail nicht gefunden!<br />";
      }
    }
    fclose($handle);
  }
  elseif ( isset($_REQUEST["export"] ))
  {
    $sql = "SELECT * FROM Schulen";
    $qu = sql_query($sql);
    $handle = fopen('public/schulen.xls','w');
    fwrite($handle, '"Schulnr"'."\t".'"Schulnr alt"'."\t".'"Bezirk"'."\t".'"Schulname"'."\t".
'"Schulart"'."\t".'"KTL"'."\t".'"E-Mail"'."\t".'"Stand"'."\t".'"Nummer"'."\t".
'"MindManager"'."\t".'"KTLBeginn"'."\t".'"Telefon"'."\t".'"Koordinator"'."\t".'"Lehrerart"'."\n");
    
    while ( $row = sql_fetch_array($qu)) {
      fwrite($handle, '"\''.$row[SchulNr].'"'."\t".'"'.$row[aSchulNr].'"'."\t".'"'.$row[Bezirk].'"'.
"\t".'"'.$row[Schulname].'"'."\t".'"'.$row[Schulart].
      '"'."\t".'"'.$row[Kontaktlehrer].'"'."\t".'"'.$row[eMail].'"'."\t".'"'.$row[Stand].'"'."\t".
      		$row[Nummer]."\t".$row[MindManager].
      "\t".'"'.$row[KTLBeginn].'"'."\t".'"'.$row[Telefon].'"'."\t".'"'.$row[Koordinator].'"'."\t".'"'.
      $row['Art'].'"'."\n");
    }
    fclose($handle);
    echo 'Daten wurden exportiert. Klicken Sie hier zum <a href="public/schulen.xls">Herunterladen</a><br />';
    sql_free_result($qu);
  }
  else
  {
  	?>
<form action="?id=<?=$id?>" method="post" enctype="multipart/form-data">
<input type="file" name="Listendatei"><br />
<input type="submit" value="Datei einlesen">
</form><br />
<form action="?id=<?=$id?>" method="post" enctype="multipart/form-data">
<input type="submit" name="export" value="Datei exportieren">
</form>
<?php
  }
}

?>