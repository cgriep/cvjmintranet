<?php
  $orgTag = $Tag;
  // NUMBER OF DAYS IN MONTH
  $TagAnzahl = mktime (0,0,0,$Monat+1,0,$Jahr);
  $TagAnzahl = (strftime ('%d',$TagAnzahl))+1;

  // ON WHAT DAY DOES THE FIRST FALL
  $sd = date ('w', mktime(0,0,0,$Monat,1-$document['Erstertag'],$Jahr));
  $Tag = 1-$sd;
  $mn = $mth[$Monat]; // SET MONTH NAME
  if ($document['Jahranzeigen']==1) {$mn = $mn.' '.$Jahr;} // ADD YEAR TO MONTH NAME?

  // ADJUST TABLE HEIGHT FOR 5 ROW MONTHS
  $checksize=$fsd+$cs+$cp;
  $checkrows=$document['Erstertag']+$TagAnzahl-$Tag;
  if ($checkrows<36) $th=$th-intval(($th-$checksize)/6);

  // CALENDAR WIDTH AND HEIGHT
  // Set $tw to the width of the calendar table
  // Set $th to the height of the calendar table (does not include the month table which is additional)
  $tw='165'; // Table Width
  $th='120'; // Table Height (with 6 rows of dates)

// CALENDAR CELL SPACING AND PADDING
// Set $cs to the number of pixels of cell spacing (space between each cell)
// Set $cp to the number of pixels of cell padding (space around names and numbers inside cells)
  $cs=1;
  $cp=0;
  // Einzelner Monat
  if ( $Anzeige == 'Month' ) {
    $tw *= 3;
    $th *= 3;
  }

?>
<table align="center" WIDTH="<?echo$tw?>" BORDER="0" CELLSPACING="<?echo$cs?>" CELLPADDING="<?echo$cp?>">
<tr>
<td CLASS="monthyear" <?if($mtc) {echo '="#'.$mtc.'"';}?>>
<div ALIGN="center"><?echo "$mn";?></div>
</td>
</tr>
</table>

<table align="center" WIDTH="<?echo$tw?>" HEIGHT="<?echo$th?>" BORDER="0" CELLSPACING="<?echo$cs?>" CELLPADDING="<?echo$cp?>">

<tr ALIGN="center" HEIGHT="<?echo$fsd+$cs+$cp?>" CLASS="daynames"<?if($dtc)
  echo ' BGCOLOR="#'.$dtc.'"';
  else
  echo ' BGCOLOR="'.$design["table_header"].'"'; ?>>
