
<!--  Skripte für Popup-Kalender -->
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>

<div tr class="zentriert">
  [&nbsp;<a href="?id={$id}">alle&nbsp;Adressen&nbsp;anzeigen</a>&nbsp;]
  {if $Adresse->Adressen_id > 0 && $Speichern}
  &nbsp;&nbsp;[ <a href="{$Buchungurl}&Buchung_Nr=-1&Adressen_id={$Adresse->Adressen_id}" title="Buchung vornehmen">Neue Buchung anlegen</a> ]
  {/if}
</div>
<table class="volleTabelle">
<form action="?id={$id}&Bearbeiten={$Adresse->Adressen_id}" method="post" name="Eingabe">
<tr class="ueberschrift">
  <th colspan="4" class="zentriert">
  Kontaktinformationen 
  {if $Adresse->Geloescht}(diese Adresse ist gelöscht){/if}
  {if $Adresse->Adressen_id<0}(neue Adresse){/if}
  </th>
</tr>
{if isset($smarty.request.Plain)}
<tr>
  <td colspan="4"><a name="Copy" id="Copy"></a>
    {$Adresse->Anrede}<br />
    {$Adresse->Vorname} {$Adresse->Name}<br />
    {if $Adresse->Zusatz != ''}{$Adresse->Zusatz|nl2br}<br />{/if}
    {$Adresse->Strasse}<br />
    {$Adresse->PLZ} {$Adresse->Ort}<br />
    {$Adresse->Telefon1}<br />
    {$Adresse->Telefon2}<br />
    {$Adresse->Fax}<br />
    {$Adresse->Email}<br />
    <hr />
  </td>
</tr>
{/if}
<tr>
  <td class="zentriert" colspan="4">
    {if $Speichern}    
    <input type="Submit" name="Save" value="Kontaktdaten Speichern" />
    {/if}
    {if $Adresse->Adressen_id >0}
      &nbsp;&nbsp;<a href="?id={$id}&Bearbeiten={$Adresse->Adressen_id}&Plain=1#Copy">
      Daten für Copy&Paste anzeigen</a>
    {/if}
  </td>
</tr>
{if isset($smarty.session.Sem)}
<tr>
  <td class="zentriert" colspan="4">
    <a href="javascript:opener.location.search='?{$smarty.session.Sem}';this.close();">Zurück zum Seminar</a>
  </td>
</tr>
{/if}
<tr>
  <td>
    <label for="Art">Anredeart</label>
  </td>
  <td>
  {if $Speichern}
    <select id="Art" name="docinput[F_Anrede_id]">
    {html_options options=$Adresse->getAnredearten() selected=$Adresse->F_Anrede_id}
    </select>
  {else}
    {$Adresse->Anrede} /
  {/if}
  <label for="Titel">Titel</label> 
  {if $Speichern}
    <input type="Text" id="Titel" name="docinput[Titel]" value="{$Adresse->Titel|escape}" size="15" maxlength="20" />
  {else}
    {$Adresse->Titel}
  {/if}
  </td>
  <td>
    {if $Adresse->Kunden_Nr > 0}
      <label for="Kundennummer">Kundennummer</label>
    {/if}
  </td>
  <td>
    {if $Adresse->Kunden_Nr > 0}
      {$Adresse->Kunden_Nr}
      {if $Adresse->Adressen_id > 0 && $Speichern}
        <font size="+1">
        [ <a href="{$Buchungurl}&Buchung_Nr=-1&Adressen_id={$Adresse->Adressen_id}" title="Buchung vornehmen">Buchung</a> ]
        </font>
      {/if}
    {else}
      {if !$Adresse->isNeu()}
        <font size="+1">
        [ <a href="?id={$id}&Bearbeiten={$Adresse->Adressen_id}&KuNr=1" title="Kundennummer erzeugen">Kundennummer vergeben</a> ]
        </font>
      {/if}
    {/if}
  </td>
</tr>
<tr>
  <td>
    <label for="Name">Name</label>
  </td>
  <td colspan="3">
    {if $Speichern}
    <input id="Name" type="Text" name="docinput[Name]" value="{$Adresse->Name|escape}" size="50" maxlength="50" />
    {else}
    {$Adresse->Name}
    {/if}
  </td>
