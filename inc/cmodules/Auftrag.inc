<?php

/* Modul Auftrag

AWF-Name: auftrag
Parameter:
Delete - Löscht Auftrag mit id

Auftrag - id des Auftrages
Titel, Betroffen, Status, Prioritaet, Ort - Speichert einen Auftrag
newauf - Neuer Auftrag

docinput[VTitel] - Vorgabe für Titel

*/

include(INC_PATH.'misc/CVJM.inc');
include(INC_PATH.'misc/cvjmArtikeltools.inc');
include(INC_PATH.'misc/cvjmAuftragstools.inc');

echo '
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>';

require_once(INC_PATH.'Smarty/Smarty.class.php');
$Smarty = new Smarty;
$Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';

// Namen für Prioritäten und Status
$Prioritaeten[1]['name'] = LANG_SOFORT;
$Prioritaeten[1]['color'] = 'style="background-color:#CC0066;color:white"';
$Prioritaeten[2]['name'] = LANG_DRINGEND;
$Prioritaeten[2]['color'] = 'style="background-color:#FF8080"';
$Prioritaeten[3]['name'] = LANG_WICHTIG;
$Prioritaeten[3]['color'] = 'style="background-color:#FF9966"'; //#993366
$Prioritaeten[4]['name'] = LANG_NORMAL;
$Prioritaeten[4]['color'] = "";
$Prioritaeten[5]['name'] = LANG_HATZEIT;
$Prioritaeten[5]['color'] = '';
$Status[1] = LANG_OFFEN;
$Status[2] = LANG_INARBEIT;
$Status[3] = LANG_UNTERBRECHUNG;
$Status[4] = LANG_ERLEDIGT;
$BenutzerURL = '';
$Benutzer = SESSION_DBID;
if ( isset($profile[CVJM_VERWALTUNGSGRUPPE]))
{
  	if ( isset($docinput['Benutzer']) && is_numeric($docinput['Benutzer']))
  	{
  		$Benutzer = $docinput['Benutzer'];
  		$BenutzerURL = '&docinput[Benutzer]='.$Benutzer;
  	}
}

if ( is_numeric($_REQUEST['Delete']) )
{
  $qu = sql_query('SELECT Autor FROM '.TABLE_AUFTRAG.' WHERE id = '.$_REQUEST['Delete'].' AND Autor = '.$Benutzer);
  if ( sql_num_rows($qu) == 1 ) { // Nur l�schen, wenn der Auftrag einem geh�rt
    $sql = 'DELETE FROM '.TABLE_AUFTRAG.' WHERE id = '.$_REQUEST['Delete'];
    if ( ! sql_query($sql) ) echo "<strong>&gt;&gt; L�schfehler: ". sql_error().'</strong><br /><br />';
    else
      echo '<strong>&gt;&gt;&gt; '.LANG_AUFTRAG.' '.$_REQUEST['Delete'].' '.LANG_DELETED.'.</strong><br /><br />';
  }
  sql_free_result($qu);
}

