<!--  Template Artikel Stammdaten -->
{popup_init src="/javascript/overlib.js"}
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>

<h1>{$title}</h1>

<table class="volleTabelle">
<form action="?id={$id}" method="post" name="Artikel" enctype="multipart/form-data">
<input type="hidden" name="docinput[Artikel_Nr]" value="{$Artikel->Artikel_id}" />
<tr>
  <td>Artikel-ID</td><td>{$Artikel->Artikel_id}</td>
</tr>
<tr>
  <td><label for="Art">Kategorie</label></td>
  <td>
    <select name="docinput[F_Art_id]">
    {html_options values=$Artikelarten_values selected=$Artikel->F_Art_id output=$Artikelarten_output}    
    </select>
    <small>(Ast bei Änderung mitnehmen <input type="Checkbox" name="docinput[AstKopieren]" value="v">)</small>
  </td>
</tr>
<tr>
  <td><label for="Bez">Bezeichnung</label></td>
  <td><input type="Text" id="Bez" name="docinput[Bezeichnung]" value="{$Artikel->Bezeichnung|escape}" 
  size="40" maxlength="60" /> 
  (<span title="Buchungen, denen dieser Artikel zugeordnet ist">B:{$Artikel->getBuchungsAnzahl()}</span>, 
  <span title="Anzahl Artikel denen dieser Artikel zugeordnet ist">V:{$Artikel->getOberArtikelAnzahl()}</span>,
		letzte Änderung: {$Artikel->ArtikelStand|date_format:"%d.%m.%Y %H:%M"})</td>
</tr>
<tr>
  <td>Vollständiger Pfad</td><td class="helleZeile">
    {$Artikel->getArtikelPfadString(100)}
  </td>
</tr>
<tr>
  <td>Beschreibung</td>
  <td>
  <textarea name="docinput[Beschreibung]" cols="40" rows="4" />{$Artikel->Beschreibung}</textarea>
  </td>
</tr>
<tr>
  <td><label for="Position">Position</label></td>
  <td>
    <input type="Text" id="Position" name="docinput[Position]" value="{$Artikel->Position}" size="5" maxlength="5" /> 
    <label for="Einruecken">Einrücken</label> 
    <input type="Text" id="Einruecken" name="docinput[Einruecken]" value="{$Artikel->Einruecken}" size="5" maxlength="5" />
  </td>
</tr>
<tr>
  <td><label for="Einheit">Einheit</label></td>
  <td><input type="Text" id="Einheit" name="docinput[Einheit]" value="{$Artikel->Einheit|escape}" size="25" maxlength="25" />
  </td>
</tr>
<tr>
  <td><label for="MWSt">MWSt</label></td>
  <td>
    <select name="docinput[F_MWSt]" id="MWSt">
    {html_options options=$Artikel->getMWSTListe() selected=$Artikel->F_MWSt}
    </select>
    Pflicht <input type="checkbox" name="docinput[Steuerpflicht]" value="1" 
    {if $Artikel->Steuerpflicht}checked="checked"{/if} 
    title="Ankreuzen wenn der Artikel immer mit Steuer abgerechnet wird" />
</tr>
<tr>
  <td><label for="Anzeigen">Anzeigen bei Buchung</label></td>
  <td><input type="Checkbox" id="Anzeigen" name="docinput[Anzeigen]" value="1" 
    {if $Artikel->Anzeigen}checked="checked"{/if} />
  </td>
</tr>
<tr>
  <td><label for="Geringwertig">Geringwertiges Wirtschaftsgut</label></td>
  <td><input type="Checkbox" id="Geringwertig" name="docinput[Geringwertig]" value="1" 
    {if $Artikel->Geringwertig}checked="checked"{/if} />
  </td>
</tr>
<tr>
  <td><label for="Rabattfaehig">Rabattfähiger Artikel</label></td>
  <td><input type="Checkbox" id="Rabattfaehig" name="docinput[Rabattfaehig]" value="1" 
    {if $Artikel->Rabattfaehig} checked="checked"{/if} />
  </td>
</tr>
<tr>
  <td colspan="2">
    {foreach from=$Dateiliste item=file}
      <a href="redirect.php?url={$file.URL}" title="{$file.Groesse} KB">{$file.Name}</a> 
      [ <a href="?id={$id}&docinput[Artikel_Nr]={$Artikel->Artikel_id}&DelDatei={$file.Name}">Löschen</a>]<br />
    {/foreach}
  </td>
</tr>
<tr> 
  <td>Werbe-/Infomaterial</td>
  <td><input type="file" name="Werbematerial" />
  </td>
</tr>
<tr>
  <td>Lieferant</td>
  <td>
    <select name="docinput[F_Lieferant_id]">
    {html_options values=$Lieferanten_values selected=$Artikel->F_Lieferant_id output=$Lieferanten_output}
    </select>
    {if (isset($Lieferant))}
     <a href="{$Adressenurl}&Bearbeiten={$Lieferant->Adressen_id}" 
     {popup text=$Lieferant->Uebersicht(true)|escape caption="Kontaktdaten bearbeiten"}>
     Zu {$Lieferant->Name}</a>
    {/if}
  </td>
