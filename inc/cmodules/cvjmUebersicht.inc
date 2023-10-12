<style type="text/css">
<!--
.RH {
   border-style:solid;
   border-color:<?=$design["table_row_bright"]?>;
   border-width:1pt;
   margin:0pt;
 }
.RD {
   border-style:solid;
   border-color:<?=$design["table_row_dark"]?>;
   border-width:1pt;
   margin:0pt;
 }
-->
</style>
<?php
/*
Modul Übersicht

AWF-Name: cvjmgesamtuebersicht

Parameter:

*/

if ( $profile["editor"] != 1 ) {
// Anzahl Tage die angezeigt werden
if( isset($_REQUEST["Tage"]) && is_numeric($_REQUEST["Tage"]) && $_REQUEST["Tage"] >= 10)
  define("ANZAHLTAGE", $_REQUEST["Tage"]);
else
  define("ANZAHLTAGE", 10);

include(INC_PATH."misc/CVJM.inc");
include(INC_PATH."misc/Kalenderjs.inc");
include(INC_PATH.'cvjm/Artikel.class.php');

$Anzahl = 0;

function ZeigeBuchung($diebuchung, $datum, & $aktiv, & $Wann)
{
  global $params;
  global $design;
  global $id;
  global $Buchungszahl;
  global $BuchungFarbe;
  global $Belegungsfarbe; // aus CVJM.inc
  if ( ! isset($diebuchung["Datum"]) ) return "";
  $Zeile = "";
  if ( mktime(0,0,0,date("m", $diebuchung["Datum"]), date("d", $diebuchung["Datum"]),
    date("Y", $diebuchung["Datum"])) == mktime(0,0,0,date("m",$datum),date("d",$datum),
    date("Y",$datum)) )
  {
    // Durchnummerierung der Buchung für Farben
    $Buchungszahl = $diebuchung["F_Buchung_Nr"];
    if ( ! isset($BuchungFarbe[$Buchungszahl]) && Count($BuchungFarbe) < Count($Belegungsfarbe))
    {
      if ( $Buchungszahl != $_REQUEST["Buchung_Nr"] )
        $BuchungFarbe[$Buchungszahl] = $Belegungsfarbe[Count($BuchungFarbe)];
      else
        $BuchungFarbe[$Buchungszahl] = $design["table_header"];
    }
    if ( isset($_REQUEST["Buchung_Nr"]) && $diebuchung["F_Buchung_Nr"] == $_REQUEST["Buchung_Nr"] )
    {
      if ( is_array($aktiv) || $aktiv >= 0 )
      {
        if ( ! is_array($aktiv) )
        {
          $menge = $aktiv;
          $w = $Wann;
          $aktiv = array();
          $Wann = array();
          // Feld erzeugen, weil mehrere Einträge an einem Tag vorliegen
          $aktiv[$w] = $menge; // bisherigen Eintrag merken
          $Wann[$w] = $w;
        }
        // Neuer Eintag
        $Wann[$diebuchung["Datum"]] = $diebuchung["Datum"];
        $aktiv[$diebuchung["Datum"]] = $diebuchung["Menge"];
      }
      else
      {
        $aktiv = $diebuchung["Menge"];
        $Wann = $diebuchung["Datum"];
      }
    }
    else
    {
      $Zeile = '<tr><td bgcolor="'; // Neu
      $Zeile .= $BuchungFarbe[$diebuchung["F_Buchung_Nr"]]; // Neu
      $Zeile .= '" align="center">'; // Neu
      $Zeile .= '<a href="'.get_url($id,array_merge($params,
        array("Buchung_Nr"=>$diebuchung["F_Buchung_Nr"]))).'" title="';
      $Zeile .= $diebuchung["Name"]." ".$diebuchung["Zusatz"]." (".
                $diebuchung["Ort"].")";
      if ( $diebuchung["BStatus"] <= BUCHUNG_VORRESERVIERUNG )
        $Zeile .= " / Vorreservierung !";
      $Zeile .= '">['.$diebuchung["F_Buchung_Nr"]."]";
      if ( $diebuchung["Menge"] > 1 )
        $Zeile .= ' '.$diebuchung["Menge"];
      $Zeile .= "</a></td>"; // Neu
    }
  }
  return $Zeile;
}

// dieArt  = CVJMART_ORT
function ArtikelAnzeigen($DieArt, $Tag, $parent = -1)
{
  global $Schlafplaetze;
  global $docinput;
  global $Anzahl;
  global $design;
  global $id;
  global $buchung;
  global $params;
  global $Alle;
  global $Buchungszahl;
  global $BuchungFarbe;
  $Wann = array();
  if ( ! is_numeric($parent) ) return 0;
  $query = sql_query("SELECT * FROM ".TABLE_ARTIKEL." WHERE id=".$parent);
  if ( ! $row = sql_fetch_array($query) ) echo "Fehler p=$parent: ".sql_error();
  sql_free_result($query);
  $parentPos = $row["Position"];
  $nextPos = findeArtikelPos($row, "r");
  // Zeigt alle Orte als Baum an
  $sql = "SELECT id, Bezeichnung, Anzeigen, Einruecken FROM ".TABLE_ARTIKEL.
    " WHERE Position BETWEEN ".($parentPos+1)." AND ".($nextPos-1)." AND F_Art_id = ".
    $DieArt." AND Anzeigen ORDER BY Position"; // TODO: $Alle wieder rein
  $qu = sql_query($sql);
  $Ebenen = array();
  $LetztesEinruecken = -1;
  while ( $row = sql_fetch_array($qu) )
  {
      if ( $row["Einruecken"] > $LetztesEinruecken+1 )
        for ($i = $LetztesEinruecken; $i < Count($Ebenen); $i++)
          $Ebenen[$i] = "";

      $Ebenen[$row["Einruecken"]] = $row["Bezeichnung"];
      $LetztesEinruecken = $row["Einruecken"];
    //if ( $row["Anzeigen"] )
    //{
      echo '<tr bgcolor="';
      if ( ($Anzahl % 2) == 0 ) echo $design["table_row_bright"];
        else echo $design["table_row_dark"];
      echo '"><td valign="top">';
      //for ( $i = 1; $i < $Ebene; $i++ )
      //  echo "&gt;";
      // Parameter heißt Ort!
      echo '<span style="width:'.($row["Einruecken"]*10).'px;float:left;">&nbsp;</span>';
      echo '<a href="'.get_url(get_cmodule_id("cvjmartikel"),'docinput[Artikel_Nr]='.$row["id"]).'#Edit">';
      if ( $row["Einruecken"] > 0 && $Ebenen[$row["Einruecken"]-1] != "" )
        echo $Ebenen[$row["Einruecken"]-1]." > ";
      echo $row["Bezeichnung"];
      //echo $row["Bezeichnung"].'</a>';
      echo '</td>';
      $docinput["Artikelnummern"] .= $row["id"].",";
      echo '<td valign="top">';
      if ( $buchung["Buchung_Nr"] > 0 )
      {
        $sql = "SELECT Count(*) FROM ".TABLE_BUCHUNGSEINTRAEGE.
            " WHERE F_Artikel_Nr=".$row["id"]." AND F_Buchung_Nr = ".$buchung["Buchung_Nr"];
        $query = sql_query($sql);
        $anzahl = sql_fetch_row($query);
        sql_free_result($query);
        echo '<input type="Checkbox" title="für den gesamten Aufenthalt" value="v" name="';
        if ( $anzahl[0] == ($buchung["Bis"] - $buchung["Von"])/86400+1)
          echo 'Reise" checked="checked" disabled="disabled';
        else
          echo 'Artikel['.$row["id"].'][Reise]';
        echo '" /> ';
      }
      echo '</td>';
      // Tage mit Belegung anzeigen
      $sql = "SELECT F_Buchung_Nr, Menge, Datum, Name, Zusatz, Ort, BStatus FROM ".TABLE_BUCHUNGSEINTRAEGE.
        " INNER JOIN ".TABLE_BUCHUNGEN." ON F_Buchung_Nr = Buchung_Nr INNER JOIN ".TABLE_ADRESSEN.
        " ON F_Kunden_Nr = Kunden_Nr WHERE F_Artikel_Nr = ".$row["id"].
          " AND Datum BETWEEN $Tag AND ".strtotime("+".(ANZAHLTAGE-1)." day",$Tag)." ORDER BY Datum, Eingang";
      $buchungen = sql_query($sql);
      $diebuchung["Datum"] = 0;
      for ( $i= 0; $i < ANZAHLTAGE; $i++ )
      {
        $zeileAnf = '<td align="center" class="';
        if ( ($Anzahl % 2) == 0 )
          $zeileAnf .= "RD";
        else
          $zeileAnf .= "RH";
        //$zeile = '">&nbsp;';
        $zeileAnf .= '" valign="top"><table width="100%" cellpadding="0" cellspacing="0">';
        $zeile = "";
        $Buchungszahl = 0;
        $aktiv = -1;
        $datum = strtotime("+$i day", $Tag);
        if ( mktime(0,0,0,date("m",$diebuchung["Datum"]),date("d",$diebuchung["Datum"]),
          date("Y",$diebuchung["Datum"]))<= $datum )
        {
          if ( mktime(0,0,0,date("m",$diebuchung["Datum"]),date("d",$diebuchung["Datum"]),
            date("Y",$diebuchung["Datum"])) == mktime(0,0,0,date("m",$datum),date("d",$datum),
            date("Y",$datum)) )
          {
            $zeile .= ZeigeBuchung($diebuchung,$datum, $aktiv, $Wann);
          }
          while ( mktime(0,0,0,date("m",$diebuchung["Datum"]),date("d",$diebuchung["Datum"]),
            date("Y",$diebuchung["Datum"])) <= mktime(0,0,0,date("m",$datum),date("d",$datum),
              date("Y",$datum)) && isset($buchungen))
          {
            if ( isset($buchungen) )
              if ( ! $diebuchung = sql_fetch_array($buchungen))
              {
                sql_free_result($buchungen);
                unset($buchungen);
                unset($diebuchung);
              }
            $zeile .= ZeigeBuchung($diebuchung, $datum, $aktiv, $Wann);
          } // solange kleiner Datum
        } // Wenn Buchungsdatum kleiner Datum
        else
        {
          $zeile .= ZeigeBuchung($diebuchung, $datum, $aktiv, $Wann);
        }
        echo $zeileAnf;
        if ( $_REQUEST["Buchung_Nr"] >= 0 )
        {
          echo '<tr><td ';
          if ( is_array($aktiv) || $aktiv >= 0 ) echo 'bgcolor="'.$BuchungFarbe[$buchung["Buchung_Nr"]].'"';
          if ( is_array($aktiv) || $aktiv > 0)
            echo ' style="border-color:#008000;border-size:1pt;border-style:solid;"';
          echo '>'; // Neu
          if ( $docinput["Art"] != CVJMART_VERLEIH &&
               $docinput["Art"] != CVJMART_PROGRAMM )
          {
            // alles was nicht Zeitabhängig ist
            echo '<input type="Checkbox" name="Artikel['.$row["id"].']['.$datum.']" value="v" ';
            if ( $aktiv >= 0 )
            {
              echo ' checked="checked"';
              if ( $docinput["Art"] == CVJMART_ORT )
              {
                $a = new Artikel($row['id']);
              	$Schlafplaetze[$i] += $a->BerechneSchlafplaetze();
              }
            }
            echo ' /> ';
          }
          else
          {
            // Programm und Verleih - mit Zeit
            if ( is_array($aktiv) )
            {
              foreach ( $aktiv as $key => $value )
              {
                echo '<input type="text" name="Artikel['.$row["id"].']['.$key.
                ']" size="2" maxlength="5" value="';
                if ( $value >= 0 )
                {
                  echo date("H:i",$Wann[$key]);
                }
                echo '" /> ';
                if ( $value >= 0 ) echo $value;
                echo '<br />';
              }
            }
            else
            {
              echo '<input type="text" name="Artikel['.$row["id"].']['.$datum.
                ']" size="2" maxlength="5" value="';
              if ( $aktiv >= 0 )
              {
                echo date("H:i",$Wann);
              }
              echo '" /> ';
            }
          }
          if ( ! is_array($aktiv) && $aktiv >= 0 )
            echo $aktiv;
          echo '</td></tr>'; // Neu
        }
        else
          if ( $zeile == "" )
            echo '<tr><td>&nbsp;</td></tr>'; // damit Rahmen angezeigt wird.
        echo $zeile;
        echo '</table>'; // Neu
        echo '</td>';
      }
      echo '</tr>';
      $Anzahl++;
    //} // wenn Anzeigen
    //ArtikelAnzeigen($DieArt, $Tag, $row["id"], $Ebene+1);
  }
  sql_free_result($qu);
}

    if ( $document['title'] != '')
    {
      echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
    }
    if ( isset($_REQUEST["Tag"]) && is_numeric($_REQUEST["Tag"]))
      $Tag = $_REQUEST["Tag"];
    if ( isset($_REQUEST["Datum"]))
    {
      $Tag = convertDatumToTimestamp($_REQUEST["Datum"]);
      $dat = "&Datum=".$_REQUEST["Datum"];
    }
    else
      $dat = "";
    echo '<div align="center">[ <a href="'.get_url(get_cmodule_id("cvjmbuchung")).'">Buchungsliste</a> ]';
    echo ' [ <a href="'.get_url(get_cmodule_id("cvjmbuchungsuebersicht"),
      "docinput[Art]=".CVJMART_ORT.$dat).'">Belegung</a> ]';
    echo ' [ <a href="'.get_url(get_cmodule_id("cvjmbuchungsuebersicht"),
      "docinput[Art]=".CVJMART_PROGRAMMPAKET.$dat).'">Programmpaket</a> ]';
    echo ' [ <a href="'.get_url(get_cmodule_id("cvjmbuchungsuebersicht"),
      "docinput[Art]=".CVJMART_PROGRAMM.$dat).'">Programmmodul</a> ]';
    if ( Personenanzahl($buchung) > 0 )
      echo ' [ <a href="'.get_url(get_cmodule_id("cvjmbuchungsuebersicht"),
        "docinput[Art]=".CVJMART_VERPFLEGUNG.$dat).'">Verpflegung</a> ]';
    echo ' [ <a href="'.get_url(get_cmodule_id("cvjmbuchungsuebersicht"),
        "docinput[Art]=".CVJMART_VERLEIH.$dat).'">Verleih</a> ]';
    echo '</div>';

    $buchung["Buchung_Nr"] = -1;
    // Datumsauswahl angeben
    KalenderScriptEinfuegen();

    echo '<form action="'.get_url($id).'" method="post" name="Wahl">';
    echo '<label for="Datum">Belegung ab Datum</label>';
    echo '<input type="hidden" name="docinput[Art]" value="'.$docinput["Art"].'" />';
    echo ' <input type="Text" name="Datum" value="'.date("d.m.Y",$Tag).
      '" size="10" maxlength="10"';
    echo KalenderEvents("Wahl", "Datum");
    echo '>';
    echo ' <input type="Submit" value="anzeigen">';
    echo '</form>';
  }
  // Reiter aufbauen
  if ( isset($_REQUEST["Tag"]) && is_numeric($_REQUEST["Tag"]))
    $Tag = $_REQUEST["Tag"];
  $params = array("docinput[Art]"=>$docinput["Art"],"Tag"=>$Tag,"Art"=>$docinput["Art"],
    "Buchung_Nr"=>$buchung["Buchung_Nr"],"Tage"=>ANZAHLTAGE,"docinput[Page]"=>$docinput["Page"]);
  echo '<table width="100%" cellpadding="5" cellspacing="0" border="0"><tr>';
  if ( !isset($docinput["Alle"]))
    $Alle = " AND Anzeigen";
  else
    $Alle = "";
  for ( $i= 0; $i < ANZAHLTAGE; $i++ )
   $Schlafplaetze[$i] = 0;
  if ( ! $query = sql_query("SELECT * FROM ".TABLE_ARTIKEL." WHERE Einruecken=0 AND F_Art_id=".
    $docinput["Art"]." $Alle ORDER BY Position")) echo "Fehler Kategorie: ".sql_error();
  $s = ZeigeBereiche($docinput["Art"], $buchung["Buchung_Nr"]);
  if ( $s != "" )
    $BelegteBereiche = explode(",",$s);
  else
    $BelegteBereiche = array();
  while ( $kategorie = sql_fetch_array($query) )
  {
    // wenn kein Reiter gewählt ist den ersten anzeigen in dem eine Buchung ist
    if ( ! isset($docinput["Page"] ) && Count($BelegteBereiche) > 0)
    {
      if ( in_array($kategorie["Bezeichnung"], $BelegteBereiche) )
        $docinput["Page"] = $kategorie["id"];
    }
    elseif ( ! isset($docinput["Page"] )) $docinput["Page"] = $kategorie["id"];
    if ( $docinput["Page"] == $kategorie["id"] )
    {
      $Color = $design["table_row_bright"];
      $Class = "selectedNav";
      if ( in_array($kategorie["Bezeichnung"], $BelegteBereiche) )
        $Color = $design["table_header"];
    }
    else
    {
      $Color = $design["table_row_dark"];
      $Class = "normalNav";
      if ( in_array($kategorie["Bezeichnung"], $BelegteBereiche) )
        $Color = $design["table_header"];
    }
    echo '<td bgcolor="'.$Color.'" width="1" background="img/edge_left.gif">';
    echo '&nbsp;</td>';
    echo '<td bgcolor="'.$Color.'" align="center" nowrap="nowrap">';
    echo '<a class="'.$Class.'" href="'.get_url($id,array_merge($params,
      array("docinput[Page]"=>$kategorie["id"]))).
      '">'.$kategorie["Bezeichnung"];
    echo '</a></td><td bgcolor="'.$Color.'" width="1" background="img/edge_right.gif">';
    echo '&nbsp;</td>';
  }
  echo '<td bgcolor="'.$design["default_background"].'" width="100%">';
  echo '&nbsp;</td></tr>';
  echo '</table>';
  echo '<table width="100%" cellspacing="0" cellpadding="1" border="0">';
  echo '<tr bgcolor="'.$design["table_row_bright"].'"><td align="center" colspan="'.(ANZAHLTAGE+2).'">';
  if ( $buchung["Buchung_Nr"] > 0 && $buchung["BStatus"] < BUCHUNG_FERTIG )
    echo '<input type="Submit" name="Save" value="Buchungen speichern">';
  echo '</td></tr>';
  echo '<tr bgcolor="'.$design["table_header"].'"><th>'.$Artikelarten[$docinput["Art"]].'
    </th><td></td><td>';
  echo '<a href="';
  echo get_url($id,array_merge($params,array("Tag"=>strtotime("-5 day",$Tag)))).
    '" title="5 Tage Früher">&lt;&lt;&lt;</a>';
  echo '</td><th colspan="'.(ANZAHLTAGE-2).'">';
  echo '<a href="';
  echo get_url($id,array_merge($params, array("Tage"=>ANZAHLTAGE-5))).
    '">&larr;</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
  echo 'Datum';
  echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="';
  echo get_url($id,array_merge($params, array("Tage"=>ANZAHLTAGE+5))).
    '">&rarr;</a>';
  echo '</th><td align="right">';
  echo '<a href="'.get_url($id,array_merge($params,array("Tag"=>strtotime("+5 day",$Tag)))).
    '" title="5 Tage später">&gt;&gt;&gt;</a>';
  echo '</td></tr>';
  echo '<tr bgcolor="'.$design["table_header"].'"><td>';
  echo '[ <a href="';
  if ( isset($docinput["Alle"]))
    echo get_url($id,$params).'">Wichtige Orte';
  else
    echo get_url($id,array_merge($params,array("docinput[Alle]"=>1))).'">Alle Orte';
  echo '</a> ]<br />';
  echo '</td><td></td>';
  for ( $i = 0; $i < ANZAHLTAGE; $i++ )
  {
    $datum = strtotime("+$i day",$Tag);
     echo '<td align="center">';
     if ( isset($buchung["Von"]) && $buchung["Von"] <= $datum && $buchung["Bis"] >= $datum )
       echo '<strong>';
     echo substr($Wochentage[date("w",$datum)],0,3).'<br />'.date("d.",$datum)."<br/>".date("m.",$datum)."<br/>".date("Y",$datum);
     if ( isset($buchung["Von"]) && $buchung["Von"] <= $datum && $buchung["Bis"] >= $datum )
       echo '</strong>';
     echo '</td>';
  }
  echo '</tr>';
  $docinput["Artikelnummern"] = "";
  ArtikelAnzeigen($docinput["Art"], $Tag, $docinput["Page"]);
  if ( $docinput["Art"] == CVJMART_ORT && isset($_REQUEST["Buchung_Nr"]) &&
       $_REQUEST["Buchung_Nr"]>0)
  {
    echo '<tr bgcolor="'.$design["table_row_bright"].'"><td>';
    echo '<em>Schlafplätze</em></td><td></td>';
    for ( $i = 0; $i < ANZAHLTAGE; $i++ )
      echo '<td>'.$Schlafplaetze[$i].'</td>';
    echo '</tr>';
  }
  echo '<tr bgcolor="'.$design["table_row_bright"].'"><td>';
  // Komma am Ende abschneiden
  if ( strlen($docinput["Artikelnummern"]) > 0 )
    $docinput["Artikelnummern"] = substr($docinput["Artikelnummern"],0,
      strlen($docinput["Artikelnummern"])-1);
  echo '<input type="hidden" name="docinput[Artikelnummern]" value="'.$docinput["Artikelnummern"].
    '">';
  echo '</td><td></td><td colspan="'.
    (ANZAHLTAGE/2).'" align="left"><a href="'.
    get_url($id,array_merge($params,array("Tag"=>strtotime("-5 day",$Tag)))).
    '" title="5 Tage Früher">&lt;&lt;&lt;</a></td>';
  echo '<td align="right" colspan="'.(ANZAHLTAGE/2+(ANZAHLTAGE % 2)).'"><a href="'.
    get_url($id,array_merge($params,array("Tag"=>strtotime("+5 day",$Tag)))).
    '" title="5 Tage später">&gt;&gt;&gt;</a></td></tr>';
  echo $design["table_end"];
  if ( $buchung["Buchung_Nr"] >= 0 && $buchung["BStatus"] < BUCHUNG_FERTIG )
  {
    echo '<div align="center"><input type="Submit" name="Save" value="Buchungen speichern"></div>';
    echo '</form>';
  }
  echo '<div align="center">[ <a href="'.get_url(get_cmodule_id("cvjmbuchung")).'">Buchungsliste</a> ]</div>';

?>