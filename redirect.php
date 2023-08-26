<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if(!isset($_GET['url']) || $_GET['url'] == '') { 
	echo 'Sorry, this is a redirection script. You can\'t expect any useful output.';
	exit(); 
	}

include('inc/database.inc');
if (!isset($_GET['referer']))
  $_GET['referer'] = ''; 
sql_query("UPDATE ".$db_table_prefix."redirect SET views=views+1 WHERE 
referer='".addslashes($_GET['referer'])."' AND url='".addslashes($_GET['url'])."'"); 
if(sql_affected_rows() < 1) {
	sql_query("INSERT INTO ".$db_table_prefix."redirect SET views=1, referer='".addslashes($_GET['referer'])."',
	url='".addslashes($_GET['url'])."'");
	if(sql_affected_rows() < 1) {
		sql_query("CREATE TABLE ".$db_table_prefix."redirect (id int not null auto_increment, 
		url text not null, referer text not null, views int default 1, primary key(id))");
		sql_query("INSERT INTO ".$db_table_prefix."redirect SET views=1,
			referer='".addslashes($_GET['referer'])."', url='".addslashes($_GET['url'])."'");
		}
	}
header("Location: ".$_GET['url']); // Redirect browser to $url

sql_close();

exit();

?>
