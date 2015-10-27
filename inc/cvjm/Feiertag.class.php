<?php
require_once(INC_PATH.'cvjm/DBEntity.class.php');
/* 

*/

class Feiertag extends DBEntity {
	static $feiertage = array();
	function __construct($feiertag_id = -1)
	{
		parent::__construct(TABLE_FEIERTAGE);
		if ( ! is_numeric($feiertag_id))
		{
			throw new Exception('Ungültige Feiertag-id: '.$feiertag_id.'!');
		}
		if ( $feiertag_id > 0 )
		{
			$sql = 'SELECT * FROM '.TABLE_FEIERTAGE.' WHERE id='.$feiertag_id;
			$query = mysql_query($sql);
			if ( ! $Artikel = mysql_fetch_array($query))
			{
				throw new Exception('Konnte Feiertag '.$feiertag_id.' nicht laden!');
			}
			mysql_free_result($query);
			$this->uebertrageFelder($Artikel);
			$this->Feiertag_id = $this->id;
		}
		else
		{
			// Vorbelegungen
			$this->Feiertag_id = -1;
		}
	}
	function isNeu()
	{
		return $this->Feiertag_id < 0;
	}
	function save()
	{
		// TODO		
	}
	function loeschen()
	{
		
	}
	// ----------------------------------------------------------------
	static function getFeiertage()
	{
		
	}
	static function isFeiertag($datum)
	{
		if ( isset(Feiertag::$feiertage[$datum]))
		{
			return Feiertag::$feiertage[$datum];
		}
		if ( is_numeric($datum))
		{
			$sql = 'SELECT Count(*) FROM '.TABLE_FEIERTAGE.' WHERE Datum='.$datum;
			$query = mysql_query($sql);
			$row = mysql_fetch_row($query);
			mysql_free_result($query);
			Feiertag::$feiertage[$datum] = ($row[0]>0);
			return ($row[0] > 0);
		}
		else
		{
			throw new Exception('isFeiertag: falsches Datum - '.$datum);
		}
	}
} // Klasse Feiertag
?>