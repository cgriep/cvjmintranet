<?
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Liquid Installer
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 05.04.2004
*/

$lftk_config['color_1'] = '#B9E3FB';
$lftk_config['color_2'] = '#9ED7F8';
$lftk_config['color_3'] = '#64BEF2';
$lftk_config['color_4'] = '#008ACE';

$lftk_config['color_row_bright'] = '#ffffff';
$lftk_config['color_row_dark']   = '#F9FBFF';

$lftk_config['default_align']    = 'left';

$lftk_config['table_begin'] = '<table width="100%" cellpadding="3" cellspacing="1" bgcolor="'.$lftk_config['color_1'].'"
border="0">';

$lftk_table_row_counter = 0;

function lftk_display_search_input ($name) {
	global $lftk_config
	?>
<table border="0" cellpadding="3" cellspacing="1" width="100%" bgcolor="<?=$lftk_config['color_2']?>">
<tr>
<td align="center" bgcolor="<?=$lftk_config['color_3']?>">
<input type="text" size="45" name="<?=$name?>" value="<?=htmlentities($_REQUEST[$name])?>" />&nbsp;
<input type="submit" value="<?=LANG_ADMIN_SEARCH?>" />
</td>
</tr>
</table>
<br />
	<?
	}

function lftk_display_warning ($text) {
	?><table border="0" cellpadding="1" cellspacing="1" bgcolor="#000000">
	<tr><td bgcolor="#ffffff">
		<table border="0" cellpadding="0" cellspacing="0">
		<tr>
		<td><?=get_image_tag('img/lftk_warning.gif')?></td>
 		<td width="100%">&nbsp;<?=htmlentities($text)?></td>
		</tr>
		</table>
	</td></tr>
	</table><?
	}

function lftk_display_error ($text) {
	?><table border="0" cellpadding="1" cellspacing="1" bgcolor="#FF0000">
	<tr><td bgcolor="#ffffff">
		<table border="0" cellpadding="0" cellspacing="0">
		<tr>
		<td><?=get_image_tag('img/lftk_error.gif')?></td>
 		<td width="100%">&nbsp;<?=htmlentities($text)?></td>
		</tr>
		</table>
	</td></tr>
	</table><?
	}

function lftk_get_number_dropdown ($name, $from, $to, $select, $empty_option = FALSE) {
	if(!is_numeric($from) || !is_numeric($to) || $to < $from) return FALSE;

	$result = '<select name="'.$name.'">'."\n";

	if($empty_option) {
		$result .= "\t".'<option value=""></option>'."\n";
		}

	for($i = $from; $i <= $to; $i++) {

		if($i == $select) {
			$_selected = ' selected="true"';
			}
		else {
			$_selected = '';
			}

		$result .= "\t".'<option value="'.$i.'"'.$_selected.'>'.$i.'</option>'."\n";
		}

	$result .= '</select>'."\n";

	return $result;
	}

function lftk_display_number_dropdown ($name, $from, $to, $select, $empty_option = FALSE) {
	echo lftk_get_number_dropdown ($name, $from, $to, $select, $empty_option);
	return TRUE;
	}

function lftk_import_argument ($name, $default = FALSE) {
	
	global $$name;

	if(!isset($$name)) {
		if(isset($_REQUEST[$name])) {
		        $$name = $_REQUEST[$name];
		        }
		else {
        		$$name = $default;
        		}
		}	

	return TRUE;
	}

function lftk_get_table_header ($cols, $align, $autolink = FALSE, $all_bold = FALSE) {
	if(!is_array($cols) || !is_array($align)) return FALSE;
	
	global $lftk_config;
	global $lftk_table_row_counter;
	global $page;
	global $sort_order;
	global $first_item;
	global $target_id;

	// Reset Counter
	$lftk_table_row_counter = 0;

	$result = $lftk_config['table_begin'];
	
	$result .= '<tr>'."\n";

	while(list($key, $value) = each($cols)) {
		if(isset($align[$key])) {
			$_align = $align[$key];
			}
		else {
			$_align = $lftk_config['default_align'];
			}

		if($sort_order == $key || $all_bold) {
			$_style = 'font-weight: bold;';
			}
		else {
			$_style = 'font-weight: normal;';
			}

		if($autolink && $key[0] != '_') {
			$result .= "\t".'<td align="'.$_align.'">'.
			           '<a style="'.$_style.'" href="'.$_SERVER['PHP_SELF'].
				   '?page='.$page.
				   '&sort_order='.$key.
				   '&first_item='.$first_item.
				   '&target_id='.$target_id.
				   '">'.$value.'</a></td>';
			}
		else {
			$result .= '<td style="'.$_style.'" align="'.$_align.'">'.$value.'</td>';
			}
		}

	$result .= '</tr>'."\n";

	return $result;
	}

