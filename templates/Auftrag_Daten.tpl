<!--  Popup-Kalender -->
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>


<form action="?id={$id}" name="Auftragform" method="post">
<input type="hidden" name="Auftrag" value="{$Auftrag->Event_id}">
<h2>{$Auftrag->Titel}</h2>
Auftrag erstellt von {$Auftrag->getAutor()} am {$Auftrag->Datum|date_format:"%d.%m.%Y %H:%M"}
{if ( $Auftrag->wurdeGeaendert() )}
, zuletzt geändert am {$Auftrag->changed|date_format:"%d.%m.%Y %H:%M"}
{/if}
<br />
<label>Verantwortlich</label> 
<select name="Betroffen[]" multiple="multiple" size="10">
{html_options options=$Auftrag->getListOfGroupsAndUsers() selected=$Auftrag->getBetroffene()}
</select> Um mehrere Einträge zu markieren bitte Strg gedrückt halten.<br />

<label>Titel</label><br /><input type="Text" name="Titel" value="{$Auftrag->Titel}" size="50" maxlength="50" /><br />
<label>Beschreibung</label><br /><textarea name="Beschreibung" cols="60" rows="5">{$Auftrag->Beschreibung}</textarea><br />

<label>Priorität</label> 
<select name="Prioritaet">
{html_options options=$Auftrag->getPrioritaeten() selected=$Auftrag->Prioritaet} 
</select>
&nbsp;&nbsp;&nbsp;&nbsp;<label>Status</label> 
<select name="Status">
{html_options options=$Auftrag->getStatusse() selected=$Auftrag->Status} 
</select><br />
<label for="BearbeitenBis">Bearbeiten bis</label>
<input type="text" name="BearbeitenBis" 
     onClick="popUpCalendar(this,this,'dd.mm.yyyy')" 
     onBlur="autoCorrectDate('Auftragform','BearbeitenBis',false);"          
 

/> &nbsp;&nbsp;
<label for="Ort">Ort</label> 
<input type="hidden" name="Ort" value="{$Auftrag->Ort}" id="Ortsauswahl" />
{if ( isset($smarty.request.MyWork) )}
	<input type="hidden" name="MyWork" value="1" />
{/if}
<span name="Ortname" id="Ortsauswahltext">{$Auftrag->getOrt(true)}</span>
&nbsp;&nbsp;[ <a href="javaScript:OrtsAuswahl('Ortsauswahl')">Ort auswählen</a> ]<br />
{if ( ! isset($smarty.request.newauf))}
	<label>Bemerkungen</label><br />
	<textarea name="Bemerkung" cols="60" rows="5">{$Auftrag->Bemerkung}</textarea><br />
{/if}
<input type="Submit" name="saveauf" value="Speichern" />
{if ( !$Auftrag->isNeu() && $Auftrag->Autor == SESSION_DBID )}
	&nbsp;&nbsp;&nbsp;&nbsp;[ <a title="Löschen" href="?id={$id}&Delete={$Auftrag->Event_id}">Löschen</a> ]
{/if}
&nbsp;&nbsp;&nbsp;&nbsp;[ <a title="Auftragsliste anzeigen" href="?id={$id}">Auftragsliste anzeigen</a> ]
</form>
