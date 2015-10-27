<?php

/*

awf version: 1.04
Purpose:   To produce a variant of the site map, where only recent changes are listed
By:        Paul Maxwell (awfers.org) 17 Feb 2003
Version:   1.0
Install:   used in a text paragraph node as php code

*/

load_lang('rum');

function get_children_summary ($type_id = -1, $flavour_id = -1, $item_id = -1) {
// gather all documents having the given parent id
global $enabled_filter;

$fields = 'MIN(created) AS created , MAX(last_change) AS last_change, COUNT(node_id) AS num_of, SUM(views) AS num_views';

// enforce secure db queries
if(!is_numeric($type_id) || !is_numeric($flavour_id) || !is_numeric($item_id)) return false;

if($item_id > 0) { $id = $item_id; } else { $id = $GLOBALS['id']; }

if($type_id >= 0) { $filter = "AND type_id = $type_id"; } else { $filter = ''; }

if($flavour_id != -1) {
  $qresult = sql_query("SELECT $fields FROM ".TABLE_NODES.", ".TABLE_NODEDATA." WHERE ".TABLE_NODEDATA.".node_id=".TABLE_NODES.".id AND parent_id = $id AND (flavour_id = $flavour_id OR flavour_id = 0) AND name='title' $filter $enabled_filter GROUP BY ".TABLE_NODES.".parent_id") ;
  }
else {
  $qresult = sql_query("SELECT $fields FROM ".TABLE_NODES.", ".TABLE_NODEDATA." WHERE ".TABLE_NODEDATA.".node_id=".TABLE_NODES.".id AND parent_id = $id AND name='title' $filter $enabled_filter GROUP BY ".TABLE_NODES.".parent_id");
  }
if(sql_num_rows($qresult) > 0) {
  $row = sql_fetch_row($qresult);
  sql_free_result($qresult);
  return $row;
  }
else {
  return 0;
  }
}

function list_nodes ($parent_id = 0, $published = 1,$since_date = 1032204264, $lvlnow = 0) {
global $db_table_prefix;
global $enabled_filter;
global $flavour;
global $docnames;

if($published == 1) { $published_filter = 'AND published = 1'; }
elseif($published == 0) { $published_filter = 'AND published = 0'; }
else { $published_filter = ''; }
      $qresult = sql_query("SELECT ".TABLE_NODES.".id, name, value, type_id, published, enabled, searchable,
      cacheable, description, members_only, key_id, views FROM ".$db_table_prefix."nodes,
      ".TABLE_NODEDATA." WHERE parent_id=$parent_id AND node_id=".TABLE_NODES.".id
      AND flavour_id=$flavour AND name='title' $published_filter $enabled_filter ORDER BY sort_order DESC");

if(sql_num_rows($qresult) > 0) {
   echo '<ul>';
     $lvlnow = $lvlnow + 1;
   while($row = sql_fetch_row($qresult)) {
    $data =get_children_summary (-1, $flavour, $row[0]);

                // ONLY putput visible text IF it is after the $since_date
    if ($data[1] > $since_date) {
       echo '<li> <b><a href="'.$_SERVER['PHP_SELF'].'?id='.$row[0].
                '">'.strip_tags(stripslashes($row[2])).'</a></b>'.
                ' &nbsp;&nbsp;&nbsp; ', transform($data[1],'date').
                '</li>';
    }

  list_nodes($row[0],1,$since_date, $lvlnow);
  }
  sql_free_result($qresult);
  echo '</ul>';
  }
return(void);
}

if(!isset($document['num_days'])) { $document['num_days'] = 32; }
$num_days = $document['num_days'];
$secs_per_day = 86400;
$since_date = getmicrotime()- ($secs_per_day * $num_days);

echo LANG_UPDATED_AFTER . ' ', transform($since_date,'date');

// this 'does it' NB: this is an iterative function (it calls itself)
list_nodes(0,1,$since_date);

if($profile['editor'] == '1') {
  create_editor_input('Anzahl Tage','num_days','text_input');
}


?>