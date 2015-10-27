{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">

  {include file="zweiKaesten.tpl" KastenGross=Daten}  
  <div class="content">
    <h1>Auswahl des Charakters</h2>
    <ul>
    {foreach name=chars key=id item=char from=$Charaktere}
    <li><a href="Atlantis-Name.php?Charakter_id={$id}" 
           onMouseOver="xajax_zeigeDaten({$id});">{$char}</a>  
    (<a href="{$self}?Del={$id}" 
    onClick="javascript:return window.confirm('Charakter wirklich unwiderruflich löschen?');">Löschen</a>)</li>
    {/foreach}
    </ul>
    <div class="footer">
      <a href=".">Startseite</a>
    </div>
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->

{include file="footer.tpl"}