</tr>
<tr>
  <td valign="top">
    <label for="Zusatz1">Zusatz</label>
  </td>
  <td colspan="3">
    {if $Speichern}
    <textarea id="Zusatz1" name="docinput[Zusatz]" cols="50" rows="2">{$Adresse->Zusatz}</textarea>
    {else}
    {$Adresse->Zusatz|nl2br}
    {/if}
  </td>
</tr>
<tr>
  <td>
    <label for="Vorname">Vorname</label>
  </td>
  <td>
    {if $Speichern}
    <input id="Vorname" type="Text" name="docinput[Vorname]" value="{$Adresse->Vorname|escape}" size="20" maxlength="50" />
    {else}
    {$Adresse->Vorname}
    {/if}
  </td>
  <td>
    <label for="GebTag">Geburtstag</label>
  </td>
  <td>
    {if $Speichern}
    <input id="GebTag" type="Text" name="docinput[Geburtsdatum]" 
      value="{if $Adresse->Geburtsdatum !=0}{$Adresse->Geburtsdatum|date_format:"%d.%m.%Y"}{/if}" 
      size="10" maxlength="10"
      onClick="popUpCalendar(this,Eingabe['docinput[Geburtsdatum]'],'dd.mm.yyyy')"
      onBlur="autoCorrectDate('Eingabe','docinput[Geburtsdatum]', false )" />(tt.mm.jjjj)
    {else}
    {if $Adresse->Geburtsdatum != 0}{$Adresse->Geburtsdatum|date_format:"%d.%m.%Y"}{/if}
    {/if}
  </td>
</tr>
<tr>
  <td>
    <label for="Strasse">Strasse</label>
  </td>
  <td colspan="3">
    {if $Speichern}
    <input id="Strasse" type="Text" name="docinput[Strasse]" value="{$Adresse->Strasse|escape}" size="50" maxlength="50" />
    {else}
    {$Adresse->Strasse}
    {/if}
  </td>
</tr>
<tr>
  <td>
    <label for="PLZ">Land, PLZ, Ort</label>
  </td>
  <td colspan="3">
    {if $Speichern}
      <input id="Land" type="Text" name="docinput[Land]" value="{$Adresse->Land}" size="1" maxlength="5" />
      <input id="PLZ" type="Text" name="docinput[PLZ]" value="{$Adresse->PLZ}" size="5" maxlength="5" />
      <input id="Ort" type="Text" name="docinput[Ort]" value="{$Adresse->Ort|escape}" size="30" maxlength="50" />
    {else}
    {$Adresse->Land} {$Adresse->PLZ} {$Adresse->Ort}
    {/if}
  </td>
</tr>
<tr>
  <td>
    <label for="Telefon1">Telefon 1</label>
  </td>
  <td>
    {if $Speichern}
    <input id="Telefon1" type="Text" name="docinput[Telefon1]" value="{$Adresse->Telefon1|escape}" size="20" maxlength="20" />
    {else}
    {$Adresse->Telefon1}
    {/if}
  </td>
  <td>
    <label for="Telefon2">Telefon 2</label>
  </td>
  <td>
    {if $Speichern}
    <input id="Telefon2" type="Text" name="docinput[Telefon2]" value="{$Adresse->Telefon2|escape}" size="20" maxlength="20" />
    {else}
    {$Adresse->Telefon2}
    {/if}
  </td>
</tr>
<tr>
  <td>
    <label for="Fax">Fax</label>
  </td>
  <td>
    {if $Speichern}
    <input id="Fax" type="Text" name="docinput[Fax]" value="{$Adresse->Fax|escape}" size="20" maxlength="20" />
    {else}
    {$Adresse->Fax}
    {/if}
  </td>
  <td>
    <label for="eMail">eMail</label>
  </td>
  <td>
    {if $Speichern}
    <input id="eMail" type="Text" name="docinput[Email]" value="{$Adresse->Email|escape}" size="40" maxlength="90" />
    {else}
    {$Adresse->Email}
    {/if}
  </td>
