{* Template Buchung_Daten - Allgemeine Buchungsdaten *}
{popup_init src="/javascript/overlib.js"}
<!--  Popup-Kalender -->
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>
<script language="javascript">
{literal}
function aenderePersonenAnzahl()
{ 
	var anzahl = 0;
	for(i=1;i<7;i++)
	{
		anzahl += parseInt(document.getElementById('Personen'+i).value);
		anzahl += parseInt(document.getElementById('Personen'+i+'w').value);
	}
	document.getElementById('Personenanzahl').innerHTML = anzahl;
}
var originalDatum = "{/literal}{$Buchung->Von|date_format:"%d.%m.%Y"}{literal}";
function uebertrageDatum(wert)
{ 
	// Wenn neue Buchung und Originaldaten  
	if ( document.getElementById('Bis').value == originalDatum ) 
	{
		document.getElementById('Bis').value = document.getElementById('Von').value;
		originalDatum = document.getElementById('Von').value;
	}
}
{/literal}
</script>
<form action="?id={$id}" method="post" name="Buchung">
<table width="100%">
<tr class="ueberschrift"><th colspan="4">
  {if $Buchung->isNeu()}
    neue <input type="hidden" name="docinput[F_Adressen_id]" value="{$Buchung->F_Adressen_id}" />
  {else}
    <input type="hidden" name="docinput[Buchung_Nr]" value="{$Buchung->Buchung_Nr}" />
  {/if}
  {if $Buchung->Seminar}
    Seminar
  {else}
    Buchung
  {/if}
  {if ! $Buchung->isNeu()}
    <input type="text" name="docinput[Neu_Buchung_Nr]" value="{$Buchung->Buchung_Nr}" {$check1} />
    <span class="KleineSchrift">erstellt am {$Buchung->getFeld('ErstelltAm')}, zuletzt geändert {$Buchung->BuchungStand|date_format:"%d.%m.%Y %H:%M"}</span>
  {/if}
  </th>
</tr>
<tr>
  <td>
    Kunde</td><td colspan="3">
    <div style="float:right" id="KundeAendern">    
      {if ! $Buchung->isNeu() && ! $KundeAendern}
      [ <a href="#" onClick="xajax_showKundenAuswahl(); return false;">
      Kunde ändern</a> ]{/if}
    </div>
    {assign value=$Buchung->Adresse->Uebersicht(true) var=adresse}
	<a	href="{$Adressebearbeiten}&Bearbeiten={$Buchung->Adresse->Adressen_id}"
			{popup text=$adresse|escape caption="Kontaktdaten bearbeiten"}>{$Buchung->Adresse->Vollname()}</a> 
    (
    {$Buchung->Adresse->Kunden_Nr}
    ) &nbsp;{$Buchung->Adresse->Zusatz}<br />
    <div id="KundeAendernFeld"></div>
    {if $Buchung->Adresse->Email != ''}
    <div><a href="#Korrespondenz">Korrespondenz per E-Mail an {$Buchung->Adresse->Email}</a></div>
    {/if}
  </td>
</tr>
{if Count($Buchung->getInstitutionsliste()) > 0}
<tr>
  <td><label for="Institution">Institution</label></td><td colspan="3">
    <select id="Institution" name="docinput[F_Institution]" {$check2} onChange="xajax_showInstitutionMail(document.getElementById('Institution').value)">
      {html_options options=$Buchung->getInstitutionsliste() selected=$Buchung->F_Institution}
    </select>
    <span id="Institutionsmail">{if $Buchung->F_Institution > 0  && $Buchung->Institution->Email != ''}<a href="#Korrespondenz">Korrespondenz an {$Buchung->Institution->Email}</a>{/if}</span>
  </td>
