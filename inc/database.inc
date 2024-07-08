<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: Wed Jun 17 21:42:34 CEST 2015
*/

define("DBMS","mysql");

function sql_query ($query) {
        try {
                $ergebnis = $GLOBALS["dbconnection"]->query($query);
        }
        catch (Exception)
        {
                $regebnis = false;
        }
        return $ergebnis;
}

function sql_affected_rows ($qresult = -1) {
        return mysqli_affected_rows($GLOBALS["dbconnection"]);
        }


function sql_num_rows ($qresult) {
	//if($qresult > 0) {
                return mysqli_num_rows($qresult);
        //        }
        //else {
        //        return 0;
        //        }
        }
 
function sql_fetch_row ($qresult) {
        if ( $qresult === null )
        {
                return false;
        }
        else
        {
                try {
                return mysqli_fetch_row($qresult);
                }
                catch (Exception)
                {
                        return false;
                }
        }
}

function sql_fetch_array ($qres) {
        return mysqli_fetch_assoc($qres);
        }
 
function sql_free_result ($qresult) {
        if ($qresult === null )
                return false;
        else 
                return mysqli_free_result($qresult);
}

function sql_insert_id ($qresult = -1) {
	return $GLOBALS['dbconnection']->insert_id;
	}

function sql_limit ($count, $offset = 0) {
	return("LIMIT $offset, $count");
	}

function sql_real_escape_string($string) {
        return $GLOBALS["dbconnection"]->real_escape_string($string);
}

// MySQL Database settings
$db_host        = "localhost";
$db_name        = "Verwaltung";
$db_username    = "xxx";
$db_password    = "xxx";
$db_table_prefix= "awf_";
 
if(function_exists("mysqli_connect")) {
        $dbconnection = mysqli_connect($db_host,$db_username,$db_password, $db_name);
        define("SQL_CONNECTION", "1");
        //mysqli_connect($db_host,$db_username,$db_password,$dbname);
        //mysql_select_db($db_name));
        }

?>