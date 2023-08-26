<?php


/**
 * Klasse Charakter
 * Kapselt die Eigenschaften eines Spielcharakters mit seinen Fertigkeiten
 * Enth�lt zus�tzlich diverse Implemtierungen der Atlantis-Regeln
 * 2006 Christoph Griep
 * 
 */
class Charakter
{
	/** 
	 * Magiepunkte pro Zauber h�ngen vom Rang ab: 
	 * Wanderer: 3 Punkte
	 * Adept: 5
	 * Meister: 10
	 * Gro�meister: 20
	 */
	var $Magiepunkte = array (
		1 => 3,
		2 => 5,
		3 => 10,
		4 => 20
	);

	var $Felder = array (
		'Charaktername',
		'F_Klasse_id',
		'Punkte',
		'Contage',
		'Atlantiscontage',
		'F_Rasse_id',
		'F_Spieler_id',
		'F_MagischeBesonderheit_id',
		'Gesamtpunkte',
		'Bild',
		'Charaktergeschichte',
		'Kurzbeschreibung',
		'Historie'
	);
	// IDs des �bernat�rliches-Wesen-Vorteils
	const UEBERNATUERLICHLEICHT = 396;
	const UEBERNATUERLICHGANZ = 398;
	// ID der Klassen
	const SCHAMANE = 7;
	const ELEMENTARIST = 11;
	// Fertigkeiten f�r Meister und Gro�meisterpr�fungen
	const MEISTER = 509;
	const GROSSMEISTER = 508;
	const MAGIEPUNKTE = 510;
	// Arten der Fertigkeiten
	const VORTEIL = 'V';
	const NACHTEIL = 'N';
	const FERTIGKEIT = 'F';
	const MAGIE = 'M';
	const VORTEILUEBERNATUERLICH = 'Uv';
	const NACHTEILUEBERNATUERLICH = 'Un';
	const TOTEM = 'St';
	const ELEMENT = 'Em';

	var $Charakter_id;
	var $Fertigkeiten;
	var $Spruchlisten; // zu Beginn ausgew�hlte Spruchlisten
	var $vorhandeneSpruchlisten; // tats�chlich schon gew�hlte Spruchlisten 
	var $Spezialisierungen;

