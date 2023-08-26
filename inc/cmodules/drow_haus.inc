<?php
require(INC_PATH . 'misc/drow.inc');

if ( $profile['SteinsbergSL'] == '1')
{
	$_SESSION['SL'] = SESSION_DBID;
}

if ( isset($_SESSION['Anmelderecht']) && is_numeric($_REQUEST['Ver']) ) 
{
	$spieler = '';
	if ( is_numeric($_REQUEST['Akzeptieren']) )
	{
		$spieler = $_REQUEST['Akzeptieren'];
		$ok = 'ok'; 
	}
	if ( is_numeric($_REQUEST['Ablehnen']) )
	{
		$spieler = $_REQUEST['Ablehnen'];
		$ok = ''; 
	}
	if ( is_numeric($spieler))
	{
		$p = get_profile($spieler, false);
		global $flavour;
		$veranstaltung = get_nodedata($_REQUEST['Ver'], $flavour);	
		if ( $ok == '' ) // Anmeldung entfernen
		{ 
			// Mail versenden
			mail(get_user_email($spieler),  
			$veranstaltung['title'].' - Anmeldung abgelehnt',
			'Hallo '.$p['Vorname'].' '.$p['Name'].",\n\n".
			'du hast dich für '.$veranstaltung['title'].' von '.date("d.m.Y",strtotime($veranstaltung['Beginn'])).' bis '.
			date("d.m.Y",strtotime($veranstaltung['Ende'])).' im Haus '.$p['VGruppe'.$_REQUEST['Ver']].
			' angemeldet. Leider hat der Hausverantwortliche '.
			'deine Anmeldung abgelehnt. Damit ist deine Anmeldung nichtig.'."\n".
			'Bitte nimm Kontakt zu einem anderen Haus auf, wenn du weiterhin teilnehmen willst.'."\n\n".
			'deine Drow-Con-SL'."\n\n".
			'Dies ist eine automatisch generierte E-Mail.',
			"From: noreply@drow-con.de\r\nbcc: ".$veranstaltung['MeldungsEMail']);
			mail(get_user_email(SESSION_DBID),  
			'Info: '.$veranstaltung['title'].' - Anmeldung abgelehnt',
			'Anmeldung wurde abgelehnt: '.$p['Vorname'].' '.$p['Name']."\n\n".
			'Veranstaltung: '.$veranstaltung['title'].' von '.date("d.m.Y",strtotime($veranstaltung['Beginn'])).' bis '.
			date("d.m.Y",strtotime($veranstaltung['Ende'])).' im Haus '.$p['VGruppe'.$_REQUEST['Ver']]."\n\n".
			'Ablehnung durch: '.get_user_nickname().' '.date()."\n",
			"From: noreply@drow-con.de\r\nbcc: ".$veranstaltung['MeldungsEMail']);
			remove_profile('VGruppe'.$_REQUEST['Ver'], $spieler);
      		remove_profile('V'.$_REQUEST['Ver'], $spieler);
      		remove_profile('VBem'.$_REQUEST['Ver'], $spieler);
      		remove_profile('Geld'.$_REQUEST['Ver'], $spieler);
      		echo 'Die Anmeldung wurde abgelehnt.<br />';
		}
		else
		{
			// Mail versenden
			mail(get_user_email($spieler),  
			$veranstaltung['title'].' - Anmeldung akzeptiert',
			'Hallo '.$p['Vorname'].' '.$p['Name'].",\n\n".
			'du hast dich für '.$veranstaltung['title'].' von '.date("d.m.Y",strtotime($veranstaltung['Beginn'])).' bis '.
			date("d.m.Y",strtotime($veranstaltung['Ende'])).' im Haus '.$p['VGruppe'.$_REQUEST['Ver']].
			' angemeldet. Der Hausverantwortliche hat deine Anmeldung akzeptiert.'."\n".
			'Damit ist deine Anmeldung verbindlich. Bitte trage nun deine Charakterdaten ein.'."\n\n".
			'deine Drow-Con-SL',
			"From: noreply@drow-con.de\r\nbcc: ".$veranstaltung['MeldungsEMail']);

			set_profile('VAkzeptiert'.$_REQUEST['Ver'], get_user_nickname().' '.date('d.m.Y'), $spieler);
			echo 'Die Anmeldung wurde akzeptiert.<br />';
		}
	}
}		

// Auflistung aller H�user
// wenn angemeldeter = Verantwortlicher dann Anzeige der Anmeldungen pro Veranstaltung
// und Button für Annahme / Ablehnung