<?
  $daycount = (intval((date ("U", mktime(20,0,0,$Monat,1,$Jahr))/86400)))-$document['Erstertag']; // FIRST OF MONTH DAY NUMBER
  $daycount=$daycount+$document['Erstertag']; // ADJUST FOR DAY START VARIABLE

  for ($I=0;$I<7;$I++) {
    $dayprint=$document['Erstertag']+$I;
    if ($dayprint>6) $dayprint=$dayprint-7;
    echo' <td WIDTH="'.($tw/7).'">'.$day[$dayprint]."</td>";
  }
  // Termine suchen
  $search = "(name='Datum' AND ";
  $search .= "value BETWEEN '" . mktime(0,0,0,$Monat,1,$Jahr)."'";
  $search .= " AND '" .mktime(23,59,59,$Monat,$TagAnzahl,$Jahr)."')";
  $search .= " OR (name='Enddatum' AND ";
  $search .= "value BETWEEN '" . mktime(0,0,0,$Monat,1,$Jahr)."'";
  $search .= " AND '" .mktime(23,59,59,$Monat,$TagAnzahl,$Jahr)."')";
  unset($Termine);
  if ( $document['AlleAnzeigen'] == '1' )
    $Termine = search_nodes($search,1000);
  else
    $Termine = search_children ($search,1000);
  $search = "name='Datum' AND ";
  $search .= "value < '" . mktime(0,0,0,$Monat,1,$Jahr)."'";
  if ( $document['AlleAnzeigen'] == '1' )
    $fTermine = search_nodes($search,1000);
  else
    $fTermine = search_children ($search,1000);
  while ( list($key, $value) = each($fTermine) )
  {
    if ( $value['Enddatum'] > mktime(23,59,59,$Monat,$TagAnzahl,$Jahr) )
      $Termine[$key] = $value;
  }
  unset($fTermine);
  $search = "name='Enddatum' AND ";
  $search .= "value > '" .mktime(23,59,59,$Monat,$TagAnzahl,$Jahr)."'";
  if ( $document['AlleAnzeigen'] == '1' )
    $fTermine = search_nodes($search,1000);
  else
    $fTermine = search_children ($search,1000);
  foreach ($fTermine as $key => $value )
  {
    if ( $value['Datum'] < mktime(0,0,0,$Monat,1,$Jahr) )
      $Termine[$key] = $value;
  }
  unset($fTermine);

  $search = "name='Turnus' AND ";
  $search .= "value <> ''";
  if ( $document['AlleAnzeigen'] == '1' )
    $fTermine = search_nodes($search,1000);
  else
    $fTermine = search_children ($search,1000);
  foreach ( $fTermine as $key => $value )
  {
    switch ($value['Turnus']) {
      case 'd':
          $ewota = 1-date ('w', mktime(0,0,0,$Monat,1-$wota,$Jahr));
          while ( $ewota < $TagAnzahl ) {
            if ( $ewota > 0 && ($value['TurnusEnde'] == 0 || mktime(0,0,0,$Monat, $ewota, $Jahr) <= $value['TurnusEnde']) &&
              $value['Datum'] <= mktime(0,0,0,$Monat, $ewota, $Jahr) ) {
              $Termine[$key.'&'.$ewota] = $value;
              $Termine[$key.'&'.$ewota]['Datum'] = mktime(date('H',$value['Datum']),
                date('i',$value['Datum']), date('s',$value['Datum']),
                $Monat, $ewota, $Jahr);
              $Termine[$key.'&'.$ewota]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                $Monat, $ewota, $Jahr);
              $Termine[$key.'&'.$ewota] = $value['author_id'];
            }
            $ewota += 1;
          }
        break;
      case 'w':
          $wota = date('w', $value['Datum']);
          $ewota = 1-date ('w', mktime(0,0,0,$Monat,1-$wota,$Jahr));
          while ( $ewota < $TagAnzahl ) {
            if ( $ewota > 0 && ($value['TurnusEnde'] == 0 || 
                 mktime(0,0,0,$Monat, $ewota, $Jahr) <= $value['TurnusEnde']) &&
                 $value['Datum'] <= mktime(0,0,0,$Monat, $ewota, $Jahr) ) {
              $Termine[$key.'&'.$ewota] = $value;
              $Termine[$key.'&'.$ewota]['id'] = $key;
              $Termine[$key.'&'.$ewota]['Datum'] = mktime(date('H',$value['Datum']),
                date('i',$value['Datum']), date('s',$value['Datum']),
                $Monat, $ewota, $Jahr);
              $Termine[$key.'&'.$ewota]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                $Monat, $ewota, $Jahr);
            }
            $ewota += 7;
          }
        break;
      case 'v':
          $wota = date('w', $value['Datum']);
          $ewota = 1-date ('w', mktime(0,0,0,$Monat,1-$wota,$Jahr));
          while ( $ewota < $TagAnzahl ) {
            if ( $ewota > 0 && ($value['TurnusEnde'] == 0 ||
              mktime(0,0,0,$Monat, $ewota, $Jahr) <= $value['TurnusEnde']) &&
              $value['Datum'] <= mktime(0,0,0,$Monat, $ewota, $Jahr) ) {
              $unter = ((mktime(date('H',$value['Datum']), date('i',$value['Datum']),0,
                $Monat,$ewota,$Jahr)-$value['Datum'])/86400);
              if ( ($unter %14) == 0 ) {
                $Termine[$key.'&'.$ewota] = $value;
                $Termine[$key.'&'.$ewota]['Datum'] = mktime(date('H',$value['Datum']),
                  date('i',$value['Datum']), date('s',$value['Datum']),
                  $Monat, $ewota, $Jahr);
                $Termine[$key.'&'.$ewota]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                  date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                  $Monat, $ewota, $Jahr);
              }
            }
            $ewota += 7;
          }
        break;
      case 'm':
          if ( $value['Datum'] <= mktime(0,0,0,$Monat, date('d', $value['Datum']), $Jahr) &&
          ($value['TurnusEnde'] == 0 || $value['TurnusEnde'] >= 
          mktime(0,0,0,$Monat, date('d', $value['Datum']), $Jahr))) {
            $Termine[$key] = $value;
            $Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
                  date('i',$value['Datum']), date('s',$value['Datum']),
                  $Monat, date('d',$value['Datum']), $Jahr);
            $Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                  date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                  $Monat, date('d',$value['Enddatum']), $Jahr);
          }
        break;
      case 'j':
        if ( date('m',$value['Datum']) == $Monat &&
          $value['Datum'] <= mktime(0,0,0,$Monat, date('d', $value['Datum']), $Jahr) &&
          ($value['TurnusEnde'] == 0 ||
          $value['TurnusEnde'] >= mktime(0,0,0,$Monat, date('d',$value['Datum']), $Jahr))) {
          $Termine[$key] = $value;
          $Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
                date('i',$value['Datum']), date('s',$value['Datum']),
                $Monat, date('d',$value['Datum']), $Jahr);
          $Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                $Monat, date('d',$value['Enddatum']), $Jahr);
        }
        break;
      default:
    }
  }
  unset($fTermine);
  if ( ! $Termine )
    unset($Termine);
  else
    $Termine = sortiereTermine($Termine);

