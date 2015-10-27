<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 20.09.2003

        ToDo: Use PHP sessions
*/

if(!isset($id) ) {
  if  (isset($_REQUEST['id']))
        $id  = $_REQUEST['id'];
  else
     $id = '';
        }
if ( isset($_REQUEST['first_item']))
$first_item  = $_REQUEST['first_item'];
  else 
  $first_item = '';
if ( isset($_REQUEST['action']))
$action             = $_REQUEST['action'];
else
  $action = '';
if ( isset($_REQUEST['docinput']))
$docinput    = $_REQUEST['docinput'];
else
$docinput = array();

if ( isset($_GET[session_name()]))
{
  session_id($_GET[session_name()]);
}
session_start();

if(isset($docinput['user_id'])) {
        $_SESSION[md5(LICENCE_KEY.'id')] = $docinput['user_id'];
//        setcookie (md5(LICENCE_KEY.'id'), $docinput['user_id'], 0);
        $session_userid = $docinput['user_id'];
        }
elseif(isset($_SESSION[md5(LICENCE_KEY.'id')])) {
//elseif(isset($_COOKIE[md5(LICENCE_KEY.'id')])) {
        //$session_userid = $HTTP_COOKIE_VARS[md5(LICENCE_KEY.'id')];
        $session_userid = $_SESSION[md5(LICENCE_KEY.'id')];
        }
elseif(isset($_REQUEST['session_userid'])) {
        $session_userid = $_REQUEST['session_userid'];
        }

if(isset($docinput['passwd2'])) {
//        setcookie (md5(LICENCE_KEY.'passwd'), md5($docinput['passwd2'].$_SERVER['REMOTE_ADDR']), 0);
        $_SESSION[md5(LICENCE_KEY.'passwd')] = md5($docinput['passwd2']); //.$_SERVER['REMOTE_ADDR']);
        $session_passwd = md5($docinput['passwd2']); // .$_SERVER['REMOTE_ADDR']);
        }
//elseif(isset($_COOKIE[md5(LICENCE_KEY.'passwd')])) {
elseif(isset($_SESSION[md5(LICENCE_KEY.'passwd')])) {
//        $session_passwd = $_COOKIE[md5(LICENCE_KEY.'passwd')];
        $session_passwd = $_SESSION[md5(LICENCE_KEY.'passwd')];
        }
elseif(isset($_REQUEST['session_passwd'])) {
        $session_passwd = $_REQUEST['session_passwd'];
        }

if(!isset($_SESSION[md5(LICENCE_KEY.'session')])) {
  if ( isset($_SERVER['REMOTE_ADDR']) && isset($_SERVER['HTTP_HOST']))
  {	
        $session_id = md5(time().$_SERVER['REMOTE_ADDR'].$_SERVER['HTTP_HOST'].LICENCE_KEY);
        $_SESSION[md5(LICENCE_KEY.'session')] = $session_id;
  }
  else
    $session_id = -1;
}
else {
//        $session_id = $_COOKIE[md5(LICENCE_KEY.'session')];
        $session_id = $_SESSION[md5(LICENCE_KEY.'session')];
        }

define('SESSION_UID', $session_id);

unset($design);

if(isset($docinput['design'])) {
        if($docinput['design'] != 'Printable') {
//                setcookie (md5(LICENCE_KEY.'design'), $docinput['design'], 0);
                $_SESSION[md5(LICENCE_KEY.'design')] = $docinput['design'];
                }
        $design['name'] = $docinput['design'];
        }
//elseif(isset($_COOKIE[md5(LICENCE_KEY.'design')])) {
elseif(isset($_SESSION[md5(LICENCE_KEY.'design')])) {
//        $design['name'] = $_COOKIE[md5(LICENCE_KEY.'design')];
        $design['name'] = $_SESSION[md5(LICENCE_KEY.'design')];
        }
else {
        $design['name'] = '';
        }

unset($lang);

if(isset($docinput['lang'])) {
//        setcookie (md5(LICENCE_KEY.'lang'), $docinput['lang'], 0);
        $_SESSION[md5(LICENCE_KEY.'lang')] = $docinput['lang'];
        $lang = $docinput['lang'];
        }
//elseif(isset($_COOKIE[md5(LICENCE_KEY.'lang')])) {
elseif(isset($_SESSION[md5(LICENCE_KEY.'lang')])) {
//        $lang = $_COOKIE[md5(LICENCE_KEY.'lang')];
        $lang = $_SESSION[md5(LICENCE_KEY.'lang')];
        }

unset($flavour);

if(isset($docinput['flavour'])) {
//        setcookie (md5(LICENCE_KEY.'flavour'), $docinput['flavour'], 0);
        $_SESSION[md5(LICENCE_KEY.'flavour')] = $docinput['flavour'];
        $flavour = $docinput['flavour'];
        }
//elseif(isset($_COOKIE[md5(LICENCE_KEY.'flavour')])) {
elseif(isset($_SESSION[md5(LICENCE_KEY.'flavour')])) {
//        $flavour = $_COOKIE[md5(LICENCE_KEY.'flavour')];
        $flavour = $_SESSION[md5(LICENCE_KEY.'flavour')];
        }
else {
        $flavour = '';
        }

if(isset($docinput['logout']) && $docinput['logout'] == 'true') {
//        setcookie (md5(LICENCE_KEY.'id'), "", 0);
//        setcookie (md5(LICENCE_KEY.'passwd'), "", 0);
        $logout_userid = $session_userid;
        $logout_passwd = $session_passwd;
        session_unset();
        session_destroy();
        $session_userid = '';
        $session_passwd = '';
        }

?>
