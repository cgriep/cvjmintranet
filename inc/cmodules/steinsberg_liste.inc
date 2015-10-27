<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.06.2002

*/

if ( $profile['editor'] == 1 ) {
  echo "<br /><br />";
  create_editor_input("Standardliste","Liste", "text_input");
}
else
{
  $Veranstaltung = "";
  $Liste = $document['Liste'];
  if ( is_numeric( $_REQUEST['Veranstaltung'])) {
    $Veranstaltung = $_REQUEST['Veranstaltung'];
  }
  else
  {
    $Veranstaltung = -1;
    $qres = sql_query("SELECT id FROM ".TABLE_NODES.' WHERE type_id = '.
    $doctypes["steinsberg_veranstaltung"].
    		' ORDER BY id DESC');
    if ( sql_num_rows($qres) > 1 ) {
      echo "<h2>Veranstaltungen</h2><p><ul>";
      while ( $row = sql_fetch_row($qres) ) {        
        $nd = get_nodedata($row[0], $flavour);
        $Veranstaltungen[strtotime($nd['Beginn'])] = $nd;
        $Veranstaltungen[strtotime($nd['Beginn'])]['id'] = $row[0]; 
      }
      $k = array_keys($Veranstaltungen);
      rsort($k);
      foreach ( $k as $vnr)
      {
        if ( $Veranstaltung == -1 ) $Veranstaltung = $Veranstaltungen[$vnr]['id'];
        echo '<li><a href="'.get_url($id, 'Veranstaltung='.$Veranstaltungen[$vnr]['id']).'">'.
        $Veranstaltungen[$vnr]['title'].'</a></li>';
      }
      echo "</ul></p>";   
    }
    else
    {
      $row = sql_fetch_row($qres);
      $Veranstaltung = $row[0];
    }
    sql_free_result($qres);
  }
  if ( isset( $_REQUEST['Liste'])) $Liste = $_REQUEST['Liste'];
  if ( $profile['SteinsbergSL'] == 1 && is_numeric($Veranstaltung) ) {
  	if ( isset($_REQUEST['WE']) && is_numeric($_REQUEST['WE']))
  	{
  		remove_profile('VWarte'.$Veranstaltung, $_REQUEST['WE']);
  	}  
  	if ( isset($_REQUEST['WL']) && is_numeric($_REQUEST['WL']))
  	{
  		set_profile('VWarte'.$Veranstaltung, "1", $_REQUEST['WL']);
  	}    
    if ( isset($_REQUEST["sendmail"]) && isset($_REQUEST["mailbody"]) ) {
      // eMail senden
      require_once("phpmail.php");
      $attachment = "";
      if ( isset($_FILES["Anhang"]["name"]) && $_FILES["Anhang"]["name"] != "" )
      {
        move_uploaded_file($_FILES["Anhang"]["tmp_name"],"files/0/".basename($_FILES["Anhang"]["name"]));
        $attachment = file_get_contents("files/0/".basename($_FILES["Anhang"]["name"]));
        //$_FILES["Anhang"]["tmp_name"]);
      }
    }
    if ( isset($_REQUEST["EMail"]) ) {
    ?>
      <form action="<?=get_url($id)?>" name="Formular" method="post" enctype="multipart/form-data">
      <input type="hidden" name="Liste" value="<?=$Liste?>" />
      <input type="hidden" name="Veranstaltung" value="<?=$Veranstaltung?>" />
      <?=LANG_SUBJECT?><br />
      <input type="Text" name="mailsubject" value="" size="50" maxlength="50" /><br />
      <?=LANG_MESSAGE?><br />
      <textarea name="mailbody" rows="5" cols="60"></textarea><br />
      <br />
      Verwendbare Platzhalter: Bezahlt: %%Geld%% / %%Stand%%, Teilnahmeart: %%Art%%,
      %%Charaktername%%, %%Name%%", %%Vorname%%, %%Unterbringung%%<br />
      Anh‰ngen <input type="file" name="Anhang" /> (Achtung, nicht zu groﬂe Datei!)<br />
      Real senden <input type="Checkbox" name="senden" value="v" /><br />
      <input type="Submit" name="sendmail" value="<?=LANG_SUBMIT?>">
      
    <?
    }
   // Liste anzeigen !
      $Summe = "";
      $Markierung = "";
      $MarkierungsWert = "";
      $Auswahlkriterien = "";
      /* Feldangaben:
         - nicht anzeigen
         # wird durch Veranstaltungsnummer ersetzt
         : trennt Bezeichnung und Feldnamen (Bemerkung:VBem#)
      */
      switch ( $Liste )
      {
           case "TN": $Ueberschrift = "Teilnehmerliste";
                      $Felder = "Name|Vorname|Adresse|PLZ|Ort|EMail|Telefon";
                      $Sort = "Name";
                      $MarkierungsFeld = "V#";
                      $MarkierungsWert = "!SC";
                      $Auswahlkriterien = "VWarte#:";
                      break;
           case "SL": $Ueberschrift = "SL-&Uuml;bersicht";
                      $Felder = "Name|Vorname|Charaktername|Rasse|VGruppe#:Gruppe|VSLBem#:Info";
                      $Auswahlkriterien = "VWarte#:";
                      $MarkierungsFeld = "V#";
                      $MarkierungsWert = "!SC";
                      $Sort = "Name";
                      break;
           case "G":  $Ueberschrift =  "Geldliste";
                      $Felder = "Name|Vorname|EMail|VBem#:Bemerkung|V#:Art|VGruppe#:Gruppe|Geld#:Geld|-Warteliste";
                      $Sort = "Name";
                      $Summe = "Geld#";
                      $MarkierungsFeld = "VWarte#";
                      $MarkierungsWert = 1;
                      break;
           case "A":  $Ueberschrift =  "Autokennzeichenliste";
                      $Felder = "Kennzeichen|Name|Vorname|Charaktername|VGruppe#:Gruppe";
                      $Sort = "Kennzeichen";
                      $Auswahlkriterien = "VWarte#:";
                      break;
           case "CF1": $Ueberschrift =  "Charakterfragen I";
                      $Felder = "Name|Charaktername|Liebe|Glaube";
                      $Sort = "Charaktername";
                      $Auswahlkriterien = "VWarte#:|V#:SC";
                      break;
           case "CF2": $Ueberschrift =  "Charakterfragen II";
                      $Felder = "Name|Charaktername|Gegenstand|Aktion|Ziele";
                      $Sort = "Charaktername";
                      $Auswahlkriterien = "VWarte#:|V#:SC";
                      break;
           case "CF3": $Ueberschrift =  "Charakterfragen III";
                      $Felder = "Name|Charaktername|Angst|Dunklerpunkt";
                      $Sort = "Charaktername";
                      $Auswahlkriterien = "VWarte#:|V#:SC";
                      break;
           case "TR":  $Ueberschrift =  "Traumliste";
                      $Felder = "Charaktername|TraumOk|VGruppe#:Gruppe|Name|Vorname";
                      $Sort = "Charaktername";
                      $Auswahlkriterien = "VWarte#:|V#:SC";
                      break;
           case "F":  $Ueberschrift =  "Fahrgemeinschaft";
                      $Felder = "PLZ|Ort|Name|Vorname|EMail";
                      $Sort = "PLZ";
                      $Auswahlkriterien = "VWarte#:";
                      break;
           case "P":  $Ueberschrift =  "Plothinweise";
                      $Felder = "Charaktername|Beschreibung|Faehigkeiten|VBem#:Bemerkungen";
                      $Sort = "Charaktername";
                      $Auswahlkriterien = "VWarte#:|V#:SC";
                      break;
           case "H":  $Ueberschrift =  "Wichtige Hinweise";
                      $Felder = "Charaktername|Bemerkungen:Besonderheiten|Name|Vorname|Dungeon|VBem#:Bemerkungen";
                      $Sort = "Charaktername";
                      $Auswahlkriterien = "VWarte#:";
                      break;
           case "U":  $Ueberschrift =  "Unterbringung";
                      $Felder = "Name|Vorname|Charaktername|VGruppe#:Gruppe|VUnter#:Unterbringung|V#:Art|Geld#:Geld";
                      $Sort = "VUnter#";
                      $Auswahlkriterien = "VWarte#:";
	       	     $MarkierungsFeld = "V#";
                      $MarkierungsWert = "!SC";
                      break;
           case "C":  $Ueberschrift =  "Charakterliste";
                      $Felder = "Name|Vorname|Charaktername|Rasse|VGruppe#:Gruppe";
                      $Sort = "Name";
                      $Auswahlkriterien = "VWarte#:|V#:SC";
                      break;
           case "W":  $Ueberschrift =  "Warteliste";
                      $Felder = "Name|Vorname|Charaktername|VEingang#:Eingegangen|VGruppe#:Gruppe|EMail";
                      $Sort = "VEingang#";
                      $Auswahlkriterien = "VWarte#:!";
                      $MarkierungsFeld = "V#";
                      $MarkierungsWert = "!SC";
                      break;
           case "E":  $Ueberschrift =  "Eincheckliste";
                      $Felder = "Name|Vorname|Geld#:Geld|VUnter#:Unterbringung|Charaktername|birthday:GebTag|Kennzeichen|Kaution";
                      $Sort = "Name";
                      $Auswahlkriterien = "VWarte#:";
                      break;
           case "EG":  $Ueberschrift =  "Eingangsliste";
                      $Felder = "Name|Vorname|Geld#:Geld|VEingang#:Eingegangen|Vegetarier|VBem#:Bemerkungen";
                      $Sort = "VEingang#";
                      break;
           case "N":  $Ueberschrift =  "NSC-Liste";
                      $Felder = "Name|Vorname|Aktiv|Kampf|Maske|Schminken|Springer|Ambienterolle";
                      $Sort = "Name";
                      $Auswahlkriterien = "V#:NSC|VWarte#:";
                      //$MarkierungsFeld = "V#";
                      //$MarkierungsWert = "!NSC";
                      break;
           case "SN":  $Ueberschrift =  "Nicht-SC-Liste";
                      $Felder = "Name|Vorname|Aktiv|Kampf|Maske|Schminken|Springer|Ambienterolle";
                      $Sort = "Name";
                      $Auswahlkriterien = "V#:!SC|VWarte#:";
                      $MarkierungsFeld = "V#";
                      $MarkierungsWert = "!NSC";
                      break;
           default:
             $Ueberschrift = "Keine Liste gew&auml;hlt";
             $Felder = "Name|Vorname";
             $Sort = "Name";
         }
    $MarkierungsFeld = str_replace("#", $Veranstaltung, $MarkierungsFeld);
    $Auswahlkriterien = str_replace("#", $Veranstaltung, $Auswahlkriterien);
    $Summe = str_replace("#", $Veranstaltung, $Summe);
    $Felder = str_replace("#", $Veranstaltung, $Felder);
    if ( isset($_REQUEST['Sort']) ) $Sort = $_REQUEST['Sort'];
    $Sort = str_replace("#", $Veranstaltung, $Sort);
    $Feld =  strtok($Felder, "|");
    $Feldanzahl = 0;
    while ($Feld )
    {
      // ausgeblendete Felder
      if ( strpos($Feld, "-") === false ) $Feldanzahl++;
      $Feld = strtok("|");
    }
    $Veranstaltungdata = get_nodedata($Veranstaltung, $flavour);
    if ( isset($_REQUEST["EMail"]) )
      $Feldanzahl++;    
    echo '<table class="tabelle" id="teilnehmertable">';
    echo '<tr bgcolor="' . $design['table_row_dark'].'"><th colspan="'.$Feldanzahl.
      '">'.$Ueberschrift."<br />".$Veranstaltungdata['title'];
    echo '</th></tr><tr bgcolor="'.$design['table_row_dark'].'">';
    if ( isset($_REQUEST["EMail"]) )
      echo '<td><a href="javascript:CheckAll(-1);">MA</a>'."</td>\n";
    
    $Feld =  strtok($Felder, "|");
    while ($Feld )
    {
      $Bez = $Feld;
      if ( ! (strpos($Feld, ":") === false) ) {
        $Bez = substr($Feld, strpos($Feld, ":")+1);
        $Feld = substr($Feld, 0, strpos($Feld, ":"));
      }
      if ( strpos($Feld, "-") === false ) echo '<td><a href="'.get_url($id,'Veranstaltung='.
        $Veranstaltung.'&Liste='.$Liste.'&Sort='.$Feld).'">'.$Bez."</a></td>";
      $Feld = strtok("|");
    }
    echo '</tr>';
    $alledaten = array();
    $query = "SELECT DISTINCT user_id FROM ".TABLE_USERDATA." WHERE name='V$Veranstaltung'";
    $query = sql_query($query);
    while ( $row = sql_fetch_row($query))
    {
      $alledaten[$row[0]] = get_profile($row[0], false);
      $alledaten[$row[0]]['EMail'] = url_to_link(get_user_email($row[0], false));
      $alledaten[$row[0]]['ID'] = $row[0];
      $alledaten[$row[0]]["VEingang".$Veranstaltung."Sort"] = $alledaten[$row[0]]["VEingang$Veranstaltung"];
      $alledaten[$row[0]]["VEingang$Veranstaltung"] = date("d.m.Y",$alledaten[$row[0]]["VEingang$Veranstaltung"]);
      $alledaten[$row[0]]["birthdaySort"] = $alledaten[$row[0]]["birthday"];
      $alledaten[$row[0]]["birthday"] = date("d.m.Y",$alledaten[$row[0]]["birthday"]);
      $Auswahl = strtok($Auswahlkriterien, "|");
      while ( $Auswahl && isset($alledaten[$row[0]]) )
      {
        $Kriterium = substr($Auswahl,strpos($Auswahl,":")+1);
        $Auswahl = substr($Auswahl, 0, strpos($Auswahl, ":"));
        if ( (substr($Kriterium,0,1) == "!" && $alledaten[$row[0]][$Auswahl] == substr($Kriterium,1) ) ||
          (substr($Kriterium,0,1) != "!" && $alledaten[$row[0]][$Auswahl] != $Kriterium) )
          unset($alledaten[$row[0]]);
        $Auswahl = strtok("|");
      }
    }
    sql_free_result($query);
    // Charakterdaten hinzuf¸gen
    // Suchen nach VChar#
    // merge der Daten
    reset($alledaten);
    global $flavour;

    while ( list($key, $value ) = each($alledaten)) {

      if ( isset($value["Char$Veranstaltung"])) {
        $CharID = $value["Char$Veranstaltung"];
        $charakter = get_node($CharID, $flavour);
        $alledaten[$key]["Charaktername"] = $charakter["title"];
        $alledaten[$key]["Rasse"] = $charakter["Rasse"];
        $alledaten[$key]["Contage"] = $charakter["Contage"];
        $alledaten[$key]["Liebe"] = $charakter["Liebe"];
        $alledaten[$key]["Traum"] = $charakter["Traum"];
        $alledaten[$key]["TraumOk"] = $charakter["TraumOk"];
        $alledaten[$key]["Dungeon"] = $charakter["Dungeon"];
        $alledaten[$key]["Glaube"] = $charakter["Glaube"];
        $alledaten[$key]["Gegenstand"] = $charakter["Gegenstand"];
        $alledaten[$key]["Aktion"] = $charakter["Aktion"];
        $alledaten[$key]["Dunklerpunkt"] = $charakter["DunkelPunkt"];
        $alledaten[$key]["Ziele"] = $charakter["Ziele"];
        $alledaten[$key]["Angst"] = $charakter["Angst"];
      }
      else
        $alledaten[$key]["Charaktername"] = "-unbekannt-";
    }
    reset($alledaten);
    // Sortieren des Feldes
    $sortarray = array();
    $numsort = false;
    while ( list($key, $value) = each($alledaten) )
    {
      if ( isset($value[$Sort."Sort"]) ) {
        $numsort = true;
        $sortarray[] = $value[$Sort."Sort"];
      }
      else
        $sortarray[] = $value[$Sort];
    }
    if ( $numsort )
      array_multisort($sortarray, SORT_NUMERIC, $alledaten);
    else
      array_multisort($sortarray, SORT_ASC, $alledaten);
    $Summenwert = 0;
    //if ( isset($_REQUEST["Anhang"]) ) {
    //  move_uploaded_file($_FILES['Anhang']['tmp_name'], $_FILES['Anhang']['name']);
    //  $handle = fopen($_FILES['Anhang']['name'], "r");
    //  $attachment = fread($handle, filesize($_FILES['Anhang']['name']));
    //  fclose($handle);
    //}    
    foreach ( $alledaten as $key => $value) 
    {
      $Feld = strtok($Felder, "|");
      echo '<tr';
      if ( $MarkierungsFeld != "" )
        if ( (substr($MarkierungsWert, 0,1) == "!" && $value[$MarkierungsFeld] != substr($MarkierungsWert,1) ) ||
          (substr($MarkierungsWert, 0,1) != "!" && $value[$MarkierungsFeld] == $MarkierungsWert ))
          echo ' bgcolor="'.$design['table_row_bright'].'"';
      echo '>';
      if ( isset($_REQUEST['EMail']) )
        echo '<td><input type="Checkbox" name="docinput[EMailID][]" value="'.$value["ID"].'"></td>';      
      while ( $Feld ) {
        if ( ! (strpos($Feld, ':') === false) )
          $Feld = substr($Feld, 0, strpos($Feld, ':'));
        echo "<td";
        if ( $Feld == $Summe ) echo ' align="right"';
        echo '>';
        if ( $Feld == substr($Felder, 0, strlen($Feld)) )
        {
          echo '<a href="'.get_url(get_cmodule_id('steinsberg_anmeldung'),
            'Ref='.$Veranstaltung.'&Spieler='.$value['ID']).'">'.$value[$Feld]."</a>";
          if ($docinput["design"] != "Printable" )
          {
            echo '(<a href="'.get_url(get_cmodule_id("user"),"UNr=".SESSION_DBID."A".$value["ID"]).'" ' .
            		'title="Spielerdaten ansehen">&raquo;</a>)';
            if ($Liste == 'W' )
      		{
      			echo '<a href="'.get_url($id,'Liste=W&Veranstaltung='.$Veranstaltung.'&WE='.$value['ID']).'" ' .
      					'title="Als Teilnehmer aufnehmen" ' .
      					'onClick="javascript:return confirm(\'Tats‰chlich als regul‰ren Teilnehmer aufnehmen?\');">&rarr; N</a>';
            }
            elseif ( $Liste == 'TN')
            	echo '<a href="'.get_url($id,'Liste=W&WL='.$value['ID']).'&Veranstaltung='.$Veranstaltung.'" ' .
            			' onClick="javascript:return confirm(\'Tats‰chlich auf Warteliste setzen?\')" ' .
            			'title="in Warteliste schieben">&rarr; WL</a>';
          }
        }
        else
          echo $value[$Feld];
        if ( $Feld == $Summe ) {
          $value[$Summe] = str_replace(',','.',$value[$Summe]);
          if ( is_numeric($value[$Feld]))
            $Summenwert += $value[$Feld];
        }
        echo '</td>';
        $Feld = strtok('|');
      }      
      echo "</tr>";
      if ( isset($_REQUEST["sendmail"]) && isset($_REQUEST["mailbody"]) && 
      $profile["SteinsbergSL"] == 1 && in_array($value["ID"],$docinput["EMailID"])) {
        // eMail senden
        $mail = new mime_mail();
        $mail->from = get_user_nickname(SESSION_DBID).' <'.get_user_email(SESSION_DBID).'>';
        $mail->from .= "\nbcc: ".get_user_email(SESSION_DBID);
        $mail->headers = "Errors-To: ".get_user_email(SESSION_DBID);
        $mail->to = get_user_email($value["ID"]);
        $mail->subject = "[".$Veranstaltungdata['title']."] ".$_REQUEST["mailsubject"];
        $body = $_REQUEST["mailbody"];
        $body .= "\nAn: ".get_user_nickname($value["ID"])." (".get_user_email($value["ID"]).")";
        if ( $value["Geld$Veranstaltung"] == "" ) $value["Geld$Veranstaltung"] = 0;
        $body = str_replace("%%Geld%%", $value["Geld$Veranstaltung"], $body);
        $body = str_replace("%%Art%%", $value["V$Veranstaltung"], $body);
        $body = str_replace("%%Charaktername%%", $value["Charaktername"], $body);
        $body = str_replace("%%Name%%", $value["Name"], $body);
        $body = str_replace("%%Vorname%%", $value["Vorname"], $body);
        $body = str_replace("%%Unterbringung%%", $value["VUnter".$Veranstaltung], $body);
        $body = str_replace("%%Stand%%", $Veranstaltungdata['GeldDatum'], $body);
        if ( strpos($body, "%") === false ) {
          //$mail->to = "griep@oszimt.de";  // TODO
          if (strlen($attachment) > 0 )
            $mail->add_attachment($attachment, $_FILES["Anhang"]["name"],$_FILES["Anhang"]["type"]);
          $mail->body = $body;
          if ( isset($_REQUEST["senden"] ) )
          {
            if ( ! $mail->send() )
              echo "Fehler beim Mailen: UserId ".$key."<br />";
            else
              echo "Mail an ".$mail->to." gesendet.<br />";
          }
          else
            echo "Mail w¸rde an ".$mail->to." gesendet.<br />";
        }
        else
          echo '&gt;&gt; Fehler: Konnte %% nicht umsetzen: "'.$body.'".<br />';
      }
    }
    if ( $Summe != "" ) {
      $Feld = strtok($Felder, "|");
      echo '<tr bgcolor="'.$design['table_row_dark'].'">';
      while ( $Feld ) {
        echo '<td align="right">';
        if ( ! (strpos($Feld, ":") === false) )
          $Feld = substr($Feld, 0, strpos($Feld, ":"));
        if ( $Feld == $Summe ) echo $Summenwert;
        echo '</td>';
        $Feld = strtok("|");
      }
      echo '</tr>';
    }
    echo $design['table_end'];
    echo "<b>".count($alledaten)." Eintr&auml;ge</b><br />";
    if (isset($_REQUEST['EMail']))
      echo '</form>';
  }
  else
    echo "Zugriff verweigert!<br />";
  if ( isset($_FILES["Anhang"]) )
    @unlink("files/0/".$_FILES['Anhang']['name']);


?>
<p><b>Andere Listen:</b><br />
<a href="<?=get_url($id,'Liste=TN&Veranstaltung='.$Veranstaltung)?>">Teilnehmerliste</a> |
<a href="?id=<?=$id?>&Liste=G&Veranstaltung=<?=$Veranstaltung?>">Geldliste</a> |
<a href="?id=<?=$id?>&Liste=TR&Veranstaltung=<?=$Veranstaltung?>">Traumliste</a> |
<a href="?id=<?=$id?>&Liste=CF1&Veranstaltung=<?=$Veranstaltung?>">Charakterfragen 1</a> |
<a href="?id=<?=$id?>&Liste=CF2&Veranstaltung=<?=$Veranstaltung?>">Charakterfragen 2</a> |
<a href="?id=<?=$id?>&Liste=CF3&Veranstaltung=<?=$Veranstaltung?>">Charakterfragen 3</a> |
<a href="?id=<?=$id?>&Liste=A&Veranstaltung=<?=$Veranstaltung?>">Kennzeichenliste</a> |
<a href="?id=<?=$id?>&Liste=H&Veranstaltung=<?=$Veranstaltung?>">Hinweise</a> |
<a href="?id=<?=$id?>&Liste=F&Veranstaltung=<?=$Veranstaltung?>">Fahrgemeinschaft</a> |
<a href="?id=<?=$id?>&Liste=P&Veranstaltung=<?=$Veranstaltung?>">Plothinweise</a> |
<a href="?id=<?=$id?>&Liste=U&Veranstaltung=<?=$Veranstaltung?>">Unterbringung</a> |
<a href="?id=<?=$id?>&Liste=C&Veranstaltung=<?=$Veranstaltung?>">Charakterliste</a> |
<a href="?id=<?=$id?>&Liste=W&Veranstaltung=<?=$Veranstaltung?>">Warteliste</a> |
<a href="?id=<?=$id?>&Liste=E&Veranstaltung=<?=$Veranstaltung?>">Eincheckliste</a> |
<a href="?id=<?=$id?>&Liste=N&Veranstaltung=<?=$Veranstaltung?>">NSC-Liste</a> |
<a href="?id=<?=$id?>&Liste=SN&Veranstaltung=<?=$Veranstaltung?>">Nicht-SC-Liste</a> |
<a href="?id=<?=$id?>&Liste=EG&Veranstaltung=<?=$Veranstaltung?>">Eingangsliste</a> |
<a href="?id=<?=$id?>&Liste=SL&Veranstaltung=<?=$Veranstaltung?>">SL-‹bersicht</a>
</p>
<p>eMail an <a href="<?=get_url($id,'Liste='.$Liste.'&Veranstaltung='.$Veranstaltung.
'&EMail=1')?>">obige Personen</a> senden</p>
<script type="text/javascript">
<!--
 function CheckAll(wert)
 {
   for (var i=0;i<document.Formular.elements.length;i++)
   {
     var e = document.Formular.elements[i];
     if (e.name == "docinput[EMailID][]" )
       if ( wert == -1 )
         e.checked = ! e.checked;
       else
         e.checked = wert;
    }
}
//-->
</script>

<?php
}
?>