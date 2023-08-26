<?php

/* Modul Wiedervorlage */
/* Parameter:

AWF-Name: cvjmwiedervorlage
(c) 2007 Christoph Griep

*/


if ( $profile["editor"] != 1 )
{
	include(INC_PATH."misc/CVJM.inc");
	include(INC_PATH."misc/cvjmBuchungstools.inc");
	if ( $document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
	// Liste der vorhandenen Buchungen
	// Übersicht über die aktuellen Belegungen
	echo $design["table_begin"];
	echo '<tr bgcolor="'.$design["table_header"].
	'"><th>Wieder-<br />vorlage</th><th>Datum</th><th>Bemerkung</th><th>Adresse</th><th>Buchung</th></tr>';
	$Where = "WHERE Wiedervorlage > 0";
	$sql = "SELECT Bemerkung, Bemerkung_id, Wiedervorlage, Datum, Name, ".
	"Zusatz, PLZ, Ort, Von, Bis, Bemerkungen, Adressen_id, Buchung_Nr, BStatus, ".
	"Personenanzahl, Altersgruppe FROM (".
	TABLE_ADRESSENBEMERKUNGEN.
	" INNER JOIN ".TABLE_ADRESSEN." ON Adressen_id = F_Adressen_id) LEFT JOIN ".
	TABLE_BUCHUNGEN." ON F_Buchung_Nr = Buchung_Nr $Where ORDER BY Wiedervorlage";
	$query = sql_query($sql);
	$Color = $design["table_row_bright"];
	while ( $buchung = sql_fetch_array($query) )
	{
		echo '<tr bgcolor="'.$Color.'">';
		echo '<td><a href="'.get_url(get_cmodule_id("cvjmadressen"),"Bearbeiten=".
		$buchung["Adressen_id"]."&AddBem=".$buchung["Bemerkung_id"]."#AddBem").
		'">'.date("d.m.Y",$buchung["Wiedervorlage"]).'</a></td>';
		echo '<td>'.date("d.m.Y",$buchung["Datum"]).'</td>';
		echo '<td>'.nl2br(stripslashes($buchung["Bemerkung"]));
		if ( $buchung["Anhang"] != "" )
		{
			$ext = pathinfo($buchung["Anhang"]);
			echo '<br />Anhang: ';
			echo '<a target="_blank" href="'.getAdressenAnhangLink($buchung["F_Adressen_id"],
			$buchung["F_Bemerkung_id"], $ext["extension"], "Bemerkung").'">'.$buchung["Anhang"].'</a>';
		}
		echo '</td>';
		echo '<td><a href="'.get_url(get_cmodule_id("cvjmadressen"),"Bearbeiten=".
		$buchung["Adressen_id"]).'">'.stripslashes($buchung["Name"])."</a>";
		if ( trim($buchung["Zusatz"]) != "" ) echo ', '.stripslashes($buchung["Zusatz"]);
		echo '<br />'.$buchung["PLZ"]." ".stripslashes($buchung["Ort"]);
		echo '</td>';
		echo '<td>';
		if ($buchung["Von"] > 0 )
		{
			switch ( $buchung["BStatus"] )
			{
				case 9: // gelöscht
					echo '<img src="img/small_edit/notok.gif" border="0" title="Vorreservierung gel&ouml;scht">';
					break;
				case 0:
				case 1:
				case 2: // Vorreservierung
					echo '<img src="img/small_edit/help.gif" border="0" title="Vorreservierung">';
					break;
				case 10:  // Reserviert
					echo '<img src="img/small_edit/wait.gif" border="0" title="Reserviert">';
					break;
				case 20: // abgerechnet
					echo '<img src="img/small_edit/ok.gif" border="0" title="Abgerechnet">';
					break;
				case 30: // Storniert
					echo '<img src="img/small_edit/halfok.gif" border="0" title="Storniert">';
			}
			echo '<a href="'.get_url(get_cmodule_id("cvjmbuchung"),
			"Buchung_Nr=".$buchung["Buchung_Nr"]).'">';
			echo $Wochentage[date("w",$buchung["Von"])]." ".date("d.m.Y",$buchung["Von"]);
			echo " ".date("H:i",$buchung["Ankunftszeit"]);
			echo "-".$Wochentage[date("w",$buchung["Bis"])]." ".date("d.m.Y",$buchung["Bis"]);
			echo " ".date("H:i",$buchung["Abfahrtszeit"]);
			echo "</a>";
			if ( $buchung["Bemerkungen"] != "" )
			echo '<br />'.nl2br(stripslashes($buchung["Bemerkungen"]));
			$wo = ZeigeBereiche(CVJMART_ORT, $buchung["Buchung_Nr"]);
			if ( $wo != "" ) echo '<br />'.$wo;
			$wo = ZeigeBereiche(CVJMART_VERPFLEGUNG, $buchung["Buchung_Nr"]);
			if ( $wo != "" ) echo '<br />'.$wo;
			$wo = ZeigeBereiche('Programm', $buchung["Buchung_Nr"]);
			if ( $wo != "" ) echo '<br />'.$wo;
			echo "<br />".Personenanzahl($buchung)." Personen (".$Altersgruppen[$buchung["Altersgruppe"]].")";
		}
		echo '</td>';
		echo "</tr>\n";
		if ( $Color == $design["table_row_bright"] )
		$Color = $design["table_row_dark"];
		else
		$Color = $design["table_row_bright"];
	} // while
	if ( sql_num_rows($query) == 0 )
	{
		echo '<tr><td colspan="5">-keine Wiedervorlagen vorhanden-</td></tr>';
	}
	sql_free_result($query);
	echo $design['table_end'];

	// Liste der unbezahlten Anzahlungen
	echo $design['table_begin'];
	echo '<tr class="ueberschrift"><th colspan="8">offene Vorauszahlungen</th></tr>';
	echo '<tr class="ueberschrift">';
	echo '<th>Nr</th><th>Name</th><th>Telefon</th><th>von</th><th>bis</th>';
	echo '<th>Forderung</th><th>Datum</th></tr>';
	$Where = 'WHERE Anzahlung>0 AND BStatus < '.BUCHUNG_FERTIG.
	' AND BStatus <> '.BUCHUNG_STORNIERT.
	' AND AnzahlungsBemerkung="" AND Rechnung=3 AND Rechnungsdatum<'.
	strtotime('-'.$document['WiedervorlageTagAnzahl'].' days'); // Bestätigung
	$sql = 'SELECT Rechnung_id,F_Buchung_Nr, Name,Vorname, '.TABLE_RECHNUNGEN.
	'.F_Adressen_id, Von, Bis, '.
	'Zusatz, PLZ, Ort, Anzahlung, Rechnungsdatum FROM '.
	TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_RECHNUNGEN.' ON Buchung_Nr = F_Buchung_Nr '.
	$Where.' ORDER BY Rechnungsdatum';
	if ( ! $query = sql_query($sql) ) echo sql_error();
	$Eintraege = array();
	$Summe = 0;
	while ( $buchung = sql_fetch_array($query) )
	if ( ! isset($Eintraege[$buchung['F_Buchung_Nr']]) )
	{
		$Eintraege[$buchung['F_Buchung_Nr']] = $buchung;
		$Summe += $buchung['Anzahlung'];
	}
	sql_free_result($query);

	$Color = 'helleZeile';
	foreach ( $Eintraege as $buchungsnummer => $buchung )
	{
		echo '<tr class="'.$Color.'">';
		// Adressen_id herausfinden
		$q = sql_query('SELECT Adressen_id, Telefon1, Telefon2, Kunden_Nr FROM '.TABLE_ADRESSEN.
		' WHERE Adressen_id='.$buchung['F_Adressen_id']);
		$Adresse = sql_fetch_array($q);
		sql_free_result($q);
		echo '<td><a href="'.get_url(get_cmodule_id('cvjmadressen'),'Bearbeiten='.
		$Adresse['Adressen_id']).'">'.$Adresse['Kunden_Nr'].'</a><br />';
		echo 'Bu: <a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.
		$buchung['F_Buchung_Nr']).'">'.$buchung['F_Buchung_Nr'].'</a></td>';
		echo "</td>\n";
		echo '<td>'.$buchung['Name'];
		if ( trim($buchung['Vorname']) != '' ) echo ','.$buchung['Vorname'];
		if ( trim($buchung['Zusatz']) != '' ) echo '<br />'.$buchung['Zusatz'];
		echo '<br />'.$buchung['PLZ'].' '.$buchung['Ort'];
		echo '</td>';
		// Telefon herausfinden
		echo '<td>'.$Adresse['Telefon1'];
		if ( $Adresse['Telefon2'] !=  '' )
		echo '<br />'.$Adresse['Telefon2'];
		echo '<td>'.date('d.m.Y',$buchung['Von']).'</td>';
		echo '<td>'.date('d.m.Y',$buchung['Bis']).'</td>';
		echo '<td align="right">'.sprintf('%01.2f',$buchung['Anzahlung']);
		echo '<a href="'.get_url(get_cmodule_id('cvjmabrechnung'),'docinput[Rechnung_id]='.
		$buchung['Rechnung_id']).'">&rarr; Mahnen</a>';
		// Datum des letzten Schreibens feststellen
		$Dateiname = getAdressenAnhangLink($Adresse['Adressen_id'],$buchung['Rechnung_id'].'-*',
		'.pdf', '*'); // CVJM_ENDUNG
		 if ( ($dateien = glob($Dateiname))=== false) $dateien = array();
		if ( Count($dateien) > 1 )
		{
			$time = 0;
			foreach ( $dateien as $datei )
			if ( filemtime($datei) > $time ) $time = filemtime($datei);
			echo '<br /><small>Letztes Schreiben: '.date('d.m.Y H:i',$time).'</small>';
		}

		echo '</td>';
		echo '<td>'.date('d.m.Y',$buchung['Rechnungsdatum']).'<br />vor ';
		echo Round(-($buchung['Rechnungsdatum'] - time()) / 86400).' Tagen';
		echo '</td>';
		echo "</tr>\n";
		if ( $Color == 'helleZeile' )
		$Color = 'dunkleZeile';
		else
		$Color = 'helleZeile';
	} // foreach
	if ( Count($Eintraege ) == 0 )
	{
		echo '<tr><td colspan="5">-keine Wiedervorlagen vorhanden-</td></tr>';
	}
	else
	echo '<tr><td colspan="6"></td><td align="center"><strong>'.sprintf("%01.2f",$Summe).
	"</strong></td></tr>\n";
	echo $design["table_end"];

	// Liste der unbezahlten Rechnungen
	echo $design["table_begin"];
	echo '<tr class="ueberschrift"><th colspan="8">offene Rechnungen</th></tr>';
	echo '<tr class="ueberschrift">';
	echo '<th>Nr</th><th>Name</th><th>Telefon</th><th>von</th><th>bis</th>';
	echo '<th>Forderung</th><th>Datum</th></tr>';
	$Where = "WHERE BStatus < ".BUCHUNG_FERTIG.
	" AND BStatus <> ".BUCHUNG_STORNIERT.
	" AND Rechnung=1 AND Rechnungsdatum<".
	strtotime("-".$document["WiedervorlageTagAnzahl"]." days"); // Rechnung
	$sql = "SELECT Rechnung_id,F_Buchung_Nr, Name,Vorname, ".TABLE_RECHNUNGEN.
	".F_Adressen_id, Von, Bis, ".
	"Zusatz, PLZ, Ort, Anzahlung, Rechnungsdatum FROM ".
	TABLE_BUCHUNGEN." INNER JOIN ".TABLE_RECHNUNGEN." ON Buchung_Nr = F_Buchung_Nr ".
	"$Where ORDER BY Rechnungsdatum";
	if ( ! $query = sql_query($sql) ) echo sql_error();
	$Summe = 0;
	$Color = "helleZeile";
	while ( $buchung = sql_fetch_array($query) )
	{
		echo '<tr class="'.$Color.'">';
		// Adressen_id herausfinden
		$q = sql_query("SELECT Adressen_id, Telefon1, Telefon2, Kunden_Nr FROM ".TABLE_ADRESSEN.
		" WHERE Adressen_id=".$buchung["F_Adressen_id"]);
		$Adresse = sql_fetch_array($q);
		sql_free_result($q);
		echo '<td><a href="'.get_url(get_cmodule_id("cvjmadressen"),"Bearbeiten=".
		$Adresse["Adressen_id"]).'">'.$Adresse["Kunden_Nr"].'</a><br />';
		echo 'Bu: <a href="'.get_url(get_cmodule_id("cvjmbuchung"),"Buchung_Nr=".
		$buchung["F_Buchung_Nr"]).'">'.$buchung["F_Buchung_Nr"]."</a>";
		echo "</td>\n";
		echo '<td>'.$buchung['Name'];
		if ( trim($buchung['Vorname']) != '' ) echo ','.$buchung['Vorname'];
		if ( trim($buchung['Zusatz']) != '' ) echo '<br />'.$buchung['Zusatz'];
		echo '<br />'.$buchung['PLZ'].' '.$buchung['Ort'];
		echo '</td>';
		// Telefon herausfinden
		echo '<td>'.$Adresse['Telefon1'];
		if ( $Adresse['Telefon2'] !=  '' )
		echo '<br />'.$Adresse['Telefon2'];
		echo '</td>';
		echo '<td>'.date('d.m.Y',$buchung['Von']).'</td>';
		echo '<td>'.date('d.m.Y',$buchung['Bis']).'</td>';
		// Summe feststellen!
		$sql = 'SELECT Sum(Menge*F_Preis*CASE WHEN Dauer=0 THEN 1 ELSE Dauer END   
- (Menge*F_Preis*CASE WHEN Dauer=0 THEN 1 ELSE Dauer END * Rabatt)/100) AS Summe
FROM cvjm_Rechnungseintraege
WHERE F_Rechnung_id='.$buchung['Rechnung_id'];
		$qr = sql_query($sql);
		$result = sql_fetch_row($qr);
		$Sum = $result[0];
		sql_free_result($qr);
		$Summe += $Sum;
		echo '<td align="right">'.sprintf('%01.2f',$Sum);
		echo '<a href="'.get_url(get_cmodule_id('cvjmabrechnung'),'docinput[Rechnung_id]='.
		$buchung['Rechnung_id']).'">&rarr; Mahnen</a>';
		// Datum des letzten Schreibens feststellen
		$Dateiname = getAdressenAnhangLink($Adresse['Adressen_id'],$buchung['Rechnung_id'].'-*',
		'.pdf', '*'); // CVJM_ENDUNG
		 if ( ($dateien = glob($Dateiname))=== false) $dateien = array();
		if ( Count($dateien) > 1 )
		{
			$time = 0;
			foreach ( $dateien as $datei )
			if ( filemtime($datei) > $time ) $time = filemtime($datei);
			echo '<br /><small>Letztes Schreiben: '.date('d.m.Y H:i',$time).'</small>';
		}
		echo '</td>';
		echo '<td>'.date('d.m.Y',$buchung['Rechnungsdatum']).'<br />vor ';
		echo Round(-($buchung['Rechnungsdatum'] - time()) / 86400).' Tagen';
		echo '</td>';
		echo "</tr>\n";
		if ( $Color == 'helleZeile' )
		$Color = 'dunkleZeile';
		else
		$Color = 'helleZeile';
	} // while
	if ( sql_num_rows($query) == 0 )
	{
		echo '<tr><td colspan="5">-keine Rechnungen vorhanden-</td></tr>';
	}
	else
	echo '<tr><td colspan="6"></td><td align="center"><strong>'.sprintf('%01.2f',$Summe).
	"</strong></td></tr>\n";
	sql_free_result($query);
	echo $design['table_end'];

	// Liste der offenen Vorreservierungen
	echo $design['table_begin'];
	echo '<tr class="ueberschrift"><th colspan="8">offene Vorreservierungen</th></tr>';
	echo '<tr class="ueberschrift">';
	echo '<th>Nr</th><th>Name</th><th>Telefon</th><th>von</th><th>bis</th>';
	echo '<th>Datum</th></tr>';
	$Where = "WHERE BStatus <= ".BUCHUNG_VORRESERVIERUNG;
	$sql = "SELECT Adressen_id, Kunden_Nr, Buchung_Nr, Von, Bis, ".
	"Zusatz, PLZ, Ort, Vorname, Name, PLZ, Ort, Zusatz, " .
	"Telefon1, Telefon2, Logtext FROM ".
	TABLE_BUCHUNGEN." INNER JOIN ".TABLE_ADRESSEN.
	" ON F_Adressen_id=Adressen_id $Where ORDER BY BuchungStand, Von";
	if ( ! $query = sql_query($sql) ) echo sql_error();
	$Summe = 0;
	$Color = "helleZeile";
	while ( $buchung = sql_fetch_array($query) )
	{
		// Datum des letzten Schreibens feststellen
		$Dateiname = getAdressenAnhangLink($buchung["Adressen_id"],$buchung["Buchung_Nr"]."-*",
		'.pdf', "*"); // CVJM_ENDUNG
		 if ( ($dateien = glob($Dateiname))=== false) $dateien = array();
		if ( Count($dateien) > 1 )
		{
			echo '<tr class="'.$Color.'">';
			echo '<td><a href="'.get_url(get_cmodule_id("cvjmadressen"),"Bearbeiten=".
			$buchung["Adressen_id"]).'">'.$buchung["Kunden_Nr"].'</a><br />';
			echo 'Bu: <a href="'.get_url(get_cmodule_id("cvjmbuchung"),"Buchung_Nr=".
			$buchung["Buchung_Nr"]).'">'.$buchung["Buchung_Nr"]."</a>";
			echo "</td>\n";
			echo '<td>'.$buchung["Name"];
			if ( trim($buchung["Vorname"]) != "" ) echo ",".$buchung["Vorname"];
			if ( trim($buchung["Zusatz"]) != "" ) echo "<br />".$buchung["Zusatz"];
			echo "<br />".$buchung["PLZ"]." ".$buchung["Ort"];
			echo '</td>';
			// Telefon herausfinden
			echo '<td>'.$buchung["Telefon1"];
			if ( $buchung["Telefon2"] !=  "" )
			{
				echo '<br />'.$buchung["Telefon2"];
			}
			echo '</td>';
			echo '<td>'.date("d.m.Y",$buchung["Von"]).'</td>';
			echo '<td>'.date("d.m.Y",$buchung["Bis"]).'</td>';
			// Summe feststellen!
			$pos = strrpos($buchung["Logtext"],"\n");
			$dat = substr($buchung["Logtext"],$pos+1,10);
			echo '<td>'.$dat."<br />vor ";
			$dat = explode(".", $dat);
			$dat = strtotime($dat[2]."-".$dat[1]."-".$dat[0]);
			echo Round(-($dat - time()) / 86400)." Tagen";
			echo '<a href="'.get_url(get_cmodule_id("cvjmbuchung"),"Buchung_Nr=".
			$buchung["Buchung_Nr"]).'">&rarr; Anschreiben</a>';
			$time = 0;
			foreach ( $dateien as $datei )
			{
				if ( filemtime($datei) > $time ) $time = filemtime($datei);
			}
			echo "<br /><small>Letztes Schreiben: ".date("d.m.Y H:i",$time)."</small>";
			echo '</td>';
			echo "</tr>\n";
			if ( $Color == "helleZeile" )
			{
			 	$Color = "dunkleZeile";
			}
			else
			{
				$Color = "helleZeile";
			}
		}
	} // while
	if ( sql_num_rows($query) == 0 )
	{
		echo '<tr><td colspan="5">-keine Buchungen vorhanden-</td></tr>';
	}
	sql_free_result($query);
	echo $design["table_end"];

	// Liste der offenen unbearbeitete Vorreservierungen
	echo $design['table_begin'];
	echo '<tr class="ueberschrift"><th colspan="8">offene unbearbeitete Vorreservierungen</th></tr>';
	echo '<tr class="ueberschrift">';
	echo '<th>Nr</th><th>Name</th><th>Telefon</th><th>von</th><th>bis</th>';
	echo '<th>Datum</th></tr>';
	$Where = "WHERE BStatus <= ".BUCHUNG_VORRESERVIERUNG;
	$sql = "SELECT Adressen_id, Kunden_Nr, Buchung_Nr, Von, Bis, ".
	"Zusatz, PLZ, Ort, Vorname, Name, PLZ, Ort, Zusatz, " .
	"Telefon1, Telefon2, Logtext FROM ".
	TABLE_BUCHUNGEN." INNER JOIN ".TABLE_ADRESSEN.
	" ON F_Adressen_id=Adressen_id $Where ORDER BY BuchungStand, Von";
	if ( ! $query = sql_query($sql) ) echo sql_error();
	$Summe = 0;
	$Color = "helleZeile";
	while ( $buchung = sql_fetch_array($query) )
	{
		// Datum des letzten Schreibens feststellen
		$Dateiname = getAdressenAnhangLink($buchung["Adressen_id"],$buchung["Buchung_Nr"]."-*",
		'.pdf', "*"); // suche Datei mit Endung PDF
		if ( ($dateien = glob($Dateiname))=== false) $dateien = array();
		if ( Count($dateien) == 0 )
		{
			echo '<tr class="'.$Color.'">';
			echo '<td><a href="'.get_url(get_cmodule_id("cvjmadressen"),"Bearbeiten=".
			$buchung["Adressen_id"]).'">'.$buchung["Kunden_Nr"].'</a><br />';
			echo 'Bu: <a href="'.get_url(get_cmodule_id("cvjmbuchung"),"Buchung_Nr=".
			$buchung["Buchung_Nr"]).'">'.$buchung["Buchung_Nr"]."</a>";
			echo "</td>\n";
			echo '<td>'.$buchung["Name"];
			if ( trim($buchung["Vorname"]) != "" ) echo ",".$buchung["Vorname"];
			if ( trim($buchung["Zusatz"]) != "" ) echo "<br />".$buchung["Zusatz"];
			echo "<br />".$buchung["PLZ"]." ".$buchung["Ort"];
			echo '</td>';
			// 	Telefon herausfinden
			echo '<td>'.$buchung["Telefon1"];
			if ( $buchung["Telefon2"] !=  "" )
			echo '<br />'.$buchung["Telefon2"];
			echo '</td>';
			echo '<td>'.date("d.m.Y",$buchung["Von"]).'</td>';
			echo '<td>'.date("d.m.Y",$buchung["Bis"]).'</td>';
			// Summe feststellen!
			$pos = strrpos($buchung["Logtext"],"\n");
			$dat = substr($buchung["Logtext"],$pos+1,10);
			echo '<td>'.$dat."<br />vor ";
			$dat = explode(".", $dat);
			$dat = strtotime($dat[2]."-".$dat[1]."-".$dat[0]);
			echo Round(-($dat - time()) / 86400)." Tagen";
			echo '<a href="'.get_url(get_cmodule_id("cvjmbuchung"),"Buchung_Nr=".
			$buchung["Buchung_Nr"]).'">&rarr; Anschreiben</a>';
			echo '</td>';
			echo "</tr>\n";
			if ( $Color == "helleZeile" )
			$Color = "dunkleZeile";
			else
			$Color = "helleZeile";
		}
	} // while
	if ( sql_num_rows($query) == 0 )
	{
		echo '<tr><td colspan="5">-keine Buchungen vorhanden-</td></tr>';
	}
	sql_free_result($query);
	echo $design["table_end"];


}
else
{
	echo '<br /><br />';
	create_editor_input('Anzeige ab Tage','WiedervorlageTagAnzahl','text_input');
}

?>
