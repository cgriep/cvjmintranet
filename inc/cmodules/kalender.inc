<?php

load_lang("Kalender");
include(INC_PATH."misc/Termin.inc");
include(INC_PATH."misc/Kalenderjs.inc");
if ( file_exists(INC_PATH."misc/CVJM.inc") )
{
  include(INC_PATH."misc/CVJM.inc");
  OrtsAuswahlScriptEinfuegen();
}

// QUICK REFERENCE
// :: ms = Mark Style
// :: ds = Day Start
// :: ot = Offset Time
// :: th = Table Height
// :: tw = Table Width
// :: cs = Cell Spacing
// :: cp = Cell Padding
// :: al = Cell Alignment
// :: nbc = Normal Numbers Background Color
// :: tfc = Todays Font Color
// :: mbc = Marked Numbers Background Color
// :: dtc = Day Names Table Background Color
// :: ntc = Blank Numbers Background Color
// :: mtc = Month and Year Table Background Color
// :: fsm = Font Fize Month
// :: fsd = Font Size Dates
// :: fsn = Font Size Numbers
// :: fwm = Font Weight Month Name
// :: fwd = Font Weight Day Names
// :: fwn = Font Weight Numbers

//$Standardcfg = "mtc=FFFFFF&al=2&cs=2&cp=2";

