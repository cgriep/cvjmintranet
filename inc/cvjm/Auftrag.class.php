<?php

/* Modul Auftrag

�berarbeitung des Auftragsmoduls f�r Kalender
(unfertig)

*/

include_once(INC_PATH.'misc/CVJM.inc');
require_once(INC_PATH.'cvjm/Event.class.php');
// Neue Art
define('EVENT_ART_AUFTRAG', 1);

class Auftrag extends Event {

	function getArt()
	{
		if ($this->Art == EVENT_ART_AUFTRAG)
		{
			return 'Auftrag';
		}
		else
		{
			return parent::getArt();
		}
	}

	function __construct($event_id = -1)
	{
		parent::__construct($event_id);
		if ( $this->isNeu())
		{
		$this->Ende = 0;
		$this->Art = EVENT_ART_AUFTRAG;
		$this->Status = EVENT_STATUS_OFFEN;
		}
	}
	function save()
	{
		$neu = $this->isNeu();
		parent::save();
		// Mail an Betroffene schicken
		$ZusatzText = "";
		if ( SESSION_DBID != $this->Autor )
		{
			$ZusatzText = "Automatisch erstellt durch ".get_user_nickname(SESSION_DBID)."\n";
		}
		// Benachrichtigung
		switch ( $neu )
		{
			case true:
				$this->sendBenachrichtigung('Auftrag erteilt');
				break;
			case false:
				if ( $this->Status == EVENT_STATUS_ERLEDIGT)
				{
					// An auftraggeber senden
					$this->sendeMail($this->Autor, 'Auftrag erledigt!');
				}
				else
				{
					// an Auftragnehmer senden
					$this->sendBenachrichtigung('Auftrag ge�ndert');
				}
		}
	}
	/**
	 * �ndert den Status eines Auftrages
	 * @param int $Status der neue Status
	 */
	function neuerStatus($Status)
	{
		if ( is_numeric($Status) && isset(Event::$Statusse[$Status]))
		{
			$this->Bemerkung .= "\n".Event::$Statusse[$Status]." am ".date("d.m.Y H:i")." - ".get_user_nickname();		
			// Fertigstellungs-Datum speichern
			$this->Status = $Status;
			if ( $Status != EVENT_STATUS_ERLEDIGT )
			{
				$this->Bemerkung .= "\n".'Dauer bisher: ';
			}
			else
			{
				$this->Bemerkung .= "\n".'Dauer: ';
			}
			$this->Bemerkung .= round((time() - $this->Datum)/86400).' Tage';
			$this->save();
		}
		else
		{
			throw new Exception('neuerStatus: Falscher Status: '.$Status);
		}
	}
	/**
	 * sucht nach Auftr�gen eines Betroffenen.
	 * @param int $Betroffene die ID des Betroffenen
	 * @param boolean $Alle true, wenn alle (auch erledigte) Auftr�ge angezeigt werden sollen
	 * @param int $Ort der Ort zu dem Auftr�ge angezeigt werden sollen (-1 = alle)
	 * @param string $Sort die Sortierung
	 * @return array die Auftraege
	 */
	static function searchAuftraege($Betroffene, $Alle, $Ort=-1, $Sort=' Prioritaet,Datum DESC')
	{
		$Where = array();;
		if ( ! $Alle ) {
			$Where[] = 'Status < '.EVENT_STATUS_ERLEDIGT;
		}
		if ( is_numeric($Ort) && $Ort > 0 ) {
			$Where[] = 'Ort = '.$Ort;
		}
		$Where[] = 'Art='.EVENT_ART_AUFTRAG;
		$sql = 'SELECT Event_id FROM '.TABLE_EVENTS.' WHERE ('.implode(') AND (',$Where).') ORDER BY ';
		if ( $Alle )
		{
			$Sort = 'Datum DESC';
		}
		elseif ( $Sort == '')
			{
				$Sort = ' Prioritaet,Datum DESC';
			}
		$sql .= sql_real_escape_string($Sort).',Status';
		if ( $qu = sql_query($sql) ) {
			while ( $row = sql_fetch_row($qu)) {
				$Auftrag = new Auftrag($row[0]);
				if ( $Auftrag->istBetroffen($Betroffene))
				{
					$Auftraege[] = $Auftrag;
				}
			}
			sql_free_result($qu);
		}
		else
		{
			throw new Exception('Auftrag: '.sql_error());
		}
		return $Auftraege;
	}
} // Klasse Auftrag
?>