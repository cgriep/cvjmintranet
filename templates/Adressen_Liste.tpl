<h1>{$title}</h1>

{if ($document.Personal != 1 || isset ($smarty.request.Bearbeiten))}
<form action="?id={$id}" method="post">
<table width="100%" class="noprint">
<tr>
  <td>
	<label for="Kategorie">Kategorie</label>
	<select id="Kategorie" name="docinput[Kategorie]" size="1"
     {if (isset ($docinput.Edit) && $docinput.Edit != 4)} disabled="disabled"{/if}>
    {html_options options=$Kategorien selected=$docinput.Kategorie}
	</select> 
{if (!isset ($docinput.Edit))}
	<input type="Submit" value="ausw&auml;hlen" />
  </td>
  <td align="right">
  {if $Speichern}
	[ <a href="?id={$id}&Bearbeiten=-1">Adresse hinzuf&uuml;gen</a> ]<br />
	{if ($docinput.Kategorie != -1 && !isset ($docinput.Zugehoerig) && !isset ($docinput.Institution))}
	[ <a href="?id={$id}&Sort={$Sort}&docinput[Edit]=1">Adressen zuordnen</a> ]<br />
	{else}
	  {if (isset ($docinput.Zugehoerig))}
	     [ <a href="?id={$id}&docinput[Edit]=2&{$params}">Adressen zuordnen</a> ]<br />
	  {else}
	    {if (isset ($docinput.Institution))}
	    [ <a href="?id={$id}&docinput[Edit]=3&{$params}">Adressen zuordnen</a> ]<br />
	    {/if}
	  {/if}
	{/if}
	[ <a href="?id={$id}&KategorieNeu=-1">Kategorie hinzuf&uuml;gen</a> ]
	{if ($docinput.Kategorie != -1)}
			<br />[ <a href="?id={$id}&KategorieNeu={$docinput.Kategorie}">Kategorie bearbeiten</a> ]
	{/if}
  {/if} {* Speichern *}
{/if} {* Edit *}
{if (isset ($docinput.Edit) && $docinput.Edit == 4)}
	<input type="hidden" name="docinput[Edit]" value="5" />
	<input type="Submit" value="in Versandliste aufnehmen"/>
  </td>
  <td align="right">
    [ <a href="?id={$id}&docinput[Edit]=5">Versandliste leeren</a> ]<br />
	[ <a href="?id={$id}&docinput[Edit]=6">Institutionen entfernen</a> ]<br />
	[ <a href="?id={$id}&docinput[Edit]=7">Ansprechpartner entfernen</a> ]<br />
{/if}
  </td>
</tr>
</table>
</form>
{/if} {* nicht personalisiert *}

<h2>{$docinput.KategorieName}</h2>
{$stammdatenlink}

<table width="100%" cellpadding="5" cellspacing="0" border="0">
<tr>
{foreach from=$Listen key=nr item=Liste}
	<td bgcolor="{$Liste.Color}" width="1" background="img/edge_left.gif">&nbsp;</td>
	<td bgcolor="{$Liste.Color}" class="zentriert" nowrap="nowrap">
		<a class="{$Liste.Class}" href="?id={$id}&docinput[Page]={$nr}&Seite={$Seite}&{$params}">
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
<tr class="helleZeile">
  <td class="zentriert" valign="middle">
	{if is_numeric($docinput.Zugehoerig)}
	<input type="hidden" name="docinput[Zugehoerig]" value="{$docinput.Zugehoerig}"/>
	{/if}
	{if is_numeric($docinput.Institution)}
	<input type="hidden" name="docinput[Institution]" value="{$docinput.Institution}"/>
	{/if}
	{if is_numeric($docinput.Edit)}
	<input type="hidden" name="docinput[Edit]" value="{$docinput.Edit}" />
	{/if}
	{if isset($docinput.Sort)}
	<input type="hidden" name="Sort" value="{$docinput.Sort}" />
	{/if}
	<input type="Text" name="Search" value="{$Search}" size="40" />
	<input type="Submit" value="Suchen" />
	{if (isset ($docinput.Edit))}
	&nbsp;&nbsp;&nbsp;[ <a href="?id={$id}&{$paramsOhneEdit}">Bearbeitung beenden</a> ] 
	{/if}
	<label for="docinput[AnzBuchungen]">min. Buchungen</label>
	<input type="text" name="docinput[AnzBuchungen]" size="2" maxlen="3" value="{$docinput.AnzBuchungen}" /> nur aus den letzten 3 Jahren: 
	<input type="checkbox" name="docinput[proJahr]" value="v" {if $docinput.proJahr == 'v'}checked="checked"{/if}/> 
  </td>
  <td align="right">
  {if ($docinput.Edit == 4)}
	 {if ($Versandmarker)}
	 [ <a href="?id={$id}&{$params}&Versand=0">alle</a> ]<br />
	 {else}
	 [ <a href="?id={$id}&$params&Versand=1">mit Marker</a> ]<br />
 	 {/if}
  {/if}
  {if $Search != ''}
	[ <a href="?id={$id}&{$paramsOhneSearch}">Suche ausschalten</a> ]<br />
  {/if}
  {if (isset ($docinput.Zugehoerig) || isset ($docinput.Institution))}
	[&nbsp;<a href="?id={$id}&{$paramsOhneAlles}">Alle&nbsp;Adressen&nbsp;anzeigen</a>&nbsp;]<br />
  {/if}  
  [	<a href="cvjmExport.php?db=Adressen&{$params}" target="_blank">Export</a> ]
  </td>
