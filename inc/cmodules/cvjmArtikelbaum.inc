<?php

/*
    awf-Modul Artikelbaum
    Zeigt den Artikelbaum an
    cvjmArtikelbaum.inc
    AWF-Name: cvjmartikelbaum
*/

function zeigeTabelle($tabelle, $artikel_id, $einruecken=0)
{
  $query = sql_query("SELECT * FROM $tabelle INNER JOIN ".TABLE_ARTIKEL.
    " ON id=F_Unterartikel_id WHERE F_Artikel_id=$artikel_id ORDER BY Bezeichnung");
  $nr = 0;
  while ( $artikel = sql_fetch_array($query) )
  {
    if ( $einruecken != 0 )
      echo '<br /><span style="width:'.($einruecken*10).'px;float:left;">&nbsp;</span>';
    elseif ( $nr != 0 )
      echo "<br />";
    echo stripslashes($artikel["Bezeichnung"])." (".$artikel["Menge"].")";
    $nr++;
    zeigeTabelle($tabelle, $artikel["F_Unterartikel_id"], $einruecken+1);
  }
  sql_free_result($query);
  return $nr;
}

if ( $profile["Editor"] != "1" )
{
  include(INC_PATH."misc/CVJM.inc");
  if ( !isset($docinput["Art_id"]) || ! is_numeric($docinput["Art_id"]) )
    $docinput["Art_id"] = -1;
  $Artikelarten = holeArtikelarten();
  echo '<h1>'.transform($document['title'], $document['datatype']['title']).'</h1>';
  if ( isset($docinput["Beziehung"]) )
  {
    $komplex = "&docinput[Beziehung]=".$docinput["Beziehung"];
  }
  else
    $komplex = "";
  reset($Artikelarten);
  echo '[ <a href="'.get_url($id).'">Alle Artikel</a> ] ';
  foreach ($Artikelarten as $key => $value )
  {
    if ( $docinput["Art_id"] == $key )
      echo '<span style="background-color:'.$design["table_header"].'">';
    echo '[ <a href="'.get_url($id, "docinput[Art_id]=$key$komplex").'">'.$value.'</a> ]';
    if ( $docinput["Art_id"] == $key ) echo '</span>';
    echo " ";
  }
  if ( $docinput["Art_id"] != -1 )
  {
    $Where = "WHERE Art_id=".$docinput["Art_id"];
    if ( ! isset($docinput["Beziehung"]) )
    {
      echo '[ <a href="'.get_url($id,array("docinput[Art_id]"=>$docinput["Art_id"],
        "docinput[Beziehung]"=>1)).'">Beziehungen anzeigen</a> ] ';
      echo '[ <a href="'.get_url($id,array("docinput[Art_id]"=>$docinput["Art_id"],
        "docinput[Beziehung]"=>2)).'">nur Beziehungen anzeigen</a> ]';
    }
  }
  else
    $Where = "";
  $sql = "SELECT * FROM ".TABLE_ARTIKEL." INNER JOIN ".TABLE_ARTIKELARTEN.
    " ON F_Art_id=Art_id $Where ORDER BY Art, Position";
  if (!$query = sql_query($sql)) echo mysql_error();
  $Art = "";
  echo '<table width="100%">';
  while ($artikel = sql_fetch_array($query) )
  {
     if ( $Art != $artikel["Art"] )
     {
       $Art = $artikel["Art"];
       echo '<tr><th colspan="';
       if ( isset($docinput["Beziehung"]) )
         echo '4';
       else
         echo "2";
       echo '" bgcolor="'.$design["table_header"].'">'.stripslashes($Art);
       echo "</th></tr>\n";
       if ( isset($docinput["Beziehung"]) )
       {
         echo '<tr><td></td><td></td><td><em>mitgebuchte Artikel</em></td><td><em>nicht berechnete Artikel</em></td></tr>';
       }
     }
     echo '<tr><td valign="top"><a href="'.get_url(get_cmodule_id("cvjmartikel"),
       "docinput[Artikel_Nr]=".$artikel["id"]).'">'.$artikel["Position"]."</a></td>";
     echo '<td valign="top" ';
     if ( isset($docinput["Beziehung"] ) )
       echo 'colspan="3"';
     echo '>';
     echo '<span style="width:'.($artikel["Einruecken"]*10).'px;float:left;">&nbsp;</span>';
     echo stripslashes($artikel["Bezeichnung"]);
     echo '</td></tr>';
     if ( isset($docinput["Beziehung"]) )
     {
       echo '<tr><td></td><td></td><td>';
       zeigeTabelle(TABLE_ARTIKELBEZIEHUNGEN, $artikel["id"]);
       echo '</td><td>';
       zeigeTabelle(TABLE_ARTIKELGRUPPEN, $artikel["id"]);
       echo '</td></tr>';
     }
  }
  echo '</table>';
  sql_free_result($query);
}


?>