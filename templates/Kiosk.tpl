<h1>{$title}</h1>
<style type="text/css">
@import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>

<div class="noprint">
<p>Aktueller Kassenstand: {$Kassenstand}</p>

<form action="?id={$id}" method="post" name="Anzeige">
Buchungen vom <input type="text" name="docinput[AnzeigeVon]" 
value="{$docinput.AnzeigeVon|default:$smarty.now|date_format:"%d.%m.%Y"}" 
onClick="popUpCalendar(this,Anzeige['docinput[AnzeigeVon]'],'dd.mm.yyyy')"
onBlur="autoCorrectDate('Anzeige','docinput[AnzeigeVon]' , false )" />
<input type="submit" value="anzeigen" />
</form>
</div>
{if isset($Buchungen)}
<table width="100%" border="1" style="border-collapse:collapse;" class="drucklinien">
<tr>
  <th>Bezeichung</th>
  <th>Preis</th>
  <th>Menge</th>
  <th>Benutzer</th>
</tr>
{assign var=Gesamt value=0}
{foreach from=$Buchungen item=Buchung}
<tr>
  <td>{$Buchung.Bezeichnung}</td>
  <td align="right">{$Buchung.Preis|string_format:"%.2f"}</td>
  <td align="right">{$Buchung.Menge}</td>
  <td>{$Buchung.Benutzer}</td>
</tr> 
{math equation="z+x*y" x=$Buchung.Menge y=$Buchung.Preis z=$Gesamt assign=Gesamt}
{/foreach} 
<tr>
  <td>Gesamt</td>
  <td align="right">{$Gesamt|string_format:"%.2f"}</td>
  <td></td>
  <td></td>  
</tr>
</table>
{/if}

<form action="?id={$id}" method="post" name="Zusammenfassung" class="noprint">
Zusammenfassung nach Artikeln von 
<input type="text" name="docinput[ZusammenfassungVon]" value="{$docinput.ZusammenfassungVon}" 
onClick="popUpCalendar(this,Zusammenfassung['docinput[ZusammenfassungVon]'],'dd.mm.yyyy')"
onBlur="autoCorrectDate('Zusammenfassung','docinput[ZusammenfassungVon]' , false )" /> 
bis
<input type="text" name="docinput[ZusammenfassungBis]" 
value="{$docinput.ZusammenfassungBis|default:$smarty.now|date_format:"%d.%m.%Y"}"
onClick="popUpCalendar(this,Zusammenfassung['docinput[ZusammenfassungBis]'],'dd.mm.yyyy')"
onBlur="autoCorrectDate('Zusammenfassung','docinput[ZusammenfassungBis]' , false )"
/>
<input type="submit" value="anzeigen" />
</form>

{if isset($Zusammenfassung)}
<table width="100%" border="1" style="border-collapse:collapse;;" class="drucklinien">
<tr>
  <th>Bezeichnung</th>
  <th>Menge</th>
</tr>
{foreach from=$Zusammenfassung item=Eintrag key=Artikel}
<tr>
  <td>{$Artikel}</td>
  <td>{$Eintrag}</td>
</tr>
{/foreach}
</table>
<br />
{/if}

<form action="?id={$id}" method="post" name="Arten" class="noprint">
Zusammenfassung nach Art vom <input type="text" name="docinput[TageVon]" 
value="{$docinput.TageVon|default:$smarty.now|date_format:"%d.%m.%Y"}" 
onClick="popUpCalendar(this,Arten['docinput[TageVon]'],'dd.mm.yyyy')"
onBlur="autoCorrectDate('Arten','docinput[TageVon]' , false )" />
bis 
<input type="text" name="docinput[TageBis]" 
value="{$docinput.TageBis|default:$smarty.now|date_format:"%d.%m.%Y"}" 
onClick="popUpCalendar(this,Arten['docinput[TageBis]'],'dd.mm.yyyy')"
onBlur="autoCorrectDate('Arten','docinput[TageBis]' , false )" />

<input type="submit" value="anzeigen" />
</form>

