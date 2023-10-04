<?php

require_once ('php/xajax/xajax.inc.php');

$xajax = new xajax('cvjm_server.php');
$xajax->registerFunction('erhoeheRechnungsMenge');
$xajax->registerFunction('senkeRechnungsMenge');

// Buchungsübersicht 
$xajax->registerFunction('fuegeArtikelHinzu');
$xajax->registerFunction('entferneArtikel');
$xajax->registerFunction('gesamterBereich');
$xajax->registerFunction('gesamteReise');
$xajax->registerFunction('loescheKorrespondenz');
$xajax->registerFunction('setzeAdressKategorie');
$xajax->registerFunction('zeigeKategorien');
$xajax->registerFunction('zeigeSeminarKorrespondenz');
$xajax->registerFunction('sendeKorrespondenzMail');
$xajax->registerFunction('aendereZuordnung');
$xajax->registerFunction('sucheArtikel');
$xajax->registerFunction('moveArtikel');
$xajax->registerFunction('aendereArtikelAnzahl');
$xajax->registerFunction('zeigeArtikelNachBarcode');
$xajax->registerFunction('tragePersonEin');
$xajax->registerFunction('createEAN');
$xajax->registerFunction('showKundenAuswahl');
$xajax->registerFunction('sucheKunden');
$xajax->registerFunction('zeigePersonenwahlDialog');
$xajax->registerFunction('schliessePersonenwahlDialog');
$xajax->registerFunction('showInstitutionMail');
$xajax->registerFunction('aktualisiereKorrespondenz');

?>
