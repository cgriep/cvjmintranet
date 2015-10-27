<?php
require_once(INC_PATH.'cvjm/DBEntity.class.php');
/**
 * Beschreibt einen Event aus der Datenbank
 *
 */
DEFINE('TABLE_EVENTS','cvjm_Events');
DEFINE('TABLE_EVENT_BETROFFENE','cvjm_Event_Betroffene');

// Turnusarten f�r den Kalender
define('EVENT_TURNUS_EINMALIG', 100);
define('EVENT_TURNUS_TAEGLICH', 101);
define('EVENT_TURNUS_WOECHENTLICH', 102);
define('EVENT_TURNUS_14TAEGIG', 103);
define('EVENT_TURNUS_MONATLICH', 104);
define('EVENT_TURNUS_JAEHRLICH', 105);
define('EVENT_TURNUS_ZWEIMONATLICH', 106);

// Referenzen 
define('EVENT_REFERENZ_BUCHUNG', 'BuNr');
define('EVENT_REFERENZ_ARTIKEL', 'ArtNr');
// Statusarten f�r Auftr�ge
define('EVENT_STATUS_OFFEN',1);
define('EVENT_STATUS_INARBEIT', 2);
define('EVENT_STATUS_UNTERBRECHUNG', 3);
define('EVENT_STATUS_ERLEDIGT', 4);

define('EVENT_PRIORITAET_SOFORT', 1);
define('EVENT_PRIORITAET_DRINGEND', 2);
define('EVENT_PRIORITAET_WICHTIG', 3);
define('EVENT_PRIORITAET_NORMAL', 4);
define('EVENT_PRIORITAET_HATZEIT', 5);

// Arten
define('EVENT_ART_TERMIN', 0);

define('EVENT_BETROFFENER_USER', 0);
define('EVENT_BETROFFENER_GRUPPE', 1);
define('EVENT_BETROFFENER_EXTERN', 2);

// Best�tigungsstatus
define('EVENT_BESTAETIGUNG_KEINE', 0);
define('EVENT_BESTAETIGUNG_ANGEFORDERT', 1);
define('EVENT_BESTAETIGUNG_GESENDET', 2);
define('EVENT_BESTAETIGUNG_GELESEN', 3);
define('EVENT_BESTAETIGUNG_BESTAETIGT', 4);
define('EVENT_BESTAETIGUNG_ZUSAGE',5);
define('EVENT_BESTAETIGUNG_ZUGESAGT', 6);
define('EVENT_BESTAETIGUNG_ABGESAGT', 7);

class Event extends DBEntity
{
	static $Tage = array( 'S', 'M', 'D', 'M', 'D', 'F', 'S');
	static $Monate = array('Januar', 'Februar', 'M�rz', 'April',
	'Mai', 'Juni', 'Juli', 'August', 'September',
	'Oktober', 'November', 'Dezember');

