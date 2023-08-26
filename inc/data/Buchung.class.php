<?php
require_once(INC_PATH.'cvjm/DBEntity.class.php');
require_once(INC_PATH.'cvjm/Adresse.class.php');
require_once(INC_PATH.'cvjm/Artikel.class.php');
/**
 * Beschreibt eine Buchung in der Datenbank
 */
class Buchung extends DBEntity
{
	/**
	 * der Kunde der Buchung
	 */
	var $Adresse;
	// Liste mit den Buchungen, die Artikel enthalten, die beim letzten Buchen hinzugef�gt wurden
	var $Fehlerliste = array();

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
				//throw new Exception('Keine g�ltige Adressen_id bei neuer Buchung angegeben!');
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
			$this->OriginalBuchung_Nr = -1;
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
			$this->OriginalBuchung_Nr = $this->Buchung_Nr;
		}
	} // Buchung erstellen
	/**
	 * �ndert den Kunden auf die angegebene Adresse. Der Datensatz wird nicht gespeichert!
	 * @param int $Adressen_id die ID der Kundenadresse
	 */
	function KundeAendern($Adressen_id)
	{
		if ( is_numeric($Adressen_id))
		{
			$a = new Adresse($Adressen_id);
			// Wichtig: Format beachten, da Frontend Bezug darauf nimmt
			$this->logAction('Kunde von KuNr '.$this->Adresse->getKundennummer().' auf '.$a->getKundennummer().' ge�ndert.', false);
			$this->F_Adressen_id = $Adressen_id;
			$this->Adresse = $a;
		}
		else
		{
			throw new Exception ('KundeAendern: '.$Adressen_id.' ist keine g�ltige Angabe.');
		}
	}
	/**
	 * liefert ein Feld mit den Kundennummern der bisherigen Kunden. Enth�lt mindestens die aktuelle 
	 * Kundennummer.
	 * @return array Feld mit den Kundennummern bisheriger Kunden. 
	 */
	function bisherigeKunden()
	{
		// auf Kundenwechsel pr�fen
		return array();
		$kunden =array($this->Adresse->Kunden_Nr);
		$text = $this->Logtext;
		while ( $i = strpos($text, ' KuNr '))
		{
			$KuNr = trim(substr($text, $i+6));
			$KuNr = substr($KuNr,0,strpos($KuNr,' '));
			$text = substr($text, strpos($KuNr, ' '));
			if ( ! is_numeric($KuNr)) 
			{
				throw new Exception('Fehler KuNr: '.$KuNr);
			}
			echo $KuNr."/";
			if ( $KuNr != $this->Adresse->Kunden_Nr)
			{
			  	$kunden[] = $KuNr;
			}
		}
		return $kunden;
	}
	/**
	 * liefert eine Zeichenkette mit der History, wobei Sonderfunktionen ber�cksichtigt sind. Die Zeichenkette
	 * enth�lt HTML-Querverweise.
	 * @param string $url die URL an die verwiesen wird. Sie wird um die Adressen_id erg�nzt und sollte mit "=" enden.
	 * @return string die History mit HTML-Verlinkungen
	 */
	function holeHistory($url)
	{
		$text = $this->Logtext;
		$LPos = 0;
		// Kundenwechsel 
		while ( $i = strpos($text, ' KuNr ', $LPos))
		{
			$KuNr = trim(substr($text, $i+6));
			$p = strpos($KuNr,' ');
			$KuNr = substr($KuNr,0,$p);
			$LPos = $i+6+$p; 
			try {
				$Kunde = Adresse::getKundePerKundennummer($KuNr);
				$neu = '<a href="'.$url.$Kunde->Adressen_id.'">'.$KuNr.'</a>';
				$text = substr_replace($text,$neu,$i+6,strlen($KuNr));
			} catch ( Exception $e )
			{
				// wenn Kundennummer nicht gefunden ...
			}
		}
		// TODO: Rechnung Nr: mit Link versehen
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
	 * @param boolean $save true, wenn die �nderung gespeichert werden soll, false sonst
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
	 * L�scht eine Buchung
	 */
	function loeschen()
	{
		// TODO!
	}
	/**
	 * Storniert die Buchung und entfernt alle Buchungseintr�ge. Diese werden unter Internes aufgef�hrt.
	 * @param boolean $nurLeeren true, wenn die Eintr�ge entfernt werden sollen, die Buchung aber unber�hrt bleibt, true wenn die Buchung storniert werden soll
	 */
	function stornieren($nurLeeren = false)
	{
		if ( $this->isNeu() || $this->isFertig() )
		{
			throw new Exception('stornieren: Die Buchung '.$this->Buchung_Nr.' ist neu oder fertig!');
		}
		// Eintr�ge in die internen Bemerkungen speichern
		$s .= 'Letzter Status: '.$this->Buchungsstatus();
		$s .= "\nStorniert am " . date('d.m.Y H:i') . "\nGebucht waren folgende Eintr�ge:\n";
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
			$this->logAction('Alle Eintr�ge ausgebucht');
		}
		// durch das Protokollieren wird die gesamte Buchung gespeichert

		// Alle Eintr�ge der Buchung l�schen
		sql_query('DELETE FROM ' . TABLE_BUCHUNGSEINTRAEGE . ' WHERE F_Buchung_Nr=' . $this->Buchung_Nr);
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
		'Eingang','BStatus','Kuechenhilfe') as $Feld)
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
		// Status pr�fen. Sobald ein Eingang vorliegt, ist es eine Reservierung
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
			// Pr�fen, ob sich die Personenanzahl ge�ndert hat.
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
					throw new Exception('Fehler bei Personen�nderung '.$sql.': ' . sql_error());
				}
				$this->logAction('Personenanzahl ge�ndert von '.$this-OriginalPersonenanzahl.' auf '.$this->personenAnzahl());
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
			// Pr�fen ob Speiseraum sich ge�ndert hat
			if ($this->F_Speiseraum_id != $this->OriginalSpeiseraum_id)
			{
				// Speiseraum setzen.
				$sql .= ',F_Speiseraum_id=' . $this->F_Speiseraum_id;
				// Wichtig: Buchung vollf�hren!
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
				$this->logAction('Speiseraum ge�ndert von '.$this->OriginalSpeiseraum_id.' nach '.$this->F_Speiseraum_id);
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
			// �nderung protokolieren
			$this->logAction('ge�ndert', false);
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
		// Originalinformation sichern f�r sp�tere Anpassungen
		$this->OriginalPersonenanzahl = $this->personenAnzahl();
		$this->OriginalSpeiseraum_id = $this->F_Speiseraum_id;
		$this->OriginalBuchung_Nr = $this->Buchung_Nr;
		$this->BuchungStand = time();
	}
	/**
	 * ergibt die Menge, die standardm��ig bei einer bestimmten Abrechnungsart angewendet werden soll.
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
	 * ergibt die Menge, die standardm��ig bei einer bestimmten Abrechnungsart angewendet werden soll.
	 * @param int $Abrechnungstyp der Abrechnungstyp
	 * @return int die Menge die eingesetzt werden soll
	 */
	function standardMengeBeiArtikel($Artikel_id)
	{
		$a = new Artikel($Artikel_id);
		return $this->standardMenge($a->getAbrechnungstyp());
	}
	/**
	 * gibt die Altersgruppe in lesbarer Form zur�ck
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
	 * gibt den Inhalt des Felder zur�ck. Hier k�nnen auch berechnete Inhalte
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
	 * @param int $nr die Nummer der gew�nschten Altersgruppe (von 1 bis 6)
	 * @return String der Name der Altersgruppe
	 */
	function getAlterswertName($nr)
	{
		switch ( $nr)
		{
			case 1: return 'unter 2';
			case 2: return '2-5';
			case 3: return '6-14';
			case 4: return '15-27';
			case 5: return '28-64';
			case 6: return 'ab 65';
			default:
				return '-unbekannt-';
		}
	}
	/**
	 *  gibt eine Liste mit den Anzahlen der Personen zur�ck
	 * @return array ein nummeriertes Feld f�r die Altersgruppen. Indizes sind m und w sowie Anzeige mit dem Namen der Altersgruppe
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
	 * liefert ein Feld der m�glichen Speiser�ume inkl. der Option "keiner". Das Feld ist mit der Artikel_id
	 * des Saals indiziert. Soweit der Saal durch eine andere Buchung belegt ist, wird deren Nummer hinter dem
	 * Namen des Saals angezeigt.
	 * @return array indiziertes Feld mit den Namen der Speises�le.
	 */
	function getSpeiseraumListe()
	{
		$Speiseraum = array ();
		$Speiseraum[-1] = '- unbekannt -';
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
	 * liefert ein Feld mit der Liste aller R�ume, die als Gruppenraum m�glich sind (also keine
	 * Schlafpl�tze haben). Zus�tzlich wird ein Eintrag f�r "keiner" angeboten.
	 * @returns array ein Feld id->Bezeichnung der Gruppenr�ume.
	 */
	function getGruppenraumListe()
	{
		$Gruppenraum = array ();
		$Gruppenraum[-1] = ' - keiner -';
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
	 * Bestimmt die Anzahl der �bernachtungen der Buchung
	 * @return die Anzahl der �bernachtungen in Tagen
	 */
	function berechneUebernachtungen()
	{
		return (($this->Bis-$this->Von) / 86400);
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
	 * Schaltet die Buchung zum Seminar bzw. wieder zur�ck
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
	 * Ver�ndert den Status der Buchung.
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
			case BUCHUNG_GELOESCHT: return 'Vorreservierung gel�scht';
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
	 * Entfernt eine Abrechnung der Buchung endg�ltig
	 * @param abrechnung_id die ID der zu l�schenden Abrechnung
	 */
	function loescheAbrechnung($abrechnung_id)
	{
		if (!$this->isNeu() && is_numeric($abrechnung_id))
		{
			sql_query('DELETE FROM ' . TABLE_RECHNUNGSEINTRAEGE . ' WHERE F_Rechnung_id=' .
			$abrechnung_id);
			sql_query('DELETE FROM ' . TABLE_RECHNUNGEN . ' WHERE Rechnung_id=' . $abrechnung_id);
			// wenn das die letzte Abrechnung war, dann die Anzahlung l�schen
			$query = sql_query('SELECT Count(*) FROM ' . TABLE_RECHNUNGEN . ' WHERE F_Buchung_Nr=' .
			$this->Buchung_Nr);
			if ($row = sql_fetch_row($query))
			if ($row[0] == 0 && $this->AnzahlungsBemerkung == '')
			{
				$this->Anzahlung = 0;
				sql_query('UPDATE ' . TABLE_BUCHUNGEN . ' SET Anzahlung=NULL