<?php
/*
        Copyright (C) 2003 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Christoph Griep (griep@oszimt.de)
        Last update: 26.03.2003
        AWF-Name: Termin

*/

load_lang("Kalender");
include(INC_PATH."misc/Termin.inc");
include(INC_PATH."misc/Kalenderjs.inc");
if ( file_exists(INC_PATH."misc/CVJM.inc"))
{
  include(INC_PATH."misc/CVJM.inc");
  OrtsAuswahlScriptEinfuegen();
}

function save_termin($dd) {
    global $document;
    global $id;
    global $flavour;
    if(!($wert = strtotime($dd['Datum']))) $wert = 0;
    if ( $wert <= 0 ) {
      list($day,$month,$year) = explode(".",$dd['Datum']);
      list($year, $uhr) = explode(" ",$year);
      if ( $uhr != "" ) {
        list($stunde, $minute) = explode(":", $uhr);
        $uhr = $stunde.":" . $minute;
      }
      $wert = strtotime($year."-".$month."-".$day." ".$uhr);
    }
    $document['Datum'] = $wert;
    if(!update_nodedata($id, 'Datum', $document["Datum"], $flavour, 'datetime'))
      add_nodedata($id, 'Datum', $document["Datum"],$flavour,'datetime');
    if(!($wert = strtotime($dd['TurnusEnde']))) $wert = 0;
    if ( $wert <= 0 ) {
      list($day,$month,$year) = explode(".",$dd['TurnusEnde']);
      list($year, $uhr) = explode(" ",$year);
      if ( $uhr != "" ) {
        list($stunde, $minute) = explode(":", $uhr);
        $uhr = $stunde.":" . $minute;
      }
      $wert = strtotime($year."-".$month."-".$day." ".$uhr);
    }
    $document['TurnusEnde'] = $wert;
    if(!update_nodedata($id, 'TurnusEnde', $document["TurnusEnde"], $flavour, 'datetime'))
      add_nodedata($id, 'TurnusEnde', $document["TurnusEnde"],$flavour,'datetime');
    if(!($wert = strtotime($dd['Enddatum']))) $wert = 0;
    if ( $wert <= 0 && $dd["Enddatum"] != "" ) {
      list($day,$month,$year) = explode(".",$dd['Enddatum']);
      list($year, $uhr) = explode(" ",$year);
      if ( $uhr != "" ) {
        list($stunde, $minute) = explode(":", $uhr);
        $uhr = $stunde.":" . $minute;
      }
      $wert = strtotime($year."-".$month."-".$day." ".$uhr);
    }
    else $wert = $document["Datum"];
    if ( $wert != 0 ) {
      $document['Enddatum'] = $wert;
      if(!update_nodedata($id, 'Enddatum', $document["Enddatum"], $flavour, 'datetime'))
        add_nodedata($id, 'Enddatum', $document["Enddatum"],$flavour,'datetime');
    }
    $Person = "";
    foreach ($dd["Person"]as $key => $value)
      $Person .= $value.",";
    if ( $Person == "" && (count($dd["Gruppe"]) == 0 || $_REQUEST["DelGrp"] == "v") )
      $Person = SESSION_DBID.",";
    if ( $Person != "" )
      $Person = substr($Person, 0, strlen($Person)-1);
    if ( $_REQUEST["DelBet"] == "v" )
      $Person = "";
    if(!update_nodedata($id, 'Person', $Person, $flavour, 'text_input'))
      add_nodedata($id, 'Person', $Person, $flavour);
    $Gruppe = "";
    foreach ( $dd['Gruppe'] as $key => $value) 
      $Gruppe .= $value.",";
    if ( $Gruppe != "" )
      $Gruppe = substr($Gruppe, 0, strlen($Gruppe)-1);
    if ( $_REQUEST["DelGrp"] == "v") $Gruppe = "";
    if(!update_nodedata($id, 'Gruppe', $Gruppe, $flavour, 'text_input'))
      add_nodedata($id, 'Gruppe', $Gruppe, $flavour);
    $document["Gruppe"] = $Gruppe;
    $document["Person"] = $Person;
}

