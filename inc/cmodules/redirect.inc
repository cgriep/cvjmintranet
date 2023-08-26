<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if($profile['editor']=='1') {
	echo '<p>';
	create_editor_input('Destination URL', 'redirect', 'text_input');
	echo '</p>';
	}
else {
        echo("<b>Sorry, this page should redirect you to other content....</b>");
        }
?>
