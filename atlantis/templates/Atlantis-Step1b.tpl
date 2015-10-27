{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">
  {include file="zweiKaesten.tpl" Kasten1=Bild Kasten2=Beschreibung}

  <div class="content">
    <form class="Formular" action="{$smarty.server.SCRIPT_NAME}" method="post">
      <label for="Klasse">Ausrichtung</label> 
      <select name="Wahl" id="Klasse">        
        {html_options values=$Fertigkeit_values selected=$Fertigkeit_selected output=$Fertigkeit_output}     
      </select>
      <input type="submit" value="Wählen" />
    </form>
    <br />
    <div class="footer">
      <a href="Atlantis-Step1a.php">Zurück zur magischen Klasse</a><br />
      <a href=".">Startseite</a>
    </div> 
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->

{include file="footer.tpl"}
