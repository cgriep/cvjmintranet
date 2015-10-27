{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">
  <div class="content">
    <form class="Formular" action="{$smarty.server.SCRIPT_NAME}" method="post">
      <label for="Magisch">Magiefaehig</label>
      <select name="Magisch" id="Magisch">
        {html_options values=$Fertigkeit_values selected=$Fertigkeit_selected output=$Fertigkeit_output}     
      </select>
      <input type="submit" value="Wählen"/>
    </form>
  <div class="footer">
    <a href="Atlantis-Step0a.php">Zurück zur Rassenwahl</a><br />
    <a href=".">Startseite</a>
  </div>
  </div><!-- Ende Content -->   
</div> <!-- Seite -->
{include file="footer.tpl"}
