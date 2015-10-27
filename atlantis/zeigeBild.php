<?php
/*
 * Zeigt Bilder aus der Datenbank an
 * Created on 15.11.2006
 * Christoph Griep
 * 
 * Aufrufm�glichkeiten:
 * - "Bild": Das Bild des aktuellen Charakters wird angezeigt
 * - "Rasse":Bild der angegebenen Rasse (id) wird angezeigt
 * - "Klasse"
 * 
 */
 session_start();
  include('atlantis.db.php');
  $Tabelle = '';
  $Bild = '';
  if ( isset($_REQUEST['Bild']) && session_is_registered('Charakter_id'))
  {
  	$_REQUEST['Charakter'] = $_SESSION['Charakter_id'];
  }
  if ( isset($_REQUEST['Charakter']) && is_numeric($_REQUEST['Charakter']))
  {
  	$query = mysql_query('SELECT Bild FROM T_Charaktere ' .
			  'WHERE charakter_id='.$_REQUEST['Charakter']);
	if( $b = mysql_fetch_row($query))
	{
		  $Bild = $b[0];
  	}
	mysql_free_result($query);    
  }
  // Bild heraussuchen 
  if ( isset($_REQUEST['Rasse']) && is_numeric($_REQUEST['Rasse']) )
  {
    $query = mysql_query('SELECT Bild FROM T_Rassen ' .
			  'WHERE Rasse_id='.$_REQUEST['Rasse']);
	if( $beschreibung = mysql_fetch_row($query))
	{
		  $Bild = $beschreibung[0];
  	}
	mysql_free_result($query);  
  }
  if ( isset($_REQUEST['Klasse']) && is_numeric($_REQUEST['Klasse']) )
  {
    $query = mysql_query('SELECT Bild FROM T_Spezialisierungsklassen ' .
			  'WHERE Klasse_id='.$_REQUEST['Klasse']);
	if( $beschreibung = mysql_fetch_row($query))
	{
		  $Bild = $beschreibung[0];
  	}
	mysql_free_result($query);
  }  
  if ( isset($_REQUEST['Spezialisierung']) && is_numeric($_REQUEST['Spezialisierung']) )
  {
    $query = mysql_query('SELECT Bild FROM T_Spezialisierungen ' .
			  'WHERE Spezialisierung_id='.$_REQUEST['Spezialisierung']);
	if( $beschreibung = mysql_fetch_row($query))
	{
		  $Bild = $beschreibung[0];
  	}
	mysql_free_result($query);
  }  
  // Bild ausgeben
  if ( $Bild != '' )
  {      
     // Senden des Response-Headers f�r den Inhaltstyp der gelieferten
     // Daten 
     header ( 'Content-Type: image/jpeg');
     // Senden der eigentlichen Bilddaten als (einzigen) Inhalt der
     // Response
     if ( isset($_REQUEST['Groesse']) && is_numeric($_REQUEST['Groesse']))
     {
     	// Bildgr��e ver�ndern
     	$altesBild=imagecreatefromstring ($Bild);       
        $breite = imageSX($altesBild);
        $hoehe = imageSY($altesBild);
     	if ( $_REQUEST['Groesse'] > 0 )
     	{
          $neueBreite=$_REQUEST['Groesse'];         
          $neueHoehe=intval($hoehe*$neueBreite/$breite);
     	}
     	else
     	{
          $neueHoehe = abs($_REQUEST['Groesse']);
          $neueBreite=intval($breite*$neueHoehe/$hoehe);
     	}
         
        $neuesBild=imagecreatetruecolor($neueBreite,$neueHoehe); 
        ImageCopyResized($neuesBild,$altesBild,0,0,0,0,$neueBreite,$neueHoehe,$breite,$hoehe);
         
        ImageJPEG($neuesBild);      	
     }
     else    
       echo ( $Bild );
  }
  else
    die ( 'Ung�ltige ID oder Bild nicht vorhanden!');
  mysql_close(); 

?>
