<!-- Template Abrechnung Stammdaten -->

<table width="100%">
<form action="?id={$id}&docinput[Rechnung_id]={$rechnung.Rechnung_id}" method="post" name="Rechnung">
<tr class="ueberschrift">
  <th colspan="4">
	Buchung {$rechnung.F_Buchung_Nr} - 
	{if $rechnung.Rechnung == 1}
	  Rechnung {$rechnung.Rechnungsnummer}
	{else} 
	  Berechnung
	{/if}
	 vom {$rechnung.Rechnungsdatum|date_format:"%d.%m.%Y"}
  </th>
</tr>
<tr>
  <td>
  {if $rechnung.Rechnung == 1}
    Rechnungsnummer
  {else}
	Dokumentart
  {/if}
  </td>
  <td>
  {if $rechnung.Rechnung == 1}
    <input type="hidden" name="docinput[IstRechnung]" value="1" />
	<input type="Text" name="docinput[Rechnungsnummer]" value="{$rechnung.Rechnungsnummer}" 
	size="8" maxlength="8" />
	 Storno: <input type="Checkbox" name="docinput[IstKeineRechnung]" value="v" 
	{if $rechnung.Storno}checked="checked"{/if}
	{if isset ($docinput.Kundenwahl)}disabled="disabled"{/if}/>
  {else}
	<select name="docinput[IstRechnung]" {if isset($docinput.Kundenwahl)}disabled="disabled"{/if}>
	  {html_options values=$DokArten_values selected=$rechnung.Rechnung output=$DokArten_output}
	</select>
  {/if}
  </td>
  <td>Datum</td>
  <td>
	<input type="Text" name="docinput[Rechnungsdatum]" value="{$rechnung.Rechnungsdatum|date_format:"%d.%m.%Y"}" 
	  size="10" maxlength="10" 
	{if isset($docinput.Kundenwahl)}
	  readonly="readonly"
	{else}
	  onClick="popUpCalendar(this,Rechnung['docinput[Rechnungsdatum]'],'dd.mm.yyyy')"
      onBlur="autoCorrectDate('Rechnung','docinput[Rechnungsdatum]' , false )"	  	
	{/if} />
  </td>
</tr>
{if !isset($docinput.Kundenwahl)}
<tr>
  <td>Kundennummer</td>
  <td>
    {$rechnung.F_Kunden_Nr} 
    [ <a href="?id={$id}&docinput[Rechnung_id]={$rechnung.Rechnung_id}&docinput[Kundenwahl]=1">Kunde ändern</a> ]
  </td>
  <td>E-Mail</td>
  <td>
	{$buchung.Email|default:"--nicht vorhanden--"}
  </td>
</tr>
{/if}
<tr>
  <td>Name</td>
  <td><input type="Text" name="docinput[Name]" value="{$rechnung.Name}" size="30" 
  maxlength="50" readonly="readonly" />
  </td>
  <td>Vorname</td>
  <td><input type="Text" name="docinput[Vorname]" value="{$rechnung.Vorname}" size="30" 
  maxlength="50" readonly="readonly" />
  </td>
</tr>
<tr>
  <td>Adresse</td>
  <td>
	<input type="Text" name="docinput[Strasse]" value="{$rechnung.Strasse}" size="30" 
	maxlength="50" readonly="readonly"/>
  </td>
  <td>PLZ/Ort</td>
  <td>
    <input type="Text" name="docinput[PLZ]" value="{$rechnung.PLZ}" size="5" 
    maxlength="5" readonly="readonly" />
	<input type="Text" name="docinput[Ort]" value="{$rechnung.Ort}" size="15" 
	maxlength="50" readonly="readonly" />
  </td>
