<?php
/*
   cvjmEintragZusammenfassung
   Zeigt BuchungsEinträge zusammengefasst an
   Wahlweise mit oder ohne Preis

   Parameter:
   Edit - wenn gesetzt, mit Editierungsmöglichkeit
   Buchung_Nr - Buchungsnummer
   Alle - wenn gesetzt, wird jeder Eintrag einzeln angezeigt

*/
function SortierteZusammenfassungAnzeigen($eintraege, $abgerechnet)
{
  global $id;
  global $Wochentage;
  global $Artikelarten;
  global $document;
  global $profile;
  $Color = 'helleZeile';
  foreach ( $eintraege as $meinkey => $meinvalue )
  {
     $daten = array();
     foreach ( $meinvalue as $meinkey2 => $eintrag )
       $daten[$meinkey2] = $eintrag['Datum'];
     reset($meinvalue);
     array_multisort($daten, SORT_ASC, SORT_NUMERIC, $meinvalue);
     // überschrift
     echo '<tr><td colspan="3"></td><td colspan="3"><strong>'.$Artikelarten[$meinkey].
        '</strong></td></tr>';
     // Sortieren nach Datum
     //array_multisort($eintrag["Datum"], SORT_ASC, SORT_NUMERIC);
  foreach ( $meinvalue as $meinkey2 => $eintrag )
  {
    if ( isset($eintrag['LetztesDatum']) )
    {
      $datum = date('d.m.Y',$eintrag['Datum']);
      if ( date('H:i',$eintrag['Datum']) != '00:00' )
        $datum .= date(' H:i',$eintrag['Datum']);
      $eintrag['LetztesDatum'] = strtotime('-1 day', $eintrag['LetztesDatum']);
      $datumvonbis = $eintrag['Datum'];
      if ( $eintrag['LetztesDatum'] > $eintrag['Datum'] )
      {
        $datum .= ' bis '.substr($Wochentage[date('w',$eintrag['LetztesDatum'])],0,2).' '.
          date('d.m.Y', $eintrag['LetztesDatum']);
        $datumvonbis = $eintrag['Datum'].'-'.$eintrag['LetztesDatum'];
      }
      $wieviel = 0;
      $Anzeigen = $eintrag['Mengen'][0];
      foreach ($eintrag['Mengen'] as $key => $value  )
      {
        $wieviel += $value;
        if ( $Anzeigen != $value ) $Anzeigen = '';
      }
      if ( $Anzeigen != $eintrag['Mengen'][0] )
        $Anzeigen = implode(',',$eintrag['Mengen']);
      echo '<tr class="'.$Color.'"><td>';
      echo $wieviel;
      echo "</td>\n";
      echo '<td valign="right">';
      // wenn die Menge an allen Tagen gleich ist
      if ( isset($_REQUEST['Edit']) && $Anzeigen == $eintrag['Mengen'][0] )
        echo '<input type="Text" name="docinput[Menge]['.$eintrag['F_Artikel_Nr'].
          ','.$datumvonbis.']" value="';
      echo $Anzeigen;
      if ( isset($_REQUEST['Edit']) && $Anzeigen == $eintrag['Mengen'][0] )
        echo '" size="2" maxlength="5" />';
      if (/**/ !$abgerechnet && strpos($Anzeigen,',') === false &&
           $profile[$document['SpeicherRecht']] == '1' && ! isset($_REQUEST['Edit']) )
      {
        echo '<a href="'.get_url($id,array('Buchung_Nr'=>$_REQUEST['Buchung_Nr'],
        /* docinput[Rechnung_id]*/
          'Menge'=>$eintrag['F_Artikel_Nr'],'Richtung'=>'+',
          'Datum'=>$datumvonbis,'Zuf'=>rand(0,100))).
          '#Einzelheiten"><img src="img/plus.gif" border="0" /></a>';
        echo '<a href="'.get_url($id,array('Buchung_Nr'=>$_REQUEST['Buchung_Nr'],  /**/
          'Menge'=>$eintrag['F_Artikel_Nr'],'Richtung'=>'-',
          'Datum'=>$datumvonbis,'Zuf'=>rand(0,100))).
          '#Einzelheiten"><img src="img/minus.gif" border="0" /></a>';
      }
      echo "</td>\n";
      echo '<td align="right">';
      $wieviel = 0;
      $Anzeigen = $eintrag['Dauern'][0];
      foreach ( $eintrag["Dauern"] as $key => $value) 
      {
        $wieviel += $value;
        if ( $Anzeigen != $value ) $Anzeigen = '';
      }
      if ( $Anzeigen != $eintrag['Dauern'][0] )
        $Anzeigen = implode(',',$eintrag['Dauern']);
      if ( isset($_REQUEST['Edit']) && Count($eintrag['Dauern']) == 1 )
        echo '<input type="Text" name="docinput[Dauer]['.$eintrag["F_Artikel_Nr"].
          ",".$datumvonbis.']" value="';
      if ( is_numeric($Anzeigen) )
      {
        if ( $Anzeigen != 0 )
          echo sprintf('%01.2f',$Anzeigen);
      }
      else
        echo $Anzeigen;
      if ( isset($_REQUEST['Edit']) && Count($eintrag['Dauern']) == 1 )
        echo '" size="2" maxlength="5" />' /* title="Einzelpreis/Stunde: '.
        holePreis($eintrag["F_Artikel_Nr"], $buchung["F_Preisliste_id"], true).'"/>'; */;
      echo "</td>\n";
/*
    if ( isset($_REQUEST["Edit"]) )
      echo '<input type="Text" name="docinput[Einheit]['.$eintrag["F_Artikel_Nr"].
        ",".$datumvonbis.']" value="'.stripslashes($eintrag["Einheit"]).
          '" size="10" maxlength="20" />';
    else
      echo stripslashes($eintrag["Einheit"]);
    echo "</td>\n<td>";
*/
      echo '<td>'.$eintrag['Einheit']."</td>\n";
      echo '<td>';
      if ( $eintrag['F_Bezeichnung'] != $eintrag['Bezeichnung'] &&
        $eintrag['F_Bezeichnung'] != '')
          $bez = stripslashes($eintrag['F_Bezeichnung']);
      else
          $bez = stripslashes($eintrag['Bezeichnung']);
      /* Rechnung : nur F_Bezeichnung! */
      if ( isset($_REQUEST['Edit']) )
        echo '<input type="Text" name="docinput[Bezeichnung]['.$eintrag["F_Artikel_Nr"].
          ",".$datumvonbis.']" value="'.$bez.'" size="40" maxlength="60" />';
      else
      {
       /* $b = holeBeschreibung($eintrag["F_Artikel_Nr"]);  */
        if ( $eintrag['Beschreibung'] != '' )
        {
          echo '<span onMouseOver="return overlib(\''.
                htmlentities($eintrag["Beschreibung"])."',CAPTION,'Artikel ".
                $bez."');";
          echo '" onMouseOut="return nd();" >';
        }
        if ( $eintrag['F_Bezeichnung'] != $eintrag['Bezeichnung'] &&
          $eintrag['F_Bezeichnung'] != '')
          echo '<em>'.$bez.'</em>';
        else
          echo $bez;
        if ( $eintrag['Beschreibung'] != '' ) echo '</span>';
      }
      echo "</td>\n";
      echo '<td><a href="'.get_url(get_cmodule_id("cvjmbuchungsuebersicht"), array(
        "Buchung_Nr"=>$_REQUEST["Buchung_Nr"],"Tag"=>$eintrag["Datum"],
        "docinput[Art]"=>$eintrag["F_Art_id"],
        "docinput[Page]"=>holeArtikelMasterParent($eintrag["F_Art_id"],
          $eintrag["Position"], $eintrag["Einruecken"])) ).
        '" title="zur BelegungsÜbersicht">';
      echo substr($Wochentage[date("w",$eintrag["Datum"])],0,2)." ";
      // TODO: Datum editierbar machen ****
      echo $datum."</a></td>\n";
      echo '<td class="zentriert">';
      $b = BerechneSchlafplaetze($eintrag["F_Artikel_Nr"]);
      if ( $b >= 0 ) echo $b;
      echo "</td>\n";
      $del = $eintrag["F_Artikel_Nr"].",".$datumvonbis;
      echo '<td>';
      if ( ! $abgerechnet && $profile[$document["SpeicherRecht"]] == "1")
      {
        if ( isset($_REQUEST["Alle"]) && $_REQUEST["Alle"] == 1 )
          echo '<a href="'.get_url($id,array("DelEintrag"=>$del,
            "Buchung_Nr"=>$_REQUEST["Buchung_Nr"],"Alle"=>1));
        else
          echo '<a href="'.get_url($id,array("DelEintrag"=>$del,
            "Buchung_Nr"=>$_REQUEST["Buchung_Nr"]));
        echo '#Einzelheiten" onClick="javascript:return window.confirm(\'Eintrag '.
          stripslashes($eintrag["Bezeichnung"]).
          " ".$datum;
        echo ' wirklich löschen?\');"><img src="/img/small_edit/delete.gif" border="0"/></a>';
      }
      echo "</td></tr>\n";
      if ( $Color == "helleZeile" )
        $Color = "dunkleZeile";
      else
        $Color = "helleZeile";

    }
  } // foreach
  }// foreach
}



?>