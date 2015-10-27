{* Template Buchung_Tabelle *}
<!--  Popup-Kalender -->
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>

<h1>{$title}</h1>

<table class="drucklinien">
 
<tr class="noprint">
  <td colspan="6">
    <form action="?id={$id}" name="SuchForm" method="post">
    <label for="Suchen">Buchung </label> 
    <input type="Text" id="Suchen" name="Search" value="{$Search|escape}" size="20" 
      onClick="popUpCalendar(this,'SuchForm['Search'],'dd.mm.yyyy')" />
    {if !$nurAnsicht}
    (Buchungsnummer, Datum, Name, PLZ oder Hinweistext eingeben)
    {else}
       plus <input type="Text" name="docinput[Tage]" value="{$Tage}" size="2" maxlength="3" /> Tage
    {/if}
    <br />
    {foreach from=$Optionen key=ID item=option}
      <input type="Checkbox" name="docinput[{$ID}]" value="v" {if $option} checked="checked"{/if}/> 
      {$ID}&nbsp;&nbsp;
    {/foreach}    
    Altergruppe <select name="docinput[Altersgruppe]">
    <option selected="selected" value="-1">-alle-</option>
    {html_options options=$Altersgruppen }
    </select>
    <input type="Submit" value="Suchen/Anzeigen" /> 
    </form>
    {if ! $nurAnsicht}
      Seite 
      {section name=Page loop=$Seiten start=1}
        {if $smarty.section.Page.index != $Seite}
          <a href="?id={$id}&Seite={$smarty.section.Page.index}{$params}">{$smarty.section.Page.index}</a> 
        {else}
          <strong>{$smarty.section.Page.index}</strong>
        {/if}
      {/section} 
    {/if}
  </td>
</tr>

<tr class="ueberschrift">
  <th></th>
  <th>Ankunft</th><th>Abfahrt</th><th>Name</th><th>Belegung/<br />Personen</th>
</tr>

{foreach from=$Buchungen item=buchung}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    <a href="{$buchung->buchungURL()}">{$buchung->Buchung_Nr} {$buchung->buchungsStatusBild()}</a>
  </td>
  <td align="center">
    {$buchung->Von|date_format:"%A"}<br />
    {$buchung->Von|date_format:"%d.%m.%Y"}
    {if $buchung->Ankunftszeit > 20400} {$buchung->Ankunftszeit|date_format:"%H:%M"}{/if}
  </td>
  <td align="center">
    {$buchung->Bis|date_format:"%A"}<br />
    {$buchung->Bis|date_format:"%d.%m.%Y"}
    {if $buchung->Abfahrtszeit > 20400} {$buchung->Abfahrtszeit|date_format:"%H:%M"}{/if}
  </td>
  <td>
    {if $buchung->Seminar != 0}Seminar{/if} 
    <a href="{$buchung->Adresse->adresseURL()}">{$buchung->Adresse->Name} {$buchung->Adresse->Vorname} ({$buchung->Adresse->Ort})</a>
    {if $buchung->Zusatz != ''}<br />{$buchung->Adresse->Zusatz}{/if}
    {if $buchung->Seminar != 0}<br />Seminarteilnehmer:<br />
    	{foreach from=$buchung->seminarteilnehmer() item=teilnehmer}
    		&nbsp;&nbsp;&nbsp;<a href="{$teilnehmer->adresseURL()}">{$teilnehmer->Name}, {$teilnehmer->Vorname} aus {$teilnehmer->Ort}</a></br>
    	{/foreach}
    {/if}    
  </td>
  <td>
    {if $buchung->getOrte() == ''}
      <span class="Fehler"><a href="{$buchung->buchungOrteURL()}">Belegung fehlt</a></span>      
    {else}
      <span><a href="{$buchung->buchungOrteURL()}">{$buchung->getOrte()}</a></span>
    {/if}
    <br />
    {if $buchung->F_Aufenthaltsraum_id == -1}
      <span class="Achtung">Kein Aufenthaltsraum</span>
    {/if} 
    {if !$buchung->verpflegungVorhanden()}
      <a href="{$buchung->buchungVerpflegungURL()}">    
      <span class="Fehler">Verpflegung fehlt</span>
      </a>
    {else}
      {if $buchung->F_Speiseraum_id == -1}
         <span class="Fehler">Kein Speiseraum!</span>
      {/if}
      {if $buchung->isSelbstVerpflegung()}
        <span class="Selbstverpflegung"><a href="{$buchung->buchungVerpflegungURL()}">Selbstverpflegung</a></span>
      {else}
        <span class="Verpflegung"><a href="{$buchung->buchungVerpflegungURL()}">Verpflegung</a></span>
      {/if}        
    {/if}
    {if $buchung->Abnahmename == '' || $buchung->Anreisename == '' }
        <br /><span class="Fehler">Personalplanung fehlt</span>
    {/if}
    {if $buchung->programmVorhanden()} 
        <br /><span class="Programm">
        <a href="{$buchung->buchungProgrammURL()}">Programm</a></span>
        {if !$buchung->programmPersonalVorhanden()}
        <span class="Fehler"> - ohne Personal!</span>
        {/if}
    {/if}
    <br />{$buchung->personenAnzahl()} Personen ({$buchung->getAltersgruppe()})
    {if $buchung->BetreuerW+$buchung->BetreuerM > 0}
        davon {eval var=$buchung->BetreuerW+$buchung->BetreuerM} Betreuer
    {/if}
    {if $buchung->schluesselAnzahl() > 0} ({$buchung->schluesselAnzahl()} Schl&uuml;ssel){/if}
    {if $buchung->angebotExistiert() == false}
      <span class="Fehler">Angebot fehlt</span>
    {/if}
    
    
  </td>
</tr>
{/foreach}

{if ! $nurAnsicht}
<tr class="noprint">
  <td colspan="6">    
      Seite 
      {section name=Page loop=$Seiten start=1}
        {if $smarty.section.Page.index != $Seite}
          <a href="?id={$id}&Seite={$smarty.section.Page.index}{$params}">{$smarty.section.Page.index}</a> 
        {else}
          <strong>{$smarty.section.Page.index}</strong>
        {/if}
      {/section}         
  </td>
</tr>
{/if}
</table>