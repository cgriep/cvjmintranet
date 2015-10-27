
<!-- 
Template Verbrauch_jahr 
Statistik über alle Jahre und Verbrauchsstellen für Verbrauch oder Kosten 
-->

<h2>Statistik f&uuml;r {$Ueberschrift}</h2>
<table>
<!--  Spaltenköpfe fehlen -->

<tr>
  <td></td>
  {section name=monate loop=13 start=1 }
  <td>{$smarty.section.monate.index}</td>
  {/section}
  <td>Gesamt</td>
</tr>

{foreach from=$MonatVerbrauch key=Jahr item=Werte}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>{$Jahr} </td>
  {assign var="Kosten" value="0"}
  {assign var="Verbrauch" value="0"}
  {foreach from=$Werte key=Monat item=Wert}
    <td>{$Wert.Kosten|string_format:"%.2f"} &euro;<br />{$Wert.Verbrauch|string_format:"%.2f"}</td>
    {eval var=$Wert.Kosten+$Kosten assign="Kosten"}
    {eval var=$Wert.Verbrauch+$Verbrauch assign="Verbrauch"}
    <!-- Kosten summieren für Jahr -->
  {/foreach}
  <td>{$Kosten|string_format:"%.2f"} &euro;<br />{$Verbrauch|string_format:"%.2f"}</td>
</tr>
{/foreach}

</table>
