<!--  Popup-Kalender -->
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>

<!-- 
Template Weideliste 
-->
<form id="Zeigen" method="post" action="?id={$id}"> 
Zeigen ab <input type="text" name="ShowDatum" id="ShowDatum" value="{$AbDatum}" 
  			onClick="popUpCalendar(this,Zeigen['ShowDatum'],'dd.mm.yyyy')"
      		onBlur="autoCorrectDate('Zeigen','ShowDatum',false);"
  			/>
<input type="submit" value="anzeigen" />
</form>
<h2>Übersicht: Weideorte</h2>
<form id="Weideliste" method="post" action="?id={$id}"> 
<table class="drucklinien" border="1" style="empty-cells:show">
<tr><th>Datum</th><th>Ort</th><th>Tierart</th><th>Erstellt</th><th>Bemerkung</th></tr>
  {foreach from=$Weideliste item=Zeile key=Tag}
    <tr {if $Zeile.Datum==""}style="background-color: red"{/if}>
    	<td>{$Tag|date_format:"%d.%m.%Y"}</td>
    	<td>{$Zeile.Ort|nl2br}</td>
    	<td>{$Zeile.Tierart|nl2br}</td>
    	<td>{$Zeile.Erstellt|date_format:"%d.%m.%Y"|nl2br}</td>
    	<td>{$Zeile.Bemerkung|nl2br}</td>
  	</tr>
  {/foreach}
  <tr class="noprint">
  		<td><input type="text" name="Weideliste[Datum]" id="Weideliste[Datum]" value="{$Datum}" 
  			onClick="popUpCalendar(this,Weideliste['Weideliste[Datum]'],'dd.mm.yyyy')"
      		onBlur="autoCorrectDate('Weideliste','Weideliste[Datum]',false);"
  			/></td>
    	<td><select name="Weideliste[Ort]">{html_options values=$Orte output=$Orte}</select></td>
    	<td><select name="Weideliste[Tierart]">{html_options values=$Tiere output=$Tiere}</select></td>
    	<td></td>
    	<td>
    		<input type="text" name="Weideliste[Bemerkung]"  /><input type="submit" value="speichern" />
    	</td>
  </tr>
</table>
</form>