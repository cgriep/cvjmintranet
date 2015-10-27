<h1>{$Rasse}</h1>

{$Beschreibung|nl2br}

{if Count($Besonderheiten)>0}
<h2>Rassenbesonderheiten</h2>
<table class="Liste">
<tr>
  <th>Fertigkeit</th>
  <th>Art</th>
  <th>Kosten</th>
  <th></th>
</tr>
{foreach name=f key=nr item=fertigkeit from=$Besonderheiten}
<tr>
  <td><span {popup textsize="1" text=$fertigkeit.Beschreibung|escape}>{$fertigkeit.Fertigkeit}</span></td>
  <td>{$fertigkeit.Art}</td>
  <td>{eval var=$fertigkeit.Kosten-$fertigkeit.Kosten1}</td>
  <td>{if $fertigkeit.Pflicht}Pflicht{else}{if $fertigkeit.Kosten==0}Verboten{/if}{/if}</td></tr>
{/foreach}
</table>
{/if}