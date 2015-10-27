{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">
  {include file="zweiKaesten.tpl" KastenGross=Beschreibung}

  <div class="content">
    Noch wählbare Spruchlisten: {$Spruchlistenanzahl}<br />
    <form class="Formular nebeneinander" action="{$smarty.server.SCRIPT_NAME}" method="post">
       <label for="Spruchliste">Spruchlisten</label><br /> 
       <select name="Spruchliste" id="Spruchliste" size="5"
        onChange="xajax_zeigeSpruchliste(document.getElementById('Spruchliste').value); return false;"
        onKeyUp="xajax_zeigeSpruchliste(document.getElementById('Spruchliste').value); return false;">        
        {html_options values=$Spruchliste_values selected=$Spruchliste_selected output=$Spruchliste_output}     
      </select>      
      {if $Spruchlistenanzahl > 0} 
        <input type="submit" value="Wählen" /> 
      {/if}
    </form>
    {if Count($vSpruchliste_values) > 0}
    <form class="Formular nebeneinander" action="{$smarty.server.SCRIPT_NAME}" method="post">
      <label for="Ent">gewählte Spruchlisten</label> <br />
      <select name="Entfernen" size="5" id="Ent"
        onChange="xajax_zeigeSpruchliste(document.getElementById('Ent').value); return false;"
        onKeyUp="xajax_zeigeSpruchliste(document.getElementById('Ent').value); return false;">
        {html_options values=$vSpruchliste_values selected=$vSpruchliste_selected output=$vSpruchliste_output}
      </select>
      <input type="submit" value="Entfernen"/>
    </form>
    {/if}
    <br class="abschluss" />  
    <div class="footer">
    {if $Spruchlistenanzahl==0}
        <a href="?Weiter=1">Spruchlisten übernehmen und weiter</a>
    {/if}
       <a href="Atlantis-Step1b.php">Zurück zur Wahl der Ausrichtung</a><br />
       <a href=".">Startseite</a>
    </div>
 
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->
<script language="javascript">
  xajax_zeigeSpruchliste(document.getElementById('Spruchliste').value); 
</script>

{include file="footer.tpl"}
