<?php
function ist_Hausmitglied($user_id, $haus_id)
{
	$sql = 'SELECT Count(*) FROM T_Charaktere WHERE F_Spieler_id='.SESSION_DBID.' AND F_Haus_id='.$haus_id;
	$query = sql_query($sql);
	$row = sql_fetch_row($query);
	sql_free_result($query);
	return $row[0] > 0;
}


function add_fertigkeit($char_id, $fertigkeit_id, $Anzahl='')
{
	$anz = 1;
	if ( is_numeric($Anzahl) )
	{
		$anz = $Anzahl;
	}
	if ( is_numeric($char_id) && is_numeric($fertigkeit_id))
	{
		for ($i = 1; $i<=$anz; $i++)
		{
		$insert = false;
		$sql = 'SELECT Erfahrungspunkte FROM T_Eigenschaften WHERE Eigenschaft_id='.$fertigkeit_id;
		$query = sql_query($sql);
		if ( $punkte = sql_fetch_row($query))
		{
			$sql = 'UPDATE T_Charaktere SET EP=EP-'.$punkte[0].' WHERE Charakter_id='.$char_id;
			sql_query($sql);
			// wenn zu wenig Punkte -> Haus pr�fen
			$sql = 'SELECT F_Haus_id, EP, Hausep FROM T_Charaktere INNER JOIN T_Haeuser ON F_Haus_id=Haus_id WHERE Charakter_id='.$char_id;
			$query = sql_query($sql);
			if ( $row = sql_fetch_array($query) )
			{
				if ( $row['EP'] < 0 )
				{
					if ( $row['Hausep'] >= abs($row['EP']))
					{
						// aus den Hauspunkten abziehen
						$sql = 'UPDATE T_Haeuser SET Hausep=Hausep-'.abs($row['EP']).' WHERE Haus_id='.$row['F_Haus_id'];
						sql_query($sql);
						$sql = 'UPDATE T_Charaktere SET EP=0 WHERE Charakter_id='.$char_id;
						sql_query($sql);
						$insert = true;
					}
				}
				else
				{
					$insert = true; 
				}
			}
			if ( $insert )
			{
				$sql = 'INSERT INTO T_Charakter_Eigenschaften (F_Charakter_id,F_Eigenschaft_id, Erstellt) VALUES ('.
				$char_id.','.$fertigkeit_id.',\''.date('Y-m-d H:i').'\')';
				sql_query($sql);
			}
		}
		else
		{
			echo 'Fehler: Eigenschaft '.$fertigkeit_id.' nicht gefunden!';
		}
		sql_free_result($query);
		}
	}
}

function save_charakter($char_id, $charakter)
{
	$sql = 'UPDATE T_Charaktere SET ';
	$a = array();
	if ( ! is_numeric($charakter['Adlig']) ) unset($charakter['Adlig']);
	$add = 0;
	if ( isset($charakter['ContageAdd']))
	{
		$add = $charakter['ContageAdd'];
		unset($charakter['ContageAdd']);
	}
	foreach ( $charakter as $eintrag => $wert)
	{
		$a[] = sql_real_escape_string($eintrag).'=\''.sql_real_escape_string($wert).'\'';
	}
	$sql .= implode(',', $a);
	if ( $add <> 0 )
	{
		// Punkte f�r Contage bearbeiten
		$sql .= ', Contage=Contage+'.$add.', EP=EP+'.$add;
	}
	$sql .= ' WHERE Charakter_id='.$char_id;
	if (! sql_query($sql))
	{
		throw new Exception('Charakter '.$char_id.' konnte nicht gespeichert werden: '.sql_error());
	}
}
function get_charakter($char_id)
{
	$char = array('Name' => '-nicht angelegt-');
	if ( is_numeric($char_id) )
	{
		$sql = 'SELECT * FROM T_Charaktere 
				INNER JOIN T_Positionen ON F_Position_id=Position_id
				INNER JOIN T_Rassen ON F_Rasse_id=Rasse_id
				INNER JOIN T_Haeuser ON F_Haus_id=Haus_id
				INNER JOIN T_Gunststufen ON F_Gunststufe_id=Gunststufe_id
				WHERE Charakter_id='.$char_id;
		$query= sql_query($sql);
		if ( ! $char = sql_fetch_array($query))
		{
			$char = array('Name' => '-nicht angelegt-');
		}
		sql_free_result($query);
		$prow = get_profile($char['F_Spieler_id'], false);
		$SpielerName = $prow['Vorname']." ".$prow['Name'];
		$char['Spielername'] = $SpielerName;
		$char['Gunststufe'] = $char[$char['Klasse']];
	}
	return $char;
}
function bestimmeGunst($haus_id)
{
	// Summe aller Gunstpunkte
	// +Summe der Gunststufenpunkte
	// +Summe der Gunstpositionspunkte
	// +Summe adel (= jeweils 1 Punkt)
	$sql = 'SELECT Sum(Adlig)+Sum(Gunstpunkte_Temp)+Sum(Gunstpunkte)+Sum(Gunststufenpunkte)+Sum(Gunstpositionpunkte) FROM T_Charaktere '.
	'INNER JOIN T_Positionen ON F_Position_id=Position_id '.
	'INNER JOIN T_Gunststufen ON F_Gunststufe_id=Gunststufe_id '.
	'WHERE F_Haus_id='.$haus_id;
	$query = sql_query($sql);
	if ( $row = sql_fetch_row($query))
	{
		$sum = $row[0];
	}
	else
	{
		$sum = 0;
	}
	sql_free_result($query);
	return $sum;
}

