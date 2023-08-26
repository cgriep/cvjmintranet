<?php

/*
  Modul BuchungsbemerkungenPutzis
  Bemerkungen zu einer Buchung
  z.B. Zerbruch, Ausleihen etc.

  Parameter: docinput[Buchung_Nr] mit der Buchungsnummer für die eingegeben werden soll
*/


if ( $profile["editor"] != 1 )
{
  include(INC_PATH."misc/CVJM.inc");
  include(INC_PATH."misc/cvjmBuchungstools.inc");
  include(INC_PATH."misc/cvjmArtikeltools.inc");
  if ( isset($docinput["Bemerkung"]) && is_array($docinput["Bemerkung"]) )
  {
    foreach ($docinput["Bemerkung"] as $key => $value )
    {
      $erzeugt = false;
      if ( is_numeric($key) )
      {
        if ( trim($value) != "" )
        {
          if ( $docinput["Nr"][$key] != -1 && is_numeric($docinput["Nr"][$key]) )
          {
            // prüfen ob geändert
            $sql = "SELECT Hinweis FROM ".TABLE_BUCHUNGSBEMERKUNGEN." WHERE Bemerkung_id=".
              $docinput["Nr"][$key];
            $query = sql_query($sql);
            $row = sql_fetch_row($query);
            sql_free_result($query);
            if ( $row[0] != trim(addslashes($value)) )
            {
              $sql = "UPDATE ".TABLE_BUCHUNGSBEMERKUNGEN." SET Hinweis='".trim(addslashes($value)).
                "', Logtext=CONCAT('".date("d.m.Y H:i")." geändert ".
                get_user_nickname()."\n',Logtext) WHERE Bemerkung_id = ".$docinput["Nr"][$key];
              if ( ! sql_query($sql) ) echo sql_error();
              $erzeugt = true;
            }
          }
          else
          {
            // Neue Bemerkung anf�gen
            if ( !sql_query("INSERT INTO ".TABLE_BUCHUNGSBEMERKUNGEN." (F_Buchung_Nr,Bemerkung,Hinweis,Logtext) VALUES (".
              $key.",'".CVJMPUTZMITTEILUNG."','".
                addslashes($value)."','".date("d.m.Y H:i")." erstellt ".get_user_nickname()."')"))
              echo '<div class="SchwererFehler">&gt;&gt;&gt; Fehler beim Erzeugen der Bemerkung.</div>';
            $erzeugt = true;
          }
          echo '<strong>&gt;&gt;&gt; Bemerkung erzeugt.</strong><br />';
        }
        else if ( $docinput["Nr"][$key] != -1 )
        {
          // vorhandene Bemerkung löschen
          $sql = "UPDATE ".TABLE_BUCHUNGSBEMERKUNGEN." SET Hinweis=CONCAT(Hinweis,'\n-Eintrag gelöscht-')".
              ", Logtext=CONCAT('".date("d.m.Y H:i")." gelöscht ".
              get_user_nickname()."\n',Logtext) WHERE Bemerkung_id = ".$docinput["Nr"][$key];
          if ( ! sql_query($sql) ) echo sql_error();
        }
        if ( $erzeugt )
        {
          // Mail an Verwaltung schreiben (Gruppe 20)
          $qresult = sql_query("SELECT user_id FROM ".TABLE_USERDATA." WHERE name='".
            CVJM_BUCHUNGSGRUPPE."' AND value != '0'");
          if(sql_num_rows($qresult) > 0) {
            while($row = sql_fetch_row($qresult)) {
              send_message ($row[0], addslashes($value), SESSION_DBID, get_user_nickname(),
              '[Buchung '.$key.'] '.CVJMPUTZMITTEILUNG, 'email');
          }
          sql_free_result($qresult);
        }
      }

      }
    }
  }
  if ( $document['title'] != '') echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  {
    // Übersicht über die aktuellen Belegungen
    echo '<form action="'.get_url($id).'" method="post">';
    echo $design["table_begin"];
    echo '<tr class="ueberschrift"><th>Ankunft</th>'.
      '<th>Abfahrt</th><th>Name</th><th>Belegung/<br />Personen</th>'.
      "<th>Zerbruch/<br />Bemerkung</th></tr>\n";
    $query = sql_query("SELECT * FROM ".TABLE_BUCHUNGEN." INNER JOIN ".TABLE_ADRESSEN.
      " ON F_Adressen_id = Adressen_id WHERE Von <= ".strtotime("+".$document["Tage"]." day")." AND Bis >= ".
      strtotime("-".$document["Tage"]." day").
      " AND BStatus <> ".BUCHUNG_STORNIERTABGERECHNET." AND BStatus <> ".
      BUCHUNG_STORNIERT." AND BStatus <> ".BUCHUNG_GELOESCHT.
      " ORDER BY Bis");
    $Color = "helleZeile";
    while ( $buchung = sql_fetch_array($query) )
    {
      echo '<tr class="'.$Color.'">';
      /*
      echo '<td>';
      echo '<a href="'.get_url($id,"docinput[Buchung_Nr]=".$buchung["Buchung_Nr"]).'">';
      echo $buchung["Buchung_Nr"];
      echo '</a><br />';
      echo '<a href="'.get_url($id,"docinput[Buchung_Nr]=".$buchung["Buchung_Nr"]).
        '"><img src="img/small_edit/edit_small.gif" border="0" title="Bemerkung eingeben" />';
      echo "<br />\n";
      switch ( $buchung["BStatus"] )
      {
        case BUCHUNG_GELOESCHT: // gelöscht
          echo '<img src="img/CVJM/geloescht.gif" border="0" title="Vorreservierung gelöscht">';
          break;
        case 0:
          echo '<img src="img/CVJM/vorres.gif" border="0" title="Vorreservierung">';
          break;
        case 1:
          echo '<img src="img/CVJM/1nachfrage.gif" border="0" title="1. Nachfrage">';
          break;
        case BUCHUNG_VORRESERVIERUNG: // Vorreservierung
         echo '<img src="img/CVJM/2nachfrage.gif" border="0" title="2. Nachfrage">';
          break;
        case BUCHUNG_RESERVIERUNG:  // Reserviert
          echo '<img src="img/CVJM/reserviert.gif" border="0" title="Reserviert">';
          break;
        case BUCHUNG_FERTIG: // abgerechnet
          echo '<img src="img/CVJM/abgerechnet.gif" border="0" title="Abgerechnet">';
          break;
        case BUCHUNG_STORNIERT: // Storniert
          echo '<img src="img/CVJM/storno.gif" border="0" title="Storniert">';
      }
      echo '</a></td>';
      */
      echo '<td class="zentriert">'.$Wochentage[date("w",$buchung["Von"])]."<br />";
      echo date("d.m.Y",$buchung["Von"]);
      if ( $buchung["Ankunftszeit"] != 0 )
        echo " ".date("H:i",$buchung["Ankunftszeit"]);
      echo '</td><td class="zentriert">'.
        $Wochentage[date("w",$buchung["Bis"])];
      echo "<br />".
        date("d.m.Y",$buchung["Bis"]);
      if ( $buchung["Abfahrtszeit"] != 0 )
        echo " ".date("H:i",$buchung["Abfahrtszeit"]);
      echo '</td>';
      echo '<td>'.stripslashes($buchung["Name"]." ".$buchung["Vorname"])." (".
        $buchung["Ort"].")";
      if ( $buchung["Zusatz"] != "" )
        echo '<br />'.stripslashes($buchung["Zusatz"]);
      echo '<br />BN '.$buchung["Buchung_Nr"];
      echo "</td>\n";
      echo '<td>'.ZeigeArtikel(CVJMART_ORT, $buchung["Buchung_Nr"]);
      echo "<br />".Personenanzahl($buchung)." Personen";
      echo '</td>';
      echo '<td>';
      echo '<textarea name="docinput[Bemerkung]['.$buchung["Buchung_Nr"].
        ']" cols="40" rows="3">';
      // TODO: Vorhandene Bemerkung anzeigen
      $query2 = sql_query("SELECT * FROM ".TABLE_BUCHUNGSBEMERKUNGEN.
        " WHERE F_Buchung_Nr=".$buchung["Buchung_Nr"]." AND Bemerkung ='".CVJMPUTZMITTEILUNG."'");
      if ( $bem = sql_fetch_array($query2) )
      {
        echo stripslashes($bem["Hinweis"]);
      }
      else
        $bem["Bemerkung_id"] = -1;
      sql_free_result($query2);
      echo '</textarea>';
      echo '<input type="hidden" name="docinput[Nr]['.$buchung["Buchung_Nr"].
        ']" value="'.$bem["Bemerkung_id"].'" />';
      echo '</td>';
      echo "</tr>\n";
      if ( $Color == "helleZeile" )
        $Color = "dunkleZeile";
      else
        $Color = "helleZeile";
    }
    sql_free_result($query);
    echo '<tr><td colspan="5" class="zentriert">';
    echo '<input type="Submit" value="Speichern"></td></tr>';
    echo $design["table_end"];
    echo "</form>\n";
  }
}
else
{
  echo '<br /><br />';
  create_editor_input("Anzahl Tage +/- in der Anzeige","Tage", "text_input");
}

?>