</tr>
{/if}
<tr>
  <td><label for="Von">Von</label></td>
  <td>{$Buchung->Von|date_format:"%A"}<br />
    <input id="Von" type="Text" name="docinput[Von]" value="{$Buchung->Von|date_format:"%d.%m.%Y"}" 
    size="10" maxlength="15"
    {if $Buchung->isFertig() || Count($Buchung->getBuchungseintraege()) != 0} readonly="readonly"{else}
      onClick="popUpCalendar(this,Buchung['docinput[Von]'],'dd.mm.yyyy')"
      onBlur="autoCorrectDate('Buchung','docinput[Von]',false);"      
    {/if} />
    <input type="Text" name="docinput[Ankunftszeit]" 
    value="{if $Buchung->Ankunftszeit != 0}{$Buchung->Ankunftszeit|date_format:"%H:%M"}{/if}" 
    size="5" maxlength="5" {$check1} />
  </td>
  <td><label for="Bis">Bis</label></td>
  <td>{$Buchung->Bis|date_format:"%A"}<br />
    <input id="Bis" type="Text" name="docinput[Bis]" title="Abfahrtsdatum oder Anzahl Tage" 
      value="{$Buchung->Bis|date_format:"%d.%m.%Y"}" size="10" maxlength="15"
    {if $Buchung->isFertig() || Count($Buchung->getBuchungseintraege()) != 0} readonly="readonly"{else}
      onFocus="uebertrageDatum();"
      onClick="popUpCalendar(this,Buchung['docinput[Bis]'],'dd.mm.yyyy')"
      onBlur="autoCorrectDate('Buchung','docinput[Bis]' , false )"      
      {/if} />
     <input type="Text" name="docinput[Abfahrtszeit]" 
       value="{if $Buchung->Abfahrtszeit != 0}{$Buchung->Abfahrtszeit|date_format:"%H:%M"}{/if}" 
       size="4" maxlength="5" {$check1} /> {if
		$Buchung->berechneUebernachtungen()< 0 ||
		$Buchung->berechneUebernachtungen() > 13}<span class="SchwererFehler">{/if}
		({$Buchung->berechneUebernachtungen()} Übernachtungen) {if
		$Buchung->berechneUebernachtungen() < 0 ||
		$Buchung->berechneUebernachtungen()> 13}</span>{/if}
    {if $Speichern && $Buchung->Buchung_Nr > 0 && ! $Verschieben}
    <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&docinput[Verschieben]=1"
    title="Buchungszeitraum verändern">Verschieben</a>
    {/if}
  </td>
</tr>
{if $Verschieben}
<tr>
  <td>Neuer Beginn</td>
  <td>
    <input id="Von" type="Text" name="docinput[NeuVon]" value="{$Buchung->Von|date_format:"%d.%m.%Y"}" size="10" maxlength="15"
      onClick="popUpCalendar(this,Buchung['docinput[NeuVon]'],'dd.mm.yyyy')"
      onBlur="autoCorrectDate('Buchung','docinput[NeuVon]' , false )"
     />
   </td>
   <td>Neues Ende</td>
   <td>
     <input id="Von" type="Text" name="docinput[NeuBis]" value="{$Buchung->Bis|date_format:"%d.%m.%Y"}" 
     size="10" maxlength="15"
     onClick="popUpCalendar(this,Buchung['docinput[NeuBis]'],'dd.mm.yyyy')"
     onBlur="autoCorrectDate('Buchung','docinput[NeuBis]' , false )"
     />
   </td>
</tr>
{/if}
{if ! $Buchung->isSelbstverpflegung()}
<tr>
  <td>erste Mahlzeit</td>
  <td>
  {if $Buchung->ersteMahlzeit('Datum') != 0}
    {if $Buchung->ersteMahlzeit('Datum') != $Buchung->Von}<span class="Achtung">{/if}
    {$Buchung->ersteMahlzeit('Bezeichnung')}
    {if $Buchung->ersteMahlzeit('Datum') != $Buchung->Von} am {$Buchung->ersteMahlzeit('Datum')|date_format:"%d.%m.%Y"}</span>{/if}
  {else}
  <span class="Fehler">nicht festgelegt</span>
  {/if}
  </td>
  <td>letzte Mahlzeit</td>
  <td>
    {if $Buchung->ersteMahlzeit('Datum') != 0}
      {if $Buchung->letzteMahlzeit('Datum') != $Buchung->Bis}<span class="Achtung">{/if}
      {$Buchung->letzteMahlzeit('Bezeichnung')}
      {if $Buchung->letzteMahlzeit('Datum') != $Buchung->Bis} am {$Buchung->letzteMahlzeit('Datum')|date_format:"%d.%m.%Y"}</span>{/if}
    {else}
    <span class="Fehler">nicht festgelegt</span>
    {/if}
  </td>
