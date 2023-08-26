<?php

/*

AWF-Modul Stundenzettel

Dateiname: cvjmStundenzettel.inc
AWF-Name: CVJM Stundenzettel

*/

function isFeiertag($Datum)
{
  $Datum = mktime(0,0,0,date('m',$Datum),date('d',$Datum),date('Y',$Datum));
  $query = sql_query('SELECT * FROM '.TABLE_FEIERTAGE.' WHERE Datum='.$Datum);
  $row = sql_fetch_array($query);
  sql_free_result($query);
  return $row['Datum'] == $Datum;
}

if ( $profile['editor'] != 1 )
{
  load_lang('Kalender');
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
  $Benutzer = SESSION_DBID;
  $Arbeitsarten = array(
  'ALG' => 'Allgemein',
  'AUS' => 'Au�enbereich',
  'KIO'	=> 'Kiosk',
  'KUE'	=> 'K�che',
  'PAE' => 'P�dagogik',
  'PUT' => 'Putzen',
  'TIE' => 'Tiere',
  'VER' => 'Verwaltung',
  'URL' => 'Erholungsurlaub',
  'SUB' => 'Sonderurlaub',
  'KRK' => 'Krank');
  
  if ( isset($docinput['Benutzer']) && isset($profile[$document['Zugriffsrecht']]) )
  {
    // Anderen Benutzer ansehen
    $Benutzer = $docinput['Benutzer'];
  }
  if ( isset($_REQUEST['Monatsabschluss']) && isset($profile[$document['Zugriffsrecht']]) )
  {
    $Benutzer = $_REQUEST['Monatsabschluss'];
  }
  echo '<h2>'.transform($document['title'], $document['datatype']['title']).' (';
  echo get_user_nickname($Benutzer).')</h2>';
  if ( SESSION_STATUS != 'ok' )
  {
    echo 'Sie müssen sich einloggen damit Sie den Stundenzettel ansehen können.<br />';
  } // SESSION nicht ok
  else
  {
    include(INC_PATH.'misc/CVJM.inc');
    include(INC_PATH.'misc/Kalenderjs.inc');
    if ( isset($docinput['Datum']) )
    {
      $stand = convertDatumToTimestamp($docinput['Datum']);
      if ( $docinput['Datum'] == '')
      {
      	$stand = time();
      }
      $docinput['Datum'] = $stand;
      if ( ! isset($docinput['Von']) ) $docinput['Von'] = '00:00';
      if ( ! isset($docinput['Bis']) ) $docinput['Bis'] = '00:00';
      if ( strpos($docinput['Von'],':') === false )
      {
        while ( strlen($docinput['Von']) < 4 )
          $docinput['Von'] = '0'.$docinput['Von'];
        $docinput['Von'] = substr($docinput['Von'],0,2).':'.substr($docinput['Von'],2,2);
      }
      if ( strpos($docinput['Bis'],':') === false )
      {
        while ( strlen($docinput['Bis']) < 4 )
          $docinput['Bis'] = '0'.$docinput['Bis'];
        $docinput['Bis'] = substr($docinput['Bis'],0,2).':'.substr($docinput['Bis'],2,2);
      }
      if ( ($docinput['Datum'] = strtotime(date('Y-m-d',$stand).' '.$docinput['Von']) ) === -1 )
      {
        unset($_REQUEST['Monatsabschluss']);
        unset($docinput['Bemerkung']);
        echo '<strong>&gt;&gt;&gt; Fehler: ungültiger Anfangszeitpunkt!</strong><br />';
        $docinput['Datum'] = time();
      }
      if ( ($docinput['Bis'] = strtotime(date('Y-m-d',$stand).' '.$docinput['Bis']) ) === -1 )
      {
        unset($_REQUEST['Monatsabschluss']);
        unset($docinput['Bemerkung']);
        echo '<strong>&gt;&gt;&gt; Fehler: ungültiger Endzeitpunkt!</strong><br />';
      }
      // Monatsabschluss
      if ( isset($_REQUEST['Monatsabschluss']) && isset($profile[$document['Zugriffsrecht']]) )
      {
        if ( sql_query('UPDATE '.TABLE_STUNDENZETTEL.' SET Gebucht=NOT Gebucht '.
          'WHERE Person_id = '.$Benutzer .
          ' AND Datum BETWEEN '.mktime(0,0,0,date('m',$docinput['Datum']),1,
         date('y',$docinput['Datum'])).' AND '.mktime(23,59,59,date('m',$docinput['Datum']),
         date('t',$docinput['Datum']),date('y',$docinput['Datum']))))
          echo '<strong>&gt;&gt;&gt; Monatsabschluss</strong><br />';
         else
           echo 'Fehler: '.sql_error();
      }
      // Speichern
      if ( isset($docinput['Bemerkung'] ) )
      {
        if ( isset($docinput['ZID']) )
        {
          // Update
          $sql = 'UPDATE '.TABLE_STUNDENZETTEL.
            ' SET Datum='.$docinput['Datum'].',Bis='.$docinput['Bis'].",Bemerkung='";
          $sql .= sql_real_escape_string($docinput["Bemerkung"])."', Art='".
          sql_real_escape_string($docinput['Art'])."' WHERE id=".$docinput["ZID"];
          if ( sql_query($sql))
            echo "<strong>&gt;&gt;&gt; Arbeitszeit geändert.<br /></strong>";
          else
            echo "<string>&gt;&gt;&gt; Fehler </strong>".sql_error()."<br />";
        }
        else
        {
          // Insert
          $sql = "INSERT INTO ".TABLE_STUNDENZETTEL.
            " (Datum, Bis, Bemerkung, Person_id,Art) VALUES (";
          $sql .= $docinput["Datum"].",".$docinput["Bis"].",'";
          $sql .= sql_real_escape_string($docinput["Bemerkung"])."',".$Benutzer.",'".
          sql_real_escape_string($docinput['Art'])."')";
          if ( sql_query($sql))
            echo "<strong>&gt;&gt;&gt; Arbeitszeit gespeichert.<br /></strong>";
          else
            echo "<string>&gt;&gt;&gt; Fehler </strong>".sql_error()."<br />";
        }
      }
    }
    require_once(INC_PATH.'Smarty/Smarty.class.php');
    $Smarty = new Smarty();
    $Smarty->template_dir = TEMPLATEPATH; //'/srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/templates';
    $Smarty->assign('id', $id);
    $Smarty->assign_by_ref('docinput', $docinput);
    if ( isset($docinput['Del']) && is_numeric($docinput['Del']))
    {
    	$query = sql_query("DELETE FROM ".TABLE_STUNDENZETTEL." WHERE id=".$docinput['Del']);
    	sql_free_result($query);
    } 
    if ( isset($docinput["Zeit"]) && is_numeric($docinput['Zeit']))
    {
        $query = sql_query("SELECT * FROM ".TABLE_STUNDENZETTEL." WHERE id=".$docinput["Zeit"]);
        if ( ! $eintrag = sql_fetch_array($query))
          echo "<strong>&gt;&gt;&gt; Eintrag nicht gefunden!<br /></strong>";
        sql_free_result($query);
    }
    if ( ! isset($eintrag) )
    {
        $eintrag["Datum"] = time();
        $eintrag["Bis"] = time();
    }
    $Smarty->assign_by_ref('eintrag', $eintrag);
    $Smarty->assign('Benutzer', $Benutzer);
    $Smarty->assign_by_ref('Arbeitsarten', $Arbeitsarten);
    $Smarty->display('Stundenzettel_Eingabe.tpl');
    
    if ( ! isset($docinput["Datum"]) || ! is_numeric($docinput["Datum"]) )
      $docinput["Datum"] = time();
    $Smarty->assign('Monat', $mth[date("n",$docinput["Datum"])].' '.date("Y", $docinput["Datum"]));
    $query = sql_query("SELECT * FROM ".TABLE_STUNDENZETTEL." WHERE Person_id=".$Benutzer.
      " AND Datum BETWEEN ".mktime(0,0,0,date("m",$docinput["Datum"]),1,
        date("y",$docinput["Datum"]))." AND ".mktime(23,59,59,date("m",$docinput["Datum"]),
        date("t",$docinput["Datum"]),date("y",$docinput["Datum"]))." ORDER BY Datum DESC");
    $Stunden = 0;
    $SoStunden = 0;
    while ( $row = sql_fetch_array($query) )
    {
      $zeit = $row["Bis"] - $row["Datum"];
      $std = sprintf("%.02f",$zeit/3600);
      if ( isFeiertag($row["Datum"]) )
      {
        $FeierStunden += $std;
        $Stunden += $std;
        $row['Normalstunden'] = $std;
        $row['Feiertagstunden'] = $std;
      }
      elseif ( date("w", $row["Datum"]) != 0 )
      {
        $Stunden += $std;
        $row['Normalstunden'] = $std;
      }
      else
      {
        $SoStunden += $std/2;
        $row['Sonntagstunden'] = $std/2;
        $Stunden += $std;
        $row['Normalstunden'] = $std;
      }
      $eintraege[] = $row;
    }
    $Smarty->assign_by_ref('Eintraege', $eintraege);
    $Smarty->assign('Stunden', sprintf("%.02f",$Stunden));
    $Smarty->assign('SoStunden', sprintf("%.02f",$SoStunden));
    $Smarty->assign('FeierStunden', sprintf("%.02f",$FeierStunden));
    $Smarty->assign('Gesamtstunden', sprintf("%.02f",$SoStunden+$FeierStunden+$Stunden));
    sql_free_result($query);
    if ( isset($_REQUEST['Tag']))
    {
    	$Smarty->display('Stundenzettel_Liste_Tage.tpl');
    }
    else
    {
    $Smarty->display('Stundenzettel_Liste_Gesamt.tpl');
    }
      echo '<div align="center" class="noprint">[ <a href="'.get_url($id,
        "docinput[Benutzer]=$Benutzer&docinput[Datum]=".date("d.m.Y",
          strtotime("-1 month",$docinput["Datum"])));
      echo '">Monat Zurück</a> ] [ <a href="'.get_url($id,"docinput[Benutzer]=$Benutzer&docinput[Datum]=".
        date("d.m.Y",strtotime("+1 month",$docinput["Datum"])));
      echo '">Monat vor</a> ]</div>';
      echo '<form action="'.get_url($id).'" name="Springen" method="post" class="noprint">';
      if ( $Benutzer != SESSION_DBID )
      {
      echo '<input type="hidden" name="docinput[Benutzer]" value="'.$Benutzer.'" />';
      }
      echo 'Springe nach Datum <input type="Text" name="docinput[Datum]" value="" size="10" maxlength="10" ';
      KalenderEvents("Springen", "docinput[Datum]");
      echo ' />';
      echo '<input type="checkbox" value="1" name="Tag" /> tagesweise ';
      echo '<input type="Submit" value="Anzeigen">';
      echo '</form>';
    if ( isset($profile[$document["Zugriffsrecht"]]) )
    {
      // Berechtigter Nutzer: Anzeige der User
      $query = "SELECT id FROM ".TABLE_USERS.' WHERE valid';
      $query = sql_query($query);
      while ( $row = sql_fetch_row($query))
      {
        $user[$row[0]] = get_user_nickname($row[0]);
      }
      sql_free_result($query);
      //array_multisort($user, SORT_ASC);
      echo '<hr class="noprint" /><form action="'.get_url($id).'" method="post" class="noprint">';
      echo "Stunden von Benutzer ";
      echo '<input type="hidden" name="docinput[Datum]" value="'.
        date("d.m.Y",$docinput["Datum"]).'" />';
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
      echo '[ <a href="'.get_url($id,"Monatsabschluss=$Benutzer&docinput[Datum]=".
        date("d.m.Y",$docinput["Datum"])).
        '">Monat für Mitarbeiter abschlie�en/freigeben</a>]';
       echo '</form>';
    }
  } // SESSION ist ok
} // wenn nicht editor
else
{
  echo '<br /><br />';
  create_editor_input('Recht Stundenzettelabrechnung','Zugriffsrecht','text_input');
}

?>
