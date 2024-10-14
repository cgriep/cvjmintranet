<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 24.08.2003
*/

if(defined('DISABLE_WYSIWYG') && DISABLE_WYSIWYG == '0') {
	include("spaw/spaw_control.class.php");

	if(isset($_REQUEST['wysiwyg_docdata'])) {
		foreach ( $_REQUEST['wysiwyg_docdata'] as $key => $value) {
			$_REQUEST['docdata'][$value] = $_REQUEST['wysiwyg_'.$value];
			}
		}

	function wysiwyg_editor_textarea ($name, $content) {
		global $document;
		global $lang;
		global $design;
		global $spaw_dropdown_data;
		global $sw;

		// make a copy of $spaw_dropdown_data array
		$spaw_array = $spaw_dropdown_data;
		// unset current styles
		unset($spaw_array['style']);	
		// set new styles
		
		$css_contents = file(BASE_PATH.'css/'.$design['css'].'.css');
		// print_r($css_contents);
		foreach ( $css_contents as $key => $value)  {
			$value = trim($value);
			if($value != '' && strpos($value,'.') === 0) {
				$spaw_array['style'][substr($value, 1)] = ucwords(strtr(substr($value, 1), '_', ' '));
				}
			}

		$sw = new SPAW_Wysiwyg(
			'wysiwyg_'.$name,
			//docdata['.$name.']',
			$content,
                        $lang, 
			'sidetable', 
			'default',
			'100%', 
			'350px', 
			'css/'.$design['css'].'.css',
			$spaw_array);

		$sw->show();
		?>
		<input type="hidden" name="wysiwyg_docdata[]" value="<?=$name?>" />
		<?php
		}
	}

?>
