<!-- Template Buchungsuebersicht_kurz -->

<!--  Kalenderskripte -->
<style type="text/css">
@import url(css/popcalendar.css);
{literal}
.Sonntag {
  border-right-style: solid;
  border-right-color: black;
  border-right-width: 1pt;
}
.aktuellerTag {
  font-weight: bold;
}
.Feiertag {
  background-color: lightblue;
}
{/literal}
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>
<!--  Overlib -->
{popup_init src="/javascript/overlib.js"}

<h1>{$Title}</h1>
<div class="noprint">
  <form action="?id={$id}" method="post" name="Wahl">
  <label for="Datum">Belegung ab Datum</label>
  <input type="Text" name="Datum" value="{$Tag|date_format:"%d.%m.%Y"}" size="10" maxlength="10"
     onClick="popUpCalendar(this,Wahl['Datum'],'dd.mm.yyyy')"
     onBlur="autoCorrectDate('Wahl','Datum' , false )" /> 
  auf <input type="Text" name="Tage" value="{$anzahltage}" size="3" maxlength="3" />
  Tage <input type="Submit" value="anzeigen" /><br />
  {html_checkboxes name="docinput[Kategorie]" options=$Kategorien selected=$docinput.Kategorie} 
  </form>
  <div class="zentriert">[ <a href="{$Buchungurl}">Buchungsliste</a> ]</div>
</div>
<a id="Oben" name="Oben"></a>
<table class="volleTabelle" cellspacing="0" cellpadding="1" border="0" style="border-collapse:collapse;">
{foreach from=$Anzeige key=kat_id item=kategorie}
<tr class="ueberschrift">
  <td></td>
  <td>
    <a href="?id={$id}&{$params}&Tag={$TageZurueck}#K{$kat_id}" title="5 Tage früher">&lt;&lt;&lt;</a>
  </td>
  <th colspan="{eval var=$anzahltage-2}">
    <a href="?id={$id}&{$params}&Tage={eval var=$anzahltage-5}" title="5 Tage weniger anzeigen">&larr;</a>
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    {$kategorie.Bezeichnung} {$Tag|date_format:"%Y"}
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    <a href="?id={$id}&{$params}&Tage={eval var=$anzahltage+5}" title="5 Tage mehr anzeigen">&rarr;</a>
  </th>
  <td align="right">
    <a href="?id={$id}&{$params}&Tag={$TageVor}#K{$kat_id}" title="5 Tage später">&gt;&gt;&gt;</a>
  </td>
</tr>
<tr class="ueberschrift">
  <td>
  </td>
  {foreach from=$Tage item=datum}
    <td class="zentriert{if $Tag == $datum} aktuellerTag{/if}{if date('w',$datum)==0} Sonntag{/if}">
       {$datum|date_format:"%a<br/>%d<br/>%m"}
    </td>
  {/foreach}
</tr>
{foreach from=$kategorie.Artikel item=artikel}
<tr class="{cycle name="zeilenfarbe" values="helleZeile,dunkleZeile"}">
  <td valign="top">
    <a href="{$Artikelurl}&docinput[Artikel_Nr]={$artikel.id}#Edit" 
    {if $artikel.Beschreibung != ''}
     {popup text=$artikel.Beschreibung|nl2br|escape caption=$artikel.Bezeichnung|escape}
    {/if} >
    {$artikel.Pfad}
    </a>
  </td>
  {foreach from=$artikel.Tage key=nr item=Buchungen} 
  <td class="{cycle name="zelle" values="RD,RH"}{if date('w',$Tage.$nr)==0} Sonntag{/if}{if $feiertag.$nr} Feiertag{/if}">
    {foreach from=$Buchungen item=Buchung}       
        <a href="{$Buchungurl}&Buchung_Nr={$Buchung.Buchung_Nr}" 
        style="background-color:{$Buchung.Color};"
       {assign value='Buchung '|cat:$Buchung.Buchung_Nr var=titel}
       {assign value=$Buchung.Name|cat:"\n"|cat:$Buchung.Zusatz|cat:"\n"|cat:$Buchung.Ort|cat:"\n"|cat:$Buchung.Status var=buchungtext}
       {popup text=$buchungtext|nl2br|escape caption=$titel}
       >X</a>      
    {/foreach}
    {if count($eintraege) == 0}
      &nbsp;
    {/if}
  </td>
  {/foreach}
  </tr>
{/foreach}
  
<tr class="helleZeile">
  <td>
  </td>
  <td colspan="{eval var=$anzahltage/2}" align="left">
    <a href="?id={$id}&{$params}&Tag={$TageZurueck}#K{$kat_id}" title="5 Tage früher">&lt;&lt;&lt;</a>
  </td>
  <td align="right" colspan="{math equation="x/2+a%2" x=$anzahltage}">
    <a href="?id={$id}&{$params}&Tag={$TageVor}#K{$kat_id}" title="5 Tage später">&gt;&gt;&gt;</a>
  </td>
</tr>
<tr>
  <td colspan="{eval var=$anzahltage+1}" class="zentriert noprint">
  [ <a href="#Oben">nach oben</a> ] [ <a href="{$Buchungurl}">Buchungsliste</a> ]
  </td>
</tr>
{/foreach}
</table>