if ( $document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
if ( $document['body'] != '') echo transform($document['body'],$document['datatype']['body']).'<br />';

echo '
<style>
.hausTable {
	border-collapse: collapse;
	empty-cells: show;
}
.hausTable td, th {
	border-style: solid;
	border-width: thin;
	margin: 0;
}
.helleZeile {
	background-color: #111111;
}
.dunkleZeile {
	background-color: #444444;
}
</style>
';

echo '<table class="hausTable">';
echo '<tr><th>Name</th><th>Verantwortlich</th><th>Beschreibung</th><th>Rang</th><th>Status</th></tr>';
$sql = "SELECT Rang, Hausname, Verantwortlich, Haus_id, EMail, Aktiv, Beschreibung FROM T_Haeuser ORDER BY Rang";
$query = sql_query($sql);
while ( $row = sql_fetch_array($query) )
{
	if ( $farbe == 'helleZeile' )
		$farbe = 'dunkleZeile';
	else
		$farbe = 'helleZeile';
	echo '<tr class="'.$farbe.'">';
	echo '<td>'.$row['Hausname'].'</td>';
	echo '<td>'.transform(get_user_id($row['EMail']),'user_id').'</td>';
	echo '<td>'.transform($row['Beschreibung'],'extrans').'&nbsp;</td>';
	echo '<td>';
	if ( $row['Rang'] < 50 ) echo $row['Rang'];
	echo '&nbsp;</td>';
	echo '<td>';
	if ($row['Aktiv']) echo 'Bespielt'; else echo 'in Entstehung';
	echo '</td>';
	echo '</tr>';
	// angemeldeter Benutzer ist Verantwortlicher oder SL?
	if ( SESSION_DBID != -1 && (get_user_id($row['EMail']) == SESSION_DBID || $profile['SteinsbergSL'] == "1" 
|| ist_Hausmitglied(SESSION_DBID, $row['Haus_id'])) )
	{
		$_SESSION['Anmelderecht'] = SESSION_DBID;
		// alle Charaktere anzeigen
		echo '<tr class="'.$farbe.'">';
		echo '<td colspan="5">';
		$sql = 'SELECT Contage, Erstellt, Charakter_id, Name, Klasse, Gunstpunkte, Charstatus, Adlig, F_Spieler_id, EP, '.
		'Position,Rasse, Kleriker, Magier, Krieger, Gunststufenpunkte, Gunstpositionpunkte '.
		'FROM T_Charaktere INNER JOIN T_Rassen ON F_Rasse_id=Rasse_id '.
		'INNER JOIN T_Gunststufen ON F_Gunststufe_id=Gunststufe_id '.
		'INNER JOIN T_Positionen ON F_Position_id=Position_id '.
		'WHERE F_Haus_id='.$row['Haus_id'].' ORDER BY Name';
		$charquery = sql_query($sql);
		$Adel = 0;
		$NichtAdel = 0;
		if ( sql_num_rows($charquery) == 0)
		{
			echo 'Keine Charaktere vorhanden';
		}
		else
		{
		echo 'Charaktere des Hauses:';
		echo '<table class="hausTable">';
		echo '<tr><th>Name</th><th>Spieler</th><th>Rasse</th><th>Klasse</th><th>Adel</th>';
		echo '<th>EP</th><th>Gunststufe</th><th>Gunstposition</th><th>Gunst</th><th>Status</th>';
		
		while ( $char = sql_fetch_array($charquery))
		{
			echo '<tr>';
			echo '<td>';
			if ( $profile['SteinsbergSL'] == '1')
			{
				$sql = 'SELECT node_id FROM '.TABLE_NODEDATA.' WHERE name="Charakter_id" AND value="'.$char['Charakter_id'].'"';
				$lquery = sql_query($sql);
				$node_id = sql_fetch_row($lquery);
				$node_id = $node_id[0];
				sql_free_result($lquery);
				echo '<a href="?id='.$node_id.'">'.$char['Name'].'</a>';
			}
			else 
				echo $char['Name'];
			echo '</td>';
			echo '<td>'.transform($char['F_Spieler_id'],'user_id').'</td>';
			echo '<td>'.$char['Rasse'].'</td>';
			echo '<td>'.$char['Klasse'].'</td>';
			echo '<td>';
			if ( $char['Adlig'] == 1)
			{
				echo 'X';
				$Adel++;
			}
			else
			{
				$NichtAdel++;
				echo '&nbsp;';
			}
			echo '</td>';
			echo '<td> '.bestimmeEP($char['Charakter_id']).'('.$char['EP'].')</td>';
			echo '<td>'.$char[$char['Klasse']].'</td>';
			echo '<td>'.$char['Position'].'</td>';
			echo '<td>'.bestimmeCharGunst($char['Charakter_id']).'</td>';
			echo '<td>'.$char['Charstatus'].' &nbsp;';
			if ( substr($char['Erstellt'],0,4) != '0000' ) echo 'A:'.number_format($char['Contage']/30,1);
			echo '</td>';
			echo '</tr>';
		}
		echo '</table>';
		if ( $Adel + $NichtAdel > 0)
		{
			echo $Adel.' Adlige, '.$NichtAdel.' b�rgerliche - Adelsprozentsatz '.number_format($Adel/($Adel+$NichtAdel)*100,2).'%';
		}
		}
		echo '</td>';
		echo '</tr>';
		sql_free_result($charquery);
		if (get_user_id($row['EMail']) == SESSION_DBID || $profile['SteinsbergSL'] == "1" )
		{
		// Anmeldungen anzeigen
		$qres = sql_query("SELECT id FROM ".TABLE_NODES.' WHERE type_id = '.
		$doctypes["steinsberg_veranstaltung"].' ORDER BY id DESC');
		if ( sql_num_rows($qres) > 1 )
		{
			while ( $ver = sql_fetch_row($qres) )
			{
				$nd = get_nodedata($ver[0], $flavour);
				$Veranstaltungen[strtotime($nd['Beginn'])] = $nd;
				$Veranstaltungen[strtotime($nd['Beginn'])]['id'] = $ver[0];
			}
			$k = array_keys($Veranstaltungen);
			rsort($k);
			foreach ( $k as $vnr)
			{
				if ( strtotime('+1d',strtotime($Veranstaltungen[$vnr]['Ende'])) > time() )
				{
				echo '<tr class="'.$farbe.'">';
				echo '<td colspan="5">';
				echo 'Anmeldungen zu '.$Veranstaltungen[$vnr]['title'].' ('.$Veranstaltungen[$vnr]['Beginn'];
				echo '-'.$Veranstaltungen[$vnr]['Ende'].')';
				echo '<table class="hausTable">';
				echo '<tr><th>Charakter</th><th>Spieler</th><th>Anmeldestatus</th><th>Geldeingang</th></tr>';
				$sql = "SELECT DISTINCT T1.user_id FROM ".TABLE_USERDATA." AS T1 INNER JOIN ".TABLE_USERDATA." AS T2 ".
				"ON T1.user_id=T2.user_id WHERE T1.name='V".$Veranstaltungen[$vnr]['id']."' AND T2.name='VGruppe".
				sql_real_escape_string($Veranstaltungen[$vnr]['id'])."' AND T2.value='".
				sql_real_escape_string($row['Hausname'])."'";
				$charquery = sql_query($sql);
				while ( $char = sql_fetch_row($charquery))
				{
					$alles = get_profile($char[0], false);
					echo '<tr>';
					echo '<td>';
					$nodedata = get_nodedata($alles['Char'.$Veranstaltungen[$vnr]['id']], $flavour);
					$charakter = get_charakter($nodedata['Charakter_id']);
					if ( $profile['SteinsbergSL'] == '1')
					{
						$sql = 'SELECT node_id FROM '.TABLE_NODEDATA.' WHERE name="Charakter_id" AND value="'.$charakter['Charakter_id'].'"';
						$lquery = sql_query($sql);
						$node_id = sql_fetch_row($lquery);
						$node_id = $node_id[0];
						sql_free_result($lquery);
						echo '<a href="?id='.$node_id.'">'.$charakter['Name'].'</a>';
						echo ' <a href="/DrowCharakterbogen.class.php?Charakter_id='.$charakter['Charakter_id'].
						'" target="_blank" title="Charakterbogen (PDF)"><img src="/img/icons_16/pdf.png" border="0" /></a>';
					}
					else 
					{
						echo $char['Name'];
					}
					echo '</td>';
					echo '<td>'.transform($char[0],'user_id');
					echo '</td>';
					echo '<td>';
					if ( $alles['VAkzeptiert'.$Veranstaltungen[$vnr]['id']] == '')
					{
						echo '<a href="'.get_url($id).'&Akzeptieren='.$char[0].'&Ver='.$Veranstaltungen[$vnr]['id'].'">Akzeptieren</a> / ';
						echo '<a href="'.get_url($id).'&Ablehnen='.$char[0].'&Ver='.$Veranstaltungen[$vnr]['id'];
						echo '" onClick="javascript:return confirm(\'Soll diese Anmeldung tats�chlich unwiderruflich abgelehnt werden?\');">Ablehnen</a>';
					}
					else
					{
						echo 'ok';
					}
					echo '</td><td>';
					if ( $alles['Geld'.$Veranstaltungen[$vnr]['id']] == '')
						echo 'fehlt';
					else
						echo 'vorhanden';
					echo '</td></tr>';
				}
				sql_free_result($charquery);
				echo '</table>';
				echo '</td>';
				echo '</tr>';
				}
			} // foreach
		}
		} // if Veranstaltungen vorhanden
		sql_free_result($qres);
	} // if SL oder Verantwortlicher
} // while
sql_free_result($query);
echo $design['table_end'];
if ( $profile['editor'] == '1')
{
	create_editor_input('Hinweistext','body','area_input');
}
?>