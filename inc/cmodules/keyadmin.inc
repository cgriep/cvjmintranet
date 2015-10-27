<?php

/* keys setzen */

if ( $profile['editor'] != 1 ) {
  echo '<h2>'.transform($document['title'], $document['datatype']['title']).
     '</h2>';
  if ( isset( $docdata['KeyID'] ))
    if ( is_numeric($docdata['KeyID']) )
      if ( is_numeric($docdata['OKeyID']) ) {
        // OKEy in Key &auml;ndern
        $sql = "UPDATE ".TABLE_NODES." SET key_id = ".$docdata["KeyID"];
        $sql .= " WHERE key_id = ".$docdata["OKeyID"];
        sql_query($sql);
        $anz = sql_affected_rows();
        if ( $anz > 0 )
          echo "<b> &gt;&gt;&gt;&nbsp;&nbsp;$anz Seiten geändert.</b>";
        else
          echo "<b> &gt;&gt;&gt;&nbsp;&nbsp;keine Seiten geändert.</b>";
        echo "<br /><br />";
      }
      else {
        echo "<b> &gt;&gt;&gt;&nbsp;&nbsp;Alter und neuer Key müssen numerisch angegeben werden.</b>";
      }

  echo '<form action="?id='.$id.'" method="post">';
  echo '<br /><br />';
  create_editor_input('Alter Key', 'OKeyID', 'text_input');
  echo '<br /><br />';
  create_editor_input('Neuer Key', 'KeyID', 'text_input');
  echo '<br /><br />';
  echo '<input type="Submit" value="Key &auml;ndern">';
  echo '</form>';
  echo '<br /><br />';
}

?>