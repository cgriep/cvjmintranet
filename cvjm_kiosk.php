<?php
/**
 * Webserver-Seite f�r die Funktionen des Kiosk
 *
 */

/* Datenbank- und AWF-Funktionalit�t einbinden */
include ('inc/functions.inc');
include ('inc/licence.key');
include ('inc/sessions.inc');
include ('inc/caching.inc');
include ('inc/config.inc');

include_once('inc/misc/CVJM.inc');
require_once (INC_PATH.'cvjm/Adresse.class.php');
require_once (INC_PATH.'cvjm/Event.class.php');

define('EVENT_ART_EVENT', 0);
define('EVENTUSER', "heike.ruhl@cvjm-feriendorf.de");
define('TABLE_EVENTS', "cvjm_Events");
define('TABLE_BETROFFENE', "cvjm_Event_Betroffene"); 

if ( !isset($_REQUEST["action"]) || ! isset($_REQUEST['Key']) || $_REQUEST['Key'] == '' )
{
	die("Unauthorisiert: ".date('dmYH'));
}

// Benutzer-ID heraussuchen
define('SESSION_DBID', get_user_id($session_userid));
define('SESSION_STATUS','ok');
$profile = get_profile();
// feststellen was die Anforderung ist
switch ($_REQUEST['action'])
{
	case "benutzerliste":
		Kiosk_LiesBenutzer();
		break;
	case "artikelliste":
		if ( ! isset($_REQUEST['Artikel_id'])) $_REQUEST['Artikel_id'] = -1;
		Kiosk_LiesArtikel($_REQUEST['Art'], $_REQUEST['Artikel_id'], $_REQUEST['Mitarbeiter']);
		break;
	case "kassenstand":
		Kiosk_getKassenstand();
		break;
	case "mitarbeiterbuchungen":
		Kiosk_getMitarbeiterKassenstand($_REQUEST['Benutzer']);
		break;
	case "aenderekassenstand":
		if ( isset($_REQUEST['Preis']) && is_numeric($_REQUEST['Preis']))
		{
			Kiosk_AendereKassenstand($_REQUEST['Preis'], $_REQUEST['Bemerkung'], $_REQUEST['Benutzer'], $_REQUEST['user']);
		}
		break;
	case "kioskmitarbeiterbuchung":
	case "kioskbuchung":
		if ( isset($_REQUEST['Menge']) && is_numeric($_REQUEST['Menge']) &&
		isset($_REQUEST['Artikel']) && is_numeric($_REQUEST['Artikel']))
		{
				Kiosk_ArtikelHinzufuegen($_REQUEST['Menge'], $_REQUEST['Artikel'], $_REQUEST['Benutzer'], 
				$_REQUEST['action'] =='kioskmitarbeiterbuchung');
		}
		else 
		{
			echo '<?xml version="1.0" encoding="ISO-8859-1"?>';
			echo '<result>Parameterfehler! '.is_numeric($_REQUEST['Menge']).
			'/'.is_numeric($_REQUEST['Artikel']).'</result>';
		}
		break;
	case "wechselgeld":
		Kiosk_ErstelleEvent('Bitte Wechselgeld bereitstellen: '.$_REQUEST['Hinweis'], 'Kiosk-Wechselgeld', $_REQUEST['user']);
		break;
	default:
		die("Keine Anweisung vorhanden!");
}

