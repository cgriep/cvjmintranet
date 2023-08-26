<?php
/*

        List the last 10 documents, regardless of what type they are or the parent/child relationship

        By Paul Maxwell (pmaxwell@cassys.biz)
        Last update: 11.10.2002

*/

// would like to take advantage of chace but don't know how yet
// so fo now we 'just do it'

global $doctypes;
global $profile;
global $enabled_filter;
// to avoid forum_mailto records choose all but type 28
$sql = "SELECT id FROM " . TABLE_NODES . " where type_id<>".$doctypes['forum_mailto'].
 " AND type_id<>".$doctypes['Termin']
." $enabled_filter order by last_change desc limit 10";
$qresult = sql_query($sql);
// should have logic here if no results returned (maybe not required?)

echo '<span style="font-size: 10px">';

while($row = sql_fetch_row($qresult)) {

   $tresult = sql_query("SELECT value FROM " . TABLE_NODEDATA . " WHERE node_id=$row[0] and name='title' LIMIT 1");
         $trow = sql_fetch_row($tresult);
         echo '<img src="',$design['arrow_bullet'],'" width="10" height="7" />';
         echo '<a href="',get_url($row[0]),'">';
         $trow[0] = str_replace("RE: ", "",$trow[0]);
         if(strlen($trow[0]) > 35) { $trow[0] = substr($trow[0], 0, 32).'...'; }
         if ( $trow[0] == "" ) $trow[0] = "(unknown)";
         echo stripslashes(transform($trow[0],"clean")),'</a><br />';
         sql_free_result($tresult);
}
sql_free_result($qresult);
echo '</span>';

?>