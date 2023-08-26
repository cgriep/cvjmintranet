<?php
require_once(INC_PATH.'cvjm/DBEntity.class.php');
require_once(INC_PATH.'cvjm/Adresse.class.php');
require_once(INC_PATH.'cvjm/Artikel.class.php');
require_once(INC_PATH.'cvjm/Event.class.php');
//require_once(INC_PATH.'misc/cvjmArtikeltools.php');

/**
 * Beschreibt eine Buchung in der Datenbank
 */
class Buchung extends DBEntity
{
	/**
	 * der Kunde der Buchung
	 */
	var $Adresse;
	// Liste mit den Buchungen, die Artikel enthalten, die beim letzten Buchen hinzugefuegt wurden
	var $Fehlerliste = array();
	var $Smilycard = null;
	// Konstruktor
	function __construct($Buchung_Nr = -1, $Adressen_id = -1)
	{
		parent::__construct(TABLE_BUCHUNGEN);
		if (! is_numeric($Buchung_Nr) || $Buchung_Nr <= 0)
		{
			// Neue Buchung
			if ( $Adressen_id <= 0)
			{
				$this->Adresse = null;
				//throw new Exception('Keine gueltige Adressen_id bei neuer Buchung angegeben!');
				$this->F_Adressen_id = -1;
			}
			else
			{
				$this->Adresse = new Adresse($Adressen_id);
				if ($this->Adresse->Kunden_Nr<=0)
				{
					$this->Adresse->getKundenNummer();
				}
				$this->F_Adressen_id = $this->Adresse->Adressen_id;
			}
			$this->Buchung_Nr = -1;
			$this->Von = time();
			$this->Bis = time();
			$this->OriginalPersonenanzahl = -1;
			$this->OriginalSpeiseraum_id = -1;
			$this->OriginalGruppenRaum_id = -1;
			$this->OriginalBuchung_Nr = -1;
			$this->F_Institution = -1;
			$this->Institution = null;
			$this->BuchungStand = time();
			$this->Buchungsrabatt = $this->Adresse->Rabattsatz;
			$this->EMailKorrespondenz = $this->Adresse->ImmerPerEMail;
			// Standardpreisliste festlegen
			$query = sql_query('SELECT Preisliste_id FROM '.TABLE_PREISLISTEN.' WHERE Standard');
			if ( $preisliste = sql_fetch_row($query))
			{
			$this->F_Preisliste_id = $preisliste[0];
			}
			sql_free_result($query);
			$this->logAction('Buchung angelegt', false);
		} else
		{
			// vorhandene Buchung laden
			$query = sql_query('SELECT * FROM ' . TABLE_BUCHUNGEN .' WHERE Buchung_Nr = ' . $Buchung_Nr);
			if (!$buchung = sql_fetch_array($query))
			{
				throw new Exception('Buchung '.$Buchung_Nr.' nicht gefunden!');
			}
			sql_free_result($query);
			$this->Adresse = new Adresse($buchung['F_Adressen_id']);
			$this->uebertrageFelder($buchung);
			$this->OriginalPersonenanzahl = $this->personenAnzahl();
			$this->OriginalSpeiseraum_id = $this->F_Speiseraum_id;
			$this->OriginalGruppenRaum_id = $this->F_Aufenthaltsraum_id;
			$this->OriginalBuchung_Nr = $this->Buchung_Nr;
			if ( $this->F_Institution >= 0)
			{
				$this->Institution = new Adresse($this->F_Institution);
			}
		}
	} // Buchung erstellen
	/**
	 * ändert den Kunden auf die angegebene Adresse. Der Datensatz wird nicht gespeichert!
	 * @param int $Adressen_id die ID der Kundenadresse
	 */
	function KundeAendern($Adressen_id)
	{
		if ( is_numeric($Adressen_id))
		{
			$a = new Adresse($Adressen_id);
			// Wichtig: Format beachten, da Frontend Bezug darauf nimmt
			$this->logAction('Kunde von KuNr '.$this->Adresse->getKundennummer().' auf '.$a->getKundennummer().' geändert.', false);
			$this->F_Adressen_id = $Adressen_id;
			$this->Adresse = $a;
		}
		else
		{
			throw new Exception ('KundeAendern: '.$Adressen_id.' ist keine gueltige Angabe.');
		}
	}
	/**
	 * liefert ein Feld mit den Kundennummern der bisherigen Kunden. Enthält mindestens die aktuelle 
	 * Kundennummer.
	 * @return array Feld mit den Kundennummern bisheriger Kunden. 
	 */
	function bisherigeKunden()
	{
		// auf Kundenwechsel pruefen
		$kunden = array($this->Adresse->Kunden_Nr);
		$text = $this->Logtext;
		while ( $i = strpos($text, ' KuNr '))
		{
			$text = trim(substr($text, $i+6));
			$KuNr = substr($text,0,strpos($text,' '));
			if ( ! is_numeric($KuNr)) 
			{
				throw new Exception('Fehler KuNr: '.$KuNr);
			}
			if ( $KuNr != $this->Adresse->Kunden_Nr)
			{
			  	$kunden[] = $KuNr;
			}
		}
		return $kunden;
	}
	/**
	 * liefert eine Zeichenkette mit der History, wobei Sonderfunktionen beruecksichtigt sind. Die Zeichenkette
	 * enthält HTML-Querverweise.
	 * @param string $url die URL an die verwiesen wird. Sie wird um die Adressen_id ergänzt und sollte mit "=" enden.
	 * @return string die History mit HTML-Verlinkungen
	 */
	function holeHistory($url)
	{
		$text = $this->Logtext;
		$LPos = 0;
		// Kundenwechsel 
		$KUNDENERKENNUNG = ' KuNr ';
		$Len = strlen($KUNDENERKENNUNG);
		while ( $i = strpos($text, $KUNDENERKENNUNG, $LPos))
		{
			$KuNr = trim(substr($text, $i+$Len));
			$p = strpos($KuNr,' ');
			$KuNr = substr($KuNr,0,$p);
			$LPos = $i+$Len+$p; 
			try {
				$Kunde = Adresse::getKundePerKundennummer($KuNr);
				$neu = '<a href="'.$url.$Kunde->Adressen_id.'">'.$KuNr.'</a>';
				$text = substr_replace($text,$neu,$i+$Len,strlen($KuNr));
			} catch ( Exception $e )
			{
				// wenn Kundennummer nicht gefunden ...
			}
		}
		$LPos = 0;
		// Kundenwechsel 
		$RECHNUNGSERKENNUNG = 'Rechnung Nr. ';
		$Len = strlen($RECHNUNGSERKENNUNG);
		while ( $i = strpos($text, $RECHNUNGSERKENNUNG, $LPos))
		{
			$KuNr = trim(substr($text, $i+$Len));
			$p = strpos($KuNr,' ');
			$KuNr = substr($KuNr,0,$p);
			$LPos = $i+$Len+$p;
			// Rechnungsnummer in Rechnung_id wandeln 
			try {
				$neu = '<a href="?id='.get_cmodule_id('cvjmabrechnung').'&docinput[Rechnungsnummer]='.$KuNr.'">'.$KuNr.'</a>';
				$text = substr_replace($text,$neu,$i+$Len,strlen($KuNr));
			} catch ( Exception $e )
			{
				// wenn Rechnungsnummer nicht gefunden ...
			}
		}
		$LPos = 0;
		// Artikel 
		$RECHNUNGSERKENNUNG = 'Artikel ';
		$Len = strlen($RECHNUNGSERKENNUNG);
		while ( $i = strpos($text, $RECHNUNGSERKENNUNG, $LPos))
		{
			$KuNr = trim(substr($text, $i+$Len));
			$p = strpos($KuNr,' ');
			$KuNr = substr($KuNr,0,$p);
			$LPos = $i+$Len+$p;
			// Rechnungsnummer in Rechnung_id wandeln 
			try {
				$neu = '<a href="?id='.get_cmodule_id('cvjmartikel').'&docinput[Artikel_Nr]='.$KuNr.'">'.$KuNr.'</a>';
				$text = substr_replace($text,$neu,$i+$Len,strlen($KuNr));
			} catch ( Exception $e )
			{
				// wenn Rechnungsnummer nicht gefunden ...
			}
		}
		return $text;
	}
	/**
	 * gibt an ob es sich um eine neue Buchung handelt oder ob sie schon in der Datenbank
	 * gespeichert ist.
	 * @return boolean true, wenn die Buchung noch nicht gespeichert ist, false sonst
	 */
	function isNeu()
	{
		return ! is_numeric($this->Buchung_Nr) || $this->Buchung_Nr <= 0;
	}
	/**
	 * protokolliert eine Aktion auf der Buchung im Logfile. Eine Speicherung erfolgt nicht,
	 * da die Methode im Verlauf des Speichervorgangs aufgerufen werden kann.
	 * @param String $action die Aktion die zu protokollieren ist
	 * @param boolean $save true, wenn die änderung gespeichert werden soll, false sonst
	 */
	function logAction($action, $save = true)
	{
		$this->Logtext = trim(date('d.m.Y H:i ').$action.' - '.get_user_nickname()."\n".$this->Logtext);
		if ( ! $this->isNeu() && $save )
		{
			sql_query('UPDATE '.TABLE_BUCHUNGEN.' SET Logtext="'.
			sql_real_escape_string($this->Logtext).'" WHERE Buchung_Nr='.$this->Buchung_Nr);
		}
	}
	/**
	 * Löscht eine Buchung
	 */
	function loeschen()
	{
		// TODO!
	}
	/**
	 * Storniert die Buchung und entfernt alle BuchungsEinträge. Diese werden unter Internes aufgefuehrt. 
	 * @param boolean $nurLeeren true, wenn die Einträge entfernt werden sollen, die Buchung aber unberuehrt bleibt, true wenn die Buchung storniert werden soll
	 */
	function stornieren($nurLeeren = false)
	{
		if ( $this->isNeu() || $this->isFertig() )
		{
			throw new Exception('stornieren: Die Buchung '.$this->Buchung_Nr.' ist neu oder fertig!');
		}
		// Einträge in die internen Bemerkungen speichern
		$s .= 'Letzter Status: '.$this->Buchungsstatus();
		$s .= "\nStorniert am " . date('d.m.Y H:i') . "\nGebucht waren folgende Einträge:\n";
		$query = sql_query('SELECT DISTINCT F_Artikel_Nr FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' WHERE F_Buchung_Nr=' . $this->Buchung_Nr);
		while ($eintrag = sql_fetch_array($query))
		{
			$a = new Artikel($eintrag['F_Artikel_Nr']);
			$s .= $a->Bezeichnung. "\n";
		}
		sql_free_result($query);
		$this->Internes = trim($this->Internes."\n".$s);
		if ( ! $nurLeeren )
		{
			// Status anpassen
			if ($this->BStatus <= BUCHUNG_VORRESERVIERUNG)
			{
				$this->BStatus = BUCHUNG_GELOESCHT;
			}
			elseif ($this->BStatus == BUCHUNG_RESERVIERUNG)
			{
				$this->BStatus = BUCHUNG_STORNIERT;
			}
			$this->logAction('storniert');
		}
		else
		{
			$this->logAction('Alle Einträge ausgebucht');
		}
		// Alle Einträge der Buchung löschen
		sql_query('DELETE FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' WHERE F_Buchung_Nr=' . $this->Buchung_Nr);
		$this->save();
	}
	/**
	 * Speichert die Buchung
	 */
	function save()
	{
		// Daten speichern
		if ( $this->Adresse == null ) return;
		foreach ( array ('Anzahlung', 'Ankunftszeit', 'Abfahrtszeit',
		'BetreueranzahlW', 'BetreueranzahlM', 'Vegetarier', 'Schweinelos',
		'Altersgruppe', 'F_Preisliste_id', 'MwStAusweisen', 'EMailKorrespondenz',
		'Eingang','BStatus','Kuechenhilfe','Buchungsrabatt') as $Feld)
		{
			if (!is_numeric($this->$Feld) )
			{
				$this->$Feld = 0;
			}
		}
		foreach ( array ('F_Aufenthaltsraum_id', 'F_Speiseraum_id',
		'F_Institution') as $Feld)
		{
			if (!is_numeric($this->$Feld) )
			{
				$this->$Feld = -1;
			}
		}
		if (! is_numeric($this->Von) || $this->Von == 0)
		{
			$this->Von = time();
		}
		if (!is_numeric($this->Bis))
		{
			$this->Bis = time();
		}
		for ($i = 1; $i < 7; $i++)
		{
			$p = 'Personen'.$i;
			$pw = 'Personen'.$i.'w';
			if (!is_numeric($this->$p))
			{
				$this->$p = 0;
			}
			if (!is_numeric($this->$pw))
			{
				$this->$pw = 0;
			}
		}
		if (! is_numeric($this->Kuechenhilfe))
		{
			$this->Kuechenhilfe = 1;
		}
		// Status pruefen. Sobald ein Eingang vorliegt, ist es eine Reservierung
		if ($this->Eingang != 0)
		{
			if ($this->BStatus < BUCHUNG_RESERVIERUNG)
			{
				$this->BStatus = BUCHUNG_RESERVIERUNG;
				$this->logAction('neuer Status: Reservierung', false);
			}
		}
		elseif ( ! $this->isFertig() )
		{
			$this->BStatus = 0;
		}
		if (! $this->isNeu() )
		{
			// Pruefen, ob sich die Personenanzahl geändert hat.
			if ($this->personenAnzahl() != $this->OriginalPersonenanzahl)
			{
				// wenn ja, Essen und Programm anpassen
				$sql = 'UPDATE ' . TABLE_BUCHUNGSEINTRAEGE . ', ' . TABLE_ARTIKEL . ', ' .
				TABLE_ARTIKELARTEN . ' SET Menge=' . $this->personenAnzahl() .
				' WHERE F_Buchung_Nr=' . $this->Buchung_Nr . ' AND Menge=' .
				$this->OriginalPersonenanzahl . ' AND F_Artikel_Nr=id AND F_Art_id=Art_id AND Typ IN (' .
				ABRECHNUNGSTYP_KOPF . ',' . ABRECHNUNGSTYP_KOPFNACHT . ',' .
				ABRECHNUNGSTYP_VERPFLEGUNG . ',' . ABRECHNUNGSTYP_VERPFLEGUNGNACHT . ')';
				if (!sql_query($sql))
				{
					throw new Exception('Fehler bei Personenänderung '.$sql.': ' . sql_error());
				}
				$this->logAction('Personenanzahl geändert von '.$this-OriginalPersonenanzahl.' auf '.$this->personenAnzahl());
			}
			// Update
			$sql = 'UPDATE ' . TABLE_BUCHUNGEN . ' SET ';
			$sql .= 'Internes="' . sql_real_escape_string($this->Internes);
			$sql .= '",Von=' . $this->Von;
			$sql .= ',Bis=' . $this->Bis;
			$sql .= ',F_Preisliste_id=' . $this->F_Preisliste_id;
			$sql .= ',Eingang='. $this->Eingang;
			$sql .= ',Anzahlung=' . $this->Anzahlung;
			$sql .= ',BetreueranzahlM=' . $this->BetreueranzahlM;
			$sql .= ',BStatus='.$this->BStatus;
			$sql .= ',BetreueranzahlW=' . $this->BetreueranzahlW;
			$sql .= ',Altersgruppe=' . $this->Altersgruppe;
			$sql .= ',Kuechenhilfe=' . $this->Kuechenhilfe;
			$sql .= ',Essenbesonderheit="' . sql_real_escape_string(trim($this->Essenbesonderheit)) . '"';
			$sql .= ',Vereinbarungen="' . sql_real_escape_string(trim($this->Vereinbarungen)) . '"';
			$sql .= ',Vegetarier=' . $this->Vegetarier;
			$sql .= ',Buchungsrabatt=' . $this->Buchungsrabatt;
			$sql .= ',Steuerbehandlung='.$this->Steuerbehandlung;
			$sql .= ',Schweinelos=' . $this->Schweinelos;
			$sql .= ',Ankunftszeit=' . $this->Ankunftszeit;
			$sql .= ',Abfahrtszeit=' . $this->Abfahrtszeit;
			$sql .= ',EMailKorrespondenz='.$this->EMailKorrespondenz;
			$sql .= ',MwStAusweisen='.$this->MwStAusweisen;
			$sql .= ',AnzahlungsBemerkung="' . sql_real_escape_string($this->AnzahlungsBemerkung) . '"';
			$sql .= ',F_Aufenthaltsraum_id=' . $this->F_Aufenthaltsraum_id;
			$sql .= ',F_Adressen_id=' . $this->F_Adressen_id;
			// Pruefen ob Speiseraum sich geändert hat
			if ($this->F_Aufenthaltsraum_id != $this->OriginalGruppenRaum_id)
			{
				$this->logAction('Gruppenraum geändert von '.$this->OriginalGruppenRaum_id.' nach '.$this->F_Aufenthaltsraum_id);
			}			
			// Pruefen ob Speiseraum sich geändert hat
			if ($this->F_Speiseraum_id != $this->OriginalSpeiseraum_id)
			{
				// Speiseraum setzen.
				$sql .= ',F_Speiseraum_id=' . $this->F_Speiseraum_id;
				// Wichtig: Buchung vollfuehren!
				// 1. Alten Speiseraum entfernen
				sql_query("DELETE FROM " . TABLE_BUCHUNGSEINTRAEGE . " WHERE F_Buchung_Nr=" .
				$this->Buchung_Nr . " AND F_Artikel_Nr=" . $this->OriginalSpeiseraum_id);
				// 2. neuen Speiseraum setzen
				if ($this->F_Speiseraum_id != -1)
				{
					$datum = strtotime(date('Y-m-d', $this->Von));
					for ($i = $datum; $i <= $this->Bis; $i = strtotime('+1 day', $i))
					{
						sql_query('INSERT INTO ' . TABLE_BUCHUNGSEINTRAEGE .
						' (F_Buchung_Nr,F_Artikel_Nr,Menge,Datum) VALUES (' .
						$this->Buchung_Nr . ',' . $this->F_Speiseraum_id . ',1,'.$i.')');
					}
				}
				$this->logAction('Speiseraum geändert von '.$this->OriginalSpeiseraum_id.' nach '.$this->F_Speiseraum_id);
			}
			for ($i = 1; $i < 7; $i++)
			{
				$p = 'Personen'.$i;
				$pw = 'Personen'.$i.'w';
				$sql .= ',Personen'.$i.'=' . $this->$p;
				$sql .= ',Personen' . $i . 'w=' . $this->$pw;
			}
			$sql .= ',F_Institution=' . $this->F_Institution . ',';
			if ($this->OriginalBuchung_Nr != $this->Buchung_Nr)
			{
				$sql .= 'Buchung_Nr=' . $this->Buchung_Nr . ',';
			}
			// änderung protokolieren
			$this->logAction('geändert', false);
			$sql .= 'Logtext="' . sql_real_escape_string($this->Logtext).'"';
			$sql .= ' WHERE Buchung_Nr = ' . $this->Buchung_Nr;
			if (!sql_query($sql))
			{
				throw new Exception('Fehler beim Buchungspeichern: '.$sql . '/' . sql_error());
			}
		} else
		{
			// Insert
			$sql = "INSERT INTO " . TABLE_BUCHUNGEN . ' (Internes, Von, Bis, ';
			$sql .= 'Anzahlung,F_Preisliste_id, Eingang, BStatus, Altersgruppe,';
			$sql .= 'Logtext, BetreueranzahlM,BetreueranzahlW,';
			$sql .= 'Kuechenhilfe,Essenbesonderheit,Vegetarier,Schweinelos,';
			for ( $i=1;$i<7;$i++)
			{
				$sql .= 'Personen'.$i.',Personen'.$i.'w,';
			}
			$sql .= 'Ankunftszeit,Abfahrtszeit,AnzahlungsBemerkung,';
			$sql .= 'Vereinbarungen,Buchungsrabatt,Steuerbehandlung,EMailKorrespondenz,';
			$sql .= 'MwStAusweisen,F_Aufenthaltsraum_id,F_Speiseraum_id,F_Institution,';
			$sql .= 'F_Adressen_id) VALUES (';
			$sql .= '"'.sql_real_escape_string($this->Internes) . '",';
			$sql .= $this->Von . "," . $this->Bis . ",";
			$sql .= $this->Anzahlung . "," . $this->F_Preisliste_id . ",";
			$sql .= $this->Eingang.',';
			$sql .= $this->BStatus.','. $this->Altersgruppe . ",";
			$sql .= '"'.sql_real_escape_string($this->Logtext).'",';
			$sql .= $this->BetreueranzahlM . "," . $this->BetreueranzahlW . ",";
			$sql .= $this->Kuechenhilfe . ",";
			$sql .= "'" . sql_real_escape_string($this->Essenbesonderheit) . "',";
			$sql .= $this->Vegetarier . "," . $this->Schweinelos;
			for ($i = 1; $i < 7; $i++)
			{
				$p = 'Personen'.$i;
				$pw = 'Personen'.$i.'w';
				$sql .= "," . $this->$p;
				$sql .= ','. $this->$pw;
			}
			$sql .= "," . $this->Ankunftszeit . "," . $this->Abfahrtszeit;
			$sql .= ',"' . sql_real_escape_string($this->AnzahlungsBemerkung) . '"';
			$sql .= ',"';
			$sql .= sql_real_escape_string($this->Vereinbarungen) . '",';
			$sql .= $this->Buchungsrabatt.','.$this->Steuerbehandlung.',';
			$sql .= $this->EMailKorrespondenz.','.$this->MwStAusweisen.',';
			$sql .= $this->F_Aufenthaltsraum_id.','.$this->F_Speiseraum_id.',';
			$sql .= $this->F_Institution.','.$this->F_Adressen_id.')';
			if (!sql_query($sql))
			{
				throw new Exception('Buchung anlegen: '.$sql . "/" . sql_error());
			}
			$this->Buchung_Nr = sql_insert_id();
		}
		// Originalinformation sichern fuer spätere Anpassungen
		$this->OriginalPersonenanzahl = $this->personenAnzahl();
		$this->OriginalSpeiseraum_id = $this->F_Speiseraum_id;
		$this->OriginalGruppenRaum_id = $this->F_Aufenthaltsraum_id;
		if ( $this->OriginalGruppenRaum_id == '' )
		{
			$this->OriginalGruppenRaum_id = -1;
		}
		$this->OriginalBuchung_Nr = $this->Buchung_Nr;
		$this->BuchungStand = time();
	}
	/**
	 * ergibt die Menge, die standardmäßig bei einer bestimmten Abrechnungsart angewendet werden soll.
	 * @param int $Abrechnungstyp der Abrechnungstyp
	 * @return int die Menge die eingesetzt werden soll
	 */
	function standardMenge($Abrechnungstyp)
	{
		if ($Abrechnungstyp != ABRECHNUNGSTYP_KOPF &&
		$Abrechnungstyp != ABRECHNUNGSTYP_KOPFNACHT &&
		$Abrechnungstyp != ABRECHNUNGSTYP_VERPFLEGUNG)
		{
			return 1;
		}
		else
		{
			return $this->personenanzahl();
		}
	}
	/**
	 * ergibt die Menge, die standardmäßig bei einer bestimmten Abrechnungsart angewendet werden soll.
	 * @param int $Abrechnungstyp der Abrechnungstyp
	 * @return int die Menge die eingesetzt werden soll
	 */
	function standardMengeBeiArtikel($Artikel_id)
	{
		$a = new Artikel($Artikel_id);
		return $this->standardMenge($a->getAbrechnungstyp());
	}
	/**
	 * gibt die Altersgruppe in lesbarer Form zurueck
	 * @return String der Name der Altersgruppe
	 */
	function getAltersgruppe()
	{
		$Altersgruppen = self::getAltersgruppen();
		if ( isset($Altersgruppen[$this->Altersgruppe]))
		{
			return $Altersgruppen[$this->Altersgruppe];
		}
		else
		{
			return '-unbekannt ('.$this->Altersgruppe.')-';
		}
	}
	/**
	 * gibt den Inhalt des Felder zurueck. Hier können auch berechnete Inhalte
	 * abgefragt werden.
	 * @param String $feld der Feldname des gesuchten Feldes
	 * @return String der Inhalt des Feldes oder eine leere Zeichenkette
	 */
	function getFeld($feld)
	{
		if ( isset($this->$feld))
		{
			return $this->$feld;
		}
		switch ($feld)
		{
			case 'ErstelltAm':
				if (strlen($this->Logtext) > 10)
				{
					$pos = strrpos($this->Logtext, "\n");
					if ($pos === false)
					{
						$pos = -1;
					}
					return substr($this->Logtext, $pos +1, 10);
					break;
				}
			default:
				if ( isset($this->Adresse->$feld))
				{
					return $this->Adresse->$feld;
				}
		}
		return '';
	}
	/**
	 * liefert den Namen der Altersgruppe mit der angegebenen Nummer
	 * @param int $nr die Nummer der gewuenschten Altersgruppe (von 1 bis 6)
	 * @return String der Name der Altersgruppe
	 */
	function getAlterswertName($nr)
	{
		switch ( $nr)
		{
			case 1: return 'unter 2';
			case 2: return '2-6';
			case 3: return '7-15';
			case 4: return '16-18';
			case 5: return '19-27';
			case 6: return 'ab 28';
			default:
				return '-unbekannt-';
		}
	}
	/**
	 *  gibt eine Liste mit den Anzahlen der Personen zurueck
	 * @return array ein nummeriertes Feld fuer die Altersgruppen. Indizes sind m und w sowie Anzeige mit dem Namen der Altersgruppe
	 */
	function getPersonenListe()
	{
		$Personen = array ();
		for ($i = 6; $i > 0; $i--)
		{
			$p = 'Personen'.$i;
			$pw = $p.'w';
			$Personen[$i]['m'] = $this->$p;
			$Personen[$i]['w'] = $this->$pw;
		}
		return $Personen;
	}
	/**
	 * Berechnet die Anzahl der Personen in der Buchung
	 * @return int die Anzahl der Personen
	 */
	function personenAnzahl()
	{
		$anz = 0;
		for ( $i=1; $i < 7; $i++)
		{
			$p = 'Personen'.$i;
			$pw = 'Personen'.$i.'w';
			if ( isset($this->$p))
			{
				$anz += $this->$p;
			}
			if ( isset($this->$pw))
			{
				$anz += $this->$pw;
			}
		}
		return $anz;
	}
	/**
	 * liefert ein Feld der möglichen Speiseräume inkl. der Option "keiner". Das Feld ist mit der Artikel_id
	 * des Saals indiziert. Soweit der Saal durch eine andere Buchung belegt ist, wird deren Nummer hinter dem
	 * Namen des Saals angezeigt.
	 * @return array indiziertes Feld mit den Namen der Speisesäle.
	 */
	function getSpeiseraumListe()
	{
		$Speiseraum = array ();
		$Speiseraum[-1] = '- unbekannt -';
		$Speiseraum[-2] = '- nicht gewünscht -';
		$raeume = Artikel::getSpeiseraeume();
		foreach ( $raeume as $raum )
		{
			$Speiseraum[$raum->Artikel_id] = $raum->Bezeichnung;
			// evtl. Belegung des Saals anzeigen
			if (! $this->isNeu() != '')
			{
				if (!$query = sql_query("SELECT DISTINCT F_Buchung_Nr FROM " . TABLE_BUCHUNGSEINTRAEGE .
				" WHERE F_Artikel_Nr=" . $raum->Artikel_id . " AND Datum BETWEEN " . $this->Von . " AND " .
				$this->Bis . " AND F_Buchung_Nr<>" . $this->Buchung_Nr))
				{
					throw new Exception('getSpeiseraumListe: '.sql_error());
				}
				while ($saal = sql_fetch_array($query))
				{
					$b = new Buchung($saal['F_Buchung_Nr']);
					if ($b->Bis == $this->Von)
					{
						$Speiseraum[$raum->Artikel_id] = '&larr; '.$Speiseraum[$raum->Artikel_id];
					}
					elseif ($b->Von == $this->Bis)
					{
						$Speiseraum[$raum->Artikel_id] = $Speiseraum[$raum->Artikel_id].' &rarr;';
					}
					else
					{
						$Speiseraum[$raum->Artikel_id] = '['.$Speiseraum[$raum->Artikel_id].']';
						//$Speiseraum[$raum->Artikel_id] .= '-' . $saal['F_Buchung_Nr'];
					}
				}
				sql_free_result($query);
			}
		}
		return $Speiseraum;
	}
	function getSpeiseraum()
	{
		if ( $this->isNeu())
		{
			return '';
		}
		$Artikel = new Artikel($this->F_Speiseraum_id);
		return $Artikel->Bezeichnung;
	}

	/**
	 * liefert ein Feld mit der Liste aller Räume, die als Gruppenraum möglich sind (also keine
	 * Schlafplätze haben). Zusätzlich wird ein Eintrag für "keiner" angeboten.
	 * @returns array ein Feld id->Bezeichnung der Gruppenräume.
	 */
	function getGruppenraumListe()
	{
		$Gruppenraum = array ();
		$Gruppenraum[-1] = ' - keiner -';
		$Gruppenraum[-2] = ' - nicht gewünscht -';
		if ( ! $this->isNeu() )
		{
			$sql = "SELECT DISTINCT id FROM " . TABLE_ARTIKEL . " INNER JOIN " . TABLE_BUCHUNGSEINTRAEGE .
			" ON id=F_Artikel_Nr WHERE F_Buchung_Nr=" . $this->Buchung_Nr . " AND F_Art_id=" .
			CVJMART_ORT . " ORDER BY Bezeichnung";
			$q = sql_query($sql);
			while ($raum = sql_fetch_row($q))
			{
				$a = new Artikel($raum[0]);
				if ($a->BerechneSchlafplaetze() < 0)
				{
					$Gruppenraum[$a->Artikel_id] = $a->Bezeichnung;
				}
			}
			sql_free_result($q);
		}
		return $Gruppenraum;
	}
	/**
	 * Bestimmt die Anzahl der Übernachtungen der Buchung
	 * @return die Anzahl der Übernachtungen in Tagen
	 */
	function berechneUebernachtungen()
	{
		return round(($this->Bis-$this->Von) / 86400);
	}
	/**
	 * Zeigt an ob die Buchung erledigt ist oder nicht
	 * @return boolean true, wenn die Buchung erledigt ist, false sonst
	 */
	function isFertig()
	{
		return $this->BStatus >= BUCHUNG_FERTIG;
	}
	/**
	 * Schaltet die Buchung zum Seminar bzw. wieder Zurück
	 * @param boolean $Seminar true, wenn es sich um ein Seminar handelt, false sonst
	 */
	function macheSeminar($Seminar)
	{
		// zum Seminar machen
		if ( ! is_numeric($Seminar))
		{
			throw new Exception ('Fehler beim Seminarmachen: Kein numerisches Argument');
		}
		sql_query("UPDATE " . TABLE_BUCHUNGEN . " SET Seminar=" . $Seminar . ' WHERE Buchung_Nr=' . $this->Buchung_Nr);
		$this->logAction('zum Seminar gemacht');
	}
	/**
	 * Verändert den Status der Buchung.
	 * @param int $art 1- setzt den Status auf Reservierung, 2- setzt den Status auf intern
	 */
	function reaktivieren($art)
	{
		if ($art == 1)
		{
			sql_query("UPDATE " .	TABLE_BUCHUNGEN . " SET BStatus=" . BUCHUNG_RESERVIERUNG .
			' WHERE Buchung_Nr=' . $this->Buchung_Nr);
			$action = 'Buchung reaktivert';
			$this->BStatus = BUCHUNG_RESERVIERUNG;
		}
		elseif ($art== 2)
		{
			sql_query("UPDATE " .TABLE_BUCHUNGEN . " SET BStatus=" . BUCHUNG_INTERN .
			' WHERE Buchung_Nr=' . $this->Buchung_Nr);
			$action = 'Buchung als intern abgeschlossen';
			$this->BStatus = BUCHUNG_INTERN;
		}
		
		$this->logAction($action);
	}
	/**
	 * setzt den Status der Buchung und schreibt eine entsprechende Bemerkung
	 * Achtung: Speichert nicht!
	 * @param int $status der neue Status der Buchung 
	 */
	function aendereStatus($status)
	{
		if ( is_numeric($status))
		{
			$this->BStatus= $status;
			$this->logAction('Buchungstatus auf "'.$this->Buchungsstatus().'" gesetzt');
		}
		else
		{
			throw new Exception('aendereStatus: Status nicht numerisch - '.$status);
		}
	}
	/**
	 * Buchungsstatus in Textform
	 * @return String der Buchungsstatus
	 */
	function Buchungsstatus()
	{
		switch ($this->BStatus)
		{
			case 0: return 'Vorreservierung';
			case 1: return '1. Nachfrage';
			case BUCHUNG_VORRESERVIERUNG: return '2. Nachfrage'; // < 2 ist vorreserviert
			case BUCHUNG_GELOESCHT: return 'Vorreservierung gelöscht';
			case BUCHUNG_RESERVIERUNG: return 'Reservierung';
			case BUCHUNG_FERTIG: return 'Abgerechnet'; // > 20 ist abgerechnet
			case BUCHUNG_STORNIERT: return 'Storniert';
			case BUCHUNG_STORNIERTABGERECHNET: return 'Storniert, abgerechnet';
			case BUCHUNG_INTERN: return 'Intern';
			default:
				return '-unbekannt-';
		}
	}
	/**
	 * Bildrepräsentation des Status
	 */
	function buchungsStatusBild()
	{
		switch ( $this->BStatus )
		{
			case BUCHUNG_GELOESCHT: // gelöscht
				return '<img src="img/CVJM/geloescht.gif" title="Vorreservierung gelöscht">';
			case 0:
				return '<img src="img/CVJM/vorres.gif" title="Vorreservierung">';
				break;
			case 1:
				return '<img src="img/CVJM/1nachfrage.gif" title="1. Nachfrage">';
				break;
			case BUCHUNG_VORRESERVIERUNG: // Vorreservierung
				return '<img src="img/CVJM/2nachfrage.gif" title="2. Nachfrage">';
				break;
			case BUCHUNG_RESERVIERUNG:  // Reserviert
				return '<img src="img/CVJM/reserviert.gif" title="Reserviert">';
				break;
			case BUCHUNG_FERTIG: // abgerechnet
				return '<img src="img/CVJM/abgerechnet.gif" title="Abgerechnet">';
				break;
			case BUCHUNG_STORNIERT: // Storniert
				return '<img src="img/CVJM/storno.gif" title="Storniert">';
			default:
				return '';
		}
	}
	/**
	 * Entfernt eine Abrechnung der Buchung endg�ltig
	 * @param abrechnung_id die ID der zu löschenden Abrechnung
	 */
	function loescheAbrechnung($abrechnung_id)
	{
		if (!$this->isNeu() && is_numeric($abrechnung_id))
		{
			sql_query('DELETE FROM ' . TABLE_RECHNUNGSEINTRAEGE . ' WHERE F_Rechnung_id=' .
			$abrechnung_id);
			sql_query('DELETE FROM ' . TABLE_RECHNUNGEN . ' WHERE Rechnung_id=' . $abrechnung_id);
			// wenn das die letzte Abrechnung war, dann die Anzahlung löschen
			$query = sql_query('SELECT Count(*) FROM ' . TABLE_RECHNUNGEN . ' WHERE F_Buchung_Nr=' .
			$this->Buchung_Nr);
			if ($row = sql_fetch_row($query))
			if ($row[0] == 0 && $this->AnzahlungsBemerkung == '')
			{
				$this->Anzahlung = 0;
				sql_query('UPDATE ' . TABLE_BUCHUNGEN . ' SET Anzahlung=NULL WHERE Buchung_Nr=' .
				$this->Buchung_Nr);
			}
			sql_free_result($query);
			$this->logAction('Abrechnung id ' . $abrechnung_id . ' gelöscht ');
		}
	}
	/**
	 * setzt die Anzahlung auf 0, wenn sie ungleich null ist und keine Anzahlungsbemerkung vorhanden ist
	 */
	function anzahlungNullen()
	{
		if ( $this->AnzahlungsBemerkung == '' && $this->Anzahlung != 0)
			{				
				$this->logAction('Anzahlung von '.$this->Anzahlung.' genullt.', false);
				$this->Anzahlung=0;
				$this->save();
			}
	}
	/**
	 * Verschiebt den Zeitraum einer Buchung und passt die Unterbringung und Verpflegung
	 * entsprechend an.
	 * @param int neuVon neues Anfangsdatum
	 * @param int neuBis neues Enddatum
	 * @return boolean true, wenn der Buchungszeitraum sich unterscheidet, false sonst
	 */
	function verschiebeBuchung($neuVon, $neuBis)
	{	// Verschiebung der Buchung
		$erg = false;
		if (! $this->isNeu() && is_numeric($neuVon) && is_numeric($neuBis))
		{
			/*
			if ( ! sql_query("UPDATE " . TABLE_BUCHUNGEN . " SET Von=" . $neuVon . ",Bis=" .
			$neuBis . " WHERE Buchung_Nr=" . $this->Buchung_Nr) )
			{
				throw new Exception('Buchungszeittaum '.$this->Buchung_Nr.' verschieben: '.sql_error());
			}
			*/
			$this->logAction('Buchungzeitraum verschoben von '.date('d.m.Y',$this->Von).
			'-'.date('d.m.Y',$this->Bis).' nach '.date('d.m.Y',$neuVon).'-'.date('d.m.Y',$neuBis), false);
			// Verschiebung aller Einträge auf das neue Datum
			$differenz = $neuVon - $this->Von;
			// Reihenfolge festlegen, um bei Überschneidungen doppelte Schlüssel zu vermeiden
			if ( $differenz < 0 )
			{
				$Order .= ' ASC';
			}
			else
			{
				$Order .= ' DESC';
			}
			if ( ! sql_query('UPDATE ' . TABLE_BUCHUNGSEINTRAEGE . ' SET Datum=Datum+'.$differenz.
			' WHERE F_Buchung_Nr=' . $this->Buchung_Nr.' ORDER BY Datum '.$Order) )
			{
				throw new Exception('BuchungsEinträge '.$this->Buchung_Nr.' verschieben: '.sql_error());;
			}
			$diff1 = $neuBis - $neuVon;
			$diff2 = $this->Bis - $this->Von;
			if ($diff1 != $diff2)
			{

				if ($diff1 < $diff2)
				{
					sql_query("DELETE FROM " .	TABLE_BUCHUNGSEINTRAEGE .
					" WHERE F_Buchung_Nr=" . $this->Buchung_Nr . " AND Datum >= " .
					strtotime("+1 day", $neuBis));
				}
				$erg = true;
			}
			$this->logAction('Buchung verschoben von '.date('d.m.Y',$this->Von).'/'.date('d.m.Y',$this->Bis).' nach '.
			date('d.m.Y',$neuVon).'/'.date('d.m.Y',$neuBis), false);
			$this->Von = $neuVon;
			$this->Bis = $neuBis;
			$this->save();			
		} else
		{
			throw new Exception('Keine gültigen Daten für Verschiebung angegeben!');
		}
		return $erg;
	}
	/**
	 * zeigt an ob ein Angebot oder eine Rechnung existiert 
	 * @return boolean
	 */
	function angebotExistiert()
	{
		$sql = 'SELECT * FROM cvjm_Rechnungen WHERE F_Buchung_Nr='.$this->Buchung_Nr;
		if (! $query = sql_query($sql) ) echo sql_error();
		if ( $erg = sql_fetch_row($query))
		{
			$da = true;
		}
		else
		{
			$da = false;
		}
		sql_free_result($query);
		return $da;
	}
	/**
	 * Set die Selbstverpflegung und passt die notwendigen Artikel entsprechend
	 * an.
	 * @param selbstverpflegung bool true, wenn Selbstverpflegung herrscht, false sonst
	 */
	function setSelbstVerpflegung($selbstverpflegung = true)
	{
		// Selbstverpflegung berücksichtigen
		if ($selbstverpflegung)
		{
			$tage = $this->berechneUebernachtungen();
			for ($i = 0; $i <= $tage; $i++)
			{
				// Doppelte Einträge werden von der Datenbank abgewiesen
				sql_query("INSERT INTO " . TABLE_BUCHUNGSEINTRAEGE .
				" (F_Buchung_Nr, F_Artikel_Nr, Menge, Datum) VALUES (" .
				$this->Buchung_Nr . "," . CVJM_SELBSTVERPFLEGUNG . ",1," .
				strtotime("+$i day", $this->Von) . ")");
			}
		}
		else
		{
			// Alle Einträge von Selbstverpflegung löschen
			if (!sql_query("DELETE FROM " . TABLE_BUCHUNGSEINTRAEGE . " WHERE F_Buchung_Nr=" .
			$this->Buchung_Nr . " AND F_Artikel_Nr=" . CVJM_SELBSTVERPFLEGUNG))
			throw new Exception(sql_error());
		}
	} // Selbstverpflegung setzen / löschen
	/**
	 * prüft ob eine Buchung Selbstverpflegung enthält
	 * @return boolean true bei Selbstverpflegung, false sonst
	 */
	function isSelbstverpflegung()
	{ // Selbstverpflegung feststellen
		if ( ! isset($this->Selbstverpflegungsfeld))
		{
			$this->Selbstverpflegungsfeld = false;
			$query = sql_query('SELECT Count(*) FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' WHERE F_Buchung_Nr='.
			$this->Buchung_Nr . ' AND F_Artikel_Nr=' . CVJM_SELBSTVERPFLEGUNG);
			if ($row = sql_fetch_row($query))
			{
				if ($row[0] > $this->berechneUebernachtungen())
				{
					$this->Selbstverpflegungsfeld = true;
				}
			}
			sql_free_result($query);
		}
		return $this->Selbstverpflegungsfeld;
	}
	/**
	 * prüft ob Verpflegung gebucht ist
	 * @return true, wenn Verpflegung gebucht ist, false sonst
	 */
	function verpflegungVorhanden()
	{
		if ( ! isset($this->Verpflegung))
		{
			$this->Verpflegung = false;
			$query = sql_query("SELECT Count(*) FROM " . TABLE_BUCHUNGSEINTRAEGE . " INNER JOIN " .
			TABLE_ARTIKEL . " ON id=F_Artikel_Nr WHERE F_Buchung_Nr=" .
			$this->Buchung_Nr . " AND F_Art_id=" . CVJMART_VERPFLEGUNG);
			if ($row = sql_fetch_row($query))
			{if ($row[0] > 0)
			{
				$this->Verpflegung = true;
			}
			}
			sql_free_result($query);
		}
		return $this->Verpflegung;
	}
	/**
	 * Stellt fest ob schon Verpflegung eingetragen ist.
	 * @return boolean true, wenn Verpflegung vorhanden ist, false sonst (auch bei Selbstverpflegung)
	 */
	function isVerpflegung()
	{
		$Verpflegung = false;
		if (!$this->isSelbstverpflegung())
		{
			$query = sql_query("SELECT Count(*) FROM " . TABLE_BUCHUNGSEINTRAEGE . " INNER JOIN " .
			TABLE_ARTIKEL . " ON id=F_Artikel_Nr WHERE F_Buchung_Nr=" .
			$this->Buchung_Nr . " AND F_Art_id=" . CVJMART_VERPFLEGUNG);
			if ($row = sql_fetch_row($query))
			if ($row[0] > 0)
			$Verpflegung = true;
			sql_free_result($query);
		}
		return $Verpflegung;
	}
	/**
	 * Bestimmt die Anzahl der Schlüssel, die bei einer Buchung ausgegeben wurden
	 * @return int die Anzahl der ausgegebenen Schlüssel
	 */
	function schluesselAnzahl()
	{
		$query2 = sql_query("SELECT SUM(Anzahl) FROM ".TABLE_SCHLUESSEL." WHERE F_Buchung_Nr=".
		$this->Buchung_Nr);
		$Schluessel = sql_fetch_row($query2);
		sql_free_result($query2);
		return $Schluessel[0];
	}
	/**
	 * Speichert die Artikel einer Buchung in der Tabelle für BuchungsEinträge
	 * @param docinput Feld mit den Artikel die gespeichert werden sollen
	 */
	function speichereArtikel($docinput)
	{
		if ($Speicherbar)
		{
			// Einträge speichern
			foreach ($this->Bezeichnung as $key => $value)
			{
				list ($Artikelnr, $datum) = explode(',', $key);
				list ($von, $bis) = explode('-', $datum);
				if ($bis == '')
				{
					$Datum = '='.$von;
				}
				else
				{
					$Datum = 'BETWEEN '.$von.' AND '.$bis;
				}
				$sql = "UPDATE " . TABLE_BUCHUNGSEINTRAEGE . " SET F_Bezeichnung='";
				$sql .= sql_real_escape_string($value) . "'";
				if (isset ($docinput["Dauer"][$key]))
				{
					$docinput["Dauer"][$key] = str_replace(",", ".", $docinput["Dauer"][$key]);
					if (is_numeric($docinput["Dauer"][$key]))
					{
						$sql .= ",Dauer=" . $docinput["Dauer"][$key];
					}
				}
				if (isset ($docinput["Menge"][$key]))
				{
					$docinput["Menge"][$key] = str_replace(",", ".", $docinput["Menge"][$key]);
					if (is_numeric($docinput["Menge"][$key]))
					{
						$sql .= ",Menge=" . $docinput["Menge"][$key];
					}
				}
				$sql .= " WHERE F_Artikel_Nr = $Artikelnr AND Datum $Datum AND ";
				$sql .= "F_Buchung_Nr = " . $this->Buchung_Nr;
				if (!sql_query($sql))
				{
					throw new Exception("Fehler $sql: " . sql_error());
				}
			}
			$this->logAction('Artikeldaten verändert');
		} // speichereArtikel
	}
	function entferneEintrag($Eintrag)
	{
		// Eintrag entfernen
		list ($Artikelnr, $Datum) = explode(',', $Eintrag);
		$this->entbucheArtikel($Artikelnr, $Datum, 0);
		$this->logAction('Artikel ' . $Artikelnr . ' für ' .date('d.m.Y', $Datum) .
		' gelöscht');
	}
	/**
	 * ändert die Menge eines vorhandenen Artikels um plus oder minus eins.
	 * @param int $Artikelnr die Artikelnummer
	 * @param char $Richtung P für plus, M für minus ein.
	 * @param int $Datum das Datum an dem der Artikel verändert werden soll
	 */
	function aendereMenge($Artikelnr, $Richtung, $Datum)
	{
		if ($Richtung == 'P')
		{
			$this->bucheArtikel($Artikelnr, $Datum);
		}
		elseif ($Richtung == 'M')
		{
			$this->entbucheArtikel($Artikelnr, $Datum);
		}
		$this->logAction('Artikel ' . $Artikelnr . ' für ' .date('d.m.Y', $Datum) .
		' verändert '.$Richtung);
	}
	/**
	 * Berechnet die erste gebuchte Mahlzeit der Buchung.
	 * @param String $feld Datum oder Bezeichnung
	 * @return array Datum und Bezeichnung der ersten gebuchten Mahlzeit, leeres Feld wenn nicht vorhanden
	 */
	function ersteMahlzeit($feld)
	{
		if ( ! isset($this->ersteMahlzeitFeld))
		{
			$sql = 'SELECT Bezeichnung, Datum FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' INNER JOIN ' .
			TABLE_ARTIKEL . ' ON F_Artikel_Nr=id WHERE F_Buchung_Nr=' . $this->Buchung_Nr . ' AND ' .
			'F_Art_id=' . CVJMART_VERPFLEGUNG . ' ORDER BY Datum, Position LIMIT 1';
			$query = sql_query($sql);
			if (!$this->ersteMahlzeitFeld = sql_fetch_array($query))
			{
				$this->ersteMahlzeitFeld = array();
			}
			sql_free_result($query);
		}
		if ( isset($this->ersteMahlzeitFeld[$feld]))
		{
			return $this->ersteMahlzeitFeld[$feld];
		}
		else
		{
			return '';
		}
	}
	/**
	 * Berechnet die letzte gebuchte Mahlzeit der Buchung
	 * @param String $feld Datum oder Bezeichnung
	 * @return array Datum und Bezeichnung der ersten gebuchten Mahlzeit, leeres Feld wenn nicht vorhanden
	 */
	function letzteMahlzeit($feld)
	{
		if ( ! isset($this->letzteMahlzeitFeld))
		{
			$query = sql_query('SELECT Bezeichnung, Datum FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' INNER JOIN ' .
			TABLE_ARTIKEL . ' ON F_Artikel_Nr=id WHERE F_Buchung_Nr=' . $this->Buchung_Nr . ' AND ' .
			'F_Art_id=' . CVJMART_VERPFLEGUNG . ' ORDER BY Datum DESC, Position DESC LIMIT 1');
			if (!$this->letzteMahlzeitFeld = sql_fetch_array($query))
			{
				$$this->letzteMahlzeitFeld = array();
			}
			sql_free_result($query);

		}
		if ( isset($this->letzteMahlzeitFeld[$feld]))
		{
			return $this->letzteMahlzeitFeld[$feld];
		}
		else
		{
			return '';
		}

	}
	/**
	 * ergibt die Bereiche (Artikel oberster Ebene) einer Art der Buchung als Zeichenkette
	 * (Kommasepariert)
	 * @param int $Art die ID der Art, deren direkte Unterebene aufgef�hrt werden soll
	 * @return String kommaseparierte Liste der Unterartikel der angegebenen Art
	 */
	function bereicheAlsListe($Art)
	{
		// Zeigt die oberste Kategorie einer Art / Buchung
		$Bereiche = array();
		$sql = 'SELECT id FROM '.TABLE_BUCHUNGSEINTRAEGE.
		' INNER JOIN '.TABLE_ARTIKEL.
		' ON F_Artikel_Nr = id WHERE F_Buchung_Nr = '.$this->Buchung_Nr.
		' AND F_Art_id ='.$Art;
		if ( ! $query = sql_query($sql))
		{
			throw new Exception('Fehler Zeigebereiche: '.sql_error());
		}
		while ( $artikel = sql_fetch_array($query) )
		{
			$Artikel = new Artikel($artikel['id']);
			$p = $Artikel->getArtikelPfad(9999);
			if ( ! in_array($p[0], $Bereiche))
			{
				$Bereiche[] = $p[0];
			}
		}
		sql_free_result($query);
		return implode(',',$Bereiche);
	}
	/**
	 * zeigt die doppelten Artikel aus der Fehlerliste an. Dabei werden die Einträge aus dem Feld gelöscht,
	 * damit sie nicht mehrfach ausgegeben werden.
	 */
	function getFehlerliste()
	{
		$f = $this->Fehlerliste; 
		$this->Fehlerliste = array();
		return $f;
	}
	/**
	 * prüft ob ein Artikel am angebenen Datum in einer anderen Buchung vorkommt
	 * @param int $artikel_id die ID des Artikel
	 * @param int $datum der timestamp des Datums
	 * @param int $dauer Dauer
	 * @return array ein Feld mit den Informationen zu den doppelten Artikeln
	 */
	function pruefeArtikelVorhanden($artikel_id, $datum, $dauer=0)
	{
		$dauer = str_replace(',','.',$dauer);
		if ( ! is_numeric($dauer))
		{
			$dauer = 0;	
		}
		if ( ! is_numeric($datum))
		{
			throw new Exception('pruefeArtikelVorhanden: falsches Datumformat ('.$datum.')');
		}
		$sql = 'SELECT F_Buchung_Nr, Menge,Bezeichnung,Datum FROM '.
		TABLE_BUCHUNGSEINTRAEGE.
		' INNER JOIN '.TABLE_ARTIKEL.' ON F_Artikel_Nr=id '.
		'WHERE (F_Art_id='.CVJMART_ORT.' OR F_Art_id='.CVJMART_PROGRAMM.
		' OR F_Art_id='.CVJMART_PROGRAMMPAKET.') '.
		' AND F_Buchung_Nr<>'.$this->Buchung_Nr.' AND F_Artikel_Nr='.$artikel_id.
		' AND (Datum BETWEEN '.$datum.' AND '.($datum+$dauer).
		' OR (Datum+Dauer) BETWEEN '.$datum.' AND '.($datum+$dauer).')';
		if ( ! $query = sql_query($sql))
		{
			throw new Exception('Fehler pruefeArtikelVorhanden: '.sql_error());
		}
		//$liste = array();
		while ( $artikel = sql_fetch_array($query) )
		{
			$s = array('Bezeichnung'=>$artikel['Bezeichnung'], 'Datum'=>$datum, 
			'Buchung_Nr'=>$artikel['F_Buchung_Nr'], 'Menge'=>$artikel['Menge']);		
			$this->Fehlerliste[] = $s;
		}
		sql_free_result($query);
		return $this->Fehlerliste;
	}
	/**
	 * Bucht einen Artikel zur Buchung und ergänzt alle zugehörigen Artikel der Gruppe
	 * Wenn die Menge des zuzubuchenden Artikels 0 beträgt wird er nur ein einziges Mal
	 * zugebucht.
	 * Um mehrere Tage auf einmal zu buchen, Datum in Form von Anfangsdatum-Enddatum angeben
	 * @param int $artikel_id die ID des Artikels
	 * @param int/String $datum das Datum als Timestamp oder Timestamp-Timestamp als von-bis
	 * @param double $menge die Menge
	 * @param double $dauer die Dauer
	 * @param int $ebene die Ebene des Artikels für rekursives Einbuchen. Maximal 10 Ebenen werden gebucht.
	 * @param boolean $unberechnet gibt an, ob der Artikel berechnet werden soll oder nicht	 
	 */
	function bucheArtikel($artikel_id, $datum, $menge=1, $dauer=0, $unberechnet=false, $ebene=0)
	{
		// Absicherung: Keine Endlosschleifen bei rekursiver Einbuchung
		if ( $ebene > 10) {
			return;
		}
		if ( ! $unberechnet )
		{
			$unberechnet = 0;
		}
		else
		{
			$unberechnet = 1;
		}
		if ( ! is_numeric($datum) )
		{
			$Datum = explode('-',$datum);
			if ( is_array($Datum) )
			{
				for ( $i = $Datum[0]; $i <= $Datum[1]; $i = strtotime('+1 day',$i))
				{
					$this->bucheArtikel($artikel_id, $i, $menge, $dauer, $unberechnet, $ebene);
				}
			}
		}
		else
		{
			$menge = str_replace(',','.',$menge);
			$dauer = str_replace(',','.',$dauer);
			if ( ! is_numeric($dauer) )
			{
				$dauer = 0;
			}
			if ( ! is_numeric($menge) )
			{
				$menge = 1;
			}
			// TODO: Die Liste auswerten!				
			$this->pruefeArtikelVorhanden($artikel_id, $datum, $dauer);
			if ( ! sql_query('INSERT INTO '.TABLE_BUCHUNGSEINTRAEGE.
			' (F_Buchung_Nr, F_Artikel_Nr, Menge,Dauer,Datum,Unberechnet) VALUES ('.
			$this->Buchung_Nr.','.$artikel_id.','.$menge.','.$dauer.','.$datum.','.$unberechnet.')') )
			{
				$sql = "UPDATE ".TABLE_BUCHUNGSEINTRAEGE.
				' SET Menge=Menge+'.$menge.', Dauer='.$dauer.', Unberechnet='.$unberechnet.
				' WHERE F_Buchung_Nr='.$this->Buchung_Nr.' AND F_Artikel_Nr='.$artikel_id.' AND Datum='.$datum;
				if ( ! sql_query($sql))
				{
					throw new Exception('bucheArtikel: '.$sql.'/'.sql_error());
				}
			}
			// Artikelgruppen zubuchen
			$query = sql_query("SELECT F_Unterartikel_id, Menge, Dauer, Beginn FROM ".TABLE_ARTIKELGRUPPEN.
			' WHERE F_Artikel_id='.$artikel_id);
			while ( $art = sql_fetch_array($query) )
			{
				// Wenn die vorangegangene Abfrage scheitert, ist vermutlich bereits ein
				// Datensatz vorhanden. Dann muss die Menge angepasst werden.
				if ( $art["Menge"] == 0 )
				{
					// prüfen, ob schon vorhanden
					if ( ! $q = sql_query("SELECT Count(F_Artikel_Nr) FROM ".TABLE_BUCHUNGSEINTRAEGE.
					" WHERE F_Buchung_Nr=".$this->Buchung_Nr." AND F_Artikel_Nr=".$artikel_id) )
					{
						throw new Exception ('bucheArtikel: '.sql_error());
					}
					$meineMenge = 0;
					if ( $row = sql_fetch_row($q) )
					{
						if ( $row[0] == 1 )
						{
							$meineMenge = 1;
							// Derartige Pauschalen sollen am n�chsten Tag gebucht werden, wenn nicht
							// anders angegeben (weil sonst im "0"-Bereich der frei-Pauschalen)
							if ( $art["Beginn"] == 0 )
							{
								$art["Beginn"] = 86400;
							}
						}
					}
					sql_free_result($q);
				}
				else
				{
					$meineMenge = $menge * $art["Menge"];
				}
				if ( $meineMenge != 0 )
				{
					if ( $art["Beginn"] != 0 )
					{
						$ndatum = $datum + $art["Beginn"];
					}
					else
					{
						$ndatum = $datum;
					}
					if ( $art["Dauer"] != 0 )
					{
						$ndauer = $art["Dauer"];
					}
					else
					{
						$ndauer = $dauer;
					}
					$ndauer = str_replace(',','.',$ndauer);			
					$this->pruefeArtikelVorhanden($art["F_Unterartikel_id"], $ndatum, $ndauer);
					// TODO: prüfen der Auswertungsliste
					// Rekursives Einbuchen der Artikel. geändert 07.12.06
					$this->bucheArtikel($art['F_Unterartikel_id'], $ndatum, $meineMenge, $ndauer, $unberechnet, $ebene+1);
				}
			}
			sql_free_result($query);
			// Einträge mit Menge 0 löschen
			sql_query('DELETE FROM '.TABLE_BUCHUNGSEINTRAEGE.' WHERE F_Buchung_Nr='.$this->Buchung_Nr.' AND Menge=0');
			if ( $ebene == 0)
			{
				$this->logAction('Artikel ' . $artikel_id . ' für ' .	date('d.m.Y', $datum) . ' gebucht ('.$menge.')');
			}
		} // Datum nicht numerisch
	}

	// Bucht einen Artikel zur Buchung und ergänzt alle zugehörigen Artikel der Gruppe
	// Wenn die Menge des zuzubuchenden Artikels 0 beträgt wird er komplett gelöscht
	function entbucheArtikel($artikel_id, $datum, $menge=1, $ebene=0)
	{
		// Absicherung: Keine Endlosschleifen bei rekursiver Einbuchung
		if ( $ebene > 10 ) {
			return;
		}
		if ( ! is_numeric($datum) )
		{
			$Datum = explode("-",$datum);
			if ( is_array($Datum) )
			{
				$query = sql_query("SELECT Datum FROM ".TABLE_BUCHUNGSEINTRAEGE.
				" WHERE Datum BETWEEN ".$Datum[0]." AND ".strtotime("+23 hours 59 minutes",$Datum[1])." AND F_Artikel_Nr=".$artikel_id.
				" AND F_Buchung_Nr=".$this->Buchung_Nr);
				while ( $b = sql_fetch_row($query) )
				{
					$this->entbucheArtikel($artikel_id, $b[0], $menge);
				}
				sql_free_result($query);
			}
		}
		else
		{
			if ( $this->isNeu() || ! is_numeric($artikel_id) ||! is_numeric($datum) ||
			!is_numeric($menge) )
			{
				throw new Exception('Fehler: Keine Zahl in entbucheArtikel: '.$artikel_id.'/'.$datum.'/'.$menge.'!');
			}
			if ( $menge == 0)
			{
				$query = sql_query("SELECT Datum, Menge FROM ".TABLE_BUCHUNGSEINTRAEGE.
				" WHERE F_Artikel_Nr=$artikel_id AND F_Buchung_Nr=$this->Buchung_Nr AND Datum=$datum");
				if ($menge = sql_fetch_row($query) )
				{
					$menge = $menge[1];
				}
				else
				{
					$menge = 0;
				}
				sql_free_result($query);
			}
			sql_query("UPDATE ".TABLE_BUCHUNGSEINTRAEGE.
			" SET Menge=Menge-$menge ".
			"WHERE F_Buchung_Nr=$this->Buchung_Nr AND F_Artikel_Nr=$artikel_id AND Datum=$datum");
			// Artikelgruppen entbuchen
			$query = sql_query("SELECT F_Unterartikel_id, Menge, Dauer, Beginn FROM ".TABLE_ARTIKELGRUPPEN.
			" WHERE F_Artikel_id=$artikel_id");
			while ( $art = sql_fetch_array($query) )
			{
				if ( $art['Menge'] == 0 )
				{
					// prüfen, ob schon vorhanden
					if ( ! $q = sql_query("SELECT Count(F_Artikel_Nr) FROM ".TABLE_BUCHUNGSEINTRAEGE.
					" WHERE F_Buchung_Nr=".$this->Buchung_Nr." AND F_Artikel_Nr=".$artikel_id.
					" AND Menge>0") )
					{
						throw new Exception('entbucheArtikel: '.sql_error());
					}
					$meineMenge = 0;
					if ( $row = sql_fetch_row($q) )
					{
						if ( $row[0] < 1 )
						{
							// Nachschauen, an welchem Datum entfernt werden muss
							$lquery = sql_query("SELECT Datum FROM ".TABLE_BUCHUNGSEINTRAEGE.
							" WHERE F_Artikel_Nr=".$art["F_Unterartikel_id"]." AND F_Buchung_Nr=$buchung_nr");
							if ($artikel = sql_fetch_row($lquery) )
							{
								$loeschDatum = $artikel[0];
							}
							else
							{
								$loeschDatum = $datum;
							}
							sql_free_result($lquery);
							$meineMenge = 1;
						}
					}
					else
					{
						$loeschDatum = 0;
					}
					sql_free_result($q);
				}
				else
				{
					$meineMenge = $menge * $art['Menge'];
					$loeschDatum = $datum;
				}
				if ( $art['Beginn'] != 0 )
				{
					$loeschDatum = $loeschDatum + $art['Beginn'];
				}
				if ( $meineMenge != 0 )
				{
					// Rekursiv ausbuchen
					$this->entbucheArtikel($art['F_Unterartikel_id'], $loeschDatum, $meineMenge, $ebene+1);
				}
			} // für alle Unterartikel
			sql_free_result($query);
			// Einträge mit Menge 0 löschen
			sql_query('DELETE FROM '.TABLE_BUCHUNGSEINTRAEGE.
			' WHERE F_Buchung_Nr='.$this->Buchung_Nr.' AND Menge=0');
			if ( $ebene == 0)
			{
				$this->logAction('Artikel ' . $artikel_id . ' für ' .	date('d.m.Y', $datum) . ' entfernt ('.$menge.')');
			}
		} // Datum nicht numerisch
	}
	/**
	 * liefert ein assoziatives Feld mit den Namen der Institutionen.
	 * Zusätzlich gibt es das Feld "keine".
	 * @return array ein Feld assoziatives Feld mit den ID->Namen der Institutionen
	 */
	function getInstitutionsliste()
	{
		// Institution angeben
		if ( ! isset($this->Institutionen))
		{
			$this->Institutionen = array();
			if (Count($this->Adresse->getInstitutionen())!=0)
			{
				$this->Institutionen[-1] = '-keine-';
			}
			foreach ( $this->Adresse->getInstitutionen() as $institution)
			{
				$this->Institutionen[$institution->Adressen_id] = $institution->Vollname();
			}
		}
		return $this->Institutionen;
	}
	/**
	 * liefert ein Feld mit den BuchungsEinträgen
	 * @param array die BuchungsEinträge
	 */
	function getBuchungsEintraege()
	{
		if ( ! isset($this->Buchungseintraege))
		{
		 $this->Buchungseintraege = array();
		 // Sind BuchungsEinträge vorhanden?
		 $q = sql_query("SELECT * FROM " . TABLE_BUCHUNGSEINTRAEGE . " WHERE F_Buchung_Nr=" . $this->Buchung_Nr);
		 while ($anz = sql_fetch_row($q))
		 {
		 	$this->Buchungseintraege[] = $anz;
		 }
		 sql_free_result($q);
		}
		return $this->Buchungseintraege;
	}
	/**
	 * ergibt eine Liste der Bemerkungen zu der Buchung
	 * @return array Feld mit den Bemerkungen
	 */
	function getBemerkungen()
	{
		if ( !isset($this->Bemerkungen))
		{
			$this->Bemerkungen = array();
			$query = sql_query("SELECT * FROM " . TABLE_BUCHUNGSBEMERKUNGEN .
			" WHERE F_Buchung_Nr = " . $this->Buchung_Nr . " ORDER BY Bemerkung_id DESC");
			if (sql_num_rows($query) > 0)
			{
				while ($bemerkung = sql_fetch_array($query))
				{
					$this->Bemerkungen[] = $bemerkung;
				}
			}
			sql_free_result($query);
		}
		return $this->Bemerkungen;
	}
	/**
	 *  ergibt die Gesamtanzahl der Schlafplätze (0 ... )
	 * @param int $datum das Datum ab dem die Schlafplätze berechnet werden sollen, 0 wenn alle
	 * @param String $auswahlID Kommaseparierte ID-Liste der Orte, deren Schlafplätze berechnet werden sollen
	 * @return int die Anzahl der Schlafplätze, 0 oder mehr
	 */
	function BerechneAlleSchlafplaetze($datum=0, $auswahlID='')
	{
		$sql = 'SELECT DISTINCT F_Artikel_Nr FROM '.TABLE_BUCHUNGSEINTRAEGE;
		$sql .= ' WHERE F_Buchung_Nr= '.$this->Buchung_Nr;
		if ($datum > 0)
		{
			$sql .= ' AND Datum='.$datum;
		}
		if ( $auswahlID != '' )
		{
			$sql .= ' AND F_Artikel_Nr IN ('.$auswahlID.')';
		}
		$query = sql_query($sql);
		$Plaetze = -1;
		while ( $row = sql_fetch_row($query) )
		{
			$a = new Artikel($row[0]);
			$p = $a->BerechneSchlafplaetze();
			if ( $p >= 0 )
			{
				if ( $Plaetze < 0 ) $Plaetze = 0;
				$Plaetze += $p;
			}
		}
		sql_free_result($query);
		return $Plaetze;
	}
	/**
	 * liefert ein Feld mit den Artikeln, deren Bezeichnung auf den Suchbegriff passt.
	 * @param String $search der Suchbegriff
	 * @return array Feld mit den Artikeln, die auf den Suchbegriff passen
	 */
	function getArtikelSuche($search= '')
	{
		if ( !isset($this->ArtikelSuche))
		{
			$this->ArtikelSuche = Artikel::getArtikelSuche($search);
		}
		return $this->ArtikelSuche;
	}
	/**
	 * liefert eine Liste von Adressen der Seminarteilnehmer
	 *	@return Adresse[] Liste der Seminarteilnehmer als Adressenobjekte
	 */
	function seminarteilnehmer()
	{
		$sql = '';
		$query = sql_query('SELECT F_Adressen_id FROM '.TABLE_SEMINARTEILNEHMER.' WHERE F_Buchung_Nr='.$this->Buchung_Nr);
		$Teilnehmer = array();
		while ($result = sql_fetch_array($query))
		{
			$Teilnehmer[] = new Adresse($result['F_Adressen_id']);
		}
		sql_free_result($query);
		return $Teilnehmer;
	}
	function seminarDaten()
	{
		if ($this->Seminar)
		{
			$query = sql_query('SELECT * FROM '.TABLE_SEMINARE.' WHERE F_Buchung_Nr='.$this->Buchung_Nr);
			$result = sql_fetch_array($query);
			sql_free_result($query);
			return $result;		
		}
		else
		{
			$a = array();
			return $a;		
		}
	}
	function zeigeArtikel($Art)
	{
		$Bereiche = array();
		$sql = 'SELECT DISTINCT id, Bezeichnung FROM '.TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.TABLE_ARTIKEL.
    	' ON F_Artikel_Nr = id WHERE F_Buchung_Nr = '.$this->Buchung_Nr.' AND F_Art_id ='.$Art;
  		if ( ! $query = sql_query($sql)) 
  		{
  			throw new Exception('Fehler ZeigeArtikel: '.sql_error());
  		}
  		while ( $artikel = sql_fetch_array($query) )
  		{
    		$Bereiche[] = $artikel['Bezeichnung'];
  		}
  		sql_free_result($query);
  		return implode(', ',$Bereiche);
	}
	function getOrte()
	{		
		return $this->zeigeArtikel(CVJMART_ORT);
	}
	/**
	 * @return true, wenn kein Programm oder alle Programm mit Personal versehen sind, false sonst.
	 */
	function programmPersonalVorhanden()
	{
		$sql = 'SELECT Datum, F_Artikel_Nr FROM '.TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.TABLE_ARTIKEL.
    	' ON F_Artikel_Nr = id WHERE F_Buchung_Nr = '.$this->Buchung_Nr.' AND F_Art_id ='.CVJMART_PROGRAMM;
  		if ( ! $query = sql_query($sql)) 
  		{
  			throw new Exception ("Fehler programmVorhanden: ".sql_error());
  		}
  		$ergebnis = true;
  		while ( ($result = sql_fetch_array($query)) && $ergebnis)
  		{
  			$events = Event::searchBuchungEvent($result['Datum'], $result['Datum'], $this->Buchung_Nr, $result['F_Artikel_Nr']);
			if ( count($events) == 0 )
			{
				$ergebnis = false;
			}
		}	
		sql_free_result($query);
		return $ergebnis;			
	}
	function programmVorhanden()
	{
  		$sql = 'SELECT Position FROM '.TABLE_BUCHUNGSEINTRAEGE.' INNER JOIN '.TABLE_ARTIKEL.
    	' ON F_Artikel_Nr = id WHERE F_Buchung_Nr = '.$this->Buchung_Nr.' AND (F_Art_id ='.CVJMART_PROGRAMM.' OR F_Art_id='.CVJMART_PROGRAMMPAKET.')';
  		if ( ! $query = sql_query($sql)) 
  		{
  			throw new Exception ("Fehler programmVorhanden: ".sql_error());
  		}
  		$i = sql_num_rows($query);
  		return $i > 0;
	}
	function buchungURL()
	{
		return get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.$this->Buchung_Nr);
	}
	function buchungOrteURL()
	{
		return get_url(get_cmodule_id('cvjmbuchungsuebersicht'),'Buchung_Nr='.$this->Buchung_Nr.'&docinput[Art]='.CVJMART_ORT);
	}
	function buchungVerpflegungURL()
	{
		return get_url(get_cmodule_id('cvjmbuchungsuebersicht'),'Buchung_Nr='.$this->Buchung_Nr.'&docinput[Art]='.CVJMART_VERPFLEGUNG);
	}
	function buchungProgrammURL()
	{
		return get_url(get_cmodule_id('cvjmbuchungsuebersicht'),'Buchung_Nr='.$this->Buchung_Nr.'&docinput[Art]='.CVJMART_PROGRAMM);
	}
	function buchungSmilycardURL()
	{
		return get_url(get_cmodule_id('cvjmsmilycard'),'Buchung_Nr='.$this->Buchung_Nr);
	}
	function readSmilycard()
	{
		$sql = 'SELECT * FROM cvjm_Smilycard WHERE Buchung_Nr='.$this->Buchung_Nr;
		$query = sql_query($sql);
		$row = sql_fetch_array($query);
		sql_free_result($query);
		return $row;
	}
	function saveSmilycard($input)
	{
		$sql = 'UPDATE cvjm_Smilycard SET ';
		$parts = array();
		foreach ( $input as $key => $value)
		{
				switch ( $key )
				{
				case 'Unterbringung':
				case 'Herkunft':
				case 'Programm':
					if ( is_array($value))
					{
						if ($key == 'Programm' && $input['ProgrammAndere'] != '' )
						{
							$value[] = 'A';
						}
						if ($key == 'Herkunft' && $input['HerkunftSonstige'] != '' )
						{
							$value[] = 'S';
						}
						if ($key == 'Herkunft' && $input['HerkunftPublikation'] != '' )
						{
							$value[] = 'C';
						}
						$value = implode(',', $value);
					}
					break;
				case 'invalid':
					$value = 1;
			}
			$parts[] = $key.'=\''.sql_real_escape_string($value).'\'';
		}
		if (! isset($input['invalid']))
		{
			$parts[] = 'invalid=0';
			$parts[] = 'Historie=CONCAT(Historie,\''."\n".date('d.m.Y').': '.get_user_nickname().' speichern\')';	
		
		}
		else
		{
			$parts[] = 'Historie=CONCAT(Historie,\''."\n".date('d.m.Y').': '.get_user_nickname().' Karte als nicht abgegeben markiert\')';	
		}
		$sql .= implode(',', $parts).' WHERE Buchung_Nr='.$this->Buchung_Nr;
		if ( ! sql_query($sql) )
		{
			throw new Exception('Smilycard konnte nicht gespeichert werden - '.sql_error().' Statement:'.$sql);
		}
		// Neueinlesen erzwingen 
		$this->Smilycard = null;
	}
	function getSmilycard()
	{
		if ( $this->Smilycard == null )
		{
			$row = $this->readSmilycard();
			if ( ! $row )
			{
				// 	Vorbelegungen für Unterbringung
				$bereiche = $this->bereicheAlsListe(CVJMART_ORT); // hole Belegungen
				// no smilycard yet - create one
				$sql = 'INSERT INTO cvjm_Smilycard (Buchung_Nr, Unterbringung, Datum, Historie) VALUES ('.
				$this->Buchung_Nr.',\''.$bereiche.'\',CURRENT_DATE,\'Anlage '.date('d.m.Y H:i').' '.
				get_user_nickname().'\')';
				sql_query($sql);
				$row = $this->readSmilycard();
				if ( ! $row )
				{
					throw new Exception('Konnte keine neue Smilycard anlegen');
				}
			}
			// Aufbereitung für Anzeige
			$row['Unterbringung'] = explode(',', $row['Unterbringung']);
			$row['Herkunft'] = explode(',', $row['Herkunft']);
			$row['Programm'] = explode(',', $row['Programm']);
			$this->Smilycard = $row;
		}
		return $this->Smilycard;
	}
	// -----------------------------------------------------------------------------
	/**
	 * liefert ein assoziatives Feld mit den Namen und IDs der vorhandenen Altersgruppen
	 * @return array ein Feld ID->Name der Altersgruppen
	 */
	static function getAltersgruppen()
	{
		// Altergruppen für die Altersangabe
		$query = sql_query('SELECT * FROM '.TABLE_ALTERSGRUPPEN.' ORDER BY Reihenfolge');
		while ( $anrede = sql_fetch_array($query) )
		{
			$Altersgruppen[$anrede['Altersgruppe_id']] = $anrede['Altersgruppe'];
		}
		sql_free_result($query);
		return $Altersgruppen;
	}
} // class Buchung
?>
