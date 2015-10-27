<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if(isset($search)) {
	$url = 'search.php?f='.$f.'&search='.$search.'&fldauthor='.$fldauthor.'&fldsubject='.$fldsubject.'&fldbody='.$fldbody.'&globalsearch='.$globalsearch.'&match='.$match.'&date='.$date;
	}
elseif(!isset($url)) { 
	$url = 'index.php';
	}
else {
	$url = urldecode($url).'&id='.$id;
	}

$dirname = dirname($_SERVER['PHP_SELF']).'/';
if($dirname == '//') $dirname = '/';

ob_start();
readfile("http://".$_SERVER['SERVER_NAME]'].$dirname."apps/phorum/".$url);
$htmlcontent = ob_get_contents();
ob_end_clean();

$htmlcontent = str_replace('&i=', '%26i=', $htmlcontent);
$htmlcontent = str_replace('&t=', '%26t=', $htmlcontent);
$htmlcontent = str_replace('&f=', '%26f=', $htmlcontent);
$htmlcontent = str_replace('&a=', '%26a=', $htmlcontent);
$htmlcontent = str_replace('&r=', '%26r=', $htmlcontent);
$htmlcontent = str_replace('&collapse=', '%26collapse=', $htmlcontent);
$htmlcontent = str_replace('.php?', '.php%3f', $htmlcontent);
$htmlcontent = str_replace('action="post.php"', 'action="apps/phorum/post.php"', $htmlcontent);
// $htmlcontent = str_replace('action="search.php"', 'action="index.php"', $htmlcontent);
echo ereg_replace('<a href="', '<a href="'.get_url($id, 'url='), $htmlcontent);
// echo("<br /><br />http://$SERVER_NAME".$dirname."phorum/".$url);
?>
<!-- BTW: Thanks for using AWF! ;-) Do you like the source? -->
