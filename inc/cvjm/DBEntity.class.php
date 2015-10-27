<?php
/**
 * Abstrakte Grundklasse f�r Datenbankobjekte
 */
abstract class DBEntity
{
	/**
	 * Konstruktor. Erzeugt alle Felder aus der Datenbank
	 * @param String $tabelle der Name der Datenbanktabelle, aus der das Objekt erstellt werden soll
	 */
	function __construct($tabelle)
	{
		if ( ! $query = mysql_query('SHOW COLUMNS FROM '.$tabelle))
		{
			throw new Exception('Die Tabelle '.$tabelle.' konnte nicht ausgelesen werden!');
		}
		while ($feld = mysql_fetch_array($query))
		{
			// �bertragen der Felder in die Attribute des Klasse
			$name = $feld['Field'];
			$this->$name = $feld['Default'];
			if ( $feld['Default'] == null)
			{
				$this->$name= '';
			}
			else
			{
				$this->$name = $feld['Default'];
			}
		}
		mysql_free_result($query);
	}
	/**
	 * �bertr�gt alle Felder aus dem Datensatz ins Objekt
	 * @param array $datensatz ein assoziatives Feld mit den Feldnamen => Feldinhalt
	 */
	public function uebertrageFelder($datensatz)
	{
		// �bertragen der Felder in die Attribute des Klasse
		foreach ( $datensatz as $key => $value )
		{
			$this->$key = $value;
		}
	}
	abstract function save();
	abstract function loeschen();
	/**
	 * zeigt an ob der Datensatz neu ist.
	 * @return boolean 
	 */
	abstract function isNeu();	
}
?>