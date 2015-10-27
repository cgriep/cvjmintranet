<form action="?id={$id}" method="post" name="Wahl" class="noprint">
<label for="Datum">Verpflegung ab Datum</label>
<input type="Text" name="docinput[Datum]" value="{$Datum|date_format:"%d.%m.%Y"}" size="10" maxlength="10" 
	onClick="popUpCalendar(this,Wahl['docinput[Datum]'],'dd.mm.yyyy')"
    onBlur="autoCorrectDate('Wahl','docinput[Datum]' , false )" />
<input type="Submit" value="anzeigen" /> 
</form>

<table class="drucklinien">
<tr class="ueberschrift">
  <th>Datum</th>
{foreach from=$Artikel item=artikel}
  <th>{$artikel}</th>
{/foreach}
</tr>
{foreach from=$Essen item=eintrag key=tag}
<tr class="{cycle values="helleZeile,dunkleZeile"} ">
  <td class="grossdruck">{$tag|date_format:"%A<br />%d.%m.%Y"}</td>
  {foreach from=$Artikel item=artikel key=artnr}
  <td class="zentriert">
  {if ( isset($eintrag.$artnr))}
    {assign var=Gesamtmenge value="0"}
    {assign var=GesamtmengeV value="0"}
    {assign var=GesamtmengeB value="0"}
    {assign var=GesamtmengeS value="0"}
    {foreach from=$eintrag.$artnr item=daten key=buchnr}
      {assign var=Buchung value=$Buchungen.$tag.$artnr.$buchnr} 
	  {if ( $Buchung->Essenbesonderheit != '' )}
		{assign var=s value=$Buchung->Essenbesonderheit}
      {else}
        {assign var=s value=''}
      {/if}
<a {popup text=$s caption="Buchung {$Buchung->Buchung_Nr}"} 
  href="?id={$id}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}&docinput[Datum]={$tag}&docinput[Art]={$artnr}">
  {if ( $Buchung->F_Speiseraum_id > 0 )}
	{$Buchung->getSpeiseraum()} :{else}&gt;{/if}
  {$daten}/{$Buchung->BetreueranzahlW+$Buchung->BetreueranzahlM}
  {if ( $Buchung->Vegetarier > 0 )}/V:{$Buchung->Vegetarier}{/if}
  {if ( $Buchung->Schweinelos > 0 )}/S:{$Buchung->Schweinelos}{/if}
   ({$Buchung->getAltersgruppe()|truncate:5}
   {if ( ! $Buchung->Kuechenhilfe)}, o.H.{/if}
   {if ( trim($Buchung->Essenbesonderheit) != '' )}<span class="Fehler">!!!!</span>{/if}
   )</a><br />
     {assign var=Gesamtmenge value=$Gesamtmenge+$daten}
     {assign var=GesamtmengeV value=$GesamtmengeV+$Buchung->Vegetarier}
     {assign var=GesamtmengeS value=$GesamtmengeS+$Buchung->Schweinelos}
     {assign var=GesamtmengeB value=$GesamtmengeB+$Buchung->BetreueranzahlM+$Buchung->BetreueranzahlW}
    {/foreach}
  <strong class="grossdruck">{$Gesamtmenge}/{$GesamtmengeB}{if ( $GesamtmengeV != 0 )} /V:{$GesamtmengeV}{/if}
      {if ( $GesamtmengeS != 0 )}/S:{$GesamtmengeS}{/if}
  </strong>
{else}
&nbsp;
{/if}
</td>
  {/foreach}
</tr>
{/foreach}
</table>
1. Zahl: Essen gesamt, 2. Zahl: Betreuer, V- Vegetarier, S-Schweinelos, !-Besonderheit beachten