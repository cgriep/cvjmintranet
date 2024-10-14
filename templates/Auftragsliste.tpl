{$title}

<div class="noprint">
 [ <a href="?id={$id}&newauf=1">Auftrag hinzufügen</a> ] 
 [ <a href="?id={$id}&Alle={$Alle}{$BernutzerURL}">{$AlleText}erledigte anzeigen</a> ]
 {if ( isset($smarty.request.SearchOrt))}
    [ <a href="?id={$id}">Alle anzeigen</a> ]
 {/if}
 {if ( isset($smarty.request.MyWork) )}
    [ <a href="?id={$id}">Nur Aufträge für mich anzeigen</a> ] 
 {else}
    [ <a href="?id={$id}&MyWork=1{$BenutzerURL}">Auftragsstatus erteilter Aufträge anzeigen</a> ] 
  {/if}
</div>

<h2>Aufträge von {$Benutzer}, Stand {$smarty.now|date_format:"%d.%m.%Y %H:%M"}</h2>

<table width="100%" border="1" style="border-collapse:collapse;" class="drucklinien">
<tr style="border: thin solid; border-width: 1pt;padding: 2pt">
  <th>
    <a title="nach Bezeichnung sortieren" href="?id={$id}&Sort=Titel">Bezeichnung</a>
  </th>
  <th class="small">
    <a href="?id={$id}&Sort=Prioritaet">Priorität</a>/<br/>
    <a href="?id={$id}&Sort=Status">Status</a>
  </th>
  <th class="small">Neuer Status</th>
</tr>

{foreach from=$Auftraege item=Auftrag}
<tr>
  <td valign="top">
    <a title="Auftrag bearbeiten" href="?id={$id}&EditAuftrag={$Auftrag->Event_id}">
      {$Auftrag->Titel}
    </a>
      {if ( $Auftrag->getOrt() != '' )}
		<em>( <a title="Alles zu {$Auftrag->getOrt()} anzeigen" 
		href="?id={$id}&SearchOrt={$Auftrag->Ort}">{$Auftrag->getOrt()}</a> )</em>
	  {/if}
	&nbsp; von {$Auftrag->getAutor()} erstellt {$Auftrag->created|date_format:"%d.%m.%Y %H:%M"}
<br />{$Auftrag->Beschreibung|nl2br}
{if ($Auftrag->Bemerkung != '')}
<br />{$Auftrag->Bemerkung|nl2br}
{/if}
  </td>
  <td valign="top" align="center"><span {$Auftrag->getPrioritaetsFarbe()}>
      {$Auftrag->getPrioritaet()}</span><br />
      <i {if ( $Auftrag->Status == EVENT_STATUS_ERLEDIGT )}
        style="background-color:#80FF80"
        {/if}
      >{$Auftrag->getStatus()}</i>
       <br />
       {foreach from=$Auftrag->Betroffene key=nr item=betroffener}
         {$Auftrag->getBetroffenenName($nr)}<br />
       {/foreach}
  </td>
  <td valign="top" align="center" class="small">
  {foreach from=$Statusse key=status item=value} 
     {if ( $status != $Auftrag->Status )}
         [&nbsp;<a title="auf {$Statusse.$status} setzen" 
         href="?id={$id}&Auftrag={$Auftrag->Event_id}&NewStatus={$status}">{$value}</a>&nbsp;]<br />
     {/if}
   {/foreach}
   </td>
</tr>
{/foreach}
</table>
<br /><br />

{* Aufträge anderer ansehen *}
{if ( isset($User) )}
<hr class="noprint" />
<form action="?id={$id}" method="post" class="noprint">
Aufträge von Benutzer 
<select name="docinput[Benutzer]" size="1">
      {html_options options=$User selected=$BenutzerID}
</select>
<input type="Submit" value="anzeigen" />
</form>
{/if}

<div class="noprint">
 [ <a href="?id={$id}&newauf=1">Auftrag hinzufügen</a> ] 
 [ <a href="?id={$id}&Alle={$Alle}{$BernutzerURL}">{$AlleText}erledigte anzeigen</a> ]
 {if ( isset($smarty.request.SearchOrt))}
    [ <a href="?id={$id}">Alle anzeigen</a> ]
 {/if}
 {if ( isset($smarty.request.MyWork) )}
    [ <a href="?id={$id}">Nur Aufträge für mich anzeigen</a> ] 
 {else}
    [ <a href="?id={$id}&MyWork=1{$BenutzerURL}">Auftragsstatus erteilter Aufträge anzeigen</a> ] 
  {/if}
</div>
ddd