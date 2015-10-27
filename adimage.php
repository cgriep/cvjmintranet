<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

unset($db_table_prefix);
include('inc/database.inc');
  $queryres = sql_query("SELECT banner FROM ".$db_table_prefix."ads WHERE host='".addslashes($_REQUEST['host'])."'");
  $row = sql_fetch_row($queryres);
  if($host != '' && $row[0] != '') {
    sql_query("UPDATE ".$db_table_prefix."ads SET views=views + 1 WHERE host='".addslashes($_REQUEST['host'])."'");
    header ("Location: ".$row[0]);
    exit;
    }
?>

