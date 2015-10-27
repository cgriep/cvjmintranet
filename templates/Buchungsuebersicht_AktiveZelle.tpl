  <div class="Rahmenzelle">
  {if ! $mitZeit}
    <input type="Checkbox" name="Artikel[{$artikel.id}][{$eintrag.Datum}]" value="v" checked="checked"
    {if !$buchung.Fertig} 
      onClick="javascript:xajax_entferneArtikel({$buchung.Buchung_Nr},{$artikel.id},{$eintrag.Datum},{$eintrag.Menge},'{$docinput.Artikelnummern}');"
    {else}
      disabled="disabled"
    {/if}
      title="Klicken um den Artikel am {$eintrag.Datum|date_format:"%d.%m.%Y"}zu entfernen" /> 
  {else}
    <input type="text" name="Artikel[{$artikel.id}][{$eintrag.Datum}]" size="2" maxlength="5"
      {if $Programmpaket} 
      title="Menge eingeben"    
      {else}
      title="Zeit als HHMM oder HH:MM eingeben"
      {/if}
      value="{if $eintrag.Menge>= 0}{if !$Programmpaket}{$eintrag.Datum|date_format:"%H:%M"}{else}{$eintrag.Menge}{/if}{/if}" />
  {/if} 
  {if $eintrag.Menge > 1} {$eintrag.Menge}{/if} 
</div>