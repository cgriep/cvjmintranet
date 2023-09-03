<style type="text/css">
@import url(css/popcalendar.css);
</style>
<script type="text/javascript" language="JavaScript" src="javascript/common.js"></script>
<script type="text/javascript" language="JavaScript" src="javascript/popcalendar.js"></script>

<form action="?id={$id}" name="NeuForm" method="post" class="noprint">
{if ( isset($docinput.Zeit) )}
<input type="hidden" name="docinput[ZID]" value="{$docinput.Zeit}" />
{/if}
<table>
<tr><th colspan="4">
{if ( ! isset($docinput.Zeit) )}
Neue Arbeitszeit
{else}
Arbeitszeit verändern
{/if}
</th></tr>
<tr>
  <td><label for="Datum">Datum</label> </td>
  <td><input type="Text" id="Datum" name="docinput[Datum]" size="12" maxlength="12" 
    onClick="popUpCalendar(this,NeuForm['docinput[Datum]'],'dd.mm.yyyy')"
    onBlur="autoCorrectDate('NeuForm','docinput[Datum]' , false )"
    accesskey="D" value="{$eintrag.Datum|date_format:"%d.%m.%Y"}" />
  </td>
  <td>Art</td><td><select name="docinput[Art]" size="1">
  {html_options options=$Arbeitsarten selected=$eintrag.Art}
  </select>
  </td>
</tr>
<tr>
  <td>Beginn</td>
  <td><input type="Text" name="docinput[Von]" size="5" maxlength="5" 
      value="{$eintrag.Datum|date_format:"%H:%M"}" /></td>
  <td>Ende</td>
  <td>
    <input type="Text" name="docinput[Bis]" size="5" maxlength="5" 
      value="{$eintrag.Bis|date_format:"%H:%M"}" />
  </td>
</tr>
<tr>
  <td>Bemerkung</td>
  <td colspan="3">
     <textarea name="docinput[Bemerkung]" cols="60" rows="5">{$eintrag.Bemerkung|escape}</textarea>
  </td>
</tr>
<tr>
  <td colspan="4" align="center"><input type="Submit" value="{if ( isset($docinput.Zeit) )}
Änderungen speichern" />&nbsp;&nbsp;[ <a href="?id={$id}">Neue Arbeitszeit</a> ]{else}
Arbeitszeit hinzufügen" />{/if}
  </td>
</tr>
<input type="hidden" name="docinput[Benutzer]" value="{$Benutzer}" />
</table>
<hr />
</form>