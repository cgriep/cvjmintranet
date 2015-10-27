<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.12.2001
*/

$Tage[0] = "Sonntag";
$Tage[1] = "Montag";
$Tage[2] = "Dienstag";
$Tage[3] = "Mittwoch";
$Tage[4] = "Donnerstag";
$Tage[5] = "Freitag";
$Tage[6] = "Samstag";

if($profile['editor']!='1') {
  if ( isset($_REQUEST["Cancel"] )) {
    if ( isset($_REQUEST['Spieler'] ))
      $SpielerNr = $_REQUEST['Spieler'];
    else
      $SpielerNr = SESSION_DBID;

    if (is_numeric($SpielerNr) && ($profile['SteinsbergSL'] == 1 ||
                    ($SpielerNr == SESSION_DBID && SESSION_STATUS == "ok"))) {
                     // Anmeldung widerrufen
      $daten = get_profile($SpielerNr, false);
      mail($document["MeldungsEMail"], $document['title'].
                        " - Anmeldung gelöscht",
                        "Eine Anmeldung wurde widerrufen ($SpielerNr):\nName: " .
                        $daten['Name'] . " ".$daten["Vorname"]."\n" .
                        "EMail: " . get_user_email($SpielerNr) . "\nGruppe: ".
                        $daten["VGruppe$id"]."\nArt: ".
                        $daten["V$id"]."\nEinzahlung: ".$daten["Geld$id"]."\nArt:".
                        $daten["VBem$id"]."\n".
                        "Datum: " . date("d.m.Y H:i"),
                        "From: ".$document["MeldungsEMail"]);
      remove_profile("VGruppe$id", $SpielerNr);
      remove_profile("V$id", $SpielerNr);
      remove_profile("VBem$id", $SpielerNr);
      remove_profile("Geld$id", $SpielerNr);
      if ( is_numeric($document["Key ".$daten["V$id"]]) )
      {
                       // in die entsprechende Gruppe eintragen
        remove_profile ("key_".$document["Key ".$daten["V$id"]], $SpielerNr);
      }
      echo '<b>&gt;&gt;</b> <font color="#ff0000">Anmeldung wurde widerrufen.</font>';
    }
  }
  if($document['title'] != '') {
    echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
  }
  echo "<p><b>".transform($document['Art'],"clean")."</b><br />von ".
                  $Tage[date("w",strtotime($document['Beginn']))].", ".
                  date("d.m.Y H:i",strtotime($document['Beginn'])).
                " bis ".$Tage[date("w",strtotime($document['Ende']))].", ".
                date("d.m.Y H:i",strtotime($document['Ende']))."</p>";
  echo '<p>Das Mindestalter für die Teilnahme beträgt '.
                  $document["Mindestalter"]." Jahre.</p>";
  echo $design['table_begin'].'<tr bgcolor="'.$design['table_header'].
                  '"><th></th><th align="center">Spieler</th><th align="center">NSC</th></tr>';
  // Belegte Plätze berechnen
  $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'V".
                  $id."' AND value='SC'";
  $query = sql_query($sql);
  $SCBelegt = 0;
  $NSCBelegt = 0;
  if ( $row = sql_fetch_row($query) )
    $SCBelegt = $row[0];
  sql_free_result($query);
  
  $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'Geld".
                  $id."' AND value!='' AND user_id IN (SELECT user_id FROM ".
                  TABLE_USERDATA." WHERE name = 'V".
                  $id."' AND value='SC')";
  $query = sql_query($sql);
  $SCBezahlt = 0;
  if ( $row = sql_fetch_row($query) )
    $SCBezahlt = $row[0];
  sql_free_result($query); 
  $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'V".
                  $id."' AND value='NSC'";
  $query = sql_query($sql);
  if ( $row = sql_fetch_row($query) )
                  $NSCBelegt = $row[0];
  sql_free_result($query);
  $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'Geld".
                  $id."' AND value!='' AND user_id IN (SELECT user_id FROM ".
                  TABLE_USERDATA." WHERE name = 'V".
                  $id."' AND value='NSC')";
  $query = sql_query($sql);
  $NSCBezahlt = 0;
  if ( $row = sql_fetch_row($query) )
    $NSCBezahlt = $row[0];
  sql_free_result($query);               
                /*
                echo '<tr><td>SC</td><td align="center">'.transform($document['SCPreis'],'clean');
                echo '</td><td align="center">';
                if ( $document['SC']-$SCBelegt <= 0 ) echo '<font color="red"><b>';
                echo ($document['SC']-$SCBelegt);
                if ( $document['SC']-$SCBelegt <= 0 ) echo '</b></font>';
                echo "</td></tr>";
                echo '<tr><td>NSC</td><td align="center">'.transform($document['NSCPreis'],'clean');
                echo ' &euro;</td><td align="center">';
                if ( $document['NSC']-$NSCBelegt <= 0 ) echo '<font color="red"><b>';
                echo ($document['NSC']-$NSCBelegt);
                if ( $document['NSC']-$NSCBelegt <= 0 ) echo '</b></font>';
                echo "</td></tr>";
                */
  $eintraege = explode("\n",$document["Preise"]);
  foreach ($eintraege as $eintrag)
  {
    $zellen = explode("|", $eintrag);
    echo "<tr>\n";
    for ($i = 0; $i<3; $i++)
    {
      echo "<td ";
      if ( $i> 0) echo 'align="center"';
      echo ">";
      echo "{$zellen[$i]}";
	  //if ( $i > 0) echo "&euro;";
	  echo "</td>\n";
    } 
    echo "</tr>\n"; 
  }
  echo '<tr><td colspan="3"><hr /></td></tr>'; 
  echo '<tr><td>freie Plätze</td><td align="center">';
  if ( $document['SC']-$SCBezahlt <= 0 ) echo '<font color="red"><b>';
  echo ($document['SC']-$SCBezahlt);  
  if ( $document['SC']-$SCBezahlt <= 0 ) echo '</b></font>';
  echo '</td><td align="center">';
  if ( $document['NSC']-$NSCBezahlt <= 0 ) echo '<font color="red"><b>';
  echo ($document['NSC']-$NSCBezahlt);  
  if ( $document['NSC']-$NSCBezahlt <= 0 ) echo '</b></font>';
  echo "</td></tr>\n"; 
  echo '<tr><td>vorläufige Anmeldungen ohne Zahlungseingang</td><td align="center">';
  echo ($SCBelegt-$SCBezahlt);
  echo '</td><td align="center">';
  echo ($NSCBelegt-$NSCBezahlt);
  echo "</td></tr>\n";     
  echo $design['table_end'];
  echo '<br />';
  if ( strtotime($document["Ende"]) > time() )
  {
    if ( $document['SC']-$SCBezahlt <= 0)
    {
    	echo '<b>Achtung: Keine SC-Plätze frei! Nur Anmeldungen für die Warteliste möglich!</b><br />';
    }
    elseif ( $SCBelegt >= $document['SC'] )
    {
    	echo '<b>Achtung: Mehr SC-Anmeldungen als freie Plätze! Plätze werden in Reihenfolge des Geldeingangs vergeben!</b><br />';
    }
  	if ( SESSION_STATUS == "ok" )
                    echo 'Gleich <a href="?id='.get_cmodule_id('steinsberg_anmeldung').'&Ref='.$id.'">online anmelden/Anmeldung bearbeiten</a>...<br /><br />';
                  else
                    echo "Zum Online-Anmelden müssen Sie sich einloggen!<br /><br />";
  }
  echo '<hr />';
  if($document['image'] != '' || $document['body'] != '') {
                        echo '<p class="justify">';
                        // Image
                        echo transform($document['image'], $document['datatype']['image']);
                        // Text
                        echo transform($document['body'], $document['datatype']['body']).'</p>';
                        }
  /*if ( strtotime($document["Beginn"]) > time() )
  {
  	echo "<br />\n";
    if ( SESSION_STATUS == "ok" )
      echo 'Gleich <a href="?id='.get_cmodule_id('steinsberg_anmeldung').'&Ref='.$id.'">online anmelden/Anmeldung bearbeiten</a>...<br /><br />';
    else
      echo "Zum Online-Anmelden müssen Sie sich einloggen!<br /><br />";
  }*/

  //if(isset($parents)) {
  //  echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).'">'.LANG_SHOW_COMPLETE_DOC.'</a> ]');
  //}
}
  /* 
   * Änderung 07.01.06: Unterseiten auflisten
   */
  $children =  get_children (0, 100, -1, false, 'DESC', false);
  if(isset($children)) {
    foreach ($children as $docid => $value)
    {
      if($value['title'] != '')
        echo '<h2><a href="'.get_url($docid).'">'.
          transform($value['title'], $value['datatype']['title']).'</a></h2>';
    }
  } // wenn Children vorhanden
  
if($profile['editor']=='1') {
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input              
  create_editor_input('Preise (Bezeichnung|SC Preis|NSC Preis)','Preise','area_input');
  // Text Input
  echo '<br /><br />';
  create_editor_input('Conart','Art','text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Beginn','Beginn','text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Ende','Ende','text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('SC-Anzahl','SC','text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('NSC-Anzahl','NSC','text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('SC-Preis','SCPreis','text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('NSC-Preis','NSCPreis','text_input');
  echo '<br /><br />';
  create_editor_input('AWF-Keynummer für SC','Key SC','text_input');
  echo '<br /><br />';
  create_editor_input('AWF-Keynummer für NSC','Key NSC','text_input');
  echo '<br /><br />';
  create_editor_input('Mindestalter in Jahren','Mindestalter','text_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('eMail Anmeldemeldung','MeldungsEMail','text_input');
  echo '<br /><br />';
  create_editor_input('Body','body','area_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Datum letzte Geldaktualisierung','GeldDatum','text_input');
}
?>