<?php
/*
        The author is Christoph Griep (cgriep@fricom.com)
        Last update: 11.07.2003
*/
load_lang("db_table");
if ( $profile['editor'] != "1" ) {
  echo '<h1 clear="all">'.transform($document['title'], $document['datatype']['title']).'</h1>';
  if($document['image'] != '' || $document['body'] != '') {
    echo '<p class="justify">';
    // Image
    echo transform($document['image'], $document['datatype']['image']);
    // Text
    echo transform($document['body'], $document['datatype']['body']).'</p><br clear="all" />';
  }
  // Datenbank auslesen
  $Felder = explode("|",$document['Felder']);
  $Feldnamen = implode(",",$Felder);
  $Auswahlfeld = explode("|", $document["AuswahlFelder"]);
  $Auswahlfelder = array();
  while ( list($key, $value) = each($Auswahlfeld) )
  {
    list($feld, $wert) = explode(":", $value);
    $Auswahlfelder[$feld]= $wert;
  }
  $new = -1;
  $Recht = get_node($document["parent_id"], $flavour);
  $Bearbeiten = ($profile[$Recht["NewDS"]] == '1' && $_REQUEST[$document['IDFeld']] == "") ||
                ($profile[$document["EditDS"]] == 1 && $_REQUEST[$document['IDFeld']] != "");
    if ( isset($_REQUEST['Del']) && $profile[$document["DelDS"]] == "1" )
    {
      if ( isset($_REQUEST[$document['IDFeld']]) && $_REQUEST[$document['IDFeld']] != "" )
      {
        $sql = "DELETE FROM " .$document['Tabelle']." WHERE " . $document['IDFeld'] . "='";
        $sql .= $_REQUEST[$document['IDFeld']] . "'";
        if ( sql_query($sql) )
          echo "Datensatz gel&ouml;scht!<br />";
        else
          echo "Fehler beim L&ouml;schen!<br />";
        //echo sql_error();
      }
    }
    else
    {
      if ( isset($_REQUEST['save']) && (
        ($profile[$Recht["NewDS"]] == 1 && isset($_REQUEST["new"]) ) ||
        ($profile[$document["EditDS"]] == 1 && ! isset($_REQUEST["new"]) ) ) ) {
        $datatypes = $_REQUEST["datatype"];
        if ( ! isset($_REQUEST['new']) )
        {
          // Sichern
          $sql = "UPDATE " . $document['Tabelle'] . " SET ";
          while ( list($key, $feld) = each ($Felder) ) {
            if (isset($_REQUEST[$feld]) )
            {
              $sql .= $feld ."=";
              if ( $datatypes[$feld] == "timestamp" ) {
                // Sonderbehandlung bei bestimmten Feldtypen
              }
              $sql .= "'";
              $sql .= $_REQUEST[$feld];
              $sql .= "',";
            }
          }
          //echo $sql;
        }
        else
        {
          // neuer Datensatz
          $sql = "INSERT INTO " . $document['Tabelle'] . " (";
          $sql2 = ") VALUES (";
          while ( list($key,$feld) = each ($Felder) ) {
            if (isset($_REQUEST[$feld]))
            {
              $sql2 .= "'";
              $sql2 .= $_REQUEST[$feld];
              $sql2 .= "',";
              $sql .= $feld . ",";
            }
          }
          // Default-Werte erg&auml;nzen
          if ( $document["DefFeld"] != "" ) {
            $Felder = explode("|",$document["DefFeld"]);
            while ( list($key, $value) = each($Felder) ) {
              $def = explode(":", $value);
              $sql .= $def[0].",";
              $sql2.= "'".$def[1]."',";
            }
          }
          $sql .= $document['IDFeld'];
          $sql .= $sql2;
          $sql .= "'".$_REQUEST[$document['IDFeld']] . "'),";
        }
        // letztes Komma abschneiden
        $sql = substr($sql, 0, strlen($sql)-1);
        if ( ! isset($_REQUEST['new']))
          $sql .= " WHERE " . $document['IDFeld'] . "='" . $_REQUEST[$document['IDFeld']] . "'";
        //echo $sql;
        
        if ( sql_query($sql) )
          echo "<b><i>&gt;&gt;&gt;Daten erfolgreich gesichert!</i></b><br />";
        else echo "<b>Fehler: ".sql_error()."</b> $sql<br />";
      }
      if ( $_REQUEST[$document['IDFeld']] != "" )
          $sql = "select $Feldnamen from " . $document['Tabelle']." WHERE " .
            $document['IDFeld'] . " = '" . $_REQUEST[$document['IDFeld']] . "'";
      else
      {
        $sql = "select $Feldnamen from " . $document['Tabelle'];
        echo "<b>&gt;&gt;&gt; Sie legen einen neuen Datensatz an!</b><br />";
      }
      if ( $Bearbeiten )
        echo '<form action="' . get_url($id) . '" method="post">';
      echo '<table width="100%">';
      if ( $query = sql_query($sql)) {
         if ( $Bearbeiten ) {
           echo '<tr><td colspan="2"><b><i></i>Geben Sie die angeforderten Informationen ein und klicken Sie auf ';
           echo LANG_SAVEDATA.'</i></b></td></tr>';
         }
         if ( isset($_REQUEST[$document['IDFeld']]) && $_REQUEST[$document['IDFeld']] != "" )
           $data = sql_fetch_array($query);
         else
           unset($data);
         $i = 0;
         while ( $i < sql_num_fields($query) )
         {
           $feld = sql_fetch_field($query, $i);
           unset($Werte);
           if ( isset($Auswahlfelder[$feld->name]) ) {
               // Auswahlbox
               $Werte = array();
               if ( ! $subquery = sql_query($Auswahlfelder[$feld->name]) )
                 echo "Fehler: ".sql_error();
               while ( $row = sql_fetch_row($subquery) )
               {
                 $Werte[$row[0]] = $row[1];
               }
               sql_free_result($subquery);
             }
           echo "<tr><td>";
           echo $feld->name . "</td><td>";
           $nochWas = true;
           if ( $feld->type == "int" && sql_field_len($query, $i) == 1 )
           {
             // bool-Type
             if ( $Bearbeiten ) {
             ?>
             <input type="radio" value="1" name="<?=$feld->name?>"<?php if($data[$i] == '1') echo ' CHECKED'; ?> /> Ja&nbsp;&nbsp;
             <input type="radio" value="0" name="<?=$feld->name?>"<?php if($data[$i] == '0') echo ' CHECKED'; ?> /> Nein
             <?php
             } else
               if ( $data[$i] == "1" ) echo "Ja"; else echo "Nein";
             $nochWas = false;
           }
           elseif ( $feld->type == "blob" )
           {
             if ( $Bearbeiten )
               echo '<textarea name="' . $feld->name . '" rows="5" cols="60">';
             echo transform($data[$i],"html");
             if ( $Bearbeiten )
               echo "</textarea>";
             $nochWas = false;
           }
           else
           {
             if ( $Bearbeiten ) {
               if ( isset($Werte) )
               {
                 echo '<select name="'.$feld->name.'">';
                 while ( list($key, $value) = each($Werte) ) {
                   echo '<option value="'.$key.'"';
                   if ( isset($data) )
                     if ( $data[$i] == $key ) echo ' selected="selected"';
                   echo '>'.$value.'</option>';
                 }
                 echo '</select>';
                 $nochWas = false;
               }
               else {
                 echo '<input type="text" name="' . $feld->name . '"';
                 if ( isset($data) ) echo ' value="' . transform($data[$i], "clean") . '" ';
               }
             }
             else {
               echo transform($data[$i],"clean");
               $nochWas = false;
             }
           }
           $len = 40;
           if ( $nochWas ) {
             if ( sql_field_len($query, $i) > 0 ) {
               echo 'maxlength="' . sql_field_len($query, $i) . '" ';
               if ( sql_field_len($query, $i) < 40 ) $len = sql_field_len($query, $i) + 2;
             }
             echo 'size="' . $len . '">';
           }
           echo '<input type="hidden" name="datatype['.$feld->name.']" value="'.
             $feld->type.'">';
           echo "</td></tr>";
           $i++;
         }
      }
      if ( $_REQUEST[$document['IDFeld']] == "" ) {
        echo '<tr><td>';
        echo '<input type="hidden" name="new" value="1">';
        // wenn das ID-Feld nicht angegeben wurde, ist es wohl ein auto_increment
        if ( strpos($document["Felder"],$document["IDFeld"]) === false ) {
          echo '</td></tr><tr><td colspan="2"><hr /></td></tr>';
          $query = sql_query("SELECT ".$document["IDFeld"]." FROM ".$document["Tabelle"].
            " WHERE ".$document["IDFeld"]." = ''");
          if ( strpos(sql_field_flags($query, 0), "auto_increment") === false ) {}
          else
            echo '<tr><td colspan="2"><b>Freilassen f&uuml;r automatische Belegung.</b></td></tr>';
          sql_free_result($query);
          echo "<tr><td>".$document['IDFeld'] . "</td><td>";
          echo '<input type="text" name="' . $document['IDFeld'] . '">';
        }
        echo '</td></tr>';
      }
      else
        echo '<input type="hidden" name="' . $document['IDFeld'] . '" value="' .
        $_REQUEST[$document['IDFeld']] . '">';
      sql_free_result($query);
      if ( $Bearbeiten ) {
        echo '<tr><td><input type="submit" value="' . LANG_SAVEDATA . '" name="save"></td>';
        echo '<td><input type="reset" value="' . LANG_RESET . '"></td></tr>';
      }
      echo "</table>";
      if ( $Bearbeiten ) echo "</form>";
      if( SESSION_STATUS == 'ok' && $profile[$document['DelDS']] == '1' && $_REQUEST[$document['IDFeld']] != "") {
        echo ' [ <a href="'.get_url($id,$document['IDFeld'].'='.$_REQUEST[$document['IDFeld']].'&Del=1').'">'.LANG_DELETE.'</a> ]';
      }
    }
  
  if(isset($parents)) {
    echo '[ <a href="'.get_url($document['parent_id'],"").'">'.LANG_SHOW_COMPLETE_DOC.'</a> ]';
  }

}

if ( $profile['editor']=='1' )
{
  echo '<br />';
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('body','body','area_input');
  echo '<br /><br />';
  create_editor_input('Tabelle','Tabelle','text_input');
  echo '<br /><br />';
  create_editor_input('Felder','Felder','text_input');
  echo '<br /><br />';
  create_editor_input('Default bei neuem Datensatz (Feld:Wert|...)','DefFeld','text_input');
  echo '<br /><br />';
  create_editor_input('Auswahlfelder (Feld:SQL id, Bez|...)','AuswahlFelder','text_input');
  echo '<br /><br />';
  create_editor_input('ID-Feld','IDFeld','text_input');
  echo '<br /><br />';
  create_editor_input('Berechtigung Datensatz l&ouml;schen','DelDS','text_input');
  echo '<br /><br />';
  create_editor_input('Berechtigung Datensatz bearbeiten','EditDS','text_input');
}

?>