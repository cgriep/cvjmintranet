<?php
class AdressBemerkung
{
	var $Bemerkung_id;
	var $F_Adressen_id;
	var $Datum;
	var $Bemerkung;
	var $Wiedervorlage;
	var $Anhang;
	var $F_Buchung_Nr; 

	function AdressBemerkung($id = -1)
	{
		$this->Bemerkung_id = -1;
		$this->F_Adressen_id = -1;
		$this->F_Buchung_id = null;
		$this->Datum = 0;
		$this->Bemerkung = '';
		$this->Wiedervorlage = 0;
		$this->Anhang = '';
		if ( $id > 0)
		{
			// Datensatz laden 
			$query = sql_query('SELECT * FROM '.TABLE_ADRESSBEMERKUNGEN.' WHERE Bemerkung_id='.$id);
			if ( $daten = sql_fetch_array($query))
			{
				foreach ( $daten as $key => $value)
				  $this->$key = $value;
			}
			sql_free_result($query);
		}
	}
	function loescheAnhang()
	{
		if ($this->Anhang != '')
		{
			// Datei löschen
			$ext = pathinfo($this->Anhang);
			$datei = getAdressenAnhangLink($this->F_Adressen_id, $this->Bemerkung_id, $ext['extension']);
			if (!unlink($datei))
			{
			throw new Exception('Datei ' . $datei . ' konnte nicht gelöscht werden');
			}
			sql_query('UPDATE ' . TABLE_ADRESSENBEMERKUNGEN .
			' SET Anhang = NULL,Bemerkung=CONCAT(Bemerkung,\'Anhang gelöscht ' . date('d.m.Y H:i') . ' ' .
			get_user_nickname() . '\') WHERE Bemerkung_id=' . $this->Bemerkung_id);
		}
	}
	function setWiedervorlage($wert)
	{
		if (strpos($wert, '.') === false)
		if (is_numeric($wert))
		$this->Wiedervorlage = strtotime('+' . $wert . ' day');
		else
		$this->Wiedervorlage = 0;
		else
		{
			$this->Wiedervorlage = convertDatumToTimestamp($wert);
		}
	}
	function uploadAnhang()
	{
		if ($_FILES["Anhang"]["name"] != "")
		{
			// Upload eines Anhangs
			if (!is_dir(CVJM_ADRESSEN_DIR . "/" . $this->F_Adressen_id))
			{
			mkdirs(CVJM_ADRESSEN_DIR .
			"/" . $this->F_Adressen_id, CONFIG_CREATE_MASK);
			}
			$ext = pathinfo($_FILES["Anhang"]["name"]);
			move_uploaded_file($_FILES["Anhang"]["tmp_name"], CVJM_ADRESSEN_DIR . "/" .
			$this->F_Adressen_id . "/Bemerkung" . $this->Bemerkung_id . "." . $ext["extension"]);
		}
	}
	function save()
	{
		if ($this->Bemerkung_id > 0)
		{
			// Korrektur
			$sql = "UPDATE " . TABLE_ADRESSENBEMERKUNGEN . " SET Bemerkung='" .
			mysql_real_escape_string($this->Bemerkung) . "\ngeändert " . date("d.m.Y H:i") . " " .
			get_user_nickname() . "',Datum=$this->Datum,Wiedervorlage=$this->Wiedervorlage," .
			"Anhang='" . mysql_real_escape_string($this->Anhang) . "' WHERE Bemerkung_id=" . $this->Bemerkung_id;
		} else
		{
			// Neue Bemerkung
			$sql = "INSERT INTO " . TABLE_ADRESSENBEMERKUNGEN .
			" (F_Adressen_id,Bemerkung,Datum,Wiedervorlage,Anhang) VALUES (" .
			$this->F_Adressen_id . ",'" .
			mysql_real_escape_string($this->Bemerkung) . "\nerzeugt " . date("d.m.Y H:i") . " " .
			get_user_nickname() . "'," . $this->Datum . "," .
			$this->Wiedervorlage . ",'" . $this->Anhang . "')";
		}
		if (!sql_query($sql))
		echo mysql_error();
		if ($this->Bemerkung_id<0)
		$this->Bemerkung_id = sql_insert_id();
	}
} // Klasse AdressBemerkung
?>