</tr>
{/if}
<tr>
  <td>Buchungsstatus</td>
  <td><strong>
    {$Buchung->Buchungsstatus()}
  {if !$Buchung->isNeu()}
  {if $Buchung->isFertig()}
    <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&docinput[Reaktiv]=1" 
    onClick="javascript:return window.confirm('Buchung {$Buchung->Buchung_Nr} wirklich wieder zur Bearbeitung freigeben?');" title="Bearbeiten">
    <img src="/img/edit.gif"/></a>
  {else}
    <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&docinput[Reaktiv]=2" 
    onClick="javascript:return window.confirm('Buchung {$Buchung->Buchung_Nr} wirklich als interne Buchung abhaken?');" 
    title="zu interner Buchung machen">
    (&rarr;Intern)</a>
  {/if}
  {/if}
  </strong></td>
  <td><label for="Eingang">Res.-Eingang</label></td>
  <td><input id="Eingang" type="Text" name="docinput[Eingang]" size="8" maxlength="10" 
  value="{if $Buchung->Eingang > 0}{$Buchung->Eingang|date_format:"%d.%m.%Y"}{/if}"
  {if $Buchung->isFertig()}{$check1}{else}
     onClick="popUpCalendar(this,Buchung['docinput[Eingang]'],'dd.mm.yyyy')"
     onBlur="autoCorrectDate('Buchung','docinput[Eingang]' , false )"
  {/if} /> Altersgruppe 
    <select name="docinput[Altersgruppe]" {$check2}>
      {html_options options=$Buchung->getAltersgruppen() selected=$Buchung->Altersgruppe}
    </select>
  </td>
</tr>
<tr>
  <td><label for="Anzahlung">Anzahlung</label></td>
  <td>
    <input type="Text" name="docinput[Anzahlung]" value="{$Buchung->Anzahlung}" size="7" maxlength="10" {$check1} /> &euro; 
    <input type="Text" name="docinput[AnzahlungsBemerkung]" value="{$Buchung->AnzahlungsBemerkung|escape}" size="7" maxlength="20" {$check1} />
  </td>
  <td>Preisliste</td>
  <td>
    <select id="Preisliste" name="docinput[F_Preisliste_id]" {$check2}>
      {html_options values=$Preislisten_values selected=$Buchung->F_Preisliste_id output=$Preislisten_output}  
    </select>
  </td>
</tr>
<tr>
  <td>Personen-<br />anzahlen</td>
  <td>
  {assign var="tmpcheck1" value="$check1"}
  {if $Buchung->Seminar}{assign var="tmpcheck1" value="readonly=\"readonly\""}{/if}
  {foreach from=$Buchung->getPersonenListe() key=nr item=wert}
    m&nbsp;<input type="Text" name="docinput[Personen{$nr}]" id="Personen{$nr}"
    value="{$wert.m}" size="2" maxlength="3" 
    {$tmpcheck1} onKeyUp="javascript:aenderePersonenAnzahl();" />
    w&nbsp;<input type="Text" name="docinput[Personen{$nr}w]"  id="Personen{$nr}w"
    value="{$wert.w}" size="2" maxlength="3" 
    {$tmpcheck1} onKeyUp="javascript:aenderePersonenAnzahl();"/> 
    &nbsp;{$Buchung->getAlterswertName($nr)}<br />
  {/foreach}
  <strong>Gesamt: <span id="Personenanzahl">{$Buchung->personenAnzahl()}</span></strong> 
  <span {if $Buchung->personenAnzahl() > $Buchung->BerechneAlleSchlafplaetze()} class="SchwererFehler"{/if} title="gebuchte Schlafplätze">
  <small>
  {if $Buchung->BerechneAlleSchlafplaetze() < 0} keine! 
  {else}({$Buchung->BerechneAlleSchlafplaetze()} SP){/if}
  </small>
  </span>
  </td><td colspan="2">
  <label for="Vereinbarungen">Besondere Vereinbarungen</label><br />
  <textarea id="Vereinbarungen" name="docinput[Vereinbarungen]" cols="50" rows="1" {$check1}>{$Buchung->Vereinbarungen}</textarea>
  <br />
  <label for="Essenbesonderheit">Besonderheiten beim Essen</label><br />
  <textarea id="Essenbesonderheit" name="docinput[Essenbesonderheit]" cols="50" rows="1" 
  {if $Buchung->isSelbstverpflegung()}readonly="readonly"{else}{$check1}{/if}>{$Buchung->Essenbesonderheit}</textarea>
  <br />
  <label for="Vegetarier">Vegetarier</label> 
  <input type="Text" id="Vegetarier" name="docinput[Vegetarier]" value="{$Buchung->Vegetarier}" size="2" maxlength="3" 
  {if $Buchung->isSelbstverpflegung()}readonly="readonly"{else}{$check1}{/if}
   />
    <label for="Schweinelos">ohne Schweinefleisch</label>
   <input type="Text" id="Schweinelos" name="docinput[Schweinelos]" value="{$Buchung->Schweinelos}" size="2" maxlength="3" 
  {if $Buchung->isSelbstverpflegung()} readonly="readonly"{else}{$check1}{/if}
   />
  Speiseraum: 
  <select name="docinput[F_Speiseraum_id]" {$check2} title="[] belegt, &larr; bis Beginn der Buchung, &rarr; ab Ende der Buchung">
  {html_options options=$Buchung->getSpeiseraumListe() selected=$Buchung->F_Speiseraum_id}
  </select><br />
  {if $Speichern} 
    <br />
    {if $Buchung->isSelbstverpflegung()}
      <span class="Selbstverpflegung">Selbstverpfleger!</span>&nbsp;
      <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&docinput[Selbstverpflegung]=N">
      Selbstverpflegung löschen</a>
    {else}
      {if ! $Buchung->verpflegungVorhanden() && ! $Buchung->isNeu()}
      <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&docinput[Selbstverpflegung]=J">
      Selbstverpflegung setzen</a>
    {/if}{/if}
  {/if}
  </td>
