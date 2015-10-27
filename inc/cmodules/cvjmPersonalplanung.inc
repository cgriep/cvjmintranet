<?php

/*
   cvjmPersonalplanung

   AWF-Name: cvjmpersonalplanung

   Parameter:
   docinput['Buchung_Nr'] - Buchungsnummer

*/

if ( $profile['editor'] != 1 )
{
  include(INC_PATH.'misc/CVJM.inc');
  include(INC_PATH.'misc/cvjmBuchungstools.inc');
  include(INC_PATH.'misc/Kalenderjs.inc');
  include(INC_PATH.'misc/cvjmPersonalplanungtools.inc');
  KalenderScriptEinfuegen();
  speicherePersonalplanung($docinput);
  if ( $document['title'] != '')
  {
    echo '<h1>'.transform($document['title'],$document['datatype']['title']);
    echo ' '.$docinput['Buchung_Nr'].'</h1>';
  }
  if ( ! isset($docinput['Buchung_Nr']) )
  {
    echo 'Es wurde keine Buchungsnummer angegeben!<br />';
    echo 'Zurück zur <a href="'.get_url(get_cmodule_id('cvjmbuchung')).'">Buchungsliste</a><br />';
  }
  else
  {
    $query = sql_query('SELECT * FROM '.TABLE_BUCHUNGEN.' INNER JOIN '.TABLE_ADRESSEN.
      ' ON Adressen_id = F_Adressen_id WHERE Buchung_Nr = '.$docinput['Buchung_Nr']);
    $Buchung = sql_fetch_array($query);
    sql_free_result($query);
    ZeigeBuchungskopf($Buchung);
    echo '<form action="'.get_url($id).'" method="post" name="Personalplanungsform">';
    ZeigePersonalplanung($Buchung, 'Personalplanungsform');
    echo '<div class="zentriert"><input type="Submit" value="Speichern"><br />';
    echo '[ <a href="'.get_url(get_cmodule_id('cvjmbuchung'),"Buchung_Nr=".
      $docinput['Buchung_Nr']).'">zur Buchung '.$docinput["Buchung_Nr"].'</a> ] [ ';
    echo transform('%%'.get_cmodule_id('cvjmbuchungsbemerkungen').'%%','extrans').' ]';
    echo '</div>';
    speichereSchluessel($docinput);
    ZeigeSchluesselliste($Buchung);
    echo '<div class="zentriert"><input type="Submit" value="Speichern"></div>';
    echo '</form>';
    echo '<div class="zentriert">';
    echo '[ <a href="'.get_url(get_cmodule_id('cvjmbuchung'),'Buchung_Nr='.
      $docinput['Buchung_Nr']).'">zur Buchung '.$docinput['Buchung_Nr'].'</a> ] [ ';
    echo transform('%%'.get_cmodule_id('cvjmbuchungsbemerkungen').'%%','extrans').' ]';
    echo '</div>';
  }
}

?>