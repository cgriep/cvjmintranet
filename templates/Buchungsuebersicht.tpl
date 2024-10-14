<!-- Template Buchungsuebersicht -->

<noscript>
<div class="Fehler">Javascript ist deaktiviert. Keine Belegung möglich.</div>
</noscript>


<script src="/javascript/overlib.js" type="text/javascript"></script>
<div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>

{if $buchung.Buchung_Nr>0}
<form action="?id={$id}" name="belegung" method="post">
  <input type="hidden" name="Buchung_Nr" value="{$buchung.Buchung_Nr}"/>
  <input type="hidden" name="Tag" value="{$Tag}" />
  {if isset($docinput.Page)}
  <input type="hidden" name="docinput[Page]" value="{$docinput.Page}"/>
  {/if}
  <input type="hidden" name="docinput[Art]" value="{$docinput.Art}"/>
  {if isset($docinput.Alle)}
  <input type="hidden" name="docinput[Alle]" value="{$docinput.Alle}"/>
  {/if}
  <h1>{$Title} {$buchung.Buchung_Nr}</h1>
  {$Buchungskopf}  
{else}
  <h1>{$Title}</h1>

<!--  Kalenderskripte -->
<style type="text/css">
@import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>

<form action="?id={$id}" method="post" name="Wahl">
  <label for="Datum">Belegung ab Datum</label>
  <input type="hidden" name="docinput[Art]" value="{$docinput.Art}" />
  <input type="Text" name="Datum" value="{$Tag|date_format:"%d.%m.%Y"}" size="10" maxlength="10"    
    onClick="popUpCalendar(this,Wahl['Datum'],'dd.mm.yyyy')"
    onBlur="autoCorrectDate('Wahl','Datum' , false )"  />
  <input type="Submit" value="anzeigen" />
</form>
{/if}
<div class="zentriert">
 [ <a href="{$Buchungurl}">Buchungsliste</a> ]
{foreach from=$Links key=Bezeichnung item=Link}
  [ <a href="?id={$id}&{$Link}">{$Bezeichnung}</a> ]
{/foreach}
{if $buchung.Buchung_Nr>0}
  [ <a href="{$Personalplanungurl}{$dat}">Personalplanung</a> ]
{/if}
</div}
<div id="Fehleranzeige" class="Fehler"></div>
<!--  Reiter aufbauen -->
<table class="volleTabelle" cellspacing="0px" cellpadding="5px">
<tr>
{foreach from=$Kategorien item=kategorie}
  <td class="{$kategorie.Color}" width="1" background="img/edge_left.gif">
    &nbsp; 
  </td>
  <td class="{$kategorie.Color}" align="center" nowrap="nowrap">
    <a class="{$kategorie.Class}" href="?id={$id}&{$params}&docinput[Page]={$kategorie.id}">
    {$kategorie.Bezeichnung}</a>
  </td>
  <td class="{$kategorie.Color}" width="1" background="img/edge_right.gif">
    &nbsp;
  </td>  
{/foreach}
  <td bgcolor="{$default_background}" width="100%">
    &nbsp;
  </td>
</tr>
</table>
<!-- Tabelle aufbauen -->
<table class="volleTabelle" cellpadding="1px" cellspacing="0px">
<tr class="helleZeile">
  <td class="zentriert" colspan="{$smarty.session.ANZAHLTAGE+2}">
  {if $buchung.Buchung_Nr > 0 && !$buchung.Fertig && $mitZeit }
    <input type="Submit" name="Save" value="Buchungen speichern" /> 
  {/if}
  </td>
</tr>
<tr class="ueberschrift">
  <th>
    {$Artikelart}
  </th>
  <td></td>
  <td {if $buchung.Buchung_Nr >= 0 && $buchung.Von < $Tag} bgcolor="red"{/if} >  
  <a href="?id={$id}&{$params}&Tag={$TageZurueck}" title="5 Tage früher">&lt;&lt;&lt;</a>
  </td>
  <th colspan="{eval var=$smarty.session.ANZAHLTAGE-2}">
    <a href="?id={$id}&{$params}&Tage={eval var=$smarty.session.ANZAHLTAGE-5}">&larr;</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
    Datum
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="?id={$id}&{$params}&Tage={eval var=$smarty.session.ANZAHLTAGE+5}">&rarr;</a>
  </th>
  <td align="right"  
  {if $buchung.Buchung_Nr >= 0 && $buchung.Bis >= $AnzeigeEnde}
    bgcolor="red" {/if} >
    <a href="?id={$id}&{$params}&Tag={$TageVor}" title="5 Tage später">&gt;&gt;&gt;</a>
  </td>
