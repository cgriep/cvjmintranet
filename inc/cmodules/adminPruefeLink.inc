<?php
/*
   AWF-Modul Link-Test

   adminLinkTest.inc
   AWF-Name: adminLinkTest

*/

function pruefeLink($id, $link)
{
  if ( is_numeric($id) && trim($link) != "" &&
       strtolower(substr($link, 0, 4)) == "http" )
  {
    echo "Prüfe Seite $id ($link): ";
    if ( @file_get_contents(trim($link)) === false )
      echo '<span style="background-color:red">offline</span>';
    else
      echo '<span style="background-color:green">online</span>';
    echo "<br />";
    flush();
  }
}

if ( $profile["editor"] != 1 )
{
  echo "Prüfe Links...<br />";
  $sql = "SELECT * FROM ".TABLE_NODES." INNER JOIN ".TABLE_NODEDATA.
    " ON node_id=".TABLE_NODES.".id WHERE type_id=".$doctypes["file_para"].
    " AND name='Link' AND value <>''";
  $query = sql_query($sql);
  while ( $node = sql_fetch_array($query) )
  {
    pruefeLink($node["node_id"], $node["value"]);
  }
  sql_free_result($query);

  $query = sql_query("SELECT * FROM ".TABLE_NODES." INNER JOIN ".TABLE_NODEDATA.
    " ON node_id=".TABLE_NODES.".id WHERE name='redirect'");
  while ( $node = sql_fetch_array($query) )
  {
    pruefeLink($node["node_id"], $node["value"]);
  }
  sql_free_result($query);
  echo "Alle Links geprüft.";
}

?>