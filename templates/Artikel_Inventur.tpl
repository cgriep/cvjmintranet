<!--  Template Artikel Stammdaten -->
{popup_init src="/javascript/overlib.js"}

<h1>{$title}</h1>

<form action="?id={$id}" method="post">
<input type="text" name="docinput[Ort]" />
<input type="submit" value="Anzeigen" />
</form>

{if isset($Ort)}
<table class="volleTabelle">
<tr class="ueberschrift">
	<td colspan="3">Artikel am Ort {$Ort->Bezeichnung}</td>
</tr>
<tr>
  <th>Barcode</th>
  <th>Artikel</th>
  <th>Menge</th>
</tr>
{foreach from=$Artikelliste item=Artikel}
<tr>
  <td>{$Artikel->Barcode}</td>
  <td>{$Artikel->Bezeichnung}</td>
  <td>{$Artikel->Menge}</td>
</tr>
{/foreach}
<tr>
  <td>
    <input type="text" name="docinput[Barcode]" onKeyUp="xajax_zeigeArtikelNachBarcode(this.value);" />
  </td>
  <td id="Artikel">&nbsp;
  </td>  
  <td>
    <input type="text" name="docinput[Menge]" value="1" />
    <button>Speichern</button>
  </td>
</tr>
</table>
{/if}