/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

// ADF Functions

function adf_dump_xml ($sql_query, $cols, $target) {
	$filename = 'admin/dumps/'.$target.'_dump_'.time().'.xml';
	$fp = fopen($filename, "w");
	fwrite($fp, '<?xml version="1.0" encoding="UTF-8"?>'."\n".'<data_dump target="'.utf8_encode($target).'">'."\n");
	fwrite($fp, "\t<sql_query>".utf8_encode($sql_query)."</sql_query>\n");
	$qres = sql_query($sql_query);

        while($row = sql_fetch_array($qres)) {
		fwrite($fp, "\t"."<item>\n");
		foreach ( $row as $key => $value )  {
			fwrite($fp, "\t\t<".utf8_encode($key).' format="'.$cols[$key]['format'].'">'.utf8_encode(stripslashes($value)).'</'.utf8_encode($key).">\n");
			}
		fwrite($fp, "\t</item>\n");
                }
	fwrite($fp, '</data_dump>');
	fclose($fp);
	sql_free_result($qres);
	return $filename;
	}

function adf_dump_xml_example ($sql_query, $cols, $target) {
	$result = ('<?xml version="1.0" encoding="UTF-8"?>'."\n\n");
	$result .=('<data_dump target="'.utf8_encode($target).'">'."\n");
	$result .= ("\t<sql_query>".utf8_encode($sql_query)."</sql_query>\n");

	$qres = sql_query($sql_query);

        $row = sql_fetch_array($qres);
	$result .= ("\t"."<item>\n");
	foreach ( $row as $key => $value )  {
		$result .= ("\t\t<".utf8_encode($key).' format="'.$cols[$key]['format'].'">'.utf8_encode(stripslashes($value)).'</'.utf8_encode($key).">\n");
		}
	$result .= ("\t</item>\n</data_dump>");
	return $result;
	}

function adf_dump_csv ($sql_query, $cols, $target, $sep = ';') {
	$filename = 'admin/dumps/'.$target.'_dump_'.time().'.csv';
	$fp = fopen($filename, "w");
	$qres = sql_query($sql_query);

        while($row = sql_fetch_array($qres)) {
		fwrite($fp, join($sep, $row)."\n");
                }
	fclose($fp);
	sql_free_result($qres);
	return $filename;
	}

function adf_dump_csv_example ($sql_query, $cols, $target, $sep = ';') {
	$qres = sql_query($sql_query);

        $row = sql_fetch_array($qres);
	$result = join($sep,$row)."\n";

	sql_free_result($qres);
	return $result;
	}

function adf_dump_html ($sql_query, $cols, $target) {
	$filename = 'admin/dumps/'.$target.'_dump_'.time().'.html';

	$fp = fopen($filename, "w");
	fwrite($fp, "<html>\n<head>\n\t<title>".$target."</title>\n</head>\n<body>\n");
	fwrite($fp, "<table>\n");

	fwrite($fp, "\t"."<tr>\n");
	foreach ( $cols as $key => $value) {
		fwrite($fp, "\t\t<th>".htmlentities($key)."</th>\n");
		}
	fwrite($fp, "\t</tr>\n");

	$qres = sql_query($sql_query);

	while($row = sql_fetch_array($qres)) {
                fwrite($fp, "\t"."<tr>\n");
                reset($cols);
                foreach ( $cols as $key => $value) {
                        if($value['source'] == 'db_table') {
                                fwrite($fp, "\t\t<td>".htmlentities(transform(stripslashes($row[$key]), $value['format']))."</td>\n");
                                }
                        else {
                                fwrite($fp, "\t\t<td>".htmlentities(adf_get_string($value, $key, $target, $row['id']))."</td>\n");
                                }
                        }
                fwrite($fp, "\t</tr>\n");
                }

	fwrite($fp, "</table>\n</body>\n</html>");

	fclose($fp);
	sql_free_result($qres);
	return $filename;
	}

