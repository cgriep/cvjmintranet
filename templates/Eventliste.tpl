{* Zeigt alle Events untereinander an *}
{foreach from=$events item=event}
<div>
    {$event->Datum|date_format:"HH:MM"}
    <a href="?id={$id}&EditEvent={$event->Event_id}" 
    {popup caption=$event->getUebersicht()|escape}>
      {$event->Titel}
    </a>
</div>
{/foreach}
      