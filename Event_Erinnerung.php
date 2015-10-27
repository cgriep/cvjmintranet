<?php
/*
 * Created on 16.05.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */

/* Datenbank- und AWF-Funktionalitaet einbinden */
echo date('d.m.Y H:i').' - start ';
include ('inc/functions.inc');
include ('inc/licence.key');
//include ('inc/sessions.inc');
//include ('/srv/www/htdocs/inc/caching.inc');
include ('inc/config.inc');
include_once('inc/misc/CVJM.inc');
require('inc/cvjm/Event.class.php');
Event::sendeBestaetigungsmails();
echo '-fertig.'."\n";
?>
