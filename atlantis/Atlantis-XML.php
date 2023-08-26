<?php


/**
 * Administrationsoberfl�che für die Atlantis-Daten
 * 2006 Christoph Griep
 */
include ('atlantis.db.php');
$template = 'Liste';

include ('atlantis.export.php');

$feld = array ();
if ($sql != '')
{
	if (isset ($_REQUEST['Sort']))
		$sort = $_REQUEST['Sort'];
	else
		$sort = '';
	header('Content-Type: text/xml');
	echo '<?xml version="1.0" encoding="ISO-8859-1"?>' . "\n";
	echo '<?xml-stylesheet href="atlantis' . $sort . '.xslt" type="text/xsl"?>' . "\n";
	//echo '<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">'."\n";
	echo '<'.$action.' stand="' . date('d.m.Y H:i') . '">' . "\n";
	if (!$query = sql_query($sql))
	{
		echo 'Fehler: ' . $sql . '/' . sql_error();
	}
	if ( $id == '' ) $id = $gruppe;
	if ( $id == '' ) $id = 'Element';
	while ($zeile = sql_fetch_array($query))
	{
		echo '<' . $id . '>' . "\n";
		foreach ($zeile as $key => $value)
		{
			if (!is_numeric($key) && $key != 'Bild')
			{
				echo '  <' . $key . '>' . htmlspecialchars($value) . '</' . $key . '>' . "\n";
			}
		}
		echo '</' . $id . '>' . "\n";
	}
	sql_free_result($query);
	echo '</'.$action.'>' . "\n";
}
?>
