<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

$image_size = getimagesize($value); 
?>
<script language="javascript">
<!--

window.open("<?=$value?>","<?=md5($value)?>","width=<?=$image_size[0]+20?>,height=<?=$image_size[1]+20?>,dependent=yes,toolbar=no,menubar=no,scrollbars=no,resizable=no,status=no,location=no");

// -->
</script>
