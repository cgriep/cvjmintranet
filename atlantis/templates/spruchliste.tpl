<h1>{$Spezialisierung.Spezialisierung}</h1>

{if Count($Sprueche)>0}
<table class="Liste">
<tr>
  <th>Fertigkeit</th>
  <th>Kosten</th>
  <th>Rang</th>
</tr>
{foreach name=sprueche key=nr item=spruch from=$Sprueche}
<tr>
  <td><span {popup textsize="1" text=$spruch.Beschreibung|escape}>{$spruch.Fertigkeit}</span></td>
  <td>{$spruch.Kosten}</td>
  <td>{$spruch.Rang}</td></tr>
{/foreach}
</table>
{if !$Spezialisierung.Spruchliste && ! $Spezialisierung.Allgemein}
Zum Meister in dieser Spezialisierung benötigt man {$Spezialisierung.Meisterpunkte}, 
zum Großmeister {$Spezialisierung.Grossmeisterpunkte} Punkte in dieser oder der 
allgemeinen Spezialisierung der Klasse {$Spezialisierung.Klasse}.
{/if}
{if $Spezialisierung.Allgemein}
Dies ist die allgemeine Spezialisierung der Klasse {$Spezialisierung.Klasse}. 
Zum Meister oder Großmeister muss man in einer der nicht allgemeinen Spezialisierungen
den entsprechenden Rang haben.
{/if}
{if $Spezialisierung.Spruchliste}
Dies ist eine Spruchliste. Die Punkte zählen zur Klasse {$Spezialisierung.Klasse}. 
{/if}

{else}
(keine Beschreibung vorhanden)
{/if}