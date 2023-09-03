<h1>{$title}</h1>

<p>1. Zahl: Anzahl Übernachtungen (Anzahl Buchungen/ davon Anzahl Selbstversorgerübernachtungen</p>

<table class="volleTabelle">
{foreach from=$Werte key=Bereich item=Datum}
	<tr class="ueberschrift">
	  <th>{$Bereich}</th>
 	  {foreach from=$dieJahre item=Jahr}
 		<th>{$Jahr}</th>
 	  {/foreach}
	</tr>
	{foreach from=$Datum key=dat item=Jahre}
 	    <tr class="{cycle values="helleZeile,dunkleZeile"}">
 	      <td>{$dat|date_format:"%d.%m.%Y"}</td>
 	    {foreach from=$dieJahre item=Jahr}
 	    	<td>
   	      {if isset($Jahre.$Jahr)}
 	        {$Jahre.$Jahr.Menge} ({$Jahre.$Jahr.Buchungen}/{$Jahre.$Jahr.Selbst})
 	      {else}
 	      	{if $dat==$Vergleichsdatum}
 	      		<i>({$Vergleich.$Bereich.$Jahr})</i>
 	      	{else} 	      
 	        	&nbsp;
 	        {/if}
 	      {/if}
 	      </td>
 	    {/foreach}    
 	    </tr>
	{/foreach}
	<tr class="helleZeile">
 	    <td><b>Gesamt</b></td>
 	    {foreach from=$dieJahre item=Jahr}
 	      <td>{$JahrGesamt.$Bereich.$Jahr|default:"&nbsp;"}</td>
 	    {/foreach}
	</tr>
{/foreach}
<tr class="dunkleZeile">
	<td><b>Jahresgesamt</b></td>
	{foreach from=$dieJahre item=Jahr}
		{assign var=s value=0}
	    {foreach from=$JahrGesamt item=b}
	    	{assign var=s value=$b.$Jahr+$s}
	    {/foreach} 
 		<td>{$s}</td>      	  
 	{/foreach}
</tr>
</table>
