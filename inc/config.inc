<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

error_reporting(1); // Disable Warnings
set_magic_quotes_runtime(0); // Disable magic_quotes_runtime

define('AWF_VERSION', '110');

// Establish DB connection
require('inc/database.inc');

// Tell the user, if the DB connection is broken...
if(!defined('SQL_CONNECTION') || SQL_CONNECTION == false) { 
	// show error page
	include('inc/data/errors/db_error.inc'); 
	// log error
	error_log($_SERVER['REMOTE_ADDR']."\t".time()."\t".$GLOBALS['id']."\t$db_host\t$db_name\n", 
			3, 'log/'.date("Y_m").'_dberror.log');
	// bye
	exit(); 
	}

// define tables names
require('inc/db_tables.inc');

// Read all constants from database
$qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP); 
while ($row = sql_fetch_row($qresult)) {
	define(strtoupper($row[0]),$row[1]);
	}
sql_free_result($qresult);

// set error handler
set_error_handler('awf_error_handler');

// Send HTTP content type
header('Content-Type: text/html; charset='.CHARSET);

// Basic settings
$image_rpath	  	= IMAGE_RPATH;
$image_path		= BASE_PATH.IMAGE_RPATH;
$site_title		= SITE_TITLE;
$default_document	= DEFAULT_DOCUMENT;
$default_flavour	= DEFAULT_FLAVOUR;
$default_design 	= DEFAULT_DESIGN;
$count_views 		= COUNT_VIEWS;
$cache_time  		= CACHE_TIME;
$webmaster_mail 	= WEBMASTER_MAIL;

// i18n
if(!defined("DEFAULT_LANG")) {
	define("DEFAULT_LANG",'en');
	define("LANG_PATH", INC_PATH.'lang/');
	}
if(!isset($lang)) $lang = DEFAULT_LANG;

define('CONFIG_CREATE_MASK', 0707);

if (SHOW_WARNINGS == 1) error_reporting(4);

if(file_exists(BASE_PATH.INC_PATH.'custom/custom_config.inc')) include(BASE_PATH.INC_PATH.'custom_config.inc');

?>