function lftk_display_table_header ($cols, $align, $autolink = FALSE, $all_bold = FALSE) {
	echo lftk_get_table_header ($cols, $align, $autolink, $all_bold);
	return TRUE;
	}

function lftk_get_table_row ($cols, $align) {
	global $lftk_config;
	global $lftk_table_row_counter;

	if($lftk_table_row_counter % 2 > 0) {
		$_row_color = $lftk_config['color_row_dark'];
		}
	else {
		$_row_color = $lftk_config['color_row_bright'];
		}

	$lftk_table_row_counter++;

	$result = '<tr bgcolor="'.$_row_color.'">'."\n";

	while(list($key, $value) = each($cols)) {
		if(isset($align[$key])) {
			$_align = $align[$key];
			}
		else {
			$_align = $lftk_config['default_align'];
			}

		$result .= "\t".'<td valign="top" align="'.$_align.'">'.$value.'</td>'."\n";
		}

	$result .= '</tr>'."\n";

	return $result;
	}

function lftk_display_table_row ($cols, $align) {
	echo lftk_get_table_row ($cols, $align);
	return TRUE;
	}

function lftk_get_table_footer () {
	$result = '</table>';
	return $result;
	}

function lftk_display_table_footer () {
	echo lftk_get_table_footer ();
	return TRUE;
	}

function lftk_add_tab($name, $label) {
        global $lftk_config;
        global $page;
	global $target_id;

        if($page == $name) {
                $bg = $lftk_config['color_3'];
                $style = 'color: black;';
                }
        else {
                $bg = $lftk_config['color_4'];
                $style = 'color: white;';
                }

        ?><td bgcolor="<?=$bg?>" width="1" background="img/edge_left.gif"><img src="img/pixel.gif" /></td><td
          bgcolor="<?=$bg?>" align="center" nowrap="nowrap"><a style="<?=$style?>"
          href="<?=$_SERVER['PHP_SELF'].'?page='.$name.'&target_id='.$target_id?>"><?=$label?></a></td><td bgcolor="<?=$bg?>" width="1"
          background="img/edge_right.gif"><img src="img/pixel.gif" width="5" /></td><?
        }

function lftk_display_page_links ($order, $count, $wrap_count, $first_item) {
        ?><table width="100%" cellpadding="3" cellspacing="0" border="0"><tr>
        <td class="text"><?=LANG_PAGE?></td>
        <?
        global $lftk_config;
        $i = 0;
        $page = 0;
        $max_dist = 10;
        while($i < $count) {
                $distance = abs($i - $first_item);
                if($distance <= $max_dist*$wrap_count || $i == 0 || ($count - $i) < $wrap_count) {
                        if($first_item == $i) {
                        echo '<td class="text" bgcolor="'.$lftk_config['color_1'].'">'; }
                        else { echo '<td class="text">'; }
                        ?><a href="<?=$_SERVER['PHP_SELF']
                        ?>?page=<?=$GLOBALS['page']?>&order=<?=$order?>&first_item=<?=$i
                        ?>&search=<?=urlencode($_REQUEST['search'])?>&target_id=<?=$GLOBALS['target_id']?>"><?=$page+1?></a><?
                        echo '</td>';
                        }
                elseif($distance == ($max_dist + 1)*$wrap_count) {
                        echo '<td class="text">';
                        echo '...</td>';
                        }
                $i += $wrap_count;
                $page++;
                }
        echo '<td class="text" width="100%" align="right">';
        printf(LANG_ROWS_FOUND, $count);
        echo '</td>';
        ?></tr></table><?
        }