if($profile['editor']!='1') {
  if ( ! inPerson($document["Person"],$document["author_id"]) &&
       ! inGruppe($document["Gruppe"])) {
    echo "<b>&gt;&gt;&gt; ".LANG_NORIGHT."</b><br /><br />";
  }
  else {
    if ( $_REQUEST["Ok"] == "1" ) {
      send_message ($document["author_id"], "Termin $id wurde am ".date("d.m.Y H:i").
        " best�tigt.\n\nTitel: ".transform($document["title"],clean)."\nInhalt: ".
        transform($docinput["body"],clean)."\n".
        "Erstellt von ".get_user_nickname($document["author_id"])."\n", SESSION_DBID, '',
        "Terminbest�tigung ".transform($document["title"],"clean"));
      echo "<B>&gt;&gt; Termin best�tigt.</b><br />";
    }
    if ( $_REQUEST['Erinnerung'] == '1' ) {
      sendeAllen($document);
    }
    if ( isset($_REQUEST['Savetermin']) &&
       (inPerson($document['Person'], $document['author_id'],$document['Aendern']=='1') ||
         inGruppe($document['Gruppe'],-1,$document['Aendern']=='1')) ) {
      clear_cache($id);
      list($docinput['Tag'], $docinput['Monat'], $docinput['Jahr']) = explode('.', $docinput['Datum']);
      list($docinput['ETag'], $docinput['EMonat'], $docinput['EJahr']) = explode('.', $docinput['EDatum']);
      if ( ! checkdate($docinput['EMonat'],$docinput['ETag'],$docinput['EJahr']) )
      {
        $docinput['ETag'] = '';
        $docinput['EMonat'] = '';
        $docinput['EJahr'] = '';
      }
      list($docinput['TurnusEndeTag'], $docinput['TurnusEndeMonat'], $docinput['TurnusEndeJahr']) =
        explode('.', $docinput['TurnusDatum']);
      if ( ! checkdate($docinput['TurnusEndeMonat'],$docinput['TurnusEndeTag'],$docinput['TurnusEndeJahr']) )
      {
        $docinput['TurnusEndeTag'] = '';
        $docinput['TurnusEndeMonat'] = '';
        $docinput['TurnusEndeJahr'] = '';
      }
      update_nodedata($id, 'body', $docinput['body'], $flavour, 'text');
      update_nodedata($id, 'title', $docinput['title'], $flavour);
      update_nodedata($id, 'Datum', mktime($docinput['Stunde'], $docinput['Minute'],0,
        $docinput['Monat'], $docinput['Tag'], $docinput['Jahr']), $flavour, 'datetime');
      if ( $docinput['ETag'] == '' ) $docinput['ETag'] = $docinput['Tag'];
      if ( $docinput['EMonat'] == '' ) $docinput['EMonat'] = $docinput['Monat'];
      if ( $docinput['EJahr'] == '' ) $docinput['EJahr'] = $docinput['Jahr'];
      if ( $docinput['EStunde'] == '' ) $docinput['EStunde'] = $docinput['Stunde'];
      if ( $docinput['EMinute'] == '' ) $docinput['EMinute'] = $docinput['Minute'];
      update_nodedata($id, 'Enddatum', mktime($docinput['EStunde'], $docinput['EMinute'],0,
          $docinput['EMonat'], $docinput['ETag'], $docinput['EJahr']), $flavour, 'datetime');
      update_nodedata($id, 'Ort', $docinput['Ort'], $flavour);
      update_nodedata($id, 'Turnus', $docinput['Turnus'], $flavour);
      if ( is_numeric($docinput['TurnusEndeTag'] )) {
        $dd = mktime(0,0,0,$docinput['TurnusEndeMonat'],$docinput['TurnusEndeTag'], $docinput['TurnusEndeJahr']);
      }
      else $dd = 0;
      if ( date('d.m.Y',$dd) == '01.01.1970' ) $dd = 0;
      update_nodedata($id, 'TurnusEnde', $dd, $flavour);
      $Person = implode(',',$docinput['Person']);
      if ( $Person == '' && count($docinput['Gruppe']) == 0 )
        $Person = SESSION_DBID;
      if ( ! update_nodedata($id, 'Person', $Person, $flavour) )
        add_nodedata($id, 'Person', $Person, $flavour);
      $Gruppe = implode(',', $docinput['Gruppe']);
      if ( ! update_nodedata($id, 'Gruppe', $Gruppe, $flavour) )
        add_nodedata($id, 'Gruppe', $Gruppe, $flavour);
      echo '<b>&gt;&gt; '.LANG_TERMINUPDATE.'</b><br />';
      unset($document);
      global $flavour;
      $document = get_node($id, $flavour);
      sendeAllen($document, 'Termin�nderung durch '.get_user_email().' am '.date('d.m.Y H:i'));
    }
    if ( $_REQUEST['Bearbeiten'] == 1 && (inPerson($document['Person'], $document['author_id'],$document['Aendern']=='1') ||
         inGruppe($document['Gruppe'],-1,$document['Aendern']=='1'))) {
      // Termin bearbeiten
      include(INC_PATH.'misc/termineingabe.inc');
    }
    else if ( isset($_REQUEST['delete']) && $_REQUEST['delete'] == 1 &&
     (inPerson($document['Person'], $document['author_id'],$document['Aendern']=='1') ||
         inGruppe($document['Gruppe'],-1,$document['Aendern']=='1')) ) {
      sendeAllen($document, 'Termin von '.get_user_nickname().' am '.date('d-m-Y H:i').' gel�scht: ');
      destroy_node($id);
      remove_profile('editor_'.$id);
      $id = $document['parent_id'];
      echo '<b>Der Termin wurde gel�scht!</b><br />';
      echo '�bergeordnetes Dokument:<br />';
      $document = get_node($id, $flavour);
      echo '<a href="'.get_url($id).'">'.transform($document['title'], $document['datatype']['title']).'</a>';
    }
    else {
      if($document['title'] != '')
        echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
  KalenderScriptEinfuegen();
  ?>
  <table width="100%">
  <tr><td><b>Beginn</b></td><td><?=transform($document['Datum'], 'datetime')?>
  </td>
  <td><b>Ende</b></td><td><?=transform($document['Enddatum'], 'datetime')?>
  </td></tr>
  <tr valign="top"><td><b>Betroffen</b></td><td>
  <?php
    if ( $document["Person"] != "" ) {
      $wer = explode(",",$document["Person"]);
      foreach ( $wer as $key => $value) {
        echo transform($value,"user_id");
        if ( $value == SESSION_DBID )
          echo ' <a href="'.get_url($id,"Ok=1").'">(Termin best�tigen)</a>';
        echo "<br />";
      }
    }
    if ( $document["Gruppe"] != "" ) {
      $wer = explode(",",$document["Gruppe"]);
      echo "<i>";
      foreach ( $wer as $key => $value) 
      {
        if ( $value == -1 )
          echo "--alle--";
        else
          echo $groups[$value] . "<br />";
      }
      echo "</i>";
    }
  ?>
  </td>
  <td><b>Verantwortlich</b></td>
  <td><?=transform($document["author_id"],"user_id")?></td>
  </tr>
  <tr>
  <?php
    if ( $document["Ort"] != "" ) {
  ?>
    <td><b>Ort</b></td><td colspan="3">
    <?php if ( function_exists("OrtsName") && is_numeric($document['Ort']))
      echo OrtsName($document['Ort'], false);
    else echo transform($document['Ort'], 'clean'); ?>
    </td>
    <?php
    }
    echo '<td>';
    if ( $document["Turnus"] != "" ) echo "<b>Turnus</b>";
    echo '</td><td>';
    if ( $document["Turnus"] == "m" ) echo "monatlich";
    else if ( $document["Turnus"] == "w" ) echo "w�chentlich";
    else if ( $document["Turnus"] == "d" ) echo "t�glich";
    else if ( $document["Turnus"] == "j" ) echo "j�hrlich";
    else if ( $document["Turnus"] == "v" ) echo "14t�gig";
    echo '</td><td>';
    if ( $document["TurnusEnde"] != 0 )
      echo "<strong>Letzter Termin</strong></td><td>".date("d.m.Y",$document["TurnusEnde"]);
    echo '</td></tr>';
    echo '</table>';
    if($document['image'] != '' || $document['body'] != '') {
      echo '<p class="justify">';
      // Image
      echo transform($document['image'], $document['datatype']['image']);
      // Text
      echo transform($document['body'], $document['datatype']['body']).'</p>';
    }
    echo "<br />";
    if ( inGruppe($document["Gruppe"]) ||
         inPerson($document["Person"], $document["author_id"]) )
      echo '[ <a href="'.get_url($id,'Erinnerung=1').'">Erinnerung versenden</a> ] ';
    if ( inGruppe($document["Gruppe"],-1,$document["Aendern"]=="1") ||
         inPerson($document["Person"], $document["author_id"],$document["Aendern"]=="1") ) {
      echo '[ <a href="'.get_url($id,'Bearbeiten=1').'">Bearbeiten</a> ] ';
      echo '[ <a href="'.get_url($id,'delete=1').'">L�schen</a> ]';
      echo "<br />";
      echo "<br />";
    }
    $x = get_cmodule_id('terminerinnerung');
    if ( $x > 0 )
      echo ' [ <a href="'.get_url($x,'Termin_id='.$id).'">Erinnerung einrichten</a> ]';
    if(isset($parents)) {
      echo('[ <a href="'.get_url($document['parent_id'],'first_item='.$first_item).
        '">'.LANG_SHOW_COMPLETE_DOC.'</a> ]');
    }
  }
  }// unberechtigt
}
else { // Editor-Modus
  if( $docinput['submit'] == "Save" ) {
    save_termin($_REQUEST['docdata']);
  } // if Save
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Body','body','area_input');
  echo '<br /><br />';
  create_editor_input('Datum', 'Datum', "date_input");
  echo '<br /><br />';
  create_editor_input('Enddatum','Enddatum','date_input');
  echo '<br /><br />';
  echo "Betroffene Person(en)<br />";
?>
  <select name="docdata[Person][]" size="<?=
  $design['input_rows']?>" multiple="multiple">
<?php
  $User = search_users();
  foreach ($User as $key => $value) {
    echo '<option value="'.$key.'"';
    if ( in_array($key, explode(",",$document["Person"])) )
      echo ' selected="selected"';
    echo '>'.$value['nickname'].'</option>';
  }
?>
  </select><br /><br />
  Gruppe<br />
  <select name="docdata[Gruppe][]" size="<?=$design['input_rows']?>" multiple="multiple">
<?php
  foreach  ($groups as $key => $gruppe)  {
    echo '<option value="' . $key . '"';
    if ( in_array($key, explode(",",$document["Gruppe"])) )
        echo ' selected="selected"';
    echo '>';
    echo stripslashes($gruppe) . '</option>';
  }
  echo '</select>';
  echo '<br /><br />';
  create_editor_input('Betroffener Ort', 'Ort', "text_input");
  echo '<br /><br />';
  create_editor_input('Turnus (d/w/v/m/j)', 'Turnus', "text_input");
  echo '<br /><br />';
  create_editor_input('TurnusEnde', 'TurnusEnde', "date_input");
  echo '<br /><br />';
  create_editor_input('User comments at bottom','comments','onoff_input');
  echo '<br /><br />';
  create_editor_input('Alle User d�rfen �ndern','Aendern','onoff_input');
}
?>
