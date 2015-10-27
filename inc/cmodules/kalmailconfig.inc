<?php
  echo "fjffjfjfkjfkjh";
  if ( isset($_REQUEST["SaveMail"] ) )
  {
    while ( list($key, value) = each($_REQUEST) )
      if ( substr($key, 0, 4) == "grp_" ) {
        $key = substr($key, 5, strlen($key));
        if ( is_numeric($key) )
        {
          if ( $value == "1" )
            set_profile("Kal".$id."MailGrp".$key, 1);
          else
            remove_profile("Kal".$id."MailGrp".$key);
        }
      }
  }
  else
  {
    echo '<form action="?id=<?=$id?>" method="post">';
    echo '<b>Mail-Konfiguration</b><br />';
    echo 'Für folgende Gruppen können bei Änderungen Benachrichtigungen versendet werden:<br />';
    echo '<input type="hidden" name="SaveMail" value="1" />';
    // alle Gruppen anzeigen
    while ( list($key, $value) = each($groups) ) {
      if ( $profile["group_".$key] == "1" )
        echo '<input type="checkbox" name="grp_'.$key.'" ';
        if ( $profile["Kal".$id."MailGrp$key"] == 1 ) echo 'checked="checked" ';
        echo '/> '.$value."<br />";
    }
    echo '<input type="Submit" name="" value="Sichern">';
    echo '</form><hr />';
  } // Else
?>