function adf_dump_html_example ($sql_query, $cols, $target) {
	$qres = sql_query($sql_query);

	$result = "<html>\n<head>\n\t<title>".$target."</title>\n</head>\n<body>\n";
	$result .= "<table>\n";

	$result .= "\t"."<tr>\n";
	foreach ( $cols as $key => $value) {
		$result .= "\t\t<th>".htmlentities($key)."</th>\n";
		}
	$result .= "\t</tr>\n";

        $row = sql_fetch_array($qres);

	$result .= "\t"."<tr>\n";

        reset($cols);

        foreach ( $cols as $key => $value) {
	        if($value['source'] == 'db_table') {
        	        $result .= "\t\t<td>".htmlentities(transform(stripslashes($row[$key]), $value['format']))."</td>\n";
                	}
                else {
                	$result .= "\t\t<td>".htmlentities(adf_get_string($value, $key, $target, $row['id']))."</td>\n";
                        }
		}

	$result .= "\t</tr>\n";
	$result .= "</table>\n</body>\n</html>";

	sql_free_result($qres);
	return $result;
	}

function adf_dump ($method, $sql_query, $cols, $target) {
	switch($method) {
		case 'xml': return adf_dump_xml($sql_query, $cols, $target);
		case 'csv': return adf_dump_csv($sql_query, $cols, $target, ',');
		case 'tsv': return adf_dump_csv($sql_query, $cols, $target, "\t");
		case 'html': return adf_dump_html($sql_query, $cols, $target);
		}
	}

function adf_dump_example ($method, $sql_query, $cols, $target) {
	switch($method) {
		case 'xml': return adf_dump_xml_example($sql_query, $cols, $target);
		case 'csv': return adf_dump_csv_example($sql_query, $cols, $target, ',');
		case 'tsv': return adf_dump_csv_example($sql_query, $cols, $target, "\t");
		case 'html': return adf_dump_html_example($sql_query, $cols, $target);
		}
	}

function adf_compose_sql_query ($tables, $cols, $count_only = false, $order = '', $count = 50, $offset = 0, $search = '') {
	$where = '';
	$distinct = '';
	$where_pre = 'WHERE ';
	$search_where = '';
	$pre = '';

	// table list and where filter
	while(list($key, $value) = each($tables)) {
		if($value['type'] == 'join' || $value['type'] == 'primary' || $value['type'] == 'secondary') {
			$table_string = $table_string.$pre.addslashes($key);
			$pre = ', ';
			if($value['type'] == 'primary') {
				$primary_col = addslashes($key).'.'.$value['key'];
				}
			if(isset($value['filter']) && $value['filter'] != '') {
				$where = $where.$where_pre.$value['filter'];
				$where_pre = ' AND ';
				}
			}
		}

	// fields
	$pre = '';
	foreach ( $cols as $key => $value) {
		if($value['source'] == 'db_table') {
			$fields = $fields.$pre.$value['table'].'.'.$key;
			$pre = ', ';
			}
		}

	if($search != '') {
		$search_blocks = array();
		$search_blocks_items = explode(' and ', trim($search));
//		print_r($search_blocks_items);
		while(list($search_block_nr, $search_block) = each($search_blocks_items)) {
			$search_or_blocks = array();
			$search_items = explode(' or ', trim($search_block));
//			print_r($search_items);
			while(list($search_item_nr, $search_item) = each($search_items)) {
				$search_or_block = '';
				$search_or_pre = '';
				reset($cols);
				foreach ( $cols as $key => $value) {
					if($value['source'] == 'db_table' && $value['format'] == 'text' && $search_item != '' && $value['searchable'] == 'true') {
						$search_or_block = $search_or_block.$search_or_pre.$value['name']." LIKE '%".addslashes($search_item)."%'";
						$search_or_pre = ' OR ';
						}
					}
				$search_or_blocks[] = $search_or_block;
				}
			$search_blocks[] = '('.join(') OR (', $search_or_blocks).')';
			}
		$search_where = '('.join(') AND (', $search_blocks).')';
// 		echo '<pre>'.$search_where.'</pre>';
		}


	if($search_where != '') {
		$where = $where.$where_pre.'('.$search_where.')';
		$where_pre = ' AND ';
		}

	if($count_only) {
		if(count($tables) > 1) {
			$fields = 'COUNT(DISTINCT '.$fields.')';
			}
		else {
			$fields = 'COUNT(*)';
			}
		}

	// join filter
	if(count($tables) > 1) {
		if(!$count_only && count($tables) > 1) {
			$distinct = 'DISTINCT'; }
		else {
			$distinct = ''; }
		reset($tables);
		while(list($key, $value) = each($tables)) {
			if($value['type'] == 'join') {
				$where = $where.$where_pre.$key.'.'.$value['key'].' = '.$primary_col;
				$where_pre = ' AND ';
				}
			}
		}

	if(!$count_only) {
		$limit = sql_limit($count, $offset);
		if(isset($cols[$order]['name'])) {
	 		$order_by = 'ORDER BY '.$cols[$order]['table'].'.'.$cols[$order]['name'];
			if(isset($cols[$order]['sort_dir'])) $order_by = $order_by.' '.$cols[$order]['sort_dir'];
			}
		else {
			$order_by = '';
//			$order_by = 'ORDER BY '.$primary_col;
			}
		}
	else {
		$limit = '';
		$order_by = ''; }

//	echo "<pre>SELECT $distinct $fields FROM $table_string $where $order_by $limit</pre>";
	return "SELECT $distinct $fields FROM $table_string $where $order_by $limit";
	}

