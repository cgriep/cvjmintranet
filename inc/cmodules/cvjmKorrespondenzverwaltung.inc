<?php
/**
 *
 * AWF-Modul cvjmkorrespondenzverwaltung
 * Dateiname: cvjmKorrespondenzverwaltung
 *
 * Ermöglicht die Ersetzung einer vorhandenen Korrespondenz durch eine neue Datei
 *
 *  Parameter:
 *  docinput[Datei]: vollständiger Pfad zur Datei
 *  docinput[Herkunft]: id und Parameter der Seite, auf die zurückverwiesen wird
 *  docinput[Adressen_id]: id der Adresse, deren Dateien bearbeitet werden
 */

if ( $profile["editor"] != 1 )
{
	include(INC_PATH."misc/CVJM.inc");
	include(INC_PATH."cvjm/Adresse.class.php");
	if ( ! is_numeric($docinput["Adressen_id"]) )
	{
		die('<div class="SchwererFehler">&gt;&gt;&gt; Fehler in Korrespondenzverwaltung - fehlende Parameter!</div>');
	}
	$params = array();
	foreach ($_REQUEST as $key => $value )
	{
		if ( ! is_array($value) && $key != "id" )
		$params[$key] = $value;
	}
	foreach ( $docinput as $key => $value )
	{
		$params["docinput[$key]"] = $value;
	}
	if ( !isset($docinput['BackID'])) $docinput['BackID'] = $_SERVER['HTTP_REFERER'];
	if ( $document['title'] != '')
	echo '<h1>'.transform($document['title'],$document['datatype']['title'])."</h1>\n";
	$Adresse = new Adresse($docinput['Adressen_id']);
	echo 'für '.$Adresse->Vollname().'<br />';
	if ( ! isset($_FILES["NeueDatei"]['name']) || $_FILES["NeueDatei"]['name'] == "" )
	{
		echo '<form action="'.get_url($id).'" method="post" enctype="multipart/form-data">';
		echo '<input type="hidden" name="docinput[BackID]" value="'.$docinput["BackID"].
		'" />'."\n";
		echo '<input type="hidden" name="docinput[Adressen_id]" value="'.$docinput["Adressen_id"].
		'" />'."\n";
		echo '<input type="hidden" name="docinput[Datei]" value="'.$docinput["Datei"].
		'" />'."\n";
		reset($_REQUEST);
		foreach ($_REQUEST as $key => $value )
		{
			if ( ! is_array($value) && $key != "id" )
			echo '<input type="hidden" name="'.$key.'" value="'.$value.'" />'."\n";		
		}
		if ( isset($docinput['Datei']) && $docinput['Datei'] != '' )
		{
			echo "Zu ersetzende Datei: ".$docinput["Datei"]."<br />\n";
		}
		echo 'Neue Datei: <input type="file" name="NeueDatei" /><br />';
		if ( isset($docinput['Datei']) && $docinput['Datei'] != '' )
		{
			echo 'Datei ersetzen <input type="Checkbox" name="docinput[Ersetzen]" value="v" />';
			echo " (wenn nicht gesetzt, bleibt eine Kopie des Originals erhalten)<br />\n";
		}
		echo '<input type="Submit" value="Datei hochladen" />';
		echo "</form>\n";
	}
	else
	{
		// Datei hochladen und zurück
		if ( $_FILES["NeueDatei"]["name"] != '' )
		{
			if (! isset($docinput['Datei']) || $docinput['Datei'] == '')
			{
				$docinput['Datei'] = $_FILES['NeueDatei']['name'];
			}
			// Upload eines Anhangs
			if (!is_dir(CVJM_ADRESSEN_DIR."/".$docinput["Adressen_id"]) )
			mkdirs(CVJM_ADRESSEN_DIR."/".$docinput["Adressen_id"],CONFIG_CREATE_MASK);
			$datei = CVJM_ADRESSEN_DIR."/".$docinput["Adressen_id"]."/".
			stripslashes(basename($docinput["Datei"]));
			$ext= pathinfo($datei);
			if (file_exists($datei))
			{
				if ( isset($docinput['Datei']) && $docinput['Datei']!='' && $docinput['Ersetzen'] != "v" )
				{
					echo "Datei wird umbenannt.<br />";
					rename (CVJM_ADRESSEN_DIR."/".$docinput["Adressen_id"]."/".
					stripslashes(basename($docinput["Datei"])),
					CVJM_ADRESSEN_DIR."/".$docinput["Adressen_id"]."/".
					stripslashes(basename($docinput["Datei"],".".$ext["extension"])).
					"- ersetzt ".date("d.m.Y H:i").".".$ext["extension"]);
				}
			}
			move_uploaded_file($_FILES['NeueDatei']['tmp_name'],
			CVJM_ADRESSEN_DIR."/".$docinput["Adressen_id"]."/".
			stripslashes(basename($docinput["Datei"])));
			chmod(CVJM_ADRESSEN_DIR."/".$docinput["Adressen_id"]."/".
			stripslashes(basename($docinput["Datei"])), CONFIG_CREATE_MASK);
		}
		echo '&gt;&gt;&gt; Datei ';
		echo $docinput['Datei'].' ';
		if ( $docinput["Ersetzen"] == "v" )
		{
			echo "ersetzt";
			if ( isset($_REQUEST["Buchung_Nr"]) && is_numeric($_REQUEST["Buchung_Nr"]) )
			sql_query("UPDATE ".TABLE_BUCHUNGEN." SET Logtext=CONCAT('".date("d.m.Y H:i").
			" Korrespondenz ".stripslashes(basename($docinput["Datei"]))." ersetzt ".
			get_user_nickname()."\n',Logtext) ".
			" WHERE Buchung_Nr=".$_REQUEST["Buchung_Nr"]);
		}
		else
		{
			echo "hochgeladen";
			if ( isset($_REQUEST["Buchung_Nr"]) && is_numeric($_REQUEST["Buchung_Nr"]) )
			sql_query("UPDATE ".TABLE_BUCHUNGEN." SET Logtext=CONCAT('".date("d.m.Y H:i").
			" Korrespondenz ".stripslashes(basename($docinput["Datei"]))." hochgeladen ".
			get_user_nickname()."\n',Logtext) ".
			" WHERE Buchung_Nr=".$_REQUEST["Buchung_Nr"]);
		}
		echo ".<br />";
	}
	echo '<a href="'.$docinput['BackID'].'">Zurück</a>';
	// get_url($docinput["BackID"],$params)
}  // wenn nicht editor


?>