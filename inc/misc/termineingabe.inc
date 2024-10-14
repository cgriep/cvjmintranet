
    <table width="100%">
    <form action="<?=get_url($id)?>" method="post" name="Termin">
    <tr><td>Terminbezeichnung</td><td><input type="Text" name="docinput[title]"
    size="<?=$design['input_size']?>" value="<?=transform($document["title"],"clean")?>"/></td></tr>
    <tr><td>Bemerkungen</td><td><textarea name="docinput[body]" cols="<?=$design['input_size']?>" rows="<?=$design['input_rows']?>"><?=
     transform($document["body"],"clean")?></textarea></td></tr>
    <tr><td>Datum</td><td>
    <input type="Text" name="docinput[Datum]" value="<?=date("d.m.Y",$document["Datum"])?>"
      size="10" maxlength="10" <?=KalenderEvents("Termin","docinput[Datum]")?> />

    <input type="Text" name="docinput[Stunde]" value="<?=date("H",$document["Datum"])?>" size="2" maxlength="2" />
    <input type="Text" name="docinput[Minute]" value="<?=date("i",$document["Datum"])?>" size="2" maxlength="2" />
    </td></tr>
    <tr><td>Ende</td><td>
    <input type="Text" name="docinput[EDatum]" value="<?php
      if ( $document["Enddatum"] > 0 ) echo date("d.m.Y",$document["Enddatum"]); ?>"
      size="10" maxlength="10" <?=KalenderEvents("Termin","docinput[EDatum]")?> />
    <input type="Text" name="docinput[EStunde]" value="<?=date("H",$document["Enddatum"])?>" size="2" maxlength="2" />
    <input type="Text" name="docinput[EMinute]" value="<?=date("i",$document["Enddatum"])?>" size="2" maxlength="2" />
    Turnus <select name="docinput[Turnus]" size="1">
    <option value="" <?php if ($document["Turnus"] == "" ) echo ' selected="selected"'; ?>
    >-einmalig-</option>
    <option value="d" <?php if ($document["Turnus"] == "d" ) echo ' selected="selected"'; ?>
    >täglich</option>
    <option value="w" <?php if ($document["Turnus"] == "w" ) echo ' selected="selected"'; ?>
    >wöchentlich</option>
    <option value="v" <?php if ($document["Turnus"] == "v" ) echo ' selected="selected"'; ?>
    >14tägig</option>
    <option value="m" <?php if ($document["Turnus"] == "m" ) echo ' selected="selected"'; ?>
    >monatlich</option>
    <option value="j" <?php if ($document["Turnus"] == "j" ) echo ' selected="selected"'; ?>
    >jährlich</option>
    </select>&nbsp;&nbsp;Ende
    <input type="Text" name="docinput[TurnusDatum]" value="<?php
      if ( $document["TurnusEnde"] != 0 ) echo date("d.m.Y",$document["TurnusEnde"]);?>"
      size="10" maxlength="10" <?=KalenderEvents("Termin","docinput[TurnusDatum]")?> />
    </tr>
    <tr><td>Betroffene<br /><small>Freilassen wenn eigener Termin<br />
    Strg halten, um mehrere zu markieren</small></td>
    <td><select name="docinput[Person][]" size="5" multiple="multiple">
    <?php
    $User = search_users();
    asort($User);
    reset($User);
    foreach ( $User as $key => $value) 
    {
      echo '<option value="'.$key.'"';
      if ( in_array($key, explode(",",$document["Person"])) )
        echo ' selected="selected"';
      echo '>'.$value['nickname'].'</option>';
    }
    ?>
    </select>&nbsp;&nbsp;kein Einzelbetroffener <input type="Checkbox" name="DelBet" value="v"
    <?php if ($document["Person"] == "" && ! isset($_REQUEST["Termin"]) ) echo ' checked="checked"'; ?>
    >
    </td></tr>
    <tr><td>Gruppe<br /><small>Strg halten, um mehrere zu markieren</small></td><td>
    <select name="docinput[Gruppe][]" size="5" multiple="multiple">
    <option value="-1"<?php 
  if ( in_array(-1, explode(",",$document["Gruppe"])))
      echo ' selected="selected"'; 
?>>--alle--</option>
<?php
      if  (! isset($document['Terminrecht']))
      {
        // Terminrecht aus uebergeordnetem Dokument (Kalender) holen 
        $doc = get_nodedata($document['parent_id'],$flavour);
        if ( isset($doc['Terminrecht']))
           $document['Terminrecht'] = $doc['Terminrecht'];
      }
      foreach ($groups as $key => $gruppe) {
        // Nur Gruppenmitglieder können die Gruppe einladen!
        if ( InGruppe($key) || $profile[$document["Terminrecht"]] == "1" )
        {
          echo '<option value="' . $key . '"';
          if ( in_array($key, explode(",",$document["Gruppe"])) )
            echo ' selected="selected"';
          echo '>';
          echo stripslashes($gruppe) . '</option>';
        }
      }
    ?>
    </select>&nbsp;&nbsp;keine Gruppe <input type="Checkbox" name="DelGrp" value="v"
    <?php if ($document["Gruppe"] == "" && ! isset($_REQUEST["Termin"])) echo ' checked="checked"'; ?>
    ></td></tr>
    <tr><td>Ort</td><td>
    <?php
     if ( function_exists("OrtsName") ) {
       echo '<input type="hidden" name="docinput[Ort]" value="'.$Ort.'" id="Ortsauswahl" />';
       echo '<span name="Ortname" id="Ortsauswahltext">'.OrtsName($Ort,true).'</span>';
       echo '&nbsp;&nbsp;[ <a href="javaScript:OrtsAuswahl(\'Ortsauswahl\')">Ort auswählen</a> ]';
      }else
      echo '<input type="Text" name="docinput[Ort]" size="'.$design['input_size'].
        '" value="'.$document["Ort"].'">';
     if ( isset($_REQUEST["Anzeige"]) ) {
      echo '<input type="hidden" name="Anzeige" value="'.$_REQUEST["Anzeige"].'" />';
      echo '<input type="hidden" name="Datum" value="'.$_REQUEST["Datum"].'" />';
      /*
      echo '<input type="hidden" name="Tag" value="'.$_REQUEST["Tag"].'">';
      echo '<input type="hidden" name="Monat" value="'.$_REQUEST["Monat"].'">';
      echo '<input type="hidden" name="Jahr" value="'.$_REQUEST["Jahr"].'">';
      */
    }
    ?>
    </td></tr>
    <tr><td>Betroffene dürfen Termin ändern</td><td>
    <input type="radio" value="1" name="docinput[Aendern]"
<?php if ( $document["Aendern"] == 1 ) echo ' checked="checked"';
?> /> <?=LANG_YES?>&nbsp;&nbsp;
    <input type="radio" value="0" name="docinput[Aendern]" <?php
if ( $document["Aendern"] == 0 ) echo ' checked="checked"'; ?> /> <?=LANG_NO?>
    </td></tr>
    <tr><td colspan="2"><input type="Submit" name="Savetermin" value="Termin speichern"></td></tr>
    </form>
    </table>