</tr>
<tr>
  <td>Bestellnummer</td>
  <td>
    <input type="Text" name="docinput[Bestellnummer]" value="{$Artikel->Bestellnummer|escape}" size="40" maxlength="40"/>
  </td>
</tr>
<tr>
  <td>Einkaufspreis</td>
  <td>
    <input type="Text" name="docinput[Einkaufspreis]" value="{$Artikel->Einkaufspreis}" size="10" maxlength="15"/> &euro;
  </td>
</tr>
<tr>
  <td><label for="EAN">EAN</label></td>
  <td>
    <input type="Text" id="EAN" name="docinput[Barcode]" value="{$Artikel->Barcode|escape}" size="40" maxlength="40"/>
    <span id="Code">
    {if $Artikel->Barcode == ''}
    <a href="#" onClick="javascript:xajax_createEAN();return false;">CVJM-EAN erzeugen</a>
    {else}
    <img src="/barcode.php?Anzeige={$Artikel->Barcode}" alt="Barcode" title="{$Artikel->Barcode|escape}"/>
    {/if}
    </span>
  </td>
</tr>
<tr>
  <td>Schlafplätze</td>
  <td>
    <input type="Text" name="docinput[Schlafplatz]" value="{$Artikel->Schlafplatz}" size="5" maxlength="5"/>
    <span class="small">(-1 für keine, 0 für beliebig)</span>
  </td>
</tr>
<tr>
  <td>bei Buchung: Person notwendig</td>
  <td>
    <input type="checkbox" name="docinput[PersonJN]" {if $Artikel->PersonJN}checked="checked"{/if} 
    size="5" maxlength="5" value="1"
    title="Ankreuzen, wenn diesem Artikel beim Buchen eine ausführende Person zugeordnet werden muss" />
  </td>
</tr>
<tr>
  <td>bei Buchung: terminierte Aktion</td>
  <td>
    <input type="Text" name="docinput[Aktion]" value="{$Artikel->Aktion|escape}" size="40" maxlength="60"
    title="Aktion, die beim Buchen dieses Artikels ausgeführt werden muss" />
  </td>
</tr>
<tr>
  <td>Prüfung</td>
  <td>
    <select name="docinput[F_PruefungArt]">
    {html_options options=$Pruefungsarten selected=$Artikel->F_PruefungArt}
    </select>
    <input type="Text" name="docinput[LetztePruefung]" value="{$Artikel->LetztePruefung|date_format:"%d.%m.%Y"}" 
    size="10" maxlength="10"
    title="Datum der letzten Prüfung dieses Artikels" 
    onClick="popUpCalendar(this,Artikel['docinput[LetztePruefung]'],'dd.mm.yyyy')"
    onBlur="autoCorrectDate('Artikel','docinput[LetztePruefung]',false);"
    />
  </td>
</tr>

<tr>
  <td colspan="4" class="zentriert">
    <input type="Submit" name="docinput[Save]" value="Speichern" />
  </td>
</tr>
<tr>
  <td colspan="4" class="zentriert">
    [ <a href="?id={$id}">Artikelgesamtliste</a> ]&nbsp;
    [ <a href="?id={$id}&docinput[F_Art_id]={$Artikel->F_Art_id}">
    {$Artikel->getArtikelart()}-Liste</a> ]
    &nbsp;[ <a href="?id={$id}&docinput[Artikel_Nr]=-1&docinput[Art_id]={$Artikel->F_Art_id}">
    Neuer Artikel</a> ]&nbsp;
    [ <a href="?id={$id}&docinput[Kopie]={$Artikel->Artikel_id}">Artikelkopie erstellen</a> ]
  </td>
</tr>
</form>
</table>

{$Tabellen}

<table width="100%">
<tr class="ueberschrift">
  <th colspan="7">
    <a id="Preise" name="Preise"></a>Preise
  </th>
</tr>
<tr>
  <th>Preisliste</th>
  <th>Gültig ab</th>
  <th>Netto-Preis</th>
  <th>Brutto</th>
  <th>Netto-Preis/Stunde</th>
  <th>Brutto/Stunde</th>
  <th></th>
