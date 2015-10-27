<!--  Template Artikel Barcodeliste -->

<h1>{$title}</h1>

<table>
{foreach from=$Artikelliste item=Artikel}
<tr>
  <td><font size="+5">{$Artikel->Bezeichnung}</font>
  {if $Artikel->Beschreibung != ''}<br /><span class="mini">{$Artikel->Beschreibung}</span>{/if}
  </td>
  <td><img src="/barcode.php?Anzeige={$Artikel->Barcode}" height="100px" /></td>
  <td style="padding-left: 10pt"> {$Artikel->getArtikelBildIMG()}</td>
</tr>
<tr height="30px">
<td></td>
</tr>
{/foreach}
</table>