<?php
global $flavour;
global $document;
load_lang('counter');
$visitors = get_node(DEFAULT_DOCUMENT, $flavour, false);
$online_users = get_online_users();
echo LANG_VISITORS.' '.$visitors['views'].'<br />'.LANG_VISITORS_PAGE.' '.$document['views'].'<br />'.LANG_USERS_ONLINE.' '.count($online_users);
?>