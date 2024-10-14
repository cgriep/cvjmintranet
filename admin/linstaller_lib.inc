<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Liquid Installer
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

define('LBI_CONFIG_FILE', 'linstaller.cfg');

$design['color_1'] = '#B9E3FB';
$design['color_2'] = '#9ED7F8';
$design['color_3'] = '#64BEF2';
$design['color_4'] = '#008ACE';

function lbi_save_var ($name, $var, $user_id = -1) {
        if(!is_numeric($user_id)) return false;
        $name = str_replace(array('<','>',':',"\\",'/','|','*','?','"','..'), '_', $name);

        $fp = fopen($name, 'w');
        if(!$fp) return false;
        fwrite($fp, serialize($var));
        fclose($fp);

        return true;
        }

function lbi_load_var ($name, $user_id = -1) {
        if(!is_numeric($user_id)) return false;
        $name = str_replace(array('<','>',':',"\\",'/','|','*','?','"','..'), '_', $name);

        if(!file_exists($name)) return NULL;
        $fp = fopen($name, 'r');
        if(!$fp) return false;
        $var = unserialize(fread($fp, filesize($name)));
        fclose($fp);

        return $var;
        }

function lbi_load_config ($filename = LBI_CONFIG_FILE) {
        if(is_file($filename)){
                $file = file_get_contents($filename);
                if($dom = domxml_open_mem($file)) {
                        $root = $dom->document_element();
                        if($root->node_name() == 'liquid_installer_config') {
                                $root_nodes = $root->child_nodes();
                                while(list($node_id, $node) = each($root_nodes)) {
                                        if($node->node_type() == XML_ELEMENT_NODE) {
                                                $text = $node->child_nodes();
  			               		define('LBI_'.strtoupper($node->node_name()), trim(utf8_decode($text[0]->content)));
                                                }

                                        }
                                }

                        }
                }
	if(!defined('BASE_PATH')) define('BASE_PATH', LBI_BASE_PATH);
	}

function lbi_add_code ($fp, $tag_name, $code) {
        if($tag_name != '' && $code != '') {
                fwrite($fp, "<$tag_name encoding=\"utf8\">".utf8_encode($code)."</$tag_name>\n");
                }
        }

function lbi_add_file($fp, $filename) {
        if(file_exists(BASE_PATH.$filename)) {
		if(LBI_ENABLE_COMPRESSION) {
	                fwrite($fp, '<file name="'.$filename.'" encoding="base64_gz">'.
        	                base64_encode(gzcompress(file_get_contents(BASE_PATH.$filename)))."</file>\n");
			}
		else {
	                fwrite($fp, '<file name="'.$filename.'" encoding="base64">'.
        	                base64_encode(file_get_contents(BASE_PATH.$filename))."</file>\n");
			}
                }
        }

function lbi_get_package_list ($dir) {
        if ($handle = opendir($dir)) {

        while (false !== ($filename = readdir($handle))) {
        if(is_file(LBI_PACKAGE_PATH.$filename)){
                $file = file_get_contents(LBI_PACKAGE_PATH.$filename);
                if($dom = domxml_open_mem($file)) {
                        $root = $dom->document_element();
                        if($root->node_name() == 'liquid_installer_package') {
                                $root_nodes = $root->child_nodes();
                                while(list($node_id, $node) = each($root_nodes)) {
                                        if($node->node_type() == XML_ELEMENT_NODE) {
                                                $text = $node->child_nodes();
                                                if($node->node_name() == 'category' || 
						 $node->node_name() == 'license' ||
						 $node->node_name() == 'name' || 
						 $node->node_name() == 'version' || 
						 $node->node_name() == 'author_name') {
                                        		$lbi_files[$filename][$node->node_name()] = utf8_decode($text[0]->content);
                                                        }
                                                }

                                        }
                                }

                        }
                }
        }
        closedir($handle);
        }
        return $lbi_files;
        }

function lbi_get_details ($filename) {
	$pre = '';

        if(is_file(LBI_PACKAGE_PATH.$filename)){
                $file = file_get_contents(LBI_PACKAGE_PATH.$filename);
                if($dom = domxml_open_mem($file)) {
                        $root = $dom->document_element();
                        if($root->node_name() == 'liquid_installer_package') {
                                $root_nodes = $root->child_nodes();
                                while(list($node_id, $node) = each($root_nodes)) {
                                        if($node->node_type() == XML_ELEMENT_NODE) {
                                                $text = $node->child_nodes();
                                                if($node->node_name() != 'file') {
                                        		$lbi_file[$node->node_name()] = lbi_decode($text[0]->content,$node->get_attribute('encoding'));
                                                        }
						else {
							$lbi_file['files'] .= $pre.$node->get_attribute('name');
							$pre = ', ';
							}
                                                }

                                        }
                                }

                        }
                }
        return $lbi_file;
        }

