<?php
/*
        Copyright (C) 2000-2001 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)


        Last update: 08.09.2003 by Ren&eacute; Jansen

          Der Disclaimer kann so direkt in die Links.txt eingebaut werden...
                1|*|3|<b>Disclaimer</b><br /><center>
                1|*|3|Gem&auml;&szlig; � 5 Abs.1 TDG sind wir ausschlie&szlig;lich nur f&uuml;r die eigenen Inhalte verantwortlich. F&uuml;r Links<br />
                1|*|3|auf fremde Inhalte dritter Anbieter sind wir gem&auml;&szlig; � 5 Abs.2 TDG nur verantwortlich, wenn wir von<br />
                1|*|3|einem rechtswidrigen oder strafbaren Gehalt positive Kenntnis haben und es technisch m&ouml;glich und<br />
                1|*|3|zumutbar ist, deren Nutzung zu verhindern. Auch sind wir nicht verpflichtet, in periodischen<br />
                1|*|3|Abst&auml;nden den Inhalt von Angeboten Dritter auf deren Rechtswidrigkeit oder Strafbarkeit zu &uuml;ber-<br />
                1|*|3|pr&uuml;fen. Sobald wir von dem rechtswidrigen Inhalt der Web-Seiten Dritter erfahren, wird der Link<br />
                1|*|3|von unserer Seite entfernt. Weiterhin m&ouml;chten wir ausdr&uuml;cklich betonen, dass wir keinen Einfluss<br />
                1|*|3|auf die Gestaltung und die Inhalte der gelinkten Seiten haben. Deshalb distanzieren wir uns hier-<br />
                1|*|3|mit ausdr&uuml;cklich von allen Inhalten aller gelinkten Seiten auf der gesamten Website inkl. aller<br />
                1|*|3|Unterseiten. Diese Erkl&auml;rung gilt f&uuml;r alle auf der Homepage ausgebrachten Links und f&uuml;r alle<br />
                1|*|3|Inhalte der Seiten, zu denen Links oder Banner f&uuml;hren. Sollten Inhalte dieser Internetangebote<br />
                1|*|3|gegen geltendes Recht versto&szlig;en, werden diese auf Hinweis schnellstm&ouml;glich entfernt.<br /></center>

          Aufbau der Links.txt
          Userlevel|Docid|Datatype|Text1|Text2

          Userlevel:         1 = alle
                                2 = angemeldete Benutzer
                                3 = Editoren

          Docid:                Nummer des Documentes, indem der Link sichtbar sein soll

          Datatype:                1 = Linktext (z.B. GMX) und LinkUrl (z.B. http://www.gmx.de)
                                2 = Gepr&uuml;ft am .... - Text
                                3 = Linkbeschreibung (z.B. Der wohl beste Freemail-Anbieter)

*/

function writelinkline ($linklineobject, $linklinetext1, $linklinetext2="") {
            if ($linklineobject == '1') {
                echo('<br /><br /><a href="'.$linklinetext2.'" target="_new">'.$linklinetext1.'</a><br />'); }
        elseif ($linklineobject == '2') {
                echo('<font size="-2">Gepr&uuml;ft am: '.$linklinetext1.'</font><br />'); }
        elseif ($linklineobject == '3') {
                echo('<font size="-1">'.$linklinetext1.' '); }
}

function userisokandisdoctoo ($checkdata1, $checkdata2, $checkdata3="", $checkdata4="") {
        global $id;
        if($checkdata1 == '*') {
                writelinkline($checkdata2, $checkdata3, $checkdata4);
        }
        if($checkdata1 == $id) {
                writelinkline($checkdata2, $checkdata3, $checkdata4);
        }
}

global $id;
$filename = INC_PATH.DATA_PATH.'links.txt';
$lines = file($filename);
foreach ( $lines as $line_num => $line)  {
        $buffer =  explode('\|', $line);
        if ($buffer[0] == '1') {
                userisokandisdoctoo ($buffer[1], $buffer[2], $buffer[3], $buffer[4]);
        }
        if ($buffer[0] == '2' && SESSION_STATUS == 'ok') {
                userisokandisdoctoo ($buffer[1], $buffer[2], $buffer[3], $buffer[4]);
        }
        if ($buffer[0] == '3' && $profile['editor']=='1') {
                userisokandisdoctoo ($buffer[1], $buffer[2], $buffer[3], $buffer[4]);
        }
}

?>