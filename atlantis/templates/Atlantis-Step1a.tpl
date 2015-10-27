{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">
  {include file="zweiKaesten.tpl" Kasten1=Bild Kasten2=Beschreibung}

  <div class="content">
    <form class="Formular" action="Atlantis-Step1a.php" method="post">
      <label for="Klasse">Klasse</label> <select name="Klasse" id="Klasse"
        onChange="xajax_zeigeKlasse(document.getElementById('Klasse').value); return false;"
        onKeyUp="xajax_zeigeKlasse(document.getElementById('Klasse').value); return false;">
        {html_options values=$Klassen_values selected=$Klassen_selected output=$Klassen_output}     
      </select>
      <input type="submit" value="Wählen" />
    </form>
    <br />
    <div class="footer">
      <a href="Atlantis-Step1.php">Zurück zur Magiewahl</a><br />
      <a href=".">Startseite</a>      
    </div>
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->
<script language="javascript">
  xajax_zeigeKlasse(document.getElementById('Klasse').value); 
</script>
{include file="footer.tpl"}