	function Charakter()
	{
		$this->Charakter_id = NULL; // Neuer Charakter
		$this->Charaktername = '';
		$this->F_Klasse_id = NULL;
		$this->Contage = 0;
		$this->Atlantiscontage = 0;
		$this->Historie = '';
		$this->F_Spieler_id = NULL;
		$this->F_MagischeBesonderheit_id = NULL;
		$this->Gesamtpunkte = 0;
		$this->F_Rasse_id = NULL;
		$this->Punkte = 35;
		$this->Bild = '';
		$this->Charaktergeschichte = '';
		$this->Kurzbeschreibung = '';
		$this->Fertigkeiten = array ();
		$this->Verboten = array ();
		// Spruchlistenanzahl entspricht Stufe in der gew�hlten Klasse
		$this->Spruchlisten = array ();
		$this->bestimmeSpezialisierungsraenge();
	}
	function nullWert($wert)
	{
		if ( $wert <=0 || $wert == NULL)
		  return 'NULL';
		else
		  return $wert; 
	}
	/**
	 * Erh�ht die Contageanzahl um die angegebene Anzahl und erg�nzt die Historie
	 * des Charakters um einen entsprechenden Eintrag. Ist es ein Atlantiscon, 
	 * so werden zus�tzlich die Atlantis-Contage erh�ht.
	 * Pro Contag wird au�erdem ein Punkt f�r Fertigkeiten gegeben.
	 * @param int Anzahl- die Anzahl der Contage, die erg�nzt werden
	 * @param string Con der Name des Cons f�r die Historie
	 * @param boolean Atlantis true, wenn es ein Atlantiscon ist
	 * @return false, wenn ein Fehler aufgetreten ist, true sonst
	 */
	function ContageErgaenzen($Anzahl, $Con, $Atlantis = false)
	{
		if (!is_numeric($Anzahl) || $Con == '' || $Anzahl <= 0)
			return false;
		$this->Contage += $Anzahl;
		if ($Atlantis)
			$this->Atlantiscontage += $Anzahl;
		$s = 'Erfahrung erg�nzt ' . date('d.m.Y H:i') . ': ' . $Con .
		' (' . $Anzahl . ' Contage';
		if ($Atlantis)
			$s .= ',Atlantis';
		$this->Historie = $s . ")\n" . $this->Historie;
		$this->Punkte += $Anzahl;
		return true;
	}
	/**
	 * Maximale Anzahl von Spruchlisten, die der Charakter belegen darf 
	 * Entspricht dem Rang in der Klasse des magischen Charakters
	 * @return int die Anzahl der Spruchlisten 
	 */
	function getMaxSpruchlistenanzahl()
	{
		if (!$this->isMagisch())
			return 0;
		$anz = 1;
		for ($i = 1; $i < 5; $i++)
			if ($this->RangErlaubt[$this->F_Klasse_id][$i])
				$anz = $i;
		return $anz;
	}
	/**
	 * Setzt die Rasse des Charakters. Dabei werden alle rassenspezifischen
	 * Fertigkeiten, Vor- und Nachteile in das entsprechende Attribut eingelesen
	 * @param int die ID der Rasse  
	 */
	function setRasse($rasse)
	{
		if (!is_numeric($rasse))
			return false;
		$this->F_Rasse_id = $rasse;
		$Kosten = 0;
		// vorgegebene Rassenfertigkeiten beruecksichtigen
		if (!$query = sql_query('SELECT * FROM T_RassenFertigkeiten INNER JOIN' .
			' T_Fertigkeiten ON Fertigkeit_id=F_Fertigkeit_id ' .
			'WHERE Pflicht AND F_Rasse_id=' . $this->F_Rasse_id))
		{
			die(sql_error());
		}
		while ($row = sql_fetch_array($query))
		{
			if ($row['Kosten'] == 0 && ! $row['Pflicht'])
				$this->Verboten[] = $row['Fertigkeit_id'];
			else
			{
				// �bernat�rliche kostenlose Rassenvorteile sind von Anfang an aktiviert
				if ($row['Art'] == self :: VORTEILUEBERNATUERLICH )
				{
					if ( $row['Kosten'] + $row['Kosten1'] == 0)
					{
				        $row['Stufe'] = 1;
					}					
				}
				$this->Fertigkeiten[$row['Fertigkeit_id']] = $row;
			}
		}
		sql_free_result($query);
		return $Kosten;
	}
	/**
	 * Gibt den Namen der Rasse des Charakters zur�ck
	 * @return der Name der Rasse
	 */
	function getRasse()
	{
		if (!is_numeric($this->F_Rasse_id))
			return '';
		$query = sql_query('SELECT * FROM T_Rassen WHERE Rasse_id=' . $this->F_Rasse_id);
		$rasse = sql_fetch_array($query);
		sql_free_result($query);
		return $rasse['Rasse'];
	}
	/**
	 * Pr�ft ob eine Fertigkeit erlaubt ist.
	 * Dazu darf sie nicht verboten sein 
	 * @param int id der Fertigkeit, die gepr�ft werden soll
	 * @return boolean true, wenn die Fertigkeit erlaubt ist, false sonst 
	 */
	function fertigkeitErlaubt($fertigkeit_id)
	{
		if (in_array($fertigkeit_id, $this->Verboten))
			return false;
		else
		{
			return true;
		}
	}
	function holeFertigkeitPunkte($fertigkeit_id, $spezialisierung_id = NULL)
	{
		if (is_numeric($fertigkeit_id))
		{
			// Besonderheit: Abh�ngigkeiten pr�fen 
			if (!$query = sql_query('SELECT * FROM T_FertigkeitenAbhaengigkeiten ' .
				'INNER JOIN T_Fertigkeiten ON Fertigkeit_id=F_Fertigkeiten_id2 ' .
				'WHERE F_Fertigkeiten_id2=' . $fertigkeit_id))
				echo sql_error();
			$ok = true;
			$Kosten = 0;
			while (($row = sql_fetch_array($query)) && $ok)
			{
				if (!isset ($this->Fertigkeiten[$row['F_Fertigkeiten_id1']]) && $row['Erlaubt'])
				{
					$ok = false;
				}
				elseif (!$row['Erlaubt'] && isset ($this->Fertigkeiten[$row['F_Fertigkeiten_id1']]))
				{
					$ok = false;
				} else
				{
					$Kosten += $row['Kosten'];
				}
			}
			sql_free_result($query);
			if (!$ok)
			{
				$Kosten = 1000;
			}
			if (is_numeric($spezialisierung_id))
			{
				if (!$query = sql_query('SELECT * FROM T_SpezialisierungenFertigkeiten ' .
					'WHERE F_Fertigkeit_id=' . $fertigkeit_id . ' AND F_Spezialisierung_id=' .
					$spezialisierung_id))
					die('hfP:' . sql_error());
				$f = sql_fetch_array($query);
				sql_free_result($query);
				$Kosten = $Kosten + $f['Kosten'];
			} else
			{

				if (!$query = sql_query('SELECT * FROM T_Fertigkeiten ' .
					'WHERE Fertigkeit_id=' . $fertigkeit_id))
					die('hfP2:' . sql_error());
				$f = sql_fetch_array($query);
				sql_free_result($query);
				$Kosten = $Kosten + $f['Kosten1'];
			}
			return $Kosten;
		}
		return 1000;
	}
	function fertigkeitHinzu($fertigkeit_id, $spezialisierung_id = NULL)
	{
		$s = 'Fertigkeit-ID nicht numerisch!';
		if (is_numeric($fertigkeit_id))
		{
			$s = '';
			if (is_numeric($spezialisierung_id))
			{
				$query = sql_query('SELECT * FROM (T_Fertigkeiten INNER JOIN  ' .
				'(T_SpezialisierungenFertigkeiten' .
				' INNER JOIN (T_Spezialisierungen INNER JOIN T_Spezialisierungsklassen ON ' .
				'F_Klasse_id=Klasse_id) ON F_Spezialisierung_id=Spezialisierung_id) ' .
				' ON F_Fertigkeit_id=Fertigkeit_id) ' .
				'INNER JOIN T_Raenge ON Rang_id=F_Rang_id' .
				' WHERE Fertigkeit_id=' . $fertigkeit_id);
				$f = sql_fetch_array($query);
				sql_free_result($query);
				$f['Spezialisierung_id'] = $spezialisierung_id;
				$punkte = $this->holeFertigkeitPunkte($fertigkeit_id, $spezialisierung_id);
			} else
			{
				// Vor-/Nachteile
				$query = sql_query('SELECT * FROM T_Fertigkeiten WHERE Fertigkeit_id=' . $fertigkeit_id);
				$f = sql_fetch_array($query);
				sql_free_result($query);
				$punkte = $f['Kosten1'];
				// �bernat�rliche Dinge werden anders berechnet
				if ($f['Art'] == self :: VORTEILUEBERNATUERLICH || $f['Art'] == self :: NACHTEILUEBERNATUERLICH)
					$punkte = 0;

				/*
				 * 
				 * {
				if ($_SESSION['�Vorteilpunkte'] + $row['Kosten1'] > Min($MaxUePunkte, abs($_SESSION['�Nachteilpunkte'])))
				{
					$_SESSION['Fehler'] = 'Maximal ' . $MaxUePunkte . ' �bernat�rliche Vorteilpunkte, �bernat�rliche ' .
					'Vorteilpunkte m�ssen mit �bernat�rlichen Nachteilpunkten ' .
					'ausgeglichen sein';
					FertigkeitHinzufuegen(- $row['Fertigkeit_id']);
				if (abs($_SESSION['�Nachteilpunkte'] + $row['Kosten1']) > $MaxUePunkte)
				{
					$_SESSION['Fehler'] = 'Maximal ' . $MaxUePunkte . ' Nachteilpunkte';
					FertigkeitHinzufuegen(- $row['Fertigkeit_id']);
				} else
				{
					$_SESSION['�Nachteilpunkte'] += $row['Kosten1'];
				}
				}
				 * normale Vorteile
				 * if ($Charakter->Hilfspunkte['Vorteilpunkte'] + $row['Kosten1'] > Min($Charakter->getMaximalVorteilsPunkte($row['Art']), $Charakter->Punkte + abs($Charakter->Hilfspunkte['Nachteilpunkte'])))
				{
					$_SESSION['Fehler'] = 'Maximal ' . $Charakter->getMaximalVorteilsPunkte($row['Art']) .
					' Vorteilpunkte, Vorteilpunkte max. 35+Nachteilpunkte';
					FertigkeitHinzufuegen(- $row['Fertigkeit_id']);
				{
				if (abs($Charakter->Hilfspunkte['Nachteilpunkte'] + $row['Kosten1']) > 40)
				{
					$_SESSION['Fehler'] = 'Maximal 40 Nachteilpunkte k�nnen vergeben werden.';
					FertigkeitHinzufuegen(- $row['Fertigkeit_id']);
				 */
			}
			$this->Fertigkeiten[$fertigkeit_id] = $f;
			$this->Punkte -= $punkte;
			$this->bestimmeSpezialisierungsraenge();
			$s = '';
			// TODO: Abh�ngigkeiten beachten!
			/*
			 * 	if (!$query = sql_query('SELECT * FROM T_FertigkeitenAbhaengigkeiten ' .
			'INNER JOIN T_Fertigkeiten ON Fertigkeit_id=F_Fertigkeiten_id2 ' .
			'WHERE F_Fertigkeiten_id1=' . $fid . ' AND Automatisch'))
			echo sql_error();
			
			 */

		}
		return $s;
	}
	function fertigkeitEntfernen($fertigkeit_id)
	{
		// TODO: Fehler auswerten und als String zur�ckgeben!
		// TODO: Abh�ngige Dinge auswerten und entfernen
		$Fehler = 'Diese Fertigkeit ist nicht vorhanden!';
		if (isset ($this->Fertigkeiten[$fertigkeit_id]))
		{
			if (isset ($this->Fertigkeiten[$fertigkeit_id]['Pflicht']) && $this->Fertigkeiten[$fertigkeit_id]['Pflicht'])
			{
				// vorgegebene Fertigkeit!
				$Fehler = 'Vorgegebene Fertigkeiten k�nnen nicht entfernt werden!';
			} else
			{
				if (isset ($this->Fertigkeiten['Spezialisierung_id']))
					$punkte = $this->holeFertigkeitPunkte($fertigkeit_id, $this->Fertigkeiten[$fertigkeit_id]['Spezialisierung_id']);
				else
					$punkte = $this->Fertigkeiten[$fertigkeit_id]['Kosten1'];
				if (isset ($this->Fertigkeiten[$fertigkeit_id]['Kosten']))
					$punkte += $this->Fertigkeiten[$fertigkeit_id]['Kosten'];
				// �bernat�rliches wird extra abgerechnet 
				if ($this->Fertigkeiten[$fertigkeit_id]['Art'] == self :: VORTEILUEBERNATUERLICH || $this->Fertigkeiten[$fertigkeit_id]['Art'] == self :: NACHTEILUEBERNATUERLICH)
					$punkte = 0;
				$this->Punkte += $punkte;
				// �bernat�rliche Fertigkeiten l�schen wenn der Vorteil entfernt wird
				if ($fertigkeit_id == self :: UEBERNATUERLICHLEICHT || $fertigkeit_id == self :: UEBERNATUERLICHGANZ)
				{
					foreach ($this->Fertigkeiten as $key => $value)
					{
						if ($value['Art'] == self :: VORTEILUEBERNATUERLICH || $value['Art'] == self :: NACHTEILUEBERNATUERLICH)
						{
							unset ($this->Fertigkeiten[$key]);
						}
					}
				}
				unset ($this->Fertigkeiten[$fertigkeit_id]);
				$this->bestimmeSpezialisierungsraenge();
				$Fehler = '';
			}
		}
		return $Fehler;
	}
	function getKlasse()
	{
		if ($this->F_Klasse_id != NULL)
		{
			$query = sql_query('SELECT Klasse FROM T_Spezialisierungsklassen ' .
			'WHERE Klasse_id=' . $this->F_Klasse_id);
			$spez = sql_fetch_row($query);
			sql_free_result($query);
			return $spez[0];
		} else
		{
			return '';
		}
	}
	function getSpezialisierung()
	{
		if ($this->F_MagischeBesonderheit_id != NULL)
		{
			$query = sql_query('SELECT Fertigkeit FROM T_Fertigkeiten ' .
			'WHERE Fertigkeit_id=' . $this->F_MagischeBesonderheit_id);
			$spez = sql_fetch_row($query);
			sql_free_result($query);
			return $spez[0];
		} else
		{
			return '';
		}
	}
	/** 
	 * Gibt an, ob es sich um einen magischen Charakter handelt
	 * @return true, wenn der Charakter magisch ist, false sonst 
	 */
	function isMagisch()
	{
		return $this->F_Klasse_id != NULL;
	}
	function saveInDatabase()
	{
		// Charakter in Datenbank speichern
		if ($this->Charakter_id == NULL)
		{
			// Neuer Charakter
			$sql = 'INSERT INTO T_Charaktere (';
			$sql .= implode(',', $this->Felder);
			$sql .= ') VALUES (';
			foreach ($this->Felder as $Feld)
			{
				if ( strpos($this->$Feld, '_id') !== false)
				  $w = $this->nullWert($this-> $Feld);
				else
				  $w = $this->$Feld;
				$sql .= '"' . sql_real_escape_string($w) . '",';
			}
			// letztes , entfernen
			$sql = substr($sql, 0, -1);
			$sql .= ')';
			if (!sql_query($sql))
				echo 'Save' . $sql . '<br />' . sql_error();
			$this->Charakter_id = sql_insert_id();
		} else
		{
			// vorhandener Charakter
			$sql = 'UPDATE T_Charaktere SET ';
			foreach ($this->Felder as $Feld)
			{
				if ( strpos($this->$Feld, '_id') !== false)
				  $w = $this->nullWert($this-> $Feld);
				else
				  $w = $this->$Feld;
				$sql .= $Feld . '="' .
				sql_real_escape_string($w) . '",';
			}
			// letztes , entfernen
			$sql = substr($sql, 0, -1);
			$sql .= ' WHERE Charakter_id=' . $this->Charakter_id;
			if (!sql_query($sql))
				echo 'x1' . $sql . '/' . sql_error();
		}
		// Spruchlisten speichern
		// Dies kann nur bei der Neuerstellung erfolgen, da die Spruchlisten 
		// danach nicht mehr zu �ndern sind 
		sql_query('DELETE FROM T_CharakterSpruchlisten WHERE F_Charakter_id=' . $this->Charakter_id);
		foreach ($this->Spruchlisten as $key => $spruchliste)
		{
			sql_query('INSERT INTO T_CharakterSpruchlisten (F_Charakter_id, ' .
			'F_Spezialisierung_id) VALUES (' . $this->Charakter_id . ',' . $key . ')');
		}

		// Fertigkeiten speichern
		sql_query('DELETE FROM T_CharakterFertigkeiten WHERE F_Charakter_id=' . $this->Charakter_id);
		$sql = 'INSERT INTO T_CharakterFertigkeiten (F_Charakter_id,F_Fertigkeit_id,' .
		'F_Spezialisierung_id,Stufe) VALUES (' . $this->Charakter_id . ',';
		foreach ($this->Fertigkeiten as $key => $f)
		{
			if ($f['Art'] == self :: FERTIGKEIT || $f['Art'] == self :: MAGIE)
				$s = $sql . $key . ',' . $this->nullWert($f['Spezialisierung_id']) . ',1)';
			else
			{
				if ($f['Art'] == self :: VORTEILUEBERNATUERLICH)
				{
					if ((!isset ($f['Stufe']) || !is_numeric($f['Stufe'])) && $f['Kosten1'] > 0)
					{
						$f['Stufe'] = 0;
					}
				}
				if (!isset ($f['Stufe']) || !is_numeric($f['Stufe']))
					$f['Stufe'] = 1;
				$s = $sql . $key . ',NULL,' . $f['Stufe'] . ')';
			}
			if (!sql_query($s))
				echo sql_query() . '/' . $s;
		}
	}
	/**
	 * Gibt an, ob der Charakter �bernat�rlich ist. 
	 * @return boolean true, wenn es ein �bernat�rlicher Charakter ist, false sonst  
	 */
	function isUebernatuerlich()
	{
		if (isset ($this->Fertigkeiten[self :: UEBERNATUERLICHLEICHT]) || isset ($this->Fertigkeiten[self :: UEBERNATUERLICHGANZ]))
			return true;
		else
			return false;
	}
	/**
	 * L�d einen Charakter aus der Datenbank.
	 * @param int die id des Charakters, der geladen werden soll
	 * @return boolean true, wenn alles ok ist, false wenn ein Fehler aufgetreten ist.
	 */
	function loadFromDatabase($characterid)
	{
		if (is_numeric($characterid))
		{
			$query = sql_query('SELECT * FROM T_Charaktere ' .
			'WHERE Charakter_id=' . $characterid);
			if (!$charakter = sql_fetch_array($query))
			{
				die('Konnte Charakter ' . $characterid . ' nicht laden.');
			}
			sql_free_result($query);
			foreach ($charakter as $key => $value)
				$this-> $key = $value;
			// Null-Werte korrigieren
			if ($this->F_Klasse_id == 0)
				$this->F_Klasse_id = NULL;
			if ($this->F_MagischeBesonderheit_id == 0)
				$this->F_MagischeBesonderheit_id = NULL;

			$this->Charakter_id = $characterid;
			// Rasse festlegen
			$this->setRasse($this->F_Rasse_id);
			// Sicherheitshalber werden nun die Rassenfertigkeiten wieder gel�scht,
			// falls �nderungen vorgenommen wurden
			$this->Fertigkeiten = array ();

			// Vor- und Nachteile auslesen			
			$sql = 'SELECT T_Fertigkeiten.*, Stufe, Spruchliste, ' .
			'Klasse, F_Klasse_id, Spezialisierung, Spezialisierung_id, Kosten, Kosten1,' .
			'Rang, F_Rang_id FROM T_CharakterFertigkeiten ' .
			'INNER JOIN (T_Fertigkeiten LEFT JOIN  ((T_SpezialisierungenFertigkeiten' .
			' INNER JOIN (T_Spezialisierungen INNER JOIN T_Spezialisierungsklassen ON ' .
			'F_Klasse_id=Klasse_id) ON F_Spezialisierung_id=Spezialisierung_id) ' .
			'INNER JOIN T_Raenge ON Rang_id=F_Rang_id)' .
			' ON F_Fertigkeit_id=Fertigkeit_id) ON ' .
			'T_CharakterFertigkeiten.F_Fertigkeit_id=Fertigkeit_id ' .
			' WHERE F_Charakter_id=' . $characterid .
			' AND (T_CharakterFertigkeiten.F_Spezialisierung_id IS NULL OR ' .
			'T_CharakterFertigkeiten.F_Spezialisierung_id=Spezialisierung_id)';
			if (!$query = sql_query($sql))
				echo 'LFD:' . sql_error();
			while ($f = sql_fetch_array($query))
			{
				$this->Fertigkeiten[$f['Fertigkeit_id']] = $f;
			}
			sql_free_result($query);
			// Spruchlisten
			if (!$query = sql_query('SELECT * FROM T_CharakterSpruchlisten ' .
				'INNER JOIN T_Spezialisierungen ON F_Spezialisierung_id=Spezialisierung_id' .
				' WHERE F_Charakter_id=' . $characterid))
				echo 'x2' . sql_error();
			$this->Spruchlisten = array ();
			while ($liste = sql_fetch_array($query))
			{
				$this->Spruchlisten[$liste['Spezialisierung_id']] = $liste;
			}
			sql_free_result($query);
			$this->bestimmeSpezialisierungsraenge();
			return TRUE;
		} else
		{
			return FALSE;
		}
	}
	/**
	 * Bestimmt ob eine Fertigkeit gew�hlt werden darf oder nicht. Dabei werden die 
	 * R�nge, die Punkte und die Spezialisierungen ber�cksichtigt.
	 * @param int id der Fertigkeiten, die gepr�ft werden soll
	 * @param int id der Spezialisierung
	 * @param int id des Ranges
	 * @return boolean true, wenn die Fertigkeit gew�hlt werden darf, false sonst 
	 */
	function fertigkeitMoeglich($fertigkeit_id, $spezialisierung_id, $rang_id)
	{
		if (!$this->fertigkeitErlaubt($fertigkeit_id))
			return false;
		if (!isset ($this->Fertigkeiten[$fertigkeit_id]))
		{
			// TODO: �nderung der Kosten + eigentliche Kosten
			$Kosten = $this->holeFertigkeitPunkte($fertigkeit_id, $spezialisierung_id);
			if ($Kosten <= $this->Punkte)
			{
				// Fertigkeit ist m�glich. Aber stimmt auch der Rang?
				if ($this->RangErlaubt[$spezialisierung_id][$rang_id])
				{
					return true;
				}
			}
		}
		return false;

	}
	/**
	* Bestimmt ob ein Vor-/Nachteil gew�hlt werden darf oder nicht. Dabei werden die 
	* Punkte ber�cksichtigt. 
	* @param int id der Fertigkeiten, die gepr�ft werden soll
	* @return boolean true, wenn die Fertigkeit gew�hlt werden darf, false sonst 
	*/
	function vorteilMoeglich($fertigkeit_id)
	{
		if (!$this->fertigkeitErlaubt($fertigkeit_id))
			return false;
		if (!isset ($this->Fertigkeiten[$fertigkeit_id]))
		{
			$query = sql_query('SELECT * FROM T_Fertigkeiten WHERE Fertigkeit_id=' . $fertigkeit_id);
			$f = sql_fetch_array($query);
			sql_free_result($query);
			$Kosten = $this->holeFertigkeitPunkte($fertigkeit_id);
			if ($f['Art'] == self :: VORTEIL || $f['Art'] == self :: NACHTEIL)
			{
				if ($Kosten <= $this->Punkte)
				{
					if (abs($this->getVorteilspunkte($f['Art']) + $Kosten) <= $this->getMaximalVorteilspunkte($f['Art']))
					{
						return true;
					}
				}
			}
			elseif ($f['Art'] == self :: VORTEILUEBERNATUERLICH || $f['Art'] == self :: NACHTEILUEBERNATUERLICH)
			{
				if ($this->getVorteilspunkte(self :: VORTEILUEBERNATUERLICH) + $this->getVorteilspunkte(self :: NACHTEILUEBERNATUERLICH) + $Kosten <= 0)
				{
					if (abs($this->getVorteilspunkte($f['Art']) + $Kosten) <= $this->getMaximalVorteilspunkte($f['Art']))
					{
						return true;
					}
				}
			}

		}
		return false;
	}
	/**
	 * Berechnet die Anzahl der verf�gbaren Punkte f�r �bernat�rliche F�higkeiten.
	 * Sie berechnen sich nach 30+Anzahl Contage-aktivierte Fertigkeiten
	 * @return int die Anzahl der verbleibenden Punkte
	 */
	function getAktivierungspunkte()
	{
		$punkte = 0;
		foreach ($this->Fertigkeiten as $key => $f)
		{
			if ($f['Art'] == self :: VORTEILUEBERNATUERLICH)
			{
				if ($f['Stufe'] > 0)
				{
					$punkte += $f['Kosten1'];
				    // �nderung der Kosten direkt aus der Rassentabelle holen 
				    $query = sql_query('SELECT Kosten FROM T_Rassenfertigkeiten ' .
						'WHERE F_Fertigkeit_id='.$f['Fertigkeit_id'].' AND F_Rasse_id='.$this->F_Rasse_id);
				    if ( $ff = sql_fetch_array($query))
				    {
					$punkte += $ff['Kosten'];
				}
				sql_free_result($query);
				}
			}
		}
		return 30 - $punkte + $this->Contage;
	}
	/**
	 * Initialisiert die Felder f�r die Bestimmung der R�nge und Spezialisierungen
	 */
	function bestimmeSpezialisierungsraenge()
	{
		$Raenge = array ();
		$SpezRang = array ();
		$query = sql_query('SELECT * FROM T_Raenge');
		while ($rang = sql_fetch_array($query))
		{
			$Raenge[$rang['Rang_id']] = $rang;
			$Raenge[$rang['Rang_id']]['Anzahl'] = 0;
			if ($this->isMagisch())
			{
				$Raenge[$rang['Rang_id']]['Maximal'] = $rang['SpezMagisch'];
			} else
			{
				$Raenge[$rang['Rang_id']]['Maximal'] = $rang['SpezUnmagisch'];
			}
			$SpezRang[$rang['Rang_id']] = array ();
		}
		sql_free_result($query);

		// Einlesen der notwendigen Punkte f�r Adept/Meister/Gro�meister 
		$Punktanzahlen = array ();
		$Adeptenpunkte = array ();
		$RangErlaubt = array ();
		$query = sql_query('SELECT * FROM T_Spezialisierungsklassen INNER JOIN ' .
		'T_Spezialisierungen ON F_Klasse_id=Klasse_id');
		while ($klasse = sql_fetch_array($query))
		{
			$Adeptenpunkte[$klasse['Klasse_id']] = $klasse['Adeptenpunkte'];
			$Punktanzahlen[$klasse['Klasse_id']][$klasse['Spezialisierung_id']]['Meisterpunkte'] = $klasse['Meisterpunkte'];
			$Punktanzahlen[$klasse['Klasse_id']][$klasse['Spezialisierung_id']]['Grossmeisterpunkte'] = $klasse['Grossmeisterpunkte'];
			$Punktanzahlen[$klasse['Klasse_id']][$klasse['Spezialisierung_id']]['Spezialisierung'] = $klasse['Spezialisierung'];
			$RangErlaubt[$klasse['Spezialisierung_id']][1] = true;
			$RangErlaubt[$klasse['Spezialisierung_id']][2] = false;
			$RangErlaubt[$klasse['Spezialisierung_id']][3] = false;
			$RangErlaubt[$klasse['Spezialisierung_id']][4] = false;
		}
		sql_free_result($query);
		// Spezialisierungen bestimmen
		$Spezialisierungen = array ();
		$vorhandeneSpruchlisten = array ();
		$SpezPunkte = array ();
		$SpruchRang = array ();
		foreach ($this->Fertigkeiten as $fertigkeit_id => $fertigkeit)
		{
			// Vor- und Nachteile haben keine Spezialisierung_id
			if (isset ($fertigkeit['Spezialisierung_id']))
			{
				$spezialisierung_id = $fertigkeit['Spezialisierung_id'];
				if (!isset ($Spezialisierungen[$fertigkeit['F_Klasse_id']][$spezialisierung_id]))
					$Spezialisierungen[$fertigkeit['F_Klasse_id']][$spezialisierung_id] = 0;
				// Spruchlisten und allgemeine Dinge werden zusammengefasst
				// Spruchliste kann nur von der Charakterklasse stammen
				if ($fertigkeit['Spruchliste'])
				{
					$id = bestimmeAllgemeinSpezialisierung($this->F_Klasse_id);
					$SpruchRang[] = $spezialisierung_id;
					if (!in_array($spezialisierung_id, $vorhandeneSpruchlisten))
						$vorhandeneSpruchlisten[] = $spezialisierung_id;
				} else
				{
					$id = $spezialisierung_id;
				}
				if (!in_array($id, $SpezRang))
					$SpezRang[] = $id;
				if (!isset ($Spezialisierungen[$fertigkeit['F_Klasse_id']][$id]))
					$Spezialisierungen[$fertigkeit['F_Klasse_id']][$id] = 0;
				$Spezialisierungen[$fertigkeit['F_Klasse_id']][$id] += $fertigkeit['Kosten'];
				// Gesamtpunktzahl f�r die Spezialisierung
				if (!isset ($SpezPunkte[$fertigkeit['F_Klasse_id']][$id]))
					$SpezPunkte[$fertigkeit['F_Klasse_id']][$id] = 0;
				$SpezPunkte[$fertigkeit['F_Klasse_id']][$id] += $fertigkeit['Kosten'];
			}
		}
		for ($i = 1; $i < 5; $i++)
			if (isset ($SpezRang[$i]) && Count($SpezRang[$i]) >= $Raenge[$i]['Maximal'])
			{
				// ok, Pensum ausgesch�pft. Andere Wandererfertigkeiten abschalten
				foreach ($RangErlaubt as $key => $rang)
				{
					if (!in_array($key, $SpruchRang) && !in_array($key, $SpezRang))
						$RangErlaubt[$key][$i] = false;
				}
			}

		// Pr�fen, ob man Adept / Meister / Gro�meister geworden ist
		foreach ($SpezPunkte as $klasse => $spez)
		{
			$idallg = bestimmeAllgemeinSpezialisierung($klasse);
			foreach ($spez as $id => $Punkte)
			{
				// Allgemein-Punkte feststellen
				$allgemeinPunkte = 0;
				if ($idallg != $id && isset ($SpezPunkte[$klasse][$idallg]))
					$allgemeinPunkte = $SpezPunkte[$klasse][$idallg];
				$allgemeinPunkte += $Punkte;
				// Adept geworden?				
				if ($allgemeinPunkte >= $Adeptenpunkte[$klasse])
				{
					// Adept in dieser Spezialisierung!
					// Allgemein und Spruchlisten m�ssen erlaubt werden
					$RangErlaubt[$id][2] = true;
					$RangErlaubt[$idallg][2] = true;
					// Pr�fen, ob die Anzahl an Adeptenspezialisierungen �berschritten ist
					$Raenge[2]['Anzahl'] = 0;
					foreach ($RangErlaubt as $dieId => $rang)
						if ($rang[2])
							$Raenge[2]['Anzahl']++;
					// haben wir zuviel ?
					if ($Raenge[2]['Anzahl'] > $Raenge[2]['Maximal'])
					{
						$RangErlaubt[$id][2] = false;
						$RangErlaubt[$idallg][2] = false;
					} else
					{
						if ($idallg == $id && $klasse == $this->F_Klasse_id)
						{
							// wenn magisch und passende Klasse, Spruchlisten aktivieren
							foreach ($this->Spruchlisten as $key => $value)
							{
								$RangErlaubt[$key][2] = true;
							}
						}
					} // else
				} // wenn gen�gend Punkte
				// Man kann nur bei einer Spezialisierung Meister werden.
				if ($klasse != -1 && $id != -1 && $allgemeinPunkte >= $Punktanzahlen[$klasse][$id]['Meisterpunkte'] && $Punktanzahlen[$klasse][$id]['Meisterpunkte'] > 0)
				{
					// TODO Pr�fen, ob Meisterfertigkeit vorhanden ist!

					$RangErlaubt[$id][3] = true;
					// Pr�fen, ob die Anzahl an Adeptenspezialisierungen �berschritten ist
					$Raenge[3]['Anzahl'] = 0;
					foreach ($RangErlaubt as $dieId => $rang)
						if (isset ($rang[3]) && $rang[3])
							$Raenge[3]['Anzahl']++;
					// haben wir zuviel ?
					if ($Raenge[3]['Anzahl'] > $Raenge[3]['Maximal'])
						$RangErlaubt[$id][3] = false;
					else
					{
						if ($idallg == $id && $klasse == $this->F_Klasse_id)
						{
							// wenn magisch und passende Klasse, Spruchlisten aktivieren
							foreach ($this->Spruchlisten as $key => $value)
							{
								$RangErlaubt[$key][3] = true;
							}
						}
					}
				}
				// TODO: Gro�meister!

			}
		} // foreach Spezialisierungen
		$this->RangErlaubt = $RangErlaubt;
		$this->Spezialisierungen = $Spezialisierungen;
		$this->vorhandeneSpruchlisten = $vorhandeneSpruchlisten;
	}
	/**
	 * Erstellt ein Feld mit den m�glichen Fertigkeiten, die der Charakter in 
	 * der gew�hlten Spezialisierung w�hlen kann. Dabei werden die R�nge und die 
	 * Punkte ber�cksichtigt.
	 * @param int id der Spezialisierung deren Fertigkeiten angezeigt werden sollen
	 * @return array ein Fertigkeit_id-indiziertes Feld mit den Fertigkeiten  
	 */
	function bestimmeMoeglicheFertigkeiten($spezialisierung_id)
	{
		// Pr�fen ob Spezialisierung erlaubt und wenn ja in welchen R�ngen
		$erlaubt = array ();
		for ($i = 1; $i < 5; $i++)
		{
			if ($this->RangErlaubt[$spezialisierung_id][$i])
				$erlaubt[] = $i;
		}
		$Magisch = '';
		if ($this->isMagisch())
			$Magisch = 'OR Art="' . self :: MAGIE . '"';
		$sql = 'SELECT * FROM (T_Fertigkeiten INNER JOIN  (T_SpezialisierungenFertigkeiten' .
		' INNER JOIN (T_Spezialisierungen INNER JOIN T_Spezialisierungsklassen ON ' .
		'F_Klasse_id=Klasse_id) ON F_Spezialisierung_id=Spezialisierung_id) ' .
		' ON F_Fertigkeit_id=Fertigkeit_id) INNER JOIN T_Raenge ON Rang_id=F_Rang_id' .
		' WHERE (Art="' . self :: FERTIGKEIT . '" ' . $Magisch . ') AND Spezialisierung_id=' .
		$spezialisierung_id . ' AND Rang_id IN (' . implode(',', $erlaubt) . ')';
		$fertigkeiten = array ();
		if (Count($erlaubt) > 0)
		{
			if (!$query = sql_query($sql))
				echo 'Fehler:' . $sql . '/' . sql_error();
			while ($f = sql_fetch_array($query))
			{
				if ($this->fertigkeitMoeglich($f['Fertigkeit_id'], $f['Spezialisierung_id'], $f['Rang_id']))
				{
					$fertigkeiten[$f['Fertigkeit_id']] = $f;
				}
			}
			sql_free_result($query);
		}
		return $fertigkeiten;
	}
	/**
	 * Erstellt ein Feld mit den m�glichen Vor- und Nachteilen, die der Charakter 
	 * w�hlen kann. Dabei werden die verf�gbaren Punkte ber�cksichtigt.
	 * @param String die Art der Vor-/Nachteile (V, Uv, N, Un)
	 * @return array ein Fertigkeit_id-indiziertes Feld mit den Vor-/Nachteilen  
	 */
	function bestimmeMoeglicheVorteile($art)
	{
		if ($art != self :: VORTEIL && $art != self :: NACHTEIL && $art != self :: VORTEILUEBERNATUERLICH && $art != self :: NACHTEILUEBERNATUERLICH)
			return array ();
		$sql = 'SELECT * FROM T_Fertigkeiten WHERE Art="' . $art . '" ';
		if (Count($this->Verboten) > 0)
			$sql .= 'AND Fertigkeit_id NOT IN (' . implode(',', $this->Verboten) . ')';
		$sql .= ' ORDER BY Fertigkeit';
		$fertigkeiten = array ();
		if (!$query = sql_query($sql))
			die('Fehler:' . $sql . '/' . sql_error());
		while ($f = sql_fetch_array($query))
		{
			// 1. Fertigkeit nicht vorhanden			
			if ($this->vorteilMoeglich($f['Fertigkeit_id']))
			{
				$fertigkeiten[$f['Fertigkeit_id']] = $f;
			}
		}
		sql_free_result($query);
		return $fertigkeiten;
	} // bestimmeMoeglicheVorteile
	/**
	 * Berechnet die Anzahl der vergebenen Punkte in einer bestimmten Vor-/Nachteilsart.
	 * @param String die Art des Vor-/Nachteils (V, N, Uv, Un)
	 * @return int die Anzahl der vergebenen Punkte in der angegebenen Art.
	 */
	function getVorteilspunkte($art)
	{
		$kosten = 0;
		foreach ($this->Fertigkeiten as $key => $value)
		{
			if ($value['Art'] == $art)
			{
				$kosten += $value['Kosten1'];
				// �nderung der Kosten direkt aus der Rassentabelle holen 
				$query = sql_query('SELECT Kosten FROM T_RassenFertigkeiten ' .
						'WHERE F_Fertigkeit_id='.$value['Fertigkeit_id'].
' AND F_Rasse_id='.$this->F_Rasse_id);
				if ( $ff = sql_fetch_array($query))
				{
					$kosten += $ff['Kosten'];
				}
				else
				echo sql_error();
				sql_free_result($query);				
			}
		}
		return $kosten;
	} // getVorteilspunkte
	/**
	  * Bestimmt die vorhandenen Fertigkeiten einer bestimmten Spezialisierung
	  * @return array ein Feld von Fertigkeiten mit allen Informationen aus Spezialisierung, Rang etc. 
	  */
	function bestimmeVorhandeneFertigkeiten($spezialisierung_id)
	{
		$fertigkeiten = array ();
		foreach ($this->Fertigkeiten as $key => $f)
		{
			if ($f['Spezialisierung_id'] == $spezialisierung_id)
			{
				$fertigkeiten[$key] = $f;
			}
		}
		return $fertigkeiten;
	} // bestimmeVorhandeneFertigkeiten
	/**
	 * Berechnet, wie viele Spruchlisten der Charakter aufgrund seiner Klasse 
	 * haben darf. 
	 * @return int die Anzahl der erlaubten Spruchlisten 
	 */
	function getKlasseSpruchlistenAnzahl()
	{
		if (!$this->isMagisch())
			return 0;
		$ergebnis = 0;
		$query = sql_query('SELECT Spruchlistenanzahl FROM T_Spezialisierungsklassen ' .
		'WHERE Klasse_id=' . $this->F_Klasse_id);
		if ($Klasse = sql_fetch_array($query))
		{
			$ergebnis = $Klasse['Spruchlistenanzahl'];
		}
		sql_free_result($query);
		return $ergebnis;
	} // getKlasseSpruchlistenAnzahl
	/**
	 * Berechnet die Spezialisierungsklassen, die R�nge usw. 
	 * Diese Methode muss nach �nderungen an den Fertigkeiten aufgerufen werden,
	 * damit die erlaubten R�nge, Spezialisierungen usw. korrekt angegeben werden.
	 *  
	 */
	function bestimmeMoeglicheSpezialisierungsklassen()
	{
		$Spruchlisten = '';
		if ($this->isMagisch())
		{
			if (Count($this->vorhandeneSpruchlisten) < $this->getMaxSpruchlistenanzahl())
			{ // man hat noch nicht alle erlaubten Spruchlisten ausgereizt,
				// es werden alle Spruchlisten angezeigt.
				$Spruchlisten = implode(',', array_keys($this->Spruchlisten));
			} else
			{ // Es sind nur Spr�che der vorhandenen Spruchlisten m�glich
				$Spruchlisten = implode(',', $this->vorhandeneSpruchlisten);
			}
			// sicherheitshalber
			if ( $Spruchlisten == '') $Spruchlisten = '-1';
			$Auswahl = ' OR Art="' . self :: MAGIE . '") AND ( NOT T_Spezialisierungsklassen.Magisch OR (' .
			'F_Klasse_id=' . $this->F_Klasse_id . ' AND (Spezialisierung_id IN (' .
			$Spruchlisten . ') OR Allgemein)))';
		} else
		{
			$Auswahl = ') AND NOT T_Fertigkeiten.Magisch AND NOT Spruchliste ' .
			'AND NOT T_Spezialisierungsklassen.Magisch';
		}
		$sql = 'SELECT DISTINCT Spezialisierung_id, Spezialisierung FROM ' .
		'(T_Fertigkeiten INNER JOIN  (T_SpezialisierungenFertigkeiten' .
		' INNER JOIN (T_Spezialisierungen INNER JOIN T_Spezialisierungsklassen ON ' .
		'F_Klasse_id=Klasse_id) ON F_Spezialisierung_id=Spezialisierung_id)' .
		' ON F_Fertigkeit_id=Fertigkeit_id) WHERE (Art="' . self :: FERTIGKEIT . '" ' . $Auswahl;
		if ( !$query = sql_query($sql)) die('Fehler9:'.$sql.'/'.sql_error());
		$spezialisierungen = array ();
		while ($s = sql_fetch_array($query))
		{
			$spezialisierungen[$s['Spezialisierung_id']] = $s['Spezialisierung'];
		}
		sql_free_result($query);
		return $spezialisierungen;
	} // bestimmeMoeglicheSpezialisierungsklassen
	/**
	 * Berechnet die verf�gbaren Magiepunkte. Dazu werden die Kosten aller Spr�che 
	* der Spruchlisten zusammengerechnet, zuz�glich der gekauften Magiepunkte.
	 * Pro gekaufter Stufe der Magiepunkt-Fertigkeit gibt es zwei Magiepunkte. 
	* @return int die Anzahl der Magiepunkte 
	 */
	function getMagiepunkte()
	{
		$magiepunkte = 0;
		$spruchlistenids = array_keys($this->Spruchlisten);
		$allgemein = bestimmeAllgemeinSpezialisierung($this->F_Klasse_id);
		foreach ($this->Fertigkeiten as $f)
		{
			if (isset ($f['Spezialisierung_id']) && (in_array($f['Spezialisierung_id'], $spruchlistenids) || $f['Spezialisierung_id'] == $allgemein))
			{ // Wenn es sich um einen Zauber handelt, zu den Spruchkosten addieren
				if ($f['Art'] == self :: MAGIE)
				{
					$magiepunkte += $this->Magiepunkte[$f['F_Rang_id']];
				}
			}
		}
		if (isset ($this->Fertigkeiten[self :: MAGIEPUNKTE]))
		{
			$magiepunkte += $this->Fertigkeiten[self :: MAGIEPUNKTE]['Stufe'] * 2;
		}
		return $magiepunkte;
	} // getMagiepunkte
	function getMagischeBesonderheiten()
	{
		$Art = '';
		if ($this->F_Klasse_id == self :: SCHAMANE)
		{ // Schamane, Totem waehlen
			$Art = self :: TOTEM;
		}
		if ($this->F_Klasse_id == self :: ELEMENTARIST)
		{ // Elementarist, Element waehlen
			$Art = self :: ELEMENT;
		}
		$feld = array ();
		if ($query = sql_query('SELECT * FROM T_Fertigkeiten WHERE Art="' .
			$Art . '" ORDER BY Fertigkeit'))
		{
			while ($row = sql_fetch_array($query))
			{
				$feld[$row['Fertigkeit_id']] = $row['Fertigkeit'];
			}
			sql_free_result($query);
		}
		return $feld;
	}
	/**
	  * Bestimmt, wie viele Punkte f�r Vor- und Nachteile vergeben werden d�rfen
	  * Legt die Punktzahl gem�� des Atlantis-Regelwerks fest.
	  * @param String die Art der Vorteile (V, N, Uv, Un)
	  * @return int die Punktanzahl 
	  */
	function getMaximalVorteilsPunkte($art)
	{
		$MaxPunkte = 0;
		switch ($art)
		{
			case self :: VORTEIL : // Maximalpunkte f�r normale Vorteile
				$MaxPunkte = 50;
				break;
			case self :: NACHTEIL : // Maximalpunkte f�r normale Nachteile
				$MaxPunkte = 40;
				break;
			case self :: VORTEILUEBERNATUERLICH :
			case self :: NACHTEILUEBERNATUERLICH : // Maximalpunkte f�r Vor- und Nachteile �bernat�rlicher Art
				if (isset ($this->Fertigkeiten[self :: UEBERNATUERLICHLEICHT]))
					$MaxPunkte = 125;
				if (isset ($this->Fertigkeiten[self :: UEBERNATUERLICHGANZ]))
					$MaxPunkte = 250;
				break;
		}
		return $MaxPunkte;
	} // getMaximalVorteilsPunkte
	/**
	 * Wandelt die Klasse in ein indiziertes Feld um
	 * @param boolean true, wenn die Fertigkeiten ebenfalls �bergeben werden sollen
	 * @return array ein Feld mit den verschiedenen Daten des Charakters 
	 */
	function alsFeld($mitFertigkeiten = false)
	{

		$Character = array ();
		$Character['Name'] = $this->Charaktername;
		if ($this->isMagisch())
		{
			$Character['Art'] = 'Magischer Charakter';
			$Character['Art'] .= ' - ' . $this->getKlasse();
		} else
			$Character['Art'] = 'Unmagischer Charakter';

		$Character['Rasse'] = $this->getRasse();
		$Character['Spruchlisten'] = array ();
		foreach ($this->Spruchlisten as $Spruchliste)
		{
			$Character['Spruchlisten'][] = $Spruchliste['Spezialisierung'];
		}
		$x = $this->getSpezialisierung();
		if ($x != '')
		{
			$Character['Ausrichtung'] = $x;
		}
		if (Count($this->Verboten) > 0) // -1 ist immer drin
		{
			$Character['Verboten'] = $this->Verboten;
		}
		if ($mitFertigkeiten)
		{
			$Character['Vorteile'] = array ();
			$Character['UeVorteile'] = array ();
			$Character['Fertigkeiten'] = array ();
			foreach ($this->Fertigkeiten as $key => $value)
			{
				if ( $value['Beschreibung'] == '') $value['Beschreibung'] = '(keine Beschreibung vorhanden)';
				if ($value['Art'] == self :: VORTEIL || $value['Art'] == self :: NACHTEIL)
					$Character['Vorteile'][$key] = $value;
				elseif ($value['Art'] == self :: VORTEILUEBERNATUERLICH || $value['Art'] == self::NACHTEILUEBERNATUERLICH) 
				{
				$Character['UeVorteile'][$key] = $value;	
				}
				else
					$Character['Fertigkeiten'][$key] = $value;
			}
			if (Count($Character['Vorteile']) == 0)
				unset ($Character['Vorteile']);
			if (Count($Character['UeVorteile']) == 0)
				unset ($Character['UeVorteile']);
			if (Count($Character['Fertigkeiten']) == 0)
				unset ($Character['Fertigkeiten']);
		}
		return $Character;
	} // alsFeld		
} //Klasse Charakter
/**
 * Bestimmt den Namen einer Spezialisierung nach einer gegebenen ID
 * @param int die ID der gesuchten Spezialisierung
 * @return string der Name der Spezialisierung oder eine leere Zeichenkette im Fehlerfall
 */