</tr>
</form>

{if ($Gesamtanzahl > $AnzahlProSeite || $Seite > 1)}
<tr class="helleZeile">
  <td>
	Seite 
	{section name=seitenanzeige start=1 loop=$Gesamtanzahl/$AnzahlProSeite+2 step=1}
	 {if ($smarty.section.seitenanzeige.index != $Seite)}
	 <a href="?id={$id}&Seite={$smarty.section.seitenanzeige.index}&{$params}">
	  {$smarty.section.seitenanzeige.index}</a> 
     {else}<strong>{$smarty.section.seitenanzeige.index}</strong>{/if}
	{/section} 	
  </td>
  <td align="right">
	<a href="?id={$id}&Seite={$Seite}&docinput[AnzahlProSeite]={eval var=$AnzahlProSeite-5}&{$params}" 
	title="Anzeigeanzahl reduzieren">&uarr;</a><br />{$Gesamtanzahl} Eintr&auml;ge
  </td>
</tr>
{/if}
</table>
<table width="100%" cellspacing="0" cellpadding="1" border="0">
<tr class="ueberschrift">
{foreach from=$Felder item=Feld}
  <td>
    <a href="?id={$id}&Sort={$Feld}&{$params}" title="Sortieren nach {$Feld}">{$Feld}</a>
  </td>
{/foreach}
  <td>
  </td>
</tr>
{foreach from=$Adressen item=Adresse}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  {foreach from=$Felder key=nr item=Feld}
  <td>
	{if ($nr == 0)}<a href="?id={$id}&Bearbeiten={$Adresse->Adressen_id}">{/if}
	{$Adresse->getFeld($Feld)|default:"(leer)"|nl2br}
	{if ($nr == 0)}</a>{/if}
  </td>
  {/foreach}
  <td>
  {if (!isset ($docinput.Edit))}
	 {if ($Adresse->getAnsprechpartnerAnzahl() > 0)}
  	 <a href="?id={$id}&docinput[Institution]={$Adresse->Adressen_id}" 
  	   title="Adressen von Ansprechpartnern">&rarr;{$Adresse->getAnsprechpartnerAnzahl()}</a>
  	 {/if}
  	 {if ($Adresse->getInstitutionenAnzahl() > 0)}
 	 <a href="?id={$id}&docinput[Zugehoerig]={$Adresse->Adressen_id}" 
	   title="Adressen von Institutionen">&larr;{$Adresse->getInstitutionenAnzahl()}</a>
	 {/if}
  {else}	
	<input type="Checkbox" value="v"
	{if ($docinput.Edit== 1)}
	  {if $Adresse->hasKategorie($docinput.Kategorie)}checked="checked"{/if}
	  onclick="xajax_aendereZuordnung({$Adresse->Adressen_id},{$docinput.Edit},this.checked,{$docinput.Kategorie});"
	{/if}
	{if ($docinput.Edit== 2)}
	  {if $Adresse->isZugehoerig($docinput.Zugehoerig)}checked="checked"{/if}
	  onclick="xajax_aendereZuordnung({$Adresse->Adressen_id},{$docinput.Edit},this.checked,{$docinput.Zugehoerig});"
	{/if}
	{if ($docinput.Edit== 3)}
	  {if $Adresse->isAnsprechpartner($docinput.Institution)}checked="checked"{/if}
	  onclick="xajax_aendereZuordnung({$Adresse->Adressen_id},{$docinput.Edit},this.checked,{$docinput.Institution});"
	{/if}	
	{if ($docinput.Edit== 4)}
	  {if $Adresse->Versandmarker}checked="checked"{/if}
	  onclick="xajax_aendereZuordnung({$Adresse->Adressen_id},{$docinput.Edit},this.checked,{$Adresse->Versandmarker});"
	{/if}
	 title="zugeh&ouml;rig zu {$docinput.KategorieName}"	/>
  {/if} 
  </td>
</tr>
{/foreach}
</table>
<table width="100%" cellspacing="0" cellpadding="1" border="0">
{if ($Gesamtanzahl > $AnzahlProSeite || $Seite > 1)}
<tr class="helleZeile">
  <td>Seite 
  {section name=seitenanzeige start=1 loop=$Gesamtanzahl/$AnzahlProSeite+1 step=1}
	 {if ($smarty.section.seitenanzeige.index != $Seite)}
	 <a href="?id={$id}&Seite={$smarty.section.seitenanzeige.index}&{$params}">
	  {$smarty.section.seitenanzeige.index}</a> 
     {else}<strong>{$smarty.section.seitenanzeige.index}</strong>{/if} 
  {/section}
  </td>
  <td align="right">
	{$Gesamtanzahl} Eintr&auml;ge<br />
	<a href="?id={$id}&Seite={$Seite}&docinput[AnzahlProSeite]={eval var=$AnzahlProSeite+5}&{$params}" 
	title="Anzeigeanzahl erh&ouml;hen">&darr;</a>
  </td>
</tr>
{/if} {* mehr als eine Seite *}
</table>
<br />
{if ($docinput.Edit != 4)}
	[ <a href="?id={$id}&docinput[Edit]=4">Versandadressen zusammenstellen</a> ] {/if}
{if ($Speichern)}
	[ <a href="?id={$id}&Bearbeiten=-1">Adresse hinzuf&uuml;gen</a> ]
{/if}


