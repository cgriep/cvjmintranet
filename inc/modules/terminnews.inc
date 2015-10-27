<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

load_lang("Kalender");
include_once(INC_PATH."misc/Termin.inc");
global $design;

global $flavour;

$news_id = get_cmodule_id('news');

global $doctypes;
global $document;

$children = get_sorted_children (0, 2, 'timestamp', $doctypes['newsitem'], $news_id);

?><span style="font-size: 10px"><?

foreach ($children as $docid => $value) {
	if(strlen($value['title']) > 47) { $value['title'] = substr($value['title'], 0, 44).'...'; }
	?>
	<img src="<?=$design['arrow_bullet']?>" border="0" width="10" height="7" />
	<?=transform($value['timestamp'], 'date')?>:
	<a href="<?=get_url($docid)?>">
	<?=transform($value['title'], $value['datatype']['title'])?></a>
	<br />
	<?
}
	?>
	<a href="<?=get_url($news_id)?>"><?=LANG_MORE_NEWS?>...</a>
	<hr />
	
	<?
             
// Termine suchen
  $Monat = date("n");
  $Tag = date("d");
  $Jahr = date("Y");
  $search = "(name='Datum' AND ";
  $search .= "value >= '" . mktime(0,0,0,$Monat,$Tag,$Jahr)."')";
  $search .= " OR (name='Enddatum' AND ";
  $search .= "value >= '" . mktime(0,0,0,$Monat,$Tag,$Jahr)."')";
  unset($Termine);
  $Termine = search_nodes($search);
  if ( ! is_array($Termine))
  {
    $Termine = array();
  }
  /*
  $search = "name='Datum' AND ";
  $search .= "value < '" . mktime(0,0,0,$Monat,$Tag,$Jahr)."'";
  $fTermine = search_nodes($search);
  if ( ! is_array($fTermine))
  {
    $fTermine = array();
  }
  foreach ( $fTermine as $key => $value) 
  {
    if ( $value["Enddatum"] > mktime(23,59,59,$Monat,$Tag,$Jahr) )
    {
      $Termine[$key] = $value;
    }
  }
  unset($fTermine);

  $search = "name='Enddatum' AND ";
  $search .= "value > '".mktime(23,59,59,$Monat,$Tag,$Jahr)."'";
  $fTermine = search_nodes($search);
  if ( ! is_array($fTermine))
  {
    $fTermine = array();
  }
  while ( list($key, $value) = each($fTermine) )
  {
    if ( $value["Datum"] < mktime(0,0,0,$Monat,$Tag,$Jahr) )
    {
      $Termine[$key] = $value;
    }
  }
*/
  unset($fTermine);
  $search = "name='Turnus' AND ";
  $search .= "value <> ''";
  $fTermine = search_nodes($search);
  if ( ! is_array($fTermine))
  {
    $fTermine = array();
  }
  foreach ( $fTermine as $key => $value) 
  {
    if (($value["TurnusEnde"] == 0 ||
        $value["TurnusEnde"] >= mktime(0,0,0,$Momat, $Tag,$Jahr)) &&
        $value["Datum"] <= mktime(0,0,0,$Monat, $Tag, $Jahr) )
    switch ($value["Turnus"]) {
      case "d":
              $Termine[$key] = $value;
              $Termine[$key]["Datum"] = mktime(date("H",$value["Datum"]),
                date("i",$value["Datum"]), date("s",$value["Datum"]),
                $Monat, $Tag, $Jahr);
              $Termine[$key]["Enddatum"] = mktime(date("H",$value["Enddatum"]),
                date("i",$value["Enddatum"]), date("s",$value["Enddatum"]),
                $Monat, $Tag, $Jahr);
              break;
        break;
      case "w":
          if ( date("w", $value["Datum"]) == date("w",mktime(0,0,0,$Monat,$Tag,$Jahr)) ) {
              $Termine[$key] = $value;
              $Termine[$key]["Datum"] = mktime(date("H",$value["Datum"]),
                date("i",$value["Datum"]), date("s",$value["Datum"]),
                $Monat, $Tag, $Jahr);
              $Termine[$key]["Enddatum"] = mktime(date("H",$value["Enddatum"]),
                date("i",$value["Enddatum"]), date("s",$value["Enddatum"]),
                $Monat, $Tag, $Jahr);
          }
        break;
      case "v":
            $unter = ((mktime(date("H",$value["Datum"]), date("i",$value["Datum"]),0,
                $Monat,$Tag,$Jahr)-$value["Datum"])/86400);
            if ( ($unter % 14) == 0 ) {
              if ( date("w", $value["Datum"]) == date("w",mktime(0,0,0,$Monat,$Tag,$Jahr)) ) {
              $Termine[$key] = $value;
              $Termine[$key]["Datum"] = mktime(date("H",$value["Datum"]),
                date("i",$value["Datum"]), date("s",$value["Datum"]),
                $Monat, $Tag, $Jahr);
              $Termine[$key]["Enddatum"] = mktime(date("H",$value["Enddatum"]),
                date("i",$value["Enddatum"]), date("s",$value["Enddatum"]),
                $Monat, $Tag, $Jahr);
            }
          }
        break;
      case "m":
          if ( date("d",$value["Datum"]) == $Tag ) {
            $Termine[$key] = $value;
            $Termine[$key]["Datum"] = mktime(date("H",$value["Datum"]),
                  date("i",$value["Datum"]), date("s",$value["Datum"]),
                  $Monat, $Tag, $Jahr);
            $Termine[$key]["Enddatum"] = mktime(date("H",$value["Enddatum"]),
                  date("i",$value["Enddatum"]), date("s",$value["Enddatum"]),
                  $Monat, date("d",$value["Enddatum"]), $Jahr);
          }
        break;
      case "j":
        if ( date("m",$value["Datum"]) == $Monat && date("d",$value["Datum"]) == $Tag) {
          $Termine[$key] = $value;
          $Termine[$key]["Datum"] = mktime(date("H",$value["Datum"]),
                date("i",$value["Datum"]), date("s",$value["Datum"]),
                $Monat, $Tag, $Jahr);
          $Termine[$key]["Enddatum"] = mktime(date("H",$value["Enddatum"]),
                date("i",$value["Enddatum"]), date("s",$value["Enddatum"]),
                $Monat, date("d",$value["Enddatum"]), $Jahr);
        }
        break;
      default:
    }
  }
  unset($fTermine);
  $document['AlleAnzeigen'] = 1;
  if ( Count($Termine) == 0 )
  {
    unset($Termine);
  }
  else
  {
    // wir wollen nur die Zeit der Termine haben!
    //foreach ( $Termine as $key => $value )
    //  $Termine[$key]['Datum'] = strtotime(date('H:i', $value['Datum']));
    $Termine = sortiereTermine($Termine);
  }

  $switch = true;
  $Anz = 0;
  foreach ( $Termine as $key => $value)  {
    if ( $switch )
      echo '<div bgcolor="'.$design['table_row_dark'].'">';
    else
      echo '<div bgcolor="'.$design['table_row_bright'].'">'; // Backgroundcolor
    $switch = ! $switch;
    echo date('d.m. H:i',$value['Datum']).' ';
    echo '<a href="';
    echo get_url($value['id']).'" title="'.get_user_nickname($value['author_id']).': ';
    if ( is_numeric($value['Enddatum']) )
       echo 'bis '.date('d.m.Y H:i',$value['Enddatum']);
    if ( strtotime("$Jahr-$Monat-$Tag 23:59") < $value['Enddatum'] )
    {
      echo ' (ganztägig!)';
    }
    switch ( $value['Turnus'] )
    {
      case 'd': echo ' täglich';
        break;
      case 'w': echo ' wöchentlich';
        break;
      case 'm': echo ' monatlich';
        break;
      case 'j': echo ' jährlich';
    }
    echo '">'.transform($value['title'],'clean')."</a>";
    echo '</div>';
    $Anz++;
    if ( $Anz > 2 ) break;
  }
echo '</span>';
//echo '<div align="center"><strong>'.count($Termine)." Termine</strong></div>";        
        
?>