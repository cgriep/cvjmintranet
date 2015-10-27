{literal}
<style type="text/css">
#Dialog { 
 position: absolute;
 width: 500px;
 height: 450px;
 max-height:450px;
 overflow: auto;     
 -moz-border-radius: 4px;
 font-weight: bold;
 display: block;
 text-align: left;
 padding-left: 32px;
 padding-right: 8px;
 padding-top: 7px;
 padding-bottom: 7px;
 border: 1px solid #7f7f7f;
 background-color: #dedede;
 z-index: 2;
 margin: 5;
}
</style>
{/literal}
<div id="Dialog">
{$title}<br />
<button onClick="speichern(true);return false;">Speichern</button>
<button onClick="speichern(false);return false;">ohne Speichern schlieﬂen</button>
<form action="" method="post" id="Daten" onSend="return false">
Aufgabe/Beschreibung<br />
<textarea name="Beschreibung" cols="40" rows="5">{$Artikel->Bezeichnung}
{$Artikel->Beschreibung}</textarea><br />
<div>
Benutzer
<table>
 {foreach from=$users key=user_id item=user}
 {cycle values="<tr class=\"helleZeile\"><td>,<td>,<td>,<tr class=\"dunkleZeile>\"><td>,<td>,<td>" name="c1"}
   <input type="checkbox" name="User[{$user_id}]" {if in_array($user_id,$markiertU)} checked="checked"{/if}/></td>
   <td>{$user}
 {cycle values="</td>,<td>,</td></tr>" name="c2"}
 {/foreach}
</table>
</div>
<!-- Gruppen -->
<div>
Gruppen
<table>
{foreach from=$groups key=group_id item=group}
 {cycle values="<tr class=\"helleZeile\"><td>,<td>,<td>,<tr class=\"dunkleZeile>\"><td>,<td>,<td>" name="c3"}
   <input type="checkbox" name="Group[{$group_id}]" {if in_array($group_id,$markiertG)} checked="checked"{/if}/></td>
   <td>{$group}
 {cycle values="</td>,<td>,</td></tr>" name="c4"}
 {/foreach}
</table>
</div>
<!-- Externe -->
<div>
  Externe (einer pro Zeile)<br />
  <textarea name="Externe" cols="40" rows="5" >{$extern}</textarea>
</div> 
</form>
<button onClick="speichern(true);return false;">Speichern</button>
<button onClick="speichern(false);return false;">ohne Speichern schlieﬂen</button>

</div>
