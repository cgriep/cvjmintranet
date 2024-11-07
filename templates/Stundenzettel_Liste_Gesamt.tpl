<table class="drucklinien">
<tr>
  <td colspan="7"><h2>Vorhandene Zeiten in {$Monat}</h2></td>
</tr>
<tr class="ueberschrift">
  <th>Datum</th><th>Von</th><th>Bis</th><th>Bemerkungen</th><th>Stunden</th><th>So-Stunden</th><th>Feiertagstd.</th><th>Art</th>
</tr>
{foreach from=$Eintraege item=row}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    {if ( ! $row.Gebucht )}<a href="?id={$id}&docinput[Zeit]={$row.id}&docinput[Benutzer]={$Benutzer}">{/if}
    {$row.Datum|date_format:"%d.%m.%Y"}
    {if ( ! $row.Gebucht )}</a>{/if}
    {if ( ! $row.Gebucht )}<a href="?id={$id}&docinput[Del]={$row.id}&docinput[Benutzer]={$Benutzer}" 
    onClick="return window.confirm('Eintrag wirklich löschen?');">
      <img src="/img/small_edit/delete.gif" border="0" title="Löschen" /></a>{/if}
    {$row.Datum|date_format:"%d.%m.%Y"}
    {if ( ! $row.Gebucht )}</a>{/if}
  </td>
  <td align="center">
      {$row.Datum|date_format:"%H:%M"}
  </td>
  <td align="center">
      {$row.Bis|date_format:"%H:%M"}
  </td>
  <td>
    {$row.Bemerkung|nl2br}
  </td>
  <td align="right">{$row.Normalstunden}</td>
  <td align="right">{$row.Sonntagstunden}</td>
  <td align="right">{$row.Feiertagstunden}</td>
  <td>{$row.Art}</td>
</tr>
{/foreach}
<tr>
  <td>Einträge</td>
  <td colspan="2">{$Eintraege|@count}</td>
  <td>Gesamt (inkl. Zulagen)</td>
  <td align="right">{$Stunden}</td>
  <td align="right" title="Sonntagsstunden, 1,5 wertig">{$SoStunden}</td>
  <td align="right" title="Feiertagsstunden, 2wertig">{$FeierStunden}</td>
</tr>
<tr>
  <td colspan="3"></td><td>Summe</td><td colspan="3" align="right"><strong>
  {$Gesamtstunden}
  </strong></td>
</tr>
</table>    