function Kiosk_ErstelleEvent($beschreibung, $titel, $Benutzer)
{
	$query = mysql_query("SELECT id FROM awf_users WHERE email='".EVENTUSER."'");
	$user = 0;
	if ( $result = mysql_fetch_array($query) )
	{
		$user = $result['id'];
	}
	mysql_free_result($query);
	if ($user != 0)
	{
		if (! $query = mysql_query( 'INSERT INTO '.TABLE_EVENTS.' (Titel,Beschreibung,Status,Prioritaet,Autor,Datum,Art,created,Referenz) '
		. 'VALUES ('
		. "'".mysql_real_escape_string($titel)."','".mysql_real_escape_string($beschreibung)."',"
		. "1,3," // Offen, wichtig
		. $Benutzer.","
		. 'UNIX_TIMESTAMP(NOW()),' //Long.toString(new java.util.Date().getTime()/1000) +"," // Datum
		. EVENT_ART_EVENT . ',' // Event
		. 'UNIX_TIMESTAMP(NOW()),' // ((Long.toString(new java.util.Date().getTime()/1000)
		.'NULL)'))
		{
			// Auftrag
			echo "Fehler ".mysql_error();
		}
		else
		{
			mysql_query("INSERT INTO ".TABLE_BETROFFENE.
			" (F_Event_id,F_Betroffener_id,Bestaetigungsstatus) VALUES (".
			mysql_insert_id().",".$user.",1)");
			echo '<result>OK</result>';
		}
	}
	else
	{
		echo 'EVENT FEHLER - kein User</result>';
	}
	
}
function Kiosk_ArtikelHinzufuegen($Menge, $id, $user, $mitarbeiter = false)
{
	// Preisliste & Bezeichnung holen
	$sql = "SELECT Preisliste_id, Bezeichnung FROM cvjm_Preislisten WHERE ";
	if ($mitarbeiter)
	{
		$sql .= ' Mitarbeiterliste';
	}
	else
	{
		$sql .= ' Standard';
	}
	if ( ! $query = mysql_query($sql))
	{
		echo '<?xml version="1.0" encoding="ISO-8859-1"?><result>Preisliste '.mysql_error().'</result>';
	}
	else
	{
		$preisliste = -1;
		if ($result = mysql_fetch_array($query)) {
			$preisliste = $result["Preisliste_id"];
			//this.setTitle(this.getTitle() + " Preisliste "
			//			+ ergebnis.getString("Bezeichnung"));
		}
		mysql_free_result($query);
		$sql = "SELECT Bezeichnung, Preis, MWST "
		. "FROM cvjm_Artikel INNER JOIN cvjm_Preise ON Artikel_Nr=id "
		. "INNER JOIN cvjm_MWST ON F_MWSt=MWST_id "
		. " WHERE F_Preisliste_id=" . $preisliste
		. ' AND Artikel_Nr='.$id;
		if (! $query = mysql_query($sql))
		{
			
			echo '<?xml version="1.0" encoding="ISO-8859-1"?><result>SQL: '.$sql.' / '.mysql_error().'</result>';
		}
		else
		{
			if ( $result = mysql_fetch_array($query))
			{
				if ( $mitarbeiter )
				{
					 $sql = "INSERT INTO cvjm_Kiosk_Mitarbeiter (Menge,Preis,F_Artikel_id,Bezeichnung,Benutzer) "
				. "VALUES (". $Menge. ",". (round($result['Preis']*(100+$result['MWST']))/100). ","	. $id. ",'"
				. $result['Bezeichnung']. "','" . mysql_real_escape_string($user) . "')";
				}
				else
				{
					 $sql = "INSERT INTO cvjm_Kiosk (Menge,Preis,F_Artikel_id,Bezeichnung,Benutzer) "
				. "VALUES (". $Menge. ",". (round($result['Preis']*(100+$result['MWST']))/100). ","	. $id. ",'"
				. $result['Bezeichnung']. "','" . mysql_real_escape_string($user) . "')";
				
				}
				if ( mysql_query($sql) )
				{
					echo '<?xml version="1.0" encoding="ISO-8859-1"?><result>OK</result>';
				}
				else
				{
					echo '<?xml version="1.0" encoding="ISO-8859-1"?><result>FEHLER - '.mysql_error(). ' / '.$sql.'</result>';
				}
			}
			mysql_free_result($query);
		}
	}
}