</tr>
<tr>
  <td>
    <label for="docinput[Rabattsatz]">Rabattsatz</label>
  </td>
  <td>
    {if $Speichern}
    <input type="Text" name="docinput[Rabattsatz]" value="{$Adresse->Rabattsatz|string_format:"%.1f"}" size="3" maxlength="5">
    {else}
    {$Adresse->Rabattsatz|string_format:"%.1f"}
    {/if} %
  </td>
  <td>Korrespondenz per E-Mail</td>
  <td>
    <input type="Checkbox" name="docinput[ImmerPerEMail]" value="v" {if !$Speichern}disabled="disabled"{/if}
    {if $Adresse->ImmerPerEMail}
      checked="checked"
    {/if} />
  </td>
</tr>
<tr>
  <td colspan="4">
    <label for="Bemerkungen">Bemerkungen</label><br />
    {if $Speichern}
    <textarea id="Bemerkungen" name="docinput[Bemerkungen]" cols="60" rows="5">{$Adresse->Bemerkungen}</textarea>
    {else}
    {$Adresse->Bemerkungen|nl2br}
    {/if}
  </td>
</tr>
<tr>
  <td>
    Letzte<br />Aktualisierung
  </td>
  <td valign="top">
    {$Adresse->Stand|date_format:"%d.%m.%Y %H:%M"}
  </td>
  {if $Adresse->Adressen_id > 0 && $Speichern}
  <td colspan="2" align="right">
    [ <a href="?id={$id}&Kopie={$Adresse->Adressen_id}" 
      title="Neuen Datensatz als Kopie hiervon erstellen">Kopie</a> ] 
    [ <a href="?id={$id}&Kopie={$Adresse->Adressen_id}&Kat=O" 
      title="Neuen Datensatz als Institutionskopie hiervon erstellen">Kopie &uarr;</a> ] 
    [ <a href="?id={$id}&Kopie={$Adresse->Adressen_id}&Kat=U" 
      title="Neuen Datensatz als Ansprechpartnerkopie hiervon erstellen">Kopie &darr;</a> ] 
    {if $Adresse->Kunden_Nr <= 0 && ! $Adresse->isNeu()}
       [&nbsp;<a href="?id={$id}&Delete={$Adresse->Adressen_id}" title="Datensatz komplett löschen" 
        onClick="javascript:return window.confirm('Adresse wirklich löschen?');">Löschen</a>&nbsp;] 
    {/if}
    [&nbsp;<a href="?id={$id}&docinput[Edit]=3&docinput[Institution]={$Adresse->Adressen_id}" 
      title="Ansprechpartner festlegen">Ansprechpartner</a>&nbsp;] 
    [&nbsp;<a href="?id={$id}&docinput[Edit]=2&docinput[Zugehoerig]={$Adresse->Adressen_id}" 
      title="Institutionen festlegen">Institutionen</a>&nbsp;] 
  </td>
  {/if}
</tr>
{if $Adresse->Adressen_id> 0}
<tr>
  <td colspan="4" class="zentriert">
     {$Vorlagen}
  </td>
</tr>
{/if}
{if $Speichern} 
<tr>
  <td class="zentriert" colspan="4"><input type="Submit" name="Save" value="Kontaktdaten Speichern" />
  {if $Adresse->Adresen_id> 0}
    &nbsp;&nbsp;<a href="?id={$id}&Bearbeiten={$Adresse->Adressen_id}&Plain=1#Copy">Daten für Copy&Paste anzeigen</a>
  {/if}
  </td>
</tr>
{/if}
{if isset($smarty.session.Sem)}
<tr>
  <td class="zentriert" colspan="4">
    <a href="javascript:opener.location.search ='?{$smarty.session.Sem}';this.close();">Zurück zum Seminar</a>
  </td>
</tr>
{/if}
</form>
</table>

{if $Adresse->Adressen_id > 0}
  {if Count($Adresse->getAnsprechpartner()) > 0}
<table class="volleTabelle">        
<tr class="ueberschrift">
  <th colspan="6">
    Ansprechpartner
  </th>