</tr>
<tr><td
  {if $Buchung->BetreueranzahlM+$Buchung->BetreueranzahlW > $Buchung->personenAnzahl()}
     class="SchwererFehler"
  {/if}>
  <label for="Betreueranzahl">davon<br />Betreueranzahl</label></td><td>
  m <input id="Betreueranzahl" type="Text" name="docinput[BetreueranzahlM]" 
    value="{$Buchung->BetreueranzahlM}" size="2" maxlength="3" {$check1} />
  w <input type="Text" name="docinput[BetreueranzahlW]" value="{$Buchung->BetreueranzahlW}" 
    size="2" maxlength="3" {$check1} />
  </td>
  <td>
    <input type="Checkbox" id="Kuechenhilfe" name="docinput[Kuechenhilfe]" value="v"
    {if $Buchung->Kuechenhilfe} checked="checked"{/if} {$check2} />
    <label for="Kuechenhilfe">Mithilfe in der K&uuml;che</label>
  </td>
  <td>
  {if Count($Buchung->getBuchungseintraege()) > 0}
    &nbsp;&nbsp;&nbsp;Gruppenraum o.B. 
    <select name="docinput[F_Aufenthaltsraum_id]" {$check2}>
      {html_options options=$Buchung->getGruppenraumListe() selected=$Buchung->F_Aufenthaltsraum_id}
    </select>
  {/if}
  </td>
</tr>
<tr>
  <td><label for="Bemerkungen">Bemerkungen</label><br />(Intern)</td>
  <td colspan="3">
    <textarea id="Bemerkungen" name="docinput[Internes]" cols="60" rows="2" {$check1}>{$Buchung->Internes}</textarea>
  </td>
</tr>
<tr>
  <td>Rabatt</td>
  <td colspan="3">
    <input type="text" name="docinput[Buchungsrabatt]" value="{$Buchung->Buchungsrabatt}" {$check1} /> % f&uuml;r diese Buchung
    &nbsp;&nbsp;&nbsp;&nbsp;Steuerliche Behandlung <select name="docinput[Steuerbehandlung]" {$check2} >
      {html_options values=$Steuerarten_values selected=$Buchung->Steuerbehandlung output=$Steuerarten_output}
    </select>
  </td>
</tr>
</table>
{if $Speichern}
<div class="zentriert">
  <input type="Submit" name="Save" value="Speichern">
</div>
{/if}
</form>

