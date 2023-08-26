<?php


/*
   cvjmArtikelliste
   AWF-Name: cvjmartikelliste

*/

if ( $profile['editor'] != 1 )
{
  if ( $document['title'] != '')
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';      
  include(INC_PATH.'misc/CVJM.inc');
  echo '<form action="'.get_url($id).'" method="post" enctype="multipart/form-data">';
  echo '<select name="docinput[Preisliste]">';
  
  echo '</select> ';
  echo '<select name="docinput[Lieferant]">';
  
  echo '</select> ';
  echo 'Datendatei <input type="file" name="Datei" />';
  echo '<input type="Submit" value="Artikel importieren" />';
  echo '</form>';
  // Kategorie
  // Lieferant
  // Position
  // Prozentsatz
  // gwg
  // Rabattf.
  // Anzeigen
  
  if ( isset($_FILES["Datei"]) && $_FILES["Datei"]["name"] != "" )
  {
    $insertsql = 'INSERT INTO '.TABELLE_ARTIKEL.
      '(F_Lieferant_id, Rabattfaehig,Einruecken,Geringwertig,F_MWST,' .
      'Anzeigen, Bezeichnung, Barcode,Einheit,Einkaufspreis,Position' .
      ') VALUES ('.$Lieferantid.','.$Rabattfaehig.','.$Einruecken.','.
      $Geringwertig.','.$MWST.',';
    $ueberschriften = fgets($dat);
    $Header = explode(';',$ueberschriften);
    while ( !feof($dat))
    {
     $buffer = fgets($dat);
     $feld = 0;
     if ( trim($buffer) != "" )
     {
       $buch = explode(";",$buffer);
       // Daten in Datenbank schreiben
       $sql = "";
       foreach ($buch as $key => $value) 
       {
       	  if ( $key < 5 )
       	  {
            if ( $value != ""  )
            {
              $sql .= "'".sql_real_escape_string(trim(str_replace('"','',$value)))."',";
            }
            else
              $sql .= "'',";
       	  }
       	  else 
       	  {
			$Preis = $value;
			if ( ! is_numeric($value))
			  $Preis = 0;
       	  }                        
       }
       if ( ! sql_query($sql))
       {
       	  // Update ausf�hren
       	  $msql = 'UPDATE '.TABLE_ARTIKEL.' SET WHERE id=';
       	  
       }
       // Preisliste ergänzen
       if ( !sql_query('INSERT INTO '.TABLE_PREISE.' (F_Preisliste_id,Artikel_Nr,' .
       		'Preis) VALUES ('.$Preislisteid.','.$Artikelnr.','.$Preis.')'))
       {
       	  //Update ausf�hren
       	  sql_query('UPDATE '.TABLE_PREISE.' SET Preis='.$Preis.' WHERE Artikel_Nr='.
       	    $Artikelnr.' AND F_Preisliste_id='.$Preislisteid);
       }
     }
   }
   echo $anz." Datens�tze importiert.";
   sql_query("UPDATE T_Stand SET Stand = ".time().", Bearbeiter='".$_SERVER["REMOTE_USER"]."'");
   echo "</td></tr>";
   fclose($dat);
   @unlink($_FILES['Datei']['tmp_name']);
  }
  
  echo '<input type="Submit" value="Ausw�hlen"></form>';  
}
?>