if ( isset($_REQUEST['Titel']) )
{
  $Betroffen = '';
  if ( isset($_REQUEST['Betroffen']))
  {
    $Betroffen = ','.implode(',', $_REQUEST['Betroffen']).',';
    if ( $Betroffen == '' ) $Betroffen = ','.$Benutzer.',';
  }
  if ( $Betroffen == '' ) $Betroffen = ','.$Benutzer.',';
  if ( is_numeric($_REQUEST['Status']) && is_numeric($_REQUEST['Prioritaet']) &&
            is_numeric($_REQUEST['Ort']) )
  {
    if ( $_REQUEST['Titel'] == '' ) // Nur mit Titel speichern
    {
      echo '<strong>&gt;&gt;&gt; Sie m�ssen einen Titel eingeben! ';
      echo '<a href="javascript:history.back();">Zur�ck</a></a></strong><br /><br />';
    }
    else
    {      
      $BearbeitenBis = convertDatumToTimestamp($_REQUEST['BearbeitenBis']); 
      if ( is_numeric($_REQUEST['Auftrag']) )
      {
        updateAuftrag($_REQUEST['Auftrag'], $_REQUEST['Titel'], $_REQUEST['Beschreibung'],
          $_REQUEST['Bemerkung'], $_REQUEST['Prioritaet'], $Betroffen, $BearbeitenBis, $_REQUEST['Ort'],
          $_REQUEST['Status']);
        $Bem = sql_real_escape_string($row['Bemerkung'])."\n".
         'ge�ndert durch '.html_entity_decode(get_user_nickname());
    	$Bem2 = sql_real_escape_string($row['Beschreibung'])."\n".$Bem;
        mail(get_user_email($row['Autor']), '[Auftrag '.$_REQUEST['Titel'].'] ge�ndert',
           'Von: '.get_user_email(SESSION_DBID)."\nDatum: ".date('d.m.Y H:i')."\n".
           $Bem2, 'From: '.get_user_email(SESSION_DBID));
      }
      else
      {
        // INSERT
        insertAuftrag($_REQUEST['Titel'], $_REQUEST['Beschreibung'], $_REQUEST['Prioritaet'],
          $Betroffen, $BearbeitenBis, $_REQUEST['Ort'], $_REQUEST['Status']);
      }
    }
  }
  else
   echo '<strong>&gt;&gt;&gt; Sie m�ssen eine g�ltige Priorit�t, Status und Ort angeben!</strong><br />';
}
// Abfrage, ob der Status eines Auftrages ge�ndert werden soll
if ( isset($_REQUEST['Auftrag']) && isset($_REQUEST['NewStatus']) &&
     is_numeric($_REQUEST['Auftrag']) && is_numeric($_REQUEST['NewStatus']) ) {
  $qu = sql_query('SELECT Beschreibung,Bemerkung, Autor, Titel FROM '.TABLE_AUFTRAG.' WHERE id = '.$_REQUEST['Auftrag']);
  if ( $row = sql_fetch_array($qu) )
  {

    $Bem = sql_real_escape_string($row['Bemerkung'])."\n".
         $Status[$_REQUEST['NewStatus']].' am '.date('d.m.Y H:i').' - '.
         html_entity_decode(get_user_nickname());
    $Bem2 = sql_real_escape_string($row['Beschreibung'])."\n".$Bem;
    if ( ! sql_query('UPDATE '.TABLE_AUFTRAG.' SET Status = '.$_REQUEST['NewStatus'].
         ", Bemerkung = '$Bem' WHERE id = ".$_REQUEST['Auftrag']) )
      echo '<strong>&gt;&gt; Datenbankfehler: '.sql_error().'</strong><br /><br />';
    else // if ( $_REQUEST['NewStatus'] == 4 ) // Auftraggeber eine Mail schicken
      mail(get_user_email($row['Autor']), '[Auftrag '.$row['Titel'].'] '.$Status[$_REQUEST['NewStatus']],
           'Von: '.get_user_email(SESSION_DBID)."\nDatum: ".date('d.m.Y H:i')."\n".
           $Bem2, 'From: '.get_user_email(SESSION_DBID));
  }
  sql_free_result($qu);
}