?>
</tr>

<?
    for ($i = 1; $i<7; $i++) {
      if ($Tag>=$TagAnzahl) break;
?>
<tr <?echo$al?> height="<?=($th/5)?>" CLASS="dates">
<?
      for ($prow = 1; $prow<8; $prow++) {
           reset($Termine);
           $Marked = false;
           $Wochenende = date('w', mktime(23,59,59,$Monat,$Tag,$Jahr));
           $TerminString = '';
           $Farbstring = '';
           while ( list($key, $value) = each($Termine) )
             if ((date('Y-m-d',$value['Datum']) <= date('Y-m-d',mktime(23,59,59,$Monat,$Tag,$Jahr))
            && date('Y-m-d',$value['Enddatum']) >= date('Y-m-d',mktime(0,0,0,$Monat,$Tag,$Jahr)))
            ) {
               $Marked = true;
               if (InPerson($value['Person']) ) {
                 $Farbstring .= '<span style="background-color:'.$document['Personcolor'].'">&nbsp;</span>';
                 $TerminString .= '<span style="background-color:'.$document['Personcolor'].
                   '" title="'.get_user_nickname($value['author_id']).': ';
                 if ( is_numeric($value['Enddatum']) )
    		  $TerminString .= 'bis '.date('d.m.Y H:i',$value['Enddatum']);
                 $TerminString .= '">'.$value['title'].'</span><br />';
               }
               else
               if (InGruppe($value['Gruppe'])) {
                 $Farbstring .= '<span style="background-color:'.$document['Gruppencolor'].'">&nbsp;</span>';
                 $TerminString .= '<span style="background-color:'.$document['Gruppencolor'].'" '.
                   ' title="'.get_user_nickname($value['author_id']).': ';
                 if ( is_numeric($value['Enddatum']) )
    		  $TerminString .= 'bis '.date('d.m.Y H:i',$value['Enddatum']);
                 $TerminString .= '">'.$value['title'].'</span><br />';
                   $value['title'].'</span><br />';
               }
               else
                 $TerminString .= $value['title'].'<br />';
             }
             if ( $TerminString != '' ) $TerminString = '<small>'.$TerminString.'</small>';
        if ($daycount==$TagNummer && $document['Tagzeigen']=='1' && $Tag>0 && $Tag<$TagAnzahl) {
          echo ' <td bgcolor="#';
           if ( ! $Marked )
             echo $document['Todaycolor'];
           else
             echo $document['Markedcolor'];
          echo '">';
          echo '<a href="';
          $a = array('Anzeige'=>'Day','Datum'=>$Tag.'.'.$Monat.'.'.$Jahr);
          if ( $Anzeige == 'Month' ) $a = array_merge($a, array('Zeit'=>strtotime("$Jahr-$Monat-$Tag 8:00"),
            'Termin'=>1));
          echo get_url($id, $a);
          echo '">';
          echo $Tag.'</a>';
          // Neuer Termin
          //echo '<br /><a href="?id='.$id.'&Anzeige='.$Anzeige.'&Zeit='.strtotime("$Jahr-$Monat-$Tag 8:00");
          //echo '&Termin=1"><img src="img/newtermin.gif" border="0"></a>';
          //echo '</font>';
          if ( $Anzeige == 'Month' )
            echo '<br />'.$TerminString;
          else
            echo $Farbstring;
          echo '</td>';
          $daycount++;$Tag++;
        }
        else { ?>
<td<?   if ($Tag>0 && $Tag<$TagAnzahl) {
           echo ' bgcolor="#';
           if ( ! $Marked )
             if ( $Wochenende == 0 || $Wochenende == 6)
               echo $document['WEBackgroundcolor'];
               else echo $document['Backgroundcolor'];
           else
             echo $document['Markedcolor'];
          echo '">';
          $a = array('Anzeige'=>'Day','Datum'=>$Tag.'.'.$Monat.'.'.$Jahr);
          if ( $Anzeige == 'Month' ) $a = array_merge($a,
            array('Zeit'=>strtotime("$Jahr-$Monat-$Tag 8:00"),'Termin'=>1));
          echo '<a href="'.get_url($id, $a);
          echo '">';
          echo $Tag; // Termine auflisten
          echo '</a>';
//          echo '<br /><a href="?id='.$id.'&Anzeige='.$Anzeige.'&Zeit='.strtotime("$Jahr-$Monat-$Tag 8:00");
//          echo '&Termin=1"><img src="img/newtermin.gif" border="0"></a>';
          if ( $Anzeige == 'Month' )
            echo '<br />'.$TerminString;
          else
            echo $Farbstring;
          $daycount++;
        }
        else
          echo '>';
        $Tag++;?></td>
<?    }
    }
     ?>
</tr>
<?
  }
  $Tag = $orgTag;