function bestimmeCharGunst($char_id)
{
	// Summe aller Gunstpunkte
	// +Summe der Gunststufenpunkte
	// +Summe der Gunstpositionspunkte
	// +Summe adel (= jeweils 1 Punkt)
	$sql = 'SELECT Sum(Adlig)+Sum(Gunstpunkte_Temp)+Sum(Gunstpunkte)+Sum(Gunststufenpunkte)+Sum(Gunstpositionpunkte) FROM T_Charaktere '.
	'INNER JOIN T_Positionen ON F_Position_id=Position_id '.
	'INNER JOIN T_Gunststufen ON F_Gunststufe_id=Gunststufe_id '.
	'WHERE Charakter_id='.$char_id;
	$query = sql_query($sql);
	if ( $row = sql_fetch_row($query))
	{
		$sum = $row[0];
	}
	else
	{
		$sum = 0;
	}
	sql_free_result($query);
	return $sum;
}

function bestimmeEP($char_id)
{
	// Summe aller verbrauchten EP
	$sql = 'SELECT Sum(Erfahrungspunkte) FROM T_Charakter_Eigenschaften '.
	'INNER JOIN T_Eigenschaften ON F_Eigenschaft_id=Eigenschaft_id '.
	'WHERE F_Charakter_id='.$char_id;
	if ( ! $query = sql_query($sql) ) throw new Exception('Fehler: '.$sql.'/'.sql_error());
	if ( $row = sql_fetch_row($query))
	{
		$sum = $row[0];
		if ( ! is_numeric($sum)) $sum = 0;
	}
	else
	{
		$sum = 0;
	}
	sql_free_result($query);
	return $sum;
}

function get_Haus($hausname)
{
	$haus = array();
	$sql = 'SELECT Haus_id FROM T_Haeuser WHERE Hausname=\''.sql_real_escape_string($hausname).'\'';
	$query = sql_query($sql);
	if ( $haus = sql_fetch_row($query))
	{
	}
	else
	{
		$haus[0] = -1;
	}
	return $haus[0];
}

function get_Position($id)
{
	if ( ! is_numeric($id)) return '-Fehler-';
	$sql = 'SELECT Position FROM T_Positionen WHERE Position_id='.$id;
	$query = sql_query($sql);
	if ( $entry = sql_fetch_row($query))
	{
	}
	else
	{
		$entry[0] = '-unbekannt-';
	}
	return $entry[0];
}
function get_Gunststufe($id, $Klasse)
{
	if ( ! is_numeric($id)) return '-Fehler-';
	$sql = 'SELECT '.$Klasse.' FROM T_Gunststufen WHERE Gunststufe_id='.$id;
	$query = sql_query($sql);
	if ( $entry = sql_fetch_row($query))
	{
	}
	else
	{
		$entry[0] = '-unbekannt-';
	}
	return $entry[0];
}

function get_Rasse($id)
{
	if ( ! is_numeric($id)) return '-Fehler-';
	$sql = 'SELECT Rasse FROM T_Rassen WHERE Rasse_id='.$id;
	$query = sql_query($sql);
	if ( $entry = sql_fetch_row($query))
	{
	}
	else
	{
		$entry[0] = '-unbekannt-';
	}
	return $entry[0];
}


function create_Charakter($SpielerNr, $hausname)
{
	$sql = 'INSERT INTO T_Charaktere (Name, Klasse, F_Gunststufe_id, F_Rasse_id, F_Position_id, Adlig, '.
	'F_Spieler_id, F_Haus_id) VALUES (\'-nicht festgelegt-\',\'Krieger\',1,1,10,0,'.
	$SpielerNr.','.get_Haus($hausname).')';
	if ( ! sql_query($sql) )
	{
		echo sql_error();
	}
	$charid = sql_insert_id();

	return $charid;
}

?>