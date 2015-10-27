<div class="contentRight">
    {if $KastenGross}
    <div class="Kasten KastenGross" id="{$KastenGross}">
      {if $Kasten1Bild}
        <img src="{$Kasten1Bild}" alt="Bild" title="Charakterbild" />
      {else}
        (nichts ausgewählt)
        <noscript><div class="Fehler">Sie müssen JavaScript aktivieren, damit hier etwas erscheint.</div></noscript>
      {/if}
    </div>         
    {/if}
    {if $Kasten1}
      <div class="Kasten Kasten1" id="{$Kasten1}">
      {if $Kasten1Bild}
        <img src="{$Kasten1Bild}" alt="Bild" title="Charakterbild" />
      {else}
        (nichts ausgewählt)
        <noscript><div class="Fehler">Sie müssen JavaScript aktivieren, damit hier etwas erscheint.</div></noscript>
      {/if}
      </div>     
    {/if}    
    {if $Kasten2 }
    <span class="abschluss"></span>
    <div class="Kasten Kasten2" id="{$Kasten2}">
      (nichts ausgewählt)
      <noscript><div class="Fehler">Sie müssen JavaScript aktivieren, damit hier etwas erscheint.</div></noscript>
    </div>
    {/if}
 </div>