</tr>
<tr><td>Name</td><td>Telefon 1</td><td>Telefon 2</td><td>Fax</td><td>Vorlage</td><td></td></tr>
{foreach from=$Adresse->getAnsprechpartner() item=adr}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    <a href="?id={$id}&Bearbeiten={$adr->Adressen_id}">{$adr->Vollname()}</a>
  </td>
  <td>{$adr->Telefon1}</td>
  <td>{$adr->Telefon2}</td>
  <td>{$adr->Fax}</td>
  <td>{$adr->Vorlagen}</td>
  <td>
    <a href="?id={$id}&docinput[Institution]={$Adresse->Adressen_id}&docinput[AdressenNr][]={$adr->Adressen_id}&docinput[Edit]=3">
      <img src="/img/small_edit/delete.gif" title="Löschen" alt="Kreuz"/>    
	</a>
  </td>
</tr>
{/foreach}
</table>
  {/if}
  {if $Adresse->getInstitutionenAnzahl() > 0}
<table class="volleTabelle">        
<tr class="ueberschrift">
  <th colspan="6">
    Institutionen
  </th>
</tr>
<tr><td>Name</td><td>Telefon 1</td><td>Telefon 2</td><td>Fax</td><td>Vorlage</td><td></td></tr>
{foreach from=$Adresse->getInstitutionen() item=adr}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    <a href="?id={$id}&Bearbeiten={$adr->Adressen_id}">{$adr->Vollname()}</a>
  </td>
  <td>{$adr->Telefon1}</td>
  <td>{$adr->Telefon2}</td>
  <td>{$adr->Fax}</td>
  <td>{$adr->listeVorlagen($Adresse->Adressen_id,$Vorlagenliste)}</td>
  <td>
    <a href="?id={$id}&docinput[Zugehoerig]={$Adresse->Adressen_id}&docinput[AdressenNr][]={$adr->Adressen_id}&docinput[Edit]=2">
      <img src="/img/small_edit/delete.gif" title="Löschen" alt="Kreuz"/>
    </a>
  </td>
</tr>
{/foreach}
</table>
  {/if}
<div class="ueberschriftzeile">Kategorien</div>
<div id="Kategorien">
    {include file="Adressen_Kategorien_Liste.tpl"}
</div>
  {if $Speichern}
<div class="zentriert">
[ <a id="KategorienLink" href="#" 
  onClick="xajax_zeigeKategorien({$Adresse->Adressen_id},true); return false;">Kategorien festlegen</a> ]
</div>
  {/if}
{if isset($Bemerkung) || Count($Adresse->getBemerkungen()) > 0}
<div class="ueberschriftzeile">
    <a id="AddBem" name="AddBem"></a>Sonstige Informationen
</div>
{/if}
{if isset($Bemerkung)}
<form action="?id={$id}&Bearbeiten={$Adresse->Adressen_id}#AddBem" method="post" enctype="multipart/form-data" name="Bem">
<label for="Bemerkung">Bemerkung</label><br />
<textarea name="docinput[Bemerkung]" cols="60" rows="5">{$Bemerkung.Bemerkung}</textarea>
{if is_numeric($Bemerkung.Bemerkung_id)}
<input type="hidden" name="docinput[Bemerkung_id]" value="{$Bemerkung.Bemerkung_id}" />
{/if}<br />
<label for="Datum">Datum</label> 
<input type="Text" id="Datum" name="docinput[Datum]" value="{$Bemerkung.Datum|date_format:"%d.%m.%Y"}" 
size="10" maxlength="10"
  onClick="popUpCalendar(this,Bem['docinput[Datum]'],'dd.mm.yyyy')"
  onBlur="autoCorrectDate('Bem','docinput[Datum]', false )" /> 
<label for="Wiedervorlage">Wiedervorlage</label>
<input titel="Wiedervorlagedatum oder Anzahl Tage nach Datum angeben" 
type="Text" id="Wiedervorlage" name="docinput[Wiedervorlage]" 
  value="{if $Bemerkung.Wiedervorlage> 0}{$Bemerkung.Wiedervorlage|date_format:"%d.%m.%Y"}"{/if} 
  size="10" maxlength="10"
    onClick="popUpCalendar(this,Bem['docinput[Wiedervorlage]'],'dd.mm.yyyy')"
    onBlur="autoCorrectDate('Bem','docinput[Wiedervorlage]', false )" /><br />
