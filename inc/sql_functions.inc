<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 17.05.2004
*/


function sql_insert ($table, $values, $debug = FALSE) {
	if(!is_array($values) || count($values) < 1) return FALSE;

	$sql_query = 'INSERT INTO '.$table.' SET ';
	
	$pre = '';

	while(list($key, $value) = each($values)) {
		if($value !== NULL) {
			$sql_query .= $pre."$key = '".addslashes($value)."'";
			$pre = ',';
			}
		}

	if($debug) echo($sql_query."\n");

	$qres = sql_query($sql_query);

	return sql_insert_id($qres);
	}

function sql_update ($table, $values, $where) {
	if(!is_array($values) || count($values) < 1) return FALSE;

	$sql_query = 'UPDATE '.$table.' SET ';
	
	$pre = '';

	while(list($key, $value) = each($values)) {
		if($value !== NULL) {
			$sql_query .= $pre."$key = '".addslashes($value)."'";
			$pre = ',';
			}
		}

	if($where != '') {
		$sql_query .= ' WHERE '.$where;
		}

	$qres = sql_query($sql_query);
	
	return sql_affected_rows($qres);
	}

function sql_result_array ($qres, $primary_key) {
	if(sql_num_rows($qres) == 0) return FALSE;

	while($row = sql_fetch_array($qres)) {
		while(list($key, $value) = each($row)) {
			$result[$row[$primary_key]][$key] = stripslashes($value);
			}
		}

	sql_free_result($qres);

	return $result;
	}

function sql_query_array ($sql_query, $primary_key) {
	if($sql_query == '' || $primary_key == '') return FALSE;
 
	$qres = sql_query($sql_query);

	return sql_result_array ($qres, $primary_key);
	}

function sql_get_single_row ($table, $where) {
	if($table == '' || $where == '') return FALSE;

	$qres = sql_query("SELECT * FROM $table WHERE $where LIMIT 1");
	if(sql_num_rows($qres) == 1) {
		$row = sql_fetch_array($qres);
		sql_free_result($qres);
		while(list($key, $value) = each($row)) {
                        $result[$key] = stripslashes($value);
                        }
		return $result;
		}
	else {
		return FALSE;
		}
	}

function sql_delete ($table, $where) {
	if($table == '' || $where == '') return FALSE;
	
	$qres = sql_query("DELETE FROM $table WHERE $where");
 
	if(sql_affected_rows($qres) > 0) {
		return TRUE;
		}
	else {
		return FALSE;
		}
	}
?>
