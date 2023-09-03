<h2>Tankerfassung für {$Groesse.Bezeichnung}</h2>
<form action="?id={$id}&docinput[Groesse]={$Groesse.Groesse_id}" method="post" name="WertForm">
<label>Datum</label>
<input type="Text" name="docinput[Datum]" value="{$smarty.now|date_format:"%d.%m.%Y"}" 
    size="10" maxlength="10" onClick="popUpCalendar(this,WertForm['docinput[Datum]'],'dd.mm.yyyy')"
     onBlur="autoCorrectDate('WertForm','docinput[Datum]' , false )" />
<br />
<label>Wert vorher</label>
<input type="text" name="docinput[WertVorher]" />
<br />
<label>Wert nachher</label>
<input type="text" name="docinput[WertNachher]" />
<br />
<label>Preis pro {$Groesse.Einheit}</label>
<input type="text" name="docinput[Preis]" />
<label>Hinweis zum Preis</label>
<input type="text" name="docinput[Hinweis]" value="Tanken am {$smarty.now|date_format:"%d.%m.%Y"}" /> 
<br />
<input type="Submit" name="" value="Speichern"/>
</form>
<hr />
