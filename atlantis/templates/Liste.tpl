{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">
{$Hinweis}
<br/>
{if Count($zeilen) > 0}
{foreach name=top key=index item=feld from=$zeilen}
<table class="Liste">
<caption>{$index}</caption>
<tr>
{foreach name=tab key=header item=zeile from=$feld.0}
  <th>{$header}</th>
{/foreach}
</tr>
{foreach name=tab key=header item=zeile from=$feld}
<tr>
  {foreach name=tab key=x item=spalte from=$zeile}
  <td>{$spalte}</td>
  {/foreach}
</tr>
{/foreach}
</table>

{/foreach}
<a href="Atlantis-XML.php?action={$smarty.request.action}">XML-Ausgabe</a><br />
<a href="{$smarty.server.PHP_SELF}">Zurück</a>
{else}
<ul>
<li><a href="{$smarty.server.PHP_SELF}?action=Rassen">Rassenfertigkeiten</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Klassen">Klassen</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Spezialisierungen">Spezialisierungen und Fertigkeiten</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Abhaengigkeiten">Abhängigkeiten</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Fertigkeiten">Fertigkeiten</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Rassendaten">Rassen</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Spezialisierungsklassen">Klassen</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Spezialisierungsdaten">Spezialisierungen allein</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Charaktere">Charaktere</a></li>
<li><a href="{$smarty.server.PHP_SELF}?action=Raenge">Ränge</a></li>
</ul>
{/if}
    
</div> <!-- Seite -->
{include file="footer.tpl"}
