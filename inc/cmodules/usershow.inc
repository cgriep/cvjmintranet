<?php

/* Userdaten anzeigen */


if ( $profile['editor'] != 1 ) {
  if ( is_numeric($_REQUEST["uid"]) ) {
    // Nodes anzeigen
    $sql = "SELECT node_id FROM ".TABLE_NODEDATA." WHERE name='author_id' AND value='".$_REQUEST["uid"]."'";
    $qu = sql_query($sql);
    echo "<b>Seiten von ".get_user_nickname($_REQUEST["uid"])."</b><br />";
    $anz = 0;
    while ( $row = sql_fetch_array($qu) ) {
      $no = get_nodedata($row['node_id'], $flavour);
      echo '<a href="?id='.$row['node_id'].'">'.transform($no['title'],$no['datatype']['title']).'</a><br />';
      $anz++;
    }
    echo "<br /><br />";
    echo $anz." Seiten wurden von ".get_user_nickname($_REQUEST["uid"])." erstellt.<br /><br />";
    sql_free_result($qu);
  }
  if ( isset($_REQUEST["Sort"]) )
    $Sort = $_REQUEST["Sort"];
  else
    $Sort = "last_login";
  echo $design['table_begin'];
  echo '<tr bgcolor="'.$design['table_row_dark'].'">';
  echo '<th>ID</th>';
  echo '<th><a href="?id='.$id.'&Sort=nickname">Benutzer</a></th>';
  echo '<th><a href="?id='.$id.'&Sort=views">Seitenabrufe</a></th>';
  echo '<th><a href="?id='.$id.'&Sort=last_login">Letzter Login</a></th>';
  echo '<th><a href="?id='.$id.'&Sort=Anzahl">Erstellte Seiten</a></th>';
  echo '</tr>';
  $qres = sql_query ("SELECT id, views, DATE_FORMAT(last_login, '%Y-%m-%d %T') As lastlogin FROM ".TABLE_USERS);
  $user = array();
  while($row = sql_fetch_row($qres)) {
    $sql = "SELECT COUNT(*) FROM ".TABLE_NODEDATA." WHERE name = 'author_id' AND value = '".$row[0]."'";
    if ( $qres2 = sql_query($sql) )
    {
      $user[$row[0]]['views'] = $row[1];
      $user[$row[0]]['nickname'] = get_user_nickname($row[0]);
      $user[$row[0]]['last_login'] = $row[2];
      $user[$row[0]]['id'] = $row[0];
      $rr = sql_fetch_row($qres2);
      $user[$row[0]]['Anzahl'] = $rr[0];
    }
    sql_free_result($qres2);
  }
  sql_free_result($qres);
  $sortarray = array();
  foreach ($user as $key => $value) 
  {
    $sortarray[] = $value[$Sort];
  }
  array_multisort($sortarray, SORT_DESC, $user);
  foreach ( $user as $uid => $value) {
    echo "<tr>";
    echo '<td align="right">'.$value['id']."</td>";
    echo '<td><a href="?id='.$id.'&uid='.$value['id'].'">'.$value['nickname']."</a></td>";
    echo '</td><td align="right">'.$value['views'];
    echo '</td><td>'.$value[last_login];
    echo '</td><td align="right">'.$value['Anzahl'];
    echo "</td></tr>";
  }
  echo $design['table_end'];
}
?>