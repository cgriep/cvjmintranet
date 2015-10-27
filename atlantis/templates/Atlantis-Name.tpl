{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">

  {if $Bild != ''} 
      {include file="zweiKaesten.tpl" Kasten1=Bild Kasten1Bild=$Bild Kasten2=Daten}  
  {else}
      {include file="zweiKaesten.tpl" KastenGross=Daten}  
  {/if}
    
  <div class="content">
    {if $Meldung!=''}<div class="Meldung">{$Meldung}</div>{/if}
    <form class="Formular" action="{$smarty.server.PHP_SELF}" method="post" enctype="multipart/form-data">
      <label for="Name">Name des Charakters</label> 
      <input type="text" name="Name" id="Name" value="{$Charakter->Charaktername|escape}"/><br />
      <label for="Bild">Bild</label> <input type="file" id="Bild" name="Bild" /><br />
       {if $Bild!=''}<a href="{$smarty.server.PHP_SELF}?DelBild=1">vorhandenes Bild löschen{/if}
      <label>Contage/Atlantis</label> 
      <input class="kurz" type="text" value="{$Charakter->Contage|default:0}" readonly="readonly" /> 
      <div style="float:left">/</div>
      <input class="kurz" type="text" value="{$Charakter->Atlantiscontage|default:0}" readonly="readonly" />
      <h2 class="abschluss">Con ergänzen</h2>
      <label for="Contage">Contage</label>
      <input type="text" name="Contage" value="3" id="Contage" maxlen="3" /><br />
      <label for="Atlantis">Atlanis-Con</label>
      <input type="checkbox" name="Atlantis" value="v" id="Atlantis" /><br />
      <label>Conname</label>
      <input type="text" name="Con" value="" /><br />      
      <input type="submit" value="Absenden"/>
    </form>
    <div class="footer">
      <a href="Atlantis-Step3a.php">Fertigkeiten bearbeiten</a><br />
      {if $Charakter->isUebernatuerlich()}
      <a href="Atlantis-Step4.php">Übernatürliche Vorteile aktivieren</a><br />
      {/if}
      <a href="Charakterbogen.php">Charakterbogen</a><br >
      <a href="{$self}?Godmode=1">SL-Bearbeitung Charakter</a><br />
       <a href="Atlantis-Auswahl.php">anderen Charakter auswählen</a><br />
       <a href=".">Startseite</a>
    </div>    
    {$Charakter->Historie|nl2br}
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->

{include file="footer.tpl"}
<script language="javascript">
  xajax_zeigeDaten(); 
</script>