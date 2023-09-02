<?php

/* Gruppenmitglieder anzeigen */


if ( $profile['editor'] != 1 ) {
  if ( isset( $_REQUEST['UID'] ) && isset($_REQUEST['GID'] ))
    if ( is_numeric($_REQUEST['UID']) && is_numeric($_REQUEST['GID'])) {
      if ( isset($_REQUEST['UAdd'] ))
        add_user_to_group ($_REQUEST['UID'], $_REQUEST['GID'], false);
      else
        remove_user_from_group ($_REQUEST['UID'], $_REQUEST['GID'], false);
    }
  echo $design['table_begin'];
  $qres = sql_query ("SELECT id, group_name FROM ".TABLE_GROUPS." ORDER BY group_name");
  while($row = sql_fetch_row($qres)) { ?>
        <tr>
        <th colspan="3" bgcolor="<?=$design['table_row_dark']?>"><a id="R<?=$row[0]?>" name=""></a><?=$row[1]?></th></tr>
        <?php
                $sql = "SELECT user_id FROM ".TABLE_USERDATA." WHERE name = 'group_".$row[0].
                  "' AND value = '1'";
                $qres2 = sql_query($sql);
                $members = array();
                while ( $row2 = sql_fetch_row($qres2) )
                {
                  echo "<tr>";
                  echo "<td>".get_user_email($row2[0])."</td><td>". get_user_nickname($row2[0]);
                  echo '</td><td><a href="?id='.$id.'&UID='.$row2[0].'&GID='.$row[0].'#R'.
                  $row[0].'">Entfernen</a>';
                  echo "</td></tr>";
                  $members[] = $row2[0];
                }
                sql_free_result($qres2);
                echo '<tr><form action="?id='.$id.'#R'.$row[0].'" method="post"><td colspan="3">';
                echo '<input type="hidden" name="GID" value="'.$row[0].'">';
                echo '<select size="1" name="UID">';
                $sql = "SELECT DISTINCT user_id FROM ".TABLE_USERDATA.' INNER JOIN '.TABLE_USERS.' ON user_id='.TABLE_USERS.'.id '.
                "WHERE valid AND name='nickname' ORDER BY value";
                $qres2 = sql_query($sql);
                while ( $row2 = sql_fetch_row($qres2) )
                {
                  if ( ! in_array($row2[0], $members) )
                    echo '<option value="'.$row2[0].'">'.get_user_nickname($row2[0]).'</option>';
                }
                sql_free_result($qres2);
                echo '</select>';
                echo '<input type="Submit" name="UAdd" value="Hinzufügen">';
                echo '</tr>';
                echo '</form>';
  }
  echo $design['table_end'];
}
?>