{if isset($Arten)}
<table width="100%" border="1" style="border-collapse:collapse;" class="drucklinien">
<tr>
  <th>Art</th>
  <th>Umsatz</th>
</tr>
{assign var=Gesamt value=0}
{foreach from=$Arten item=Art}
<tr>
  <td>{$Art.Art|default:"Verwaltungsbuchung"}</td>
  <td align="right">{$Art.Umsatz|string_format:"%.2f"}</td>
</tr> 
{math equation="z+x" x=$Art.Umsatz z=$Gesamt assign=Gesamt}
{/foreach} 
<tr>
  <td>Gesamt</td>
  <td align="right">{$Gesamt|string_format:"%.2f"}</td>  
</tr>
</table>
<br /> 
{/if}

<form action="?id={$id}" method="post" name="Umsatz" class="noprint">
Umsatzsumme von 
<input type="text" name="docinput[UmsatzVon]" value="{$docinput.UmsatzVon}" 
onClick="popUpCalendar(this,Umsatz['docinput[UmsatzVon]'],'dd.mm.yyyy')"
onBlur="autoCorrectDate('Umsatz','docinput[UmsatzVon]' , false )" /> 
bis
<input type="text" name="docinput[UmsatzBis]" 
value="{$docinput.UmsatzBis|default:$smarty.now|date_format:"%d.%m.%Y"}"
onClick="popUpCalendar(this,Umsatz['docinput[UmsatzBis]'],'dd.mm.yyyy')"
onBlur="autoCorrectDate('Umsatz','docinput[UmsatzBis]' , false )"
/>
<input type="submit" value="anzeigen" />
</form>

{if isset($Umsatz)}
<strong>Umsatz im Zeitraum: {$Umsatz|string_format:"%.2f"}.</strong>
<br />
<br />
{/if} 

<form action="?id={$id}" method="post" name="Arten" class="noprint">
Geldbuchung von Summe <input type="text" name="docinput[Geld]" />
<input type="submit" value="hinzufügen" />
</form>

{if Count($Mitarbeiter)== 0}
Es gibt momentan keine Einträge auf Mitarbeiterlisten.
{else}
<div class="noprint">
Mitarbeiter haben {$Mitarbeiterbetrag} Euro auf ihren Listen.<br />
<form action="?id={$id}" method="post" name="Mitarbeiter">
Mitarbeiterliste von 
<select name="docinput[Mitarbeiter]">
{html_options options=$Mitarbeiter selected=$docinput.Mitarbeiter}</select>
<input type="submit" value="ansehen" />
</form>
</div>
{if isset($Mitarbeiterliste)}
<h2>Einkäufe von {$docinput.Mitarbeiter}</h2>
<table width="100%" border="1" style="border-collapse:collapse;" class="drucklinien">
<tr>
  <th>Bezeichung</th>
  <th>Preis</th>
  <th>Menge</th>
  <th>Benutzer</th>
</tr>
{assign var=Gesamt value=0}
{foreach from=$Mitarbeiterliste item=Buchung}
<tr>
  <td>{$Buchung.Bezeichnung}</td>
  <td align="right">{$Buchung.Preis|string_format:"%.2f"}</td>
  <td align="right">{$Buchung.Menge}</td>
  <td>{$Buchung.Benutzer}</td>
</tr> 
{math equation="z+x*y" x=$Buchung.Menge y=$Buchung.Preis z=$Gesamt assign=Gesamt}
{/foreach} 
<tr>
  <td>Gesamt</td>
  <td align="right">{$Gesamt|string_format:"%.2f"}</td>
  <td></td>
  <td></td>  
</tr>
</table>
<form action="?id={$id}" method="post" name="Mitarbeiter" class="noprint">
Einzahlung von {$Gesamt} Euro - Liste von <input type="text" 
name="docinput[Einzahlen]" value="{$docinput.Mitarbeiter}" readonly="readonly"/>  

<input type="submit" value="leeren" onClick="javascript:window.print()" />
</form>

{/if}
{/if}
<a href="?id={$id}&Barcodes=1" class="noprint">CVJM-Barcodeliste</a>
 