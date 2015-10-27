<?php
// - refresh parent list
// param - 
/*
 if window.parent 
 window.parent.aktualisiereKorrespondenz ...
 redirection ...
 */

if ( ! isset($_REQUEST['file']) || $_REQUEST['file'] == '')
{
	throw new Exception('Keine Datei zum Anzeigen angegeben!');
}
header('Location: '.$file);

?>