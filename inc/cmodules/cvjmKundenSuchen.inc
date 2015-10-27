<?php
/*
 * Created on 09.01.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
 
 if ( $profile["editor"] != 1)
 {
   if ( $document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
   include(INC_PATH."misc/CVJM.inc");  
   // Formular zur Kategorie-Auswahl
   // und Datum-Auswahl
   if ( ! isset($docinput["DatumBis"])) $docinput["DatumBis"] = date("d.m.Y");
   if ( ! is_array($docinput["Kategorie"])) $docinput["Kategorie"] = array();
   ?>
   <table>
   <form action="<?=get_url($id)?>" method="post">
   <tr>
     <td>
       Bestandteil des Kundennamens 
     </td>
     <td>
       <input type="text" name="docinput[Name]" value="<?=$docinput["Name"]?>"/>
     </td>
   </tr>
   <tr>
     <td>
       Kategorie(n) der Kunden
     </td>
     <td>
       <select name="docinput[Kategorie][]" multiple="multiple">
<?php
  if (!$query = sql_query("SELECT * FROM ".TABLE_KATEGORIEN." ORDER BY Kategorie")) 
    echo mysql_error();
  while ( $row = sql_fetch_array($query) )
  {
  	 echo '<option value="'.$row["Kategorie_id"].'" ';
  	 if ( in_array($row["Kategorie_id"], $docinput["Kategorie"]))
  	   echo 'selected="selected"';
     echo '>';
  	 echo $row["Kategorie"];
  	 echo "</option>\n";
  }
  sql_free_result($query);
?>       
       </select>
     </td>
   </tr>
   <tr>
     <td>
       Kunde zwischen
     </td>
     <td>
       <input type="text" name="docinput[DatumVon]" value="<?=$docinput["DatumVon"]?>" "/>
       und
       <input type="text" name="docinput[DatumBis]" value="<?=$docinput["DatumBis"]?>" />
     </td>
   </tr>    
   <tr>
     <td>
       <input type="submit" value="Suchen" />
     </td>
   </tr>
   </form>
   </table>  
   <?php
   if ( isset($docinput["Name"]) && is_array($docinput["Kategorie"]))
   {
     $sql = "SELECT DISTINCT Adressen_id, Name, Vorname, Strasse, PLZ, Ort, Zusatz, Telefon1 FROM (".
            TABLE_ADRESSEN." INNER JOIN ".TABLE_ADRESSEN_KATEGORIE.
            " ON ".TABLE_ADRESSEN_KATEGORIE.".F_Adressen_id=Adressen_id) ".
            "INNER JOIN ".TABLE_BUCHUNGEN." ON ".TABLE_BUCHUNGEN.
            ".F_Adressen_id=Adressen_id WHERE 1 ";
     if ( Count($docinput["Kategorie_id"]) > 0 )
       $sql .= "AND F_Kategorie_id IN (".
            implode(",",$docinput["Kategorie"]).")";
     if ( $docinput["Name"] != "" )
     {
     	$docinput["Name"] = mysql_real_escape_string($docinput["Name"]);     	
     	$sql .= " AND Name LIKE '%{$docinput["Name"]}%'";
     }

     if ( $docinput["DatumVon"] != "" )
     {
     	$docinput["DatumVon"] = convertDatumToTimestamp($docinput["DatumVon"]);
     	$sql .= " AND Von >= {$docinput["DatumVon"]}";
     }
     if ( $docinput["DatumBis"] != "" )
     {
     	$docinput["DatumBis"] = convertDatumToTimestamp($docinput["DatumBis"]);
     	$sql .= " AND Bis <= {$docinput["DatumBis"]}";
     }
     if (!$query = sql_query($sql)) echo mysql_error();
     echo '<table border="1">';
     echo "<tr><th>Name</th><th>Adresse</th><th>Telefon</th></tr>\n";
     while ( $row = sql_fetch_array($query))
     {
     	echo "<tr><td>\n";
     	echo '<a href="'.get_url(get_cmodule_id("cvjmadressen"),"Bearbeiten=".
     	  $row["Adressen_id"]).'">';
     	echo $row["Name"].",".$row["Vorname"]."</a>";
     	if ( $row[Zusatz] != "" ) echo "<br />".nl2br($row["Zusatz"]);
     	echo "</td>";
        echo "<td>";
        echo $row["Strasse"]."<br />{$row["PLZ"]} {$row["Ort"]}</td>\n";
        echo "<td>{$row["Telefon1"]}</td></tr>\n";
     }
     if ( mysql_num_rows($query) == 0)
       echo '<tr><td colspan="3">Keine passenden Kunden gefunden!</td></tr>';
     echo "</table>\n";
     sql_free_result($query);		
   }
 }
 
?>
