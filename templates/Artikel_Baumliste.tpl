<tr class="ueberschrift">
  <th>
	{if ( $smarty.session.ArtikelAnzahl != 'Alle' && (($smarty.session.ArtikelAnzahl - 10) >= 5) )}
		[ <a onClick="xajax_aendereArtikelAnzahl({$id},{$Artikel->Artikel_id},{eval var=$smarty.session.ArtikelAnzahl-10});return false;" href="#">10 weniger</a> ]
	{/if}
  </th>
  <th class="zentriert">
		Position im Artikelbaum
  </th>
  <th>
  {if ( $smarty.session.ArtikelAnzahl != 'Alle' )}
	[ <a onClick="xajax_aendereArtikelAnzahl({$id},{$Artikel->Artikel_id},{eval var=$smarty.session.ArtikelAnzahl+10}); return false;" href="#">10 mehr</a> ]
	[ <a onClick="xajax_aendereArtikelAnzahl({$id},{$Artikel->Artikel_id},'Alle');return false;" href="#">alle</a> ]
  {else}
	[ <a onClick="xajax_aendereArtikelAnzahl({$id},{$Artikel->Artikel_id},5);return false;" href="#">Ausschnitt</a> ]
  {/if}
  </th>
</tr>
{foreach from=$Artikel->baueArtikelBaumAuf($smarty.session.ArtikelAnzahl) item=baumartikel}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>
    <a href="#" onClick="xajax_moveArtikel({$id}, {$baumartikel->Artikel_id}, 'l');return false;">
    &larr;</a>&nbsp;{$baumartikel->Position}
  </td>
  <td>
	<span style="width:{math equation="10*x" x=$baumartikel->Einruecken}px;float:left;">&nbsp;</span>
	{if ( $baumartikel->Artikel_id == $Artikel->Artikel_id )}<strong>{/if}
	<a href="?id={$id}&docinput[Artikel_Nr]={$baumartikel->Artikel_id}"
	{popup text=$baumartikel->Beschreibung|escape}>
	{$baumartikel->Bezeichnung}</a>
	{if ( $baumartikel->Artikel_id == $Artikel->Artikel_id )}</strong>{/if}
	{if ( ! $baumartikel->Anzeigen )}<em>(nicht angezeigt)</em>{/if}
  </td>
  <td>
    <a href="#" onClick="xajax_moveArtikel({$id}, {$baumartikel->Artikel_id}, 'r');return false;">
    &rarr;</a>&nbsp;
	<a href="#" onClick="xajax_moveArtikel({$id}, {$baumartikel->Artikel_id}, 'u');return false;">
	&uarr;</a>&nbsp;
	<a href="#" onClick="xajax_moveArtikel({$id}, {$baumartikel->Artikel_id}, 'd');return false;">
	&darr;</a>&nbsp;&nbsp;&nbsp;&nbsp;
	<a href="?id={$id}&docinput[Artikel_Nr]=-1&docinput[F_Art_id]={$Artikel->F_Art_id}&docinput[Position]={$baumartikel->Position}&docinput[Einruecken]={$baumartikel->Einruecken}">
		neuer Artikel hier</a>
  </td>
</tr>
{/foreach}