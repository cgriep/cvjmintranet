<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

echo '<?xml version="1.0" encoding="'.FLAVOUR_CHARSET.'"?>';
?>

<!DOCTYPE HTML>

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title><?=$page_title?></title>
	<meta name="format" content="text/xhtml" />
	<meta name="robots" content="index,follow" />
	<meta name="security" content="public" />
	<meta name="charset" content="<?=FLAVOUR_CHARSET?>" />
	<meta name="generator" content="adaptive website framework" />
	<meta name="description" content="<?=$meta_description?>" />
	<meta name="author" content="<?=$meta_author?>" />
	<meta name="keywords" content="<?=$meta_keywords?>" /><?php

	if(defined('URL_REWRITE') && URL_REWRITE == 1) { ?>

	<base href="<?=get_base_url()?>" />
<?php }
	include(BASE_PATH.INC_PATH."javascript.inc");
	include(BASE_PATH.INC_PATH."css.inc");
	?>

</head>