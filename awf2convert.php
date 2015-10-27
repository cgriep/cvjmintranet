<?php
/*
 * Created on 03.01.2007 Christoph Griep
 * Konvertierung AWF nach AWF2 
 */
 
 $db_host        = "localhost";
$db_name        = "awf";
$db_username    = "awf";
$db_password    = "c3myNawf";
$db_table_prefix= "awf_";
 
if(!mysql_connect($db_host,$db_username,$db_password)) echo mysql_error();
mysql_select_db('awf');

 /**
  * Teil 1: Termine
  */
  $sql = 'SELECT * FROM awf_nodes WHERE type_id=46'; // 46 = Termin
  if (! $query= mysql_query($sql)) die(mysql_error());
  
  while ($row = mysql_fetch_array($query))
  {
  	mysql_select_db('awf');
  	$sql = 'SELECT * FROM awf_nodedata WHERE node_id='.$row['id'];
  	$q2 = mysql_query($sql);
  	$daten = array();
  	while ( $data = mysql_fetch_array($q2))
    {
    	$daten[$data['name']] = $data['value'];
    }    
  	mysql_free_result($q2);
  	mysql_select_db('awf2');
  	$isql = 'INSERT INTO cvjm_Termine (Titel,Beschreibung,' .
  				'Autor,changed,Datum,Dauer,Turnus,TurnusEnde,Art,' .
  				'created,Betroffene) VALUES (';
  	$isql .= '"'.$daten['title'].'",';
  	$isql .= '"'.$daten['body'].'",';
  	$isql .= '"'.$daten['author_id'].'",';
  	$isql .= '"'.$daten['timestamp'].'",';
  	$isql .= '"'.$daten['Datum'].'",';
  	$isql .= '"'.($daten['Enddatum']-$daten['Datum']).'",';
  	switch ($daten['Turnus'])
  	{
  		case "j":
  			$isql .= '0,';
  			break;
  	    case 'w':
  	         $isql .= '2,';
  	         break;
  	    case 'm':
  	      $isql .= '1,';
  	      break;
  	    case 'd':
  	      $isql .= '3,';
  	      break;
  	    case 'v': 
  	      $isql .= '4,';
  	      break;
  	    default: 
  	      $isql .= 'NULL,';
  	}
  	
  	$isql .= '"'.$daten['TurnusEnde'].'",';
  	$isql .= '0,';
  	$isql .= '"'.$daten['timestamp'].'",';
  	$isql .= '"'.$daten['Gruppe'].':'.$daten['Person'].'")';
  	
  	if (!mysql_query($isql)) 
  	  echo 'Fehler: '.mysql_error().'<br />';
  	else
  	  echo 'eingefügt '.$isql.'<br />';    
  }
  mysql_free_result($query);
  /**
   * Teil 2: Aufträge
   */
  $sql = 'SELECT * FROM cvjm_Auftraege'; // 46 = Termin
  mysql_select_db('awf');
  $query= mysql_query($sql);
  while ($row = mysql_fetch_array($query))
  {
  	 $isql = 'INSERT INTO cvjm_Termine (Titel,Beschreibung,Bemerkung,Status,' .
  				'Prioritaet,Autor,Ort,changed,Datum,Dauer,Turnus,TurnusEnde,Art,' .
  				'created,Betroffene) VALUES (';
     $isql .= '"'.$row['Titel'].'","';
     $isql .= $row['Beschreibung'].'","';
     $isql .= $row['Bemerkung'].'","';
     $isql .= $row['Status'].'","';
     $isql .= $row['Prioritaet'].'","';
     $isql .= $row['Autor'].'","';
     $isql .= $row['Ort'].'","';
     $isql .= $row['Aenderung'].'","';
     $isql .= strtotime($row['Datum']).'","';
     $isql .= '0",NULL,NULL,1,'.$row['Aenderung'];     
     $isql .= $row['Betroffene'].')';
     mysql_select_db('awf2');
     mysql_query($isql);
  }
  mysql_free_result($query);
?>
