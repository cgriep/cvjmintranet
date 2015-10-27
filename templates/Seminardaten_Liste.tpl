<a id="Teilis" name="Teilis"></a>
<div class="ueberschriftzeile">{$Teilnehmeranzahl} Teilnehmer</div>

<form action="?id={$id}#Teilis" method="post">
<input type="hidden" name="Buchung_Nr" value="{$Buchung->Buchung_Nr}" />
<table class="volleTabelle">
{if $Teilnehmeranzahl > 0}  
  <tr>
    <td>Entf.</td><td>Gruppe</td><td>Name</td><td>Alter</td><td>Preis</td>
    <td>Anzahlung</td><td>Anz.-Bemerkung</td>
    <td>Veg.</td><td>Schw.</td><td>Bemerkung</td>
    <td>Vorlagen</td>
  </tr>
{/if}
{foreach from=$SeminarTeilnehmer item=teilnehmer}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
  {if ! $Buchung->isFertig()}
    <input type="Checkbox" name="docinput[DelTeili][]" value="{$teilnehmer.F_Adressen_id}"/>
  {/if}
  </td>
  <td>
  {if ! $Buchung->isFertig()} 
	<input type="text" name="docinput[SeminarGruppe][{$teilnehmer.Adressen_id}]" value="{$teilnehmer.SeminarGruppe}" size="2" />
  {else}
    {$teilnehmer.SeminarGruppe}
  {/if}
  </td>
  <td>
     <a href="{$Adressenurl}&Bearbeiten={$teilnehmer.Adressen_id}">
       {$teilnehmer.Name}, {$teilnehmer.Vorname} ({$teilnehmer.Ort}, {$teilnehmer.Kunden_Nr})
     </a>
     (<a id="KorrLink{$teilnehmer.Adressen_id}" title="Korrespondenz zeigen" 
     onClick="javascript:xajax_zeigeSeminarKorrespondenz({$Buchung->Buchung_Nr},{$teilnehmer.Adressen_id},true);">K</a>)
     <div id="Korrespondenz{$teilnehmer.Adressen_id}">
     </div>
  </td>
  <td {if ( $teilnehmer.Geburtsdatum == 0 )} bgcolor="red"{/if}>{$teilnehmer.Alter}
  </td>
  <td>
     {if ( !$Buchung->isFertig())}
       <input type="Text" name="docinput[SeminarPreis][{$teilnehmer.Adressen_id}]" 
       value="{$teilnehmer.SeminarPreis}" size="4" maxlength="6" />
     {else}
     {$teilnehmer.SeminarPreis}
     {/if}
  </td><td>
     {if ( !$Buchung->isFertig())}
       <input type="Text" name="docinput[Seminaranzahlung][{$teilnehmer.Adressen_id}]" 
       value="{$teilnehmer.Seminaranzahlung}" size="4" maxlength="6"/>
     {else}
     	{$teilnehmer.Seminaranzahlung}
     {/if}
  </td><td>
     {if ( !$Buchung->isFertig())}
       <input type="Text" name="docinput[SeminarAnzahlungsBemerkung][{$teilnehmer.Adressen_id}]" 
       value="{$teilnehmer.SeminarAnzahlungsBemerkung|escape}" size="8" maxlength="20"/>
     {else}
     {$teilnehmer.SeminaranzahlungsBemerkung}
     {/if}
  </td>
  <td>
     <input type="Checkbox" name="docinput[SeminarVegetarier][{$teilnehmer.Adressen_id}]" value="1" title="Vegetarier" 
     {if ( $teilnehmer.SeminarVegetarier )}	checked="checked"{/if}
     {if ( $Buchung->isFertig())} disabled="disabled"{/if} />
  </td>
  <td>
     <input type="Checkbox" name="docinput[SeminarSchweinelos][{$teilnehmer.Adressen_id}]" value="1" title="Schweinelos" 
     {if ( $teilnehmer.SeminarSchweinelos )}checked="checked"{/if}
     {if ( $Buchung->isFertig())} disabled="disabled"{/if} />     
   </td>
   <td>
     <textarea name="docinput[SeminarBemerkung][{$teilnehmer.Adressen_id}]" cols="20" rows="2">{$teilnehmer.SeminarBemerkung}</textarea>     
   </td>
   <td>
     {$teilnehmer.Vorlagen}
   </td>
</tr>
{/foreach}
{if ( $Teilnehmeranzahl > 0 && !$Buchung->isFertig() )}
    <tr><td><input type="Checkbox" name="docinput[Aktualisieren]" value="v" /></td>
    <td colspan="7">Altersangaben und Essen in der Buchung aktualisieren</td></tr>
{/if}
</table>
<div class="zentriert ganzeZeile">
{if $Teilnehmeranzahl==0}
    <p>Keine Teilnehmer vorhanden!</p>
{else}
  {if ( !$Buchung->isFertig())}
  <input type="Submit" value="Markierte entfernen/Restliche Speichern" 
  onClick="javascript:return window.confirm('Markierte Teilnehmer wirklich aus dem Seminar entfernen und Daten speichern?');" />
  {/if}
{/if}
</div>
</form>
<div class="zentriert">[ <a href="{$Buchungurl}&Buchung_Nr={$Buchung->Buchung_Nr}">Zurück zur Buchung</a> ]</div>
