<?
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Liquid Installer
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

include('linstaller_lib.inc');

lbi_load_config ();

$lbi_details = lbi_get_details (base64_decode($_REQUEST['file']));

?> 
<html>
<head>
<title>View Details</title>
<link rel="stylesheet" type="text/css" href="admin.css" />

<?

function add_row ($key, $value) {
	global $design;
	?>
	<tr bgcolor="<?=$design['color_2']?>">
	<td width="20%" valign="top" align="right" nowrap="nowrap"><i><?=htmlentities($key)?>:</i></td>
	<td width="80%" valign="top" align="left"><?=$value?></td>
	</tr>
	<?
	}

?>

</head>

<body bgcolor="#ffffff" topmargin="0" rightmargin="0" leftmargin="0" marginwidth="0" marginheight="0">
<script language="javascript">
<!--
function open_window(url) {
	window.open(url, "<?=time()?>", "width=400,height=320,dependent=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=no,status=no,location=no");
	}
// -->
</script>
<table bgcolor="<?=$design['color_3']?>" width="100%" cellpadding="5" cellspacing="0" border="0"><tr><td>
<table width="100%" cellpadding="4" cellspacing="0" border="0">
<tr>
<td colspan="2" bgcolor="<?=$design['color_1']?>"><b>Author</b></td>
</tr>
<?
add_row('Name/Company',$lbi_details['author_name']);
add_row('Address',$lbi_details['author_address']);
add_row('eMail','<a href="mailto:'.$lbi_details['author_email'].'">'.$lbi_details['author_email'].'</a>');
add_row('Website','<a target="_blank" href="'.$lbi_details['author_website'].'">'.$lbi_details['author_website'].'</a>');
?>
<tr>
<td colspan="2" bgcolor="<?=$design['color_1']?>"><b>Software</b></td>
</tr>
<?
add_row('Name',$lbi_details['name']);
add_row('Version',$lbi_details['version']);
add_row('License',$lbi_details['license']);
add_row('Category',$lbi_details['category']);
add_row('Description',nl2br(htmlentities($lbi_details['description'])));
add_row('Dependencies',$lbi_details['dependencies']);
add_row('Download URL','<a target="_blank" href="'.$lbi_details['download_url'].'">'.$lbi_details['download_url'].'</a>');
add_row('Filename',base64_decode($_REQUEST['file']));
?>
</table>
</body>
</html>

