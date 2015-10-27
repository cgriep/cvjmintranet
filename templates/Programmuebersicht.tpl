
<!-- 
Template Programmuebersicht 
-->

<h1>{$title}</h1>
{popup_init src="/javascript/overlib.js"}

<style type="text/css">
	@import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>
<form action="?id={$id}" method="post" name="Wahl">
	<label for="Datum">Programme ab Datum</label>
	<input type="Text" name="Datum" value="{$Tag|date_format:"%d.%m.%Y"}" size="10" maxlength="10"
	 onClick="popUpCalendar(this,Wahl['Datum'],'dd.mm.yyyy')"
	 onBlur="autoCorrectDate('Wahl','Datum' , false )" />
	 <input type="Submit" value="anzeigen"/>
</form>

<table class="volleTabelle cellpadding="1px" cellspacing="0px">
<tr class="ueberschrift">
  <th>Modul</th><th>Datum</th><th>Personen</th><th>Zuständig</th><th>Buchung</th>
</tr>
{if isset($smarty.request.Buchung_Nr) && is_numeric($smarty.request.Buchung_Nr)}
	<h2>Angezeigt wird nur Buchung {$smarty.request.Buchung_Nr}</h2>
{/if}
{foreach from=$zeilen item=row}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
	<td valign="top">
		<a href="{$artikelurl}&docinput[Artikel_Nr]={$row.F_Artikel_id}#Edit"
		{popup text=$row.Artikel->Beschreibung|nl2br|escape caption=$row.Artikel->Bezeichnung|escape} >
		{$row.Artikel->Bezeichnung}
		</a>
		</td>
		<td>
		{$row.Datum|date_format:"%d.%m.%Y %H:%M"}
		{if ($row.Dauer > 0)}({$row.Dauer} h){/if}
		</td>
		<td>{$row.Buchung->Personenanzahl()}/ {$row.Buchung->BetreueranzahlW+$Buchung->BetreueranzahlM} Bet.</td>
		<td>{$row.zustaendig}</td>
		<td>
		  <a href="{$Buchungurl}&Buchung_Nr={$row.Buchung->Buchung_Nr}">{$row.Buchung->Buchung_Nr}</a></td>

</tr>
{/foreach}
</table>

<div class="zentriert">
  [ <a href="{$Buchungurl}">Buchungsliste</a> ]
</div>