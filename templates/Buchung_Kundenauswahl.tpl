<input type="hidden" name="docinput[KuNr]" value="" />
Kundenauswahl: <input type="Text" name="Kundenwahl" value="" id="Kundenauswahl" title="Kundennummer, Name oder Vorname (oder Teile davon) eingeben"/>
  <a href="#" onClick="xajax_sucheKunden(document.getElementById('Kundenauswahl').value); return false;">Suchen</a>
{if isset($Kunden)}
<table width="100%">
{foreach from=$Kunden item=Kunde}
<tr>
  <td><input type="radio" name="docinput[KuNr]" value="{$Kunde->Kunden_Nr}" /></td>
  <td>
  {$Kunde->Vollname()}</a>
  </td>
  <td>{$Kunde->Strasse}, {$Kunde->PLZ} {$Kunde->Ort}</td>
  <td>{$Kunde->Kunden_Nr}</td>
</tr>
{/foreach}
{if Count($Kunden)== 0}
<tr><td>Keine passenden Kunden gefunden</td></tr>
{/if}
</table>
{/if}
