<?php


/*
  Modul cvjmPreise
  Verwaltet Preislisten und Preise

  AWF-Name:
  cvjmpreise

*/

if ( $profile['editor'] != 1 )
{
  include(INC_PATH.'misc/CVJM.inc');
  include(INC_PATH.'misc/Kalenderjs.inc');
  include(INC_PATH.'cvjm/Artikel.class.php');
  if ( $document['title'] != '')
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  if ( isset($docinput['DelPreisliste']) && is_numeric($docinput['DelPreisliste']) )
  {
    // Preisliste löschen
    $query = sql_query('SELECT * FROM '.TABLE_BUCHUNGEN.
      ' WHERE F_Preisliste_id='.$docinput['DelPreisliste']);
    if ( sql_num_rows($query) == 0 )
    {
      sql_query('DELETE FROM '.TABLE_PREISE.' WHERE F_Preisliste_id='.
        $docinput['DelPreisliste']);
      sql_query('DELETE FROM '.TABLE_PREISLISTEN.' WHERE Preisliste_id='.
        $docinput['DelPreisliste']);
      echo '&gt;&gt;&gt; Preisliste wurde gelöscht.<br />';

    }
    else
      echo '<div class="SchwererFehler">&gt;&gt;&gt; Preisliste wird in '.sql_num_rows($query).
        ' Buchungen benutzt und kann nicht gelöscht werden.</div>';
    sql_free_result($query);
  }
  if ( isset($_REQUEST['Kopie']) && is_numeric($_REQUEST['Kopie']) )
  {
    // Kopie einer Preisliste erstellen
    $query = sql_query('SELECT * FROM '.TABLE_PREISLISTEN.' WHERE Preisliste_id='.$_REQUEST['Kopie']);
    if ( $preisliste = sql_fetch_array($query) )
    {
      if ( ! sql_query('INSERT INTO '.TABLE_PREISLISTEN.
        " (Bezeichnung,GueltigAb,Bemerkung) VALUES ('Kopie von ".
        $preisliste['Bezeichnung']."',".$preisliste['GueltigAb'].
        ",'Kopiert von ".$preisliste['Bezeichnung'].' am '.date('d.m.Y').
        "\n".$preisliste['Bemerkung']."')")) echo sql_error();
      $docinput['Preisliste'] = sql_insert_id();
      $preise = sql_query('SELECT * FROM '.TABLE_PREISE.' WHERE F_Preisliste_id='.$_REQUEST['Kopie']);
      while ( $preis = sql_fetch_array($preise) )
      {
        $sql = 'INSERT INTO '.TABLE_PREISE.' (Preis,PreisStunde,Artikel_Nr,F_Preisliste_id) VALUES ';
        $sql .= '('.$preis['Preis'].','.$preis['PreisStunde'].','.$preis['Artikel_Nr'].','.
            $docinput['Preisliste'].')';
          if ( ! sql_query($sql) ) echo "Fehler $sql:".sql_error();

      }
      sql_free_result($preise);
    }
    sql_free_result($query);
  }
  $preislisten = sql_query('SELECT * FROM '.TABLE_PREISLISTEN.' WHERE NOT Inaktiv '.
    ' ORDER BY GueltigAb DESC, Bezeichnung');
  echo '<form action="'.get_url($id).'" method="post">';
  echo 'Aktive Preislisten <select name="docinput[Preisliste]">';
  $selected = false;
  while ( $preisliste = sql_fetch_array($preislisten) )
  {
    echo '<option value="'.$preisliste["Preisliste_id"].'"';
    if ( !isset($docinput['Preisliste']) && !$selected && $preisliste['Standard'])
    {
    	echo ' selected="selected"';
    	$selected =true;
    }
    if ( isset($docinput['Preisliste']) &&
         $docinput['Preisliste'] == $preisliste['Preisliste_id'])
    {
      echo ' selected="selected"';
      $Preislistenname = stripslashes($preisliste['Bezeichnung']);
      $GueltigAb = $preisliste['GueltigAb'];
      $Bemerkung = stripslashes($preisliste['Bemerkung']);
      $Std = $preisliste['Standard'];
      $Inaktiv = $preisliste['Inaktiv'];
      $Mitarbeiterliste = $preisliste['Mitarbeiterliste'];
    }
    echo '>'.stripslashes($preisliste['Bezeichnung']).' ('.date('d.m.Y',
      $preisliste['GueltigAb']).')</option>';
  } // while
  sql_free_result($preislisten);
  echo '</select> ';
  echo '<input type="Submit" value="Auswählen"></form>';
  $preislisten = sql_query('SELECT * FROM '.TABLE_PREISLISTEN.' WHERE Inaktiv '.
    ' ORDER BY GueltigAb DESC, Bezeichnung');
  echo '<form action="'.get_url($id).'" method="post">';
  echo 'Inaktive Preislisten <select name="docinput[Preisliste]">';
  $selected = false;
  while ( $preisliste = sql_fetch_array($preislisten) )
  {
    echo '<option value="'.$preisliste["Preisliste_id"].'"';
    if ( !isset($docinput['Preisliste']) && !$selected && $preisliste['Standard'])
    {
    	echo ' selected="selected"';
    	$selected =true;
    }
    if ( isset($docinput['Preisliste']) &&
         $docinput['Preisliste'] == $preisliste['Preisliste_id'])
    {
      echo ' selected="selected"';
      $Preislistenname = stripslashes($preisliste['Bezeichnung']);
      $GueltigAb = $preisliste['GueltigAb'];
      $Bemerkung = stripslashes($preisliste['Bemerkung']);
      $Std = $preisliste['Standard'];
      $Inaktiv = $preisliste['Inaktiv'];
      $Mitarbeiterliste = $preisliste['Mitarbeiterliste'];
    }
    echo '>'.stripslashes($preisliste['Bezeichnung']).' ('.date('d.m.Y',
      $preisliste['GueltigAb']).')</option>';
  } // while
  sql_free_result($preislisten);
  echo '</select> ';
  echo '<input type="Submit" value="Auswählen"></form>';
  echo '[ <a href="'.get_url($id,array('docinput[Preisliste]'=>-1,'Edit'=>-1)).
      '">Neue (leere) Preisliste</a> ]';
  if ( isset($docinput['Preisliste']) && is_numeric($docinput['Preisliste']) )
  {
    if ( $docinput['Preisliste'] < 0 )
    {
      // Neue Preisliste
      $Bemerkung = '';
      $GueltigAb = time();
      $Preislistenname = '-Neue Preisliste-';
      sql_query('INSERT INTO '.TABLE_PREISLISTEN." (Bezeichnung, GueltigAb) VALUES ('$Preislistenname',$GueltigAb)");
      $docinput["Preisliste"] = sql_insert_id();
    }
    if ( isset($docinput["Prozent"]) && is_numeric($docinput["Prozent"]))
    {
      $was = (100+$docinput["Prozent"])/100;
      sql_query("UPDATE ".TABLE_PREISE." SET Preis=Preis*$was WHERE F_Preisliste_id = ".$docinput["Preisliste"]);
    }
    if ( isset($_REQUEST["SaveData"]))
    {
      $sql = "UPDATE ".TABLE_PREISLISTEN." SET Bezeichnung='".
        sql_real_escape_string($docinput["Bezeichnung"]);
      $sql .= "',Bemerkung='".sql_real_escape_string($docinput["Bemerkung"]).
       "',GueltigAb=";
      $datum = explode('.',$docinput['GueltigAb']);
      $datum = strtotime($datum[2].'-'.$datum[1].'-'.$datum[0]);
      $sql .= $datum;
      if ( $docinput['Std'] == 'v' )
      {
        $Std = 1;
        sql_query('UPDATE '.TABLE_PREISLISTEN.' SET Standard=0');
      }
      else
        $Std = 0;
      $sql .= ' ,Standard='.$Std;
      if ( $docinput['Inaktiv'] == 'v' )
      {
        $Inaktiv = 1;
        
      }
      else
      $Inaktiv = 0;
      $sql .= ',Inaktiv='.$Inaktiv;
      if ( $docinput['Mitarbeiterliste'] == 'v' )
      {
        $Mitarbeiterliste = 1;
        
      }
      else
      $Mitarbeiterliste = 0;
      $sql .= ',Mitarbeiterliste='.$Mitarbeiterliste;      
      $sql .= ' WHERE Preisliste_id = '.$docinput['Preisliste'];
      if ( ! sql_query($sql)) echo "Fehler $sql: ".sql_error();
      $Preislistenname = $docinput['Bezeichnung'];
      $GueltigAb = $datum;
      $Bemerkung = $docinput['Bemerkung'];
    }
    if ( isset($_REQUEST['SavePreis']))
    {  
      foreach ($docinput['Preise'] as $key => $value )
      {
        $value = trim(str_replace(',','.',$value));
        $value2 = trim(str_replace(',','.',$docinput['PreiseStunde'][$key]));
        if ( $value == '' && $value2 == '' )
        {
          // Preis löschen
          if ( ! sql_query('DELETE FROM '.TABLE_PREISE.' WHERE F_Preisliste_id='.$docinput['Preisliste'].
            " AND Artikel_Nr=$key")) echo "Fehler: ".sql_error();
        }
        else
        {
          if ( ! is_numeric($value2) ) $value2 = 0;
          if ( ! is_numeric($value) ) $value = 0;
          $sql = "UPDATE ".TABLE_PREISE." SET Preis=".$value.", PreisStunde=".$value2.
              " WHERE Artikel_Nr=$key AND F_Preisliste_id = ".$docinput["Preisliste"];
          if ( ! sql_query($sql) ) echo "Fehler $sql:".sql_error();
        }

      } // while
      // Neue Preise prüfen
      Foreach ( $docinput["NeuPreis"]  as $key => $value)
      {
        $value = trim(str_replace(',','.',$value));
        $value2 = trim(str_replace(',','.',$docinput['NeuPreisStunde'][$key]));
        if ( ! is_numeric($value) ) $value = 0;
        if ( ! is_numeric($value2) ) $value2 = 0;
        if ( $value != 0 || $value2 != 0 )
        {
          $sql = 'INSERT INTO '.TABLE_PREISE.' (Preis,PreisStunde,Artikel_Nr,F_Preisliste_id) VALUES ';
          $sql .= "($value,$value2,$key,".$docinput['Preisliste'].')';
          if ( ! sql_query($sql) ) echo "Fehler $sql:".sql_error();
        }
      } // while
    } // wenn Sichern
    echo '<hr />';
    if ( isset($_REQUEST['Edit']) && $_REQUEST['Edit'] <= -1 )
    {
      KalenderScriptEinfuegen();
      echo '&nbsp;&nbsp;&nbsp;[ <a href="'.get_url($id,'Kopie='.$docinput['Preisliste']).
        '" title="Preisliste zur Preisanpassung kopieren">Preisliste kopieren</a> ]';
      echo '<form action="'.get_url($id).'" method="post" name="Preisliste">';
      echo '<input type="hidden" name="docinput[Preisliste]" value="'.$docinput["Preisliste"].'" />';
      echo 'Preislisten-Bezeichnung <input type="Text" name="docinput[Bezeichnung]" value="';
      echo $Preislistenname;
      echo '" size="30" maxlength="30" /><br />';
      echo 'Gültig ab <input type="Text" name="docinput[GueltigAb]" value="';
      echo date("d.m.Y",$GueltigAb).'" name="Gueltig" size="10" maxlength="10"';
      KalenderEvents("Preisliste", "Gueltig");
      echo '>&nbsp;&nbsp;&nbsp;Standard-Preisliste ';
      echo '<input type="Checkbox" name="docinput[Std]" value="v" ';
      if ( $Std )
        echo 'checked="checked"';
      echo '/>';
      echo '&nbsp;&nbsp;&nbsp;Inaktiv ';
      echo '<input type="Checkbox" name="docinput[Inaktiv]" value="v" ';
      if ( $Inaktiv )
        echo 'checked="checked"';
      echo ' title="Liste inaktiv machen damit sie nicht mehr angezeigt wird"';
      echo '/>';
      echo '&nbsp;&nbsp;&nbsp;Mitarbeiterliste ';
      echo '<input type="Checkbox" name="docinput[Mitarbeiterliste]" value="v" ';
      if ( $Mitarbeiterliste )
        echo 'checked="checked"';
      echo ' title="Liste als Mitarbeiterpreisliste markieren (für Kiosk)"';
      echo '/>';      
      echo "<br />\n";
      echo 'Bemerkung<br />';
      echo '<textarea name="docinput[Bemerkung]" cols="60" rows="5">';
      echo $Bemerkung;
      echo '</textarea>';
      echo "<br />\n";
      echo '<input type="Submit" name="SaveData" value="Speichern" />';
      echo '</form>'."\n";
    }
    else
    {
      echo "<h2>$Preislistenname</h2>";
      echo 'Gültig ab: '.date('d.m.Y',$GueltigAb);
      if ( $Inaktiv ) echo " (diese Preisliste ist inaktiv)";
      if ( $Mitarbeiterliste ) echo " (diese Preisliste ist eine Mitarbeiterpreisliste)";
      if ( $Std ) echo '&nbsp;&nbsp;<span class="Achtung">Standard-Preisliste</span>';
      echo '<br />';
      echo nl2br($Bemerkung).'<br />';
    }
    echo '[ <a href="'.get_url($id,'Kopie='.$docinput['Preisliste']).
        '" title="Preisliste zur Preisanpassung kopieren">Preisliste kopieren</a> ]';
    echo '[ <a href="'.get_url($id,array("docinput[Preisliste]"=>$docinput["Preisliste"],
      "Edit"=>-1)).'">Preisliste Bearbeiten</a> ]';
    echo '[ <a href="'.get_url(get_cmodule_id('cvjmartikelliste'),
      'docinput[Preisliste]='.$docinput['Preisliste']).'">Preise ansehen</a> ]';
    $query = sql_query("SELECT * FROM ".TABLE_BUCHUNGEN." WHERE F_Preisliste_id=".$docinput["Preisliste"]);
    if ( sql_num_rows($query) == 0 )
      echo ' [ <a href="'.get_url($id,"docinput[DelPreisliste]=".$docinput["Preisliste"]).
        '">Preisliste löschen</a> ]';
    else
      echo ' Preisliste wird in '.sql_num_rows($query).' Buchungen benutzt<br />';
    sql_free_result($query);

    echo '<form action="'.get_url($id, 'docinput[Preisliste]='.
      $docinput['Preisliste']).'" method="post">';
    echo '<a id="Liste" name="Liste"></a>Artikelgruppe <select name="Edit">';
    $preise = sql_query("SELECT * FROM ".TABLE_ARTIKELARTEN." ORDER BY Art");
    while ( $row = sql_fetch_array($preise) )
    {
      echo '<option value="'.$row['Art_id'].'" ';
      if ( isset($_REQUEST['Edit']) && $_REQUEST['Edit'] == $row['Art_id'])
        echo 'selected="selected"';
      echo '>'.$row['Art'].'</option>'."\n";
      //' [ <a href="#'.$row["Art_id"].'V">vorhandene Artikel zur Preisliste</a> ] ';
      //echo '[ <a href="'.get_url($id,array("docinput[Preisliste]"=>$docinput["Preisliste"],
      //    "Edit"=>$row["Art_id"])).'#'.$row["Art_id"].'N">Preise bearbeiten</a> ]';
      //echo '</li>';
    }
    sql_free_result($preise);
    echo '</select>';
    echo ' <input type="submit" value="bearbeiten" />';
    echo '</form>';
    if ( isset($_REQUEST["Edit"]) && $_REQUEST["Edit"] >= 0 )
    {
      echo '<form action="'.get_url($id,'docinput[Preisliste]='.
         $docinput['Preisliste']).'" method="post">';
      echo '<table>';     
      echo '<tr><td colspan="6" align="center">';
      echo '<input type="Submit" name="SavePreis" value="Speichern" />';
      echo '<br />Geben Sie 0 ein wenn der Artikel nichts kostet, geben Sie nichts
      (auch keine 0) ein, um einen Preis zu löschen.';
      echo '</td></tr>';
    $Where = '';
    if ( $_REQUEST['Edit'] > 0 )
      $Where = ' AND F_Art_id='.$_REQUEST['Edit'];
    if ( !$preise = sql_query("SELECT * FROM (".TABLE_PREISE." INNER JOIN ".TABLE_ARTIKEL.
      " ON id=Artikel_Nr) INNER JOIN ".TABLE_ARTIKELARTEN.
      " ON F_Art_id=Art_id WHERE F_Preisliste_id=".
      $docinput["Preisliste"]." $Where ORDER BY Art,Bezeichnung"))
        echo "Fehler: ".sql_error();
    echo '<tr bgcolor="'.$design["table_header"].'"><th>Bezeichnung</th>';
    echo '<th>Preis EK</th><th>Preis netto</th><th>Preis brutto</th><th>Preis/h netto</th><th>Preis/h brutto</th></tr>'."\n";
    $Art = -1;
    $ids = array();
    $Color = $design["table_row_bright"];
    while ( $preis = sql_fetch_array($preise) )
    {
      $Artikel = new Artikel($preis['id']);
      if ( $Art != $preis["Art_id"] )
      {      	
      	echo '<tr><td><a id="'.$preis["Art_id"].'V" name="'.$preis["Art_id"].
            'V"></a><strong>'.$preis["Art"].'</strong></td><td></td><td></td>' .
            		'<td></td><td></td><td align="right">';
        echo '<a href="#Liste"><img src="img/small_edit/up.gif" border="0" />' .
        		'</td></tr>';
        $Art = $preis["Art_id"];
      }
      echo '<tr bgcolor="'.$Color.'">';
      echo '<td>';
      echo '<span style="width:'.($Artikel->Einruecken*10).'px">&nbsp;</span>';
      echo '<a href="'.get_url(get_cmodule_id("cvjmartikel"),"docinput[Artikel_Nr]=".$Artikel->Artikel_id);
      echo '">'.stripslashes($Artikel->Bezeichnung)."</a>";
      echo '</span>';
      echo '</td>';
      echo '<td align="right">';
      echo number_format($Artikel->Einkaufspreis,2);
      echo '</td>';
      echo '<td align="right">';
      if ( isset($_REQUEST["Edit"]) && $_REQUEST['Edit'] != -1 )
        echo '<input type="Text" name="docinput[Preise]['.$Artikel->Artikel_id.']" size="7" maxlength="10" value="';
      echo number_format($preis['Preis'],2);
      if ( isset($_REQUEST['Edit']) && $_REQUEST['Edit'] != -1)
        echo '" />';
      echo '</td>';
      echo '<td align="right">'.number_format($preis['Preis']*(100+$Artikel->getMWST())/100,2).'</td>';
      echo '<td align="right">';
      if ( isset($_REQUEST['Edit']) && $_REQUEST['Edit'] != -1 )
        echo '<input type="Text" name="docinput[PreiseStunde]['.$Artikel->Artikel_id.']" size="7" maxlength="10" value="';
      echo number_format($preis['PreisStunde'],2);
      if ( isset($_REQUEST['Edit']) && $_REQUEST['Edit'] != -1 )
        echo '" />';
      echo '</td>';
      echo '<td align="right">'.number_format($preis['PreisStunde']*(100+$Artikel->getMWST())/100,2).'</td>';
      echo '</tr>'."\n";
      if ( $Color == $design['table_row_bright'] )
        $Color = $design['table_row_dark'];
      else
        $Color = $design['table_row_bright'];
      $ids[] = $preis['id'];
    } // while
    sql_free_result($preise);
    if ( isset($_REQUEST['Edit']) && $_REQUEST['Edit'] != -1 )
    {
      echo '<tr><td colspan="6"><hr /></td></tr>'."\n";
      // neue Artikel anzeigen
      $Art = -1;
      $sql = 'SELECT * FROM '.TABLE_ARTIKEL.' INNER JOIN '.
          TABLE_ARTIKELARTEN.' ON F_Art_id=Art_id';
      $ssql = '';
      if ( $_REQUEST['Edit'] > 0 )
        $ssql = 'F_Art_id='.$_REQUEST['Edit'];
      if ( Count($ids) != 0 )
      {
        if ( $ssql != '' ) $ssql .= ' AND ';
        $ssql .= 'id NOT IN ('.implode(',',$ids).')';
      }
      if ( $ssql != '' ) $sql .= ' WHERE '.$ssql;
      $sql .= ' ORDER BY Art, Position';
      if ( ! $artikel = sql_query($sql)) echo "Fehler $sql: ".sql_error();
      //echo $sql;
      while ( $preis = sql_fetch_array($artikel) )
      {
        if ( $Art != $preis['Art_id'] )
        {
          echo '<tr><td><a id="'.$preis["Art_id"].'N" name="'.$preis["Art_id"].
            'N"></a><strong>'.$preis["Art"].'</strong></td><td></td><td></td><td></td><td></td><td align="right">';
          echo '<a href="#Liste"><img src="img/small_edit/up.gif" border="0" /></td></tr>';
          $Art = $preis["Art_id"];
        }
        echo '<tr bgcolor="'.$Color.'">';
        echo '<td>';
        echo '<span style="width:'.($preis["Einruecken"]*10).'px;float:left;">&nbsp;</span>';
        echo stripslashes($preis["Bezeichnung"]);
        echo '</span>';
        echo '</td>';
        echo '<td align="right">';
        echo number_format($preis['Einkaufspreis'],2);
        echo '</td>';
        echo '<td align="right">';
        echo '<input type="Text" name="docinput[NeuPreis]['.$preis["id"].
          ']" size="7" maxlength="10" />';
        echo '</td><td></td><td align="right">';
        echo '<input type="Text" name="docinput[NeuPreisStunde]['.$preis["id"].
          ']" size="7" maxlength="10" />';
        echo "</td><td></td></tr>\n";
        if ( $Color == $design["table_row_bright"] )
          $Color = $design["table_row_dark"];
        else
          $Color = $design["table_row_bright"];
      } // while
      sql_free_result($artikel);
      echo '<tr><td colspan="6" align="center">';
      echo '<input type="Submit" name="SavePreis" value="Speichern" />';
      echo '<br />Geben Sie 0 ein wenn der Artikel nichts kostet, geben Sie nichts
      (auch keine 0) ein, um einen Preis zu löschen.';
      echo '</td></tr>';
      echo '</form>'."\n";
      echo '<form action="'.get_url($id).'" method="post">';
      echo '<tr><td colspan="2">Preisanpassung um ';
      echo '<input type="hidden" name="docinput[Preisliste]" value="'.$docinput["Preisliste"].
        '" />';
      echo '<input type="Text" name="docinput[Prozent]" size="5" maxlength="5" /> % ';
      echo '<input type="Submit" value="durchführen" />';
      echo '</td></tr>';
      echo '</form>';
      } // wenn editieren
      echo '</table>';
    } // wenn editieren
    echo '[ <a href="'.get_url($id,array("docinput[Preisliste]"=>$docinput["Preisliste"],
      "Edit"=>-1)).'">Preisliste Bearbeiten</a> ]';
    echo '[ <a href="'.get_url(get_cmodule_id('cvjmartikelliste'),
      'docinput[Preisliste]='.$docinput['Preisliste']).'">Preise ansehen</a> ]';
    $query = sql_query("SELECT * FROM ".TABLE_BUCHUNGEN." WHERE F_Preisliste_id=".$docinput["Preisliste"]);
    if ( sql_num_rows($query) == 0 )
      echo ' [ <a href="'.get_url($id,"docinput[DelPreisliste]=".$docinput["Preisliste"]).
        '">Preisliste löschen</a> ]';
    else
      echo ' Preisliste wird in '.sql_num_rows($query).' Buchungen benutzt';
    sql_free_result($query);
  } // Wenn Preisliste gewählt
} // wenn nicht Editor

?>