if($profile['editor']!='1') {
  if ($document['title'] != '') {
    echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
  }
  if($document['image'] != '' || $document['body'] != '') {
    echo '<p class="justify">';
    // Image
    echo transform($document['image'], $document['datatype']['image']);
    // Text
    echo transform($document['body'], $document['datatype']['body']).'</p>';
  }
  if ( isset($_REQUEST["Savetermin"]) && // $profile[$document["Terminrecht"]] == "1" &&
       $docinput['title'] != "" )
  {
      // neuen Termin anlegen
      clear_cache($id);
      // Datum aufarbeiten
      list($docinput["Tag"], $docinput["Monat"], $docinput["Jahr"]) = explode(".", $docinput["Datum"]);
      list($docinput["ETag"], $docinput["EMonat"], $docinput["EJahr"]) = explode(".", $docinput["EDatum"]);
      if ( ! checkdate($docinput["EMonat"],$docinput["ETag"],$docinput["EJahr"]) )
      {
        $docinput["ETag"] = "";
        $docinput["EMonat"] = "";
        $docinput["EJahr"] = "";
      }
      list($docinput["TurnusEndeTag"], $docinput["TurnusEndeMonat"], $docinput["TurnusEndeJahr"]) =
        explode(".", $docinput["TurnusDatum"]);
      if ( ! checkdate($docinput["TurnusEndeMonat"],$docinput["TurnusEndeTag"],$docinput["TurnusEndeJahr"]) )
      {
        $docinput["TurnusEndeTag"] = "";
        $docinput["TurnusEndeMonat"] = "";
        $docinput["TurnusEndeJahr"] = "";
      }
      $nodeid = create_node($id, $doctypes['Termin'],
                   count_children($doctypes['Termin']), 1, 0, "", $document['key_id'],
                   $document['searchable'], $document['cacheable'], 0, $document['members_only']);
      add_nodedata($nodeid,'timestamp',time(), $flavour);
      add_nodedata($nodeid, 'body', $docinput['body'], $flavour, "text");
      add_nodedata($nodeid, 'title', $docinput['title'], $flavour);
      add_nodedata($nodeid, 'Aendern', $docinput['Aendern'], $flavour);
      add_nodedata($nodeid, 'author', SESSION_EMAIL,$flavour,'email');
      add_nodedata($nodeid, 'author_id', SESSION_DBID, $flavour, 'user_id');
      add_nodedata($nodeid, 'Datum', mktime($docinput["Stunde"], $docinput["Minute"],0,
        $docinput["Monat"], $docinput["Tag"], $docinput["Jahr"]), $flavour, 'datetime');
      if ( $docinput["ETag"] == "" ) $docinput["ETag"] = $docinput["Tag"];
      if ( $docinput["EMonat"] == "" ) $docinput["EMonat"] = $docinput["Monat"];
      if ( $docinput["EJahr"] == "" ) $docinput["EJahr"] = $docinput["Jahr"];
      if ( $docinput["EStunde"] == "" ) $docinput["EStunde"] = $docinput["Stunde"];
      if ( $docinput["EMinute"] == "" ) $docinput["EMinute"] = $docinput["Minute"];
      add_nodedata($nodeid, 'Enddatum', mktime($docinput["EStunde"], $docinput["EMinute"],0,
          $docinput["EMonat"], $docinput["ETag"], $docinput["EJahr"]), $flavour, 'datetime');
      if ( is_numeric($docinput["TurnusEndeTag"] )) {
        $dd = mktime(0,0,0,$docinput["TurnusEndeMonat"],$docinput["TurnusEndeTag"], $docinput["TurnusEndeJahr"]);
      }
      else $dd = 0;
      add_nodedata($nodeid, 'TurnusEnde', $dd, $flavour);
      add_nodedata($nodeid, 'Ort', $docinput['Ort'], $flavour);
      add_nodedata($nodeid, 'Turnus', $docinput['Turnus'], $flavour);
      add_nodedata($nodeid, 'comments', 1, $flavour);
      $Person = "";
      $Person = implode(",",$docinput["Person"]);
      if ( $Person == "" && (count($docinput["Gruppe"]) == 0 || $_REQUEST["DelGrp"] == "v") )
        $Person = SESSION_DBID;
      if ( $_REQUEST["DelBet"] == "v" )
        $Person = "";
      if ( $Person != "" ) {
        add_nodedata($nodeid, 'Person', $Person, $flavour);
      }
      $Gruppe = "";
      $Gruppe = implode(",",$docinput["Gruppe"]);
      if ( $_REQUEST["DelGrp"] == "v") $Gruppe = "";
      if ( $Gruppe != "" ) {
        add_nodedata($nodeid, 'Gruppe', $Gruppe, $flavour);
      }
      add_nodedata($nodeid,'Aendern',$docinput["Aendern"],$flavour);
      echo '<strong>&gt;&gt; Termin <a href="'.get_url($nodeid).'">'.stripslashes($docinput['title']).'</a> angelegt.</strong><br />';
      // Zum eingegebenen Datum springen
      $_REQUEST["Datum"] = $docinput["Tag"].".".$docinput["Monat"].".".$docinput["Jahr"];
  }
  else if ( $docinput['title'] == "" && isset($_REQUEST["Neu"]) )
    echo "&gt;&gt; ".LANG_TITLE_REQUIRED."<br />";
  else if ( isset($_REQUEST["Termin"]) ) { //&& $profile[$document["Terminrecht"]] == "1" ) {
    // Neuen Termin eingeben
   $document["Datum"] = $_REQUEST["Zeit"];
   $document["Enddatum"] = $_REQUEST["Zeit"];
   $document["TurnusEnde"] = 0;
   include(INC_PATH."misc/termineingabe.inc");
  }
  else if ( isset($_REQUEST["Config"]) ) {
    include(INC_PATH."misc/kalmailconfig.inc");
  }
  // Anzeige aus dem Profil holen
  $Anzeige = $profile["KalenderAnzeige$id"];
  if ( !$Anzeige ) $Anzeige = $document["Anzeige"];
  if ( !$Anzeige ) $Anzeige = "Month";
  if ( isset($_REQUEST["Anzeige"]) ) {
    $Anzeige = $_REQUEST["Anzeige"];
    // letzte Anzeige im Profil sichern
    if ( SESSION_STATUS == "ok" ) set_profile("KalenderAnzeige$id", $Anzeige);
  }
  if ( $docinput["design"] != "Printable" ) {
?>
<form action="<?=get_url($id)?>" method="post" name="Springen">zu Datum
<input type="hidden" name="Anzeige" value="<?=$Anzeige?>">
<input type="Text" name="Datum" size="10" maxlength="10" <?=KalenderEvents("Springen","Datum");?>>
<input type="Submit" value="springen"></form>
<?php
  $Param = "";
  if ( isset($_REQUEST["Datum"]) )
    $Param = "Datum=".$_REQUEST["Datum"]."&";
  $Param .= "Anzeige=";
  if ( $docinput["design"] != "Printable" ) {
?>
<p><strong><?=LANG_ANZEIGE?>:</strong> <a href="<?=get_url($id,$Param)?>Day"><?=LANG_ONEDAY?></a> |
<a href="<?=get_url($id,$Param)?>Week"><?=LANG_SEVENDAYS?></a> |
<a href="<?=get_url($id,$Param)?>erwWeek"><?=LANG_NINEDAYS?></a> |
<a href="<?=get_url($id,$Param)?>Month"><?=LANG_MONTH?></a> |
<a href="<?=get_url($id,$Param)?>erwMonth"><?=LANG_THREEMONTH?></a> |
<a href="<?=get_url($id,$Param)?>Year"><?=LANG_YEAR?></a> | <a
href="<?=get_url($id)?>&Anzeige=<?=$Anzeige?>">heute</a>
<br />
<?php
  $Param .= $Anzeige;
?>
<strong>Ausgewählte Termine anzeigen:</strong> <a href="<?=get_url($id,$Param)?>&Betroffen=<?=SESSION_DBID?>">
alle Termine</a>, <a href="<?=get_url($id,$Param)?>&Selbst=<?=SESSION_DBID?>">persönliche Termine</a>
(Mailbenachrichtigung <a href="<?=get_url($id, array('Config'=>1))?>">konfigurieren</a>)</p>

<?php
  }
  if ( $profile["KalenderWer$id"] != "" ) {
    $Wer = $profile["KalenderWer$id"];
    $Was = $profile["KalenderWas$id"];
  }
  $Sichern = false;
  if ( isset($_REQUEST["Selbst"] ) ) {
    $Wer = $_REQUEST["Selbst"];
    $Was = "Selbst";
    $Sichern = true;
  }
  if ( isset($_REQUEST["Gruppe"] ) ) {
    $Wer = array();
    foreach ($_REQUEST["Gruppe"] as $key => $value )
      if ( is_numeric($value) )
        $Wer[] = $value;
    $Wer = implode(",",$Wer);
    $Was = "Gruppe";
    $Sichern = true;
  }
  if ( isset($_REQUEST["Betroffen"] ) ) {
    $Wer = $_REQUEST["Betroffen"];
    $Was = "Betroffen";
    $Sichern = true;
  }
  if ( SESSION_STATUS == "ok" && $Sichern ) {
    set_profile("KalenderWer$id", $Wer);
    set_profile("KalenderWas$id", $Was);
  }
  if ( $Was == "Selbst" ) {
    $zeigen = "";
    $_REQUEST["Selbst"] = $Wer;
    $wer = explode(",", $Wer);
    foreach ($wer as $w => $n)
      $zeigen .= "<i>".get_user_nickname($n)."</i> ";
    echo '<font size="+1">'.str_replace("%%WER%%", $zeigen, LANG_ANZEIGE_BESCHRAENKT_PERSON);
    echo "</font><br />";
  }
  if ( $Was == "Betroffen" )
  {
    $zeigen = "";
    $_REQUEST["Betroffen"] = $Wer;
    $wer = explode(",", $Wer);
    foreach ( $wer as $w => $n) 
      $zeigen .= "<em>".get_user_nickname($n)."</em> ";
    echo '<font size="+1">'.str_replace("%%WER%%", $zeigen, LANG_ANZEIGE_BESCHRAENKT_BETROFFEN);
    echo "</font><br />";
  }
  if ( $Was == "Gruppe" ) {
    $zeigen = "";
    $_REQUEST["Gruppe"] = $Wer;
    $wer = explode(",", $Wer);
    foreach ($wer as $key => $value)
      $zeigen .= "<em>".$groups[$value]."</em> ";
    echo '<font size="+1">'.str_replace("%%WER%%", $zeigen, LANG_ANZEIGE_BESCHRAENKT_GRUPPE);
    echo "</font><br />";
  }

//////////////////////////////////////////
//////////BEGIN CALENDAR CODE ////////////
//////////////////////////////////////////

// LOAD CONFIGURATION FILE
if (!isset($document['Art'])) $document['Art'] = "mtc=FFFFFF&al=2&cs=2&cp=2";
$config = explode ("&",$document["Art"]);
foreach ($config as $values) {
  $c0 = explode ("=",$values);
  if ( $c0[0] != "" && $c0[1] != "" ) {
    $$c0[0] = $c0[1];
  }
}

// DATE NUMBER ALIGNMENT
// Set $algn to 0 [defult] to align the dates to the middle and center of table cells.
// Set $algn to 1 to align the dates to the upper-right corner of table cells.
// 2 - Links oben
if (!$al) $al="0";

// FONT SIZES AND WEIGHT
// These options can be set in the script or overridden by command-line options of
// the same named variables. The sizes are in pixels.

if (!$fsm) $fsm="14"; // FONT SIZE MONTH
if (!$fsd) $fsd="10"; // FONT SIZE DAY NAMES
if (!$fsn) $fsn="10"; // FONT SIZE NUMBERS

if (!$fwm) $fwm="bold"; else $fwm="normal"; // FONT WEIGHT MONTH
if (!$fwd) $fwd="normal"; else $fwd="bold"; // FONT WEIGHT DAY NAMES
if (!$fwn) $fwn="normal"; else $fwn="bold"; // FONT WEIGHT NUMBERS

// ONLY EDIT THE STYLE SHEET BELOW IF YOU WANT TO CHANGE THE FONT FACE AND/OR COLOR

KalenderScriptEinfuegen();
?>
<style TYPE="text/css">
<!--
.monthyear { font-family: Verdana, Arial, Helvetica, sans-serif; font-size: <?echo$fsm?>px; font-weight: <?echo$fwm?>; color: #000000}
.daynames { font-family: Arial, Helvetica, sans-serif; font-size: <?echo$fsd?>px; font-weight: <?echo$fwd?>; color: #000000}
.dates { font-family: Arial, Helvetica, sans-serif; font-size: <?echo$fsn?>px; font-weight: <?echo$fwn?>; color: #000000}
-->
</style>

<?php

////////////////////////////////////////
///// DO NOT EDIT BELOW THIS POINT /////
////////////////////////////////////////

// DETERMINE AND SET DATE NUMBER CELL ALIGNMENT
if ($al==1) $al='align="right" valign="top"';
elseif ($al==2) $al='align="left" valign="top"';
elseif ($al!=1 && $al!=2) $al='align="center" valign="middle"';

// DETERMINE TODAYS DAY NUMBER
$ot = 0; // Offset-Zeit
$ot = $ot*3600;
$tmo = date("m", time()+$ot);
$tda = date("j", time()+$ot);
$tyr = date("Y", time()+$ot);
$TagNummer = (intval((date ("U", mktime(20,0,0,$tmo,$tda,$tyr))/86400))); // TODAY'S DAY NUMBER

if ( isset($_REQUEST["Datum"]) )
{
  $Datum = explode(".",$_REQUEST["Datum"]);
  $Monat = $Datum[1]; //$_REQUEST["Monat"];
  $Jahr = $Datum[2]; //_REQUEST["Jahr"];
  $Tag = $Datum[0]; //_REQUEST["Tag"];
}
if ( ! is_numeric($Tag) ) $Tag = date("d", time()+$ot);
// CHECK FOR COMMAND LINE DATE VARIABLES
if (! is_numeric($Monat) ) $Monat=$tmo;
if (! is_numeric($Jahr) ) $Jahr=$tyr;

$Monat=intval($Monat);
$mn = $mth[$Monat]; // SET MONTH NAME
if ($document["Jahranzeigen"]==1) {$mn = $mn." ".$Jahr;} // ADD YEAR TO MONTH NAME?

// Die Art des Kalenders festlegen
// M&ouml;glichkeiten:
// Anzeige -
//    Month  - ein Monat
//    Week   - eine Woche
//    erwWeek- 9 Tage
//    Day    - ein Tag
//    erwMonth-3 Monate

}

$PrevPage = "";
if ( isset($_REQUEST["Anzeige"])) $PrevPage .= "&Anzeige=".$Anzeige;
$NextPage = $PrevPage;
if ( $Anzeige == "Month" || $Anzeige == "erwMonth" )
{
  $Tag = 1;
  $ptime = mktime(0,0,0,$Monat-1, $Tag, $Jahr);
  $ntime = mktime(0,0,0,$Monat+1, $Tag, $Jahr);
}
elseif ( $Anzeige == "Week" )
{
  $ptime = mktime(0,0,0,$Monat, $Tag-7, $Jahr);
  $ntime = mktime(0,0,0,$Monat, $Tag+7, $Jahr);
  $AnzTage = 7;
}
elseif ( $Anzeige == "erwWeek" )
{
  $ptime = mktime(0,0,0,$Monat, $Tag-9, $Jahr);
  $ntime = mktime(0,0,0,$Monat, $Tag+9, $Jahr);
  $AnzTage = 9;
}
elseif ( $Anzeige == "Year" )
{
  $ptime = mktime(0,0,0,$Monat, $Tag, $Jahr-1);
  $ntime = mktime(0,0,0,$Monat, $Tag, $Jahr+1);
}
else // day
{
  $ptime = mktime(0,0,0,$Monat, $Tag-1, $Jahr);
  $ntime = mktime(0,0,0,$Monat, $Tag+1, $Jahr);
}
//$PrevPage .= "&Tag=".date("d",$ptime)."&Monat=".date("m",$ptime)."&Jahr=";
//$PrevPage .= date("Y",$ptime);
$PrevPage .= "&Datum=".date("d.m.Y",$ptime);
//$NextPage .= "&Tag=".date("d",$ntime)."&Monat=".date("m",$ntime)."&Jahr=";
//$NextPage .= date("Y",$ntime);
$NextPage .= "&Datum=".date("d.m.Y",$ntime);
?>
<table width="100%" border="0" >
<tr>
<td CLASS="monthyear" <?if($mtc) {echo ' BGCOLOR="#'.$mtc.'"';}?>>
<div ALIGN="center"><a href="<?=get_url($id,$PrevPage)?>">&laquo;</a></div>
</td><td valign="top">

<?php
$M = $Monat;
$Y = $Jahr;
if ( $Anzeige == "erwMonth" )
{
  for ( $Monatszaehler = -1; $Monatszaehler < 2; $Monatszaehler++ )
  {
    $zeit = strtotime("$Monatszaehler month",mktime(0,0,0,$M, 1, $Y));
    $Monat = date("n", $zeit);
    $Jahr = date("Y", $zeit);
    include(INC_PATH."misc/kalMonth.inc");
    if ( $Monatszaehler < 1 ) echo '</td><td valign="top">';
  }
  $Monat = $M;
  $Jahr = $Y;;
}
elseif ( $Anzeige == "erwWeek" )
  include(INC_PATH."misc/kalWeek.inc");
elseif ( $Anzeige == "Year" )
{
  echo "<table><tr>";
  for ( $Monatszaehler = 1; $Monatszaehler < 13; $Monatszaehler++ )
  {
    $zeit = mktime(0,0,0,$Monatszaehler, 1, $Y);
    $Monat = date("n", $zeit);
    $Jahr = date("Y", $zeit);
    echo '<td valign="top">';
    include(INC_PATH."misc/kalMonth.inc");
    echo "</td>";
    if ( $Monatszaehler % 3 == 0 ) echo '</tr><tr>';
  }
  $Monat = $M;
  echo "</tr></table>";
  $Jahr = $Y;
}
else
  include(INC_PATH."misc/kal$Anzeige.inc");

/*
$Monat = $_REQUEST["Monat"];
$Jahr = $_REQUEST["Jahr"];
$Tag = $_REQUEST["Tag"];
if ( ! is_numeric($Tag) ) $Tag = date("d", time()+$ot);
// CHECK FOR COMMAND LINE DATE VARIABLES
if (! is_numeric($Monat) ) $Monat=$tmo;
if (! is_numeric($Jahr) ) $Jahr=$tyr;
*/
$Param = "";
if ( isset($_REQUEST["Datum"]) )
  $Param = "Datum=".$_REQUEST["Datum"]."&";
$Param .= "Anzeige=";

?>

</td>
<td CLASS="monthyear" <?if($mtc) {echo ' BGCOLOR="#'.$mtc.'"';}?>>
<div ALIGN="center"><a href="<?=get_url($id,$NextPage)?>">&raquo;</a></div>
</td></tr></table>

<?php
if ( $docinput["design"] != "Printable" ) {
?>

<p><br /><strong><?=LANG_ANZEIGE?>:</strong> <a href="<?=get_url($id,$Param)?>Day"><?=LANG_ONEDAY?></a> |
<a href="<?=get_url($id,$Param)?>Week"><?=LANG_SEVENDAYS?></a> |
<a href="<?=get_url($id,$Param)?>erwWeek"><?=LANG_NINEDAYS?></a> |
<a href="<?=get_url($id,$Param)?>Month"><?=LANG_MONTH?></a> |
<a href="<?=get_url($id,$Param)?>erwMonth"><?=LANG_THREEMONTH?></a> |
<a href="<?=get_url($id,$Param)?>Year"><?=LANG_YEAR?></a> | <a
href="<?=get_url($id)?>&Anzeige=<?=$Anzeige?>">heute</a>
<br />
<?php
  $Param .= $Anzeige;
?>
<strong>Ausgewählte Termine anzeigen:</strong> <a href="<?=get_url($id,$Param)?>&Betroffen=<?=SESSION_DBID?>">
alle Termine</a>, <a href="<?=get_url($id,$Param)?>&Selbst=<?=SESSION_DBID?>">persönliche Termine</a>
(Mailbenachrichtigung <a href="<?=get_url($id, array('Config'=>1))?>">konfigurieren</a>)</p>
<p><form action="<?=get_url($id)?>" method="post" name="Springen2">zu Datum
<input type="hidden" name="Anzeige" value="<?=$Anzeige?>">
<input type="Text" name="Datum" size="10" maxlength="10" <?=KalenderEvents("Springen2", "Datum");?>>
<input type="Submit" value="springen"></form>
<form action="<?=get_url($id,$Param)?>" method="post">
<?php
  // Gruppe
  foreach ($groups as $gid => $g) {
    if ( InGruppe($gid)) //||uskommentiert 1.5.2011- $profile[$document["Terminrecht"]] == "1" )
    {
      echo '<input type="Checkbox" name="Gruppe[]" value="'.$gid.'" ';
      if ( in_array($gid, explode(",", $_REQUEST["Gruppe"]) )) echo 'checked="checked"';
      echo ' /> '.$g."&nbsp;&nbsp;";
    }
  }
?>
<input type="Submit" value="anzeigen" name="Gruppe[]"></form>
<?php
if ( $profile["SeeAll"] == "1" ) {
?>
<form action="<?=get_url($id,$Param)?>" method="post">Person
<select name="Betroffen">
<?php
  $u = search_users();
  foreach ( $u as $uid => $us ) {
    echo '<option value="'.$uid.'"';
    if ( $uid == SESSION_DBID ) echo ' selected="selected"';
    echo '>'.$us["nickname"].'</option>';
  }
?>
</select>
<input type="Submit" value="anzeigen"></form></p>
<br />

<?php
} // if seeAll
} // if not Printable
} // if not editor
if ( $profile['editor'] == "1" ) {
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Body','body','area_input');
  echo "<br /><br />";
  create_editor_input("Darstellungsart","Art", "text_input");
  echo "<br /><br />";
  echo "Art der Anzeige<br />";
  ?>
  <select name="docdata[Anzeige]" size="1">
  <option value="Month" <?if ($document['Anzeige'] == "Month") echo "selected";?>>Monat</option>
  <option value="Week" <?if ($document['Anzeige'] == "Week") echo "selected";?>>Woche</option>
  <option value="erwWeek" <?if ($document['Anzeige'] == "erwWeek") echo "selected";?>>9-Tage-Woche</option>
  <option value="Day" <?if ($document['Anzeige'] == "Day") echo "selected";?>>Tag</option>
  <option value="erwMonth" <?if ($document['Anzeige'] == "erwMonth") echo "selected";?>>3 Monate</option>
  </select>
  <?php
  echo "<br /><br />";
  create_editor_input("Alle Termine im System anzeigen","AlleAnzeigen", "onoff_input");
  echo "<br /><br />";
  create_editor_input("Jahr anzeigen","Jahranzeigen", "onoff_input");
  echo "<br /><br />";
  create_editor_input("Aktuellen Tag markieren","Tagzeigen", "onoff_input");
  echo "<br /><br />";
  create_editor_input("Erster Tag der Woche (0=Sonntag)","Erstertag", "text_input");
  echo "<br /><br />";
  create_editor_input("Zeitlicher Abstand in Minuten","Abstand", "text_input");
  echo "<br /><br />";
  create_editor_input("Farbe Hintergrund","Backgroundcolor", "text_input");
  echo "<br /><br />";
  create_editor_input("Farbe Hintergrund Wochenende","WEBackgroundcolor", "text_input");
  echo "<br /><br />";
  create_editor_input("Farbe Hintergrund markiert","Markedcolor", "text_input");
  echo "<br /><br />";
  create_editor_input("Farbe pers&ouml;nlicher Termin","Personcolor", "text_input");
  echo "<br /><br />";
  create_editor_input("Farbe Gruppentermin","Gruppencolor", "text_input");
  echo "<br /><br />";
  create_editor_input("Farbe Aktueller Tag","Todaycolor", "text_input");
  echo "<br /><br />";
  create_editor_input("Recht für Terminerstellung","Terminrecht", "text_input");
}
?>
