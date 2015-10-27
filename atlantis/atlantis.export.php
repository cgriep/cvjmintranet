<?php
/*
 * Created on 24.12.2006 Christoph Griep
 * 
 */
 $hinweis = '';
 $id = '';
 $gruppe = '';
if ( isset($_REQUEST['action']))
{
	$action = $_REQUEST['action'];
}
else
{
	$action = '';
}
switch ($action)
{
	case 'Klassen' :
		// Spezialisierungen und Klassen
		$hinweis = 'Allgemein=1: allgemeine Spezialisierung dieser Klasse<br />' .
				'Spruchliste=1: Spezialisierung ist eine Spruchliste.';
		$sql = 'SELECT Klasse, Spezialisierung, Spruchliste, Allgemein ' .
		'FROM T_Spezialisierungsklassen LEFT JOIN T_Spezialisierungen ' .
		'ON F_Klasse_id=Klasse_id ORDER BY Klasse, Spezialisierung';
		$gruppe ='Klasse';
		break;
	case 'Spezialisierungen' :
		// Spezialisierungen und Fertigkeiten
		$hinweis = ''; 
		$sql = 'SELECT Spezialisierung, Fertigkeit, Kosten, Rang, Magisch, Art, Fertigkeit_id ' .
		'FROM T_Spezialisierungen LEFT JOIN ((T_SpezialisierungenFertigkeiten INNER JOIN' .
		' T_Fertigkeiten ON F_Fertigkeit_id=Fertigkeit_id) INNER JOIN T_Raenge ON F_Rang_id=Rang_id) ' .
		'ON F_Spezialisierung_id=' .
		'Spezialisierung_id ORDER BY Spezialisierung, Fertigkeit';
		$gruppe = 'Spezialisierung';
		break;
	case 'Rassen' :
		// Rassenfertigkeiten
		$gruppe = 'Rasse';
		$hinweis = 'Pflicht=1: Die Fertigkeit muss für die Rasse gewählt werden.<br />' .
				'Kosten: zusätzliche Kosten der Fertigkeit für die Rasse.<br />Kosten=0 bei Pflicht=0: Verbotene Fertigkeit für diese Rasse.';
		$sql = 'SELECT Rasse, Fertigkeit, Art, Kosten, Kosten1, Pflicht ' .
		'FROM T_Rassen LEFT JOIN (T_RassenFertigkeiten INNER JOIN T_Fertigkeiten' .
		' ON F_Fertigkeit_id=Fertigkeit_id) ON Rasse_id=F_Rasse_id ' .
		'ORDER BY Rasse, Fertigkeit';
		break;
	case 'Abhaengigkeiten' :
		// Abhängigkeiten 
		$gruppe = 'Fertigkeit1';
		$hinweis = 'Automatisch=1: Man Fertigkeit2 sobald die Fertigkeit gewählt wird.<br />' .
				'Erlaubt=0: Fertigkeit2 ist nicht erlaubt wenn Fertigkeit gewählt ist<br />' .
				'Erlaubt=1: Fertigkeit2 kostet Kosten zusätzlich wenn Fertigkeit vorhanden ist.';
		$sql = 'SELECT Fertigkeiten1.Fertigkeit AS Fertigkeit1, Fertigkeiten2.Fertigkeit AS Fertigkeit2,' .
		' Automatisch, Erlaubt, Kosten, Fertigkeiten2.Kosten1 AS Kosten1 ' .
		'FROM (T_FertigkeitenAbhaengigkeiten INNER JOIN T_Fertigkeiten AS Fertigkeiten1 ' .
		'ON F_Fertigkeiten_id1=Fertigkeit_id) INNER JOIN T_Fertigkeiten AS Fertigkeiten2 ' .
		'ON F_Fertigkeiten_id2=Fertigkeiten2.Fertigkeit_id ' .
		'ORDER BY Fertigkeiten1.Fertigkeit, Fertigkeiten2.Fertigkeit';
		break;
	case 'Raenge':
	    $hinweis = '(Un)Magisch: Erlaubte Spezialisierungen in diesem Rang für magische/unmagische Charaktere';
	    $gruppe = '';
	    $sql = 'SELECT * FROM T_Raenge ORDER BY Rang';
	    $id = 'Rang';
	    break;
		
	case 'Fertigkeiten':
	    $hinweis = '';
	    $gruppe = '';
	    $sql = 'SELECT * FROM T_Fertigkeiten ORDER BY Fertigkeit';
	    $id = 'Fertigkeit';
	    break;
	case 'Rassendaten':
	    $hinweis = '';
	    $gruppe = '';
	    $sql = 'SELECT * FROM T_Rassen ORDER BY Rasse';
	    $id = 'Rasse';
	    break;
	case 'Spezialisierungsklassen':
	    $hinweis = '';
	    $gruppe = '';
	    $sql = 'SELECT * FROM T_Spezialisierungsklassen ORDER BY Klasse';
	    $id = 'Klasse';
	    break;
    case 'Spezialisierungsdaten':
	    $hinweis = '';
	    $gruppe = '';
	    $id = 'Spezialisierung';
	    $sql = 'SELECT * FROM T_Spezialisierungen ORDER BY Spezialisierung';	    
	    break;
	case 'Charaktere':
	    $hinweis = '';
	    $gruppe = '';
	    $sql = 'SELECT * FROM T_Charaktere ORDER BY Charaktername';
	    $id = 'Charakter';
	    break;
	default :
		$sql = '';
}
?>
