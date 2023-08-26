<?php
/*
 Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
 http://www.liquidbytes.net/

 This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
 The author is Michael Mayer (michael@liquidbytes.net)
 Last update: 07.06.2002
 */

require(INC_PATH . 'misc/drow.inc');
load_lang('steinsberg');

require_once (INC_PATH .'xajax/xajax.inc.php');

/* Dies muss auch in DROW-Server stehen */
$xajax = new xajax('drow_server.php');
$xajax->registerFunction('zeigeGunststufe');
$xajax->registerFunction('sichereGunststufe');
$xajax->registerFunction('zeigeGunst');
$xajax->registerFunction('sichereName');
$xajax->registerFunction('sichereRasse');
$xajax->registerFunction('sichereGunstpunkte');
$xajax->registerFunction('zeigeFertigkeit');

//$xajax->registerFunction('senkeRechnungsMenge');
/* Ende Inhalt DROW-Server */

$xajax->printJavascript('./Skripte');

/* AJAX-Javascript */


if ( $document['Charakter_id'] == '')
{
	die('Ung�ltiger Charakter '.$document['Charakter_id']);
}

$bbasedir = get_dir_name('files',$id).'/published/';
$SpielerNr = $document['SpielerID'];
if(SESSION_STATUS == 'ok' && ! $profile['editor'] == "1" &&
($SpielerNr == SESSION_DBID || $profile['SteinsbergSL'] == "1") ) {
	$prow = get_profile($SpielerNr, false);
	$SpielerName = $prow['Vorname']." ".$prow['Name'];
	if ( $profile["SteinsbergSL"] == "1" )
	{
		echo 'Zur&uuml;ck zur <a href="?id='.get_cmodule_id('steinsberg_liste').'&Veranstaltung='.$_REQUEST['Ref'].'">Liste</a>';
		$_SESSION['SL'] = SESSION_DBID;
	}
	// Abspeichern der Daten
	if (isset($_REQUEST['Fertigkeit']) && is_numeric($_REQUEST['Fertigkeit']))
	{
		add_fertigkeit($document['Charakter_id'], $_REQUEST['Fertigkeit'], $_REQUEST['FertigkeitAnzahl']);
	}
	if ( $_FILES['spielerbild']['size'] <= 150000 && $_FILES['spielerbild']['size'] > 0) {
		echo "Datei ".$bbasedir;
		if(!file_exists($bbasedir)) {
			mkdirs($bbasedir, CONFIG_CREATE_MASK);
		}
		if ( file_exists(BASE_PATH.$bbasedir.'Spielerbild.jpg') )
		unlink (BASE_PATH.$bbasedir.'Spielerbild.jpg');
		move_uploaded_file($_FILES['spielerbild']['tmp_name'], BASE_PATH.$bbasedir.'Spielerbild.jpg');
		chmod (BASE_PATH.$bbasedir.'Spielerbild.jpg', CONFIG_CREATE_MASK);
	}
	if(isset($_REQUEST['charakter']) ) {

		if ( $profile['SteinsbergSL'] == 1 && isset($docinput['geprueft']))
		{
			$_REQUEST['charakter']['Erstellt'] = date('Y-m-d H:i');
		}
		//if ( ! isset($_REQUEST['charakter']['Adlig'])) $_REQUEST['charakter']['Adlig'] = 0;
		save_charakter($document['Charakter_id'],$_REQUEST['charakter']);
		global $flavour;
		$charakter = get_charakter($document['Charakter_id']);
		add_nodedata($id, 'title', transform(trim($charakter['Name']),'text'), $flavour);
		//add_nodedata($id, 'Contage', transform(trim($docinput['Contage']),'text'), $flavour);
		add_nodedata($id, 'Charakterbeschreibung', transform(trim($docinput['Charakterbeschreibung']),'text'), $flavour);

		global $document;
		$document = get_node($id, $flavour, false);
		if ( $document['title'] == "" ) {
			update_nodedata ($id, 'title', '(unbenannter Charakter)', $flavour);
		}
		echo '<b>&gt;&gt; Ihre Charakterdaten wurden gespeichert!</b><br /><br />';
	}
	else
	{
		$charakter = get_charakter($document['Charakter_id']);
	}

	if ( substr($charakter['Erstellt'],0,4) == '0000')
	{
		$veraenderbar = true;
	}
	else
	$veraenderbar = false;

	if ( $profile['SteinsbergSL'] == 1 || $veraenderbar )
	{
		if ( isset($_REQUEST['E']) && is_numeric($_REQUEST['E']) && isset($_REQUEST['Eid']) && is_numeric($_REQUEST['Eid']))
		{
			// Eigenschaft entfernen
			sql_query('DELETE FROM T_Charakter_Eigenschaften WHERE F_Charakter_id='.$document['Charakter_id'].
			' AND F_Eigenschaft_id='.$_REQUEST['E'].' AND EC_id='.$_REQUEST['Eid']);
			if ( $veraenderbar && sql_affected_rows() > 0 )
			{
				// Punkte erg�nzen
				$query = sql_query('SELECT Erfahrungspunkte FROM T_Eigenschaften WHERE Eigenschaft_id='.$_REQUEST['E']);
				$row = sql_fetch_row($query);
				sql_free_result($query);
				$punkte = $row[0];
				$sql = 'SELECT Sum(Erfahrungspunkte) FROM T_Charakter_Eigenschaften INNER JOIN T_Eigenschaften ON '.
				'Eigenschaft_id=F_Eigenschaft_id WHERE F_Charakter_id='.$document['Charakter_id'];
				$query = sql_query($sql);
				$row = sql_fetch_row($query);
				sql_free_result($query);
				if ( $row[0] >= 100 )
				{
					// nur Hauspunkte verwendet
					if  ( ! sql_query('UPDATE T_Haeuser SET HausEP=HausEP+'.$punkte.' WHERE Haus_id='.$charakter['F_Haus_id']))
					echo sql_error();
				}
				else
				{
					if ( $row[0] + $punkte <= 100 )
					{
						// nur Punkte anpassen
						if (! sql_query('UPDATE T_Charaktere SET EP=EP+'.$punkte.' WHERE Charakter_id='.$charakter['Charakter_id']))
						echo sql_error();
						$charakter['EP'] += $punkte;
					}
					else
					{
						if (! sql_query('UPDATE T_Charaktere SET EP=EP+'.(100-$row[0]).' WHERE Charakter_id='.$charakter['Charakter_id']))
						echo sql_error();
						if ( ! sql_query('UPDATE T_Haeuser SET HausEP=HausEP+'.($punkte+$row[0]-100).' WHERE Haus_id='.$charakter['F_Haus_id']))
						echo sql_error();
						$charakter['EP'] += 100-$row[0];
					}
				}
					
			}
		}
	}
	?>

<br />
	<?php

	if ( isset($_REQUEST['Ref']))
	echo 'Zur&uuml;ck zur <a href="?id='.$_REQUEST['Ref'].
	$Verweis.'">Anmeldung</a>';
	?>
<br />
<form action="<?=$_SERVER['PHP_SELF']?>" method="post"><input
	type="hidden" name="id" value="<?=$id?>" /> <?php
	if ( isset($_REQUEST['Ref'])) {  ?> <input type="hidden" name="Ref"
	value="<?=$_REQUEST['Ref']?>" /> <?php } ?> <?=$design['table_begin'];?>
<tr bgcolor="<?=$design['table_row_dark']?>">
	<th colspan="5">Charakterdaten</th>
</tr>
<tr>
	<td colspan="5">Am Ende gibt es die M&ouml;glichkeit,
	Hintergrundgeschichten und andere Informationen in Dateiform
	hochzuladen.</td>
</tr>
<tr>
	<td>Spieler:</td>
	<td><?=$SpielerName?></td>
	<td>&nbsp;</td>
	<td colspan="2">Haus 
	<?php
	if ( $profile['SteinsbergSL'] == 1 )
	{
		print '<select name="charakter[F_Haus_id]">';
		$sql = 'SELECT Haus_id, Hausname FROM T_Haeuser ORDER BY Hausname';
		$query = sql_query($sql);
		while ( $row = sql_fetch_array($query))
		{
			echo '<option value="'.$row['Haus_id'].'" ';
			if ( $row['Haus_id'] == $charakter['F_Haus_id'])
			{
				print 'selected="selected"';
			}
			echo '>'.$row['Hausname'].'</option>';
		}
		sql_free_result($query);		
		print '</select>';		
	}
	else
	{
		print $charakter['Hausname'];
	}
	?>
	</td>
</tr>
<tr>
	<td>Charaktername</td>
	<td><input type="text" name="charakter[Name]" id="charakter[Name]"
		size="40" value="<?=$charakter['Name']?>"
		onChange="xajax_sichereName(<?=$charakter['Charakter_id']?>,document.getElementById('charakter[Name]').value,<?=$id?>)" /><br />
	</td>
	<td>&nbsp;</td>
	<td colspan="2"><?php 
	if ( substr($charakter['Erstellt'],0,4) == '0000')
	{
		echo 'in Erstellung';
		$klasseveraenderbar = true;
		// pr�fen ob bereits Eigenschaften vorhanden sind, die die Klassen�nderung verhindern
		$sql = 'SELECT Count(*) FROM T_Charakter_Eigenschaften INNER JOIN T_Eigenschaften ON Eigenschaft_id=F_Eigenschaft_id '.
		'WHERE (';

		foreach ( array('Krieger', 'Kleriker', 'Magier') as $a )
		{
			if ( $charakter['Klasse'] != $a)
			{
				$sql .= $a.'_Gunststufe=0 OR ';
			}
		}
		$sql .= '0) AND F_Charakter_id='.$charakter['Charakter_id'];
		$query = sql_query($sql);
		if ( $row = sql_fetch_row($query))
		{
			if ( $row[0] > 0) $klasseveraenderbar = false;
		}
		sql_free_result($query);

		if ( $profile['SteinsbergSL'] == 1  )
		{
			echo ' / SL gepr�ft: <input type="checkbox" value="1" name="docinput[geprueft]" />';
		}
	}
	else
	{
		echo 'erstellt '.date('d.m.Y',strtotime($charakter['Erstellt']));
		$klasseveraenderbar = false;
	}
	?></td>
</tr>
<tr>
	<td>Adlig</td>
	<td id="Adel"><?php
	if ( $profile['SteinsbergSL'] == 1 || $veraenderbar && $charakter['F_Rasse_id'] == 1) // nur Drow k�nnen adlig sein
	{
		echo '<input type="checkbox" name="charakter[Adlig]" id="charakter[Adlig]"
value="1" onChange="xajax_zeigeGunst('.$charakter['Charakter_id'].
		',document.getElementById(\'charakter[Adlig]\').checked,document.getElementById(\'charakter[F_Position_id]\').value)"';
		if ( $charakter['Adlig'] )
		echo 'checked="checked"';
		echo ' />';
	}
	else
	{
		echo '<input type="checkbox" name="charakter[Adlig]" id="charakter[Adlig]" disabled="disabled"
value="1" onChange="xajax_zeigeGunst('.$charakter['Charakter_id'].
		',document.getElementById(\'charakter[Adlig]\').checked,document.getElementById(\'charakter[F_Position_id]\').value)"';
		if ( $charakter['Adlig'] )
		echo 'checked="checked"';
		echo ' />';
	}
	?></td>
	<td> </td>
	<td>
	Status </td>
	<td>
	<?php
	
	if ( $profile["SteinsbergSL"] == 1 )
	{
		echo '<input type="text" name="charakter[Charstatus]" value="'.$charakter['Charstatus'].'" />';
	}
	else
	{
		echo $charakter['Charstatus'];
	}
	 
	?>
	</td>
</tr>
<tr>
	<td>Rasse</td>
	<td><?php
	if ( $profile['SteinsbergSL'] == 1 || $veraenderbar )
	{
		echo '<select name="charakter[F_Rasse_id]" id="charakter[F_Rasse_id]" onChange="xajax_sichereRasse('.
		$charakter['Charakter_id'].',document.getElementById(\'charakter[F_Rasse_id]\').value)">';
		$sql = 'SELECT Rasse, Rasse_id FROM T_Rassen ORDER BY Rasse';
		$query = sql_query($sql);
		while ( $row = sql_fetch_array($query))
		{
			echo '<option value="'.$row['Rasse_id'].'" ';
			if ( $charakter['F_Rasse_id'] == $row['Rasse_id']) echo 'selected="selected"';
			echo '>'.$row['Rasse'].'</option>';
		}
		sql_free_result($query);
		echo '</select>';
	}
	else
	{
		echo get_Rasse($charakter['F_Rasse_id']);
	}
	?></td>
	<td></td>
	<td>Klasse</td>
	<td id='Klasse'><?php
	if ( $profile['SteinsbergSL'] == 1 || ($veraenderbar && $klasseveraenderbar) )
	{
		echo '<select name="charakter[Klasse]" id="charakter[Klasse]"
onChange="xajax_zeigeGunststufe('.$charakter['Charakter_id'].',document.getElementById(\'charakter[Klasse]\').value)">';
		foreach (array('Kleriker', 'Krieger', 'Magier') as $a)
		{
			echo '<option value="'.$a.'" ';
			if ( $charakter['Klasse'] == $a) echo 'selected="selected"';
			echo '>'.$a.'</option>';
		}
		echo '</select>';
	}
	else
	{
		echo $charakter['Klasse'];
	}
	?></td>
</tr>
<tr>
	<td>Position</td>
	<td><?php
	$Min_EP = 100;
	$Max_EP = 120;
	if ( $profile['SteinsbergSL'] == 1 || $veraenderbar )
	{
		echo '<select name="charakter[F_Position_id]" id="charakter[F_Position_id]" onChange="xajax_zeigeGunst('.$charakter['Charakter_id'].
		',document.getElementById(\'charakter[Adlig]\').checked,
document.getElementById(\'charakter[F_Position_id]\').value)">';
		$sql = 'SELECT Position, Position_id FROM T_Positionen ORDER BY Position';
		$query = sql_query($sql);
		while ( $row = sql_fetch_array($query))
		{
			echo '<option value="'.$row['Position_id'].'" ';
			if ( $charakter['F_Position_id'] == $row['Position_id'])
			{
				echo 'selected="selected"';
				//$MinEP = $row['Min_EP'];
				//$MaxEP = $row['Max_EP'];
			}
			echo '>'.$row['Position'].'</option>';
		}
		sql_free_result($query);
		echo '</select>';
	}
	else
	{
		echo get_Position($charakter['F_Position_id']);
	}
	?></td>
	<td></td>
	<td>Gunststufe</td>
	<td id="Gunststufe"><?php
	if ( $profile['SteinsbergSL'] == 1 )
	{
		echo '<select name="charakter[F_Gunststufe_id]" id="charakter[F_Gunststufe_id]" onChange="xajax_sichereGunststufe('.
		$charakter['Charakter_id'].',document.getElementById(\'charakter[F_Gunststufe_id]\').value)">';
		$sql = 'SELECT '.$charakter['Klasse'].', Gunststufe_id FROM T_Gunststufen ORDER BY Gunststufe_id';
		$query = sql_query($sql);
		while ( $row = sql_fetch_array($query))
		{
			echo '<option value="'.$row['Gunststufe_id'].'" ';
			if ( $charakter['F_Gunststufe_id'] == $row['Gunststufe_id']) echo 'selected="selected"';
			echo '>'.$row[$charakter['Klasse']].'</option>';
		}
		sql_free_result($query);
		echo '</select>';
	}
	else
	{
		echo get_Gunststufe($charakter['F_Gunststufe_id'], $charakter['Klasse']);
	}
	?></td>
</tr>
<tr>
	<td>Gunstpunkte</td>
	<td><?php
	if ( $profile['SteinsbergSL'] == 1 )
	{
		echo '<input type="text" name="charakter[Gunstpunkte]" id="charakter[Gunstpunkte]" size="5"
	title="Gunstpunkte" value="'.$charakter['Gunstpunkte'].'" onChange="xajax_sichereGunstpunkte('.$charakter['Charakter_id'].
		',document.getElementById(\'charakter[Gunstpunkte]\').value)" />';
		echo '<input type="text" name="charakter[Gunstpunkte_Temp]" id="charakter[Gunstpunkte_Temp]" size="5"
	title="Tempor�re Gunstpunkte" value="'.$charakter['Gunstpunkte_Temp'].'" />';
	}
	else
	{
		echo $charakter['Gunstpunkte'].' / '.$charakter['Gunstpunkte_Temp'];
	}
	echo ' - Gunst insgesamt: <span id="Gunstgesamt">';
	$Gunst = bestimmeCharGunst($charakter['Charakter_id']);
	echo $Gunst.'</span>';
	?></td>
	<td></td>
	<td>freie EP</td>
	<td><?php
	if ( $profile['SteinsbergSL'] == 1 )
	{
		echo '<input type="text" name="charakter[EP]" size="5" value="'.$charakter['EP'].'" />';
	}
	else
	{
		echo $charakter['EP'];
	}
	$query = sql_query('SELECT Hausep FROM T_Haeuser WHERE Haus_id='.$charakter['F_Haus_id']);
	$punkte = sql_fetch_row($query);
	$hausep = $punkte[0];
	echo ' / Haus: '.$hausep;
	sql_free_result($query);
	?></td>
</tr>
<tr>
	<td>EP aus Contagen</td>
	<td>
	<?php
	echo $charakter['Contage'];
	if ($profile['SteinsbergSL'] == 1 )
	{
		echo ' + <input type="text" name="charakter[ContageAdd]" size="5" value="0" />';
	}
?>
	</td>
</tr>
<tr>
	<td colspan="5">Kurzbeschreibung des Charakters (nicht mehr als 5
	Zeilen)<br />
	<textarea name="docinput[Charakterbeschreibung]" cols="60" rows="5"><?=transform($document['Charakterbeschreibung'], 'clean')?>
</textarea></td>
</tr>
<?php  
if ($profile['SteinsbergSL'] == '1' )
{
	?> 
<tr>
	<td colspan="5">Hinweise der SL zum Charakter (nur sichtbar f�r SL)<br />
	<textarea name="charakter[SLBemerkungen]" cols="60" rows="5"><?=$charakter['SLBemerkungen']?></textarea></td>
</tr>
<?php } ?>
<tr>
	<td colspan="5" align="center"><input type="submit" value="Speichern" /></td>
</tr>
</table>
</form>
<form
	action="<?=$_SERVER['PHP_SELF']?>#fertigkeit" method="post"><input type="hidden"
	name="id" value="<?=$id?>" /> <?php
	if ( isset($_REQUEST['Ref'])) {  ?> <input type="hidden" name="Ref"
	value="<?=$_REQUEST['Ref']?>" /> <?php } ?> <!--  Charaktereigenschaften festlegen -->
	<?=$design['table_begin'];?>
<tr bgcolor="<?=$design['table_row_dark']?>">
	<th colspan="5">Eigenschaften</th>
</tr>
	<?php
	$sql = 'SELECT * FROM T_Eigenschaften INNER JOIN T_Charakter_Eigenschaften ON F_Eigenschaft_id=Eigenschaft_id '.
	'WHERE F_Charakter_id='.$charakter['Charakter_id'].' ORDER BY Eigenschaftsname';
	$query = sql_query($sql);
	$summe = 0;
	while ( $row = sql_fetch_array($query))
	{
		echo '<tr>';
		echo '<td colspan="2">'.$row['Eigenschaftsname'].'</td>';
		//echo '<td></td>';
		echo '<td>'.$row['Erfahrungspunkte'].'</td>';
		echo '<td>';
		echo $row['Art'];
		echo '</td>';
		$summe += $row['Erfahrungspunkte'];
		//echo '<td>'.nl2br($row['Effekt']).'</td>';
		echo '<td>';
		if ( $veraenderbar || $profile['SteinsbergSL'] == 1 )
		echo '<a href="'.get_url($id, array('E'=>$row['Eigenschaft_id'],'Eid'=>$row['EC_id'])).'#fertigkeit" onClick="return confirm(\'Soll diese Eigenschaft wirklich entfernt werden?\');">Entf</a>';
		echo '</td>';
		echo '</tr>';
	}
	sql_free_result($query);
	echo '<tr><td colspan="2"><b>EP Gesamt</b></td></td></td><td>'.$summe;
	echo '</td></tr>';
	// TODO : Min+MaxEP ber�cksichtigen

	if ( $profile['SteinsbergSL'] == 1 || $veraenderbar && ($hausep > 0 || $charakter['EP']>0 ) )
	{
		$summe = $charakter['EP'];
		if ( $veraenderbar ) $summe += $hausep;
		// Fertigkeiten hinzuf�gen
		echo '<tr>';
		if ( $profile['SteinsbergSL'] == 1 )
		{
			$Komponenten = ' OR Art=\'Komponente\' OR Art=\'Trank/Artefakt\'';
		}
		else
		{
			$Komponenten = '';
		}
		echo '<td colspan="2" id="Fertigkeiten"><a name="fertigkeit" id="fertigkeit"></a>';
		$sql = 'SELECT Eigenschaftsname, Eigenschaft_id, Erfahrungspunkte, Art FROM T_Eigenschaften '.
		'WHERE (Erfahrungspunkte<='.$summe.' AND '.$charakter['Klasse'].'_Gunststufe>0 AND '.
		$charakter['Klasse'].'_Gunststufe<='.$charakter['F_Gunststufe_id'].' AND Art<>\'Komponente\' AND Art<>\'Trank/Artefakt\')'.$Komponenten.
		//' AND Minimum_Gunstpunkte<='.$Gunst.
		//' AND Eigenschaft_id NOT IN (SELECT F_Eigenschaft_id FROM '.
		//'T_Charakter_Eigenschaften WHERE F_Charakter_id='.$charakter['Charakter_id'].')
		' ORDER BY Eigenschaftsname';
		$query = sql_query($sql);
		if ( sql_num_rows($query) > 0 )
		{
			if ( $profile['SteinsbergSL'] == '1')
			{
				echo '<input type="text" name="FertigkeitAnzahl" value="1" />';
			}
			echo '<select name="Fertigkeit" id="Fertigkeit" '.
			'onChange="xajax_zeigeFertigkeit('.$charakter['Charakter_id'].',document.getElementById(\'Fertigkeit\').value)">';
			while ($row = sql_fetch_array($query))
			{
				echo '<option value="'.$row['Eigenschaft_id'].'">'.$row['Eigenschaftsname'].' ('.$row['Art'].'/'.$row['Erfahrungspunkte'].')</option>';
			}
			echo '</select>';
		}
		else
		{
			echo 'Es gibt keine Fertigkeiten die der Charakter erwerben k�nnte.';
		}
		sql_free_result($query);
		echo '</td>';
		echo '</tr>';
		echo '<tr><td colspan="2" id="Fertigkeitsbeschreibung"></td><td id="Fertigkeitsgunststufe"></td></tr>';
		echo '<tr><td colspan="5" align="center"><input type="submit" value="Hinzuf�gen" /></td></tr>';
	}
	?>
<tr>
	<td colspan="5">
	<hr />
	</td>
</tr>
</form>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post"
	enctype="multipart/form-data"><input type="hidden" name="id"
	value="<?=$id?>" /> <?php
	if ( isset($_REQUEST['Ref'])) {  ?> <input type="hidden" name="Ref"
	value="<?=$_REQUEST['Ref']?>" /> <?php } 
	echo $design['table_end']; ?> <?=LANG_PICTURE?> (max. 150 kB, muss
.jpg-Datei sein)<br />
<input type="file" name="spielerbild"> <?php
if ( file_exists($bbasedir.'Spielerbild.jpg') ) {
	echo '&nbsp;&nbsp;&nbsp;<a href="'.$bbasedir.
	'Spielerbild.jpg">vorhandenes Bild allein</a><br />';
	echo '<img src="'.$bbasedir.'Spielerbild.jpg">';
}
?> <br />
<br />
<br />
<br />
<input type="submit" name="savechar" value="Bild hochladen" /></form>
</p>
<p><a target="_Blank" href="/DrowCharakterbogen.class.php">Charakterbogen (PDF)</a></p>
<?php
$_SESSION['Charakter_id'] = $charakter['Charakter_id'];
$_SESSION['id'] = $id;
if ( isset($_REQUEST['Ref']))
{
	echo 'Zur&uuml;ck zur <a href="?id='.$_REQUEST['Ref'].$Verweis.'"></a>';
}
}
else
{
	echo "Falsche Charakternummer!<br />";
   $document['show_files'] = false;
   $document['comments'] = false;
}
?>