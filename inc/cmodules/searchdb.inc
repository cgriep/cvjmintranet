<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003

	This file was modified on 27.02.2003 by Paul Maxwell (hax@awfers.org)
*/

load_lang('search');

$search = sql_real_escape_string(trim($_REQUEST['search']));
$manner = $_REQUEST['manner'];

// if not set, default to 1 (phrase)
if (!isset($manner)) { $manner = 1; }

if($profile['editor'] != 1) {
?>
<p>
<form action="<?=$_SERVER['PHP_SELF']?>" method="get">
<input type="hidden" name="id" value="<?=$id?>">
<input type="text" size="50" name="search" value="<?=$search?>">

<input type="submit" value="<?=LANG_SEARCH?>"><br />
<?php
if ($manner == 1) {
echo '<input type="radio" value="1" name="manner" checked>'.LANG_SEARCH_PHRASE;
echo '<input type="radio" value="2" name="manner">'.LANG_SEARCH_ALL;
echo '<input type="radio" value="3" name="manner">'.LANG_SEARCH_ANY;
}
elseif ($manner == 2) {
echo '<input type="radio" value="1" name="manner">'.LANG_SEARCH_PHRASE;
echo '<input type="radio" value="2" name="manner" checked>'.LANG_SEARCH_ALL;
echo '<input type="radio" value="3" name="manner">'.LANG_SEARCH_ANY;
}
elseif ($manner == 3) {
echo '<input type="radio" value="1" name="manner">'.LANG_SEARCH_PHRASE;
echo '<input type="radio" value="2" name="manner">'.LANG_SEARCH_ALL;
echo '<input type="radio" value="3" name="manner" checked>'.LANG_SEARCH_ANY;
}
?>
</form>
</p>
<br />
<?php

if(isset($search) && $search && is_numeric($flavour)) {

  if ( is_numeric($search) )
  {
    // nach id suchen
    $search_results = search_nodes ("node_id=$search");
    if($search_results)
      foreach ($search_results as $value )
      echo '<div style="background-color:'.$design['table_header'].
        '">Direkt zu Seite Nummer '.$search.' mit dem Titel '.
        transform("%%$search%%","extrans")."</div>";
  }

// Links auf die anderen Suchbereiche anzeigen
if ( isset($document["DBNamen"]) )
{
  $document["DBNamen"] = explode("\n",$document["DBNamen"]);
  echo "Fundstellen in";
  echo "<ul>";
  foreach ( $document["DBNamen"] as $key => $value) 
  {
    if ( $value != "" )
    {
      $s = explode(":",$value);
      echo '<li><a href="#'.$s[1].'">'.$s[0].'</a></li>';
    }
    else
      unset($document["DBNamen"][$key]);
  }
  echo "</ul>";
}
$lookfor = $search;
$lookforHTML = htmlentities($search);

if ($manner > 1) {

	//replace multiple spaces with single spaces

	// need code for this ... is this the best way?
	$lookfor = ereg_replace ('  ', ' ', $lookfor);
	$lookforHTML = ereg_replace ('  ', ' ', $lookforHTML);
	
	if ($manner == 3) {

		//replace single spaces with sql |%' OR LIKE '%| statements
		// eg
		// $search='1stword 2ndword 3rdword'
		// becomes '1stword%' OR LIKE '%2ndword%' OR LIKE '%3rdword'

		// need code for this ...will this work?
		$lookfor = ereg_replace (' ', '%" OR value LIKE "%', $lookfor);
		$lookforHTML = ereg_replace (' ', '%" OR value LIKE "%', $lookforHTML);
		$way = LANG_SEARCH_ANY;
	}

	elseif ($manner == 2) {

		//replace single spaces with sql |%' AND LIKE '%| statements
		// eg
		// $search='1stword 2ndword 3rdword'
		// becomes '1stword%' AND LIKE '%2ndword%' AND LIKE '%3rdword'

		// need code for this ...will this work?
		$lookfor = ereg_replace (' ', '%" AND value LIKE "%', $lookfor);
     	$lookforHTML = ereg_replace (' ', '%" AND value LIKE "%', $lookforHTML);
		$way = LANG_SEARCH_ALL;
	}

}
else {
	$way = LANG_SEARCH_PHRASE;
}

if(DBMS == 'mysql') {
  $where = '(flavour_id = '.$flavour.' OR flavour_id = 0) AND (value LIKE "%'.
    $lookfor.'%" OR value LIKE "%'.$lookforHTML.'%")';
}
else { $where = "value LIKE '%".$lookfor."%'"; }

$search_results = search_nodes ($where);

if($search_results) {
	?>
	<table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="#ffffff"><tr>
	<td align="left" bgcolor="<?=$design['table_header']?>">
	<?php printf(LANG_SEARCHED_THIS_SITE.'<br />('.$way.')','<b>'.$search.'</b>'); ?>	</td>
	<td align="right" bgcolor="<?=$design['table_header']?>">
	<?php printf(LANG_RESULT_COUNT, count($search_results), $search_time); ?>
	</td>
	</tr>
	</table>
	<table width="100%" border="0" cellpadding="5" cellspacing="0" bgcolor="#ffffff">
	<?php
	$counter = 0;
	foreach ($search_results as $key => $value)  {
		$counter++;
		echo '<tr><td align="right" bgcolor="'.$design['table_row_dark'].'" valign="top" width="5%">'.$counter.'</td>
	        <td align="left" bgcolor="'.$design['table_row_bright'].'" valign="top" width="95%">
		<b><a href="'.get_url($key).'">'.transform($value['title'], 'clean').'</a></b><br />';
		if(isset($value['author_id'])) printf(LANG_RESULT_CREATED_BY.' ', transform($value['author_id'], 'user_id'));
		echo(transform($value['timestamp'], 'datetime'));
		if(COUNT_VIEWS==1) {
			echo(', '.$value['views'].' '.LANG_VIEWS.', '.count_children($doctypes['comment'], $flavour, $key).' '.LANG_COMMENTS_LOW);
			}
		echo('</td></tr>');
		}
	?>
	</table>
	<?php
	}
else {
	printf('<p>'.LANG_NOT_FOUND.' ('.$way.')</p>','<b>'.$search.'</b>');
	}
 // Datenbanken durchsuchen
 reset($document["DBNamen"]);
 foreach ( $docment["DBNamen"] as $key => $value) 
 {
   echo '<hr />';
   list($Beschreibung,$seitenid, $param, $Datenbankname, $Felder) = explode(":",$value);
   $Felder = trim($Felder);
   $FeldArray = explode("|",$Felder);
   $Namen = array();
   foreach ( $FeldArray as $key2 => $value2 )
   {
     list($kkey, $vvalue) = explode(";",$value2);
     if ( $vvalue != "" )
     {
       $FeldArray[$key2] = $vvalue;
       $Namen[$key2] = $kkey;
     }
     else
       $Namen[$key2] = $value2;
   }
   reset($FeldArray);
   $FeldString = implode(",",$FeldArray); //explode("|",$Felder));
   $Suche = str_replace(" ","|",$search);
   if ( $manner == 1 )
     $SearchString = implode(" LIKE '%".$search."%' OR ",$FeldArray)." LIKE '%".$search."%'";
   elseif ( $manner == 2 )
   {
     $SearchString = "";
     $fel = explode("|", $FeldArray);
     foreach ( $fel as $kkey => $vvalue) 
     {
       if ( $SearchString == "" )
         $SearchString = "(";
       else
         $SearchString .= " OR (";
       $fel2 = explode(" ", $search);
       foreach ( $fel2 as $kkkey => $vvvalue) 
         $SearchString .= $vvalue." REGEXP '".$vvvalue."' AND ";
       $SearchString .= "1)";
     }
   }
   else
     $SearchString = implode(" REGEXP '$Suche' OR ", explode("|",$FeldArray))." REGEXP '$Suche'";
   echo '<a id="'.$seitenid.'" name="'.$seitenid.'"></a><h2>Suche in '.$Beschreibung.'</h2>';
   if ( strpos($Datenbankname,"|") !== false )
   {
     list($Datenbankname, $suchen) = explode("|",$Datenbankname);
     $sql = "SELECT $FeldString FROM $Datenbankname WHERE ($SearchString) AND $suchen";
   }
   else
     $sql = "SELECT $FeldString FROM $Datenbankname WHERE $SearchString";
   if ( ! $query = sql_query($sql) )
     echo sql_error();
   $Felder = explode("|", $Felder);
   while ( $found = sql_fetch_array($query) )
   {
     reset($FeldArray);
     foreach ( $FeldArray as $kkey => $vvalue) 
     {
       if ( $kkey == 0 )
         echo '<a href="'.get_url($seitenid,$param."=".$found[$vvalue]).'">';
       else
         echo $Namen[$kkey].": ".transform($found[$vvalue], "text");
       if ( $kkey == 1 )
         echo '</a>';
       echo "<br />";
     }
     echo "<br />";
   }
   if ( sql_num_rows($query) == 0 )
     echo 'Keine Fundstellen.<br />';
   sql_free_result($query);
 }

} // if search
} // if Editor
else
  create_editor_input("Zusätzlich durchsuchte Datenbanken", "DBNamen", "area_input");

?>