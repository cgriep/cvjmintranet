<?
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.12.2001
        Parameter docinput[AlleAn] = 0/1 schaltet alle auf den entsprechenden Wert
*/

  load_lang('newsletter');
  if($profile['editor']!='1') {
    if ( isset($docinput["Save"]) )
    {
      echo "&gt;&gt;&gt; Speichere Daten ...<br />";
      while ( list($key, $value) = each($docinput) )
        if ( substr($key,0,1) == "U" && is_numeric(substr($key, 1)) )
          if ( isset($docinput["AlleAn"] ) && is_numeric($docinput["AlleAn"]) )
            set_profile("newsletter", $docinput["AlleAn"], substr($key, 1));
          else
          set_profile("newsletter", $value, substr($key, 1));
    }
    if($document['title'] != '') {
      echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
    }
    $que = sql_query("SELECT id FROM " . TABLE_USERS." WHERE valid");
    echo '<form action="'.get_url($id).'" method="post">';
    echo '<input type="Submit" name="docinput[Save]" value="Speichern">';
    if ( $profile["SendToAll"] == 1 )
     echo 'alle einschalten <input type="Checkbox" name="docinput[AlleAn]" value="1"><br />';
    echo '<table border="1">';
    echo '<tr><th>Name</td><th>eMail</td><th>Newsletter</td></tr>';
    while ( $u = sql_fetch_row($que) )
    {
      $prof = get_profile($u[0]);
      $eMail = get_user_email($u[0]);
      echo '<tr><td>'.get_user_nickname($u[0]).'</td><td>'.$eMail.'</td><td>';
      if ( $prof['newsletter'] == 1 ) {
        if ( isemail($eMail) ) {
          echo '<input type="radio" value="1" name="docinput[U'.$u[0].']" checked="checked" /> '.LANG_YES;
          echo '&nbsp;&nbsp;';
          echo '<input type="radio" value="0" name="docinput[U'.$u[0].']" /> '.LANG_NO;
          //echo '<a href="?id='.$id.'&Rem='.$u[0].'">abbestellen</a>';
        }
        else
          echo "<b>".LANG_UNGUELTIGE_MAIL."</b>";
      }
      else
      {
        echo '<input type="radio" value="1" name="docinput[U'.$u[0].']" /> '.LANG_YES;
        echo '&nbsp;&nbsp;';
        echo '<input type="radio" value="0" name="docinput[U'.$u[0].']" checked="checked" /> '.LANG_NO;
        //echo '<a href="?id='.$id.'&Add='.$u[0].'">bestellen</a>';
      }
      echo "</td></tr>";
    }
    sql_free_result($que);
    echo "</table>";
    echo '<input type="Submit" name="docinput[Save]" value="Speichern">';
    echo '</form>';
  }
  elseif($profile['editor']=='1') {
    // Image Input
    create_editor_input('Image','image','image_input');
    echo '<br /><br />';
    // Text Input
    create_editor_input('Body','body','area_input');
    echo '<br /><br />';
  }
?>