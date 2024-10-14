<h2>{$title}</h2>

<a href="{$Adressenurl}&Bearbeiten={$Adresse->Adresse.Adressen_id}">
{$Adresse->Adresse.Vorname} {$Adresse->Adresse.Name}
</a>
{if $Adresse->Adresse.Zusatz != ''} ({$Adresse->Adresse.Zusatz}){/if}
<br />
<div align="center">
  [ <a href="?id={$ret}&Bearbeiten={$Adresse->Adresse.Adressen_id}">
  Zurück zu den Adressdaten</a> ]
</div>
<table width="100%">
{assign var="i" value="0"}
{foreach from=$Kategorien item=Kategorie}
  {if $i%2==0}
  <tr class="{cycle values="helleZeile,dunkleZeile"}">
  {eval var=$i assign="Tab"}
  {else}
  {eval var="$i+ceil(Count($Kategorien)/2)" assign="Tab"}
  {/if}
  <td>
    <input type="Checkbox" name="docinput[Kategorien][]" 
      tabindex="{$Tab}" value="{$Kategorie.Kategorie_id}" {if $Kategorie.Da}checked="checked"{/if} 
      onClick="xajax_setzeAdressKategorie({$Adresse->Adresse.Adressen_id},{$Kategorie.Kategorie_id},this.checked)"      
      />
  </td>
  <td>
    {$Kategorie.Kategorie} (<span id="K{$Kategorie.Kategorie_id}">{$Kategorie.Anz}</span>)
  </td>
  {if $i%2==1}
  </tr>
  {/if}
  {eval var=$i+1 assign="i"}
{/foreach}
{if $i%2==1}</tr>{/if}
</table>
<div align="center">
  [ <a href="?id={$ret}&Bearbeiten={$Adresse->Adresse.Adressen_id}">
  Zurück zu den Adressdaten</a> ]
</div>