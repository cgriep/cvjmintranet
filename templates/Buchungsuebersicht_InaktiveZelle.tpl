  {if ! $mitZeit}
    <input type="Checkbox" name="Artikel[{$artikel.id}][{$tagnr}]" value="v" 
           {if !$buchung.Fertig}
           onClick="javascript:xajax_fuegeArtikelHinzu({$buchung.Buchung_Nr},{$artikel.id},{$tagnr},'{$docinput.Artikelnummern}');"
           {else}
           disabled="disabled"
           {/if}
           title="Klicken um {$menge} Artikel am {$tagnr|date_format:"%d.%m.%Y"} zu buchen" /> 
  {else}
    <input type="text" name="Artikel[{$artikel.id}][{$tagnr}]" size="2" maxlength="5" value="" 
    {if !$Programmpaket}
    title="Zeit in der Form HH:MM oder HHMM eingeben" 
    {else}
    title="Menge eingeben"
    {/if} />
  {/if}
