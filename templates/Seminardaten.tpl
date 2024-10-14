<h1>{$title}</h1>

{popup_init src="/javascript/overlib.js"}
{include file="Buchung_Kopf.tpl"}


<div class="ueberschriftzeile">
  Seminardaten {$Buchung->Buchung_Nr}
</div>
<a id="Daten" name="Daten"></a>
<form action="?id={$id}#Daten" method="post">
<input type="hidden" name="Buchung_Nr" value="{$Buchung->Buchung_Nr}"/>
<table>
<tr>
  <td>Seminarbezeichnung</td>
  <td>
    <input type="Text" name="docinput[Bezeichnung]" value="{$seminar.Bezeichnung|escape}" size="50" maxlength="50" {$check} />
  </td>
</tr>
<tr>
  <td>Beschreibung</td>
  <td>
    <textarea name="docinput[Beschreibung]" rows="5" cols="60" {$check}>{$seminar.Beschreibung|escape}</textarea>
  </td>
</tr>
<tr>
  <td>Leitung</td>
  <td>
    <input type="Text" name="docinput[Leitung]" value="{$seminar.Leitung|escape}" size="40" maxlength="40" {$check} />
  </td>
</tr>
<tr>
  <td>Min. Teilnehmer</td>
  <td>
    <input type="Text" name="docinput[MinTeilnehmer]" value="{$seminar.MinTeilnehmer}" size="3" maxlength="3" {$check} />
  </td>
</tr>
<tr>
  <td>Max. Teilnehmer</td>
  <td>
    <input type="Text" name="docinput[MaxTeilnehmer]" value="{$seminar.MaxTeilnehmer}" size="3" maxlength="3" {$check} />
  </td>
</tr>
<tr>
  <td>Anmeldeschluss</td>
  <td>
    <input type="Text" name="docinput[Anmeldeschluss]" value="{$seminar.Anmeldeschluss|date_format:"%d.%m.%Y"}" size="10" maxlength="10" {$check} />
  </td>
</tr>
</table>
<div class="ueberschriftzeile">Preise</div>
<table>
<tr>
  <td>Bezeichnung</td><td>Preis</td><td>Löschen</td>
</tr>
{foreach from=$Preise item=preis}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>{$preis.Gebuehrbezeichnung}</td>
  <td>
    {if !$Buchung->isFertig()}
    <input type="Text" name="docinput[Preis][{$preis.Gebuehrbezeichnung}]" value="{$preis.Gebuehr}" size="4" maxlength="4"/>
    {else}
    {$preis.Gebuehr}
    {/if}
  </td>
  <td>
    {if !$Buchung->isFertig()}
    <input type="Checkbox" name="docinput[DelPreis][{$preis.Gebuehrbezeichnung}]" value="v" />
    {/if}
  </td>
</tr>
{/foreach}
{if !$Buchung->isFertig()}
<tr>
  <td>  
    <input type="Text" name="docinput[NeuBez]" size="25" maxlength="25"/>
  </td>
  <td>
    <input type="Text" name="docinput[NeuPreis]" size="4" maxlength="4" />
  </td>
</tr>
{/if}
</table>
{if !$Buchung->isFertig()}
  <div class="ganzeZeile zentriert"><input type="Submit" value="Speichern" /></div>
{/if}    
</form>
{$Vorlagen}
<div class="zentriert">
  [ <a href="{$Buchungurl}&Buchung_Nr={$Buchung->Buchung_Nr}">Zurück zur Buchung</a> ]
</div>
{if !$Buchung->isFertig()}  
<a id="Hinzu" name="Hinzu"></a><div class="ueberschriftzeile">Teilnehmer hinzufügen</div>
<form action="?id={$id}#Hinzu" method="post" >
Name/Ort/KuNr des Teilnehmers <input type="Text" name="docinput[Search]" 
value="{$docinput.Search|escape}" size="25" maxlength="25" /> 
<input type="Submit" value="Suchen" />
<input type="hidden" name="Buchung_Nr" value="{$Buchung->Buchung_Nr}" />
{if Count($Adressen) > 0}
<a id="Hinzufuegen" name="Hinzufuegen"></a><table class="volleTabelle">
{foreach from=$Adressen item=gefunden}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    <input type="Checkbox" name="docinput[Teilnehmer][]" value="{$gefunden->Adressen_id}" />
  </td>
  <td>{$gefunden->Name}</td>
  <td>{$gefunden->Vorname}</td>
  <td>{$gefunden->Strasse}</td>
  <td>{$gefunden->Ort}</td>
  <td>{$gefunden->Alter()} Jahre</td>
  <td>
    <select name="docinput[Art][{$gefunden->Adressen_id}]">
    {foreach from=$Preise key=nr item=value}
      <option value="{$value.Gebuehr}" {if ( $nr== 0 )} selected="selected"{/if}>{$value.Gebuehrbezeichnung}</option>
    {/foreach}
    </select>
  </td>
</tr>
{/foreach}
<tr>
  <td></td>
  <td colspan="5"><a href="{$Adressenurl}&Bearbeiten=-1&Sem={$sem}&docinput[Search]={$docinput.Search}" target="_blank">Neue Adresse hinzufügen</a>
  </td>
</tr>
</table>
<div class="ganzeZeile zentriert"><input type="Submit" value="Hinzufügen" /></div>
{else}
{if isset($docinput.Search)}
<div class="ganzeZeile zentriert Fehler">Keinen passenden Adressdatensatz gefunden!</div>
{/if}
{/if}
</form>
{/if}
