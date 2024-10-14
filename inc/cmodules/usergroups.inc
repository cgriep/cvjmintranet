<?php

/* Userdaten anzeigen */


if ( $profile['editor'] != 1 ) {
  if ( isset($_REQUEST["Sort"]) )
    $Sort = $_REQUEST["Sort"];
  else
    $Sort = "nickname";
  echo $design['table_begin'];
  echo '<tr bgcolor="'.$design['table_row_dark'].'">';
  echo '<th>ID</th>';
  echo '<th><a href="?id='.$id.'&Sort=nickname">Benutzer</a></th>';
  echo '<th><a href="?id='.$id.'&Sort=views">Seitenabrufe</a></th>';
  echo '<th><a href="?id='.$id.'&Sort=last_login">Letzter Login</a></th>';
  echo '<th>Gruppen</th>';
  echo '</tr>';
  $qres = sql_query ("SELECT id, views, DATE_FORMAT(last_login, '%Y-%m-%d %T') As lastlogin FROM ".TABLE_USERS);
  $user = array();
  while($row = sql_fetch_row($qres)) {
    $user[$row[0]]['views'] = $row[1];
    $user[$row[0]]['nickname'] = get_user_nickname($row[0]);
    $user[$row[0]]['last_login'] = $row[2];
    $user[$row[0]]['id'] = $row[0];
  }
  sql_free_result($qres);
  $sortarray = array();
  foreach ( $user as $key => $value) 
  {
    $sortarray[] = $value[$Sort];
  }
  array_multisort($sortarray, SORT_DESC, $user);
  foreach ($user as $uid => $value )
  {
    echo "<tr>";
    echo '<td align="right">'.$value["id"]."</td>";
    echo '<td>'.$value["nickname"]."</td>";
    echo '</td><td align="right">'.$value["views"];
    echo '</td><td>'.$value["last_login"];
    echo '</td><td align="right">';
    $drin = false;
    $prof = get_profile($value["id"], false);
    foreach ( $groups as $key => $value2 )
      if($prof['group_'.$key] == 1) {  echo "$value2 "; $drin = true; }
    if ( !$drin) echo "-ohne-";
    echo "</td></tr>";
  }
  echo $design['table_end'];
}
?>