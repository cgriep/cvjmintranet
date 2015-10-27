<p>
  <a href="{$Adressenurl}&Bearbeiten={$Buchung->Adresse->Adressen_id}"
    {popup text=$Buchung->Adresse->Uebersicht(true) caption="Kontaktdaten bearbeiten"}>
    {$Buchung->Adresse->Vollname()}
  </a> ({$Buchung->Adresse->Ort})<br />
  {if ( trim($Buchung->Adresse->Zusatz) != '' )}
  {$Buchung->Adresse->Zusatz|nl2br}<br />
  {/if}
  <a href="{$Buchungurl}&Buchung_Nr={$Buchung->Buchung_Nr}">
    {$Buchung->Von|date_format:"%A %d.%m.%Y"}
    {if ( $Buchung->Ankunftszeit != 0)}
       {$Buchung->Ankunftszeit|date_format:"%H:%M"}
     {/if} - {$Buchung->Bis|date_format:"%A %d.%m.%Y"}
    {if ( $Buchung->Abfahrtszeit != 0)}
      {$Buchung->Abfahrtszeit|date_format:"%H:%M"}
    {/if}
    ({$Buchung->berechneUebernachtungen()} Übernachtungen) mit 
    {$Buchung->personenAnzahl()} Personen ({$Buchung->getAltersgruppe()})
    {eval var=$Buchung->BetreueranzahlM+$Buchung->BetreueranzahlW assign="anz"}
    {if ( $anz != 0 )}davon {$anz} Betreuer{/if}
  </a>
  {if ( trim($Buchung->Internes) != '' )}
    <br />{$Buchung->Internes|nl2br}
  {/if}
  {if ( trim($Buchung->Vereinbarungen) != '' )}
    <br />{$Buchung->Vereinbarungen|nl2br}
  {/if}
</p>
