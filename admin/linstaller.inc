<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Liquid Installer
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

include('linstaller_lib.inc');

lbi_load_config ();

function add_tab($name, $label) {
	global $design;
	global $page;

	if($page == $name) {
		$bg = $design['color_3'];
		$style = 'color: black;';
		}
	else {
		$bg = $design['color_4'];
		$style = 'color: white;';
		}

	?><td bgcolor="<?=$bg?>" width="1" background="img/edge_left.gif"><img src="img/pixel.gif" /></td><td
bgcolor="<?=$bg?>" align="center" nowrap="nowrap"><a style="<?=$style?>"
href="<?=$_SERVER['PHP_SELF'].'?page='.$name?>"><?=$label?></a></td><td bgcolor="<?=$bg?>" width="1"
background="img/edge_right.gif"><img src="img/pixel.gif" width="5" /></td><?php
	}

if(isset($_REQUEST['page'])) {
	$page = $_REQUEST['page'];
	}
else {
	$page = 'list';
	}

?> 

<script language="javascript">
<!--
function open_window(url) {
	window.open(url, "<?=time()?>", 
"width=400,height=320,dependent=yes,toolbar=no,menubar=no,scrollbars=yes,resizable=no,status=no,location=no");
	}
// -->
</script>
<table width="90%" cellpadding="0" cellspacing="0" border="0">
<tr><td align="left">
<?php
if(function_exists('domxml_open_mem') && version_compare("4.3.0", phpversion()) < 1) {
?>
<table width="100%" cellpadding="5" cellspacing="0" border="0"><tr>
<?php
add_tab('list', 'Install/Remove');
if(LBI_ENABLE_FILE_SHARING) {
	add_tab('share', 'Share');
	}
add_tab('files', 'File Library');
add_tab('create', 'Create new package');
add_tab('settings', 'Settings');
?>
<td bgcolor="#ffffff" width="100%"><img src="img/pixel.gif" width="1" /></td>
</tr>
</table>
<table bgcolor="<?=$design['color_3']?>" width="100%" cellpadding="9" cellspacing="0" border="0"><tr><td>
<?php 
/*
================================================================================================
Settings page
================================================================================================
*/

if($page == 'settings') { 
	if(isset($_REQUEST['settings'])) {

	$settings = $_REQUEST['settings'];	

	if(!isset($settings['delete_modified_files'])) $settings['delete_modified_files'] = '0';
	if(!isset($settings['overwrite_existing_files'])) $settings['overwrite_existing_files'] = '0';
	if(!isset($settings['execute_sql_commands'])) $settings['execute_sql_commands'] = '0';
	if(!isset($settings['execute_php_commands'])) $settings['execute_php_commands'] = '0';
	if(!isset($settings['enable_compression'])) $settings['enable_compression'] = '0';
	if(!isset($settings['enable_file_sharing'])) $settings['enable_file_sharing'] = '0';

	$fp = fopen(LBI_CONFIG_FILE, 'w');
	fwrite($fp, '<?xml version="1.0" encoding="UTF-8"?>'."\n".
		"<liquid_installer_config version=\"1.0\">\n".
		"\t<delete_modified_files>".$settings['delete_modified_files']."</delete_modified_files>\n".
		"\t<overwrite_existing_files>".$settings['overwrite_existing_files']."</overwrite_existing_files>\n".
		"\t<execute_sql_commands>".$settings['execute_sql_commands']."</execute_sql_commands>\n".
		"\t<execute_php_commands>".$settings['execute_php_commands']."</execute_php_commands>\n".
		"\t<enable_compression>".$settings['enable_compression']."</enable_compression>\n".
		"\t<enable_file_sharing>".$settings['enable_file_sharing']."</enable_file_sharing>\n".
		"\t<share_server>".$settings['share_server']."</share_server>\n".
		"\t<share_client>".$settings['share_client']."</share_client>\n".
		"\t<create_mask>".$settings['create_mask']."</create_mask>\n".
		"\t<base_path>".$settings['base_path']."</base_path>\n".
		"\t<package_path>".$settings['package_path']."</package_path>\n".
		"\t<product_name>".$settings['product_name']."</product_name>\n".
		"\t<product_version>".$settings['product_version']."</product_version>\n".
		"</liquid_installer_config>\n");

?>
<b>Settings saved.</b>
<?php
		}
	else {
	?>

<table bgcolor="<?=$design['color_2']?>" width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td>
<form enctype="multipart/form-data" action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="page" value="settings" />

<table width="50%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td>
<input type="checkbox" name="settings[delete_modified_files]" <?php
if(LBI_DELETE_MODIFIED_FILES == '1') echo 'checked="checked" ';
?>value="1" /> Delete modified files
</td><td>
<input type="checkbox" name="settings[overwrite_existing_files]" <?php 
if(LBI_OVERWRITE_EXISTING_FILES == '1') echo 'checked="checked" ';
?>value="1" /> Overwrite existing files
</td>
</tr>
<tr>
<td>
<input type="checkbox" name="settings[execute_sql_commands]" <?php 
if(LBI_EXECUTE_SQL_COMMANDS == '1') echo 'checked="checked" ';
?>value="1" /> Execute SQL commands
</td><td>
<input type="checkbox" name="settings[execute_php_commands]" <?php 
if(LBI_EXECUTE_PHP_COMMANDS == '1') echo 'checked="checked" ';
?>value="1" /> Execute PHP commands
</td>
</tr>
<tr>
<td>
<input type="checkbox" name="settings[enable_compression]" <?php 
if(LBI_ENABLE_COMPRESSION == '1') echo 'checked="checked" ';
?>value="1" /> Enable compression
</td><td>
<input type="checkbox" name="settings[enable_file_sharing]" <?php 
if(LBI_ENABLE_FILE_SHARING == '1') echo 'checked="checked" ';
?>value="1" /> Enable file sharing
</td>
</tr>
</table>
<br />
Create Mask<br />
<input maxlength="4" type="text" size="60" name="settings[create_mask]" value="<?=LBI_CREATE_MASK?>" />
<br />
<br />
Share Server<br />
<input type="text" size="60" name="settings[share_server]" value="<?=LBI_SHARE_SERVER?>" />
<br />
<br />
Share Client<br />
<input type="text" size="60" name="settings[share_client]" value="<?=LBI_SHARE_CLIENT?>" />
<br />
<br />
Product Name<br />
<input type="text" size="60" name="settings[product_name]" value="<?=LBI_PRODUCT_NAME?>" />
<br />
<br />
Product Version<br />
<input type="text" size="60" name="settings[product_version]" value="<?=LBI_PRODUCT_VERSION?>" />
<br />
<br />
Base Path<br />
<input type="text" size="60" name="settings[base_path]" value="<?=LBI_BASE_PATH?>" />
<br />
<br />
Package Path<br />
<input type="text" size="60" name="settings[package_path]" value="<?=LBI_PACKAGE_PATH?>" />
<br />
<br />

<input type="submit" name="submit" value="Save" />
<input type="reset" name="reset" value="Reset" />
</form>
</td>
</tr></table>
	<?php
		}
	}

/*
================================================================================================
File Library page
================================================================================================
*/

elseif($page == 'files') { 

if (is_uploaded_file($_FILES['package_file']['tmp_name']) && $_REQUEST['upload'] == 'true') {
	$file = file_get_contents($_FILES['package_file']['tmp_name']);
	if(!$dom = domxml_open_mem($file)) {
        	echo 'This is not a valid XML file.';
        } else {
		$root = $dom->document_element();

		if($root->node_name() == 'liquid_installer_package') {
			move_uploaded_file($_FILES['package_file']['tmp_name'], LBI_PACKAGE_PATH.$_FILES['package_file']['name']);
		//	echo $_FILES['package_file']['name']." was placed in your file library.<br /><br />";
			}
		}


	}
// else {
?>
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td bgcolor="<?=$design['color_1']?>"><b>Add</b></td>
</td>
</tr>
<tr>
<td bgcolor="<?=$design['color_2']?>">
<form enctype="multipart/form-data" action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="page" value="files" />
<input type="hidden" name="upload" value="true" />
<input type="hidden" name="MAX_FILE_SIZE" value="1000000">
<input name="package_file" type="file">
<input type="submit" value="Add package">
</form>	
</td></tr></table>
<br />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td bgcolor="<?=$design['color_1']?>"><b>Edit/Delete</b></td>
</td>
</tr>
<tr>
<td bgcolor="<?=$design['color_2']?>">
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="page" value="files" />
<input type="hidden" name="delete" value="true" />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr bgcolor="<?=$design['color_1']?>">
<td><b>Name</b></td>
<td><b>Version</b></td>
<td><b>Category</b></td>
<td><b>Author</b></td>
<td align="right"><b>Size</b></td>
<td align="center"><b>Edit</b></td>
<td align="center"><b>Delete</b></td>
</td>
</tr>
<?php

$lbi_files = lbi_get_package_list(LBI_PACKAGE_PATH);

while(list($key, $value) = each($lbi_files)) {
	if(isset($_POST[base64_encode($key)])) { unlink(LBI_PACKAGE_PATH.$key); }
        else {
	?><tr bgcolor="<?=$design['color_2']?>">
        <td><a href="javascript:open_window('linstaller_details.php?file=<?=base64_encode($key)?>')" title="View Details"><?=$value['name']?></a></td>
        <td><?=$value['version']?></td>
        <td><?=$value['category']?></td>
        <td><?=$value['author_name']?></td>
        <td align="right"><?=round(filesize(LBI_PACKAGE_PATH.$key) / 1024)?> KB</td>
        <td align="center"><a href="<?=$SERVER['PHP_SELF']?>?page=create&file=<?=base64_encode($key)?>">
	<img src="img/lbi_edit.gif" border="0" /></a></td>
        <td align="center"><input onclick="return confirm('Are you sure?')" type="submit" name="<?=base64_encode($key)?>" value="Delete" />
        </td>
        </tr><?php
	}
        }
?>
</table>
</form>
</td></tr></table>
<?php 
// }
}
elseif($page == 'share' && LBI_ENABLE_FILE_SHARING) {
?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="page" value="share" />
<input type="hidden" name="delete" value="true" />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr bgcolor="<?=$design['color_1']?>">
<td><b>Name</b></td>
<td><b>Version</b></td>
<td><b>Category</b></td>
<td><b>Author</b></td>
<td><b>License</b></td>
<td align="center"><b>Status</b></td>
</td>
</tr>
<?php

$lbi_files = lbi_get_package_list(LBI_PACKAGE_PATH);

while(list($key, $value) = each($lbi_files)) {
        if($value['license'] == 'GPL') {
	if(isset($_POST[base64_encode($key)])) {
		lbi_share($value['name'], $value['version']);
		}
	?><tr bgcolor="<?=$design['color_2']?>">
        <td><a href="javascript:open_window('linstaller_details.php?file=<?=base64_encode($key)?>')" title="View Details"><?=$value['name']?></a></td>
        <td><?=$value['version']?></td>
        <td><?=$value['category']?></td>
        <td><?=$value['author_name']?></td>
        <td><?=$value['license']?></td>
        <td align="center"><?php
		$is_shared = lbi_is_shared($value['name'], $value['version']);
		if($is_shared) {
			echo 'Shared';
			}
		else {
		        ?><input type="submit" name="<?=base64_encode($key)?>" value="Share" /><?php
			}
	?>
	</td>
        </tr><?php
	}
        }
?>
</table>
</form>
<b>Please note:</b> You can share files with GPL license only. This function requires, that your website is not password protected
and that a working share server is available.
<?php

} elseif($page == 'create') {	

function create_input ($name, $label) {
	global $package;
	?>
	<?=$label?><br />
	<input type="text" size="60" name="package[<?=$name?>]" value="<?=htmlentities($package[$name])?>" />
	<br />
	<br />
	<?	
	}

if(!isset($_REQUEST['package']['name']) || $_REQUEST['package']['name'] == '') {

if(isset($_REQUEST['file'])) {
	$package = lbi_get_details (base64_decode($_REQUEST['file']));
	}
elseif(isset($_REQUEST['package']['name'])) {
	$package = $_REQUEST['package'];
	}
else {
	$package = lbi_load_var('linstaller_defaults.db');
	}

?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="page" value="create" />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td bgcolor="<?=$design['color_1']?>"><b>Author</b></td>
</td>
</tr>
<tr>
<td bgcolor="<?=$design['color_2']?>">
<?php
create_input('author_name', 'Name/Company');
create_input('author_address', 'Address (street, zip, city, country)');
create_input('author_email', 'eMail');
create_input('author_website', 'Website URL (e.g. &quot;http://www.domain.com/&quot;)');
?>
</td></tr></table>
<br />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td bgcolor="<?=$design['color_1']?>"><b>Software description</b></td>
</td>
</tr>
<tr>
<td bgcolor="<?=$design['color_2']?>">
<?php
create_input('category', 'Category (e.g. &quot;Themes&quot;)');
create_input('name', 'Name (e.g. &quot;Dummy Software&quot;)');
create_input('version', 'Version (e.g. &quot;1.0&quot;)');
?>
Description<br />
<textarea name="package[description]" cols="60" 
rows="10"><?=htmlentities($package['description'])?></textarea>
<br />
<br />
<?php
create_input('dependencies', 'Dependencies');
create_input('download_url', 'Download URL');
create_input('license', 'License (e.g. &quot;GPL&quot;)');
?>
</td></tr></table>
<br />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td bgcolor="<?=$design['color_1']?>"><b>Files</b> (filenames must be comma separated, e.g. &quot;inc/functions.inc, 
inc/init.inc&quot;)</td>
</td>
</tr>
<tr>
<td bgcolor="<?=$design['color_2']?>">
<textarea name="package[files]" cols="60" rows="10"><?=htmlentities($package['files'])?></textarea>
</td></tr></table>
<br />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td bgcolor="<?=$design['color_1']?>"><b>SQL Commands</b></td>
</td>
</tr>
<tr>
<td bgcolor="<?=$design['color_2']?>">
Install<br />
<textarea name="package[sql_install]" cols="60" rows="10"><?=htmlentities($package['sql_install'])?></textarea>
<br />
<br />
Remove<br />
<textarea name="package[sql_remove]" cols="60" rows="10"><?=htmlentities($package['sql_remove'])?></textarea>
</td></tr></table>
<br />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
<td bgcolor="<?=$design['color_1']?>"><b>PHP Commands</b></td>
</td>
</tr>
<tr>
<td bgcolor="<?=$design['color_2']?>">
Install<br />
<textarea name="package[php_install]" cols="60" 
rows="10"><?=htmlentities($package['php_install'])?></textarea>
<br />
<br />
Remove<br />
<textarea name="package[php_remove]" cols="60" 
rows="10"><?=htmlentities($package['php_remove'])?></textarea>
</td></tr></table>
<br />
<input type="submit" value="Create package" />
</form>
</td></tr>
</table>
<?php
}
elseif($_REQUEST['package']['name'] != '' && $_REQUEST['package']['version'] != '' && $_REQUEST['package']['author_name'] != '') {

$package = $_REQUEST['package'];

lbi_save_var('linstaller_defaults.db', $package);

$basename = strtr(strtolower($package['name']), ' -/\()[]=?.;,!����', '______________aous');
$baseversion = strtr(strtolower($package['version']), ' -/\()[]=?;,!����', '................');
$filename = LBI_PACKAGE_PATH.$basename.'-'.$baseversion.'.lbi';

$fp = fopen($filename,'w');

fwrite($fp, '<?xml version="1.0" encoding="UTF-8"?>'."\n<liquid_installer_package version=\"1.0\">\n".
"<author_name>".$package['author_name']."</author_name>\n".
"<author_email>".$package['author_email']."</author_email>\n".
"<author_website>".$package['author_website']."</author_website>\n".
"<author_address>".$package['author_address']."</author_address>\n".
"<date>".time()."</date>\n".
"<category>".$package['category']."</category>\n".
"<version>".$package['version']."</version>\n".
"<dependencies>".$package['dependencies']."</dependencies>\n".
"<name>".$package['name']."</name>\n".
"<license>".$package['license']."</license>\n".
"<download_url>".$package['download_url']."</download_url>\n".
"<description>".$package['description']."</description>\n");

?>Added XML header and meta information<br /><?php

$files = explode(',', $package['files']);
while(list($key, $value) = each($files)) {
	$value = trim($value);
	if($value != '' && file_exists(BASE_PATH.$value)) {
	echo 'Added '.$value."<br />";
		lbi_add_file($fp, $value);
		}
	}

if($package['sql_install'] != '') {
	?>Added SQL install code<br /><?php
	lbi_add_code ($fp, 'sql_install', $package['sql_install']);
	}

if($package['sql_remove'] != '') {
	?>Added SQL remove code<br /><?php
	lbi_add_code ($fp, 'sql_remove', $package['sql_remove']);
	}

if($package['php_install'] != '') {
	?>Added PHP install code<br /><?php
	lbi_add_code ($fp, 'php_install', $package['php_install']);
	}

if($package['php_remove'] != '') {
	?>Added PHP remove code<br /><?php
	lbi_add_code ($fp, 'php_remove', $package['php_remove']);
	}

fwrite($fp, "</liquid_installer_package>\n");
fclose($fp);

?>
<b>Package created in <a href="<?=$filename?>"><?=$filename?></a></b>
<?php
}
}
elseif($page == 'list') {

if(is_dir(LBI_PACKAGE_PATH)) {

$lbi_files = lbi_get_package_list(LBI_PACKAGE_PATH);

?>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="page" value="list" />
<input type="hidden" name="install" value="true" />
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr bgcolor="<?=$design['color_1']?>">
<td><b>Name</b></td>
<td><b>Version</b></td>
<td><b>Category</b></td>
<td><b>Author</b></td>
<td align="center"><b>Action</b></td>
</td>
</tr>
<?php

$package_db = array();

$package_db = lbi_load_var('linstaller.db');

while(list($key, $value) = each($lbi_files)) {
	if($_POST['install'] == 'true') {
		if(isset($_POST[base64_encode($key)])) {
			if(!isset($package_db[$key])) {
				if(lbi_manage_package(LBI_PACKAGE_PATH.$key, 'install')) $package_db[$key] = true;		
				}
			else {
				if(lbi_manage_package(LBI_PACKAGE_PATH.$key, 'remove')) unset($package_db[$key]);
				}
			}
		}

	?><tr bgcolor="<?=$design['color_2']?>">
	<td><a href="javascript:open_window('linstaller_details.php?file=<?=base64_encode($key)?>')" title="View Details"><?=$value['name']?></a></td> 
	<td><?=$value['version']?></td>
	<td><?=$value['category']?></td>
	<td><?=$value['author_name']?></td>
	<td align="center"><?php
		if(isset($package_db[$key])) {
			?><input onclick="return confirm('Are you sure?')" type="submit" name="<?=base64_encode($key)?>" value="Remove" /><?php
			}
		else {
			?><input type="submit" name="<?=base64_encode($key)?>" value="Install" /><?php
			}
	?>
	</td>
	</tr><?php
	}

lbi_save_var('linstaller.db', $package_db);

?>
</table>
</form>
<?php
}
else {
	echo 'The directory <b>'.LBI_PACKAGE_PATH.'</b> does not exist. Check settings.';
	}
}
} 
else {
	echo 'DOM XML support and PHP >= 4.3.0 are required to use this feature.';
	}
?>
</td></tr>
</table>
<?php
if(isset($javascript_info)) {
?>
<script language="javascript">
<!--
alert('<?=$javascript_info?>');
// -->
</script>
<?php } ?>

