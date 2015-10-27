{if $Speichern}
{literal}
<script language="javascript">
function loesche(datei, id)
{
  if ( window.confirm('Korrespondenz '+datei+' wirklich löschen?'))
{/literal}
    xajax_loescheKorrespondenz({$id}, {$Adresse->Adressen_id}, datei, id);
{literal}
}
function StandardDokumenteAnzeigen()
{
  var e = document.getElementsByName('docinput[KorrespondenzMail]');
  for(i=0;i<e.length;i++)
  {
    var p = e[i].parentNode.parentNode; // p - TR-Element
    if ( e[i].parentNode.title == "Standard" )
    {
      if ( p.style.display == "none" )
      {
        p.style.display = "";
		
      }
      else if ( !e[i].checked )
      {
        p.style.display = "none";
      }
    }
  }  
  var a = document.getElementById("Korr_StdDok");
  if ( a.innerHTML != "Standarddokumente anzeigen" )
      a.innerHTML = "Standarddokumente anzeigen";
    else
      a.innerHTML = "Standarddokumente verbergen";
}
function zeigeMail()
{
  var a = document.getElementById("mailzeile");
  var b = document.getElementById("MailHinweis");
  if ( a.style.display=="none" )
  {
      a.style.display="";
  	  b.innerHTML = "Mail verstecken";
  }
  else
  {
      a.style.display="none";
      b.innerHTML = "Mail vorbereiten";
  }
  
}
function sendeMail()
{  
  var dateien = new Array();
  var e = document.getElementsByName('docinput[KorrespondenzMail]');
  for(i=0;i<e.length;i++)
  {
    if ( e[i].checked )
    {
      dateien.push(e[i].value);
    }
  }  
  var senden = false;
  if ( dateien.length == 0 )
  { 
    if ( window.confirm('Es wurden keine Korrespondenzen ausgewählt. Wollen Sie trotzdem eine Mail schicken?'))
	    senden = true;
  }
  else
    senden = true;
  if ( senden )
  { 
    var betreff = document.getElementById('MailBetreff').value;
    var mail = document.getElementById('mail').value;
    var text = document.getElementById('MailText').value;
    if ( dateien.length != 0 )
    {
      if ( window.confirm('Wollen Sie die markierten Dateien wirklich versenden?'))   
{/literal}
      xajax_sendeKorrespondenzMail({$Adresse->Adressen_id},'{$Dateien.0.id}', betreff, text, dateien, mail);
{literal}
    }
    else
{/literal}
      xajax_sendeKorrespondenzMail({$Adresse->Adressen_id},'', betreff, text, dateien, mail);
{literal}    
  }  
}
</script>
{/literal}
{/if}
<table class="volleTabelle">
<tr class="ueberschrift">
  <th colspan="3" class="zentriert">
    <a id="Korrespondenz" name="Korrespondenz"></a>
    <div class="rechteSeite">[ 
     <a href="{$Korrespondenzurl}&docinput[Adressen_id]={$Adresse->Adressen_id}">Neu</a> 
 ]</div>Korrespondenzen{if $Adresse->Kunden_Nr>0} KuNr {$Adresse->Kunden_Nr}{/if}
  </th>
</tr>
{foreach from=$Dateien item=datei}
<tr class="{cycle values="helleZeile,dunkleZeile"}" {if $datei.Art!=''}style="display:none;"{/if}>
  <td {if $datei.Art != ''}title="{$datei.Art}"{/if}>
    {if Speichern}<input type="checkbox" name="docinput[KorrespondenzMail]" 
    value="{$datei.Name|escape}" title="Anklicken zum Auswählen"/>{/if}
    {$datei.Zeit|date_format:"%d.%m.%Y %H:%M"}
    {if $datei.Art != ''} Standardvorlage{/if}
  </td>
  <td>
    <a href="/redirect.php?url={$datei.Link}" target="_blank">
      {if $datei.Extension=='pdf'}
        <img src="/img/icons_16/pdf.png" border="0" title="PDF-Datei" alt="PDF" />
      {else}
        <img src="/img/icons_16/OOo.gif" border="0" title="Openoffice-Datei" alt="OO" />
		{/if} {$datei.NurName} </a>
  </td>
  <td>
    {if $Speichern && $datei.Art == ''}
    <a href="{$Korrespondenzurl}&docinput[Datei]={$datei.Name}&docinput[Adressen_id]={$Adresse->Adressen_id}">Ersetzen</a> 
    	<img src="/img/small_edit/delete.gif" border="0"
			onClick="loesche('{$datei.Name|escape}', '{$datei.id}');" />
		<!--  ?id={$id}&{$params}&docinput[DelKorrespondenz]={$datei.NurName}#Korrespondenz" -->
    {/if}
  </td>
</tr>
{/foreach}
{if Count($Dateien)==0}
<tr>
  <td colspan="3" class="zentriert">- Keine Korrespondenz vorhanden - </td>
</tr>
{else}
  {if $Speichern}
  <tr id="mailzeile" style="display:none">
  <td>
    Empfänger-E-Mail <select id="mail">
    <option selected="selected">{$Adresse->Email}</option>
    {foreach from=$Adresse->getInstitutionen() item=Inst}
      {if $Inst->Email != ''} 
      <option>{$Inst->Email}</option>
      {/if}
    {/foreach}
    </select><br />
  Betreff: <input type="text" id="MailBetreff" len="50" /><br />
  <div class="mini">Dem Betreff wird Kundennummer bzw. Buchungsnummer automatisch vorangestellt.</div>
  </td>
  <td>
  Text der Mail:<br /><textarea rows="5" cols="40" id="MailText">
{$Adresse->Anredezeile()}

{include file='../Vorlagen/email_signatur.txt'}</textarea>
  </td>
  <td>
    <button onClick="javascript:sendeMail();">Mail senden</Button>
  </td>
  </tr>
  <tr>
    <td class="zentriert"> 
    [ <a id="Korr_StdDok" href="javascript:StandardDokumenteAnzeigen()">Standarddokumente anzeigen</a> ] 
    [ <a href="javascript:zeigeMail();" id="MailHinweis">Mail vorbereiten</a> ]
    </td>
  </tr>
  {/if}
{/if}
</table>