function Kiosk_LiesBenutzer()
{
	if ( ! $query = mysql_query("SELECT id FROM awf_groups WHERE group_name='Kiosk'"))
	{
		echo '<?xml version="1.0" encoding="ISO-8859-1"?><result>FEHLER '.mysql_error().'</result>';
	}
	else
	{
		$result = mysql_fetch_array($query);
		$gruppe = $result["id"];
		mysql_free_result($query);
		if (! $query = mysql_query("SELECT user_id FROM awf_userdata WHERE name='group_" . $gruppe . "'"))
		{
			echo mysql_error();
		}
		else
		{
			$s = "-1";
			while ( $result = mysql_fetch_array($query) ) {
				$s =$s . ",".$result["user_id"];
			}
			mysql_free_result($query);
			$sql = "SELECT password, value, awf_users.id as userid "
			. "FROM awf_users INNER JOIN awf_userdata ON awf_users.id=user_id"
			. " WHERE valid AND name='nickname' AND user_id IN (".$s.")"
			. " ORDER BY value";
			if (! $query = mysql_query($sql))
			{
				echo mysql_error();
			}
			else
			{
				$xml = '<?xml version="1.0" encoding="ISO-8859-1"?><users>';
				while ( $result = mysql_fetch_array($query))
				{
					// XML erstellen
					$xml .= '<user><benutzer>'.$result['value'].'</benutzer><password>'.
					$result['password'].'</password><userid>'.$result['userid'].'</userid></user>';
				}
				$xml .= '</users>';
				mysql_free_result($query);
				echo $xml;
			}
		}
	}

}

function Kiosk_LiesArtikelRekursiv($art, $artikel_id, $preisliste)
{
		$sql = "SELECT id, Bezeichnung, Barcode, Preis, MWST "
		. "FROM cvjm_Artikel INNER JOIN cvjm_Preise ON Artikel_Nr=id "
		. "INNER JOIN cvjm_MWST ON F_MWSt=MWST_id ";
		switch ( $art )
		{
			case 'unterartikel':
				$sql .= " INNER JOIN cvjm_Artikelgruppen ON F_Unterartikel_id=id";
				$sql .= " WHERE F_Artikel_id=".$artikel_id
				. " AND F_Preisliste_id=" . $preisliste;
				break;
			case 'schnellartikel':
				$sql .= " WHERE Bezeichnung LIKE '%Kiosk %' "
				. " AND F_Preisliste_id=" . $preisliste
				. " ORDER BY Bezeichnung LIMIT 12";
				break;
			case 'normal':
				$sql .= " WHERE F_Preisliste_id=" . $preisliste;
		}
		if (! $query = mysql_query($sql))
		{
			echo 'SQL: '.$sql.' / '.mysql_error();
		}
		else
		{
			while ( $result = mysql_fetch_array($query))
			{
				// XML erstellen
				if ( $art == 'unterartikel')
				{
					$xml .= '<unterartikel>';
				}
				else
				{
					$xml .= '<artikel>';
				}
				// "Kiosk" wegfiltern
				$xml .= '<id>'.$result['id'].'</id><Bezeichnung>'.
				htmlspecialchars(str_replace('Kiosk','',$result['Bezeichnung'])).'</Bezeichnung><Barcode>'.$result['Barcode'].
					'</Barcode><Preis>'.(round($result['Preis']*(100+$result['MWST']))/100).
					'</Preis>';
				// Unterartikel pr�fen
				$xml .= Kiosk_LiesArtikelRekursiv('unterartikel', $result['id'], $preisliste);
				if ( $art == 'unterartikel')
				{
					$xml .= '</unterartikel>';
				}
				else
				{
					$xml .= '</artikel>';
				} 
			}
			mysql_free_result($query);
	}
	return $xml;
}

function Kiosk_LiesArtikel($art, $artikel_id, $mitarbeiter)
{
	$sql = "SELECT Preisliste_id, Bezeichnung FROM cvjm_Preislisten";
	if ( $mitarbeiter == 'true') 
	{
		$sql .= " WHERE Mitarbeiterliste";
	}
	else
	{
		$sql .=  ' WHERE Standard';
	}
	if ( ! $query = mysql_query($sql))
	{
		echo mysql_error();
	}
	else
	{
		$preisliste = -1;
		if ($result = mysql_fetch_array($query)) {
			$preisliste = $result["Preisliste_id"];
			//this.setTitle(this.getTitle() + " Preisliste "
			//			+ ergebnis.getString("Bezeichnung"));
		}
		mysql_free_result($query);
		$xml = '<?xml version="1.0" encoding="ISO-8859-1"?><artikelliste>';
		$xml .= Kiosk_LiesArtikelRekursiv($art, $artikel_id, $preisliste);
		$xml .= '</artikelliste>';
		echo $xml;
	}
}