function adf_display_page_links ($order, $count, $wrap_count, $first_item) {
	?><table width="100%" cellpadding="3" cellspacing="0" border="0"><tr>
	<td class="text"><?=LANG_PAGE?></td>
	<?php
	$i = 0;
	$page = 0;
	$max_dist = 10;
	while($i < $count) {
		$distance = abs($i - $first_item);
		if($distance <= $max_dist*$wrap_count || $i == 0 || ($count - $i) < $wrap_count) {
			if($first_item == $i) { echo '<td class="text" bgcolor="'.$GLOBALS['design']['table_header'].'">'; }
			else { echo '<td class="text">'; }
			echo '<a href="'.get_url($GLOBALS['id'], "page=".
                           $_REQUEST['page']."&order=".$order."&first_item=".
                           $i."&search=".urlencode($_REQUEST['search']).
                           "&target_id=".$GLOBALS['target_id']).'">'.$page.'</a>';
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
	?></tr></table><?php
	}

function adf_display_table_header ($cols, $order, $count, $wrap_count, $first_item) {
	adf_display_page_links ($order, $count, $wrap_count, $first_item);
	?><table width="100%" bgcolor="<?=$GLOBALS['design']['table_header']?>" cellpadding="1" cellspacing="0" border="0"><tr><td><?php
	?><table width="100%" cellpadding="3" cellspacing="1" border="0"><tr><?php
	foreach ( $cols as $key => $value) {
		if($order == $value['name']) { $style = 'class="text" style="font-weight: bold"'; }
		else { $style = 'class="text" style="font-weight: normal"'; }
		switch($value['source']) {
			case 'db_table':
				if($value['hidden'] == 'true') break;
				switch($value['format']) {
					case 'blob':
					case 'country':
					case 'text':
						?><td align="left" bgcolor="<?=$GLOBALS['design']['table_header']?>" <?=$style?>><?php
				        	?><a href="<?=get_url($GLOBALS['id'])?>?page=<?=$_REQUEST['page']?>&order=<?=$value['name']?>&target_id=<?=$GLOBALS['target_id']?>&first_item=<?=$_REQUEST['first_item']?>&search=<?=urlencode($_REQUEST['search'])?>"><?=$value['label']?></a><?php
						echo '</td>';
						break;
					case 'number':
						?><td align="right" bgcolor="<?=$GLOBALS['design']['table_header']?>" <?=$style?>><?php
				        	?><a href="<?=get_url($GLOBALS['id'])?>?page=<?=$_REQUEST['page']?>&order=<?=$value['name']?>&target_id=<?=$GLOBALS['target_id']?>&first_item=<?=$_REQUEST['first_item']?>&search=<?=urlencode($_REQUEST['search'])?>"><?=$value['label']?></a><?php
						echo '</td>';
						break;
					case 'timestamp':
						?><td align="right" bgcolor="<?=$GLOBALS['design']['table_header']?>" <?=$style?>><?php
				        	?><a href="<?=get_url($GLOBALS['id'])?>?page=<?=$_REQUEST['page']?>&order=<?=$value['name']?>&target_id=<?=$GLOBALS['target_id']?>&first_item=<?=$_REQUEST['first_item']?>&search=<?=urlencode($_REQUEST['search'])?>"><?=$value['label']?></a><?php
						echo '</td>';
						break;
					case 'bool':
						?><td align="center" bgcolor="<?=$GLOBALS['design']['table_header']?>" <?=$style?>><?php
				        	?><a href="<?=get_url($GLOBALS['id'])?>?page=<?=$_REQUEST['page']?>&order=<?=$value['name']?>&target_id=<?=$GLOBALS['target_id']?>&first_item=<?=$_REQUEST['first_item']?>&search=<?=urlencode($_REQUEST['search'])?>"><?=$value['label']?></a><?php
						echo '</td>';
						break;
					default:
						?><td align="center" bgcolor="<?=$GLOBALS['design']['table_header']?>" <?=$style?>><?php
				        	?><a href="<?=get_url($GLOBALS['id'])?>?page=<?=$_REQUEST['page']?>&order=<?=$value['name']?>&target_id=<?=$GLOBALS['target_id']?>&first_item=<?=$_REQUEST['first_item']?>&search=<?=urlencode($_REQUEST['search'])?>"><?=$value['label']?></a><?php
						echo '</td>';
						break;
					}
				break;
			case 'edit':
				?><td bgcolor="<?=$GLOBALS['design']['table_header']?>" <?=$style?> align="center"><?=$value['label']?></td><?php
				break;
			default:
				?><td bgcolor="<?=$GLOBALS['design']['table_header']?>" <?=$style?> align="left"><?=$value['label']?></td><?php
				break;

			}
		}
	echo '</tr>';
	}

function adf_display_table_footer ($cols, $order, $count, $wrap_count, $first_item) {
	?></td></tr></table>
	</table><?php
	adf_display_page_links ($order, $count, $wrap_count, $first_item);
	}

function adf_delete_items ($tables, $id_list) {
	while(list($key, $value) = each($tables)) {
		$pre = '';
		$where = '';

		if($value['type'] == 'join' || $value['type'] == 'primary') {
			reset($id_list);
			while(list($item_nr, $item) = each($id_list)) {
				// this is done for security reasons
				if(!is_numeric($item)) return false;
				$where .= $pre.$value['key'].' = '.$item;
				$pre = ' OR ';
				}
			$pre = ' AND ';
			}

		if(isset($value['delete_filter']) && $value['delete_filter'] != '') $where .= $pre.'('.$value['delete_filter'].')';
		if($where != '') sql_query("DELETE FROM ".$value['name']." WHERE ".$where);
		}
	}

function adf_switch_bool ($tables, $cols, $col, $id_list) {
	$pre = '';
	$where = '';

	reset($id_list);

	while(list($item_nr, $item) = each($id_list)) {
		// this is done for security reasons
		if(!is_numeric($item)) return false;
		$where .= $pre.$tables[$cols[$col]['table']]['key'].' = '.$item;
		$pre = ' OR ';
		}

	if(isset($value['filter']) && $value['filter'] != '') $where .= ' AND ('.$value['filter'].')';

	$qres = sql_query("SELECT ".$cols[$col]['name']." FROM ".$cols[$col]['table']." WHERE ".$where." LIMIT 1");
	$row = sql_fetch_row($qres);
	sql_free_result($qres);

	if($row[0] == '1') {
		sql_query("UPDATE ".$cols[$col]['table']." SET $col = 0 WHERE ".$where."\n");
		}
	else {
		sql_query("UPDATE ".$cols[$col]['table']." SET $col = 1 WHERE ".$where."\n");
		}
	}

function adf_link_field ($field, $link, $row) {
	if(isset($link) && $link != '') {
		$link_targets = explode('%%', $link);
		for($i = 1; $i < count($link_targets); $i = $i + 2) {
			$link = str_replace('%%'.$link_targets[$i].'%%', $row[$link_targets[$i]], $link);
			}
		return '<a href="'.$link.'">'.$field.'</a>';
		}
	return $field;
	}

function adf_display_table_row ($row, $cols, $target) {
	static $color;

	if($color == $GLOBALS['design']['table_row_bright']) {
		$color = $GLOBALS['design']['table_row_dark']; }
	else {
		$color = $GLOBALS['design']['table_row_bright']; }

	echo '<tr bgcolor="'.$color.'">';

        foreach ( $cols as $key => $value) {
		switch($value['source']) {
			case 'db_table':
				if($value['hidden'] == 'true') break;
				switch($value['format']) {
					case 'country':
						echo '<td align="left" class="text" valign="top">';
						echo adf_link_field(code_to_country($row[$key]), $value['link'], $row);
						echo '&nbsp;</td>';
						break;
					case 'blob':
						echo '<td align="center" class="text" valign="top">';
						if(strlen($row[$key]) == 32){
							echo get_image_tag('img/small_edit/download.gif', 'Download', 'get_blob.php?sid='.$row[$key]);
							}
						echo '&nbsp;</td>';
						break;
					case 'text':
						echo '<td align="left" class="text" valign="top">';
						echo adf_link_field(stripslashes($row[$key]), $value['link'], $row);
						echo '&nbsp;</td>';
						break;
					case 'number':
						echo '<td align="right" class="text" valign="top">&nbsp;';
						echo adf_link_field($row[$key], $value['link'], $row);
						echo '</td>';
						break;
					case 'timestamp':
						echo '<td align="right" class="text" valign="top">';
						echo adf_link_field(timestamp_to_string($row[$key]), $value['link'], $row);
						echo '</td>';
						break;
					case 'bool':
						if(isset($value['changeable']) && $value['changeable'] == 'true') {
							unset($params);
							$params['page'] = $_REQUEST['page'];
							$params['switch'] = $row['id'];
							$params['col'] = $value['name'];
							$params['first_item'] = $_REQUEST['first_item'];
							$params['order'] = $_REQUEST['order'];
							$params['search'] = $_REQUEST['search'];
							$params['target'] = $GLOBALS['target'];
							$params['target_id'] = $GLOBALS['target_id'];

							echo '<td align="center" valign="top"><a href="'.get_url($GLOBALS['id'],$params).'">';
							if($row[$key] == '1') { echo '<img src="img/small_edit/checked.gif" border="0">'; }
							else { echo '<img src="img/small_edit/not_checked.gif" border="0">'; }
							echo '</a></td>';
							}
						else {
							echo '<td align="center" valign="top">';
							if($row[$key] == '1') { echo '<img src="img/small_edit/checked.gif" border="0">'; }
							else { echo '<img src="img/small_edit/not_checked.gif" border="0">'; }
							echo '</td>';
							}
						break;
					default:
						echo '<td align="center" class="text" valign="top">&nbsp;';
						echo adf_link_field(transform($row[$key], $value['format']), $value['link'], $row);
						echo '</td>';
						break;
					}
				break;
			case 'edit':
				unset($params);
				$params['page'] = $_REQUEST['page'];
				$params['first_item'] = $_REQUEST['first_item'];
				$params['order'] = $_REQUEST['order'];
				$params[$value['function']] = $row['id'];
				$params['search'] = $_REQUEST['search'];
				$params['target'] = $GLOBALS['target'];
				$params['target_id'] = $GLOBALS['target_id'];

				echo '<td align="center" valign="top"><a href="'.get_url($GLOBALS['id'],$params).'" onclick="return confirm(\''.LANG_ARE_YOU_SURE.'\')"><img src="img/small_edit/'.$value['function'].'.gif" border="0" /></a></td>';
				break;
			default:
				echo '<td align="left" valign="top" class="text">'.adf_get_string($value,$target,$row['id']).'&nbsp;</td>';
				break;
			}
                }
	echo '</tr>';
	}

function adf_sql_prepare_field ($value, $col_value) {
	switch($col_value['format']) {
		case 'hidden_number':
			return $col_value['value'];
			break;
		case 'hidden_text':
			return "'".addslashes($col_value['value'])."'";
			break;
		case 'bool':
		case 'number':
			if(!is_numeric($value)) return false;
			return $value;
			break;
		case 'country':
		case 'textarea':
		case 'textarea_xxl':
		case 'random':
		case 'text':
			return "'".addslashes($value)."'";
			break;
		case 'timestamp':
			if(!is_numeric($value['month']) || !is_numeric($value['day']) || !is_numeric($value['year'])) return false;
			return mktime(0,0,0,$value['month'],$value['day'],$value['year']);
			break;
		}
	}

function adf_add_record ($fields, $tables, $cols) {
	while(list($key, $value) = each($tables)) {
		$_set = '';
		$pre  = '';
		reset($cols);
		while(list($col_key, $col_value) = each($cols)) {
			if($col_value['table'] == $value['name']) {
				$field = adf_sql_prepare_field($fields[$col_value['name']],$col_value);
				if($field === false) return false;
				$_set .= $pre.$col_value['name'].' = '.$field;
				$pre = ', ';
				}
			}

		if($value['set_timestamp'] == 'true') {
			$_set .= $pre.'created  = '.time().', last_change = '.time();
			}

		switch($value['type']) {
			case 'primary':
				$primary_query = ("INSERT INTO ".$value['name']." SET ".$_set);
				break;
			case 'join':
				if($_set != '') {
					$join_query[$value['name']] = "INSERT INTO ".$value['name']." SET ".$_set;
					if(isset($value['filter'])) {
						$join_query[$value['name']] .= ', '.$value['filter'];
						}
					$join_query[$value['name']] .= ', '.$value['key'].' = ';
					}
				break;
			}
		}

//	echo("<pre>$primary_query\n");
	$qres = sql_query($primary_query);
	$insert_id = sql_insert_id($qres);
	sql_free_result($qres);

	if($insert_id != 0) {
		while(list($key, $value) = each($join_query)) {
//			echo($value.$insert_id);
			sql_query($value.$insert_id);
			}
		}

//	echo '</pre>';

	return $insert_id;
	}

function db_get_array ($table, $key, $target_id) {
	$result = array();
	$qres = sql_query("SELECT * FROM $table WHERE $key=$target_id");

	if(sql_num_rows($qres) > 0) {
		$row = sql_fetch_array($qres);
		sql_free_result($qres);
		foreach ( $row as $key => $value )  {
			$result[$key] = stripslashes($value);
			}
		$value = stripslashes($row[0]);
		}

	return $result;
	}

function db_get_string ($name, $type, $table, $key, $target_id) {

	// convert type parameter
	$type_array = explode(":", $type);
	$type = $type_array[0];

	$qres = sql_query("SELECT $name FROM $table WHERE $key=$target_id");

	if(sql_num_rows($qres) > 0) {
		$row = sql_fetch_row($qres);
		sql_free_result($qres);
		$value = stripslashes($row[0]);
		}
	else {
		$value = '';
		}

	switch($type) {
		case 'country':
			$value = code_to_country ($value);
			break;
		case 'timestamp':
			$value = transform($value, 'date');
			break;
		}

	return $value;
	}

function db_create_docinput ($name, $type, $table, $key, $target_id, $autosave = false, $set_date = true) {

	if($type == '') return false;

	if($autosave == true) {
		db_save_docinput($name, $type, $table, $key, $target_id, $set_date);
		}

	// convert type parameter
	$type_array = explode(":", $type);
	$type = $type_array[0];

	$qres = sql_query("SELECT $name FROM $table WHERE $key=$target_id");

	if(sql_num_rows($qres) > 0) {
		$row = sql_fetch_row($qres);
		sql_free_result($qres);
		$value = stripslashes($row[0]);
		}
	else {
		if($set_date) {
			sql_query("INSERT INTO $table ($key, created, last_change) VALUES ($target_id, ".time().", ".time().")");
			}
		else {
			sql_query("INSERT INTO $table ($key) VALUES ($target_id)");
			}

		if($type == 'text' || $type == 'textarea') {
			$value = '';
			}
		else {
			$value = 0;
			}
		}

	switch($type) {
		case 'textarea':
			echo '<textarea cols="60" wrap="virtual" rows="8" name="docinput[db]['.$name.']">'.htmlentities($value).'</textarea>';
			break;

		case 'textarea_xxl':
			echo '<textarea cols="60" wrap="virtual" rows="20" name="docinput[db]['.$name.']">'.htmlentities($value).'</textarea>';
			break;

		case 'text':
			echo '<input type="text" size="60" name="docinput[db]['.$name.']" value="'.htmlentities($value).'">';
			break;

		case 'number':
			echo '<input type="text" size="20" name="docinput[db]['.$name.']" value="'.htmlentities($value).'">';
			break;

		case 'password':
			echo '<input type="password" size="60" name="docinput[db]['.$name.']" value="'.htmlentities($value).'">';
			break;

		case 'short':
			echo '<input type="text" size="20" name="docinput[db]['.$name.']" value="'.htmlentities($value).'">';
			break;

		case 'bool':
			echo '<input type="checkbox" value="1" name="docinput[db]['.$name.']"';
			if($value == '1') echo ' checked="true"';
			echo '>';
			break;

		case 'country':
			echo '<select name="docinput[db]['.$name.']">'."\n";
			$countries = unserialize(join('', file(INC_PATH.DATA_PATH.'/countries.ser')));
			while(list($ckey, $cvalue) = each($countries)) {
				?>	<option value="<?=$ckey?>"<?php if($value == $ckey) echo ' selected="true"'; ?>><?=$cvalue?></option><?php
				echo "\n";
			        }
			echo '</select>';
			break;

		case 'dropdown':
			$_ref_table  = $type_array[1];
			$_ref_key    = $type_array[2];
			$_ref_label  = $type_array[3];

			if(isset($type_array[4])) {
				$_ref_order  = 'ORDER BY '.$type_array[4];
				}
			else {
				$_ref_order  = '';
				}

			if(isset($type_array[5])) {
				$_ref_where = 'WHERE '.$type_array[5];
				}
			else {
				$_ref_where = '';
				}

			// echo("<pre>SELECT $_ref_key, $_ref_label FROM $_ref_table $_ref_where $_ref_order </pre>");
			$qres = sql_query("SELECT $_ref_key, $_ref_label FROM $_ref_table $_ref_where $_ref_order");

			echo '<select name="docinput[db]['.$name.']">'."\n";
			while($row = sql_fetch_row($qres)) {
				?>	<option value="<?=$row[0]?>"<?php if($value == $row[0]) echo ' selected="selected"'; ?>><?=htmlentities(stripslashes($row[1]))?></option><?php
				echo "\n";
			        }
			echo '</select>';

			sql_free_result($qres);

			break;

		case 'timestamp':
			if(isset($value) && is_numeric($value)) {
				$date = getdate($value);
				}
			else {
				$date = getdate();
				}

                        echo "\t".'<select name="docinput[db]['.$name.'][day]">'."\n";
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
                        echo "\t".'<select name="docinput[db]['.$name.'][month]">'."\n";
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
                        echo "\t".'<select name="docinput[db]['.$name.'][year]">'."\n";
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
	}

function db_save_docinput($name, $type, $table, $key, $target_id, $set_date = true) {
	if(!is_numeric($target_id) || $target_id == 0) return false;

	// convert type parameter
	$type_array = explode(":", $type);
	$type = $type_array[0];
	$value = $GLOBALS['docinput']['db'][$name];
	switch($type) {
		case 'timestamp':
			if(!is_numeric($value['month']) || !is_numeric($value['day']) || !is_numeric($value['year'])) return false;
			$value = mktime(0,0,0,$value['month'],$value['day'],$value['year']);
			break;
		case 'bool':
			if($value == '1') { $value = '1'; }
			else		  { $value = '0'; }
			break;
		case 'dropdown':
		case 'number':
		case 'textarea':
		case 'textarea_xxl':
		case 'country':
		case 'text':
		case 'short':
		case 'password':
			$value = "'".addslashes($value)."'";
			break;
		}

	$last_change = '';

	if($set_date == true) $last_change = ", last_change = ".time();

	sql_query("UPDATE $table SET $name = $value $last_change WHERE $key=$target_id");
	}

function profile_remove_profile ($name, $user_id) {
        // removes a key/value-pair from a user profile

        // enforce secure db queries
        if ($name == '' || !is_numeric($user_id)) return false;
        $name = addslashes($name);

        $qresult = sql_query ("DELETE FROM ".TABLE_USERDATA." WHERE name='".$name."' AND user_id=".$user_id);

        return true;
        }

function profile_get_profile ($user_id, $use_groups = true) {
        // returns a user profile

        // enforce secure db queries
        if(!is_numeric($user_id) || !is_bool($use_groups)) return false;

        $qprofile = sql_query ("SELECT name, value FROM ".TABLE_USERDATA." WHERE user_id=".$user_id);
               if(sql_num_rows($qprofile) > 0) {
                while($prow = sql_fetch_row($qprofile)) {
                        $profile[stripslashes($prow[0])] = stripslashes($prow[1]);
                        if(substr($prow[0], 0, 6) == 'group_' && $prow[1] == '1' && $use_groups) {
                                $profile = array_merge(get_group_profile (substr($prow[0], 6)), $profile);
                                }
                        }
                sql_free_result($qprofile);
                }

        return $profile;
        }

function profile_set_profile ($name, $value, $user_id) {
	// enforce secure db queries
        if (($name == '' && $value == '') || !is_numeric($user_id)) return false;

	$value = addslashes($value);
        $name = addslashes($name);
        if ($value == '' && $name != '') {
                sql_query("DELETE FROM ".TABLE_USERDATA." WHERE name='".$name."' AND user_id=".$user_id);
                return true;
                }
        $qresult = sql_query ("UPDATE ".TABLE_USERDATA." SET value='".$value."' WHERE name='".$name."' AND user_id=".$user_id);
        if(sql_affected_rows() == 0) {
                $res = sql_query("SELECT id FROM ".TABLE_USERDATA." WHERE name='".$name."' AND user_id=".$user_id);
                if(sql_num_rows($res) > 0) {
                        return true; }
                else {
                        sql_query ("INSERT INTO ".TABLE_USERDATA." (name, value, user_id) VALUES ('".$name."', '".$value."', ".$user_id.")");
                        }
                }
        return true;
	}

function profile_get_string ($name, $type, $target_id = -1) {
	if($target_id == -1 || $target_id == SESSION_DBID) {
		$profile = $GLOBALS['profile'];
		}
	else {
		$profile = profile_get_profile($target_id);
		}

	return $profile[$name];
	}

function profile_save_docinput ($name, $type, $target_id = -1) {
	if($target_id == 0) return false;

	$value = $GLOBALS['docinput'][$name];

	if(($target_id == -1 || $target_id == SESSION_DBID)) {
		$GLOBALS['profile'][$name] = $value;
		}

	switch($type) {
		case 'timestamp':
			if(!is_numeric($value['month']) || !is_numeric($value['day']) || !is_numeric($value['year'])) return false;
			return profile_set_profile($name, mktime(0,0,0,$value['month'],$value['day'],$value['year']), $target_id);
		case 'bool':
			if($value == '1') { return profile_set_profile($name, '1', $target_id); }
			else		  { return profile_remove_profile($name, $target_id); }
		case 'text':
			return profile_set_profile($name, $value, $target_id);
		}

	}

function profile_create_docinput ($name, $type, $target_id = -1, $autosave = false) {
	if(isset($autosave)) profile_save_docinput($name, $type, $target_id);

	if(($target_id == -1 || $target_id == SESSION_DBID)) {
		$profile = $GLOBALS['profile'];
		}
	else {
		$profile = profile_get_profile($target_id);
		}

	switch($type) {
		case 'text':
			echo '<input type="text" size="60" name="docinput['.$name.']" value="'.$profile[$name].'">';
			break;

		case 'number':
			echo '<input type="text" size="20" name="docinput['.$name.']" value="'.$profile[$name].'">';
			break;

		case 'small_text':
			echo '<input type="text" size="37" name="docinput['.$name.']" value="'.$profile[$name].'">';
			break;

		case 'bool':
			echo '<input type="checkbox" value="1" name="docinput['.$name.']"';
			if($profile[$name] == '1') echo ' checked="true"';
			echo '>';
			break;

		case 'timestamp':
			if(isset($profile[$name])) {
				$date = getdate($profile[$name]);
				}
			else {
				$date = getdate();
				}

                        echo "\t".'<select name="docinput['.$name.'][day]">'."\n";
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
                        echo "\t".'<select name="docinput['.$name.'][month]">'."\n";
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
                        echo "\t".'<select name="docinput['.$name.'][year]">'."\n";
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
	}

function adf_create_docinput ($attributes, $target, $target_id, $autosave = false, $tables = array()) {

// $source, $name, $target, $target_id, $type, $autosave = false, $table, $tables = array()) {

	// returns db field as input element

	$code = get_code ('adf_inputs', $attributes['source']);

	if($code != '') {
		return eval($code);
		}

	}

function adf_get_string ($attributes, $target, $target_id, $tables = array()) {
	// returns db field as string

	$code = get_code ('adf_strings', $attributes['source']);

	if($code != '') {
		return eval($code);
		}

	}

function update_target ($node) {
	global $target;
	global $target_id;
	if($node->get_attribute('target') != '') 	$target = $node->get_attribute('target');
	if($node->get_attribute('target_id') !='')	$target_id = eval('return "'.utf8_decode($node->get_attribute('target_id')).'";');
	}

function has_access ($node, $profile, $action) {
	return ($node->get_attribute($action.'_user_access') == '' || $node->get_attribute($action.'_user_access') == SESSION_EMAIL)
		&& ($node->get_attribute($action.'_group_access') == '' || $profile['group_'.$GLOBALS['groups_ids'][$node->get_attribute($action.'_group_access')]] == '1');
	}
