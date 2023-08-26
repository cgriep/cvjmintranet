<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

define('URL_REWRITE', '1');

$redirect = false;

$default = 'index.html';

$pathinfo   = pathinfo($_SERVER['REQUEST_URI']);
$s_pathinfo = pathinfo($_SERVER['SCRIPT_NAME']);

if($pathinfo['dirname'] == '/') $pathinfo['dirname'] = '';
if($s_pathinfo['dirname'] == '/') $s_pathinfo['dirname'] = '';

$exploded_name = explode('.', $pathinfo['basename']);

if(is_numeric($exploded_name[0])) {
	$id = $exploded_name[0];
	}
elseif($exploded_name[0] != 'index') {
        $redirect = file('inc/data/redir_map.conf');
        foreach ( $redirect as $key => $value) {
                if($s_pathinfo['dirname'].'/'.strtok($value,"\t") == $_SERVER['REQUEST_URI']) {
                        header("Location: ".$s_pathinfo['dirname'].'/'.trim(strstr($value,"\t")));
                        exit();
                        }
                }
        if(isset($default)) header("Location: ".$s_pathinfo['dirname'].'/'.$default);
        exit();
	}

parse_str(substr(strstr($pathinfo['extension'], '?'),1), $parsed_get);

if(isset($parsed_get['id'])) {
	$id = $parsed_get['id'];
	$new_url = $pathinfo['dirname'].'/'.$id.'.html';

	if(count($parsed_get) > 1) {
		$exploded_params = array_unique(explode('&',substr(strstr($pathinfo['extension'], '?'),1)));
	
		$new_param = '';
		$pre = '?';
		while(list($key, $param) = each($exploded_params)) {
			if(substr($param, 0, 3) != 'id=') {
				$new_param .= $pre.$param;
				$pre = '&';
				}
			}
		}
	$redirect = true;
	$new_url = $new_url.$new_param;
	}
elseif(isset($_POST['id']) && is_numeric($_POST['id']) && $_POST['id'] != $id) {
	$new_url = $pathinfo['dirname'].'/'.$_POST['id'].'.html';
	$redirect = true;
	}
else {
	$new_url = $_SERVER['REQUEST_URI'];
	}

if($redirect) { 
	header('Location: '.$new_url); 
	exit(); 
	}
else { 
	if(isset($id)) {
		$_SERVER['PHP_SELF'] = $pathinfo['dirname'].'/'.$id.'.html';
		}
	else {
		$_SERVER['PHP_SELF'] = $pathinfo['dirname'].'/'.$default;
		}
	include('index.php'); 
	}
?>
