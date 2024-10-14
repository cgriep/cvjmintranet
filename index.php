<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 24.08.2003
*/
	include("inc/functions.inc");
	include("inc/licence.key");
	include("inc/sessions.inc");
	include("inc/caching.inc");
	include("inc/config.inc");
	include("inc/wysiwyg.inc");
	include(BASE_PATH.INC_PATH.INIT_INC);
	include(BASE_PATH.INC_PATH.DESIGN_INC);

    include(BASE_PATH.INC_PATH."html_head.inc"); 
?>
<body background="<?=$design['background']?>" bgcolor="<?=$design['bgcolor']?>"  link="<?=$design['linkcolor']?>" vlink="<?=$design['vlinkcolor']?>" text="<?=$design['textcolor']?>"> 

<?php
	include(BASE_PATH.INC_PATH.CONTENT_INC);
?>
</body>
</html>
<?php
	include(BASE_PATH.INC_PATH.SHUTDOWN_INC);

?>



