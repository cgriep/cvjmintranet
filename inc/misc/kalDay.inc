<table WIDTH="100%" BORDER="0" CELLSPACING="<?echo$cs?>" CELLPADDING="<?echo$cp?>">
<tr>
<td CLASS="monthyear" <?if($mtc) {echo ' BGCOLOR="#'.$mtc.'"';}?>>
<div ALIGN="center"><?
switch ( date("w",mktime(0,0,0,$Monat,$Tag,$Jahr)) )
{  case 0: echo "Sonntag"; break;
  case 1: echo "Montag"; break;
  case 2: echo "Dienstag"; break;
  case 3: echo "Mittwoch"; break;
  case 4:echo "Donnerstag"; break;
  case 5:echo "Freitag"; break;
  case 6: echo "Samstag"; break;
}
echo " ".$Tag.". $mn";?></div>
</td>
</tr>
</table>

<table align="center" WIDTH="100%" BORDER="0" CELLSPACING="<?echo$cs?>" CELLPADDING="<?echo$cp?>">

<tr ALIGN="center" HEIGHT="<?echo$fsd+$cs+$cp?>" CLASS="daynames"<?if($dtc)
  echo ' BGCOLOR="#'.$dtc.'"';
  else
  echo ' BGCOLOR="'.$design["table_header"].'"'; ?>>
