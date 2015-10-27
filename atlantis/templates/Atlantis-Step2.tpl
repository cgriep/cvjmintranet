{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">
  {include file="zweiKaesten.tpl" KastenGross=Beschreibung}

  <div class="content">
    <p>{if $smarty.request.U}
    W�hle hier die �bernat�rlichen Vor- und Nachteile aus. Es d�rfen maximal {$MaxUePunkte} 
    Punkte ausgegeben werden, und alle Vorteilspunkte m�ssen durch Nachteilspunkte 
    ausgeglichen werden.
    {else}
    W�hle nun die Vor- und Nachteile f�r den Charakter. In den normalen Vor- und Nachteilen d�rfen 
    maximal 50 Vorteils- und 40 Nachteilspunkte gew�hlt werden. Die Nachteilspunkte erh�hen die 
    Startpunktzahl f�r Fertigkeiten, w�hrend Vorteile sie reduzieren. Nat�rlich darf die Punktzahl 
    nicht kleiner Null werden.
    {/if}</p>    
    {if $Uebernatuerlich}
      <div>
      {if $smarty.request.U}
      <a href="{$smarty.server.PHP_SELF}">Normale Vor- und Nachteile w�hlen</a>
      {else}
      <a href="{$smarty.server.PHP_SELF}?U=1">�bernat�rliche Vor- und Nachteile w�hlen</a>
      {/if}
      </div> 
    {/if}
    {if $FehlerHinweis != ''}
    <div class="Fehler">{$FehlerHinweis}</div>
    {/if}
    {if !$smarty.request.U || !$Uebernatuerlich}
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}" method="post">
      <label for="Vorteile">m�gliche Vorteile:</label><br />
      <select name="Vorteile" id="Vorteile" size="5"
        onChange="xajax_zeigeFertigkeit(document.getElementById('Vorteile').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('Vorteile').value); return false;">
        {html_options values=$fertigkeit_values selected=$fertigkeit_selected output=$fertigkeit_output}
      </select>
      <input type="submit" value="hinzuf�gen"/>
    </form>
    {if Count($vfertigkeit_values)>0}
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}" method="post">
      <label for="Ent">vorhandene Vorteile:</label> <br />
      <select name="Entfernen" size="5" id="Ent"
        onChange="xajax_zeigeFertigkeit(document.getElementById('Ent').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('Ent').value); return false;">
        {html_options values=$vfertigkeit_values selected=$vfertigkeit_selected output=$vfertigkeit_output}
      </select>
      <input type="submit" value="Entfernen"/>
    </form>
    {/if}
    <br class="abschluss" />    
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}" method="post">
      <label for="Nachteile">m�gliche Nachteile:</label><br />
      <select name="Vorteile" id="Nachteile" size="5"
        onChange="xajax_zeigeFertigkeit(document.getElementById('Nachteile').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('Nachteile').value); return false;">
        {html_options values=$nfertigkeit_values selected=$nfertigkeit_selected output=$nfertigkeit_output}
      </select>
      <input type="submit" value="hinzuf�gen"/>
    </form>
    {if Count($vnfertigkeit_values)>0}
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}" method="post">
      <label for="EntN">vorhandene Nachteile:</label> <br />
      <select name="Entfernen" size="5" id="EntN"
        onChange="xajax_zeigeFertigkeit(document.getElementById('EntN').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('EntN').value); return false;">
        {html_options values=$vnfertigkeit_values selected=$vnfertigkeit_selected output=$vnfertigkeit_output}
      </select>
      <input type="submit" value="Entfernen"/>
    </form>
    {/if}
    <br class="abschluss" />        
    Maximal verwendbare Punkte f�r weitere Vorteile und Fertigkeiten:{$Verwendbarepunkte}<br />       
    {else}
     <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}?U=1" method="post">
        <label>m�gliche �bernat�rliche Vor- und Nachteile:</label><br /> 
        <select name="Vorteile" id="UeVorteile" size="5" 
          onChange="xajax_zeigeFertigkeit(document.getElementById('UeVorteile').value); return false;"
          onKeyUp="xajax_zeigeFertigkeit(document.getElementById('UeVorteile').value); return false;">
        {html_options values=$ufertigkeit_values selected=$ufertigkeit_selected output=$ufertigkeit_output}
        </select>
        <input type="submit" value="Hinzuf�gen"/>
    </form>
    {if Count($vufertigkeit_values)>0}
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}?U=1" method="post">
        <label>vorhandene �bernat�rliche Vor- und Nachteile:</label> <br />
        <select name="Entfernen" size="5" id="UeEnt"
          onChange="xajax_zeigeFertigkeit(document.getElementById('UeEnt').value); return false;"
          onKeyUp="xajax_zeigeFertigkeit(document.getElementById('UeEnt').value); return false;">
          {html_options values=$vufertigkeit_values selected=$vufertigkeit_selected output=$vufertigkeit_output}
        </select>          
        <input type="submit" value="Entfernen"/>
    </form>
    {/if}
    <br class="abschluss" />
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}?U=1" method="post">
      <label for="Nachteile">m�gliche �bernat�rliche Nachteile:</label><br />
      <select name="Vorteile" id="UeNachteile" size="5"
        onChange="xajax_zeigeFertigkeit(document.getElementById('UeNachteile').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('UeNachteile').value); return false;">
        {html_options values=$nufertigkeit_values selected=$nufertigkeit_selected output=$nufertigkeit_output}
      </select>
      <input type="submit" value="hinzuf�gen"/>
    </form>
    {if Count($vnufertigkeit_values)>0}
    <form class="Formular nebeneinander" action="{$smarty.server.PHP_SELF}?U=1" method="post">
      <label for="EntN">vorhandene �bernat�rliche Nachteile:</label> <br />
      <select name="Entfernen" size="5" id="EntNU"
        onChange="xajax_zeigeFertigkeit(document.getElementById('EntNU').value); return false;"
        onKeyUp="xajax_zeigeFertigkeit(document.getElementById('EntNU').value); return false;">
        {html_options values=$vnufertigkeit_values selected=$vnufertigkeit_selected output=$vnufertigkeit_output}
      </select>
      <input type="submit" value="Entfernen"/>
    </form>
    {/if}
    <br class="abschluss" />
    Maximal verwendbare Punkte f�r weitere �bernat�rliche Vorteile: {$MaximalPunkte}<br />  
    {/if}
    <div class="footer">
      <a href="Atlantis-Name.php" onClick="javascript:return window.confirm('Nach dem Erstellen k�nnen keine weiteren �nderungen an den Vor- und Nachteilen gemacht werden. Wirklich speichern?');">
      Charakter erstellen</a><br />
      <a href="Atlantis-Step1.php">zur�ck zur Magiewahl</a><br />
       <a href=".">Startseite</a>
    </div>    
  </div><!-- Ende Content -->

</div> <!-- Seite -->

{include file="footer.tpl"}