function Kiosk_getKassenstand()
{
	$xml = '<?xml version="1.0" encoding="ISO-8859-1"?><historie>';
	$xml .= '<kassenstand>';	
	$query = mysql_query('SELECT Sum(Preis*Menge) AS Kassenstand FROM cvjm_Kiosk');
	if ($result = mysql_fetch_array($query)) {
		$xml .= (round($result['Kassenstand']*100)/100);
	}
	mysql_free_result($query);
	$xml .= '</kassenstand>';
	$xml .= '<verkaeufe>';
	$query = mysql_query('SELECT * FROM cvjm_Kiosk WHERE Datum >= CURDATE() ORDER BY Datum DESC');
	while ($entry = mysql_fetch_array($query))
	{
		$xml .='<eintrag>';
		$xml .= '<Bezeichnung>'.htmlspecialchars($entry['Bezeichnung']).'</Bezeichnung>';
		$xml .= '<Benutzer>'.htmlspecialchars($entry['Benutzer']).'</Benutzer>';
		$xml .= '<Datum>'.$entry['Datum'].'</Datum>';
		$xml .= '<Menge>'.$entry['Menge'].'</Menge>';
		$xml .= '<Preis>'.$entry['Preis'].'</Preis>';
		$xml .='</eintrag>';
	}
	mysql_free_result($query);
	$xml .= '</verkaeufe>';
	$xml .= '</historie>';
	echo $xml;
}

function Kiosk_getMitarbeiterKassenstand($mitarbeiter)
{
	$xml = '<?xml version="1.0" encoding="ISO-8859-1"?><historie>';
	$xml .= '<kassenstand>';	
	$query = mysql_query('SELECT Sum(Preis*Menge) AS Kassenstand FROM cvjm_Kiosk_Mitarbeiter WHERE Benutzer="'.$mitarbeiter.'"');
	if ($result = mysql_fetch_array($query)) {
		$xml .= (round($result['Kassenstand']*100)/100);
	}
	mysql_free_result($query);
	$xml .= '</kassenstand>';
	$xml .= '<verkaeufe>';
	$query = mysql_query('SELECT * FROM cvjm_Kiosk_Mitarbeiter WHERE Benutzer="'.mysql_real_escape_string($mitarbeiter).
	'" ORDER BY Datum DESC');
	while ($entry = mysql_fetch_array($query))
	{
		$xml .='<eintrag>';
		$xml .= '<Bezeichnung>'.htmlspecialchars($entry['Bezeichnung']).'</Bezeichnung>';
		$xml .= '<Benutzer>'.htmlspecialchars($entry['Benutzer']).'</Benutzer>';
		$xml .= '<Datum>'.$entry['Datum'].'</Datum>';
		$xml .= '<Menge>'.$entry['Menge'].'</Menge>';
		$xml .= '<Preis>'.$entry['Preis'].'</Preis>';
		$xml .='</eintrag>';
	}
	mysql_free_result($query);
	$xml .= '</verkaeufe>';
	$xml .= '</historie>';
	echo $xml;
}


function Kiosk_AendereKassenstand($preis, $bemerkung, $user, $Benutzer)
{
	if ( ! mysql_query('INSERT INTO cvjm_Kiosk (Menge,Preis,Bezeichnung,Benutzer, F_Artikel_id) VALUES (1,' . $preis
	. ",'Geldbuchung:" . mysql_real_escape_string($bemerkung)	. "','" . mysql_real_escape_string($user) . "',-1)") )
	{
		echo '<result>'.mysql_error().'</result>';	
	}
	else {
		Kiosk_ErstelleEvent($bemerkung.' (Summe: '.$preis.')', 'Kiosk-Geldbuchung', $Benutzer);
	}
}
?>