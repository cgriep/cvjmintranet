<?php
class AdressKategorie
{
	var $Kategorie;

	function AdressKategorie($Kategorie_id = -1)
	{
		$this->Kategorie['Kategorie_id'] = -1;
		if ( is_numeric($Kategorie_id) && $Kategorie_id > 0 )
		{
			$sql = 'SELECT * FROM '.TABLE_KATEGORIEN.' WHERE Kategorie_id='.$Kategorie_id;
			$query = sql_query($sql);
			if ( ! $this->Kategorie = sql_fetch_array($query))
			{
				throw new Exception('Konnte Kategorie '.$Kategorie_id.' nicht laden!');
			}
			sql_free_result($query);
		}
			
	}
	function save()
	{
		if ( $this->Kategorie['Kategorie_id'] <= 0)
		{
			if (sql_query('INSERT INTO ' . TABLE_KATEGORIEN . ' (Kategorie) VALUES ("' .
			sql_real_escape_string($this->Kategorie['Kategorie']) . '")'))
			{				
				$this->Kategorie['Kategorie_id'] = sql_insert_id();
			}
		}
		else
		{
			if (! sql_query('UPDATE '.TABLE_KATEGORIEN.' SET Kategorie="'.$this->Kategorie['Kategorie'].'" WHERE Kategorie_id='.$this->Kategorie['Kategorien_id']))
			{
				throw new Exception('Konnte Kategorie '.$this->Kategorien['Kategorie_id'].' nicht aktualisieren.');
			}
		}		
	}
	function setName($name)
	{
		if ( $name!= '' )
		{
			$this->Kategorie['Kategorie'] = $name;			
		}
	}
} // Klasse AdressKategorie
?>