?>
</table>

<?php if (count($Termine) > 0 ) { ?>
<table align="center" WIDTH="<?echo$tw?>" BORDER="0" CELLSPACING="<?echo$cs?>" CELLPADDING="<?echo$cp?>">
<tr><th colspan="2">&nbsp;</th></tr>
<?php
reset ($Termine);
while ( list($key, $value) = each($Termine) ) {
  echo '<tr';
  if (InGruppe($value['Gruppe']))
      echo ' bgcolor="#'.$document['Gruppencolor'].'"';
  else
    if (InPerson($value['Person']) ) {
      echo ' bgcolor="#'.$document['Personcolor'].'"';
    }
  echo '><td><small>';
  if ( date('H:i',$value['Datum']) == '00:00' )
    echo date('d.m.',$value['Datum']);
  else
    echo date('d.m. H:i',$value['Datum']);
  echo '</small></td><td><small><a href="';
  echo get_url($value["id"]).'" title="';
  echo get_user_nickname($value['author_id']).': ';
  if ( is_numeric($value['Enddatum']) )
    echo 'bis '.date('d.m.Y H:i',$value['Enddatum']);
  else
    echo '';
  echo '">'.transform($value['title'],'clean')."</a>";
  echo '</small></td></tr>';
}
echo '</table>';
} // if
?>