<?

  // Termine suchen
  $search = "(name='Datum' AND ";
  $search .= "value BETWEEN '" . mktime(0,0,0,$Monat,$Tag,$Jahr)."'";
  $search .= " AND '" .mktime(23,59,59,$Monat,$Tag,$Jahr)."')";
  $search .= " OR (name='Enddatum' AND ";
  $search .= "value BETWEEN '" . mktime(0,0,0,$Monat,$Tag,$Jahr)."'";
  $search .= " AND '" .mktime(23,59,59,$Monat,$Tag,$Jahr)."')";
  unset($Termine);
  if ( $document['AlleAnzeigen'] == "1" )
    $Termine = search_nodes($search,1000);
  else
    $Termine = search_children ($search, 1000);
  if ( ! is_array($Termine))
  {
    $Termine = array();
  }
  $search = "name='Datum' AND ";
  $search .= "value < '" . mktime(0,0,0,$Monat,$Tag,$Jahr)."'";
  if ( $document['AlleAnzeigen'] == "1" )
    $fTermine = search_nodes($search,1000);
  else
    $fTermine = search_children ($search,1000);
  if ( ! is_array($fTermine))
  {
    $fTermine = array();
  }
  while ( list($key, $value) = each($fTermine) )
  {
    if ( $value["Enddatum"] > mktime(23,59,59,$Monat,$Tag,$Jahr) )
    {
      $Termine[$key] = $value;
    }
  }
  unset($fTermine);
  $search = "name='Enddatum' AND ";
  $search .= "value > '".mktime(23,59,59,$Monat,$Tag,$Jahr)."'";
  if ( $document['AlleAnzeigen'] == "1" )
    $fTermine = search_nodes($search,1000);
  else
    $fTermine = search_children ($search,1000);
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
  unset($fTermine);
  $search = "name='Turnus' AND ";
  $search .= "value <> ''";
  if ( $document['AlleAnzeigen'] == "1" )
    $fTermine = search_nodes($search,1000);
  else
    $fTermine = search_children ($search,1000);
  if ( ! is_array($fTermine))
  {
    $fTermine = array();
  }
  while (list($key, $value) = each($fTermine) )
  {
    if (($value['TurnusEnde'] == 0 ||
        $value['TurnusEnde'] >= mktime(0,0,0,$Monat, $Tag,$Jahr)) &&
        $value['Datum'] <= mktime(0,0,0,$Monat, $Tag, $Jahr) )
    switch ($value['Turnus']) {
      case 'd':
              $Termine[$key] = $value;
              $Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
                date('i',$value['Datum']), date('s',$value['Datum']),
                $Monat, $Tag, $Jahr);
              $Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                $Monat, $Tag, $Jahr);
              break;
        break;
      case 'w':
          if ( date('w', $value['Datum']) == date('w',mktime(0,0,0,$Monat,$Tag,$Jahr)) ) {
              $Termine[$key] = $value;
              $Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
                date('i',$value['Datum']), date('s',$value['Datum']),
                $Monat, $Tag, $Jahr);
              $Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                $Monat, $Tag, $Jahr);
          }
        break;
      case 'v':
            $unter = ((mktime(date('H',$value['Datum']), date('i',$value['Datum']),0,
                $Monat,$Tag,$Jahr)-$value['Datum'])/86400);
            if ( ($unter % 14) == 0 ) {
              if ( date('w', $value['Datum']) == date('w',mktime(0,0,0,$Monat,$Tag,$Jahr)) ) {
              $Termine[$key] = $value;
              $Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
                date('i',$value['Datum']), date('s',$value['Datum']),
                $Monat, $Tag, $Jahr);
              $Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                $Monat, $Tag, $Jahr);
            }
          }
        break;
      case 'm':
          if ( date('d',$value['Datum']) == $Tag ) {
            $Termine[$key] = $value;
            $Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
                  date('i',$value['Datum']), date('s',$value['Datum']),
                  $Monat, $Tag, $Jahr);
            $Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                  date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                  $Monat, date('d',$value['Enddatum']), $Jahr);
          }
        break;
      case 'j':
        if ( date('m',$value['Datum']) == $Monat && date('d',$value['Datum']) == $Tag) {
          $Termine[$key] = $value;
          $Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
                date('i',$value['Datum']), date('s',$value['Datum']),
                $Monat, $Tag, $Jahr);
          $Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
                date('i',$value['Enddatum']), date('s',$value['Enddatum']),
                $Monat, date('d',$value['Enddatum']), $Jahr);
        }
        break;
      default:
    }
  }
  unset($fTermine);
  if ( Count($Termine) == 0 )
  {
    unset($Termine);
  }
  else
    $Termine = sortiereTermine($Termine);
  echo '<th>Zeit</th><th>&nbsp;</th><th width="80%">Termin</th>';
  echo "</tr>";
  $zeit = strtotime("$Jahr-$Monat-$Tag 8:00");
  $diff = $document['Abstand'];
  if ( isset($_REQUEST['Diff']) && is_numeric($_REQUEST['Diff']) )
    $diff = $_REQUEST['Abstand'];
  if ( ! is_numeric($diff)) $diff = 30;
  $switch = true;
  while ( $zeit < strtotime("$Jahr-$Monat-$Tag 23:59") ) {
    $nextzeit = strtotime("+$diff minutes", $zeit);
    if ( $switch )
      echo '<tr bgcolor="'.$design["table_row_dark"].'">';
    else
      echo '<tr bgcolor="'.$design["table_row_bright"].'">'; // Backgroundcolor
    $switch = ! $switch;
    echo '<td align="center">';
    echo date("H:i",$zeit) . '</td><td align="center">';
    //if ( $profile[$document["Terminrecht"]]=="1") {
      echo '<a href="'.get_url($id, array('Zeit' => $zeit, "Termin" => 1));
      echo '"><img src="img/newtermin.gif" border="0"></a>';
    //}
    echo '</td>';
    echo "<td>";
    reset($Termine);
    $Anz = 0;
    while ( list($key, $value) = each($Termine) ) {
      if ( ($value['Datum'] >= $zeit && $value['Datum'] < $nextzeit) ||
        ( $value['Datum'] <= $zeit && $value['Enddatum'] >= $zeit )
      ) {
        if ( !isset($value["Ok"])) {
        if ( $Anz != 0 ) { echo "<br />";  }
        $Anz++;
        echo '<span ';
        if (InGruppe($value["Gruppe"]))
            echo 'style="background-color:'.$document["Gruppencolor"].'"';
        else
          if (InPerson($value["Person"]) )
            echo 'style="background-color:'.$document["Personcolor"].'"';
        echo '>';
        echo '<a href="';
        echo get_url($value["id"]).'" title="'.get_user_nickname($value["author_id"]).": ";
        if ( is_numeric($value["Enddatum"]) )
          echo 'bis '.date("d.m.Y H:i",$value["Enddatum"]);
        if ( strtotime("$Jahr-$Monat-$Tag 23:59") < $value["Enddatum"] )
        {
          echo " (ganztägig!)";
          $Termine[$key]["Ok"] = true;
        }
        echo '">'.transform($value['title'],'clean')."</a>";
        echo "</span>";
        }
      }
    }
    echo "&nbsp;</td>";
    $zeit = $nextzeit;
    echo "</tr>";
    $anz++;
  }

echo '</table>';
echo "<strong>".count($Termine)." Termine</strong><br />";
?>