function lbi_mkdirs ($path, $create_mask) {
        $dirs = explode('/', $path);
        while(list($key, $value) = each($dirs)) {
                $tmp_path .= $value.'/';
                if(!file_exists($tmp_path)) mkdir($tmp_path, $create_mask);
                }
        }

function lbi_decode ($string, $encoding) {
        if(!isset($encoding) || $encoding == '') $encoding = 'utf8';

        switch ($encoding) {
                case 'base64_gz': return gzuncompress(base64_decode($string));
                case 'base64': return base64_decode($string);
                case 'utf8': return utf8_decode($string);
                default: return utf8_decode($string);
                }
        }

function lbi_manage_package ($filename, $action = 'install') {
        if(is_file($filename)){
                $file = file_get_contents($filename);
                if($dom = domxml_open_mem($file)) {
                        $root = $dom->document_element();
                        if($root->node_name() == 'liquid_installer_package') {
                                $root_nodes = $root->child_nodes();
                                while(list($node_id, $node) = each($root_nodes)) {
                                        if($node->node_type() == XML_ELEMENT_NODE) {

                                                $text = $node->child_nodes();

                                                if($action == 'install') {
                                                        if($node->node_name() == 'file') {
								lbi_mkdirs(dirname(BASE_PATH.$node->get_attribute('name')), LBI_CREATE_MASK);
                                                                if(LBI_OVERWRITE_EXISTING_FILES == '1' || 
									!file_exists(BASE_PATH.$node->get_attribute('name'))) {
									$fp = fopen(BASE_PATH.$node->get_attribute('name'), 'w');
	                                                                fwrite($fp, lbi_decode(trim($text[0]->content),
        	                                                                $node->get_attribute('encoding')));
                	                                                fclose($fp);
                        						}
			                                        }

                                                        if($node->node_name() == 'sql_install' 
								&& LBI_EXECUTE_SQL_COMMANDS 
								&& trim($text[0]->content) != '') {
                                                                sql_query(lbi_decode(trim($text[0]->content),
                                                                        $node->get_attribute('encoding')));
                                                                }

                                                        if($node->node_name() == 'php_install' 
								&& LBI_EXECUTE_PHP_COMMANDS
                                                                && trim($text[0]->content) != '') {
                                                                eval(lbi_decode(trim($text[0]->content),
                                                                        $node->get_attribute('encoding')));
                                                                }
                                                        }

                                                elseif($action == 'remove') {
                                                        if($node->node_name() == 'file' && (LBI_DELETE_MODIFIED_FILES || 
md5(lbi_decode(trim($text[0]->content), $node->get_attribute('encoding'))) == 
md5(file_get_contents(BASE_PATH.$node->get_attribute('name')))							
)) {
                                                                unlink(BASE_PATH.$node->get_attribute('name'));
                                                                }

                                                        if($node->node_name() == 'sql_remove' 
								&& LBI_EXECUTE_SQL_COMMANDS
								&& trim($text[0]->content) != '') {
                                                                sql_query(lbi_decode(trim($text[0]->content),
                                                                        $node->get_attribute('encoding')));
                                                                }

                                                        if($node->node_name() == 'php_remove'
								&& LBI_EXECUTE_PHP_COMMANDS
                                                                && trim($text[0]->content) != '') {
                                                                eval(lbi_decode(trim($text[0]->content),
                                                                        $node->get_attribute('encoding')));
                                                                }
                                                        }
                                                }
                                        }
                                return true;
                                }
                        }
                }
        return false;
        }


function lbi_is_shared($name, $version) {
	$basename = strtr(strtolower($name), ' -/\()[]=?.;,!����', '______________aous');
	$baseversion = strtr(strtolower($version), ' -/\()[]=?;,!����', '................');
			
	$url = LBI_SHARE_SERVER.'?request=is_shared&product_name='.LBI_PRODUCT_NAME.'&product_version='.LBI_PRODUCT_VERSION.'&name='.$basename.'&version='.$baseversion;
	$result = file_get_contents($url);

	return $result;	
	}

function lbi_share($name, $version) {
	$basename = strtr(strtolower($name), ' -/\()[]=?.;,!����', '______________aous');
	$baseversion = strtr(strtolower($version), ' -/\()[]=?;,!����', '................');

	$url = LBI_SHARE_SERVER.'?request=share_package&client='.LBI_SHARE_CLIENT.'&product_name='.LBI_PRODUCT_NAME.
	'&product_version='.LBI_PRODUCT_VERSION.'&name='.$basename.'&version='.$baseversion;
	file_get_contents($url);
	}

?>
