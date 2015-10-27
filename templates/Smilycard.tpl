<!--  Popup-Kalender -->
<style type="text/css">
  @import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>
<a href="?id=1093&Buchung_Nr={$Buchung->Buchung_Nr}">Zurück zur Buchung</a>

<h2>Buchung {$Buchung->Buchung_Nr} und Unterbringung</h2>
Name der Gruppe: 
<a href="{$Adressebearbeiten}&Bearbeiten={$Buchung->Adresse->Adressen_id}">
{$Buchung->Adresse->Vollname()}</a> ({$Buchung->Adresse->Kunden_Nr}
    ) &nbsp;{$Buchung->Adresse->Zusatz}<br />

Datum der Anwesenheit: {$Buchung->Von|date_format:"%d.%m.%Y"} bis {$Buchung->Bis|date_format:"%d.%m.%Y"}<br /><br />
{assign var="smilycard" value=$Buchung->getSmilycard()}



<form method="post" action="?id={$id}&Buchung_Nr={$Buchung->Buchung_Nr}">
Datum der Smilycard: <input type="Smilycard[Datum]" value="{$smilycard.Datum|date_format:"%d.%m.%Y"}" /><br />
{if $smilycard.invalid==1}
<div class="Achtung">Diese Smileycard ist als nicht abgegeben markiert worden.</div>
Speichern Sie die Karte, um die Markierung aufzuheben.
{else}
Smileycard als nicht abgegeben markieren: <input type="checkbox" name="Smilycard[invalid]" />
{/if}
<br />

Wie sind Sie auf das CVJM Feriendorf aufmerksam geworden:<br />
{html_checkboxes name="Smilycard[Herkunft]" options=$Herkunft selected=$smilycard.Herkunft separator="<br />"}

Wo waren Sie untergebracht: <br />
{html_checkboxes name="Smilycard[Unterbringung]" options=$Unterbringung selected=$smilycard.Unterbringung}
<br />

<br />
Wie beurteilen Sie
<table>
<tr>
	<td></td>
	<td><img src="/img/CVJM/Smilies.png" />
	</td>
</tr>
<tr>
	<td>Anmeldevorgang bei Reservierung</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungAnmeldung name="Smilycard[BeurteilungAnmeldung]"} 
	</td>
</tr>
<tr>
	<td>Empfang/Anreise</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungEmpfang name="Smilycard[BeurteilungEmpfang]"} 
	
	</td>
</tr>
<tr>
	<td>Parken</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungParken name="Smilycard[BeurteilungParken]"} 
	</td>
</tr>
<tr>
	<td>Sauberkeit</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungSauberkeit name="Smilycard[BeurteilungSauberkeit]"} 
	
	</td>
</tr>
<tr>
	<td>Ausstattung</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungAusstattung name="Smilycard[BeurteilungAusstattung]"} 
	
	</td>
</tr>
<tr>
	<td>Bequemlichkeit</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungBequemlichkeit name="Smilycard[BeurteilungBequemlichkeit]"} 
	</td>
</tr>
<tr>
	<td>Sanitäre Anlagen</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungSanitaer name="Smilycard[BeurteilungSanitaer]"} 
	
	</td>
</tr>
<tr>
	<td>Gästebetreuung</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungBetreuung name="Smilycard[BeurteilungBetreuung]"} 
	</td>
</tr>
<tr>
	<td>Verpflegung</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungVerpflegung name="Smilycard[BeurteilungVerpflegung]"} 
	</td>
</tr>
<tr>
	<td>Spiel- und Sportanlagen</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungAnlagen name="Smilycard[BeurteilungAnlagen]"} 
	</td>
</tr>
<tr>
	<td>Tiere</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungTiere name="Smilycard[BeurteilungTiere]"} 
	</td>
</tr>
<tr>
	<td>Abreise</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungAbreise name="Smilycard[BeurteilungAbreise]"} 
	</td>
</tr>
</table>

Worauf Sie uns noch aufmerksam machen möchten<br />
<textarea name="Smilycard[Aufmerksamkeit]" cols="50" rows="5">{$smilycard.Aufmerksamkeit}
</textarea>

<h2>Programm und Freizeitaktivitäten</h2>
Welches Programm haben Sie in Anspruch genommen?<br />
{html_checkboxes name="Smilycard[Programm]" options=$Programm selected=$smilycard.Programm separator="<br />"}
<br />

Wie beurteilen Sie
<table>
<tr>
	<td>Vorbereitungsmaterial</td>
	<td>
	{html_radios values=$Werte selected=$smilycard.BeurteilungVorbereitungsmaterial name="Smilycard[BeurteilungVorbereitungsmaterial]"} 
	</td>
</tr>
<tr>
	<td>Anleitung und Motivation (Methoden)</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungAnleitung name="Smilycard[BeurteilungAnleitung]"} 
	</td>
</tr>
<tr>
	<td>Materialeinsatz</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungMaterial name="Smilycard[BeurteilungMaterial]"} 
	</td>
</tr>
<tr>
	<td>zeitliche Gestaltung des Prog.-Ablaufes</td>
	<td>
		{html_radios values=$Werte selected=$smilycard.BeurteilungGestaltung name="Smilycard[BeurteilungGestaltung]"} 
	</td>
</tr>
</table>

Wünsche und Anregungen dazu:<br />
<textarea name="Smilycard[ProgrammWuensche]" cols="50" rows="5">{$smilycard.ProgrammWuensche}
</textarea><br />

Programmhighlights<br />
<textarea name="Smilycard[ProgrammHighlights]" cols="50" rows="5">{$smilycard.ProgrammHighlights}
</textarea><br />

Allgemeine Anregungen und Programmwünsche<br />
<textarea name="Smilycard[ProgrammAnregungen]" cols="50" rows="5">{$smilycard.ProgrammAnregungen}
</textarea>
<br />
<input type="submit" value="Speichern"/>
</form>
<a href="?id=1093&Buchung_Nr={$Buchung->Buchung_Nr}">Zurück zur Buchung</a>
<br />
{$smilycard.Historie|nl2br}