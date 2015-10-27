<h2>Werteerfassung für {$Groesse.Bezeichnung}</h2>
<table>
<form action="?id={$id}&Wert=-1&Groesse={$Groesse.Groesse_id}" method="post" name="WertForm">
<tr class="ueberschrift">
  <td>Datum</td>
  <td>Zähler</td>
  <td>Bemerkung</td>
  <td>Verbrauch</td>
  <td>Kosten</td>
</tr>
<tr>
  <td>
    <input type="Text" name="docinput[Datum]" value="{$smarty.now|date_format:"%d.%m.%Y"}" 
    size="10" maxlength="10" onClick="popUpCalendar(this,WertForm['docinput[Datum]'],'dd.mm.yyyy')"
     onBlur="autoCorrectDate('WertForm','docinput[Datum]' , false )" />
  </td>
  <td>
    <input type="Text" name="docinput[Wert]" size="15" maxlength="15"/>
  </td>
  <td>
    <input type="Text" name="docinput[Bemerkung]" />
  </td>
  <td>
    Füllung <input type="Checkbox" name="docinput[Fuell]" value="v" title="bei Füllung anklicken"/>
  </td>
  <td>
    <input type="Submit" name="" value="Speichern"/>
  </td>
</tr>
{assign var=Gesamt value=0}
{foreach from=$Werte item=wert}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>{$wert.Datum|date_format:"%d.%m.%Y"}</td>
  <td align="right">
	{$wert.Wert|string_format:"%01.2f"}  
  </td>
  <td>
	{eval var=$Gesamt+$wert.Verbrauchskosten assign="Gesamt"}
	{$wert.Bemerkung}
  </td>
  <td align="right">
    {$wert.Verbrauch|string_format:"%01.2f"}
  </td>
  <td align="right">
    {$wert.Verbrauchskosten|string_format:"%01.2f"}
	<a href="?id={$id}&docinput[Del]={$wert.Wert_id}&Wert=-1&docinput[Groesse]={$smarty.request.Groesse}&Groesse={$smarty.request.Groesse}">
	<img src="/img/small_edit/delete.gif" border="0"/></a>	
  </td>
</tr>
{/foreach}
<tr>
  <td></td>
  <td></td>
  <td>Gesamt</td>
  <td></td>
  <td>{$Gesamt}</td>
</tr>
</table>
<input type="hidden" name="docinput[Groesse]" value="{$Groesse.Groesse_id}" />
</form>
<hr />
