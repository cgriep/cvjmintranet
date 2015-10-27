<?php

/*

 cvjmSchluesselliste
 Zeigt die Liste der Schlüssel an

 Parameter:
 docinput[Buchung_Nr] - Die Buchungs-Nummer
 docinput[Ort][Artikel_id] - Die Schlüsselmenge an diesem Ort


*/


if ( $profile["editor"] != 1 )
{
  if ( $document['title'] != '')
    echo '<h1>'.transform($document['title'],$document['datatype']['title']).'</h1>';
  if ( ! is_numeric($docinput["Buchung_Nr"]) )
    echo "Keine Buchungsnummer angegeben!";
  else
  {
    include(INC_PATH.'misc/CVJM.inc');
    include(INC_PATH.'misc/cvjmPersonalplanungtools.inc');
    include(INC_PATH.'misc/cvjmBuchungstools.inc');
    $query = sql_query('SELECT * FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_ADRESSEN.
      ' ON Adressen_id=F_Adressen_id WHERE Buchung_Nr = '.$docinput['Buchung_Nr']);
    $Buchung = sql_fetch_array($query);
    sql_free_result($query);
    ZeigeBuchungskopf($Buchung);
    speichereSchluessel($docinput);
    echo $design['table_begin'];
    echo '<form action="'.get_url($id).'" method="post">';
    ZeigeSchluesselliste($Buchung);
    echo '<input type="hidden" name="docinput[Buchung_Nr]" value="'.$docinput["Buchung_Nr"].'" />';
    echo '<tr><td align="center"><input type="Submit" value="Speichern"></td></tr>';
    echo '</form>';
    echo $design['table_end'];
    echo '<div align="center">';
    echo '[ <a href="'.get_url(get_cmodule_id('cvjmbuchung'),"Buchung_Nr=".
      $docinput['Buchung_Nr']).'">zur Buchung '.$docinput['Buchung_Nr'].'</a> ] [ ';
    echo transform('%%'.get_cmodule_id('cvjmbuchungsbemerkungen').'%%','extrans').' ]';
    echo '</div>';
  }
} // wenn nicht editor

?>