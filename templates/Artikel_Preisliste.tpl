<h1>{$title}</h1>

<form action="?id={$id}" method="post" class="noprint">
<select name="docinput[Preisliste]">
{html_options options=$Preislisten selected=$Preisliste_id}
</select> 
<input type="Submit" value="Auswählen" />
{if Count($Artikel)>0}
<form action="?id={$id}" method="post">
<table class="drucklinien">
<tr class="ueberschrift">
  <th>Bezeichnung</th><th>Einheit</th><th>Preis netto</th><th>Preis brutto</th><th>Preis/h netto</th><th>Preis/h brutto</th>
</tr>
{assign var="Art" value=''}
{foreach from=$Artikel item=artikel}
{if ( $Art != $artikel->getArtikelart() )}
<tr class="ueberschrift">
  <td colspan="6">
  <strong>{$artikel->getArtikelart()}</strong>
  <input type="checkbox" name="docinput[Art][]" value="{$artikel->F_Art_id}" checked="checked" class="noprint"/>
  </td>
</tr>
{assign var="Art" value=$artikel->getArtikelart()}
{/if}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
      <td>{$artikel->Bezeichnung|escape}</td>
      <td>{$artikel->Einheit}</td>
      <td align="right">{$artikel->holePreis($Preisliste_id)|string_format:"%0.2f"}</td>
	  <td align="right">
		{math equation="x*(100+y)/100" x=$artikel->holePreis($Preisliste_id) y=$artikel->getMWST() assign=preis}
		{$preis|string_format:"%0.2f"}
		</td>
	  <td align="right">{$artikel->holePreis($Preisliste_id,true)|string_format:"%0.2f"}</td>
	  <td align="right">
	    {math equation="x*(100+y)/100" x=$artikel->holePreis($Preisliste_id,true) y=$artikel->getMWST() assign=preis} 
	    {$preis|string_format:"%0.2f"}
		</td>
</tr>
{/foreach}
</table>
<input type="submit" value="Auswählen" class="noprint"/>
<div class="noprint">
<strong>Vorlagen: </strong>
{foreach from=$Vorlagen item=filename}
[ <a href="cvjmVorlagen.php?Vorlage={$filename}{$smarty.const.CVJM_ENDUNG}&id={$Preisliste_id}&db=ArtikelPreislisten&User={$username}&Arten={$Arten}" 
  target="_blank">{$filename}</a> ] 
{/foreach}
{if ( Count($Vorlagen)== 0 )}-keine vorhanden-{/if}
</div>
{/if}
</form>