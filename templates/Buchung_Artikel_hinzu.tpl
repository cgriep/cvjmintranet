{*  Template Buchung Artikel hinzufügen *}
<script language="javascript">
function showPersonen(feld, Anzahl)
{literal}
{
  if ( feld.value == '' )
  {
    feld.value = Anzahl;
    feld.focus();
    feld.select();
  }
}
{/literal}
</script>
<div class="ueberschriftzeile noprint">
    Artikel hinzufügen<a name="Artikel" id="Artikel"></a>
</div>
<div class="noprint zentriert">
<div class="Fehler">{$Fehlerliste}</div>
<form action="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}#Artikel" method="post">
    <label for="ArtikelBezeichnung">Artikel zum Hinzufügen wählen</label> 
    <input type="Text" id="ArtikelBezeichnung" name="docinput[ArtikelSearch]" 
      value="{$docinput.ArtikelSearch|escape}" 
      size="20" maxlength="30" />
    <input type="submit" value="anzeigen"
	{if ( !isset($docinput.Rechnung_id) )} 
    onClick="xajax_sucheArtikel(document.getElementById('ArtikelBezeichnung').value, {$Buchung->Buchung_Nr},{$spalten},{$id});return false;" />
    {else}
	onClick="xajax_sucheArtikel(document.getElementById('ArtikelBezeichnung').value, {$Buchung->Buchung_Nr},{$spalten},{$id},{$docinput.Rechnung_id});return false;" />
    {/if}
</form>
</div>
{if ( isset($docinput.ArtikelSearch) && trim($docinput.ArtikelSearch) != '' )}
<form action="?id={$id}#Artikel" method="post">
{if ( !isset($docinput.Rechnung_id) )}
   <input type="hidden" name="Buchung_Nr" value="{$Buchung->Buchung_Nr}" />
{else}
   <input type="hidden" name="docinput[Rechnung_id]" value="{$docinput.Rechnung_id}" />
{/if}
<input type="hidden" name="docinput[ArtikelSearch]" value="{$docinput.ArtikelSearch}" />
<table width="100%">
<tr class="noprint">
  <th colspan="2">Menge</th>
  <th>Dauer</th><th></th>
  <th>Bezeichnung</th>
  <th colspan="2">Datum</th>
  {if ( $spalten < 9 )}<th>Unberechnet</th>{else}<th>Preis</th><th>Rabatt</th>{/if}
</tr>

{foreach from=$Buchung->getArtikelSuche($docinput.ArtikelSearch) item=artikel}
<tr class="noprint {cycle values="helleZeile,dunkleZeile"}">
  <td colspan="2">
    <input type="Text" name="docinput[Menge][{$artikel->Artikel_id}]" size="3" maxlength="5" 
     title="Einzelpreis {$artikel->holePreis($Buchung->F_Preisliste_id)} pro {$artikel->Einheit}" 
     onFocus="javascript:showPersonen(this,{if $artikel->isAbrechnungNachPersonen()}{$Buchung->personenAnzahl()}{else}1{/if});" /></td>
  <td>
  {if ( $artikel->holePreis($Buchung->F_Preisliste_id, true)> 0 )}
    <input type="Text" name="docinput[Dauer][{$artikel->Artikel_id}]" size="2" 
      maxlength="4" title="Stundenpreis: {$artikel->holePreis($Buchung->F_Preisliste_id,true)}" />
  {/if}
  </td>
  <td>{$artikel->Einheit}</td>
	<td {if ( $artikel->Beschreibung !='' )}
    {assign var="beschreibung" value=$artikel->Beschreibung} 
    {assign var="bezeichnung" value=$artikel->Bezeichnung} 
    {popup text=$beschreibung|escape caption="Artikel $bezeichnung"}
  {/if}>{$artikel->Bezeichnung}</td>
  <td colspan="2">
    <input type="Text" name="docinput[Datum][{$artikel->Artikel_id}]" size="10" 
      maxlength="15" value="{$Buchung->Von|date_format:"%d.%m.%Y"}" />
    bis <input type="Text" name="docinput[EDatum][{$artikel->Artikel_id}]" 
      value="{if ( !$artikel->isProgrammpaket() )}{$Buchung->Bis|date_format:"%d.%m.%Y"}{/if}" 
      size="10" maxlength="15" />
  </td>
  {if ( $spalten < 9 )}
  <td class="zentriert">
  	<input type="checkbox" name="docinput[Unberechnet][{$artikel->Artikel_id}]" value="v" 
  	title="Anklicken wenn der Artikel nicht an den Kunden berechnet werden soll" />
  </td>
  {else}
  <td>
    <input type="Text" name="docinput[Preis][{$artikel->Artikel_id}]" 
    value="{$artikel->holePreis($Buchung->F_Preisliste_id)}" 
      size="5" maxlength="15" />
  </td>
  <td>
    <input type="Text" name="docinput[Rabatt][{$artikel->Artikel_id}]" 
      value="{if ( $artikel->Rabattfaehig)}{$Buchung->Buchungsrabatt}{else}0{/if}" 
      size="2" maxlength="4" />
  </td>
  {/if}
</tr>
{/foreach}

<tr class="noprint">
{if Count($Buchung->getArtikelSuche($docinput.ArtikelSearch))==0}
  <td colspan="{$spalten}" class="zentriert">- keine passenden Artikel gefunden -
{else}
  <td colspan="{$spalten}" class="zentriert">
    <input type="Submit" value="Hinzufügen" />
{/if}
  </td>
</tr>
</table>
</form>
{/if} {* Suchbegriff vorhanden *}
