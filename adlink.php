<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

include('inc/database.inc');
  $queryres = sql_query("SELECT destination FROM ".$db_table_prefix."ads WHERE host='".$host."'");
  $row = sql_fetch_row($queryres); 
  if($host != '' && $row[0] != '') {
    sql_query("UPDATE ".$db_table_prefix."ads SET counter=counter + 1 WHERE host='".$host."'");
    header ("Location: ".$row[0]);
    exit;
    }
?>
