<form action="?id={$id}" method="post">
<label for="Datum">Datum</label> <input id="Datum" type="Text" name="docinput[Datum]" value="{$smarty.now|date_format:"%d.%m.%Y"}" />
<table>
<tr class="ueberschrift">
  <th>Gr&ouml;&szlig;e</th><th>Alter Wert</th><th>Z&auml;hlerstand</th>
</tr>
{foreach from=$groessen item=groesse}
<tr>
  <td>
    {$groesse.Bezeichnung}
  </td>
  <td>
  {foreach from=$groesse.werte item=wert}
     {$wert|string_format:"%0.2f"} ({$groesse.Dat|date_format:"%d.%m.%Y"})
     <input type="hidden" name="docinput[AltWert][{$groesse.Groesse_id}]" value="{$wert}" />
  {/foreach}
  {if Count($groesse.werte) == 0}-n/v-{/if}
  </td>
  <td>
    <input type="Text" name="docinput[Wert][{$groesse.Groesse_id}]" value="" size="8" maxlength=""/>
    {$groesse.Ableseeinheit}
  </td>
</tr>
{/foreach}
</table>
<input type="Submit" name="SaveValues" value="Speichern" />
</form>