<?php

include("inc/functions.inc");
include("inc/licence.key");
include("inc/sessions.inc");
include("inc/caching.inc");
include("inc/config.inc");
include("inc/wysiwyg.inc");

include(BASE_PATH.INC_PATH.INIT_INC);
include(BASE_PATH.INC_PATH.DESIGN_INC);

include (INC_PATH . 'misc/CVJM.inc');
require_once (INC_PATH.'Smarty/Smarty.class.php');
include_once (INC_PATH . 'cvjm/Adresse.class.php');
include_once (INC_PATH . 'cvjm_ajax.php');
$xajax->printJavascript('./javascript');

$Smarty = new Smarty;
$Smarty->template_dir = '/var/www/httpdocs/templates';
$Smarty->assign('id', 1);
$Smarty->assign('paramsOhneEdit', 'nix');
$Smarty->display('Adressen_Liste.tpl');

?>