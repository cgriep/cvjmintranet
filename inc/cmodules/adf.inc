<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

load_code('adf_functions');
load_lang('adf');

if($children = get_children (0, 100, $doctypes['paragraph'])) {
        reset($children);
        while (list ($docid, $value) = each ($children)) {
                if($value['type_id']==$doctypes['paragraph']) {
                        include(INC_PATH.'misc/paragraph.inc');
                        }
                }
	}

if($profile['editor'] != '1') {

$show_search_form = false;

if(isset($_POST['save'])) $save = true;

if(!isset($first_item) || !is_numeric($first_item)) $first_item = 0;

if(isset($_REQUEST['order'])) { 
	$order = $_REQUEST['order']; 
	}
else { 
	$order = ''; 
	}

// XML Parsing
if(!$dom = domxml_open_mem($document['body'])) {
	echo '<p class="pSpace">Error in XML form definition.</p>';
	}

$root = $dom->document_element();

if($root->node_name() == 'adf') {

$root_nodes = $root->child_nodes();

// parse context tags ("Tabs")
while(list($node_id, $node) = each($root_nodes)) {
	if($node->node_type() == XML_ELEMENT_NODE && $node->node_name() == 'context' && has_access($node, $profile, 'read')) {
		$_tabs[utf8_decode($node->get_attribute('name'))] = utf8_decode($node->get_attribute('label'));
		if($node->get_attribute('name') == $_REQUEST['page'] || !isset($_REQUEST['page'])) {
			$main_node = $node; 
			$_REQUEST['page'] = $node->get_attribute('name');
			$attr = $node->attributes();
			while(list($key, $value) =  each($attr)) {
				$main_node_attr[utf8_decode($value->name)] = eval('return "'.utf8_decode($value->value).'";');
				}
			}

		}
	}

if(isset($main_node)) {

// set target
if($root->get_attribute('target') != '')	{ $target    = eval('return "'.utf8_decode($root->get_attribute('target')).'";'); }
elseif(isset($main_node_attr['target']))	{ $target    = $main_node_attr['target']; }
else 						{ $target    = $_REQUEST['target']; }

// set target_id
if($root->get_attribute('target_id') != '')	{ $target_id = eval('return "'.utf8_decode($root->get_attribute('target_id')).'";'); }
elseif(isset($main_node_attr['target_id'])) 	{ $target_id = $main_node_attr['target_id']; }
else 						{ $target_id = $_REQUEST['target_id']; }

?>
<center>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
<input type="hidden" name="id" value="<?=$id?>" />
<input type="hidden" name="page" value="<?=$_REQUEST['page']?>" />
<input type="hidden" name="target" value="<?=$target?>" />
<input type="hidden" name="target_id" value="<?=$target_id?>" />
<? if(count($_tabs) > 1) { ?>
<table width="100%" cellpadding="5" cellspacing="0" border="0"><tr>
<?
while(list($tab, $label) = each($_tabs)) {
	if($tab == $_REQUEST['page']) {
		$background = $design['table_row_bright']; 
		echo '<td bgcolor="'.$background.'" width="1" background="img/edge_left.gif"><img src="img/pixel.gif" /></td>';
		echo '<td bgcolor="'.$background.'" align="center" nowrap="nowrap"><a class="selectedNav" href="'.get_url($id, 'page='.$tab.'&target_id='.$target_id).'">'.$label.'</a></td>';
		echo '<td bgcolor="'.$background.'" width="1" background="img/edge_right.gif"><img src="img/pixel.gif" width="5" /></td>';
		}
	else {
		$background = $design['table_row_dark'];
		echo '<td bgcolor="'.$background.'" width="1" background="img/edge_left.gif"><img src="img/pixel.gif" /></td>';
		echo '<td bgcolor="'.$background.'" align="center" nowrap="nowrap"><a class="normalNav" href="'.get_url($id, 'page='.$tab.'&target_id='.$target_id).'">'.$label.'</a></td>';
		echo '<td bgcolor="'.$background.'" width="1" background="img/edge_right.gif"><img src="img/pixel.gif" width="5" /></td>';
		}
	}

?>
<td bgcolor="<?=$design['default_background']?>" width="100%"><img src="img/pixel.gif" width="1" /></td></tr>
</table>
<? }
if($main_node_attr['no_frame'] == 'true') {
	echo '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="0" border="0">';
	$_height = 100;
	}
else {
	echo '<table width="100%" cellpadding="'.$design['table_cellpadding'].'" cellspacing="0" border="0" bgcolor="'.$design['table_row_bright'].'">';
	$_height = 350;
	}
?>
<tr>
<td height="<?=$_height?>" valign="top" class="text">
<?
$elements = $main_node->child_nodes();

while(list($node_id, $node) = each($elements)) {
	if($node->node_type() == XML_ELEMENT_NODE && has_access($node, $profile, 'read')) {
		update_target ($node);

		switch($node->node_name()) {
			case 'col': 
				$attr = $node->attributes();
				while(list($key, $value) =  each($attr)) {
					if($value->name == 'searchable' && $value->value == 'true') $show_search_form = true;
					$cols[utf8_decode($node->get_attribute('name'))][utf8_decode($value->name)] = eval('return "'.utf8_decode($value->value).'";');
					}
				break;
			case 'db_table': 
				$attr = $node->attributes();
				while(list($key, $value) =  each($attr)) {
					$tables[utf8_decode($node->get_attribute('name'))][utf8_decode($value->name)] = eval('return "'.utf8_decode($value->value).'";');
					}
				break;
			default:
				$code = get_code('adf_tags', $node->node_name());

				if($code != '') {
					$attr = $node->attributes();
					$attributes = array();
					while(list($key, $value) =  each($attr)) {
						$attributes[utf8_decode($value->name)] = utf8_decode($value->value);
						}

					eval($code);
					}

			}
		}
	}

switch($main_node_attr['mode']) {
case 'edit':
	// nothing to do anymore...
	break;

case 'list':
	if($show_search_form) {
		?><table width="100%" bgcolor="<?=$GLOBALS['design']['table_row_dark']?>" cellpadding="3" cellspacing="1" border="0"><tr>
		<td class="text" bgcolor="<?=$GLOBALS['design']['table_row_bright']?>" align="center" valign="middle">
		<input type="text" size="42" name="search" value="<?=$_REQUEST['search']?>" />
		<input type="submit" name="docinput[start_search]" value="<?=LANG_SEARCH?>" /></td></tr></table><br /><?
		}
	$qres = sql_query(adf_compose_sql_query($tables, $cols, true, '', 0, 0, $_REQUEST['search']));
	$row  = sql_fetch_row($qres);
	sql_free_result($qres);
	$count = $row[0];

	if(isset($_REQUEST['delete'])) {
		if(is_numeric($_REQUEST['delete']) && has_access($main_node, $profile, 'write')) {
			adf_delete_items($tables, array($_REQUEST['delete']));
			}
		else { ?>
			<script language="javascript"> 
			<!-- 
			alert('You may not delete this item or your request was malformed.');
			// -->
			</script>
			<?	
			}
		}

	if(isset($_REQUEST['switch'])) {
		if(is_numeric($_REQUEST['switch']) && isset($_REQUEST['col']) && has_access($main_node, $profile, 'write')) {
			adf_switch_bool($tables, $cols, $_REQUEST['col'], array($_REQUEST['switch']));
			}
		else { ?>
			<script language="javascript"> 
			<!-- 
			alert('You may not change this item or your request was malformed.');
			// -->
			</script>
			<?
			}
		}

	adf_display_table_header($cols, $order, $count, $main_node_attr['wrap'], $first_item);

	$qres = sql_query(adf_compose_sql_query($tables, $cols, false, $order, $main_node_attr['wrap'], $first_item, $_REQUEST['search']));

	while($row = sql_fetch_array($qres)) {
		adf_display_table_row($row, $cols, $target);
		}

	sql_free_result($qres);

	adf_display_table_footer($cols, $order, $count, $main_node_attr['wrap'], $first_item);
	break;

case 'insert':
	if(isset($_REQUEST['insert'])) {
		$insert_id = adf_add_record($_REQUEST['docinput']['insert'], $tables, $cols);
		if($insert_id === false) {
			echo '<p class="text" style="font-weight:bolder">'.LANG_DATALIST_FIELDS_REQ.'</p>';
			$insert_id = 0;
			}
		elseif($insert_id == 0) {
			echo '<p class="text" style="font-weight:bolder">'.LANG_DATALIST_RECORD_EXISTS.'</p>';
			}
		else {
			echo '<p class="text" style="font-weight:bolder">';
			printf(LANG_DATALIST_RECORD_ADDED, $insert_id);
			echo '</p>';
			$save = true;
			if($main_node_attr['oncreate'] != '') {
				?>
				<script language="javascript">
                	        <!--
				<?=str_replace('%%id%%', $insert_id, $main_node_attr['oncreate'])?>
	                        // -->
	                        </script>
				<?
				}
			}
		}
	else {
		$save = false;
		$insert_id = 0;
		}
		
	reset($cols);
	while(list($key, $value) = each($cols)) {
		switch($value['source']) {
			case 'db_table':
				switch($value['format']) {
					case 'random':
						?>
						<input type="hidden" name="docinput[insert][<?=$value['name']?>]" value="<?=random_id()?>" />
						<?
						break;
					case 'hidden':
                                                ?>
                                                <input type="hidden" name="docinput[insert][<?=$value['name']?>]" value="<?=$value['value']?>" />
                                                <?
                                                break;
					case 'number':
					case 'text':
						if(isset($value['values'])) {
							unset($_values);
							$_values = explode(',', $value['values']);
							echo '<p class="text">'.$value['label']."<br />\n".'<select name="docinput[insert]['.$value['name'].']">'."\n";
							while(list($item_nr, $item) = each($_values)) {
								echo "\t<option>$item</option>\n";
								}
							echo "</select>\n</p>";
							}
						elseif(isset($value['valuesource'])) {
							unset($_values);
							$_valuessource = explode(':', $value['valuesource']);

							echo '<p class="text">'.$value['label']."<br />\n".'<select name="docinput[insert]['.$value['name'].']">'."\n";

							$qres = sql_query("SELECT ".$_valuessource[2].", ".$_valuessource[1]." FROM ".$_valuessource[0]." ORDER BY ".$_valuessource[3]);
							while($row = sql_fetch_row($qres)) {
								if($value['format'] == 'number') {
									echo "\t<option value=\"".$row[0]."\">".htmlentities(stripslashes($row[1]))."</option>\n";
									}
								else {
									echo "\t<option>".htmlentities(stripslashes($row[0]))."</option>\n";
									}
								}
							echo "</select>\n</p>";
							}
						elseif(isset($value['itemsource'])) {
							$_values = items_get_items ($value['itemsource']);
							echo '<p class="text">'.$value['label']."<br />\n".'<select name="docinput[insert]['.$value['name'].']">'."\n";
							while(list($item_nr, $item) = each($_values)) {
								echo "\t<option value=\"$item_nr\">".htmlentities($item)."</option>\n";
								}
							echo "</select>\n</p>";
							}
						else {
							echo '<p class="text">'.$value['label'].'<br /><input type="text" size="60" name="docinput[insert]['.$value['name'].']" value="'.htmlentities($_REQUEST['docinput']['insert'][$value['name']]).'" /></p>';
							}
						break;
					case 'textarea':
						echo '<p class="text">'.$value['label'].'<br /><textarea cols="60" rows="8" name="docinput[insert]['.$value['name'].']">'.htmlentities($_REQUEST['docinput']['insert'][$value['name']]).'</textarea></p>';
						break;
					case 'textarea_xxl':
						echo '<p class="text">'.$value['label'].'<br /><textarea cols="60" rows="20" name="docinput[insert]['.$value['name'].']">'.htmlentities($_REQUEST['docinput']['insert'][$value['name']]).'</textarea></p>';
						break;
					case 'country':
						if(!isset($docinput['insert'][$value['name']])) $docinput['insert'][$value['name']] = 'DE';
			                        echo '<p class="text">'.$value['label'].'<br />
						<select name="docinput[insert]['.$value['name'].']">'."\n";
			                        $countries = unserialize(join('', file(INC_PATH.DATA_PATH.'/countries.ser')));
			                        while(list($ckey, $cvalue) = each($countries)) {
			                                ?>      <option value="<?=$ckey?>"<? if($docinput['insert'][$value['name']] == $ckey) echo ' selected="true"'; ?>><?=$cvalue?></option><?
			                                echo "\n";
			                                }
			                        echo '</select>';
			                        break;
					case 'bool':
						echo '
						<p class="text">'.$value['label'].'<br />
		  					<select name="docinput[insert]['.$value['name'].']">
								<option value="1">Yes</option>
								<option value="0">No</option>
							</select>
						</p>';
						break;
					case 'timestamp':
						$date = getdate();
						echo '<p class="text">'.$value['label']."<br />\n";
						echo "\t".'<select name="docinput[insert]['.$value['name'].'][day]">'."\n";
						for($i = 1; $i <= 31; $i++) {		
							if($date['mday'] == $i) {
								$selected = ' selected="selected"';	
								}
							else {
								$selected = '';	
								}
							echo "\t\t<option value=\"$i\"$selected>".str_pad($i, 2, '0', STR_PAD_LEFT)."</option>\n";
							}
						echo "\t</select> . \n";
						echo "\t".'<select name="docinput[insert]['.$value['name'].'][month]">'."\n";
						for($i = 1; $i <= 12; $i++) {		
							if($date['mon'] == $i) {
								$selected = ' selected="selected"';	
								}
							else {
								$selected = '';	
								}
							echo "\t\t<option value=\"$i\"$selected>".str_pad($i, 2, '0', STR_PAD_LEFT)."</option>\n";
							}
						echo "\t</select> . \n";
						echo "\t".'<select name="docinput[insert]['.$value['name'].'][year]">'."\n";
						for($i = 1900; $i <= 2020; $i++) {		
							if($date['year'] == $i) {
								$selected = ' selected="selected"';	
								}
							else {
								$selected = '';	
								}
							echo "\t\t<option$selected>$i</option>\n";
							}
						echo "\t</select>\n";
						echo "</p>";
						break;
					}
				break;
			default:
				echo "\n".'<p class="text">'.$value['label']."<br />\n";

				adf_create_docinput($value, $target, $insert_id, $save);

				echo "</p>\n";
				break;
			}
		}
	break;

case 'export':
	$qres = sql_query(adf_compose_sql_query($tables, $cols, true));
	$row  = sql_fetch_row($qres);
	sql_free_result($qres);
	$count = $row[0];

	?><p>
	<span class="text"><?
	printf(LANG_DUMP_ITEMS, '<input type="text" name="from" value="0" size="7" />', '<input type="text" name="to" size="7" value="'.$count.'" />');
	?> 
	<select name="as">
		<option value="xml"<? if($_REQUEST['as'] == 'xml') echo ' selected="selected"'; ?>>XML</option>
		<option value="csv"<? if($_REQUEST['as'] == 'csv') echo ' selected="selected"'; ?>>CSV (Comma Separated Values)</option>
		<option value="tsv"<? if($_REQUEST['as'] == 'tsv') echo ' selected="selected"'; ?>>TSV (Tab Separated Values)</option>
		<option value="html"<? if($_REQUEST['as'] == 'html') echo ' selected="selected"'; ?>>HTML</option>
	</select>
	</span>
	<p><?
	if(isset($_REQUEST['show_example']) && isset($_REQUEST['as']) && is_numeric($_REQUEST['from']) && is_numeric($_REQUEST['to']) && ($_REQUEST['from'] < $_REQUEST['to'])) {
		?><span class="pSpaceBold">Example</span><?
		?><pre><?
		?><?=htmlentities(wordwrap(adf_dump_example ($_REQUEST['as'], adf_compose_sql_query($tables, $cols, false, $order, $_REQUEST['to']-$_REQUEST['from'], $_REQUEST['from']), $cols, $target), 71, "\n\t"))?><?
		?></pre><? 
		}
	break;
}
?>
</td>
</tr>
<tr>
<td height="25" valign="top" align="right" class="text">
<? if(isset($main_node_attr['footnote'])) { ?>
<?=$main_node_attr['footnote']?>
<? } ?>
<? if($main_node_attr['mode'] == 'insert') { ?>
	<input type="submit" name="insert" value="<?=LANG_ADD_NOW?>" />
<? } elseif($main_node_attr['mode'] == 'export') { ?>
	<? if(!isset($_REQUEST['dump_create']) || !isset($_REQUEST['as']) || !is_numeric($_REQUEST['from']) || !is_numeric($_REQUEST['to']) || ($_REQUEST['from'] > $_REQUEST['to'])) { ?>
		<input type="submit" name="show_example" value="<?=LANG_SHOW_EXAMPLE?>" />
		<input type="submit" name="dump_create" value="<?=LANG_CREATE_DUMP?>" />
		<? } else { ?> 
			<br />
			<br />
			<center><a style="font-weight: bolder" href="<?=adf_dump($_REQUEST['as'],adf_compose_sql_query($tables, $cols, false, $order, $_REQUEST['to']-$_REQUEST['from'], $_REQUEST['from']), $cols, $target)?>">Download Dump</a></center>
			<? } ?> 
<? } elseif($main_node_attr['mode'] == 'edit') { ?>
	<input type="hidden" name="save" value="1" />
	<input type="submit" value="<?=LANG_SAVE?>" />
	<?
	if($main_node_attr['onsave'] != '' && $_REQUEST['save'] == '1') {
		?>
		<script language="javascript">
               	        <!--
			<?=str_replace('%%id%%', $insert_id, $main_node_attr['onsave'])?>
	                // -->
	        </script>
		<?
		}

	} ?>
</td> 
</tr> 
</table> 
</form> 
<?
} else {
	echo '<p class="pSpace">Error: Access denied.</p>'; 
	}
} else {
	echo '<p class="pSpace">Error: Wrong data format.</p>';
	}

} elseif($profile['editor'] == '1' && SESSION_STATUS == 'ok') {

	create_editor_input('XML template','body','area_input');

	}
?>
