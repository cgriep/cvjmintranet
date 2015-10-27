{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">

  {include file="zweiKaesten.tpl" Kasten1=Bild Kasten2=Beschreibung}

  <div class="content">
    <form class="Formular" action="{$smarty.server.SCRIPT_NAME}" method="post">
      <label for="Rasse">Rasse</label> <select name="Rasse" id="Rasse" 
        onChange="xajax_zeigeRasse(document.getElementById('Rasse').value); return false;"
        onKeyUp="xajax_zeigeRasse(document.getElementById('Rasse').value); return false;">
        {html_options values=$rassen_values selected=$rassen_selected output=$rassen_output}
      </select>
      <input type="submit" value="Wählen"/>
    </form>            
    <div class="footer">
      <a href="Atlantis-Step0.php">zurück zur Namenswahl</a><br />
      <a href=".">Startseite</a>
    </div>      
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->


{include file="footer.tpl"}
<script language="javascript">
  xajax_zeigeRasse(document.getElementById('Rasse').value); 
</script>