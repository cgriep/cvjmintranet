<div class="contentRight">
    {if $KastenGross}
    <div class="Kasten KastenGross" id="{$KastenGross}">
      {if $Kasten1Bild}
        <img src="{$Kasten1Bild}" alt="Bild" title="Charakterbild" />
      {else}
        (nichts ausgew�hlt)
        <noscript><div class="Fehler">Sie m�ssen JavaScript aktivieren, damit hier etwas erscheint.</div></noscript>
      {/if}
    </div>         
    {/if}
    {if $Kasten1}
      <div class="Kasten Kasten1" id="{$Kasten1}">
      {if $Kasten1Bild}
        <img src="{$Kasten1Bild}" alt="Bild" title="Charakterbild" />
      {else}
        (nichts ausgew�hlt)
        <noscript><div class="Fehler">Sie m�ssen JavaScript aktivieren, damit hier etwas erscheint.</div></noscript>
      {/if}
      </div>     
    {/if}    
    {if $Kasten2 }
    <span class="abschluss"></span>
    <div class="Kasten Kasten2" id="{$Kasten2}">
      (nichts ausgew�hlt)
      <noscript><div class="Fehler">Sie m�ssen JavaScript aktivieren, damit hier etwas erscheint.</div></noscript>
    </div>
    {/if}
 </div>