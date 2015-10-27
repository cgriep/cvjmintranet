{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">

  <div class="content">
    {if $Fehler!=''}<div class="Fehler">{$Fehler}</div>{/if}
    <form class="Formular" action="Atlantis-Step0.php" method="post" enctype="multipart/form-data">
      <label for="Name">Name des Charakters</label> <input type="text" id="Name" name="Name" value="{$Name}" />
      <input type="submit" value="Wählen"/>
    </form>       
    <div class="footer">
      <a href="?Neu=1">Neuen Charakter beginnen</a>
    </div>    
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->

{include file="footer.tpl"}
