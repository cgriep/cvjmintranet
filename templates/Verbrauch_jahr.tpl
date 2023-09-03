
<!-- 
Template Verbrauch_jahr 
Statistik über alle Jahre und Verbrauchsstellen für Verbrauch oder Kosten 
-->

<h2>Verbrauchsstatistik</h2>
<table >
<tr class="ueberschrift"><td>Gr&ouml;&szlig;e</td>
  {foreach from=$Jahre item=Jahr}
    <td>{$Jahr}</td>
  {/foreach}
  {* <td>Gesamt</td> *}
</tr>

{foreach from=$Verbrauch key=Groesse item=Werte}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>{$Groesse}</td>
  {assign var="GesamtK" value="0"}
  {assign var="GesamtV" value="0"}
  {foreach from=$Jahre item=Jahr}
  <td>
  {if isset($Werte.$Jahr.Kosten) }
    {$Werte.$Jahr.Kosten|string_format:"%.2f"} &euro;
    {eval var=$GesamtK+$Werte.$Jahr.Kosten assign="GesamtK"}
  {/if}
  {if isset($Werte.$Jahr.Verbrauch) }<br />
    {$Werte.$Jahr.Verbrauch|string_format:"%.2f"}
    {eval var=$GesamtV+$Werte.$Jahr.Verbrauch assign="GesamtV"}
  {/if}
  </td>
  {/foreach} 
  {*
  <td>
    {$GesamtK|string_format:"%.2f"} &euro;<br />
    {$GesamtV|string_format:"%.2f"}
  </td>
  *}
</tr>
{/foreach}

<tr><td>Gesamt</td>
{foreach from=$Jahre item=Jahr}
  <td>
    {$JahresWert.$Jahr.Kosten|string_format:"%.2f"} &euro;<br />
    {$JahresWert.$Jahr.Verbrauch|string_format:"%.2f"}
  </td>
{/foreach}
</tr>



</table>
