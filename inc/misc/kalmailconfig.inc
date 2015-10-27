<?php
  if ( $_REQUEST["Config"] == "2" )
  {
    while ( list($key, $value) = each($groups) ) {
      if ( $profile["group_".$key] == "1" )
        if ( $_REQUEST["grp_$key"] == "1" )
          set_profile("Kal".$id."MailGrp".$key, 1);
        else
          remove_profile("Kal".$id."MailGrp".$key);
    }
    reset($groups);
  }
  else
  {
    echo '<form action="'.get_url($id).'" method="post">';
    echo '<b>Mail-Konfiguration</b><br />';
    echo 'Für folgende Gruppen sollen bei Änderungen Benachrichtigungen versendet werden:<br />';
    echo '<input type="hidden" name="Config" value="2" />';
    // alle Gruppen anzeigen
    while ( list($key, $value) = each($groups) ) {
      if ( $profile["group_".$key] == "1" ) {
        echo '<input type="checkbox" name="grp_'.$key.'" value="1" ';
        if ( $profile["Kal".$id."MailGrp$key"] == 1 ) echo 'checked="checked" ';
        echo '/> '.$value."<br />";
      }
    }
    reset($groups);
    echo '<input type="Submit" name="" value="Sichern">';
    echo '</form><hr />';
  } // Else
?>