{if (!$Buchung->isNeu() )}
<div class="zentriert">
 [ <a href="{$Buchungsuebersichturl}&docinput[Art]={$smarty.const.CVJMART_ORT}&Buchung_Nr={$Buchung->Buchung_Nr}"
	{if ($Buchung->BerechneAlleSchlafplaetze() < 0)}class="Fehler"{/if}>Belegung</a> ]
  {if ($Buchung->personenAnzahl() > 0)}
 [ <a href="{$Buchungsuebersichturl}&docinput[Art]={$smarty.const.CVJMART_PROGRAMMPAKET}&Buchung_Nr={$Buchung->Buchung_Nr}"
 >Programmpaket</a> ]
 [ <a href="{$Buchungsuebersichturl}&docinput[Art]={$smarty.const.CVJMART_PROGRAMM}&Buchung_Nr={$Buchung->Buchung_Nr}"
 >Programmmodul</a> ]
 [ <a href="{$Buchungsuebersichturl}&docinput[Art]={$smarty.const.CVJMART_VERPFLEGUNG}&Buchung_Nr={$Buchung->Buchung_Nr}" 
  {if (!$Buchung->isVerpflegung() && !$Buchung->isSelbstverpflegung())}class="Fehler"{/if} >Verpflegung</a> ]
  {/if}
 [ <a href="{$Buchungsuebersichturl}&docinput[Art]={$smarty.const.CVJMART_VERLEIH}&Buchung_Nr={$Buchung->Buchung_Nr}"
 >Verleih</a> ]
 [ <a {if $Buchung->Abnahmename =='' || $Buchung->Anreisename == ''}class="Fehler"{/if} href="{$Personalplanungurl}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}">Personalplanung</a> ]
  [ <a href="{$Buchung->buchungSmilycardURL()}">Smileycard</a> ]
</div>
<div class="zentriert">
  {if ($Speichern)}
  {if (!$Buchung->Seminar && !$Buchung->isFertig() )}
	[ <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&docinput[Seminar]=1" 
	onClick="javascript:return window.confirm('Diese Buchung Nr. {$Buchung->Buchung_Nr} zum Seminar machen?');">
	zum Seminar machen</a> ]
  {else} 
	 {if ($Buchung->Seminar)}
		[ <a href="{$Seminarurl}&Buchung_Nr={$Buchung->Buchung_Nr}">Seminarverwaltung</a> ] 
	 {/if}
  {/if}
  {if ($Buchung->BStatus <= $smarty.const.BUCHUNG_VORRESERVIERUNG)}
	[ <a href="?id={$id}&docinput[Storno]={$smarty.const.BUCHUNG_GELOESCHT}&docinput[DelBuchung]={$Buchung->Buchung_Nr}" 
	onClick="javascript:return window.confirm('Diese Buchung Nr. {$Buchung->Buchung_Nr} unwiderruflich löschen und Belegung freigeben?');">
	Buchung löschen</a> ] 
  {else}
    {if ($Buchung->BStatus == $smarty.const.BUCHUNG_RESERVIERUNG)}
    [ <a href="{$Abrechnungurl}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}&docinput[Storno]={$smarty.const.BUCHUNG_STORNIERT}"
		onClick="javascript:return window.confirm('Diese Buchung Nr. {$Buchung->Buchung_Nr} unwiderruflich stornieren und Belegung freigeben?');">
		Buchung stornieren</a> ] 
	{/if}
  {/if}
  {if (Count($Buchung->getBuchungseintraege()) > 0)}
	 {if (!$Buchung->isFertig())}
		<span class="Achtung">[ <a href="?id={$id}&docinput[DelBuchung]={$Buchung->Buchung_Nr}" 
		onClick="javascript:return window.confirm('Alle Buchungseinträge von dieser Buchung löschen?');">
		alle Einträge ausbuchen</a> ]</span>
	 {/if}
	 [ <a href="{$Abrechnungurl}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}">Neue Abrechnung</a> ]
  {/if}
  <br />
  {foreach from=$Rechnungen item=rechnung}
	[ <a href="{$Abrechnungurl}&docinput[Rechnung_id]={$rechnung.Rechnung_id}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}">
		{if ($rechnung.Rechnung == 1)}
			Rechnung {$rechnung.Rechnungsnummer}
		{else}
			Berechnung {$rechnung.Rechnung_id}
		{/if}
		 vom {$rechnung.Rechnungsdatum|date_format:"%d.%m.%Y"}</a> ]
  {/foreach}
  <br />
  {/if} {* Speichern *}
  {$Vorlagen}
[ <a href="{$Auftragurl}&newauf=1&docinput[VTitel]=Buchung {$Buchung->Buchung_Nr}">
  Auftrag zu dieser Buchung</a> ] 
[ <a href="{$Buchungsbemerkungenurl}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}">
Bemerkungen hinzufügen</a> ] 
{/if} {* nicht neu *}
&nbsp;[ <a href="?id={$id}">Buchungsliste anzeigen</a> ] 
</div>

{if $Speichern}
<div id="Artikel_Hinzu">
	{include file="Buchung_Artikel_hinzu.tpl"}