// wenn kein einzelnen Auftrag angezeigt wird, dann Tabelle anzeigen
if ( ! ((isset($_REQUEST['EditAuftrag']) && is_numeric($_REQUEST['EditAuftrag'])) ||
  isset($_REQUEST['newauf'])))
{
  //if ( $_REQUEST['Unterbrochen'] == 1 )
  //{
    remove_profile('AuftragUnterbrochen');
    unset($profile['AuftragUnterbrochen']);
  //}
  //else
  //{
  //  set_profile('AuftragUnterbrochen', 1);
  //  $profile['AuftragUnterbrochen'] = 1;
 // }
  if ( isset($_REQUEST['MyWork']) )
    echo '<h2>Auftr�ge von '.get_user_nickname($Benutzer).', Stand '.date('d.m.Y H:i').'</h2>';
  else
    echo '<h2>Auftr�ge f�r '.get_user_nickname($Benutzer).', Stand '.date('d.m.Y H:i').'</h2>';
  //if ( $profile['AuftragUnterbrochen'] != 1 )
  //  echo '[ <a href=''.get_url($id,'Unterbrochen=0').''>Unterbrochene ausblenden</a> ] ';
  //else
  //  echo '[ <a href=''.get_url($id,'Unterbrochen=1').''>Unterbrochene anzeigen</a> ] ';
  echo '<div class="noprint">';
  echo '[ <a href="'.get_url($id,'newauf=1').'">'.LANG_HINZUFUEGEN.'</a> ] [ <a href="';
  if ( ($_REQUEST["Alle"] == 1 || $profile["AlleAuftraege"] == 1) && $_REQUEST["Alle"] != "0" ) {
    $Where = "";
    if ( !isset($profile["AlleAuftraege"]) ) set_profile("AlleAuftraege", 1);
  }
  else {
    $Where = "WHERE Status < 4";
    if ( $profile["AlleAuftraege"] == 1 ) remove_profile("AlleAuftraege");
  }
  if ( $profile["AuftragUnterbrochen"] == 1 )
    if ( $Where == "" )
      $Where = "WHERE Status <> 3";
    else
      $Where .= " AND Status <> 3";
  if ( is_numeric($_REQUEST["SearchOrt"] ) ) {
    if ( $Where == "" ) $Where = "WHERE Ort = ".$_REQUEST["SearchOrt"];
    else
     $Where .= " AND Ort = ".$_REQUEST["SearchOrt"];
  }
  if ( ! isset($profile["AlleAuftraege"]) ) echo get_url($id,'Alle=1'.$BenutzerURL).'">auch ';
    else echo get_url($id,'Alle=0'.$BenutzerURL).'">nur un';
  echo 'erledigte anzeigen</a> ] ';
  if ( isset($_REQUEST["SearchOrt"]))
    echo '[ <a href="'.get_url($id,$BenutzerURL).'">'.LANG_ALLEORTE.'</a> ] ';
  if ( isset($_REQUEST["MyWork"]) )
    echo '[ <a href="'.get_url($id).'">Nur Auftr�ge f�r mich anzeigen</a> ] ';
  else
    echo '[ <a href="'.get_url($id,'MyWork=1'.$BenutzerURL).'">Auftragsstatus erteilter Auftr�ge anzeigen</a> ] ';
echo '</div>';
  echo $design['table_begin'];
  echo '<table width="100%" border="1" style="border-collapse:collapse;border-color: '.
        $design['table_border'].';" class="drucklinien">';
  echo '<tr style="background: '.$design['table_header'].
       ';border: thin solid;border-bottom-width: '.
       '2pt; border-left-width: 1pt;' .
       'border-right-width: 1pt; border-top-width: 1pt; padding: 2pt">';
  echo '<th><a title="'.LANG_BEZEICHNUNGSORTIEREN.'" href="'.get_url($id,'Sort=Titel'.$BenutzerURL).
        '">'.LANG_BEZEICHNUNG.
       '</a></th><th><font size="-1"><a href="'.get_url($id,'Sort=Prioritaet'.$BenutzerURL).'">';
  echo LANG_PRIORITAET.'</a>/<br/><a href="'.get_url($id,'Sort=Status'.$BenutzerURL).'">'.
    LANG_STATUS.'</a></font></th>';
  // echo '<th><font size="-1">'.LANG_DATUMAUTOR.'</font></th>';
  echo '<th><font size="-1">'.LANG_NEUERSTATUS.'</font></th></tr>';


  if ( !isset($_REQUEST["MyWork"]) ) {
    if ( $Where == "" ) $Where = "WHERE (";
     else $Where .= " AND (";
    $Where .= "Betroffene LIKE '%,".$Benutzer.",%' ";
    if ( $Benutzer != SESSION_DBID) 
    {
    	$p = get_profile($Benutzer);
    }
    else
    {
    	$p = $profile;
    }
    while ( list($key, $value) = each($p) )
      if ( substr($key, 0, 6) == "group_" ) $Where .= "OR Betroffene LIKE '%,g".substr($key, 6).",%' ";
    $Where .= ")";
  }
  else
    if ( $Where == "" )
      $Where = "WHERE Autor = ".SESSION_DBID;
    else $Where .= " AND Autor = ".SESSION_DBID;
  $sql = "SELECT id, Titel, Prioritaet, Status , Bemerkung, Beschreibung, Datum, Ort, ".
         "DATE_FORMAT(Aenderung, '%Y-%m-%d %T') As Aenderung, Betroffene," .
         "Autor FROM ".TABLE_AUFTRAG." $Where ORDER BY ";
  if ( isset($_REQUEST["Sort"]) && $_REQUEST["Sort"] != "" )
    $sql .= $_REQUEST["Sort"].",Status";
  elseif ($profile['AlleAuftraege'] == 1)
    $sql .= 'Datum DESC';
  else
    $sql .= "Prioritaet, Datum DESC";
    if ( $qu = sql_query($sql) ) {
    while ( $row = sql_fetch_array($qu)) {
      echo '<tr><td valign="top"><a title="'.LANG_AUFTRAGBEARBEITEN.'" href="'.
        get_url($id,"EditAuftrag=".$row["id"].$BenutzerURL).'">';
      echo stripslashes($row["Titel"])."</a> ";
      if ( $row['Ort'] != -1 ) {
        $ort = getOrtsname($row["Ort"]);
        echo '<em>( <a title="Alles zu '.OrtsName($row["Ort"], false).' anzeigen" href="'.
          get_url($id,'SearchOrt='.$row["Ort"].$BenutzerURL).'">'.$ort."</a> )</em>";
      }
      echo "&nbsp; von ".transform($row["Autor"],"user_id");
      echo ' erstellt '.transform(strtotime($row["Datum"]),"datetime");
      echo "<br />".nl2br(stripslashes($row["Beschreibung"]));
      if ($row["Bemerkung"] != "")
        echo "<br />".nl2br(stripslashes($row["Bemerkung"]));
      echo "</td>";
      echo '<td valign="top" align="center"><span '.$Prioritaeten[$row["Prioritaet"]]["color"].'>'.
            $Prioritaeten[$row["Prioritaet"]]["name"]."</span>".
           "<br /><i ";
      if ( $row["Status"] == 4 ) echo 'style="background-color:#80FF80"';
      echo ">".$Status[$row["Status"]]."</i>";
      //if ( isset($_REQUEST["MyWork"]) ) {
        $wer = explode(",",$row["Betroffene"]);
        echo "<br />";
        while ( list($key, $value) = each($wer) )
          if ( substr($value, 0, 1) == "g" )
            echo $groups[substr($value,1)]."<br />";
          elseif ( is_numeric($value) > 0 )
            echo transform($value,"user_id")."<br />";
      //}
      echo "</td>";
  //    echo '<td><font size="-2">'.date("d.m.Y H:i",strtotime($row['Datum'])).'<br />'.
  //         transform($row["Autor"], 'user_id')."<br />".date("d.m.Y H:i",strtotime($row['Aenderung']));
  //    echo "</font></td>";
      echo '<td valign="top" align="center"><font size="-2">';
      foreach ($Status as $key => $value) 
      {
        if ( $key != $row["Status"] )
          echo '[&nbsp;<a title="auf '.$Status[$key].' setzen" href="'.
            get_url($id,'Auftrag='.$row["id"].'&NewStatus='.$key.$BenutzerURL).'">'.$value."</a>&nbsp;]<br />";
      }
      echo '</font></td></tr>';
    }
  }
  else
    die("Datenbankfehler: ".sql_error());

  echo $design['table_end'];
  if ( sql_num_rows($qu) > 0 ) {
    echo sql_num_rows($qu);
    if ( sql_num_rows($qu) == 1 )
      echo " ".LANG_AUFTRAG;
    else
      echo " Auftr�ge";
  }
  sql_free_result($qu);
  echo "<br /><br />";
  
  // Auftr�ge anderer ansehen
  if ( isset($profile[CVJM_VERWALTUNGSGRUPPE]) )
    {
      // Berechtigter Nutzer: Anzeige der User
      $query = "SELECT id FROM ".TABLE_USERS.' WHERE valid';
      $query = sql_query($query);
      while ( $row = sql_fetch_row($query))
      {
        $user[$row[0]] = get_user_nickname($row[0]);
      }
      sql_free_result($query);
      //reset($user);
      //array_multisort($user, SORT_ASC);
      echo '<hr class="noprint" /><form action="'.get_url($id).'" method="post" class="noprint">';
      echo "Auftr�ge von Benutzer ";
      echo '<select name="docinput[Benutzer]" size="1">';
      $u = $user;
      asort($u);
      foreach ( $u  as $key => $value )
      {
        echo '<option value="'.$key.'" ';
        if ( $Benutzer == $key ) echo 'selected="selected"';
        echo '>'.stripslashes($value).'</option>';
      }
      echo '</select>';
      echo '<input type="Submit" value="anzeigen">';
      echo '</form>';
    }
}
else
// einen speziellen Auftrag anzeigen
if ( is_numeric($_REQUEST["EditAuftrag"]) || isset($_REQUEST["newauf"]))
{
  OrtsAuswahlScriptEinfuegen();
  echo '<form action="?id='.$id.'" method="post" name="Auftrag">';
  $Titel = $docinput["VTitel"];
  $Bemerkung = "";
  $BearbeitenBis = '';
  $Beschreibung = "";
  $Prioritaet = 4; // normal
  $Ort = -1;  // Kein Ort
  $Statuswert = 1; // offen
  $Autor = -1;
  if ( is_numeric($_REQUEST["EditAuftrag"]) ) {
    if ( ! $qu = sql_query('SELECT *, DATE_FORMAT(Aenderung, "%Y-%m-%d %T") As Aenderung FROM '.
      TABLE_AUFTRAG.' WHERE id = '.$_REQUEST["EditAuftrag"]))
      echo "<strong>&gt;&gt; Datenbankfehler: ".sql_error()."</strong><br /><br />";

    if ( $row = sql_fetch_array($qu) ) {
      echo '<input type="hidden" name="Auftrag" value="'.$_REQUEST["EditAuftrag"].'">';
      $Titel = stripslashes($row["Titel"]);
      $Bemerkung = stripslashes($row["Bemerkung"]);
      $Prioritaet = $row["Prioritaet"];
      $Beschreibung = stripslashes($row["Beschreibung"]);
      $Statuswert = $row["Status"];
      $Ort = $row["Ort"];
      $Autor = $row["Autor"];
      $BearbeitenBis = $row['BearbeitenBis'];
      if ( $BearbeitenBis != 0 ) 
        $BearbeitenBis = date('d.m.Y', $BearbeitenBis);
      else
        $BearbeitenBis = '';
      echo '<h2>'.$Titel.'</h2>';
      echo "Auftrag erstellt von ".transform($row["Autor"],"user_id")." am ";
      echo transform(strtotime($row["Datum"]),"datetime");
      if ( $row["Datum"] != $row["Aenderung"] )
        echo " zuletzt ge�ndert am ".transform(strtotime($row["Aenderung"]),"datetime");
      echo "<br />";
    }
    sql_free_result($qu);
  }
  echo '<strong>'.LANG_BEARBEITER.'</strong> <select name="Betroffen[]" multiple="multiple" size="10">';
  $wer = explode(",",$row["Betroffene"]);
  reset($groups);
  foreach ($groups as $key => $value)  {
    echo '<option value="g'.$key.'"';
    if ( in_array("g".$key, $wer) ) echo ' selected="selected"';
    echo '>'.$value.'</option>';
  }
  $qres = sql_query("SELECT ".TABLE_USERS.".id, value FROM ".TABLE_USERS." INNER JOIN ".TABLE_USERDATA.
    " ON ".TABLE_USERS.".id = user_id WHERE valid = 1 AND name='nickname' ORDER BY value");
  while ( $row = sql_fetch_array($qres) ) {
    echo '<option value="'.$row["id"].'"';
    if ( in_array($row["id"], $wer) ) echo ' selected="selected"';
    echo '>'.$row["value"].'</option>';
  }
  sql_free_result($qres);
  echo '</select> '.LANG_MEHREREBEARBEITER.'<br />';
  echo '<strong>'.LANG_TITEL.'</strong><br /><input type="Text" name="Titel" value="'.$Titel.'" size="50" maxlength="50"><br />';
  echo '<strong>'.LANG_BESCHREIBUNG.'</strong><br /><textarea name="Beschreibung" cols="60" rows="5">'.$Beschreibung.'</textarea><br />';
  echo '<strong>'.LANG_PRIORITAET.'</strong> ';
  echo '<select name="Prioritaet">';
  while ( list($key, $value) = each($Prioritaeten) )
  {
    echo '<option value="'.$key.'"';
    if ( $Prioritaet == $key )
      echo ' selected="selected" ';
    echo '>'.$value["name"].'</option>';
  }
  echo '</select>';
  echo '&nbsp;&nbsp;&nbsp;&nbsp;<strong>'.LANG_STATUS.'</strong> ';
  echo '<select name="Status">';
  reset($Status);
  foreach ($Status as $key => $value)
  {
    echo '<option value="'.$key.'"';
    if ( $Statuswert == $key )
      echo ' selected="selected" ';
    echo '>'.$value.'</option>';
  }
  echo '</select>';
  echo "<br />";
  echo 'Erledigen bis ';
  echo '<input type="text" name="BearbeitenBis" value="'.$BearbeitenBis.'"    
      onClick="popUpCalendar(this,this,\'dd.mm.yyyy\')" 
      onBlur="autoCorrectDate(\'Auftrag\',\'BearbeitenBis\',false);"          
  />';
  echo '<br >/';
  echo '<strong>'.LANG_ORT.'</strong> ';
  echo '<input type="hidden" name="Ort" value="'.$Ort.'" id="Ortsauswahl" />';
  if ( isset($_REQUEST["MyWork"]) )
    echo '<input type="hidden" name="MyWork" value="1" />';
  echo '<span name="Ortname" id="Ortsauswahltext">'.OrtsName($Ort,true).'</span>';
  echo '&nbsp;&nbsp;[ <a href="javaScript:OrtsAuswahl(\'Ortsauswahl\')">Ort ausw�hlen</a> ]<br />';
/*
  echo '<select name="Ort">';
  echo '<option value="-1">'.LANG_OHNEORT.'</option>';
  $qu = sql_query("SELECT id FROM ".TABLE_ARTIKEL." WHERE Art = 'Ort' ORDER BY Bezeichnung");
  while ( $row = sql_fetch_array($qu) )
  {
    echo '<option value="'.$row['id'].'" ';
    if ( $Ort == $row['id'] ) echo ' selected="selected"';
    echo '>'.OrtsName($row['id'], false).'</option>';
  }
  sql_free_result($qu);
  echo '</select><br />';
*/
  if ( ! isset($_REQUEST["newauf"]))
    echo '<strong>'.LANG_BEMERKUNGEN.'</strong><br /><textarea name="Bemerkung" cols="60" rows="5">'.$Bemerkung.'</textarea><br />';
  echo '<input type="Submit" name="saveauf" value="'.LANG_SUBMIT.'">';
  if ( is_numeric($_REQUEST["EditAuftrag"]) && $Autor == SESSION_DBID )
    echo '&nbsp;&nbsp;&nbsp;&nbsp;[ <a title="'.LANG_LOESCHEN.'" href="'.
      get_url($id,'Delete='.$_REQUEST["EditAuftrag"]).'">'.LANG_LOESCHEN.'</a> ]';
  echo '&nbsp;&nbsp;&nbsp;&nbsp;[ <a title="'.LANG_LISTEANZEIGEN.'" href="'.get_url($id).
    '">'.LANG_LISTEANZEIGEN.'</a> ]';
  echo '</form>';
}

echo '<div class="noprint">';
echo '[ <a href="'.get_url($id,'newauf=1').'">'.LANG_HINZUFUEGEN.'</a> ] [ <a href="';
if ( ! isset($profile["AlleAuftraege"]) ) echo get_url($id,'Alle=1'.$BenutzerURL).'">auch ';
    else echo get_url($id,'Alle=0'.$BenutzerURL).'">nur un';
  echo 'erledigte anzeigen</a> ] ';
  if ( isset($_REQUEST["SearchOrt"]))
    echo '[ <a href="'.get_url($id,$BenutzerURL).'">'.LANG_ALLEORTE.'</a> ] ';
  if ( isset($_REQUEST["MyWork"]) )
    echo '[ <a href="'.get_url($id).'">Nur Auftr�ge f�r mich anzeigen</a> ] ';
  else
    echo '[ <a href="'.get_url($id,'MyWork=1'.$BenutzerURL).'">Auftragsstatus erteilter Auftr�ge anzeigen</a> ] ';
echo '</div>';
?>
