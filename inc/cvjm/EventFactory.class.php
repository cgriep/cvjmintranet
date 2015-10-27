<?php

include_once(INC_PATH.'cvjm/Event.class.php');
include_once(INC_PATH.'cvjm/Auftrag.class.php');

class EventFactory 
{
	/**
	 * liefert einen Event der angegebenen Art aus der Datenbank
	 * @param int $event_id die ID des Events der geladen werden soll
	 * @return Event der Event
	 */
	static function getEvent($event_id)
	{
		$sql = 'SELECT Art FROM '.TABLE_EVENTS.' WHERE Event_id='.$event_id;
		$query = mysql_query($sql);
		if ( ! $Event = mysql_fetch_array($query))
		{
				throw new Exception('EventFactory: Konnte ID '.$event_id.' nicht laden!');
		}
		mysql_free_result($query);
		switch ( $Event[0] )
		{
			case EVENT_ART_TERMIN:
				return new Event($event_id);
			case EVENT_ART_AUFTRAG:
				return new Auftrag($event_id);
			default:
				throw new Exception("getEvent: unbekannter Eventtyp ".$Event[0]);
		}
	}
	/**
	 * Erstellt einen neuen Event der angegebenen Art.
	 * @param int $event_art die Art des zu erzeugenden Events 
	 * @return Event der erzeugte Event
	 */
	static function createEvent($event_art)
	{
		switch ($event_art)
		{
			case EVENT_ART_TERMIN:
				return new Event();
			case EVENT_ART_AUFTRAG:
				return new Auftrag();
			default:
				throw new Exception('EventFactory: Falsche Eventart '.$event_art);
		}
	}
}
?>