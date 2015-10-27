{config_load file=test.conf section="setup"}
{include file="header.tpl" title=foo}
<!-- Ende Kopf -->
<div class="Seite">

  <div class="content">
  <h1>Willkommen in Atlantis!</h1>
  <br />
  Was wollen Sie tun:
  <ul>
  <li><a href="Atlantis-Step0.php">Erstellen eines neuen Charakters</a></li>
  <li><a href="Atlantis-Name.php">Bearbeiten eines bestehenden Charakters</a></li>
  <li><a href="Atlantis-Admin.php">Listen der Datenbank ansehen</a></li>
  </ul>
  </div><!-- Ende Content -->
    
</div> <!-- Seite -->

{include file="footer.tpl"}
