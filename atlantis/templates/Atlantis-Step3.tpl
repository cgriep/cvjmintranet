{config_load file=test.conf section="setup"}
{include file="header.tpl" title=Atlantis}
<!-- Ende Kopf -->
<div class="Seite">
  {include file="zweiKaesten.tpl" KastenGross=Beschreibung}

  <div class="content">
    <h1>{$Spezialisierung}</h1>     
     {if $FehlerHinweis!=''}<div class="Fehler">{$FehlerHinweis}</div>{/if}
    verbleibende Punkte {$Charakter->Punkte}<br />
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}" method="post">    
      moegliche Fertigkeiten:<br />
      <select name="Fertigkeit" id="Fertigkeiten" size="8"
        onChange="xajax_zeigeFertigkeit(document.getElementById('Fertigkeiten').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('Fertigkeiten').value); return false;">
        {html_options values=$fertigkeit_values selected=$fertigkeit_selected output=$fertigkeit_output}
      </select>
      <input type="submit" value="hinzufügen"/>
    </form>        
    {if Count($vfertigkeit_values)>0}
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}" method="post">
      vorhandene Fertigkeiten:<br /> 
      <select name="Entfernen" size="8" id="Ent"
        onChange="xajax_zeigeFertigkeit(document.getElementById('Ent').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('Ent').value); return false;">
        {html_options values=$vfertigkeit_values selected=$vfertigkeit_selected output=$vfertigkeit_output}
      </select>
      <input type="submit" value="Entfernen"/>
    </form>    
    {/if}
    <div class="footer">
      <a href="Atlantis-Step3a.php">andere Fertigkeitenklasse</a><br />
      <a href="Atlantis-Name.php">zurück zum Charakter</a><br />
      <a href="{$smarty.server.PHP_SELF}?MehrPunkte=1">Mehr punkte</a><br />
       <a href=".">Startseite</a>
    </div>     
  </div><!-- Ende Content -->

</div> <!-- Seite -->
{include file="footer.tpl"}
