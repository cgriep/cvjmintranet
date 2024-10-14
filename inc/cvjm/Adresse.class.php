<?php
require_once(INC_PATH.'cvjm/DBEntity.class.php');
require_once(INC_PATH.'cvjm/Buchung.class.php');
/**
 * Beschreibt eine Adresse aus der Datenbank
 *
 */
class Adresse extends DBEntity
{
	// die Anreden aus der Datenbank
	private static $Anredeartenfeld = array();

	// Datensatz aus der Datenbank
	function __construct($adressen_id = -1)
	{
		parent::__construct(TABLE_ADRESSEN);
		if ( ! is_numeric($adressen_id))
		{
			throw new Exception('Ungültige Adressen-id: '.$adressen_id.'!');
		}
		if ( $adressen_id > 0 )
		{
			$sql = 'SELECT * FROM '.TABLE_ADRESSEN.' WHERE Adressen_id='.$adressen_id;
			$query = sql_query($sql);
			if ( ! $Adresse = sql_fetch_array($query))
			{
				throw new Exception('Konnte Adresse '.$adressen_id.' nicht laden!');
			}
			$this->uebertrageFelder($Adresse);
		}
		else
		{
			// Vorbelegungen
			$this->Stand = time();
			$this->Adressen_id = -1;
			$this->Kunden_Nr = -1;
			$this->Geburtsdatum = 0;
			$this->Land = 'D';
			$this->F_Anrede_id = 0;
		}
		sql_free_result($query);
	}
	/**
	 * gibt an ob die Adresse neu ist oder bereits in der Datenbank gespeichert wurde 
	 * @return boolean true, wenn die Adresse noch nicht in der Datenbank steht, false sonst 
	 */
	function isNeu()
	{
		return ! is_numeric($this->Adressen_id) || $this->Adressen_id <= 0;
	}
	/**
	 * Löscht die Adresse
	 */
	function loeschen()
	{
		if ( ! $this->isNeu() )
		{
			sql_query('UPDATE ' . TABLE_ADRESSEN . ' SET Geloescht=1 WHERE Adressen_id=' . $this->Adressen_id);
			$this->Geloescht = 1;
			$this->logAction('Adresse als gelöscht markiert.');
		}
	}
	/**
	 * Setzt die angegebenen Kategorien für die Adresse.
	 * Sollte add false sein, so werden alle vorhandenen vorher gelöscht, ansonsten die Kategorie hinzugefügt.
	 * @param array Kategorien die Kategorie_id der gewünschten Kategorien
	 * @param boolean add false, wenn alle Kategorien im Array sind (vorhandene Kategorien werden gelöscht), true sonst (Kategorien werden hinzugefügt)
	 */
	function setAdressKategorien($Kategorien, $add = false)
	{
		if ( ! $add)
		{
			sql_query('DELETE FROM '.TABLE_ADRESSEN_KATEGORIE.' WHERE F_Adressen_id='.$this->Adressen_id);
			$this->logAction('Kategorien gelöscht.');
		}
		foreach ($Kategorien as $value)
		{
			sql_query('INSERT INTO ' .TABLE_ADRESSEN_KATEGORIE . ' (F_Kategorie_id,F_Adressen_id) VALUES (' .
			$value . ',' . $this->Adressen_id . ')');
			$this->logAction('Kategorie Nr. '.$value.' gesetzt.');
		}
	}
	/**
	 * setzt oder löscht eine einzelne Kategorie
	 * @param int $Kategorie_id die ID der Kategorie
	 * @param boolean $ein true, wenn die Kategorie gesetzt werden soll, false sonst
	 */
	function setKategorie($Kategorie_id, $ein = false)
	{
		if ( $this->isNeu() || ! is_numeric($Kategorie_id))
		{
			throw new Exception('setKategorie: Es wurde ein ungültige Adressen-id übergeben!');
		}
		if ( !$ein )
		{
			sql_query('DELETE FROM ' .	TABLE_ADRESSEN_KATEGORIE .
			' WHERE F_Adressen_id = ' . $this->Adressen_id.' AND F_Kategorie_id='.
			$Kategorie_id);
			$this->logAction('Kategorie Nr. '.$Kategorie_id.' gelöscht.');
		}
		else
		{
			sql_query('INSERT INTO ' .	TABLE_ADRESSEN_KATEGORIE .
			' (F_Kategorie_id,F_Adressen_id) VALUES (' .
			$Kategorie_id . ',' . $this->Adressen_id. ')');
			$this->logAction('Kategorie Nr. '.$Kategorie_id.' gesetzt.');
		}
		unset($this->Kategorien[true]);
	}
	/**
	 * Neuen Datensatz als Kopie erstellen
	 * @param String $Kat O, wenn Original Ansprechpartner von Kopie, U wenn umgekehrt
	 */
	function erstelleKopie($Kat = '')
	{
		if ( $this->isNeu() )
		{
			return false;
		}
		$Originalid = $this->Adressen_id;
		$this->Name = 'Kopie von ' . $this->Name;
		$this->Geburtsdatum = 0;
		$this->Kunden_Nr = -1;
		$this->Adressen_id = -1;
		$this->save();
		$this->logAction('Als Kopie von Adressen_id '.$Originalid.' erstellt.');
		// Kategorien und Institutionen übernehmen
		if ( $Kat == 'O' )
		{
			sql_query('INSERT INTO ' . TABLE_ADRESSEN_INSTITUTIONEN. ' (F_Adressen_id,F_UAdressen_id) VALUES ('.
			$this->Adressen_id.','.$Originalid.')');
		}
		if ( $Kat == 'U')
		{
			sql_query('INSERT INTO ' . TABLE_ADRESSEN_INSTITUTIONEN. ' (F_Adressen_id,F_UAdressen_id) VALUES ('.
			$Originalid.','.$this->Adressen_id.')');
		}
		// Kategorien kopieren
		sql_query('INSERT INTO ' . TABLE_ADRESSEN_KATEGORIE . ' (F_Kategorie_id,F_Adressen_id) '.
		'SELECT F_Kategorie_id, '.$this->Adressen_id.' FROM '.TABLE_ADRESSEN_KATEGORIE.' WHERE F_Adressen_id = ' . $Originalid);
		
	}
	/**
	 * Hole die Kategorien dieser Adresse. Ist alle wahr, so werden alle vorhandenen
	 * Kategorien mit der Anzahl und einem Kennzeichen ob eingeteilt oder nicht Zurückgegeben
	 * Im letzten Fall sind zusätzlich die Felder Da und Anz vorhanden.
	 * @param boolean alle true, wenn alle Kategorien geholt werden soll, false wenn nur die vorhandenen Zurückgegeben werden sollen
	 * @return array ein Feld der Kategorien dieser Adresse
	 */
	function getKategorien($alle = false)
	{
		if ( ! isset($this->Kategorien[$alle]))
		{
			if ( $alle )
			{
				$sql = 'SELECT Kategorie_id, Kategorie, Count(F_Kategorie_id) AS Anz, '.
				'Sum(F_Adressen_id='.$this->Adressen_id.') AS Da FROM '.
				TABLE_KATEGORIEN.' LEFT JOIN '.TABLE_ADRESSEN_KATEGORIE.
				' ON Kategorie_id=F_Kategorie_id GROUP BY Kategorie, Kategorie_id ORDER BY Kategorie';
			}
			else
			{
				$sql = 'SELECT Kategorie_id, Kategorie FROM ' . TABLE_KATEGORIEN . ' INNER JOIN ' .
				TABLE_ADRESSEN_KATEGORIE . ' ON F_Kategorie_id = Kategorie_id WHERE F_Adressen_id =' .
				$this->Adressen_id . ' ORDER BY Kategorie';
			}
			if ( ! $query = sql_query($sql))
			{
				throw new Exception('Fehler beim Laden der Kategorien: '.sql_error());
			}
			// Kategorien listen
			$this->Kategorien[$alle]= array ();
			while ($adr = sql_fetch_array($query))
			{
				$this->Kategorien[$alle][$adr['Kategorie_id']] = $adr;
			}
			sql_free_result($query);
		}
		return $this->Kategorien[$alle];
	}
	/**
	 * Speichert die Adresse
	 */
	function save()
	{
		// Stammdaten speichern oder hinzufügen
		
		if (trim($this->Ort) == '' && is_numeric($this->PLZ))
		{
			$this->Ort = holeOrt($this->PLZ);
		}
		if (trim($this->Ort) != '' && !is_numeric($this->PLZ))
		{
			$this->PLZ = holePLZ($this->Ort);
		}
		if ($this->ImmerPerEMail && trim($this->Email) != '')
		{
			$this->ImmerPerEMail = 1;
		}
		else
		{
			$this->ImmerPerEMail = 0;
		}
		foreach (array('Geburtsdatum','F_Anrede_id','Kunden_Nr','Rabattsatz') as $feld)
		{
			if (!is_numeric($this->$feld))
			{
				$this->$feld= 0;
			}
		}
		
		if ($this->isNeu() )
		{
			// Neue Adresse
			$this->logAction("Neu erstellt.", false);
			$sql = 'INSERT INTO ' . TABLE_ADRESSEN . ' (Name,Vorname,Strasse,PLZ, Land, Ort,' .
			'Telefon1,Telefon2,Fax,Email,Bemerkungen,Zusatz,F_Anrede_id,Geburtsdatum,Kunden_Nr,' .
			'Rabattsatz,Titel,History,ImmerPerEMail) VALUES ("' .
			sql_real_escape_string($this->Name) . '","'.
			sql_real_escape_string($this->Vorname) . '","' .
			sql_real_escape_string($this->Strasse) . '","' .
			sql_real_escape_string($this->PLZ) . '","' .
			sql_real_escape_string($this->Land) . '","' .
			sql_real_escape_string($this->Ort) . '","' .
			sql_real_escape_string($this->Telefon1) . '","' .
			sql_real_escape_string($this->Telefon2) . '","' .
			sql_real_escape_string($this->Fax) . '","' .
			sql_real_escape_string($this->Email) . '","' .
			sql_real_escape_string(trim($this->Bemerkungen)) . '","' .
			sql_real_escape_string(trim($this->Zusatz)) . '",' .
			$this->F_Anrede_id . ',' . $this->Geburtsdatum. ',' .
			$this->Kunden_Nr . ',' .
			$this->Rabattsatz . ',"' .
			sql_real_escape_string($this->Titel) . '","' .
			sql_real_escape_string($this->History).'",'.
			$this->ImmerPerEMail . ')';
		} else
		{
			// Update einer Adresse
			$this->logAction("Daten gespeichert.", false);
			$sql = 'UPDATE ' . TABLE_ADRESSEN . ' SET Name="' . sql_real_escape_string($this->Name) .
			'",Vorname="' . sql_real_escape_string($this->Vorname) .
			'",F_Anrede_id=' . $this->F_Anrede_id .
			',Bemerkungen="' . sql_real_escape_string(trim($this->Bemerkungen)) .
			'",Strasse="' .	sql_real_escape_string($this->Strasse) .
			'",PLZ="' .	sql_real_escape_string($this->PLZ) .
			'",Land="' . sql_real_escape_string($this->Land) .
			'",Ort="' .	sql_real_escape_string($this->Ort) .
			'",Telefon1="' . sql_real_escape_string($this->Telefon1) .
			'",Telefon2="' .sql_real_escape_string($this->Telefon2) .
			'",Email="' .	sql_real_escape_string($this->Email) .
			'", Fax="' . sql_real_escape_string($this->Fax) .
			'",Geburtsdatum='.$this->Geburtsdatum.
			',Kunden_Nr=' .	$this->Kunden_Nr .
			',Zusatz="' .sql_real_escape_string(trim($this->Zusatz)) .
			'", Rabattsatz=' . $this->Rabattsatz .
			', Titel="' . $this->Titel . '"' .
			', History="'.$this->History.'"'.
			', ImmerPerEMail='.$this->ImmerPerEMail .
			' WHERE Adressen_id=' . $this->Adressen_id;
		}
		if (!sql_query($sql))
		{
			throw new Exception('Fehler beim Adressspeichern: '.$sql.':' . sql_error());
		}
		if ($this->isNeu() )
		{
			$this->Adressen_id = sql_insert_id();
		}
		$this->Stand = time();
	} // save
	/*
	 if (isset ($docinput['Edit']) && $docinput['Edit'] >= 5 && $docinput['Edit'] <= 7)
	 {
	 // Alle Versandmarker löschen
	 if (!isset ($docinput['Kategorie']))
	 {
	 switch ($docinput['Edit'])
	 {
	 case 5 : // alle löschen
	 if (!sql_query('UPDATE ' . TABLE_ADRESSEN .
	 ' SET Versandmarker=0,Stand=Stand'))
	 echo sql_error();
	 break;
	 case 6 : // Institutionen löschen
	 if (!sql_query('UPDATE ' . TABLE_ADRESSEN . ',' . TABLE_ADRESSEN_INSTITUTIONEN .
	 ' SET Versandmarker=0,Stand=Stand ' .
	 'WHERE F_Adressen_id=Adressen_id'))
	 echo sql_error();
	 break;
	 case 7 : // Ansprechpartner löschen
	 if (!sql_query('UPDATE ' . TABLE_ADRESSEN . ',' . TABLE_ADRESSEN_INSTITUTIONEN .
	 ' SET Versandmarker=0,Stand=Stand ' .
	 'WHERE F_UAdressen_id=Adressen_id'))
	 echo sql_error();
	 break;
	 } // Switch
	 }
	 elseif (is_numeric($docinput['Kategorie'])) if (!sql_query('UPDATE ' .
	 TABLE_ADRESSEN . ',' . TABLE_ADRESSEN_KATEGORIE .
	 ' SET Versandmarker=1,Stand=Stand ' .
	 'WHERE Adressen_id=F_Adressen_id AND F_Kategorie_id=' .
	 $docinput['Kategorie']))
	 echo sql_error();
	 $docinput['Edit'] = 4;
	 unset ($docinput['Kategorie']); // löschen damit nicht nur die Kategorie angezeigt wird
	 }
	 */
	/**
	 * Berechnet die Anzahl der Ansprechpartner
	 * @return int die Anzahl der Ansprechpartner
	 * TODO: Entfernen
	 */
	function getAnsprechpartnerAnzahl()
	{
		$ansprechpartner = sql_query('SELECT Count(*) FROM ' . TABLE_ADRESSEN_INSTITUTIONEN . ' WHERE ' .
		' F_Adressen_id = ' . $this->Adressen_id);
		$anzahl = sql_fetch_row($ansprechpartner);
		sql_free_result($ansprechpartner);
		return $anzahl[0];
	}
	/**
	 * Berechnet die Anzahl der Institutionen
	 * @return int die Anzahl der Institutionen
	 * TODO: Entfernen
	 */
	function getInstitutionenAnzahl()
	{
		$ansprechpartner = sql_query('SELECT Count(*) FROM ' . TABLE_ADRESSEN_INSTITUTIONEN . ' WHERE ' .
		' F_UAdressen_id = ' . $this->Adressen_id);
		$anzahl = sql_fetch_row($ansprechpartner);
		sql_free_result($ansprechpartner);
		return $anzahl[0];
	}
	/**
	 *
	 */
	function isZugehoerig($Adressen_id)
	{
		$ergebnis = false;
		$qu = sql_query('SELECT * FROM ' . TABLE_ADRESSEN_INSTITUTIONEN .
		' WHERE F_UAdressen_id=' . $Adressen_id. ' AND F_Adressen_id=' .
		$this->Adressen_id);
		if (sql_num_rows($qu) == 1)
		{
			$ergebnis = true;
		}
		sql_free_result($qu);
		return $ergebnis;
	}
	/**
	 *
	 */
	function isAnsprechpartner($Adressen_id)
	{
		$ergebnis = false;
		$qu = sql_query('SELECT * FROM ' . TABLE_ADRESSEN_INSTITUTIONEN .
		' WHERE F_Adressen_id=' . $Adressen_id . ' AND F_UAdressen_id=' .
		$this->Adressen_id);
		if (sql_num_rows($qu) == 1)
		{
			$ergebnis = true;
		}
		sql_free_result($qu);
		return $ergebnis;
	}
	/**
	 * zeigt an ob eine Adresse zur angegeben Kategorie gehört
	 * @param int $Kategorie_id
	 * @return boolean true, wenn die Adresse zur Kategorie gehört, false sonst 
	 */
	function hasKategorie($Kategorie_id)
	{
		$Kategorien = $this->getKategorien();
		if ( isset($Kategorien[$Kategorie_id]))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	 * Anrede in lesbarer Form
	 */
	function Anrede()
	{
		return self::getAnredearten($this->F_Anrede_id);
	}
	function Anredezeile()
	{
		$value = $this->Anrede();
       	if ( $value == 'Frau' || $value == 'Liebe' )
          {
            $Anrede = 'Sehr geehrte Frau '.$this->Name;
          }
          elseif ( $value == 'Herr' || $value == 'Lieber' )
          {
            $Anrede = 'Sehr geehrter Herr '.$this->Name;
          }
          else
          {
            $Anrede = 'Sehr geehrte Damen und Herren';
          }
        return $Anrede.',';        
	}
	/**
	 * berechent das Alter in Jahren der Person
	 * @return int das Alter, 0 wenn ein Fehler aufgetreten ist
	 */
	function Alter()
	{
		if ( $this->Geburtsdatum != 0)
		{
		return floor((time()-$this->Geburtsdatum)/31536000);
		}
		else
		{
			return 0;
		}
	}
	/**
	 * ergibt die Ansprechpartner der Adresse
	 * @return array ein Feld mit den Adressen der Ansprechpartner
	 */
	function getAnsprechpartner()
	{
		if ( ! isset($this->Ansprechpartnerfeld))
		{
			$query = sql_query('SELECT F_UAdressen_id FROM '.TABLE_ADRESSEN_INSTITUTIONEN .
			' WHERE F_Adressen_id = ' .	$this->Adressen_id);
			$this->Ansprechpartnerfeld = array ();
			while ($adr = sql_fetch_row($query))
			{
				//$adr['Vorlagen'] = ListeVorlagen($adr['Adressen_id'], $Adresse['Adressen_id'], $Vorlagen);
				$this->Ansprechpartnerfeld[] = new Adresse($adr[0]);
			}
			sql_free_result($query);
		}
		return $this->Ansprechpartnerfeld;
	}
	/**
	 * ergibt die Institutionen der Adresse
	 * @return array ein Feld mit den Adressen der Institutionen
	 */
	function getInstitutionen()
	{
		if ( ! isset($this->Institutionenfeld))
		{
			$query = sql_query('SELECT F_Adressen_id FROM '.TABLE_ADRESSEN_INSTITUTIONEN .

		 ' WHERE F_UAdressen_id = ' .$this->Adressen_id);
		 $this->Institutionen = array ();
			while ($adr = sql_fetch_row($query))
			{
				//$adr['Vorlagen'] = ListeVorlagen($Adresse['Adressen_id'], $adr['Adressen_id'], $Vorlagen);
				$this->Institutionenfeld[] = new Adresse($adr[0]);
			}
			sql_free_result($query);
		}
		return $this->Institutionenfeld;
	}
	/**
	 * ergibt die Bemerkungen, die zu der Adresse gehören
	 * @return array ein Feld mit den Bemerkungen
	 */
	function getBemerkungen()
	{
		if ( ! isset($this->Bemerkungenfeld))
		{
			$query = sql_query('SELECT * FROM ' . TABLE_ADRESSENBEMERKUNGEN . ' WHERE F_Adressen_id =' .
			$this->Adressen_id . ' ORDER BY Datum DESC');
			$this->Bemerkungenfeld = array ();
			while ($adr = sql_fetch_array($query))
			{
				if ($adr['Anhang'] != '')
				{
					$ext = pathinfo($Bemerkung['Anhang']);
					$adr['Anhanglink'] = $this->getAdressenAnhangLink($adr['Bemerkung_id'], $ext['extension'], 'Bemerkung');
				}
				$this->Bemerkungenfeld[] = $adr;
			}
			sql_free_result($query);
		}
		return $this->Bemerkungenfeld;
	}
	/**
	 * Bestimmt die absolute Adresse einer Datei, die der Adresse zugeordnet ist.
	 * @param int $Bemerkung_id ID der Bemerkung
	 * @param String $ext die Endung, die die Datei hat
	 * @param String $Art Art des Anhangs
	 * @param String $Zusatz ein Zusatz zum Dateinamen, der am Ende der Datei angehngt wird
	 * @return String der Dateiname bezogen auf das Hauptverzeichnis des Servers
	 */
	function getAdressenAnhangLink($Bemerkung_id, $ext=CVJM_ENDUNG, $Art='Bemerkung',$Zusatz='')
	{
		if ( $Zusatz != '')
		{
			$Zusatz = '-'.$Zusatz;
		}
		if ( substr($ext,0,1) != '.') {
			$ext = '.'.$ext;
		}
		if ( is_numeric($Bemerkung_id) && $Bemerkung_id < 0 )
		{
			return CVJM_ADRESSEN_DIR.'/'.$this->Adressen_id.'/'.$Art.$ext;
		}
		else
		{
			return CVJM_ADRESSEN_DIR.'/'.$this->Adressen_id.'/'.$Art.'-'.$Bemerkung_id.$Zusatz.$ext;
		}
	}
	/**
	 *  ergibt ein Feld mit den Buchungen der Adresse
	 * @return array ein Feld mit den Buchungen der Adresse (Buchungsobjekte)
	 */
	function getBuchungen()
	{
		if ( ! isset($this->Buchungen) || Count($this->Buchungen) == 0)
		{

			if (! $query = sql_query('SELECT Buchung_Nr FROM ' . TABLE_BUCHUNGEN . ' WHERE F_Adressen_id= ' .
			$this->Adressen_id . ' ORDER BY Von DESC'))
			{
				throw new Exception('getBuchungen: Konnte Buchungen nicht laden.'.sql_error());
			}
			$this->Buchungen = array ();
			while ($Buchung = sql_fetch_row($query))
			{
				$this->Buchungen[] = new Buchung($Buchung[0]);
			}
			sql_free_result($query);
			// falls Adressat Seminarteilnehmer ist, auch anzeigen
			$sql = 'SELECT Buchung_Nr FROM ' . TABLE_BUCHUNGEN . ' INNER JOIN '.
			TABLE_SEMINARTEILNEHMER.' ON F_Buchung_Nr=Buchung_Nr WHERE '.
			TABLE_SEMINARTEILNEHMER.'.F_Adressen_id= ' .
			$this->Adressen_id . ' ORDER BY Von DESC';
			$query = sql_query($sql);
			while ($Buchung = sql_fetch_row($query))
			{
				$this->Buchungen[] = new Buchung($Buchung[0]);
			}
			sql_free_result($query);
		}
		return $this->Buchungen;
	}
	/**
	 * Artikel der Adresse anzeigen
	 * @return array ein Feld der Artikel der Adresse
	 */
	function getArtikel()
	{
		if ( ! isset($this->Artikelfeld))
		{
			$Artikel = array ();
			if (!$query = sql_query('SELECT * FROM ' . TABLE_ARTIKEL . ' WHERE F_Lieferant_id=' .$this->Adressen_id))
			{
				throw new Exception('Fehler beim Laden der Artikel: '.sql_error());
			}
			$this->Artikelfeld = array();
			if (sql_num_rows($query) > 0)
			{
				$Artikelarten = holeArtikelarten();
				while ($artikel = sql_fetch_array($query))
				{
					$artikel['Artikelart'] = $Artikelarten[$artikel['F_Art_id']];
					$this->Artikelfeld[] = $artikel;
				}
			}
			sql_free_result($query);
		}
		return $this->Artikelfeld;
	}
	/**
	 * Erstellt eine neue Kundennummer sofern noch nicht vorhanden.
	 * Wenn eine neue Nummer vergeben wird, wird der Datensatz gespeichert
	 * @return int die Kundennummer
	 */
	function getKundenNummer()
	{
		if ( $this->Kunden_Nr <= 0)
		{
			$kunr = sql_query('SELECT MAX(Kunden_Nr) FROM '.TABLE_ADRESSEN);
			$Kundennummer = sql_fetch_row($kunr);
			sql_free_result($kunr);
			$this->Kunden_Nr = $Kundennummer[0]+1;
			$this->logAction('Neue Kundennummer vergeben: '.$this->Kunden_Nr,false);
			$this->save();
			
		}
		return $this->Kunden_Nr;
	}
	/**
	 * setzt oder entfernt den Versandmarker auf einer Adresse, ohne den Stand zu verändern
	 * @param boolean $ein true, wenn der Marker gesetzt werden soll, false sonst
	 */
	function setVersandmarker($ein = true)
	{
		if ( $this->isNeu() )
		{
			throw new Exception('setVersandmarker: Es wurde ein ungültige Adressen-id übergeben!');
		}
		if ( $ein )
		{
			$wert = 1;
		}
		else
		{
			$wert = 0;
		}
		sql_query('UPDATE ' .TABLE_ADRESSEN . ' SET Versandmarker='.$wert.',Stand=Stand ' .
		' WHERE Adressen_id='.$this->Adressen_id);
	}
	/**
	 * setzt oder entfernt eine Institution zu einer Adresse
	 * @param int $institution_id die ID der Institution
	 * @param boolean $ein true, wenn die Institution hinzugefügt werden soll, false sonst
	 */
	function setInstitution($institution_id, $ein = true)
	{
		if ( $this->isNeu() || ! is_numeric($institution_id))
		{
			throw new Exception('setInstitution: Es wurde eine ungültige Adressen-id übergeben!');
		}
		if ( $ein )
		{
			sql_query('INSERT INTO ' .TABLE_ADRESSEN_INSTITUTIONEN .
			' (F_UAdressen_id,F_Adressen_id) VALUES (' .
			$this->Adressen_id . ',' . $institution_id . ')');
			$this->logAction('Institution Nr. '.$institution_id.' gesetzt.');
		}
		else
		{
			sql_query('DELETE FROM ' .	TABLE_ADRESSEN_INSTITUTIONEN . ' WHERE ' .
			'F_UAdressen_id = ' . $this->Adressen_id . ' AND F_Adressen_id = ' .
			$institution_id);
			$this->logAction('Institution Nr. '.$institution_id.' entfernt.');
		}
		unset($this->Institutionenfeld);
		
	}
	/**
	 * setzt oder entfernt den Ansprechpartner zu einer Adresse
	 * @param int $ansprechpartner_id die ID des Ansprechpartners
	 * @param boolean $ein true, wenn der Ansprechpartner hinzugefügt werden soll, false sonst
	 */
	function setAnsprechpartner($ansprechpartner_id, $ein = true)
	{
		if ( $this->isNeu() || ! is_numeric($ansprechpartner_id))
		{
			throw new Exception('setAnsprechpartner: Es wurde ein ungültige Adressen-id übergeben!');
		}
		if ( $ein )
		{
			sql_query('INSERT INTO ' .	TABLE_ADRESSEN_INSTITUTIONEN .
			' (F_Adressen_id,F_UAdressen_id) VALUES (' .
			$this->Adressen_id. ',' . $ansprechpartner_id . ')');
			$this->logAction('Ansprechpartner Nr. '.$ansprechpartner_id.' gesetzt.');
		}
		else
		{
			sql_query('DELETE FROM ' .	TABLE_ADRESSEN_INSTITUTIONEN . ' WHERE ' .
			'F_Adressen_id = ' . $this->Adressen_id.
			' AND F_UAdressen_id = ' . $ansprechpartner_id);
			$this->logAction('Ansprechpartner Nr. '.$ansprechpartner_id.' gesetzt.');
		}
		unset($this->Ansprechpartnerfeld);
	}
	/**
	 * Ergibt den kompletten Namen der Form Name, Vorname.
	 * Wenn kein Vorname vorhanden ist, wird nur der Name angegeben
	 * @return String der Name und Vorname
	 */
	function Vollname()
	{
		if ( $this->Vorname != '')
		{
			return $this->Name.', '.$this->Vorname;
		}
		else
		{
			return $this->Name;
		}
	}
	/**
	 * gibt Strasse, PLZ, Ort und Telefon Übersichtlich Zurück.
	 * @param boolean $html true, wenn HTML-Zeilenumbrüche verwendet werden sollen, false sonst
	 * @return String die Felder in drei Zeilen
	 */
	function Uebersicht($html = false)
	{
		$s = $this->Strasse."\n".$this->PLZ.' '.$this->Ort."\n".$this->Telefon1;
		if ( $html )
		{
			return nl2br($s);
		}
		else
		{
			return $s;
		}
	}
	/**
	 * Hilfsfunktion, gibt den Inhalt des jeweiligen Feldes Zurück
	 * @param String $feld der Name des Feldes
	 * @return String der Inhalt des Feldes, leer wenn Fehler
	 */
	function getFeld($feld)
	{
		$feld = trim($feld);
		if ( isset($this->$feld))
		{
			return $this->$feld;
		}
		else
		{
			switch ( $feld )
			{
				case 'Vollname':
					return $this->Vollname();
					break;
				case 'Anrede':
					return $this->Anrede();
					break;
			}
			return '';
		}
	}
	/**
	 * ergibt eine Zeichenkette mit HTML-Markup für die Vorlagen
	 * @param int $fuerID die ID des Ansprechpartners / der Institution
	 * @param array $Vorlagen Feld mit den Namen der Vorlagendateiein
	 * @return String HTML-Zeichenkette mit den Vorlagenlinks  
	 */
	function listeVorlagen($fuerID, $Vorlagen)
	{
		return ListeVorlagen($this->Adressen_id, $fuerID, $Vorlagen);
	}
	/**
	 * entfernt den angegebenen Dateianhang einer Bemerkung aus dem Filesystem
	 * @param int id die ID der Bemerkung, deren Anhang entfernt werden soll
	 */
	function entferneAnhang($id)
	{ 
		$query = sql_query("SELECT Anhang FROM " . TABLE_ADRESSENBEMERKUNGEN .
			" WHERE Bemerkung_id = " . $id);
		$bemerkung = sql_fetch_row($query);
		sql_free_result($query);
		$ext = pathinfo($bemerkung[0]);
		if (!unlink($this->getAdressenAnhangLink($id, $ext["extension"])))
		{
			throw new Exception("Datei " . $this->getAdressenAnhangLink($id, $ext["extension"]) .
			" konnte nicht gelöscht werden");
		}
		sql_query("UPDATE " . TABLE_ADRESSENBEMERKUNGEN .
		" SET Anhang = NULL,Bemerkung=CONCAT(Bemerkung,'Anhang gelöscht " . date("d.m.Y H:i") . " " .
			get_user_nickname() . "') WHERE Bemerkung_id=" . $id);
	}
	function holeHistory()
	{
		$text = $this->History;
		return $text;
	}
	function adresseURL()
	{
		return get_url(get_cmodule_id('cvjmadressen'),'Bearbeiten='.$this->Adressen_id);
	}		
		/**
	 * protokolliert eine Aktion auf der Buchung im Logfile. Eine Speicherung erfolgt nicht,
	 * da die Methode im Verlauf des Speichervorgangs aufgerufen werden kann.
	 * @param String $action die Aktion die zu protokollieren ist
	 * @param boolean $save true, wenn die änderung gespeichert werden soll, false sonst
	 */
	function logAction($action, $save = true)
	{
		$this->History = trim(date('d.m.Y H:i ').$action.' - '.get_user_nickname()."\n".$this->History);
		if ( ! $this->isNeu() && $save )
		{
			sql_query('UPDATE '.TABLE_ADRESSEN.' SET History="'.
			sql_real_escape_string($this->History).'" WHERE Adressen_id='.$this->Adressen_id);
		}
	}
	
	// ------------------------------------------------------------------------------

	/**
	 * liefert alle Adressen, die in der Kategorie Lieferant eingetragen sind
	 * @return array assoziatives Feld (ID -> Name) der Lieferanten
	 */
	static function getAlleLieferanten()
	{
		$Lieferanten = array();
		$sql = "SELECT * FROM (cvjm_Adressen INNER JOIN cvjm_Adressen_Kategorie ON ".
		"F_Adressen_id=Adressen_id) INNER JOIN `cvjm_Kategorien` ON ".
		"F_Kategorie_id=Kategorie_id WHERE Kategorie='Lieferant' ORDER BY Name";
		$query = sql_query($sql);
		while ( $lieferant = sql_fetch_array($query) )
		{
			$Lieferanten[$lieferant["Adressen_id"]] = $lieferant['Name'];
		}
		sql_free_result($query);
		return $Lieferanten;
	}
	/**
	 * Entfernt den Versandmarker von allen Adressen
	 */
	static function removeAllVersandmarker()
	{
		sql_query('UPDATE ' . TABLE_ADRESSEN .	' SET Versandmarker=0,Stand=Stand');
	}
	/**
	 * Entfernt den Versandmarker von allen Adressen, die eine Institution sind
	 */
	static function removeAllVersandmarkerInstitutionen()
	{
		sql_query('UPDATE ' . TABLE_ADRESSEN . ',' . TABLE_ADRESSEN_INSTITUTIONEN .
					' SET Versandmarker=0,Stand=Stand ' .
					'WHERE F_Adressen_id=Adressen_id');
	}
	/**
	 * Entfernt den Versandmarker von allen Adressen, die Ansprechpartner sind 
	 */
	static function removeAllVersandmarkerAnsprechpartner()
	{ 
		sql_query('UPDATE ' . TABLE_ADRESSEN . ',' . TABLE_ADRESSEN_INSTITUTIONEN .
					' SET Versandmarker=0,Stand=Stand ' .
					'WHERE F_UAdressen_id=Adressen_id');
	}
	/**
	 * Setzt den Versandmarker für alle Adressen der angegebenen Kategorie
	 * @param int $Kategorie die ID der Kategorie, die gesetzt werden soll
	 */
	static function setVersandmarkerKategorie($Kategorie)
	{
		if (!is_numeric($Kategorie)) 
		{
		throw new Exception('setVersandmarkerKategorie: Keine numerische Kategorie!');		
		}
		sql_query('UPDATE ' .	TABLE_ADRESSEN . ',' . TABLE_ADRESSEN_KATEGORIE .
		' SET Versandmarker=1,Stand=Stand ' .
		'WHERE Adressen_id=F_Adressen_id AND F_Kategorie_id=' .
		$Kategorie);
	}
	/**
	 * hole die Anreden aus der Datenbank. Gibt entweder eine spezielle Anrede Zurück oder ein Feld aller vorhandenen
	 * @param int $anrede_id die ID der Anrede, die Zurückgegeben werden soll. -1 wenn alle Anreden Zurückgegeben werden sollen.
	 * @return String/array die Anrede oder das Feld der Anreden
	 */
	static function getAnredearten($anrede_id = -1)
	{
		if ( ! isset(self::$Anredeartenfeld) || Count(self::$Anredeartenfeld)==0)
		{
			$query = sql_query('SELECT * FROM '.TABLE_ANREDEN.' ORDER BY Anrede');
			self::$Anredeartenfeld = array();
			while ( $anrede = sql_fetch_array($query) )
			{
				self::$Anredeartenfeld[$anrede['Anrede_id']] = $anrede['Anrede'];
			}
			sql_free_result($query);
		}
		if ( $anrede_id < 0)
		{
			return self::$Anredeartenfeld;
		}
		else
		{
			return self::$Anredeartenfeld[$anrede_id];
		}
	}
	/**
	 * liefert die Adresse zur Kundennummer. Existiert keine, wird ein Fehler ausgelöst.
	 * @throws Exception Kundennummer nicht gefunden 
	 * @param int $Kundennummer die gesuchte Kundennummer
	 * @return Adresse die Adresse
	 */
	static function getKundePerKundennummer($Kundennummer)
	{
		$a = null;
		if ( is_numeric($Kundennummer))
		{
			$query = sql_query('SELECT Adressen_id FROM '.TABLE_ADRESSEN.' WHERE Kunden_Nr='.$Kundennummer);
			if ($Adresse = sql_fetch_row($query))
			{
				$a = new Adresse($Adresse[0]);
			}
			else
			{
				throw new Exception('getKundePerKundennummer: Kundennummer '.$Kundennummer.' existiert nicht.');
			}
			sql_free_result($query);
			return $a;
		}
		else
		{
			throw new Exception('getKundePerKundennummer: Die Kundennummer ist nicht numerisch');
		}
	}
} // Klasse Adresse
?>