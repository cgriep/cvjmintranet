<?php

// DAY NAMES
// Edit the calendar day name column headers below
$day[0]='S';
$day[1]='M';
$day[2]='D';
$day[3]='M';
$day[4]='D';
$day[5]='F';
$day[6]='S';

// MONTH NAMES
// Edit the calendar month names below
$mth[1]=LANG_JANUARY;
$mth[2]=LANG_FEBRUARY;
$mth[3]=LANG_MARCH;
$mth[4]=LANG_APRIL;
$mth[5]=LANG_MAY;
$mth[6]=LANG_JUNE;
$mth[7]=LANG_JULY;
$mth[8]=LANG_AUGUST;
$mth[9]=LANG_SEPTEMBER;
$mth[10]=LANG_OCTOBER;
$mth[11]=LANG_NOVEMBER;
$mth[12]=LANG_DECEMBER;

/**
 * sortiert die Termine und entfernt jene, von denen man weder betroffen noch der Autor ist.
 * @param array $Termine die zu sortierenden Termine
 * @return array die verbliebenen Termin nach Datum sortiert
 */
function sortiereTermine($Termine)
{
  $sortarray = array();
  global $id;
  global $document;
  foreach ($Termine as $key => $value)
  {
    $okey = $key;
    if ( strpos($key, '&') !== false ) $okey = substr($key, 0, strpos($key, '&'));
    if ( $value['parent_id'] == $id || $document['AlleAnzeigen'] == '1') {
      // prüfen, ob Betroffen
      if ( isset($_REQUEST['Gruppe']) )
      {
        // nur Gruppe anzeigen
        $da = false;
        if ( $_REQUEST['Gruppe'] != '' )
        {
          $Gruppen = explode(',',$_REQUEST['Gruppe']);
          foreach ( $Gruppen as $nr => $gruppe)
          {
            if ( in_array($gruppe, explode(',',$value['Gruppe'])) ||
                 in_array(-1, explode(',',$value['Gruppe'])) )
            {
              // Gruppe vorhanden: Termin anzeigen
              $sortarray[$key] = $value['Datum'];
              $Termine[$key]['id'] = $okey;
              $da = true;
            }
          }
        }
        if ( ! $da )
        {
          if ( InPerson($value['Person'], $value['author_id'] )) {
            $sortarray[] = $value['Datum'];
            $Termine[$key]['id'] = $okey;
          }
          else
            unset($Termine[$key]);
        }
      }
      else if ( isset($_REQUEST['Betroffen']) )
      {
        // nur von Einzelbetroffenen anzeigen
        $Gruppen = explode(',',$_REQUEST['Betroffen']);
        foreach ( $Gruppen as $nr => $gruppe)
        {
          // Person ist klar, nun noch die Gruppen, in denen die Person ist
          $prof = get_profile($gruppe, false);
          if ( in_array(-1, explode(',',$value['Person'])) ||
               in_array($gruppe, explode(',',$value['Person'])) ||
               InGruppe($value['Gruppe'], $prof))
          {
            // Person betroffen: Termin anzeigen
            $sortarray[$key] = $value['Datum'];
            $Termine[$key]['id'] = $okey;
          }
          else
            unset($Termine[$key]);
        }
      }
      else if ( isset($_REQUEST['Selbst']) )
      {
        // nur von Einzelbetroffenen anzeigen
        $Gruppen = explode(',',$_REQUEST['Selbst']);
        foreach ( $Gruppen as $nr => $gruppe)
        {
          if ( $gruppe == -1 || in_array($gruppe, explode(',',$value['Person'])) )
          {
            // Person betroffen: Termin anzeigen
            $sortarray[$key] = $value['Datum'];
            $Termine[$key]['id'] = $okey;
          }
          else
            unset($Termine[$key]);
        }
      }
      elseif ( InGruppe($value['Gruppe']) || InPerson($value['Person'], $value['author_id'] )) {
        $sortarray[] = $value['Datum'];
        $Termine[$key]['id'] = $key;
      }
      else
        unset($Termine[$key]);
    }
    else
      unset($Termine[$key]);
  }
  array_multisort($sortarray, SORT_ASC, SORT_NUMERIC, $Termine);
  //array_multisort($Termine, $Termine['Datum'], SORT_ASC, SORT_NUMERIC);
  return $Termine;
}

// prüft, ob der Ansehende Mitglied der Gruppe ist
function InGruppe($Gruppe, $prof = -1, $save = true)
{
  if ( $prof == -1 ) {
    global $profile;
    $prof = $profile;
  }
  $Gruppen = explode(',',$Gruppe);
  foreach ($Gruppen as $key => $value) 
  {
    if ( $prof['group_'.$value] == 1 || $value == -1 )
      return true && $save;
  }
  return false;
}
// prüft, ob der Ansehende Autor oder Betroffene Person ist
function InPerson($Person, $author='', $save = true)
{
  if ( SESSION_STATUS != 'ok' ) return false;
  if ( SESSION_DBID == $author ) return true;
  return in_array(SESSION_DBID, explode(',',$Person)) && $save;
}

function sendErinnerung($Wem, $document, $Besonderes = '') {
  global $id;
  if ( $Besonderes != '' ) $Besonderes .= "\n\n";
  if ( strpos($Besonderes,'gelöscht') !== false )
  {
    $Message = 'Termin '.transform($document['title'],'clean').
      ' am '.transform($document['Datum'],'datetime')."\n";
    $Message .= "\n".$document['body'];
  }
  else
  {
    $Message = 'Sie sind von '.get_user_nickname($document['author_id']).
      ' zum Termin '.transform($document['title'],'clean').
      ' am '.transform($document['Datum'],'datetime');
    if ($document['Ort'] != '' )
      if ( function_exists('OrtsName') && is_numeric($document['Ort']))
        $Message .= ' ('.OrtsName($document['Ort'], false).') ';
      else
        $Message .= ' ('.transform($document['Ort'], 'clean').') ';
    $Message .= " eingeladen worden.\n".$document['body']."\n\nDirekter Link: ".get_url($id,'',true);
  }
  send_message ($Wem, $Besonderes . $Message, $document['author_id'], '',
    '['.SITE_TITLE.'] Termin '.transform($document['title'],'clean').' am '.
     transform($document['Datum'],'datetime'));
}
function sendeAllen($document, $Besonderes = '') {
  $gesendet = false;
  if ( $document['Person'] != '' ) {
    $wer = explode(',',$document['Person']);
    foreach ($wer as $key => $value) 
    {
      sendErinnerung($value, $document, $Besonderes);
      $gesendet = true;
    }
  }
  if ( $document['Gruppe'] != '' ) {
    $wer = explode(',',$document['Gruppe']);
    foreach ($wer as $key => $value) {
      $qres2 = sql_query ('SELECT user_id FROM '.TABLE_USERDATA.
        " WHERE name = 'group_".$value."' AND value = '1'");
      while ( $row2 = sql_fetch_row($qres2) ) {
        $prof = get_profile($row2[0], false);
        if ( $prof['Kal'.$document['parent_id'].'MailGrp'.$value] == 1 )
        {
          sendErinnerung($row2[0], $document, $Besonderes);
          $gesendet = true;
        }
      }
      sql_free_result($qres2);
    }
  }
  if ( $gesendet )
    echo '<B>&gt;&gt; '.LANG_MEMOSEND.'</b><br />';
}

?>