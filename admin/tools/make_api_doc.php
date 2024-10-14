#!/usr/bin/php -q
<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

echo "\nThis script generates the documentation of the AWF API functions.\n";
echo "Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.\n\n";

$content = file('../../inc/functions.inc');

$fp = fopen('../../docs/api/index.html', 'w');
$fp2 = fopen('../../docs/api/functions.html', 'w');

fwrite($fp, '
<html><head>
<link rel="stylesheet" type="text/css" href="api.css" />
<title>AWF API functions</title></head><body bgcolor="#ffffff">
<h1>AWF API functions</h1>
<ul>
');

fwrite($fp2, '
<html><head>
<link rel="stylesheet" type="text/css" href="api.css" />
<title>AWF API functions</title></head><body bgcolor="#ffffff">
<h1>AWF API functions</h1>
<pre>
');

while(list($key, $value) = each($content)) {
	if(strpos($value, 'unction') == 1) {
		$functions[$key] = trim(substr(substr($value, 9),0,strpos(substr($value, 9), ' ')));
		$args[$key] = trim(substr(substr($value, 9),strpos(substr($value, 9), ' ')));
		fwrite($fp2, '<a name="'.$functions[$key].'">function <b>'.$functions[$key].'</b> '.$args[$key]."\n");
		list($key2, $value) = each($content);
		$desciptions[$key] = trim(substr(strrchr($value, '/'), 1));
		}
	else {
		fwrite($fp2, htmlentities($value));
		}
	}

natsort($functions);

while(list($key, $value) = each($functions)) {
	fwrite($fp, '<li><a href="functions.html#'.$value.'">'.$value.'</a>&nbsp;&nbsp;--&nbsp;&nbsp;'.$desciptions[$key].'</li>');
	}

echo "Wrote ".count($functions)." lines.\n\n";

fwrite($fp, '</ul></body></html>');
fwrite($fp2, '</pre></body></html>');
fclose($fp);
fclose($fp2);
?>
