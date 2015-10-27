{* Template Buchung_Bemerkung_Edit *} 
{popup_init src="/javascript/overlib.js"}
<form action="?id={$id}" method="post">
<input type="hidden" name="docinput[Buchung_Nr]" value="{$Buchung->Buchung_Nr}">
<table class="volleTabelle">
<tr>
  <td colspan="2">
    {include file="Buchung_Kopf.tpl"}	
  </td>
</tr>
<tr>
  <td colspan="2" class="zentriert">
    [ <a href="?id={$id}">Buchungsliste</a> ]
  </td>
</tr>
<tr class="dunkleZeile">
  <td colspan="2" align="center">
    <input type="Submit" name="Save" value="Speichern" />
  </td>
</tr>
<tr class="dunkleZeile">
  <th colspan="2">
    Neue Bemerkung/Ausleihe/Zerbruch<br />
    &nbsp; &rarr; &nbsp;
    	<label for="Reparatur">Reparaturauftrag</label> 
    	<input type="Checkbox" name="docinput[Reparatur]" id="Reparatur" value="v" />
    	<!-- 
    	&nbsp;&nbsp;
      	  <label for="Kueche">Küchenauftrag</label>
      	  <input type="Checkbox" name="docinput[Kueche]" id="Kueche" value="v" />
      	   -->
      	&nbsp;&nbsp;
      	  <label for="Verwaltung">Verwaltungsauftrag</label>
      	  <input type="Checkbox" name="docinput[Verwaltung]" id="Verwaltung" value="v" />
    	<!-- 
      	&nbsp;&nbsp;
      	  <label for="Paedagogik">Pädagogikauftrag</label>
      	  <input type="Checkbox" name="docinput[Paedagogik]" id="Paedagogik" value="v" />
    	 -->
    <br/>
    <textarea name="docinput[NewBem]" cols="60" rows="3"></textarea>
  </th>
</tr>
{if Count($Bemerkungen) > 0}
<tr class="ueberschrift">
  <th colspan="2">Vorhandene Bemerkungen</th>
</tr>
{/if}
{foreach from=$Bemerkungen item=bemerkung}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    {if ! ($bemerkung.F_Auftrag_id=='')}
    Zu dieser Bemerkung gehört ein <a href="{$Auftragurl}&EditAuftrag={$bemerkung.F_Auftrag_id}">Auftrag {$bemerkung.F_Auftrag_id}</a><br />
    {/if}
    {$bemerkung.Bemerkung|nl2br}<br />
    <textarea name="docinput[H{$bemerkung.Bemerkung_id}]" cols="40" rows="2">{$bemerkung.Hinweis}</textarea>
  </td>
  <td valign="top">
    <font size="-2">{$bemerkung.Logtext|nl2br}</font>
  </td>
</tr>
{/foreach}
<tr>
  <td colspan="2" class="zentriert">
    <input type="Submit" name="Save" value="Speichern">
  </td>
</tr>
</table>
</form>
<div class="zentriert">[ <a href="?id={$id}">Buchungsliste</a> ]</div>
