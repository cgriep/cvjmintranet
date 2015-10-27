<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.06.2002

*/

require(INC_PATH . 'misc/drow.inc');

$SpielerNr = SESSION_DBID;
$Verweis = "";
if ( isset ($_REQUEST["Spieler"]) && is_numeric($_REQUEST["Spieler"]) &&
  ($profile['SteinsbergSL'] == "1" || $profile["SteinsbergGeld"] == 1) )
{
  $SpielerNr = $_REQUEST["Spieler"];
  $Verweis = "&Spieler=$SpielerNr";
}
$daten = get_profile($SpielerNr, false);
$ref = $_REQUEST['Ref'];
global $flavour;
$docinfo = get_node($ref, $flavour, false);
$veranstaltung = get_nodedata($ref, $flavour);

if($profile['editor'] != "1") {
if ( ! is_numeric($ref) ) die("Referenz nicht numerisch!");
if ( $daten['Name'] == "" || $daten['Vorname'] == "" || $daten['Adresse'] == "" ) {
  echo '<p>Bitte vor der Anmeldung erst die <a href="?id='.get_cmodule_id('user').
  '">persönlichen Daten</a> ergänzen.</p>';
  echo '<a href="'.get_url($ref).'">Zurück</a> zur Veranstaltungsbeschreibung.</p>';
}
elseif ( is_numeric($veranstaltung["Mindestalter"]) &&
         (strtotime("+".$veranstaltung["Mindestalter"]." year",$daten["birthday"]) >
           strtotime($docinfo["Beginn"])) &&
         $profile["SteinsbergSL"] != 1) {
  echo '<p><b>Die Teilnahme an diesem Con ist erst ab '.$veranstaltung["Mindestalter"].
    ' Jahren erlaubt.</b> <a href="'.get_url($ref).'">Zurück</a></p>';
}
else {
  if ( $profile["SteinsbergSL"] == "1" || $profile["SteinsbergGeld"] == 1)
    echo 'Zurück zur <a href="'.get_url(get_cmodule_id('drow_liste'),
      'Veranstaltung='.$_REQUEST['Ref']).'">Liste</a><br />';
  if ( SESSION_STATUS != "ok" )
    echo transform("Zurück zu %%".DEFAULT_DOCUMENT."%%","extrans");
  elseif ( !is_numeric($_REQUEST['Ref']) )
    die ("Fehler - unzulässiges Argument!");
  else {
    // Speichern notwendig?
    if ( isset($_REQUEST["newchar"]))
    {
      // neuen Charakter anlegen ...
      clear_cache($id);
      $nodeid = create_node($id, $doctypes['drow_charakter'],
                  count_children($doctypes['drow_charakter']), 1, 0, "",
                  $document['key_id'], 0, 1, 0, 1);
      add_nodedata($nodeid,'Charakter_id', create_Charakter($SpielerNr, $profile['VGruppe'.$ref]), $flavour); 
      add_nodedata($nodeid,'timestamp',time(), $flavour);
      add_nodedata($nodeid, 'title', "Neuer Charakter von ".$daten['Vorname']." ".
          $daten['Name'], $flavour);
      add_nodedata($nodeid, 'comments', 1, $flavour);
      add_nodedata($nodeid, 'SpielerID', $SpielerNr, $flavour);
      add_nodedata($nodeid, 'show_files', 1, $flavour);
      add_nodedata($nodeid, 'author', SESSION_EMAIL,$flavour,'email');
      add_nodedata($nodeid, 'author_id', SESSION_DBID, $flavour, 'user_id');
      add_nodedata($nodeid, 'basedir', get_dir_name("files",$nodeid, true), $flavour);
      // Schreibmöglichkeit erzeugen
      $handle = fopen(get_dir_name("files",$nodeid).'/.allow_upload', 'w');
      fclose($handle);
      $handle = fopen(get_dir_name("files",$nodeid).'/.allow_delete', 'w');
      fclose($handle);
      set_profile("Char$ref", $nodeid, $SpielerNr);
      $daten = get_profile($SpielerNr, false);
      echo '<strong>&gt;&gt; Charakter erzeugt. Bitte den "neuen Charakter" unten anklicken um ihn zu bearbeiten.</strong><br /><br />';
    }
    if ( isset( $_REQUEST['savechar'] ))
    {
      // charspeichern
      set_profile("Char$ref", $docinput['charakter'], $SpielerNr);
      $daten = get_profile($SpielerNr, false);
    }
    // Belegte Plätze berechnen
    $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'V".
             $ref."' AND value='SC'";
    $query = sql_query($sql);
    $SCBelegt = 0;
    $NSCBelegt = 0;
    if ( $row = sql_fetch_row($query) )
       $SCBelegt = $row[0];
    sql_free_result($query);
    $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'V".
         $ref."' AND value='NSC'";
    $query = sql_query($sql);
    if ( $row = sql_fetch_row($query) )
       $NSCBelegt = $row[0];
    sql_free_result($query);
    $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'Geld".
                  $ref."' AND value<>'' AND user_id IN (SELECT user_id FROM ".
                  TABLE_USERDATA." WHERE name = 'V".
                  $ref."' AND value='SC')";
  if ( ! $query = sql_query($sql) ) echo mysql_error();
  $SCBezahlt = 0;
  if ( $row = sql_fetch_row($query) )
    $SCBezahlt = $row[0];
  sql_free_result($query); 
  $sql = "SELECT count(user_id) FROM ".TABLE_USERDATA." WHERE name = 'Geld".
                  $ref."' AND value<>'' AND user_id IN (SELECT user_id FROM ".
                  TABLE_USERDATA." WHERE name = 'V".
                  $ref."' AND value='NSC')";
  $query = sql_query($sql);
  $NSCBezahlt = 0;
  if ( $row = sql_fetch_row($query) )
    $NSCBezahlt = $row[0];
  sql_free_result($query);               
    $SCFrei = ($veranstaltung['SC']-$SCBezahlt);
    $NSCFrei = $veranstaltung['NSC'] - $NSCBezahlt;
    $SCVoraussichtlich = ($veranstaltung['SC']-$SCBelegt);
    $NSCVoraussichtlich = ($veranstaltung['NSC']-$NSCBelegt);
    $AnzahlWarnung = false;
    if ( isset($_REQUEST['save']) ) {
      if ( $_REQUEST['AGB'] == 'v' || $profile['SteinsbergSL'] == 1 || 
           $profile['SteinsbergGeld'] == 1 ) {
        $neu = false;
        // bei Geldwechsel - Warteliste prüfen
        if ( $daten['V'.$ref] == '' || 
        $daten['Geld'.$ref] != $docinput['Geld'.$ref]) {
          $neu = true;
          set_profile('VEingang'.$ref, time(), $SpielerNr);
          if ( $daten['Geld'.$ref] == '' && $docinput['Geld'.$ref] == '')
          {
          	set_profile('VWarte'.$ref, 1, $SpielerNr);
          }
          $zahlPlatz = false;          
          if ( $docinput['V'.$ref] == 'SC' )
          {
            if ( $SCFrei <= 0 && $daten['Geld'.$ref] == '' && $docinput['Geld'.$ref] == '' )  {
              echo 'Keine Spielerplätze frei ! Anmeldung steht auf der Warteliste!';              
            }
            else
            {				
            	if ( $docinput['Geld'.$ref] != '' || 
				$daten['Geld'.$ref] != '' )
				  remove_profile('VWarte'.$ref, $SpielerNr);
            }
            if ($SCVoraussichtlich <= 0 && 
            ($daten['Geld'.$ref] == '' && $docinput['Geld'.$ref] == '' ) )
            {
            	echo '<br/>Achtung: Es gibt mehr Anmeldungen als Plätze - die Plätze werden nach Geldeingang vergeben!';
            	$AnzahlWarnung = true;            	
            }            
          }
          else if ( $docinput['V'.$ref] == 'NSC' )
          {
          	if ( $NSCFrei <= 0 && $daten['Geld'.$ref] == '' && $docinput['Geld'.$ref] == '' )  {
          
              echo 'Keine NSC-Plätze frei ! Anmeldung steht auf der Warteliste!';              
            }
            else
            {
              if ( $docinput['Geld'.$ref] != '' || 
				$daten['Geld'.$ref] != '' )
				remove_profile('VWarte'.$ref, $SpielerNr);
              if ($NSCVoraussichtlich <= 0 &&
              ($daten['Geld'.$ref] == '' && $docinput['Geld'.$ref] == '' ) )
              {
            	echo '<br/>Achtung: Es gibt mehr Anmeldungen als Plätze - die Plätze werden nach Geldeingang vergeben!';
            	$AnzahlWarnung= true;
              }
            }
          }
          else
          {
          	// sonstige 
			if ( $docinput['Geld'.$ref] != '' || 
				$daten['Geld'.$ref] != '' )
				remove_profile('VWarte'.$ref, $SpielerNr);   
          }
        }
        if ( isset($docinput['Gruppe'])) set_profile("VGruppe$ref", $docinput['Gruppe'], $SpielerNr);
        set_profile("V$ref", $docinput["V$ref"], $SpielerNr);
        //if ( $daten['Geld'.$ref] == '' ) set_profile('VAkzeptiert'.$ref, '');
        set_profile('VTeilnahmeart'.$ref, $docinput['Teilnahmeart'], $SpielerNr);
        if ( $docinput["V$ref"] == "" )
          set_profile("V$ref", "SC", $SpielerNr);
        if ( isset($docinput["VUnter$ref"]) )
          set_profile("VUnter$ref", $docinput["VUnter$ref"], $SpielerNr);
        if ( $docinput["V$ref"] == "Spezial" )
              set_profile("V$ref", $docinput["Spezial"], $SpielerNr);
        set_profile("VBem$ref", $docinput["VBem$ref"], $SpielerNr);
        if ( $profile["SteinsbergSL"] == "1" )
          set_profile("VSLBem$ref", $docinput["VSLBem$ref"], $SpielerNr);
        if ( $profile['SteinsbergGeld'] == "1" ) {
          if ( $daten["Geld$ref"] != $docinput["Geld$ref"] ) {
          	$neu = true;
            set_profile("Geld$ref", $docinput["Geld$ref"], $SpielerNr);
            // Geldaktualisierungdatum des Vaters aktualisieren
            update_nodedata ($ref, 'GeldDatum', date("d.m.Y H:i"), $flavour);
          }
        }
        $daten = get_profile($SpielerNr, false);
        if ( is_numeric($docinfo["Key ".$docinput['V'.$ref]]) )
        {
          // in die entsprechende Gruppe eintragen
          set_profile ("key_".$docinfo["Key ".$docinput['V'.$ref]], 1, SESSION_DBID);
        }
        $message = "Ihre Anmeldung wurde mit folgenden Daten gespeichert:\n";
        if ( $daten['VWarte'.$ref] == '1' && $daten['Geld'.$ref] == '' )
        {
        	if ( $SCFrei <= 0 && $daten['V'.$ref] == 'SC' ||
        	$NSCFrei <= 0 && $daten['V'.$ref] == 'NSC')  
        	{
        	  $message .= "\nAchtung: Keine Plätze frei! Anmeldung steht auf der Warteliste!\nBitte nicht überweisen!\n\n";
        	}
        	elseif ( $AnzahlWarnung )
        	{
        		$message .= "\nHinweis: Es gibt mehr vorläufige Anmeldungen als Plätze. Die Plätze werden nach Reihenfolge des Geldeinganges vergeben!\n\n";
        	}
        }
        $message .=  $daten['Name'] . " ".stripslashes($daten["Vorname"])."\n";
        $message .= "Teilnahme als: " . $daten["V$ref"]."\n";
        $message .= "Haus: " . stripslashes($daten['VGruppe'.$ref])."\n";
        if ( $daten['VAkzeptiert'.$ref] == "")
        {
        	$message .= 'Der Hausverantwortliche hat diese Anmeldung noch nicht akzeptiert. Du wirst benachrichtigt, wenn ';
        	$message .= 'eine Entscheidung getroffen wurde.'."\n";
        }
        else
        {
        	$message .= 'Die Anmeldung wurde akzeptiert. ';
        	if ($daten['Char'.$ref] == '' )
        		$message .= 'Bitte trage deinen Charakterdaten ein.';
			$message .= "\n";
        }
        if ( $daten['VBem'.$ref] != "" )
        	$message .= "Bemerkungen:\n" . stripslashes($daten["VBem$ref"])."\n";
        if ( $daten['Geld'.$ref] != '' )
          $message .= "Geldeingang: ".$daten['Geld'.$ref]."\n";
        if ( $daten["VUnter$ref"] != "" )
          $message .= "Unterbringung: ".$daten["VUnter$ref"]."\n";
        $message .= "Datum: ".date("d.m.Y H:i")."\n";
        if ( $daten['Geld'.$ref] == '' )
        {
           $message .= 'Achtung: Dies ist eine vorläufige Anmeldung solange kein Zahlungseingang verbucht wurde.';
           $message .= "\n".'Die Bankverbindung ist auf der Anmeldeseite der Homepage (http://drow-con.de/index.php?id=123) vermerkt.';
        } 
        // an Hausverantwortlichen senden 
		if ( $daten['VAkzeptiert'.$ref] == '' )
		{
			$sql = "SELECT Verantwortlich, EMail FROM T_Haeuser WHERE Hausname='".mysql_real_escape_string($daten['VGruppe'.$ref])."'";
			$query = sql_query($sql);
  			if ( $row = sql_fetch_row($query) )		
  			{
  				mail($row[1],$docinfo['title'].' - Neue Hausanmeldung',
  				'Hallo '.$row[0]."\n\n".
  				'es gibt eine neue Anmeldung für das Haus '.$daten['VGruppe'.$ref].".\n".
  				'Als Verantwortlicher für das Haus musst du Entscheiden, ob die Anmeldung angenommen wird oder nicht.'."\n".
  				'Spieler: '.$daten['Name'].' '.$daten['Vorname']."\n".
  				'Nach dem Login auf drow-con.de kannst du unter folgendem Link weitere Informationen finden und ggf. Kontakt aufnehmen:'."\n".
  				'http://www.drow-con.de/index.php?id=9&user='.$SpielerNr."\n".
  				'Nach der Prüfung des Spielers kannst du auf der Hausübersichtsseite die Anmeldung annehmen oder ablehnen.'."\n\n".
  				'deine Spielleitung',
      			"From: ".$docinfo['MeldungsEMail']);
      		}
      		else
      		{
      			mail($docinput['MeldungsEMail'],'Fehler bei Anmeldung','Keine EMail für Haus '.
      			$daten['VGruppe'.$ref].' eingetragen!');
      		}
  			sql_free_result($query);
  		}
        if ( $neu )
        {
          // An Spieler senden
          mail(get_user_email($SpielerNr), $docinfo['title']." - Anmeldung", $message,
            "From: ".$docinfo['MeldungsEMail']);
          // an Orga senden
          if ( strtotime("+18 year",$daten["birthday"]) > strtotime($docinfo["Beginn"]))
           $message .= "\nAchtung: Teilnehmer ist minderjährig!!!!\n";
           mail($docinfo["MeldungsEMail"], $docinfo['title']." - Neue Anmeldung ($SpielerNr)",
          $message."\neMail: ".get_user_email($SpielerNr), "From: ".$docinfo["MeldungsEMail"]);
            $neu = false;
        }
        else
          mail($docinfo["MeldungsEMail"], $docinfo['title']." - Anmeldung geändert ($SpielerNr)",
          $message."\neMail: ".get_user_email($SpielerNr), "From: ".$docinfo["MeldungsEMail"]);
        echo '<b>&gt;&gt; Ihre Daten wurden gespeichert!</b><br /><br />';
      }// if AGB
      else
      {
        echo '<big><b>&gt;&gt;&gt; Sie müssen die <a href="'.$docinfo['AGBUrl'].'" target="_blank">AGB akzeptieren!</b></big><br /><br />';
        $daten["V$ref"] = $docinput["V$ref"];
        if ( isset($docinput['Gruppe'])) $daten["VGruppe$ref"] = $docinput['Gruppe'];
        $daten["VBem$ref"] = $docinput["VBem$ref"];
        $neu = true;
      }
    } // Speichern
    if ( $daten["V$ref"] == '' ) { $daten["V$ref"] = "SC"; $neu = true; }
    echo $design['table_begin'];
    echo '<form action="?id='.$id.'&Ref='.$ref.$Verweis.'" method="post">';
    ?>
<tr bgcolor="<?=$design['table_row_dark']?>"><th>
Anmeldung für <?=$docinfo['title']?></th></tr>
<tr bgcolor="<?=$design['table_row_bright']?>"><td align="center"><b>Spieler: <?=$daten['Vorname']." ".$daten['Name']?></b>, Eingang: <?
  if (isset($daten["VEingang$ref"]))
    echo timestamp_to_string($daten["VEingang$ref"]);
  else
    echo timestamp_to_string(time());
?></td></tr>
<?
  if ( $daten["VWarte$ref"] == "1" )
  if ( $daten['Geld'.$ref] != '') {
    echo '<tr><td align="center">Keine Plätze frei ! Anmeldung steht auf der Warteliste!</td></tr>';
  }
  else
    echo '<tr><td align="center">Vorläuftige Anmeldung, solange kein Geldeingang verbucht wurde.</td></tr>';
?>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<input type="hidden" name="docinput[V<?=$ref?>]" value="SC" />
Teilnahmehinweise: <input type="text" name="docinput[Spezial]" value="<?=$spez?>" size="20" 
<?php
if ($profile['SteinsbergSL'] != '1') 
{
	echo 'disabled="disabled"';
}
?> />
<br />
<br />
Hauszugehörigkeit 
<?php if ( $daten['VAkzeptiert'.$ref] != '' ) { echo $daten['VGruppe'.$ref]; }
else
{
?>
(vorbehaltlich Zustimmung des Hausvorsitzes - <?=transform('%%Hausübersicht%%','extrans')?>)<br />
<select name="docinput[Gruppe]">
<option <?php if (!isset($daten['VGruppe'.$ref]) || $daten["VGruppe$ref"] == "-Alleinreisend-" ) echo 'checked="checked"';?>>-Alleinreisend-</option>
<?php
  $sql = "SELECT Hausname, Haus_id FROM T_Haeuser ORDER BY Hausname";
  $query = sql_query($sql);
  while ( $row = sql_fetch_row($query) )
  {
      echo "<option ";
      if ( stripslashes($daten["VGruppe$ref"]) == stripslashes($row[0]) ) echo 'selected="selected"';
      echo ">" . stripslashes($row[0]) . "</option>";
  }
  sql_free_result($query);
  ?>
</select>
<br />
Wenn du ein neues Haus gründen willst, bewirb dich bitte bei der Spielleitung (unter hausanmeldung at drow-con.de)
<br />
<?php 
if ( ! $neu && $daten['VAkzeptiert'.$ref] == '')
{
?>
<b>Der Hausverantwortliche hat diese Anmeldung noch nicht akzeptiert. Charakterinformationen können erst
eingegeben werden, wenn die Anmeldung für das Haus akzeptiert wurde. Du wirst benachrichtigt, wenn die 
Entscheidung getroffen wurde.</b><br />
<?php  
}
} // if not akzeptiert
?>
<br />
Bemerkungen zur Anmeldung (besondere Absprachen, Früher-/Späterkommen, Abholwünsche etc.)<br />
<textarea name="docinput[VBem<?=$ref?>]" cols="60" rows="5"><?=stripslashes($daten["VBem$ref"])?>
</textarea>
<?php
if ( $profile["SteinsbergSL"] == "1" )
{
  echo "<br /><br />";
  echo "Interne Hinweise (nur für SL lesbar)<br />";
  echo '<textarea name="docinput[VSLBem'.$ref.']" cols="60" rows="5">';
  echo stripslashes($daten["VSLBem$ref"]);
  echo '</textarea>';
}
?>
<br />
<br />
Geldeingang <small>(letzte Aktualisierung: <? $dd = get_nodedata($ref, $flavour);
echo $dd["GeldDatum"]; ?>)</small><br/>
<?php
  if ( $profile['SteinsbergGeld'] == "1" )
    echo '<input type="text" name="docinput[Geld'.$ref.']" size="40" value="'.
      $daten['Geld'.$ref].'"/> &euro;';
  else
    if ( $daten["Geld$ref"] == "" )
      echo "<b>Vorläufige Anmeldung, da bisher kein Geldeingang.</b>";
    else
      echo "<b>".$daten["Geld$ref"]." &euro;</b>";
?>
<br /><br/>
Unterbringung (vorbehaltlich Änderungen)<br/>
<?php
  if ( $profile['SteinsbergGeld'] == "1" )
    echo '<input type="text" name="docinput[VUnter'.$ref.']" size="40" value="'.
      $daten['VUnter'.$ref].'"/> ';
  else
    if ( $daten["VUnter$ref"] == "" )
      echo "<b>-- noch nicht festgelegt--</b>";
    else
      echo "<b>".$daten["VUnter$ref"]."</b>";
?>
<br /><br/>
<?php
  if ( $Verweis != "" )
    echo '<input type="hidden" name="Spieler" value="'.$SpielerNr.'">';
?>
Ich erkenne die <a href="<?=$docinfo['AGBUrl']?>" target="_blank">AGB</a> des Veranstalters an <input type="checkbox" value="v" name="AGB"></input>
<br />
<input type="submit" name="save" value="Anmeldedaten speichern" />
<?php
if ( $profile['SteinsbergSL'] == "1"  || $profile['SteinsbergGeld'] == "1")  {
  echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
  echo '<a href="'.get_url($ref,$Verweis.'&Cancel=1').'">Anmeldung widerrufen</a>';
}
?>
</td></tr>
</form>
<tr><td></td></tr>
<?php
    echo $design['table_end'];

    if ( ! $neu && ($daten["V$ref"] != "SC" || $daten['VAkzeptiert'.$ref] != "" ))
    {
      echo "<p>&nbsp;</p>";
      echo $design['table_begin'];
?>
<tr bgcolor="<?=$design['table_row_dark']?>"><th>Wichtige Zusatzinformationen</th></tr>
<? if ( $daten["V$ref"] != "SC" )
  {
?>
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
  Bitte <a href="<?=get_url(get_cmodule_id("steinsberg_nsc"),"Ref=$ref$Verweis")?>">
NSC-Zusatzinformationen</a> angeben.
</td></tr>
<? }
if ( $daten["V$ref"] != "NSC" && $daten['VAkzeptiert'.$ref] != '') {
?>
<form action="?id=<?=$id?>&Ref=<?=$ref?>#Char" method="post">
<tr bgcolor="<?=$design['table_row_bright']?>"><td>
<a name="Char"></a>Charakter für  <?=$docinfo['title']?><br>
<?php
  $sql = "SELECT ".TABLE_NODES.".id FROM ".TABLE_NODES." LEFT JOIN ".TABLE_NODEDATA." ON
    ".TABLE_NODES.".id = ".TABLE_NODEDATA.".node_id WHERE type_id = ".
      $doctypes['drow_charakter']." AND name = 'SpielerID' AND value='".
        $SpielerNr."'";
  $query = sql_query($sql);
  if ( sql_num_rows($query) > 0 )
  {
    echo '<select name="docinput[charakter]">';
    echo '<option value="">-keiner gewählt-</option>';
    $charakter['name'] = array();
    $charakter['id'] = array();
    while ( $erg = sql_fetch_row($query) )
    {
      echo '<option value="'.$erg[0].'"';
      if ( $daten["Char$ref"] == $erg[0] ) echo ' selected="selected"';
      echo '>';
      $nd = get_nodedata($erg[0],$flavour);
      echo $nd['title'];
      $charakter['name'][] = $nd['title'];
      $charakter['id'][] = $erg[0];
      echo "</option>";
    }
    echo '</select>';
    echo '<br />';
    echo '<input type="submit" name="savechar" value="Charakterauswahl speichern" />';
  }
  else
    echo "<b>Bisher keine Charaktere vorhanden!</b>";
  sql_free_result($query);
?>
<?
  reset($charakter['name']);
  reset($charakter['id']);
  echo "<br />Charaktere bearbeiten:";
  echo "<ul>";
  while ( list($key, $value) = each($charakter['name']) )
  {
    echo '<li><a href="'.get_url($charakter['id'][$key],'Ref='.$ref).'">'.$value."</a> bearbeiten</li>";
  }
?>
<li>Neuen Charakter <a href="<?=get_url($id,"newchar=1&Ref=$ref$Verweis")?>#Char">anlegen</a>
</li>
</ul>
<br />
<?php
  if ( $Verweis != "" )
    echo '<input type="hidden" name="Spieler" value="'.$SpielerNr.'">';
?>

</td></tr>
</form>
<?
      }
      echo $design['table_end'];
    }
  } // else ! editor
}
}
?>