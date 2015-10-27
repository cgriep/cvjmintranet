<html>
<body>
<?php
  if ( isset($_FILES['KAnfragedatei']['tmp_name']) &&
       $_FILES['KAnfragedatei']['tmp_name'] != '' )
  {
    mysql_connect("localhost","root","jopodata");
    mysql_select_db("awf");
    $handle = fopen($_FILES['KAnfragedatei']['tmp_name'],"r");
    // &Uuml;berschrift lesen
    $sql = "INSERT INTO cvjm_Adressen_Kategorie ( F_Adressen_id, F_Kategorie_id ) VALUES (";
    while ( ! feof($handle) ) {
      $zeile = fgets($handle, 4096);
      $sql2 = $sql;
      echo $zeile."<br />";
      $query = mysql_query("SELECT Adressen_id FROM cvjm_Adressen WHERE Kunden_Nr= ".$zeile);
      $row = mysql_fetch_array($query);
      mysql_free_result($query);
      $sql2 .= $row["Adressen_id"].",40)";
      if ( ! mysql_query($sql2) ) echo $sql2." ".mysql_error();
      else echo "Hinzugefügt<br />";
      echo $sql2."<br />";
    }
    fclose($handle);
    mysql_close();
  }
  if ( isset($_FILES['Anfragedatei']['tmp_name']) &&
       $_FILES['Anfragedatei']['tmp_name'] != '' )
  {
    mysql_connect("localhost","root","jopodata");
    mysql_select_db("awf");
    $handle = fopen($_FILES['Anfragedatei']['tmp_name'],"r");
    // &Uuml;berschrift lesen
    $ueberzeile = fgets($handle, 4096);
    $ueberzeile = str_replace('"','', $ueberzeile);
    $felder = explode(";",$ueberzeile);
    $sql = "INSERT INTO cvjm_Buchungen ( ";
    $sql .= implode(",",$felder);
    $sql .= ") VALUES (";
    while ( ! feof($handle) ) {
      $zeile = fgets($handle, 4096);
      $daten = explode(";",$zeile);
      $sql2 = $sql;
      while ( list($key, $value) = each($daten) ) {
        echo ".";
        $value = trim($value);
        if ( $value == "" ) $value = "''";
        if ( substr_count($value,".") == 2 && substr($value,0,1) != '"')
        {
          // Datum
               $tag = substr($value,0,strpos($value,"."));
               $value = substr($value,strpos($value,".")+1);
               $monat = substr($value, 0, strpos($value,"."));
               $value = substr($value,strpos($value,".")+1);
               $jahr = substr($value, 0, strpos($value," "));
               $value = strtotime($jahr."-".$monat."-".$tag);
        }
        if ( strpos('"', $value) === false )
          $value = str_replace(",",".", $value);
        $sql2 .= $value.",";
      }
      $sql2 = substr($sql2, 0, strlen($sql2)-1);
      $sql2.= ")";
      if ( ! mysql_query($sql2) ) echo mysql_error();
      else echo "Hinzugefügt<br />";
      echo $sql2."<br />";
    }
    fclose($handle);
    mysql_close();
  }
  if ( isset($_FILES['Listendatei']['tmp_name']) &&
       $_FILES['Listendatei']['tmp_name'] != '' )
  {
    mysql_connect("localhost","root","jopodata");
    mysql_select_db("awf");
    $handle = fopen($_FILES['Listendatei']['tmp_name'],"r");
    // &Uuml;berschrift lesen
    $ueberzeile = fgets($handle, 4096);
    $ueberzeile = str_replace('"','', $ueberzeile);
    $felder = explode(";",$ueberzeile);
    $sql = "INSERT INTO cvjm_Adressen ( ";
    $sql .= implode(",",$felder);
    $sql .= ") VALUES (";
    while ( ! feof($handle) ) {
      $zeile = fgets($handle, 4096);
      $daten = explode(";",$zeile);
      $sql2 = $sql;
      while ( list($key, $value) = each($daten) ) {
        echo ".";
        $value = trim($value);
        if ( $value == "" ) $value = "''";
        if ( strpos('"', $value) === false )
          $value = str_replace(",",".", $value);
        $sql2 .= $value.",";
      }
      $sql2 = substr($sql2, 0, strlen($sql2)-1);
      $sql2.= ")";
      if ( ! mysql_query($sql2) ) echo mysql_error();
      else echo "Hinzugefügt<br />";
      echo $sql2."<br />";
    }
    fclose($handle);
    mysql_close();
  }
?>

<form action="<?=$_SERVER["PHP_SELF"]?>" method="post" enctype="multipart/form-data">
Adressen <input type="file" name="Listendatei"><br />
<input type="submit" value="Datei einlesen">
</form><br />
<form action="<?=$_SERVER["PHP_SELF"]?>" method="post" enctype="multipart/form-data">
Anfrage <input type="file" name="Anfragedatei"><br />
<input type="submit" value="Datei einlesen">
</form><br />
<form action="<?=$_SERVER["PHP_SELF"]?>" method="post" enctype="multipart/form-data">
Konfi <input type="file" name="KAnfragedatei"><br />
<input type="submit" value="Datei einlesen">
</form><br />
</body>
</html>