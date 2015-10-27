{* Gibt die Kategorien einer Adresse aus *}
{foreach from=$Adresse->getKategorien() item=adr}
    <a href="?id={$id}&docinput[Kategorie]={$adr.Kategorie_id}">{$adr.Kategorie}</a>&nbsp;&bull;&nbsp;
{/foreach}