	static $Prioritaeten = array(
	EVENT_PRIORITAET_SOFORT => 'sofort',
	EVENT_PRIORITAET_DRINGEND => 'dringend',
	EVENT_PRIORITAET_WICHTIG => 'wichtig',
	EVENT_PRIORITAET_NORMAL => 'normal',
	EVENT_PRIORITAET_HATZEIT => 'hat Zeit'
    );
	static $Farben = array(
	EVENT_PRIORITAET_SOFORT => 'style="background-color:#CC0066;color:white"',
	EVENT_PRIORITAET_DRINGEND => 'style="background-color:#FF8080"',
	EVENT_PRIORITAET_WICHTIG => 'style="background-color:#FF9966"', //#993366
	EVENT_PRIORITAET_NORMAL => '',
	EVENT_PRIORITAET_HATZEIT => ''
    );
	static $Statusse = array(
	EVENT_STATUS_OFFEN => 'offen',
	EVENT_STATUS_INARBEIT => 'in Arbeit',
	EVENT_STATUS_UNTERBRECHUNG => 'Unterbrechung',
	EVENT_STATUS_ERLEDIGT => 'erledigt'
    );
	static $Turnusse = array(
	EVENT_TURNUS_EINMALIG => '-einmalig-',
	EVENT_TURNUS_TAEGLICH => 't�glich',
	EVENT_TURNUS_WOECHENTLICH => 'w�chentlich',
	EVENT_TURNUS_14TAEGIG => '14t�gig',
	EVENT_TURNUS_MONATLICH => 'monatlich',
	EVENT_TURNUS_ZWEIMONATLICH => 'zweimonatlich',
	EVENT_TURNUS_JAERHLICH => 'j�hrlich'
	);
	function __construct($event_id = -1)
	{
		parent::__construct(TABLE_EVENTS);
		if ( ! is_numeric($event_id))
		{
			throw new Exception('Event: Ung�ltige id: '.$event_id.'!');
		}
		if ( $event_id > 0 )
		{
			$sql = 'SELECT * FROM '.TABLE_EVENTS.' WHERE Event_id='.$event_id;
			$query = mysql_query($sql);
			if ( ! $Event = mysql_fetch_array($query))
			{
				throw new Exception('Event: Konnte id '.$event_id.' nicht laden!');
			}
			mysql_free_result($query);
			$this->uebertrageFelder($Event);
		}
		else
		{
			// Vorbelegungen
			$this->Event_id = -1;
			$this->Autor = SESSION_DBID;
			$this->Art = EVENT_ART_TERMIN;
			$this->Datum = time();
			$this->Dauer = 0;
			$this->Created = time();
			$this->Changed = date('Y-m-d H:i:s', $this->Created);
			// TODO $this->Aendern = false;
			$this->TurnusEnde = 0;
			$this->Prioritaet = EVENT_PRIORITAET_NORMAL;
			$this->Turnus = EVENT_TURNUS_EINMALIG;
			$this->Status = EVENT_STATUS_NORMAL;
			$this->Ort = -1;
			$this->BearbeitenBis = 0;
			$this->Betroffene = array();
		}
		$this->holeBetroffene();
	}
	function getPrioritaet()
	{
		// Namen f�r Priorit�ten und Status
		if ( isset(Event::$Prioritaeten[$this->Prioritaet]))
		{
			return Event::$Prioritaeten[$this->Prioritaet];
		}
		else
		{
			return '('.$this->Prioritaet.')';
		}
	}
	function getStatusse()
	{
		return Event::$Statusse;
	}
	function getPrioritaeten()
	{
		return Event::$Prioritaeten;
	}
	function getPrioritaetsFarbe()
	{
		return Event::$Farben[$this->Prioritaet];
	}
	function uebertrageFelder($felder)
	{
		parent::uebertrageFelder($felder);
		// Besonderheit: Checkboxen ber�cksichtigen
		//TODO
		foreach ( array('Anzeigen','Rabattfaehig','Geringwertig', 'Steuerpflicht', 'PersonJN') as $Feld)
		{
			if ( ! isset($felder[$Feld]))
			{
				$this->$Feld = false;
			}
		}
	}
	/**
	 * �bertr�gt einen Termin aus dem alten AWF-Kalendersystem
	 * @param array $termin der Termin in der Form wie er aus der Datenbank kommt
	 */
	function uebertrageTermin($termin)
	{
		$this->Datum = $termin['Datum'];
		//	$this->$nTermin['Enddatum'] = strtotime('+'.$termin->Dauer.'min',$termin->Datum);
		$this->Autor = $termin['author_id'];
		$this->Turnus = $termin['Turnus'];
		$this->Title = $termin['title'];
	}
	function save()
	{
		// Sicherheitspr�fungen
		foreach (array('Autor','Status','Ort','Datum','Dauer','Turnus','TurnusEnde','Art') as $feld)
		{
			if (!is_numeric($this->$feld))
			{
				$this->$feld= 0;
			}
		}
		if ( $this->isNeu())
		{
			$sql = 'INSERT INTO '.TABLE_EVENTS.' (Titel, Beschreibung, Bemerkung, Status, Prioritaet,';
			$sql .= ' Autor,Ort,Datum,Turnus,TurnusEnde,Referenz,Art,created,Dauer,BearbeitenBis) VALUES (';
			$sql .= '"'.mysql_real_escape_string($this->Titel).'",';
			$sql .= '"'.mysql_real_escape_string($this->Beschreibung).'",';
			$sql .= '"'.mysql_real_escape_string($this->Bemerkung).'",';
			$sql .= $this->Status.',';
			$sql .= $this->Prioritaet.',';
			$sql .= $this->Autor.',';
			$sql .= $this->Ort.',';
			$sql .= $this->Datum.',';
			$sql .= $this->Turnus.',';
			$sql .= $this->TurnusEnde.',';
			$sql .= '"'.mysql_real_escape_string($this->Referenz).'",';
			$sql .= $this->Art.',';
			$sql .= time().',';
			$sql .= $this->Dauer.',';
			$sql .= $this->BearbeitenBis;
			$sql .= ')';
			if ( ! mysql_query($sql))
			{
				throw new Exception('Event Insert: '.mysql_error());
			}
			else
			{
				$this->Event_id = mysql_insert_id();
			}
		}
		else
		{
			$sql = 'UPDATE '.TABLE_EVENTS.' SET ';
			$sql .= 'Titel="'.mysql_real_escape_string($this->Titel).'",';
			$sql .= 'Beschreibung="'.mysql_real_escape_string($this->Beschreibung).'",';
			$sql .= 'Bemerkung="'.mysql_real_escape_string($this->Bemerkung).'",';
			$sql .= 'Status='.$this->Status.',';
			$sql .= 'Prioritaet='.$this->Prioritaet.',';
			$sql .= 'Autor='.$this->Autor.',';
			$sql .= 'Ort='.$this->Ort.',';
			$sql .= 'Datum='.$this->Datum.',';
			$sql .= 'Dauer='.$this->Dauer.',';
			$sql .= 'Turnus='.$this->Turnus.',';
			$sql .= 'TurnusEnde='.$this->TurnusEnde.',';
			$sql .= 'BearbeitenBis='.$this->BearbeitenBis.',';
			$sql .= 'Art='.$this->Art;
			$sql .= ' WHERE Event_id='.$this->Event_id;
			if ( ! mysql_query($sql))
			{
				throw new Exception('Event Update: '.mysql_error());
			}
		}
		$this->schreibeBetroffene();
	}
	function loeschen()
	{
		if ( ! $this->isNeu() )
		{
			sql_query('DELETE FROM '.TABLE_EVENT_BETROFFENE.' WHERE F_Event_id='.$this->Event_id);
			sql_query('DELETE FROM '.TABLE_EVENTS.' WHERE Event_id=' . $this->Event_id);
		}
	}
	/**
	 * zeigt an ob der Datensatz neu ist.
	 * @return boolean
	 */
	function isNeu()
	{
		if ($this->Event_id <= 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	 * liefert den Beschreibungsstring mit Ersetzungen f�r Systemevents (Querverweise zu Buchung etc.)
	 * @return string der String in HTML-Form mit Querverweisen
	 */
	function getBeschreibung()
	{

	}
	/**
	 * liefert eine Zeichenkette mit der �bersicht �ber den Event
	 * @return string eine HTML-Zeichenkette mit der �bersicht
	 */
	function getUebersicht()
	{
		$s = '';
		$s = $this->Autor;
		$s = $this->Dauer;
		/*
		 $this->Turnus
		 $this->Prioritaet
		 $this->Art
		 $this->TurnusEnde

		 $this->Ort
		 */
	}
	/**
	 * liefert eine Zeichenkette mit der Art des Termins. Muss f�r erbende Objekte entsprechend
	 * �berladen werden.
	 * @return string Zeichenkette mit den Namen der Eventart
	 */
	function getArt()
	{
		if ($this->Art == EVENT_ART_TERMIN)
		{
			return 'Termin';
		}
		else
		{
			return 'unbekannt';
		}
	}
	function getStatus()
	{
		return Event::$Statusse[$this->Status];
	}
	function getTurnus()
	{
		return Event::$Turnusse[$this->Turnus];
	}
	/**
	 * zeigt an, ob der Eintrag nach der Erstellung noch mindestens einmal ge�ndert wurde
	 * @return true, wenn eine �nderung vorgenommen wurde
	 */
	function wurdeGeaendert()
	{
		if (strtotime('Y-m-d H:i:s', $this->created) != $this->changed && ! $this->isNeu() ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	 * liefert den Namen des Autors 
	 * @return string der Name des Autors
	 */
	function getAutor()
	{
		return get_user_nickname($this->Autor);
	}
	function istAutor($autor)
	{
		return $autor = $this->Autor;
	}
	function darfSchreiben()
	{
		return ($this->istBetroffen() && $this->Aendern) || $this->Autor == SESSION_DBID;
	}
	/**
	 * pr�ft AWT-Spezifische User- und Gruppenberechtigungen
	 * @return true, wenn der Benutzer betroffen ist, false sonst
	 */
	function istBetroffen($Betroffen_id = SESSION_DBID)
	{
		global $profile;
		if ( $Betroffen_id == SESSION_DBID)
		{
			$p= $profile;	
		}
		else
		{
			$p = get_profile($Betroffen_id);
		}
		foreach ( $this->Betroffene as $betroffen)
		{
			switch ($betroffen['Betroffener_Art'])
			{
				case EVENT_BETROFFENER_USER:
					if ( $Betroffen_id == $betroffen['F_Betroffener_id'])
					{
						return true;
					}
					break;
				case EVENT_BETROFFENER_GRUPPE:
					if ( $p['group_'.$betroffen['F_Betroffener_id']] == 1 )
					{
						return true;
					}
					break;
				case EVENT_BETROFFENER_EXTERN:
					break;
			}
		}
		return false;
	}
	/*
	 * Sucht die Betroffenen eines Termins
	 */
	function holeBetroffene()
	{
		$this->Betroffene = array();
		$query = mysql_query('SELECT * FROM '.TABLE_EVENT_BETROFFENE.
		' WHERE F_Event_id='.$this->Event_id);
		while ( $row = sql_fetch_array($query))
		{
			$this->Betroffene[] = $row;
		}
		mysql_free_result($query);
	}
	/*
	 * Schreibt die Betroffenen eines Termins in die Betroffenen-Tabelle
	 */
	function schreibeBetroffene()
	{
		$sql = 'DELETE FROM '.TABLE_EVENT_BETROFFENE.' WHERE F_Event_id='.$this->Event_id;
		if (! sql_query($sql))
		{
			throw new Exception('Event.schreibeBetroffene: '.mysql_error());
		}
		foreach ($this->Betroffene as $betroffener)
		{
			if ( ! is_numeric($betroffener['F_Betroffener_id']))
			{
				$betroffener['F_Betroffener_id'] = -1;
			}
			if ( ! is_numeric($betroffener['Erinnerungsstatus']))
			{
				$betroffener['Erinnerungsstatus'] = 0;
			}
			if ( ! is_numeric($betroffener['Bestaetigungsstatus']))
			{
				$betroffener['Bestaetigungsstatus'] = EVENT_BESTAETIGUNG_KEINE;
			}
			if ( ! is_numeric($betroffener['Betroffener_Art']))
			{
				$betroffener['Betroffener_Art'] = EVENT_BETROFFENER_EXTERN;
			}
			if ( $betroffener['Betroffener'] == '' )
			{
				$betroffener['Betroffener'] = 'NULL';
			}
			else
			{
				$betroffener['Betroffener'] = '"'.mysql_real_escape_string($betroffener['Betroffener']).'"';
			}
			$sql = 'INSERT INTO '.TABLE_EVENT_BETROFFENE.
			' (F_Event_id,F_Betroffener_id, Betroffener_Art, Betroffener,Bestaetigungsstatus,'.
			'Erinnerungsstatus) '.
			' VALUES ('.
			$this->Event_id.','.$betroffener['F_Betroffener_id'].','.
			$betroffener['Betroffener_Art'].','.
			$betroffener['Betroffener'].','.
			$betroffener['Bestaetigungsstatus'].','.
			$betroffener['Erinnerungsstatus'].')';
			if (! sql_query($sql))
			{
				throw new Exception('Event-Betroffene: '.$sql.'/'.mysql_error());
			}
		}
	}
	/**
	 * liefert die Betroffenen als Feld mit Indexwerten, nutzbar im Frontend bezogen auf 
	 * getListOfUsersAndGroups
	 * @return array Liste mit ID und gID der Betroffenen
	 */
	function getBetroffene()
	{
		$ergebnis = array();
		foreach ( $this->Betroffene as $value)
		{
			if ( $value['Betroffener_Art'] == EVENT_BETROFFENER_USER )
			{
				$ergebnis[] = $value['F_Betroffener_id'];
			}
			else if ( $value['Betroffener_Art'] == EVENT_BETROFFENER_USER )
			{
				$ergebnis[] = 'g'.$value['F_Betroffener_id'];
			}
		}
		return $ergebnis;
	}
	/**
	 * liefert die Betroffenen als Zeichenkette 
	 */
	function getBetroffenenName($nr)
	{
		if ( isset($this->Betroffene[$nr]))
		{
			 switch ($this->Betroffene[$nr]['Betroffener_Art'])
			 {
			 	case EVENT_BETROFFENER_USER:
					return get_user_nickname($this->Betroffene[$nr]['F_Betroffener_id']);
				case EVENT_BETROFFENER_GRUPPE:
					global $groups;
					init_groups();
					return 'Gruppe '.$groups[$this->Betroffene[$nr]['F_Betroffener_id']];
				case EVENT_BETROFFENER_EXTERN:
					return $this->Betroffene[$nr]['Betroffener'];
			 }
			 return '-?-';
		}
		else
		{
			return '';
		}
	}
	function sendBestaetigung()
	{
		
	}
	function getOrt($mitPfad = false)
	{
		if ( is_numeric($this->Ort) && $this->Ort > 0 )
		{
			return Ortsname($this->Ort, $mitPfad);
		}
		else
		{
			return '';
		}
	}
	/**
	 * setzt eine Nachricht zusammen die per Mail gesendet wird.
	 * Muss f�r sp�teer Klassen abgeleitet werden.
	 * @param string $Text ein zus�tzlicher Text, der vor den eigentlichen Daten angezeigt wird
	 * @return string der Nachrichtentext
	 */
	function getMessageText($Text='')
	{
		if ( $Text != '' )
		{
			$Text .= "\n\n";
		}
		$Text = 'Von: '.get_user_email($this->Autor).' ('.get_user_nickname($this->Autor).')'.
		"\nDatum: ".date('d.m.Y H:i', $this->Datum)."\n\n".
		$this->Beschreibung;
		$Ort = $this->getOrt(false);
		if ( $Ort != '' )
		{
			$Text .= "\nOrt: ".$Ort;
		}
		$Text .= "\nPriorit�t: ".$this->getPrioritaet();
		
		if (Count($this->Betroffene) > 0)
		{
	    	$Text .= "\nZugewiesen an: ";
	    	for ( $i=0; $i < Count($this->Betroffene); $i++)
	    	{
	    		$Text .= $this->getBetroffenenName($nr).', ';
	    	}
			$Text = substr($Text, 0, strlen($Text)-2);
		}
		if ( $this->BearbeitenBis != 0 ) 
  		{
  			$Text .= "\n".'Erledigen bis '.date('d.m.Y H:i', $this->BearbeitenBis);
  		}
		$Text .= "\n\n".$this->Bemerkung;
		return $Text;
	}
	/**
	 * Benachrichtigt alle Betroffenen 
	 * @param string $zusatztext der Text, der zus�tzlich zu den Rahmendaten angezeigt wird
	 */
	function sendBenachrichtigung($zusatztext='')
	{
		// alle betroffenen user_id feststellen
		$Betroffene = array();
		foreach ( $this->Betroffene as $betroffen)
		{
			switch ($betroffen['Betroffener_Art'])
			{
				case EVENT_BETROFFENER_USER:
					if ( SESSION_DBID != $betroffen['F_Betroffener_id'])
					{
						if ( ! in_array($betroffen['F_Betroffene_id'], $Betroffene))
						{
							$Betroffene[] = $betroffen['F_Betroffener_id'];
						}
					}
					break;
				case EVENT_BETROFFENER_GRUPPE:							
					$sql2 = "SELECT DISTINCT user_id FROM ".TABLE_USERDATA.
					" WHERE name='group_".$betroffen['F_Betroffener_id']."'";
					$qu = sql_query($sql2);
					while ( $row = sql_fetch_row($qu) ){
						if ( $row[0] != SESSION_DBID && 
						! in_array($row[0], $Betroffene))
						{
							$Betroffene[] = $row[0];	
						}
					}
					mysql_free_result($query);
					break;
				case EVENT_BETROFFENER_EXTERN:
					break;
			}
		}
		// mail senden 
		foreach ($Betroffene as $user_id)
		{
			$this->sendeMail($user_id, $zusatztext);
		}
	}
	/**
	 * sendet eine Mail mit der Benachrichtigung �ber den Event an den angegebenen User
	 * @param int $user_id die ID des zu benachrichtigenden Benutzers
	 * @param string $zusatztext ein zus�tzlicher Text, der vor den Ereignisdaten angezeigt wird
	 * @return boolean true wenn die Mail erfolgreich abgesendet wurde, false sonst
	 */
	function sendeMail($user_id, $zusatztext = '')
	{
		return mail(get_user_email($user_id),
				$this->getArt().' '.$this->Titel,
				$this->getMessageText($zusatztext), 				
				'From: '.get_user_nickname($this->Autor).' <'.get_user_email($this->Autor).'>');
	}
	/**
	 * Liefert eine Liste aus Gruppen und Benutzern von AWF. Der Key ist bei Gruppen mit einem f�hrenden
	 * g versehen (g13 f�r Gruppe 13). Bei Benutzern wird nur eine Zahl verwendet.
	 * @return array Liste der Benutzer und Gruppen
	 */
	function getListOfGroupsAndUsers()
	{
		global $groups;
		$liste = array();
		foreach ( $groups as $key => $value )
		{
			$liste['g'.$key] = $value;
		}
		$qres = sql_query('SELECT '.TABLE_USERS.'.id, value FROM '.TABLE_USERS.' INNER JOIN '.TABLE_USERDATA.
		' ON '.TABLE_USERS.".id = user_id WHERE valid = 1 AND name='nickname' ORDER BY value");
		while ( $row = sql_fetch_array($qres) ) 
		{
			$liste[$row['id']] = $row['value'];
		}
		mysql_free_result($qres);
		return $liste;	
	}
	
	// ----------------------------------------------------------------------------------

	static function sqlWhereDate($Datum)
	{
		// stellen wir sicher, dass das Datum ein Tagesbeginn (0:00) ist
		$Datum = strtotime(date('Y-m-d',$Datum));
		return '(Datum<'.strtotime('+1day',$Datum).
		' AND DATE_ADD(FROM_UNIXTIME(Datum),INTERVAL Dauer MINUTE)>=FROM_UNIXTIME('.$Datum.'))';
	}
	static function sqlWhereDates($StartDatum, $EndDatum)
	{
		// stellen wir sicher, dass das Datum ein Tagesbeginn (0:00) ist
		$StartDatum = strtotime(date('Y-m-d',$StartDatum));
		$EndDatum = strtotime(date('Y-m-d',strtotime('+1day',$EndDatum)));
		return '(Datum >= '.$StartDatum.' AND Datum<'.$EndDatum.')';
	}
	static function sqlWhereReferenz($Referenz)
	{
		$RefString = implode('/%" AND Referenz LIKE "%', $Referenz);
		return '(Referenz LIKE "%'.$RefString.'/%")';
	}
	
	/**
	 * Liefert ein Feld von Events, die im angegebenen Datum f�r den aktuellen Benutzer aktiv sind.
	 * @param date $Datum das Datum an dem die Events gesucht werden
	 * @param date $EndDatum das Enddatum eines Bereichs in dem Events gesucht werden sollen
	 * @return array Feld der Events, null wenn keine vorhanden
	 */
	static function searchEvents($Datum)
	{
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE '.Event::sqlWhereDate($Datum).
		' ORDER BY Datum';
		$ergebnis = array();
		if ( ! $query = mysql_query($sql))
		{
			throw new Exception('searchEvents: '.mysql_error());
		}
		while ($row = mysql_fetch_array($query))
		{
			$event = new Event($row['Event_id']);
			if ( $event->istBetroffen() )
			{
				$ergebnis[] = $event;
			}
		}
		mysql_free_result($query);
		// Turnusevents hinzuf�gen
		// Monatlich - Tag muss �bereinstimmen, Monat + Jahr egal 
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE Turnus='.EVENT_TURNUS_MONATLICH.' AND (TurnusEnde=0 OR TurnusEnde>='.$Datum.')';
		$sql .= ' AND Datum<='.$Datum;
		$sql .= ' AND DAYOFMONTH(FROM_UNIXTIME(Datum))=DAYOFMONTH(FROM_UNIXTIME('.$Datum.')) ORDER BY Datum';
		if ( ! $query = mysql_query($sql))
		{
			throw new Exception('searchEvents: '.mysql_error().' / '.$sql);
		}
		while ($row = mysql_fetch_array($query))
		{
			$event = new Event($row['Event_id']);
			if ( $event->istBetroffen() )
			{
				$event['Datum'] = mktime(date('h',$event['Datum']),date('i',$event['Datum']), date('s',$event['Datum']),
				date('m',$Datum), date('d',$Datum), date('Y',$Datum));
				$ergebnis[] = $event;
			}
		}
		mysql_free_result($query);		
		// W�chentlich - Wochentag muss gleich sein  
		$sql = ' SELECT Event_id FROM '.TABLE_EVENTS.' WHERE Turnus='.EVENT_TURNUS_WOECHENTLICH.' AND (TurnusEnde=0 OR TurnusEnde>='.$Datum.')';
		$sql .= ' AND Datum<='.$Datum;
		$sql .= ' AND DAYOFWEEK(FROM_UNIXTIME(Datum))=DAYOFWEEK(FROM_UNIXTIME('.$Datum.')) ORDER BY Datum';
		if ( ! $query = mysql_query($sql))
		{
			throw new Exception('searchEvents: '.mysql_error().' / '.$sql);
		}
		while ($row = mysql_fetch_array($query))
		{
			$event = new Event($row['Event_id']);
			if ( $event->istBetroffen() )
			{
				$event['Datum'] = mktime(date('h',$event['Datum']),date('i',$event['Datum']), date('s',$event['Datum']),
				date('m',$Datum), date('d',$Datum), date('Y',$Datum));
				$ergebnis[] = $event;
			}
		}
		mysql_free_result($query);
		
		// t�glich alles egal 
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE Turnus='.EVENT_TURNUS_TAEGLICH.' AND (TurnusEnde=0 OR TurnusEnde>='.$Datum.')';		
		$sql .= ' AND Datum<='.$Datum.' ORDER BY Datum';
		if ( ! $query = mysql_query($sql))
		{
			throw new Exception('searchEvents: '.mysql_error().' / '.$sql);
		}
		while ($row = mysql_fetch_array($query))
		{
			$event = new Event($row['Event_id']);
			if ( $event->istBetroffen() )
			{
				$event['Datum'] = mktime(date('h',$event['Datum']),date('i',$event['Datum']), date('s',$event['Datum']),
				date('m',$Datum), date('d',$Datum), date('Y',$Datum));
				$ergebnis[] = $event;
			}
		}
		mysql_free_result($query);
		
		// 14t�gig - Tag muss �bereinstimmen, Kalenderwoche/2 muss gleiche Zahl ergeben  
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE Turnus='.EVENT_TURNUS_14TAEGIG;
		$sql .= ' AND Datum<='.$Datum.' AND (TurnusEnde=0 OR TurnusEnde>='.$Datum.')';
		$sql .= ' AND DAY(FROM_UNIXTIME(Datum))=DAY(FROM_UNIXTIME('.$Datum.')) ';
		$sql .= ' AND WEEKOFYEAR(FROM_UNIXTIME(Datum))%2=WEEKOFYEAR(FROM_UNIXTIME('.$Datum.'))%2 ORDER BY Datum';
		if ( ! $query = mysql_query($sql))
		{
			throw new Exception('searchEvents: '.mysql_error().' / '.$sql);
		}
		while ($row = mysql_fetch_array($query))
		{
			$event = new Event($row['Event_id']);
			if ( $event->istBetroffen() )
			{
				$event['Datum'] = mktime(date('h',$event['Datum']),date('i',$event['Datum']), date('s',$event['Datum']),
				date('m',$Datum), date('d',$Datum), date('Y',$Datum));
				$ergebnis[] = $event;
			}
		}
		mysql_free_result($query);
		
		// Monatlich - Tag muss �bereinstimmen, Jahr egal 
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE Turnus='.EVENT_TURNUS_JAEHRLICH.' AND (TurnusEnde=0 OR TurnusEnde>='.$Datum.')';
		$sql .= ' AND Datum<='.$Datum;
		$sql .= ' AND DAY(FROM_UNIXTIME(Datum))=DAY(FROM_UNIXTIME('.$Datum.'))';
		$sql .= ' AND MONTH(FROM_UNIXTIME(Datum))=MONTH(FROM_UNIXTIME('.$Datum.')) ORDER BY Datum';
		if ( ! $query = mysql_query($sql))
		{
			throw new Exception('searchEvents: '.mysql_error().' / '.$sql);
		}
		while ($row = mysql_fetch_array($query))
		{
			$event = new Event($row['Event_id']);
			if ( $event->istBetroffen() )
			{
				$event['Datum'] = mktime(date('h',$event['Datum']),date('i',$event['Datum']), date('s',$event['Datum']),
				date('m',$Datum), date('d',$Datum), date('Y',$Datum));
				$ergebnis[] = $event;
			}
		}
		mysql_free_result($query);
				
		// Zweimonatlich - Tag muss �bereinstimmen, Monat/2 muss gleiche Zahl ergeben
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE Turnus='.EVENT_TURNUS_ZWEIMONATLICH.' AND (TurnusEnde=0 OR TurnusEnde>='.$Datum.')';
		$sql .= ' AND Datum<='.$Datum;
		$sql .= ' AND DAY(FROM_UNIXTIME(Datum))=DAY(FROM_UNIXTIME('.$Datum.'))';
		$sql .= ' AND MONTH(FROM_UNIXTIME(Datum))%2=MONTH(FROM_UNIXTIME('.$Datum.'))%2 ORDER BY Datum';		
		
		if ( ! $query = mysql_query($sql))
		{
			throw new Exception('searchEvents: '.mysql_error().' / '.$sql);
		}
		while ($row = mysql_fetch_array($query))
		{
			$event = new Event($row['Event_id']);
			if ( $event->istBetroffen() )
			{
				$event['Datum'] = mktime(date('h',$event['Datum']),date('i',$event['Datum']), date('s',$event['Datum']),
				date('m',$Datum), date('d',$Datum), date('Y',$Datum));
				$ergebnis[] = $event;
			}
		}
		mysql_free_result($query);
		return $ergebnis;
	}
	/**
	 * sendet Best�tigungs-Mails f�r Events wo dies angefordert wurde und setzt dann die den Status entsprechend
	 */
	static function sendeBestaetigungsMails()
	{
		$sql = 'SELECT Betroffene_id, F_Event_id, F_Betroffener_id FROM '.TABLE_EVENT_BETROFFENE.
		' WHERE Bestaetigungsstatus='.EVENT_BESTAETIGUNG_ANGEFORDERT;
		$query = mysql_query($sql);
		while ( $zeile = mysql_fetch_row($query))
		{
			$Event = new Event($zeile[1]);
			//echo "Sende Mail f�r Event ".$zeile[1]." an ".$zeile[2]."\n";
			$Event->sendeMail($zeile[2], 'Bitte beachten Sie das folgende Ereignis:');
			// Status �ndern
			mysql_query('UPDATE '.TABLE_EVENT_BETROFFENE.' SET Bestaetigungsstatus='.EVENT_BESTAETIGUNG_GESENDET.
			' WHERE Betroffene_id='.$zeile[0]);
		}
		mysql_free_result($query);
	}
	static function searchEventErinnerung()
	{
		$sql = 'SELECT Betroffene_id, F_Event_id, F_Betroffener_id FROM '.TABLE_EVENT_BETROFFENE.
		' INNER JOIN '.TABLE_EVENT.' ON F_Event_id=Event_id '.
		'WHERE Erinnerungsstatus>0 AND DATE_SUB(FROM_UNIXTIME(DATUM),INTERVAL Erinnerungsstatus DAY) <= CURDATE()';
		$query = mysql_query($sql);
		while ( $zeile = mysql_fetch_row($query))
		{
			$Event = new Event($zeile[1]);
			$Event->sendeMail($zeile[2]);
			// Status �ndern
			mysql_query('UPDATE '.TABLE_EVENT_BETROFFENE.
			' SET Erinnerungsstatus=-Erinnerungsstatus '.
			' WHERE Betroffene_id='.$zeile[0]);
		}
		mysql_free_result($query);
	}
	/**
	 * sucht nach Events zu einer Referenz. 
	 * @param array $Referenz ein Feld mit den Referenzen nach denen gesucht werden soll
	 * @param int $StartDatum das Anfangsdatum
	 * @param int $Enddatum das Enddatum
	 * @return array ein Feld von Events 
	 */
	static function searchReferenz($Referenz, $StartDatum=-1, $EndDatum = -1)
	{
		$RefString = Event::sqlWhereReferenz($Referenz);
		if ( $StartDatum > 0)
		{
			if ( $EndDatum < 0)
			{
				$EndDatum = $StartDatum; 
			}
			$RefString .= ' AND '.Event::sqlWhereDates($StartDatum, $EndDatum);
		}
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE '.$RefString;
		
		$query = mysql_query($sql);
		$Events = array();
		while ( $zeile = mysql_fetch_row($query))
		{
			$Events[] = new Event($zeile[0]);
		}
		mysql_free_result($query);
		return $Events;
	}
	static function searchBuchungEvent($StartDatum, $EndDatum, $Buchung_Nr, $Artikel_Nr =-1)
	{
		// Referenz zusammenbauen
		$Referenz = array();
		$Referenz[] = EVENT_REFERENZ_BUCHUNG.$Buchung_Nr;
		if ( $Artikel_Nr > 0)
		{
			$Referenz[] =EVENT_REFERENZ_ARTIKEL.$Artikel_Nr;
		}
		return Event::searchReferenz($Referenz, $StartDatum, $EndDatum);
	}
}
?>