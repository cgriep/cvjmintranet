<table class="drucklinien">
<tr>
  <td colspan="7"><h2>Vorhandene Zeiten in {$Monat}</h2></td>
</tr>
<tr class="ueberschrift">
  <th>Datum</th><th>Stunden</th><th>So-Stunden</th><th>Feiertagstd.</th>
</tr>
{assign var="Datum" value="0"}
{foreach from=$Eintraege item=row}
{if strtotime(date('Y-m-d',$Datum)) != strtotime(date('Y-m-d',$row.Datum))}
{if $Datum != 0}
<tr class="{cycle values="helleZeile,dunkleZeile"}" name="zeile">
  <td>
    {$Datum|date_format:"%d.%m.%Y"}
  </td>
  <td align="right">{$Normalstunden}</td>
  <td align="right">{$Sonntagstunden}</td>
  <td align="right">{$Feiertagstd}</td>
</tr>
{/if}
  {assign var="Normalstunden" value="0"}
  {assign var="Sonntagstunden" value="0"}
  {assign var="Feiertagstd" value="0"}
  {assign var="Datum" value=$row.Datum}
{/if}
{assign var="Normalstunden" value=$Normalstunden+$row.Normalstunden}
{assign var="Sonntagstunden" value=$Sonntagstunden+$row.Sonntagstunden}
{assign var="Feiertagstd" value=$Feiertagstd+$row.Feiertagstunden}
{/foreach}
{if $Datum!=0}
<tr class="{cycle values="helleZeile,dunkleZeile" name="zeile"}">
  <td>
    {$Datum|date_format:"%d.%m.%Y"}
  </td>
  <td align="right">{$Normalstunden}</td>
  <td align="right">{$Sonntagstunden}</td>
  <td align="right">{$Feiertagstd}</td>
</tr>
{/if}
<tr>
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