<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

?>

<html>
<head>
<meta name="title" content="PHP Info">
<meta name="description" content="PHP configuration information.">
<meta name="sort_order" content="12">
<title>
Liquid Bytes AWF PHP Info
</title>
<?
        include('header.inc');
?>
<center>
<iframe src="phpinfo_iframe.php" width="95%" frameborder="1" 
align="center" height="400" 
name="phpinfo">
</iframe>
</center>
<?
	include('footer.inc');
?>
</body>
</html>
