{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">
  {include file="zweiKaesten.tpl" KastenGross=Beschreibung}

  <div class="content">
    <form class="Formular" action="Atlantis-Step3a.php" method="post">
      <label for="Klasse">Fertigkeitenklasse</label> <select name="Klasse" id="Klasse"
      onChange="xajax_zeigeSpruchliste(document.getElementById('Klasse').value); return false;"
        onKeyUp="xajax_zeigeSpruchliste(document.getElementById('Klasse').value); return false;">        
        {html_options values=$fertigkeit_values selected=$fertigkeit_selected output=$fertigkeit_output}     
      </select>
      <input type="submit" value="Anzeigen" />
    </form>
    <br />
    <div class="footer">
      <a href="Atlantis-Name.php">Zurück zum Charakter</a><br />
      <a href=".">Startseite</a>
    </div>  
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->
<script language="javascript">  
  xajax_zeigeSpruchliste({$fertigkeit_selected});
</script>
{include file="footer.tpl"}
