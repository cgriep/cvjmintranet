<h2>{$title}</h2>

1. Eingabe 

2. Anzeige 
{include file=''}

<form action="?id={$id}" method="post" name="Springen">zu Datum
<input type="hidden" name="Anzeige" value="{$Anzeige}">
<input type="Text" name="Datum" size="10" maxlength="10" <?=KalenderEvents("Springen","Datum");?>>
<input type="Submit" value="springen"></form>
<p><strong>Anzeige:</strong> <a href="?id={$id}&Anzeige=Day">ein Tag</a> |
<a href="<?=get_url($id,$Param)?>Week"><?=LANG_SEVENDAYS?></a> |
<a href="<?=get_url($id,$Param)?>erwWeek"><?=LANG_NINEDAYS?></a> |
<a href="<?=get_url($id,$Param)?>Month"><?=LANG_MONTH?></a> |
<a href="<?=get_url($id,$Param)?>erwMonth"><?=LANG_THREEMONTH?></a> |
<a href="<?=get_url($id,$Param)?>Year"><?=LANG_YEAR?></a> | <a
href="<?=get_url($id)?>&Anzeige=<?=$Anzeige?>">heute</a>
<br />
<strong>Ausgew�hlte Termine anzeigen:</strong> <a href="<?=get_url($id,$Param)?>&Betroffen=<?=SESSION_DBID?>">
alle Termine</a>, <a href="<?=get_url($id,$Param)?>&Selbst=<?=SESSION_DBID?>">pers�nliche Termine</a>
(Mailbenachrichtigung <a href="<?=get_url($id, array('Config'=>1))?>">konfigurieren</a>)</p>



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
<strong>Ausgew�hlte Termine anzeigen:</strong> <a href="<?=get_url($id,$Param)?>&Betroffen=<?=SESSION_DBID?>">
alle Termine</a>, <a href="<?=get_url($id,$Param)?>&Selbst=<?=SESSION_DBID?>">pers�nliche Termine</a>
(Mailbenachrichtigung <a href="<?=get_url($id, array('Config'=>1))?>">konfigurieren</a>)</p>
<p><form action="<?=get_url($id)?>" method="post" name="Springen2">zu Datum
<input type="hidden" name="Anzeige" value="<?=$Anzeige?>">
<input type="Text" name="Datum" size="10" maxlength="10" <?=KalenderEvents("Springen2", "Datum");?>>
<input type="Submit" value="springen"></form>
<form action="<?=get_url($id,$Param)?>" method="post">
<?php
  // Gruppe
  reset($groups);
  foreach ($groups as $gid => $g)  {
    if ( InGruppe($gid) || $profile[$document["Terminrecht"]] == "1" )
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
  foreach ($u as $uid => $us)  {
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
  create_editor_input("Recht f�r Terminerstellung","Terminrecht", "text_input");
}
?>
