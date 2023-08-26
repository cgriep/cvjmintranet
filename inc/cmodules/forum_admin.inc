<?php
/*
 * Created on 30.01.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
 
 if ( $profile["editor"] != 1)
 {
 	if ( isset($docinput["RemoveMF"]))
 	{
 		foreach ($docinput["RemoveMF"] as $forum_id => $User)
 		  foreach ( $User as $user_id)
 		  {
 		    remove_profile("MailForum_$forum_id", $user_id);
 		    echo get_user_nickname($user_id)." aus Forum $forum_id entfernt.<br />";
 		  }
 	}
 	if ( $document['title'] != '') 
 	  echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
    echo "Folgende Foren sind von den jeweils angegebenen Personen abonniert:\n";
    // Alle Foren auflisten
    if ( !$query = sql_query("SELECT id FROM ".TABLE_NODES.
      " WHERE type_id=".$doctypes["forum_forum"])) echo sql_error();
    echo '<form action="'.get_url($id).'" methode="post">';
    while ( $node = sql_fetch_row($query))
    {
      echo $design["table_begin"];
      $nodedata = get_nodedata($node[0],$flavour);
      echo '<tr class="ueberschriftzeile"><th colspan="2">'.
        transform($nodedata["title"],$nodedata["datatype"]["title"]).'</th></tr>';
      $q2 = sql_query("SELECT * FROM ".TABLE_USERDATA." WHERE name LIKE 'MailForum_{$node[0]}'");
      $user = array();
      while ( $userdata = sql_fetch_array($q2) )
        echo "<tr><td>\n".get_user_nickname($userdata["user_id"]).
          "</td><td><input type=\"checkbox\" name=\"docinput[RemoveMF][{$node[0]}][]" .
          "\" value=\"{$userdata["user_id"]}\"/></td></tr>\n";
      if ( sql_num_rows($q2) == 0 )
        echo '<tr><td colspan="2">keine Abonnenten</td></tr>';
      sql_free_result($q2);
      echo "</table>\n";	
    }
    echo '<input type="hidden" name="id" value="'.$id.'" />';
    echo '<input type="submit" value="Markierte Abonnenten aus der Liste entfernen"/>';
    echo '</form>';
    sql_free_result($query);
    // dazu alle Benutzer mit MailForum_* auflisten -> Abmelden und Anmelden
 } 
 
?>