<label for="Anhang">Dateianhang</label> 
{if $Bemerkung.Anhang == ''}
  <input type="file" name="Anhang" />
{else}
  <a target="_blank" href="{$Bemerkung.Anhanglink}">{$Bemerkung.Anhang}</a>
  &nbsp;&nbsp;&nbsp;[ <a href="?id={$id}&Bearbeiten={$Adresse->Adressen_id}&AddBem={$Bemerkung.Bemerkung_id}&DelAnhang=1#AddBem">Löschen</a> ]
{/if}
<div class="zentriert"><input type="Submit" name="BemSave" value="Bemerkung speichern"/></div>
</form>
{/if}

{foreach from=$Adresse->getBemerkungen() item=adr}
<div class="helleZeile">
    <a href="?id={$id}&Bearbeiten={$Adresse->Adressen_id}&AddBem={$adr->Bemerkung_id}#AddBem">
      {$adr.Datum|date_format:"%d.%m.%Y"}
    </a>
    {if $adr.Wiedervorlage > 0}
    &nbsp;&nbsp;&nbsp;(Wiedervorlage am {$adr.Wiedervorlage|date_format:"%d.%m.%Y"})
    {/if}
</div>
{$adr.Bemerkung|nl2br}
{if $adr.Anhang !=''}
  (Anlage: <a target="_blank" href="{$adr.Anhanglink}">{$adr.Anhang})</a>
{/if}<br />
{/foreach}
<div class="zentriert">
  <a href="?id={$id}&Bearbeiten={$Adresse->Adressen_id}&AddBem=-1#AddBem">Bemerkung hinzufügen</a>
</div>
{if Count($Adresse->getBuchungen())>0}
<table class="volleTabelle">
<tr class="ueberschrift">
  <th colspan="4">
    <a id="Buchung" name="Buchung"></a>{$Adresse->getBuchungen()|@Count} Buchungen
  </th>
</tr>
{foreach from=$Adresse->getBuchungen() item=Buchung}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td colspan="4">
    {if	$Buchung->F_Adressen_id!=$Adresse->Adressen_id}Seminar:{/if} 
	<a href="{$Buchungurl}&Buchung_Nr={$Buchung->Buchung_Nr}">     
      {$Buchung->Buchung_Nr} - {$Buchung->Von|date_format:"%A %d.%m.%Y"} bis
      {$Buchung->Bis|date_format:"%A %d.%m.%Y"} mit {$Buchung->personenAnzahl()} Personen 
      ({$Buchung->getAltersgruppe()} / {$Buchung->Buchungsstatus()})</a>
    &nbsp;&nbsp;[ <a href="{$Buchungsuebersichturl}&Buchung_Nr={$Buchung->Buchung_Nr}">Belegungsübersicht</a> ] 
      [ <a href="{$Buchungsbemerkungenurl}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}">Bemerkungen</a> ]
  </td>
</tr>
{/foreach}
</table>
{/if}

{$Korrespondenz}

{if Count($Adresse->getArtikel())>0}
<table class="volleTabelle">
<tr class="ueberschrift">
  <th colspan="4">
    <a id="Buchung" name="Buchung"></a>{$Adresse->getArtikel()|@Count} Artikel bestellbar
  </th>
</tr>
{foreach from=$Adresse->getArtikel() item=artikel}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    <a href="{$Artikelurl}&docinput[Artikel_Nr]={$artikel.id}">{$artikel.Bezeichnung}</a>
  </td>
  <td>
    {$artikel.Artikelart}
  </td>
  <td>
    {$artikel.Bestellnummer}
  </td>
  <td align="right">
    {$artikel.Einkaufspreis|string_format:"%0.2f"}
  </td>
</tr>        
{/foreach}
</table>
{/if}
{/if} <!--  Adresse_id > 0 -->     


<div class="zentriert">
  [ <a href="?id={$id}">alle Adressen anzeigen</a> ]
  {if $Adresse->Adressen_id > 0 && $Speichern}
    &nbsp;&nbsp;[ <a href="{$Buchungurl}&Buchung_Nr=-1&Adressen_id={$Adresse->Adressen_id}" title="Buchung vornehmen">Neue Buchung anlegen</a> ]
  {/if}
</div>
<strong>Historie</strong>
<div id="Historie">
{$Adresse->holeHistory()|nl2br}
</div>
