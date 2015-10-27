<?php
require_once(INC_PATH.'cvjm/DBEntity.class.php');
/**
 * Beschreibt einen Artikel aus der Datenbank
 *
 */
class Artikel extends DBEntity
{
	/**
	 *  Artikel-Datensatz aus der Datenbank
	 */
	function __construct($artikel_id = -1)
	{
		parent::__construct(TABLE_ARTIKEL);
		if ( ! is_numeric($artikel_id))
		{
			throw new Exception('Ungültige Artikel-id: '.$artikel_id.'!');
		}
		if ( $artikel_id > 0 )
		{
			// TODO: id in Artikel_id ändern
			$sql = 'SELECT * FROM '.TABLE_ARTIKEL.' WHERE id='.$artikel_id;
			$query = mysql_query($sql);
			if ( ! $Artikel = mysql_fetch_array($query))
			{
				throw new Exception('Konnte Artikel '.$artikel_id.' nicht laden!');
			}
			mysql_free_result($query);
			$this->uebertrageFelder($Artikel);
			// TODO: umstellung von id auf Artikel_id für konsistenze Bezeichnungen
			$this->Artikel_id = $this->id;
		}
		else
		{
			// Vorbelegungen
			$this->ArtikelStand = time();
			$this->Artikel_id = -1;
			$this->Schlafplatz = -1;
			$this->LetztePruefung = time();
		}
		$this->OriginalF_Art_id = $this->F_Art_id;
		$this->OriginalPosition = $this->Position;
	}
	function uebertrageFelder($felder)
	{
		parent::uebertrageFelder($felder);
		// Besonderheit: Checkboxen berücksichtigen
		foreach ( array('Anzeigen','Rabattfaehig','Geringwertig', 'Steuerpflicht', 'PersonJN') as $Feld)
		{
			if ( ! isset($felder[$Feld]))
			{
				$this->$Feld = false;
			}
		}
	}
	/**
	 * gibt an ob der Artikel neu ist oder bereits in der Datenbank gespeichert wurde
	 * @return boolean true, wenn der Artikel noch nicht in der Datenbank steht, false sonst
	 */
	function isNeu()
	{
		return !is_numeric($this->Artikel_id) || $this->Artikel_id <= 0;
	}
	/**
	 * Löscht den Artikel
	 */
	function loeschen()
	{
		if ( ! $this->isNeu())
		{
			// TODO: id->Artikel_id
			sql_query('DELETE FROM ' . TABLE_ARTIKEL. ' WHERE id=' . $this->Artikel_id);
			$this->Artikel_id = '';
		}
	}
	/**
	 * setzt einen Artikel an eine neue Position bezogen auf die Art des aktuellen Artikelobjekts
	 * @param int $Artikel_Nr die ID des Artikels
	 * @param int $Position die neue Position des Artikels
	 */
	private function AendereArtikelPosition($Artikel_Nr, $Position)
	{
		if ( ! is_numeric($Position) || ! is_numeric($Artikel_Nr)) return;
		// Positionänderung
		if ( ! $query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE Position='.
		$Position.' AND F_Art_id='.$this->F_Art_id.	' AND id<>'.$Artikel_Nr) )
		{
			throw new Exception('AendereArtikelPosition:'.mysql_error());
		}
		if ( sql_num_rows($query) > 0 )
		{
			// Position schon belegt
			sql_query('UPDATE '.TABLE_ARTIKEL.' SET Position=Position+1 WHERE F_Art_id='.
			$this->F_Art_id.' AND Position >= '.$Position);
		}
		sql_free_result($query);
		sql_query('UPDATE '.TABLE_ARTIKEL.' SET F_Art_id='.$this->F_Art_id.',Position='.
		$Position.' WHERE id='.$Artikel_Nr);
	}
	/**
	 * Speichert den Artikel
	 * @param boolean verschieben true, wenn alle Unterartikel mit verschoben werden sollen, false sonst
	 */
	function save($verschieben = false)
	{
		// Numerische Überprüfung
		foreach ( array('Anzeigen', 'Geringwertig', 'Rabattfaehig', 'Einruecken',
		'Position','Steuerpflicht', 'F_MWSt', 'F_Lieferant_id', 'Einkaufspreis',
		'F_Art_id','PersonJN', 'F_PruefungArt', 'LetztePruefung') as $feld)
		{
			if ( ! is_numeric($this->$feld) )
			{
				$this->$feld = 0;
			}
		}
		if  (! is_numeric($this->Schlafplatz))
		{
			$this->Schlafplatz = -1;
		}
		if ( !$this->isNeu())
		{
			$sql = "UPDATE ".TABLE_ARTIKEL.
			' SET Einheit="'.mysql_real_escape_string($this->Einheit).'", '.
			'Bezeichnung="'.mysql_real_escape_string($this->Bezeichnung).'",'.
			'Anzeigen='.$this->Anzeigen.',';
			$sql .= 'Geringwertig='.$this->Geringwertig.',';
			$sql .= 'Rabattfaehig='.$this->Rabattfaehig.',';
			$sql .= 'Einruecken='.$this->Einruecken.',';
			$sql .= 'F_MWSt='.$this->F_MWSt.',';
			$sql .= 'Beschreibung="'.mysql_real_escape_string($this->Beschreibung).'",';
			$sql .= 'F_Lieferant_id='.$this->F_Lieferant_id.',';
			$sql .= 'Barcode="'.mysql_real_escape_string($this->Barcode).'",';
			$sql .= 'Steuerpflicht='.$this->Steuerpflicht.',';
			$sql .= 'Bestellnummer="'.mysql_real_escape_string($this->Bestellnummer).'",';
			$sql .= 'Einkaufspreis='.$this->Einkaufspreis.',';
			$sql .= 'PersonJN='.$this->PersonJN.',';
			$sql .= 'Schlafplatz='.$this->Schlafplatz.',';
			$sql .= 'Aktion="'.mysql_real_escape_string($this->Aktion).'",';
			$sql .= 'LetztePruefung='.$this->LetztePruefung.',';
			$sql .= 'F_PruefungArt='.$this->F_PruefungArt;
			$sql .= ' WHERE id='.$this->Artikel_id;
			if ( ! sql_query($sql))
			{
				throw new Exception('Artikelupdate: '.mysql_error());
			}
		}
		else
		{
			// Neue Position suchen
			if ( $this->Position == 0)	 // Position für den Artikel freimachen
			{
				$query = sql_query('SELECT MAX(Position) FROM '.TABLE_ARTIKEL.' WHERE F_Art_id='.$this->F_Art_id);
				$Pos = sql_fetch_row($query);
				sql_free_result($query);
				$this->Position = $Pos[0];
			}
			$sql = "INSERT INTO ".TABLE_ARTIKEL;
			$sql .= ' (Bezeichnung,Einheit,F_Art_id, Einruecken,Rabattfaehig,Steuerpflicht,';
			$sql .= 'Geringwertig,Einkaufspreis,Bestellnummer,Barcode,Beschreibung,F_MWSt,';
			$sql .= 'F_Lieferant_id,Anzeigen,Schlafplatz,PersonJN,Aktion,LetztePruefung,';
			$sql .= 'F_PruefungArt) VALUES ("';
			$sql .= mysql_real_escape_string($this->Bezeichnung).'",';
			$sql .= '"'.mysql_real_escape_string($this->Einheit).'",';
			$sql .= $this->F_Art_id.',';
			$sql .= $this->Einruecken.',';
			$sql .= $this->Rabattfaehig.',';
			$sql .= $this->Steuerpflicht.',';
			$sql .= $this->Geringwertig.',';
			$sql .= $this->Einkaufspreis.',';
			$sql .= '"'.mysql_real_escape_string($this->Bestellnummer).'",';
			$sql .= '"'.mysql_real_escape_string($this->Barcode).'",';
			$sql .= '"'.mysql_real_escape_string($this->Beschreibung).'",';
			$sql .= $this->F_MWSt.',';
			$sql .= $this->F_Lieferant_id.',';
			$sql .= $this->Anzeigen.',';
			$sql .= $this->Schlafplatz.',';
			$sql .= $this->PersonJN.',';
			$sql .= '"'.mysql_real_escape_string($this->Aktion).'",';
			$sql .= $this->LetztePruefung.',';
			$sql .= $this->F_PruefungArt;
			$sql .= ')';
			if ( ! mysql_query($sql))
			{
				throw new Exception ('Artikel einfügen: '.$sql.'/'.mysql_error());
			}
			$this->Artikel_id = sql_insert_id();
			$this->id = $this->Artikel_id;
		}
		if ($this->OriginalF_Art_id != $this->F_Art_id || $this->OriginalPosition != $this->Position)
		{
			if ( $verschieben )
			{
				$pos = $this->findeArtikelPos('r', true);
				// Alle Unterartikel kopieren
				$query = sql_query('SELECT id, Position FROM '.TABLE_ARTIKEL.
				' WHERE F_Art_id='.$this->OriginalF_Art_id.' AND Position >= '.
				$this->OriginalPosition.' AND Position < '.$pos.' ORDER BY Position');
				while ( $Art2 = sql_fetch_array($query) )
				{
					$NPos = $Art2['Position'] - $this->OriginalPosition;
					$this->AendereArtikelPosition($Art2['id'],$this->Position+$NPos);
				}
				sql_free_result($query);
			}
			else
			{
				$this->AendereArtikelPosition($this->Artikel_id, $this->Position);
			}

		}
		$this->OriginalPosition = $this->Position;
		$this->OriginalF_Art_id = $this->F_Art_id;
	}
	/**
	 * Liste mit den möglichen Prüfungsarten
	 *
	 */
	function PruefungsArten()
	{
		$arten = array();
		$query = sql_query('SELECT * FROM cvjm_PruefungArten ORDER BY Bezeichnung');
		while ( $row = sql_fetch_array($query) )
		{
			$arten[$row['PruefungArt_id']] = $row['Bezeichnung']; 
		}
		sql_free_result($query);
		return $arten;
	}
	function getFeld($feld)
	{
		if (isset($this->$feld))
		{
			return $this->$feld;
		}
		else
		{
			switch ( $feld )
			{
				case 'MWST':
					return $this->getMWST();
					break;
				case 'Art':
					return $this->getArtikelart();
					break;
				default:
					return '';
			}
		}
	}
	function erstelleKopie()
	{
		$Originalid = $this->Artikel_id;
		$this->Bezeichnung = 'Kopie von '.$this->Bezeichnung;
		$this->Artikel_id = -1;
		$this->save();
		if (! sql_query('INSERT INTO '.TABLE_ARTIKELBEZIEHUNGEN.' (F_Artikel_id,F_Unterartikel_id,Menge,Beginn,Dauer) '.
		'SELECT '.$this->Artikel_id.',F_Unterartikel_id, Menge,Beginn,Dauer FROM '.TABLE_ARTIKELBEZIEHUNGEN.
		' WHERE F_Artikel_id='.$Originalid))
		{
			throw new Exception('Fehler bei Artikelkopie 1: '.mysql_error());
		}
		if ( ! sql_query('INSERT INTO '.TABLE_ARTIKELGRUPPEN.' (F_Artikel_id,F_Unterartikel_id,Menge,Beginn,Dauer) '.
		'SELECT '.$this->Artikel_id.',F_Unterartikel_id,Menge,Beginn,Dauer FROM '.TABLE_ARTIKELGRUPPEN.
		' WHERE F_Artikel_id='.$Originalid))
		{
			throw new Exception ('Fehler bei Artikelkopie 2: '.mysql_error());
		}
	}
	/**
	 * bestimmt die Anzahl der Buchungen, in denen der Artikel verwendet wird
	 * @return int die Anzahl der Buchungen
	 */
	function getBuchungsAnzahl()
	{
		$query = sql_query('SELECT Count(*) FROM '.TABLE_BUCHUNGSEINTRAEGE.
		' WHERE F_Artikel_Nr='.$this->Artikel_id);
		$anz = sql_fetch_row($query);
		sql_free_result($query);
		return $anz[0];
	}
	/**
	 * bestimmt die Anzahl der Artikel, denen der aktuelle Artikel zugeordnet ist
	 * @return int die Anzahl der Oberartikel
	 */
	function getOberArtikelAnzahl()
	{
		$query = sql_query('SELECT Count(*) FROM '.TABLE_ARTIKELBEZIEHUNGEN.
		' WHERE F_Unterartikel_id='.$this->Artikel_id);
		$anz = sql_fetch_row($query);
		sql_free_result($query);
		return $anz[0];
	}
	/**
	 * gibt den Pfad der Artikel über mehrere Ebenen zurück. Ist Ebene = 0, so wird nur der
	 * Artikelname selbst zurückgegeben, sonst eine Pfadangabe der Form X > Y > Z
	 * @param int $Ebenen die Anzahl der zurückgegebenen Ebenen
	 * @return array Feld (Bezeichnung) mit dem Artikelpfad
	 */
	function getArtikelPfad($Ebenen = 0)
	{
		$erg = array();
		if ( $Ebenen > 0 )
		{
			$parent = $this->getArtikelParent();
			if ( $parent != null )
			{
				$erg = array_merge($parent->getArtikelPfad($Ebenen-1));
			}
		}
		$erg[] = $this->Bezeichnung;
		return $erg;
	}
	function getArtikelPfadString($Ebenen = 0)
	{
		return implode(' > ', $this->getArtikelPfad($Ebenen));
	}
	/**
	 * liefert den nächsthöheren Artikel einer Art bezogen auf die Position
	 * @return Artikel das Artikelobjekt oder NULL wenn keiner vorhanden
	 */
	function getArtikelParent()
	{
		$artikel = null;
		$sql = 'SELECT MAX(Position) FROM '.TABLE_ARTIKEL;
		$sql .= ' WHERE Position<'.$this->Position.' AND Einruecken<'.$this->Einruecken.
		' AND F_Art_id='.$this->F_Art_id;
		if ( ! $query = sql_query($sql))
		{
			throw new Exception('getArtikelParent: '.$sql.':'.mysql_error());
		}
		if ( $zeile = sql_fetch_row($query))
		{
			$zeile = $zeile[0];
		}
		sql_free_result($query);
		if ( is_numeric($zeile)) {
			if ( ! $query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.
			' WHERE Position='.$zeile.' AND F_Art_id='.$this->F_Art_id) )
			{
				throw new Exception('HAP2: '.mysql_error());
			}
			if ( $art = sql_fetch_row($query)) {
				$artikel = new Artikel($art[0]);
			}
			sql_free_result($query);
		}
		return $artikel;
	}
	function getArtikelart()
	{
		$a = self::getArtikelArten();
		return $a[$this->F_Art_id];
	}
	/**
	 *
	 * stellt ein Feld mit den vorhandenen Mehrwertsteuern zur Verfügung
	 * @return array assoziatives Feld mit ID -> Beschreibung
	 */
	function getMWSTListe()
	{
		if ( ! isset($this->MWST))
		{
			$q = sql_query('SELECT * FROM '.TABLE_MWST.' ORDER BY MWSTBemerkung');
			$this->MWST = array();
			while ($mwst = sql_fetch_array($q))
			{
				$this->MWST[$mwst['MWST_id']] = $mwst['MWSTBemerkung'];
			}
			sql_free_result($q);
		}
		return $this->MWST;
	}
	/**
	 * ergibt die Mehrwertsteuer des Artikels in Prozent
	 * @return double die Mehrwertsteuer
	 */
	function getMWST()
	{
		$q = sql_query('SELECT MWST FROM '.TABLE_MWST.' WHERE MWST_id='.$this->F_MWSt);
		$st = 0;
		if ($mwst = sql_fetch_row($q))
		{
			$st = $mwst[0];
		}
		sql_free_result($q);
		return $st;
	}
	/**
	 * entfernt den Preis des Artikels von der angegebenen Preisliste
	 */
	function entfernePreis($Preisliste_id)
	{
		if ( is_numeric($Preisliste_id))
		{
			sql_query('DELETE FROM '.TABLE_PREISE.' WHERE F_Preisliste_id='.$Preisliste_id.
			' AND Artikel_Nr='.$this->Artikel_id);
			unset($this->Preise);
		}
	}
	/**
	 * liefert ein Feld von Preisen aus den verschiedenen Preislisten für den Artikel
	 * Das Feld ist nach der Gültigkeit der Preislisten sortiert.
	 * @return array Feld mit Preis und Preislisteneintrag
	 */
	function getPreise()
	{
		if ( !isset($this->Preise))
		{
			$query = sql_query('SELECT * FROM '.TABLE_PREISLISTEN.' INNER JOIN '.TABLE_PREISE.
			' ON Preisliste_id = F_Preisliste_id WHERE Artikel_Nr = '.$this->Artikel_id.
			' ORDER BY GueltigAb DESC');
			$this->Preise = array();
			while ( $preis = sql_fetch_array($query) )
			{
				$this->Preise[$preis['Preisliste_id']] = $preis;
			}
			mysql_free_result($query);
		}
		return $this->Preise;
	}
	/**
	 * liefert den Preis des Artikels auf einer bestimmten Preisliste. Je nach zweitem Parameter ist das
	 * der Einzelpreis oder der Preis pro Stunde.
	 * @param int $Preisliste_id die ID der Preisliste
	 * @param boolean $Stunde true, wenn der Stundenpreis verlangt ist, false sonst
	 * @return double der gewünschte Preis des Artikels
	 */
	function holePreis($Preisliste_id, $Stunde = false)
	{
		if ( ! is_numeric($Preisliste_id) )
		{
			return 0;
		}
		if ( ! $query = sql_query('SELECT Preis, PreisStunde FROM '.TABLE_PREISE.
		' WHERE F_Preisliste_id = '.$Preisliste_id.' AND Artikel_Nr = '.$this->Artikel_id) )
		{
			throw new Exception('holePreis: '.mysql_error());
		}
		if ( ! $preis = sql_fetch_row($query) )
		{
			// parent prüfen
			$parent= $this->getArtikelParent();
			if ( $parent != NULL )
			{
				$erg = $parent->holePreis($Preisliste_id, $Stunde);
			}
			else
			{
				$erg = 0;
			}
		}
		else
		{
			if ( $Stunde )
			{
				$erg =$preis[1];
			}
			else
			{
				$erg =$preis[0];
			}
		}
		if ( ! is_numeric($erg) )
		{
			$erg = 0;
		}
		sql_free_result($query);
		return $erg;
	}
	/**
	 * Bewegt einen Artikel im Baum.
	 * @param char $richtung die Richtung (l, r, u, o)
	 * @param boolean $einzeln true, wenn nur der Artikel verändert werden soll, false wenn alle Unterartikel mitbewegt werden sollen
	 */
	function bewegen($richtung, $einzeln = true)
	{
		if ( $einzeln )
		{
			switch ( $richtung )
			{
				case 'l': sql_query('UPDATE '.TABLE_ARTIKEL.
				' SET Einruecken=Einruecken-1 WHERE id='.$this->Artikel_id.
				' AND Einruecken > 0');
				$this->Einruecken= Max(0, $this->Einruecken-1);
				break;
				case 'r': sql_query('UPDATE '.TABLE_ARTIKEL.
				' SET Einruecken=Einruecken+1 WHERE id='.$this->Artikel_id);
				$this->Einruecken=$this->Einruecken+1;
				break;
				case 'u':
					sql_query('UPDATE '.TABLE_ARTIKEL.
					' SET Position=Position+1 WHERE Position='.($this->Position-1).
					' AND F_Art_id='.$this->F_Art_id);
					sql_query('UPDATE '.TABLE_ARTIKEL.
					' SET Position=Position-1 WHERE id='.$this->Artikel_id.
					' AND Position > 0');
					$this->Position=Max(0, $this->Position-1);
					break;
				case 'd':
					sql_query('UPDATE '.TABLE_ARTIKEL.
					' SET Position=Position-1 WHERE Position='.($this->Position+1).
					' AND F_Art_id='.$this->F_Art_id);
					sql_query('UPDATE '.TABLE_ARTIKEL.
					' SET Position=Position+1 WHERE id='.$this->Artikel_id);
					$this->Position= $this->Position+1;
					break;
			}
		}
		else
		{
			// Ganzen Ast verschieben
			$next = $this->findeArtikelPos('r');
			switch ( $richtung )
			{
				case 'l':
					sql_query('UPDATE '.TABLE_ARTIKEL.
					' SET Einruecken=Einruecken-1 WHERE Position BETWEEN '.$this->Position.
					' AND '.($next-1).' AND Einruecken > 0 AND F_Art_id='.$this->F_Art_id);
					break;
				case 'r': sql_query('UPDATE '.TABLE_ARTIKEL.
				' SET Einruecken=Einruecken+1 WHERE Position BETWEEN '.$this->Position.
				' AND '.($next-1).' AND F_Art_id='.$this->F_Art_id);
				break;
				case 'u':
					$anz = $next-$this->Position;
					$pos = $this->findeArtikelPos('h');
					if ( $anz > 0 )
					{
						if ( ! sql_query('UPDATE '.TABLE_ARTIKEL.
						' SET Position=-(Position+'.$anz.') WHERE Position BETWEEN '.
						$pos.' AND '.($this->Position-1).
						" AND F_Art_id=".$this->F_Art_id))
						{
							throw new Exception('bewegen: '.mysql_error());
						}
						$anz = $pos-$this->Position; // negativer Wert
						//echo "Anz: $anz";
						sql_query("UPDATE ".TABLE_ARTIKEL.
						" SET Position=Position+$anz WHERE Position BETWEEN ".$this->Position.
						" AND ".($next-1)." AND F_Art_id=".$this->F_Art_id);
						sql_query("UPDATE ".TABLE_ARTIKEL." SET Position=-Position WHERE Position < 0");
					}
					break;
				case 'd':
					$pos = $this->findeArtikelPos("ru");
					$anz = $next-$this->Position;
					if ( $anz > 0 )
					{
						sql_query("UPDATE ".TABLE_ARTIKEL.
						" SET Position=-(Position-$anz) WHERE Position BETWEEN ".
						$next." AND ".($pos-1)." AND F_Art_id=".$this->F_Art_id);
						$anz = $pos-$next;
						sql_query("UPDATE ".TABLE_ARTIKEL.
						" SET Position=Position+$anz WHERE Position BETWEEN ".$this->Position.
						" AND ".($next-1)." AND F_Art_id=".$this->F_Art_id);
						sql_query("UPDATE ".TABLE_ARTIKEL." SET Position=-Position WHERE Position < 0");
					}
					break;
			} // switch
		} // else
	}
	/**
	 * Sucht Artikelnachfolger in Bezug auf die Position eines andere Artikels.
	 * Es geht also um die Positionierung im Artikelbaum. Die Einrücktiefe bleibt
	 * dabei gleich.
	 * Es wird der Artikel gefunden, der vor bzw. nach dem Artikel liegt.
	 * @param char $Richtung h (hoch), r (runter) oder egal: Nachfolger des Nachfolgers (zweimal r)
	 * @param boolean $Original true, wenn OriginalPosition verwendet werden soll, false wenn Position genommen wird
	 * @return int die Position des gesuchten Artikels
	 */
	function findeArtikelPos($Richtung, $Original= false)
	{
		// Sucht
		if ( $Original )
		{
			$Position = $this->OriginalPosition;
		}
		else
		{
			$Position = $this->Position;
		}
		if ( $Richtung == 'h' )
		{
			// der Vorgänger
			$query = sql_query('SELECT MAX(Position) FROM '.TABLE_ARTIKEL.
			' WHERE Einruecken<='.$this->Einruecken.' AND Position < '.$Position.
			' AND F_Art_id='.$this->F_Art_id);
			if ( ! $Pos = sql_fetch_row($query) )
			{
				$Pos[0] = $Position-1;
			}
			if ( ! is_numeric($Pos[0]) ) $Pos[0] = 0;
		}
		else if ( $Richtung == 'r' )
		{ // der nachfolgende
			$sql = 'SELECT MIN(Position) FROM '.TABLE_ARTIKEL.
			' WHERE Einruecken<='.$this->Einruecken.' AND Position > '.$Position.
			' AND F_Art_id='.$this->F_Art_id;
			if ( ! $query = sql_query($sql))
			{
				throw new Exception('findeArtikelpos: '.mysql_error());
			}
			if ( ! $Pos = sql_fetch_row($query) )
			{
				$Pos[0] = $Position+1;
			}
			if ( ! is_numeric($Pos[0]) )
			{
				sql_free_result($query);
				$query = sql_query('SELECT MAX(Position) FROM '.TABLE_ARTIKEL.
				' WHERE F_Art_id='.$this->F_Art_id);
				$Pos = sql_fetch_row($query);
				$Pos[0] = $Pos[0]+1;
			}
		}
		else
		{ // der Nachfolger des Nachfolgers
			$sql = 'SELECT MIN(Position) FROM '.TABLE_ARTIKEL.
			' WHERE F_Art_id='.$this->F_Art_id.' AND Einruecken<='.
			$this->Einruecken.' AND Position > '.
			$this->findeArtikelPos('r', $Original);
			if ( ! $query = sql_query($sql))
			{
				throw new Exception ('FAP '.$sql.': '.mysql_error());
			}
			if ( ! $Pos = sql_fetch_row($query) )
			{
				$Pos[0] = $Artikel['Position']+1;
			}
			if ( ! is_numeric($Pos[0]) ) {
				sql_free_result($query);
				$query = sql_query('SELECT MAX(Position) FROM '.TABLE_ARTIKEL.
				' WHERE F_Art_id='.$this->F_Art_id);
				$Pos = sql_fetch_row($query);
				$Pos[0] = $Pos[0]+1;
			}
		}
		sql_free_result($query);
		return $Pos[0];
	}
	/**
	 * berechnet die Schlafplätze an diesem Ort, ohne die Unterorte zur berücksichtigen.
	 * Sonderfall:
	 * -1 wenn es keine Schlafplätze gibt
	 * 0 wenn es beliebig viele Schlafplätze gibt (Zeltplatz)
	 * @return int die Schlafplätze am aktuelle Artikel, -1 wenn dort keine Schlafplätze vorliegen, 0 beim Zeltplätz
	 */
	function BerechneEinzelnSchlafplaetze()
	{		
		return $this->Schlafplatz;
	}
	/**
	 * Berechnet die Schlafplätze eines Ortes. Dabei werden eventuell zugehörige
	 * Unterartikel berücksichtigt. Im Ergebnis bekommt man 0 oder mehr Schlafplätze. Sofern es gar keine
	 * Schlafplätze gibt, wird -1 zurückgegeben.
	 * @return int die Gesamt-Schlafplätze am Ort, oder -1 wenn es keine gibt
	 */
	function BerechneSchlafplaetze()
	{
		$Plaetze = $this->BerechneEinzelnSchlafplaetze();
		// Bestimmt die Position des nächsten Artikels
		if ( ! $query = sql_query('SELECT Position FROM '.TABLE_ARTIKEL.
		' WHERE Position>'.$this->Position.' AND Einruecken<='.$this->Einruecken.
		' AND F_Art_id='.$this->F_Art_id.' ORDER BY Position LIMIT 1'))
		{
			throw new Exception('berechneSchlafplaetze: '.mysql_error());
		}
		if ( ! $artikel2 = sql_fetch_array($query))
		{
			$artikel2['Position'] = 999999999;
		}
		sql_free_result($query);
		$query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.
		' WHERE Position>'.$this->Position.
		' AND F_Art_id='.$this->F_Art_id.' AND Position<'.$artikel2['Position']) ;
		while ( $Ort = sql_fetch_row($query))
		{
			$a = new Artikel($Ort[0]);
			$p = $a->BerechneEinzelnSchlafplaetze();
			if ( $p >= 0 )
			{
				// Artikel ohne Schlafplätze ignorieren
				if ( $Plaetze < 0 ) $Plaetze = 0;
				$Plaetze += $p;
			}
		}
		sql_free_result($query);
		return $Plaetze;
	}
	/**
	 * zeigt an ob der Artikel ein Programmpaket ist
	 * @return boolean true, wenn es sich um ein Programmpaket handelt, false sonst
	 */
	function isProgrammpaket()
	{
		return $this->F_Art_id == CVJMART_PROGRAMMPAKET;
	}
	/**
	 * gibt an, ob die Abrechnung des Artikels nach Personen erfolgt
	 * @return boolean true wenn die Abrechnung nach Personen erfolgt
	 */
	function isAbrechnungNachPersonen()
	{
		$t = $this->getAbrechnungstyp();
		if ( in_array($t, array(ABRECHNUNGSTYP_KOPF,ABRECHNUNGSTYP_KOPFNACHT,ABRECHNUNGSTYP_VERPFLEGUNG)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	 * gibt an, ob die Abrechnung des Artikels nach Übernachtungen erfolgt
	 * @return boolean true, wenn die Abrechnung nach Übernachtungen erfolgt
	 */
	function isAbrechnungNachUebernachtungen()
	{
		$t = $this->getAbrechnungstyp();
		if ( in_array($t, array(ABRECHNUNGSTYP_NACHT,ABRECHNUNGSTYP_VERPFLEGUNGNACHT)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	 * bestimmt den Abrechnungstyp des Artikels
	 * @return int der Abrechnungstyp
	 */
	function getAbrechnungstyp()
	{
		if ( ! isset($this->Abrechnungstyp))
		{
			$query= mysql_query('SELECT Typ FROM '.TABLE_ARTIKELARTEN.' WHERE Art_id='.$this->F_Art_id);
			$art = mysql_fetch_array($query);
			mysql_free_result($query);
			$this->Abrechnungstyp = $art[0];
		}
		return $this->Abrechnungstyp;
	}
	/**
	 * baut ein Feld mit Artikel auf, das die Artikel der Art des aktuellen Artikels im Form eines Baumes
	 * abbilden. Der aktuelle Artikel wird in der Mitte des Feldes positioniert.
	 * @param int/String $Anzahl die Anzahl der Artikel in der Baumliste oder 'Alle' für alle.
	 * @return array ein Feld mit den Artikeln
	 */
	function baueArtikelBaumAuf($Anzahl = 10)
	{
		$sql = 'SELECT id FROM '.TABLE_ARTIKEL.' WHERE F_Art_id='.$this->F_Art_id;
		if ( $Anzahl != 'Alle' && is_numeric($Anzahl))
		{
			$sql .= ' AND Position BETWEEN '.
			($this->Position-$Anzahl).' AND '.($this->Position+$Anzahl);
		}
		$sql .= ' ORDER BY Position';
		if ( ! $query = sql_query($sql))
		{
			throw new Exception('baueArtikelBaumAuf: '.$sql.'/'.mysql_error());
		}
		$Artikelliste = array();
		while ( $baumartikel = sql_fetch_row($query) )
		{
			$Artikelliste[] = new Artikel($baumartikel[0]);
		}
		sql_free_result($query);
		return $Artikelliste;
	}
	function getArtikelBildIMG()
	{
		$ext = array('.JPG','.jpg','.gif','.GIF','.png', '.PNG');
		foreach ( $ext as $extender)
		if ( file_exists($_SERVER['DOCUMENT_ROOT'].'/Artikel/'.$this->Artikel_id.'/Bild '.$this->Barcode.$extender))
		{
			return '<img src="/Artikel/'.$this->Artikel_id.'/Bild '.$this->Barcode.$extender.
			'" alt="Artikel '.$this->Artikel_id.'" height="100px" width="100px" title="'.$this->Bezeichnung.'" />';
		}
		return '';
	}
	// ---------------------------------------------------------------
	/**
	 * Nummeriert die Positionen der Artikel im Artikelbaum neu durch, so dass jeder Artikel eine
	 * eindeutige Nummer erhält.
	 */
	static function neuNummerieren()
	{
		$query = sql_query('SELECT * FROM '.TABLE_ARTIKEL.' WHERE F_Art_id='.$this->F_Art_id.' ORDER BY Position, Einruecken');
		$Position = 1;
		while ( $art = sql_fetch_array($query) )
		{
			sql_query('UPDATE '.TABLE_ARTIKEL." SET Position=$Position WHERE id=".$art["id"]);
			$Position++;
		}
		sql_free_result($query);

	}
	/**
	 * liefert ein assoziatives Feld mit den Artikelarten
	 * @return array Feld mit ID->Artikelart
	 */
	static function getArtikelArten()
	{
		$Artikelarten = array();

		if ( ! $query = sql_query('SELECT * FROM '.TABLE_ARTIKELARTEN.' ORDER BY Art'))
		{
			throw new Exception('getArtikelArten: '.mysql_error());
		}
		while ( $art = sql_fetch_array($query) )
		{
			$Artikelarten[$art['Art_id']] = $art['Art'];
		}
		sql_free_result($query);
		return $Artikelarten;
	}
	/**
	 * liefert eine Liste aller Artikel mit der angegeben Bezeichnung. Ist
	 * eine Artikel-id angegeben, wird diese nicht in die Liste aufgenommen.
	 * @param String $Bezeichnung die gesuchte Bezeichnung
	 * @param int $NichtArtikel_id die ID, die nicht aufgenommen werden soll, -1 wenn alle angegeben werden sollen
	 * @return array ein Feld mit Artikelobjekten
	 */
	static function getArtikelListe($Bezeichnung, $NichtArtikel_id = -1)
	{
		$Artikel = array();
		if ( is_numeric($NichtArtikel_id))
		{
			// TODO: id->Artikel_id
			$query = sql_query("SELECT id FROM ".TABLE_ARTIKEL." WHERE Bezeichnung='".
			mysql_real_escape_string($Bezeichnung)."' AND id<>".$NichtArtikel_id);
			while ( $a = mysql_fetch_row($query) )
			{
				$Artikel[] = new Artikel($a[0]);
			}
		}
		return $Artikel;
	}
	/**
	 * liefert eine Liste aller Speiseräume (erkennbar am Namen "Saal". Das Feld ist nach
	 * Namen sortiert.
	 * @return array Feld mit Artikelobjekten
	 */
	static function getSpeiseraeume()
	{
		$raeume = array();
		$sql = "SELECT DISTINCT id FROM " . TABLE_ARTIKEL .
		" WHERE F_Art_id=" . CVJMART_ORT . " AND Bezeichnung LIKE '%Saal %' ORDER BY Bezeichnung";
		$q = sql_query($sql);
		while ( $saal = mysql_fetch_row($q))
		{
			$raeume[] = new Artikel($saal[0]);
		}
		mysql_free_result($q);
		return $raeume;
	}
	/**
	 * liefert ein Feld mit den Artikeln, deren Bezeichnung zum gegebenen Suchstring passt
	 * @param String $bezeichnung die Bezeichnung des gesuchten Artikels
	 * @return array ein Feld von Artikeln
	 */
	static function getArtikelSuche($bezeichnung='')
	{
		$query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE Bezeichnung LIKE "%'.
		mysql_real_escape_string($bezeichnung).'%" ORDER BY Bezeichnung');
		$ergebnis = array();
		while ($artikel = mysql_fetch_row($query))
		{
			$ergebnis[] = new Artikel($artikel[0]);
		}
		mysql_free_result($query);
		return $ergebnis;
	}
	/**
	 * liefert ein Feld mit den Artikel, die in der Kategorie Orte eingestuft sind. Die Orte werden nach 
	 * ihrer Position aufgelistet
	 * @return array ein Feld von Artikeln
	 */
	static function getOrte()
	{
		$query = sql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE F_Art_id='.CVJMART_ORT.' ORDER BY Position');
		$ergebnis = array();
		while ($artikel = mysql_fetch_row($query))
		{
			$ergebnis[] = new Artikel($artikel[0]);
		}
		mysql_free_result($query);
		return $ergebnis;
	}
	/**
	 * Sucht einen Artikel nach dem angegebenen Text. Gesucht wird in den Feldern Bezeichnung und Barcode. 
	 * Der Barcode muss genau übereinstimmen, die Bezeichnung wird mit Wildcards druchsucht. 
	 * @return array ein Feld der passenden Artikel 
	 */
	static function search($text)
	{
		$text = mysql_real_escape_string(trim($text));
		$query = mysql_query('SELECT id FROM '.TABLE_ARTIKEL.' WHERE Bezeichnung LIKE "%'.$text.
		'%" OR Barcode="'.$text.'"');
		$Feld = array();
		while ($artikel = mysql_fetch_array($query))
		{
			$Feld[] = new Artikel($artikel['id']);
		}
		mysql_free_result($query);
		return $Feld;
	}
	static function findNextCVJMEAN()
	{
		$query = mysql_query('SELECT Max(Barcode) FROM '.TABLE_ARTIKEL.' WHERE Barcode LIKE "CVJM%"');
		if ( ! $row = mysql_fetch_row($query))
		{
			$nr = 'CVJM00000001';
		}
		else
		{
			$nr = substr($row[0],4);
			if ( ! is_numeric($nr))
			{
				throw new Exception('Vorhandene EAN '.$row[0].' ist ungültig.');
			}
			$nr = $nr + 1;
			$nr = 'CVJM'.sprintf('%08d',$nr);
		}
		mysql_free_result($query);
		return $nr;
	}
} // Klasse Artikel ?>