</div>
{/if}
<div id="Personenwahldialog"></div>
<div id="Artikeltabelle">
<table width="100%">
<tr class="ueberschrift">
  <th colspan="8" class="zentriert">
	<a id="Einzelheiten" name="Einzelheiten"></a>Einzelheiten
  </th>
</tr>
{if (!$Buchung->isFertig() && $Speichern)}
<tr>
  <td colspan="7" class="zentriert">
	{if (!isset ($smarty.request.Alle))}
		[ <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&Edit=1&Alle=1#Einzelheiten">
		Einträge einzeln bearbeiten</a> ]&nbsp;&nbsp;
	{/if}
	{if (isset ($smarty.request.Alle) || !isset ($smarty.request.Edit))}
		[ <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&Edit=1#Einzelheiten">
		Einträge bearbeiten</a> ]
	{/if}
	{if (isset ($smarty.request.Edit))}
		&nbsp;&nbsp; [ <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}#Einzelheiten">
		Bearbeitung abbrechen</a> ]
	{/if}
	</td>
</tr>
{/if}
<tr>
  <td colspan="2">Menge</td><td>Dauer</td><td>Einheit</td><td>Artikel</td><td>Datum&nbsp;&nbsp;
{if (isset ($smarty.request.Alle))}
	[ <a href="?id={$id}&{$params}#Einzelheiten">Zusammenfassung</a> ]
{else}
	[ <a href="?id={$id}&{$params}&Alle=1#Einzelheiten">jedes Datum einzeln</a> ]
{/if}
	</td><td>Betten</td><td>Unberechnet</td>
</tr>
{if (isset ($smarty.request.Edit) && $Speichern)}
	<form action="?id={$id}#Einzelheiten" method="post">
	<input type="hidden" name="Buchung_Nr" value="{$Buchung->Buchung_Nr}">
{/if}
{$eintraege}
<tr>
  <td colspan="3"></td>
  <td colspan="3"><strong><em>Gesamtsumme</em></strong> ({$Eintragsanzahl} Einträge)</td>
	<td class="zentriert">
	{if ($Buchung->BerechneAlleSchlafplaetze()>= 0)}{$Buchung->BerechneAlleSchlafplaetze()}{else}keine!{/if}
	</td></tr>
{if (isset ($smarty.request.Edit) && $Speichern)}
<tr>
  <td colspan="7" class="zentriert">
    <input type="Submit" name="SaveEdit" value="Einträge speichern" />
  </td>
</tr>
</form>
{/if}

{if (!$Buchung->isFertig() && $Speichern)}
<tr>
  <td colspan="7" class="zentriert">
  {if (!isset ($smarty.request.Alle))}
	[ <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&Edit=1&Alle=1#Einzelheiten">
	Einträge einzeln bearbeiten</a> ]&nbsp;&nbsp;
  {/if}
	{if (isset ($smarty.request.Alle) || !isset ($smarty.request.Edit))}
	[ <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}&Edit=1#Einzelheiten">Einträge bearbeiten</a> ]
	{/if}
	{if (isset ($smarty.request.Edit))}
	&nbsp;&nbsp; [ <a href="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}#Einzelheiten">Bearbeitung abbrechen</a> ]
	{/if}
	</td>
</tr>
{/if}
</table>
</div>

{if (Count($Buchung->getBemerkungen()) > 0)}
<div class="ueberschriftzeile">
  {$Buchung->getBemerkungen()|@count} Bemerkungen
</div>
{foreach from=$Buchung->getBemerkungen() item=bemerkung}
<div class="{cycle values="helleZeile,dunkleZeile"} ganzeZeile">
{$bemerkung.Bemerkung|nl2br}
&nbsp;&nbsp;[ <a href="{$Buchungsbemerkungenurl}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}">Bearbeiten</a> ]
{if ($bemerkung.Hinweis != '')}
	<br />{$bemerkung.Hinweis|nl2br}
{/if}
</div>
{/foreach}
{/if}

<div class="zentriert">
{if (! $Buchung->isNeu() )}
	[ <a href="{$Buchungsbemerkungenurl}&docinput[Buchung_Nr]={$Buchung->Buchung_Nr}">Bemerkungen hinzufügen</a> ]
{/if}
	 [ <a href="?id={$id}">Buchungsliste anzeigen</a> ]
</div>
{$Korrespondenz}

<hr />
<br />
<strong>Historie</strong>
<div id="Historie">
{$Buchung->holeHistory('%%%&Bearbeiten=')|replace:"%%%":$Adressebearbeiten|nl2br}
</div>