</tr>
{if isset($docinput.Kundenwahl)}
<tr>
  <td colspan="4" class="zentriert">
	Kundenname <input type="Text" name="docinput[Kunde]" />
	<input type="hidden" name="docinput[Kundenwahl]" value="1" />
	<input type="hidden" name="docinput[Rechnung_id]" value="{$docinput.Rechnung_id}" />
	<input type="Submit" name="docinput[Suchen]" value="suchen" />
	{if isset ($Kunden)}
	<table>
	  <tr>
	    <th>neuer Kunde</th><th>Name</th><th>Ort</th><th>Adresse</th>
	  </tr>
	  {foreach from=$Kunden item=Kunde}
	  <tr>
	    <td>
	      <a href="?id={$id}&docinput[Rechnung_id]={$docinput.Rechnung_id}&docinput[Kundenwechsel]={$Kunde.Adressen_id}">&rarr; 
			{if $Kunde.Kunden_Nr != 0}{$Kunde.Kunden_Nr}{else}(ohne KuNr){/if}
		  </a>
		</td>
		<td>{$Kunde.Name}{if $Kunde.Vorname != ''}, {$Kunde.Vorname}{/if}</td>
		<td>{$Kunde.PLZ} {$Kunde.Ort}</td>
		<td>{$Kunde.Strasse}
		</td>
	  </tr>
	  {/foreach}
	</table>
	{/if}
  </td>
</tr>
{else}
<tr>
  <td>eingegangene Anzahlung</td>
  <td>
	<input type="Text" name="docinput[VorhAnzahlung]" value="{$rechnung.VorhAnzahlung}" 
	size="5" maxlength="8" /> &euro; (Soll: {$rechnung.Anzahlung})
  </td>
  <td>Barzahlung</td>
  <td>
	<input type="Text" name="docinput[Barzahlung]" value="{$rechnung.Barzahlung}" 
	size="5" maxlength="8" /> &euro; (Restsumme: {$rechnung.Restsumme} &euro;)
  </td>
</tr>
<tr>
  <td></td>
  <td></td>
  <td>Mit Zahlungsziel</td>
  <td><input type="Checkbox" name="docinput[Zahlungsziel]" value="v" 
		{if $rechnung.Zahlungsziel}checked="checked"{/if}/>
  </td>
</tr>
<tr>
  <td colspan="2">Hinweise für den Kunden</td>
  <td>Rabatt für diese Buchung: {$rechnung.Buchungsrabatt}%</td>
</tr>
<tr>
  <td colspan="4">
    <textarea name="docinput[Hinweise]" cols="60" rows="5">{$rechnung.Hinweise}</textarea>
  </td>
</tr>
{if ! $Buchungfertig && $Buchungaktuell}
<tr>
  <td colspan="4" class="zentriert">
    Buchung als abgerechnet und damit erledigt markieren
	<input type="Checkbox" name="docinput[Erledigt]" value="{$Status}" />
  </td>
</tr>
{/if}
<tr>
  <td colspan="4" class="zentriert">
	{if !isset($docinput.Kundenwahl)}
		<input type="Submit" name="Save" value="Speichern" />
	{/if}
	&nbsp;&nbsp;&nbsp;[ <a href="{$Buchungurl}">ohne Speichern zurück zur Buchung</a> ]
	&nbsp;&nbsp;&nbsp;[ <a href="{$Buchungurl}&docinput[DelAbrechnung]={$rechnung.Rechnung_id}" 
	  onClick="javascript:return window.confirm('Berechnung wirklich löschen?');">
	Berechnung löschen</a> ]
	{if $rechnung.Rechnung == 1 && $rechnung.Storno != 1 && ! $Buchungfertig}
	  &nbsp;&nbsp;&nbsp;[ 
	  <a href="?id={$id}&docinput[Rechnung_id]={$rechnung.Rechnung_id}&docinput[Bereinigen]=1" 
	  onClick="javascript:return window.confirm('Bereits berechnete Einträge wirklich entfernen?');">
	Splitrechnung</a> ]
	{/if}
  </td>
</tr>
{/if}
</form>
</table>