</tr>
{if ( isset($docinput.EditPreis) )}
<form action="?id={$id}&docinput[Artikel_Nr]={$Artikel->Artikel_id}#Preise" name="Preise" method="post">
{/if}
<script language="javascript">
{literal}
function rechneSteuer(id, wert)
{
  if ( isNaN(wert))
  {
    wert = wert.replace(/,/, "."); 
  }
  if ( ! isNaN(wert) )
  {
  {/literal}
    document.getElementById(id).value = Math.round(wert*100/{math equation="(1+x/100)" x=$Artikel->getMWST()})/100;  
  {literal}
  }
} 
{/literal}
</script>
{foreach from=$Artikel->getPreise() item=preis}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    <a href="{$Preisurl}&docinput[Preisliste]={$preis.Preisliste_id}">
    {$preis.Bezeichnung|escape}</a>
  </td>
  <td>{$preis.GueltigAb|date_format:"%d.%m.%Y"}</td>
  <td align="right">
  {if ( isset($docinput.EditPreis) )}
      <input type="Text" name="docinput[Preis][{$preis.Preisliste_id}]" size="3" 
      maxlength="8" id="PL{$preis.Preisliste_id}" value="{$preis.Preis|string_format:"%01.2f"}" />
  {else}
  {$preis.Preis|string_format:"%01.2f"}
  {/if}
  </td>
  <td align="right">
  {if ( isset($docinput.EditPreis) )}
        <input type="Text" size="3" maxlength="8" 
        value="{math equation="x*(1+y/100)" x=$preis.Preis y=$Artikel->getMWST() format="%01.2f"}"
        onKeyUp="rechneSteuer('PL{$preis.Preisliste_id}', this.value);"/>
  {else}
      {math equation="x*(1+y/100)" x=$preis.Preis y=$Artikel->getMWST() format="%01.2f"}
  {/if}
  </td>
  <td align="right">
  {if ( isset($docinput.EditPreis) )}
      	<input type="Text" id="PLS{$preis.Preisliste_id}" 
      	name="docinput[PreisStunde][{$preis.Preisliste_id}]" size="3" 
      	maxlength="8" value="{$preis.PreisStunde|string_format:"%01.2f"}" />  
  {else}
      {$preis.PreisStunde|string_format:"%01.2f"}  
  {/if}      
  </td>
  <td align="right">
  {if ( isset($docinput.EditPreis) )}
      <input type="Text" size="3" maxlength="8" 
      value="{math equation="x*(1+y/100)" x=$preis.PreisStunde y=$Artikel->getMWST() format="%01.2f"}" 
      onKeyUp="rechneSteuer('PLS{$preis.Preisliste_id}', this.value);"/>
  {else} 
    {math equation="x*(1+y/100)" x=$preis.PreisStunde y=$Artikel->getMWST() format="%01.2f"}
  {/if}
  <td>
  <td>
      <a href="?id={$id}&docinput[Artikel_Nr]={$Artikel->Artikel_id}&docinput[DelPreis]={$preis.Preisliste_id}#Preise" 
      onClick="javascript:return window.confirm('Artikel wirklich aus Preisliste entfernen?');">
      <img src="/img/small_edit/delete.gif" border="0"/></a>
  </td>
</tr>
{/foreach}

{if ( isset($docinput.EditPreis) )}
<tr>
  <td>
    <select name="docinput[Preisliste_id]">
    {html_options options=$Preislisten selected=$Preisliste}
    </select>
  </td>
  <td></td>
  <td align="right">
     <input type="Text" name="docinput[Preis][-1]" size="3" maxlength="8" value="0,00" />
  </td>
  <td></td>
  <td align="right">
     <input type="Text" name="docinput[PreisStunde][-1]" size="3" maxlength="8" value="0,00" />
  </td>
  <td></td>
</tr>
<tr> 
  <td colspan="7" align="center">
    <input type="Submit" name="docinput[SavePreis]" value="Preise speichern" />
  </td>
</tr>
</form>
{else}
    {if ( Count($Artikel->getPreise() ) == 0 )}
      <tr><td colspan="7" align="center">Artikel steht auf keiner Preisliste</td></tr>
    {/if}    
    <tr><td colspan="7" align="center">[ <a href="{$Preisurl}">zu den Preislisten</a> ]&nbsp;&nbsp;
{/if}
{if ( ! isset($docinput.EditPreis) && ! $Artikel->isNeu() )}
 	[ <a href="?id={$id}&docinput[Artikel_Nr]={$Artikel->Artikel_id}&docinput[EditPreis]=1#Preise">
    	Preise bearbeiten</a> ]
{/if}
</td></tr>
</table>

<a id="Baumliste" name="Baumliste"></a>
<table width="100%" id="Artikelbaumliste">
	{include file="Artikel_Baumliste.tpl"}
</table>

<div class="zentriert">
[ <a href="?id={$id}">Artikelgesamtliste</a> ]&nbsp;
[ <a href="?id={$id}&docinput[F_Art_id]={$Artikel->F_Art_id}">
{$Artikel->getArtikelart()}-Liste</a> ]
&nbsp;[ <a href="?id={$id}&docinput[Artikel_Nr]=-1&docinput[F_Art_id]={$Artikel->F_Art_id}">
Neuer Artikel</a> ]
&nbsp;&nbsp;&nbsp;
[ <a href="?id={$id}&Baum=1&docinput[F_Art_id]={$Artikel->F_Art_id}">
Baum neu nummerieren</a> ]
</div>
