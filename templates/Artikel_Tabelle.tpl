<!--  Template Artikel Liste -->
{popup_init src="/javascript/overlib.js"}

<h1>{$title}</h1>

{foreach from=$Artikelarten key=nr item=value}
  {if ( $docinput.F_Art_id == $nr )} 
	<strong>
  {/if}
[&nbsp;<a href="?id={$id}&docinput[F_Art_id]={$nr}">{$value}</a>&nbsp;]
{if ( $docinput.F_Art_id == $nr )} 
	</strong>
{/if}
{/foreach}
&nbsp;
[&nbsp;<a href="?id={$id}&docinput[Artikel_Nr]=-1&docinput[F_Art_id]={$docinput.F_Art_id}">
Neuer Artikel</a>&nbsp;]
		
<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
{foreach from=$Listen key=nr item=Liste}
	<td bgcolor="{$Liste.Color}" width="1" background="img/edge_left.gif">&nbsp;</td>
	<td bgcolor="{$Liste.Color}" class="zentriert" nowrap="nowrap">
		<a class="{$Liste.Class}" href="?id={$id}&docinput[Page]={$nr}&{$params2}">
		  {$Liste.Listenname}
		</a>
	</td>
	<td bgcolor="{$Liste.Color}" width="1" background="img/edge_right.gif">&nbsp;</td>
{/foreach}
	<td width="100%">&nbsp; </td>
</tr>
</table>

<table width="100%" cellspacing="0" cellpadding="1" border="0">
<form action="?id={$id}" method="post">
<tr class="helleZeile noprint">
  <td class="zentriert" valign="middle">
	<input type="Text" name="Search" value="{$Search}" size="40" />
	<input type="Submit" value="Suchen" />	
  </td>
  <td align="right">       
  [	<a href="cvjmExport.php?db=Artikel&{$params2}" target="_blank">Export</a> ]
  </td>
</tr>
</form>

{if ($Gesamtanzahl > $AnzahlProSeite || $Seite > 1)}
<tr class="helleZeile noprint">
  <td>
	Seite 
	{section name=seitenanzeige start=1 loop=$Gesamtanzahl/$AnzahlProSeite+1 step=1}
	 {if ($smarty.section.seitenanzeige.index != $Seite)}
	 <a href="?id={$id}&Seite={$smarty.section.seitenanzeige.index}&{$params2}">
	  {$smarty.section.seitenanzeige.index}</a> 
     {else}<strong>{$smarty.section.seitenanzeige.index}</strong>{/if}
	{/section} 	
  </td>
  <td align="right">
	<a href="?id={$id}&Seite={$Seite}&docinput[AnzahlProSeite]={eval var=$AnzahlProSeite-5}&{$params2}" 
	title="Anzeigeanzahl reduzieren">&uarr;</a><br />{$Gesamtanzahl} Einträge
  </td>
</tr>
{/if}
</table>
<table width="100%" cellspacing="0" cellpadding="1" border="0" class="drucklinien">
<tr class="ueberschrift">
{foreach from=$Felder item=Feld}
  <td>
    <a href="?id={$id}&Sort={$Feld}&{$params2}" title="Sortieren nach {$Feld}">{$Feld}</a>
  </td>
{/foreach}
  <td>
  </td>
</tr>
{foreach from=$Artikelliste item=Artikel}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  {foreach from=$Felder key=nr item=Feld}
  <td>
	{if ($nr == 0)}<a href="?id={$id}&docinput[Artikel_Nr]={$Artikel->Artikel_id}"
	{popup text=$Artikel->Beschreibung|escape}>{/if}
	{$Artikel->getFeld($Feld)|default:"(leer)"|nl2br}
	{if ($nr == 0)}</a>{/if}
  </td>
  {/foreach}
  <td>
  </td>
</tr>
{/foreach}
</table>
<table width="100%" cellspacing="0" cellpadding="1" border="0">
{if ($Gesamtanzahl > $AnzahlProSeite || $Seite > 1)}
<tr class="helleZeile noprint">
  <td>Seite 
  {section name=seitenanzeige start=1 loop=$Gesamtanzahl/$AnzahlProSeite+1 step=1}
	 {if ($smarty.section.seitenanzeige.index != $Seite)}
	 <a href="?id={$id}&Seite={$smarty.section.seitenanzeige.index}&{$params2}">
	  {$smarty.section.seitenanzeige.index}</a> 
     {else}<strong>{$smarty.section.seitenanzeige.index}</strong>{/if} 
  {/section}
  </td>
  <td align="right">
	{$Gesamtanzahl} Einträge<br />
	<a href="?id={$id}&Seite={$Seite}&docinput[AnzahlProSeite]={eval var=$AnzahlProSeite+5}&{$params2}" 
	title="Anzeigeanzahl erhöhen">&darr;</a>
  </td>
</tr>
{/if} {* mehr als eine Seite *}
</table>
