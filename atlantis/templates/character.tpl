<h1>{$Character.Name}</h1>
{$Character.Art} {$Character.Rasse}
{if isset($Character.Ausrichtung) }
 (Ausrichtung: {$Character.Ausrichtung})
{/if}
<hr />

{if Count($Character.Spruchlisten)>0 }
Spruchlisten:
{foreach from=$Character.Spruchlisten item=fertigkeit}
  {$fertigkeit}, 
{/foreach} 	 	
<br />
{/if}

{if Count($Character.Vorteile) > 0 }
<table class="Liste">
<caption>Vor- und Nachteile:</caption>
<tr><th>Name</th><th>Stufe</th><tr>
  {foreach from=$Character.Vorteile item=fertigkeit}
    <tr {popup text=$fertigkeit.Beschreibung|escape textsize="1"}><td>{$fertigkeit.Fertigkeit}</td><td>{$fertigkeit.Stufe}</td></tr>
  {/foreach} 	 	
</table>
{/if}

{if Count($Character.UeVorteile) > 0 }
<table class="Liste">
<caption>Übernatürliche Vor- und Nachteile:</caption>
<tr><th>Name</th><th>Aktiv</th><tr>
  {foreach from=$Character.UeVorteile item=fertigkeit}
    <tr {popup text=$fertigkeit.Beschreibung|escape textsize="1"}><td>{$fertigkeit.Fertigkeit}</td><td>{if $fertigkeit.Stufe>0}Ja{else}Nein{/if}</td></tr>
  {/foreach} 	 	
</table>
{/if}

{if Count($Character.Fertigkeiten) > 0 }
<table class="Liste">
<caption>Fertigkeiten:</caption>
<tr><th>Name</th><th>Bereich</th><th>Rang</th><tr>
  {foreach from=$Character.Fertigkeiten item=fertigkeit}
    <tr {popup text=$fertigkeit.Beschreibung|escape textsize="1"}><td>{$fertigkeit.Fertigkeit}</td><td>{$fertigkeit.Spezialisierung}</td><td>{$fertigkeit.Rang}</td></tr>
  {/foreach} 	 	
</table>
{/if}

{if Count($Character.Verboten) > 0 }
  (Verbotene Faehigkeiten:
  {foreach from=$Character.Verboten item=fertigkeit}
    {$fertigkeit}, 
  {/foreach})
{/if}
