{* Artikel der Buchung anzeigen *}
{* Verwendung sowohl in Buchung als auch Abrechnung *}
<script language="javascript">
var artikel=-1;
var datum=-1;
var id=-1;
function personEintragen(id,artikel,datum)
{literal}
{
  person = document.getElementById("Personname"+id).value;
  showPerson(id,artikel,datum,false);
{/literal}
  xajax_tragePersonEin({$smarty.request.Buchung_Nr},artikel,datum,person,id);
{literal}
}
function speichern(save)
{
	if ( save )
	{
	{/literal}
		xajax_tragePersonEin({$smarty.request.Buchung_Nr},artikel,datum,xajax.getFormValues('Daten'),id);
	{literal}
	}
	showPerson(id,artikel,datum,false);
}
function showPerson(dieid,derArtikel,dasDatum,an)
{
  if ( an )
  {
    id=dieid;
    datum=dasDatum;
    artikel=derArtikel;
    //element = document.getElementById('Person'+id);
    //element.innerHTML = 'zuständig: <input type="text" id="Personname'+id+'"/>';
    //element.innerHTML = element.innerHTML+'<button onClick="personEintragen('+id+','+artikel+',\''+datum+'\')">eintragen</button>';
    document.getElementById('PersonLink'+id).onclick = function(){showPerson(dieid,derArtikel,dasDatum,false);return false;};
    document.getElementById('PersonLink'+id).title = "Personenanzeige ausblenden";
    // Personenwahlfenster öffnen
    {/literal}
    xajax_zeigePersonenwahlDialog({$smarty.request.Buchung_Nr},artikel,datum);
    {literal} 
  }
  else
  {
    xajax_schliessePersonenwahlDialog();
    //document.getElementById('Person'+id).innerHTML = "";
    document.getElementById('PersonLink'+id).onclick = function() {showPerson(dieid,derArtikel,dasDatum,true);return false;};
    document.getElementById('PersonLink'+id).title = "zuständige Person festlegen";
    id=-1;
    datum=-1;
    artikel=-1;
  }
}
</script>
{/literal}
{assign var="nr" value="0"}
{foreach from=$Artikel key=meinkey item=meinArtikel}
<tr>
  <td colspan="3"></td>
  <td colspan="3"><strong>{$Artikelarten.$meinkey}</strong></td>
</tr>
  {foreach from=$meinArtikel item=eintrag}
<tr class="{cycle values="helleZeile,dunkleZeile"}">
  <td>{$eintrag.wieviel}</td>
  <td valign="right">
  {if $Edit && $eintrag.MengeAnzeigen == $eintrag.Mengen.0}
    <input type="Text" name="docinput[Menge][{$eintrag.F_Artikel_Nr},{$eintrag.DatumVonBis}]" 
    value="{/if}{$eintrag.MengeAnzeigen}{if $Edit && $eintrag.MengeAnzeigen == $eintrag.Mengen.0}" size="2" maxlength="5" />{/if}
    {if $Aenderbar && ! $Edit && strpos($eintrag.MengeAnzeigen,',') === false }
        <a href="?id={$id}&Buchung_Nr={$smarty.request.Buchung_Nr}&Menge={$eintrag.F_Artikel_Nr}&Richtung=P&Datum={$eintrag.DatumVonBis}&Zuf={$eintrag.Zuf}#Einzelheiten">
        <img src="img/plus.gif" border="0" /></a>
        <a href="?id={$id}&Buchung_Nr={$smarty.request.Buchung_Nr}&Menge={$eintrag.F_Artikel_Nr}&Richtung=M&Datum={$eintrag.DatumVonBis}&Zuf={$eintrag.Zuf}#Einzelheiten">
        <img src="img/minus.gif" border="0" /></a>
    {/if}
  </td>
  <td align="right">
      {if $Edit && Count($eintrag.Dauern) == 1}
      <input type="Text" name="docinput[Dauer][{$eintrag.F_Artikel_Nr},{$eintrag.DatumVonBis}]" value="{/if}
      {if is_numeric($eintrag.DauerAnzeige)}
        {if $eintrag.DauerAnzeige != 0}{$eintrag.DauerAnzeige|string_format:"%01.2f"}{/if}
      {else}
      {$eintrag.DauerAnzeige}
      {/if}
      {if $Edit && Count($eintrag.Dauern) == 1}" size="2" maxlength="5" />{/if}
  </td>
  <td>{$eintrag.Einheit}</td>
  <td>
  {if $Edit}
    <input type="Text" name="docinput[Bezeichnung][{$eintrag.F_Artikel_Nr},{$eintrag.DatumVonBis}]" 
    value="{$eintrag.Bezeichnung}" size="40" maxlength="60" />
  {else}
    {if $eintrag.Beschreibung != ''}
      <span {popup text=$eintrag.Beschreibung|nl2br|escape caption=$eintrag.Bezeichnung|escape}>
    {/if}
    {$eintrag.Bezeichnung}
    {if $eintrag.Beschreibung != ''}</span>{/if}
  {/if}
  </td>
  <td>
    <a href="{$Buchungsuebersichturl}&Tag={$eintrag.Datum}&docinput[Art]={$eintrag.F_Art_id}&docinput[Page]={$eintrag.Master}" 
      title="zur Belegungsübersicht">{$eintrag.Datum|date_format:"%a"} {$eintrag.DatumAnzeige}</a>
      <br />
	<span id="Person{$nr}" class="mini">{if $eintrag.Zustaendig !=''}Zuständig: {$eintrag.Zustaendig}{else}
	  {if $eintrag.F_Art_id==CVJMART_PROGRAMM}<span class="Fehler">Zuständiger fehlt!</span>{/if}
	  {/if}</span>
  </td>
  <td class="zentriert">
     {$eintrag.Schlafplaetze}
  </td>
  <td>
  {if $Edit}
    <input type="checkbox" name="docinput[Unberechnet][{$eintrag.F_Artikel_Nr},{$eintrag.DatumVonBis}]" 
    title="Anklicken wenn der Artikel nicht an den Kunden berechnet werden soll"
    value="v" {if $eintrag.Unberechnet}checked="checked"{/if} />
  {else}
    {if $eintrag.Unberechnet}<span style="font-size:4pt;font-family:Verdana;color:red;">Unberechnet!</span>{/if}  
  {/if}
  {if $Aenderbar}
    <a href="#" title="zuständige Person festlegen" onClick="showPerson({$nr},{$eintrag.F_Artikel_Nr},'{$eintrag.DatumVonBis}',true); return false;" id="PersonLink{$nr}">
      <img src="/img/person.gif" border="0"/></a>
    {eval var=$nr+1 assign="nr"}
    <a href="?id={$id}&DelEintrag={$eintrag.F_Artikel_Nr},{$eintrag.DatumVonBis}&Buchung_Nr={$smarty.request.Buchung_Nr}{if isset($smarty.request.Alle) && $smarty.request.Alle == 1}&Alle=1{/if}#Einzelheiten"
    onClick="javascript:return window.confirm('Eintrag {$eintrag.Bezeichnung} {$eintrag.DatumAnzeige} wirklich löschen?');">
    <img src="/img/small_edit/delete.gif" border="0"/></a>
  {/if}
  </td>
</tr>
  {/foreach}

{/foreach}