function holeSpezialisierungsNamen($Spezialisierung_id)
{
	if (!is_numeric($Spezialisierung_id))
		return '';
	$ergebnis = '';
	$query = sql_query('SELECT Spezialisierung FROM T_Spezialisierungen ' .
	'WHERE Spezialisierung_id=' . $Spezialisierung_id);
	if ($spezialisierung = sql_fetch_array($query))
		$ergebnis = $spezialisierung['Spezialisierung'];
	sql_free_result($query);
	return $ergebnis;
}
/**
 * Bestimmt die "allgemein"-Spezialisierung einer gegebenen Klasse
* @param int die ID der Klasse, deren allgemein-Spezialisierung bestimmt werden soll
* @return die ID der allgemein-Spezialisierung, NULL im Fehlerfall
 */
function bestimmeAllgemeinSpezialisierung($klasse)
{
	if (!is_numeric($klasse))
		return '';
	$ergebnis = NULL;
	$query = sql_query('SELECT Spezialisierung_id FROM T_Spezialisierungen ' .
	'WHERE Allgemein AND F_Klasse_id=' . $klasse);
	if ($spezialisierung = sql_fetch_array($query))
		$ergebnis = $spezialisierung['Spezialisierung_id'];
	sql_free_result($query);
	return $ergebnis;
}
?>