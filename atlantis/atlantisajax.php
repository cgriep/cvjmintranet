<?php
/*
 * Registrierungsfunktionen für die AJAX-Schnittstelle
 * Created on 12.11.2006
 *
 * Christoph Griep
 */

require_once ("xajax/xajax.inc.php");

$xajax = new xajax("atlantis.server.php");
$xajax->registerFunction("zeigeFertigkeit");
$xajax->registerFunction("zeigeRasse");
$xajax->registerFunction("zeigeKlasse");
$xajax->registerFunction("zeigeSpruchliste");
$xajax->registerFunction("zeigeDaten");

?>