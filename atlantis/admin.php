<?php
session_start();
?>
<html>
<body>
<?php
/*
 * Created on 16.11.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
 include('atlantis.db.php');
 echo '<form action="'.$_SERVER['PHP_SELF'].'" method="post">';
 echo '<select name="Tabelle">';
 echo '<option value="T_Spezialisierungen">Spezialisierungen</option>';
 echo '<option value="T_Fertigkeiten">Fertigkeiten</option>';
 echo '<option value="T_Rassen">Rassen</option>';
 echo '<option value="T_Spezialisierungsklassen">Spezialisierungsklassen</option>';
 echo '</select>';
 echo '<input type="submit" value="Wählen" />';
 echo '</form>';
 if ( isset($_REQUEST['Tabelle']))
 {
   $_SESSION['Tabelle'] = $_REQUEST['Tabelle'];
   session_unregister('Eintrag');
 }
 if ( session_is_registered('Tabelle'))
 {
 	echo '<form action="'.$_SERVER['PHP_SELF'].'" method="post">';
    echo '<select name="Eintrag">';
    $query = mysql_query('SELECT * FROM '.$_SESSION['Tabelle']);
    while ( $row = mysql_fetch_array($query))
    {
    	echo '<option value="'.$row[0].'"';
        if ( isset($_REQUEST['Eintrag']) && $_REQUEST['Eintrag'] == $row[0])
        {
          echo 'selected="selected"';
          $_SESSION['Eintrag'] = $row;          
        }
        echo '>';
    	echo $row[1];
    	echo '</option>'."\n";
    }
    echo '</select>';
    echo '<input type="submit" value="Wählen" />';
    echo '</form>';
    if ( session_is_registered('Eintrag'))
    {
 	  if ( isset($_REQUEST['Bemerkung']))
 	  {
 	  	$key = array_keys($_SESSION['Eintrag']);
 	  	//print_r($key);
 	  	$sql = 'UPDATE '.$_SESSION['Tabelle'].' SET Beschreibung="'.
 	  	  sql_real_escape_string($_REQUEST['Bemerkung']).'" WHERE '.$key[1].
 	  	  '='.$_SESSION['Eintrag'][0];
 	  	 echo $sql;
 	  	if ( ! mysql_query($sql)) echo mysql_error();
 	  	$_SESSION['Eintrag']['Beschreibung'] = $_REQUEST['Bemerkung'];
 	  }

 	  echo '<form action="'.$_SERVER['PHP_SELF'].'" method="post">';
 	  echo $_SESSION['Eintrag'][1].'<br/>';
      echo '<textarea name="Bemerkung" cols="80" rows="8">';
      if( isset($_SESSION['Eintrag']['Beschreibung']))
        echo $_SESSION['Eintrag']['Beschreibung'];
      echo '</textarea>';
      echo '<input type="submit" value="Speichern" />';
      echo '</form>';
      if( isset($_SESSION['Eintrag']['Beschreibung']))
      {      	
        echo 'HTML-Ansicht:<br />';
        echo $_SESSION['Eintrag']['Beschreibung'];
      }
    }           
 }
?>
</body>
</html>