</tr>
<tr class="ueberschrift">
  {if $buchung.Buchung_Nr>0}
  <td>
    [ <a href="?id={$id}&{$params}{if !isset($docinput.Alle)}&docinput[Alle]=1{/if}">
    {if isset($docinput.Alle)}Wichtige Orte{else}Alle Orte{/if}</a> ]<br />
  {if $buchung.Buchung_Nr > 0 && ! $buchung.Fertig && ! $mitZeit }
    <input type="Checkbox" id="Gesamt" name="docinput[Gesamt]" value="v" 
      onClick="javascript:xajax_gesamterBereich({$buchung.Buchung_Nr},'{$docinput.Artikelnummern}',{$docinput.Page},this.checked);" /> 
    <label for="Gesamt">Gesamter Bereich</label>
  {/if}
  </td>
  <td>Alle<br />Tage</td>
  {else}
  <td></td><td></td>
  {/if}
  {foreach from=$Tage item=datum}
     <td class="zentriert">
     {if isset($buchung.Von) && $buchung.Von <= $datum && $buchung.Bis >= $datum}
       <strong>
     {/if}
     {$datum|date_format:"%a<br />%d.<br />%m.<br />%Y"}
     {if isset($buchung.Von) && $buchung.Von <= $datum && $buchung.Bis >= $datum}
       </strong>
     {/if}     
     </td>
  {/foreach}
</tr>
{foreach from=$Artikel item=artikel}
<tr class="{cycle name="zeilenfarbe" values="helleZeile,dunkleZeile"}">
  <td valign="top">
    <a href="{$Artikelurl}&docinput[Artikel_Nr]={$artikel.id}#Edit"
    {if $artikel.Beschreibung != ''}
      onMouseOver="return overlib('{$artikel.Beschreibung|escape}',CAPTION,'Artikel {$artikel.Bezeichnung|escape}');" 
      onMouseOut="return nd();"
    {/if} >
      {$artikel.Pfad}
    </a>
  </td>      
  <td valign="top">
  {if $buchung.Buchung_Nr> 0 && !$mitZeit }
    <input type="Checkbox" title="für den gesamten Aufenthalt" value="v" id="Alle{$artikel.id}"
    name="{if $artikel.anzahl == ($buchung.Bis - $buchung.Von)/86400+1}Reise" 
    checked="checked{else}Artikel[{$artikel.id}][Reise]{/if}" 
    onClick="javascript:xajax_gesamteReise({$buchung.Buchung_Nr},{$artikel.id},{$docinput.Page},this.checked,'{$docinput.Artikelnummern}');"
    /> 
  {/if}
  </td>
  {foreach from=$artikel.tage key=tagnr item=zeile}
  <td class="{cycle name="zelle" values="RD,RH"}">
    {foreach from=$zeile.Buchungen item=eintrag}
    <div style="background-color:{$eintrag.Farbe}" class="zentriert">
        <a href="?id={$id}&{$params}&Buchung_Nr={$eintrag.F_Buchung_Nr}" 
          title="{$eintrag.Name} {$eintrag.Zusatz} ({$eintrag.Ort}) {$eintrag.Status}">
          [{$eintrag.F_Buchung_Nr}] {if $eintrag.Menge > 1}{$eintrag.Menge}{/if}
        </a>
    </div>
    {/foreach}    
    {if $buchung.Buchung_Nr >= 0}
    <div id="A{$artikel.id}D{$tagnr}">
    {foreach from=$zeile.Aktiv item=eintrag}
      {include file="Buchungsuebersicht_AktiveZelle.tpl"}
    {/foreach}
    {if Count($zeile.Aktiv)==0}
      {include file="Buchungsuebersicht_InaktiveZelle.tpl"}
    {/if}
    </div>
    {else}
      {if Count($zeile.buchungen)==0}
      &nbsp;      
      {/if}
    {/if}         
  </td>
  {/foreach}
</tr>
{/foreach}  
{if isset($Schlafplaetze)}
<tr class="ueberschrift">
  <td>
    <em>Schlafplätze</em>
  </td>
  <td></td>
  {foreach from=$Schlafplaetze key=tagnr item=platz}
  <td class="zentriert" id="Schlaf{$tagnr}">{$platz}</td>
  {/foreach}        
</tr>
{/if}
<tr class="helleZeile">
  <td>
    <input type="hidden" name="docinput[Artikelnummern]" value="{$docinput.Artikelnummern}" />
  </td>
  <td></td>
  <td colspan="{$smarty.session.ANZAHLTAGE/2}" align="left"
  {if $buchung.Buchung_Nr >= 0 && $buchung.Von < $Tag}
    bgcolor="red"
  {/if}>
    <a href="?id={$id}&{$params}&Tag={$TageZurueck}" title="5 Tage früher">&lt;&lt;&lt;</a>
  </td>
  <td align="right" colspan="{math equation="x/2+(x%2)" x=$smarty.session.ANZAHLTAGE}" 
  {if $buchung.Buchung_Nr >= 0 && $buchung.Bis >= $AnzeigeEnde}
    bgcolor="red"
  {/if}><a href="?id={$id}&{$params}&Tag={$TageVor}" title="5 Tage später">&gt;&gt;&gt;</a>
  </td>
</tr>
</table>
{if $buchung.Buchung_Nr >= 0 && ! $buchung.Fertig && $mitZeit }
  <div class="zentriert"><input type="Submit" name="Save" value="Buchungen speichern" /></div>
  </form>
{/if}

<div class="zentriert">[ <a href="{$Buchungurl}">Buchungsliste</a> ]</div>
