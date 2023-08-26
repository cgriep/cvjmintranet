<?php

/* Modul Orte

AWF-Name: cvjmorte (wird in cvjmbuchungsuebersicht verwendet

Parameter: Ort - zeigt den entsprechenden Ort an

*/



include(INC_PATH."misc/CVJM.inc");
include(INC_PATH."misc/cvjmArtikeltools.inc");

OrtsAuswahlScriptEinfuegen();
$Artikelarten = holeArtikelarten();

// CSS-Styles für ein- und ausgeblendete Nodes
?>
<style type="text/css">
<!--
.da { visibility: visible; display: block; }
.weg { visibility: hidden; display: none; }
-->
</style>

<script language="javascript">
function Switch(id) {
  Elementknoten = document.getElementById("O"+id);
  ElementknotenI = document.getElementById("P"+id);
  if ( Elementknoten.className=="da" ){
    Elementknoten.className = "weg";
    ElementknotenI.innerHTML = '<img src="img/kleinplus.gif" border="0" />';
  }else {
    Elementknoten.className = "da";
    ElementknotenI.innerHTML = '<img src="img/kleinminus.gif" border="0" />';
}}
function ZeigeAlle(){
  Elemente = document.getElementsByTagName("span");
  for ( i = 0; i < Elemente.length; i++)
    if ( Elemente[i].id.charAt(0) == "O" )
      Elemente[i].className = "da";
  Elemente = document.getElementsByTagName("a");
  for ( i = 0; i < Elemente.length; i++)
    if ( Elemente[i].id.charAt(0) == "P" )
      Elemente[i].innerText = '<img src="img/kleinminus.gif" border="0" />';
}
function SchliesseAlle(){
  Elemente = document.getElementsByTagName("span");
  for ( i = 0; i < Elemente.length; i++)
    if ( Elemente[i].id.charAt(0) == "O" )
      Elemente[i].className = "weg";
  Elemente = document.getElementsByTagName("a");
  for ( i = 0; i < Elemente.length; i++)
    if ( Elemente[i].id.charAt(0) == "P" )
      Elemente[i].innerText = '<img src="img/kleinplus.gif" border="0" />';
}
</script>
<?php

if ( $profile["editor"] != 1 )
{
  if ( isset($_REQUEST["Paste"]) && is_numeric($_REQUEST["Paste"]) && is_numeric($profile["OrtCopy$id"]))
  {
    $sql = "UPDATE ".TABLE_ARTIKEL." SET parent = ".$_REQUEST["Paste"]." WHERE id = ".$profile["OrtCopy$id"];
    if ( sql_query($sql) )
      echo "<strong>&gt;&gt;&gt; Eintrag ".Ortsname($profile["OrtCopy$id"])." verschoben.</strong><br />";
    remove_profile("OrtCopy$id");
    $DerOrt = "";
    $_REQUEST["Ort"] = $_REQUEST["Paste"];
  }
  if ( isset($_REQUEST["NeuArt"]) )
  {
     $sql = "INSERT INTO ".TABLE_ARTIKELARTEN." (Art) VALUES ('".trim($_REQUEST["NeuArt"]);
     $sql .= "')";
     sql_query($sql);
     $_REQUEST["Art"] = sql_insert_id();
  }
  if ( isset($_REQUEST["Art"]) && is_numeric($_REQUEST["Art"]) )
  {
    $DieArt = $_REQUEST["Art"];
  }
  else {
    $DieArt = CVJMART_ORT;
    $Nr = "";
    if ( is_numeric($_REQUEST["Ort"] ) ) $Nr = $_REQUEST["Ort"];
    if ( is_numeric($_REQUEST["DelX"] ) ) $Nr = $_REQUEST["DelX"];
    if ( is_numeric($_REQUEST["DelV"] ) ) $Nr = $_REQUEST["DelV"];
    if ( is_numeric($Nr) )
    {
      $qu = sql_query("SELECT F_Art_id FROM ".TABLE_ARTIKEL." WHERE id = ".$Nr);
      $row = sql_fetch_array($qu);
      $DieArt = $row["F_Art_id"];
    }
  }
  $openNodes = explode(",",$profile[$DieArt."Nodes$id"]);
  if ( is_numeric($_REQUEST["Open"] ) ) {
    if ( in_array($_REQUEST["Open"] , $openNodes)) {
      $key = array_search($_REQUEST["Open"], $openNodes);
      unset($openNodes[$key]);
    }
    else
      $openNodes[] = $_REQUEST["Open"];
    set_profile($DieArt."Nodes$id", implode(",",$openNodes));
  }
  if ( isset($_REQUEST["Copy"]) && is_numeric($_REQUEST["Copy"]) && SESSION_STATUS == "ok")
  {
    set_profile("OrtCopy$id", $_REQUEST["Copy"]);
    $profile["OrtCopy$id"] = $_REQUEST["Copy"];
  }
  if ( is_numeric($profile["OrtCopy$id"]) )
  {
    echo "<strong>Verschiebemodus:</strong> ".Ortsname($profile["OrtCopy$id"])." wird verschoben.<br />";
  }
  $DerOrt = "";
  if ( !isset($_REQUEST["Wahl"]) ) {
    echo '<form action="'.get_url($id).'" method="post">';
    echo 'Kategorie <select name="Art">';
    foreach ( $Artikelarten as $key => $value)
    {
      echo "<option ";
      if ( $key == $DieArt ) echo ' selected="selected"';
      echo ">".$value."</option>";
    }
    echo '</select>';
    echo '<input type="Submit" value="Kategorie ändern">';
    echo '</form><br />';
  }
  else
  {  // Ortswahl
    ?>
    <script language="javascript">
    function Uebertragen(id, info)
    {
      opener.document.getElementById("<?=$_REQUEST["Wahl"]?>").value = id;
      opener.document.getElementById("<?=$_REQUEST["Wahl"]?>text").innerHTML = info;
      this.close();
    }
    </script>
    <?php
  }
}

function OrteAnzeigen($Ebene = 1, $parent = -1, $oclass = "")
{
  global $id;
  global $DieArt;
  global $openNodes;
  // Zeigt alle Orte als Baum an
  $qu = sql_query("SELECT id, Bezeichnung FROM ".TABLE_ARTIKEL." WHERE F_Art_id = ".$DieArt.
    " AND parent = $parent");
  while ( $row = sql_fetch_array($qu) )
  {
    if ( $oclass != "" ) echo '<span id="O'.$parent.'" class="'.$oclass.'">';
    echo "<strong>";
    for ( $i = 1; $i < $Ebene; $i++ )
      echo "&nbsp;&nbsp;";
    if ( in_array($row["id"], $openNodes)) {
      $class = "da";
      $Zeige = "-";
    }
    else
    {
      $class = "weg";
      $Zeige = "+";
    }
    $param = 'Open='.$row["id"]."&Art=".$DieArt;
    if ( isset($_REQUEST["Wahl"] ) ) $param = "&Wahl=".$_REQUEST["Wahl"];
    // docinput[design]=Printable&
    echo '<font size="4"><a id="P'.$row["id"].'" '. // onclick="javascript:Switch('.$row["id"].');"'.
      ' href="'.get_url($id, $param);
    echo '">'.$Zeige.'</a></font></strong> ';
    if ( $row["id"] == $_REQUEST["Ort"] ) echo "<strong>";
    if ( !isset($_REQUEST["Wahl"] ))
      echo '<a href="'.get_url($id,'Ort='.$row["id"]).'">';
    else
      echo '<a href="javascript:Uebertragen(\''.$row["id"].'\',\''.OrtsName($row["id"],false).'\')">';
    echo $row["Bezeichnung"].'</a>';
    if ( $row["id"] == $_REQUEST["Ort"] ) echo "</strong>";
    echo '<br />';
    OrteAnzeigen($Ebene+1, $row["id"],$class);
    if ( $oclass != "" ) echo '</span>';
  }
  sql_free_result($qu);
}

function OrteAnzeigenLinear()
{
  global $id;
  global $DieArt;
  global $openNodes;
  // Zeigt alle Orte als Baum an
  if ( ! $qu = sql_query("SELECT id, Bezeichnung, Einruecken FROM ".TABLE_ARTIKEL.
    " WHERE F_Art_id = ".$DieArt." ORDER BY Position")) echo sql_error();
  $class = "da";
  $einrueck = 0;
  $parent = array();
  $parent[-1] = -1;
  while ( $row = sql_fetch_array($qu) )
  {
    $parent[$row["Einruecken"]] = $row["id"];
    while ( $einrueck > $row["Einruecken"] )
    {
      echo '</span>';
      $einrueck--;
    }
    if ( $einrueck < $row["Einruecken"] )
    {
      echo '<span id="O'.$parent[$row["Einruecken"]-1].'" class="'.$class.'">';
      $einrueck = $row["Einruecken"];
    }
    echo "<strong>";
    echo '<span style="width:'.($row["Einruecken"]*10).'px;float:left;">&nbsp;</span>';
    //for ( $i = 1; $i < $row["Einruecken"]; $i++ )
    //  echo "&nbsp;&nbsp;";
    if ( in_array($row["id"], $openNodes) ) {
      $class = "da";
      $Zeige = '<img src="img/kleinminus.gif" border="0" />';
    }
    else
    {
      $class = "weg";
      $Zeige = '<img src="img/kleinplus.gif" border="0" />';
    }
    $param = 'Open='.$row["id"]."&Art=".$DieArt;
    if ( isset($_REQUEST["Wahl"] ) ) $param .= "&Wahl=".$_REQUEST["Wahl"];
    //docinput[design]=Printable&
    echo '<a id="P'.$row["id"].'"'; // onclick="javascript:Switch('.$row["id"].');"';
    echo ' href="'.get_url($id, $param).'"';
    echo '>'.$Zeige.'</a></strong> ';
    if ( $row["id"] == $_REQUEST["Ort"] ) echo "<strong>";
    if ( !isset($_REQUEST["Wahl"] ))
      echo '<a href="'.get_url($id,'Ort='.$row["id"]).'">';
    else
      echo '<a href="javascript:Uebertragen(\''.$row["id"].'\',\''.OrtsName($row["id"],false).'\')">';
    echo $row["Bezeichnung"].'</a>';
    if ( $row["id"] == $_REQUEST["Ort"] ) echo "</strong>";
    echo "<br />\n";
  }
  while ( $einrueck > 0 )
  {
    echo '</span>';
    $einrueck--;
  }
  sql_free_result($qu);
}

function DeleteAll($id = -1)
{
  if ( $id > -1 && is_numeric($id) )
  {
    if ( ! sql_query("DELETE FROM ".TABLE_ARTIKEL." WHERE id = ".$id) )
      echo sql_error();
    $qu = sql_query("SELECT id FROM ".TABLE_ARTIKEL." WHERE parent = ".$id);
    while ( $row = sql_fetch_array($qu) )
      DeleteAll($row["id"]);
    sql_free_result($qu);
  }
}

// Hinzuf�gen
if ( isset($_REQUEST["OrtName"]) && is_numeric($_REQUEST["Ort"]) && is_numeric($DieArt) ) {
  if ( $_REQUEST["OrtName"] != "" ) {
    if ( ! sql_query("INSERT INTO ".TABLE_ARTIKEL." (Bezeichnung, parent, F_Art_id, Anzeigen) VALUES ('".
      addslashes($_REQUEST["OrtName"])."',".$_REQUEST["Ort"].",".$DieArt.",1)") )
        echo sql_error();
    // $_REQUEST["Ort"] = sql_insert_id();
  }
}

// ändern
if ( isset($_REQUEST["OrtNeu"]) && is_numeric($_REQUEST["Ort"]) ) {
  if ( $_REQUEST["OrtNeu"] != "" ) {
    if ( isset($_REQUEST["Anzeigen"])) $Anzeigen = 1;
    else $Anzeigen = 0;
    if ( ! sql_query("UPDATE ".TABLE_ARTIKEL." SET Bezeichnung = '".addslashes($_REQUEST[OrtNeu]).
      "', Anzeigen=".$Anzeigen." WHERE id = ".$_REQUEST["Ort"]) ) echo sql_error();
  }
}
// löschen, verschieben
if ( is_numeric($_REQUEST["DelV"])  ) {
    $qu = sql_query("SELECT parent FROM ".TABLE_ARTIKEL." WHERE id = ".$_REQUEST["DelV"]);
    if ( $row = sql_fetch_array($qu) )
      $parent = $row["parent"];
    else
      $parent = -1;
    sql_free_result($qu);
    if ( ! sql_query("UPDATE ".TABLE_ARTIKEL." SET parent = $parent WHERE parent = ".$_REQUEST["DelV"]) )
      echo sql_error();
    if ( ! sql_query("DELETE FROM ".TABLE_ARTIKEL." WHERE id = ".$_REQUEST["DelV"]) )
      echo sql_error();
    $_REQUEST["Ort"] = $parent;
}
// löschen, vollst�ndig
if ( is_numeric($_REQUEST["DelX"])  ) {
  DeleteAll($_REQUEST["DelX"]);
}

if($profile['editor']!='1') {
  if ($document['title'] != '') {
    echo '<h2>'.transform($document['title'], $document['datatype']['title']).'</h2>';
  }
  if($document['image'] != '' || $document['body'] != '') {
    echo '<p class="justify">';
    // Image
    echo transform($document['image'], $document['datatype']['image']);
    // Text
    echo transform($document['body'], $document['datatype']['body']).'</p>';
  }
  OrteAnzeigenLinear();
  echo "<br />";
  echo '<a href="#" onClick="javascript:ZeigeAlle()">[ Alles aufklappen ]</a>&nbsp;';
  echo '<a href="#" onClick="javascript:SchliesseAlle()">[ Alles zuklappen ]</a>';
  echo "<br /><br />";
  if ( is_numeric($_REQUEST["Ort"]) )
  {
    echo '<table width="100%"><tr bgcolor="'.$design["table_row_dark"].'"><td>';
    echo '<strong><a href="'.
      get_url($id, array("Art" => $DieArt)).'">'.$Artikelarten[$DieArt].'</a> > '.
      Ortsname($_REQUEST["Ort"])."</strong>";
    echo '</td></tr></table>';
  }
  else
    echo "<strong>CVJM Feriendorf</strong><br />";
  /*
  echo '<form action="'.get_url($id).'" method="post">';
  if ( is_numeric($_REQUEST["Ort"]) )
    echo '<input type="hidden" name="Ort" value="'.$_REQUEST["Ort"].'" />';
  else
    echo '<input type="hidden" name="Ort" value="-1" />';
  echo '<input type="hidden" name="Art" value="'.$DieArt.'" />';
  echo '<input type="text" name="OrtName" />';
  echo '<input type="submit" value="Unterkategorie hinzuf�gen" /></form>';
  if ( is_numeric($_REQUEST["Ort"]) ) {
    echo '<a id="Edit" name="Edit"></a><form action="'.get_url($id).'" method="post">';
    echo '<input type="hidden" name="Ort" value="'.$_REQUEST["Ort"].'" />';
    echo '<input type="hidden" name="Art" value="'.$DieArt.'" />';
    $query = sql_query("SELECT * FROM ".TABLE_ARTIKEL." WHERE id = ".$_REQUEST["Ort"]);
    $Ort = sql_fetch_array($query);
    sql_free_result($query);
    echo '<input type="text" name="OrtNeu" value="'.$Ort["Bezeichnung"].'" />';
    echo '<input type="Checkbox" id="Anzeigen" name="Anzeigen" value="v" ';
    if ( $Ort["Anzeigen"] ) echo 'checked="checked"';
    echo '> <label for="Anzeigen">Anzeigen</label> ';
    echo '<input type="submit" value="Bezeichnung &auml;ndern" /></form>';
  }
  */
  if ( is_numeric($profile["OrtCopy$id"]) )
  {
    if ( is_numeric($_REQUEST["Ort"]))
      echo 'Elemente '.Ortsname($profile["OrtCopy$id"]).' unterhalb '.$DerOrt.' <a href="'.
        get_url($id,'Paste='.$_REQUEST["Ort"]).'">'.
        ' einf&uuml;gen</a><br />';
    else
      echo 'Elemente '.Ortsname($profile["OrtCopy$id"]).' als neuen Hauptast <a href="'.
        get_url($id,'Paste=-1').'">'.
        ' einf&uuml;gen</a><br />';
  }
  /*
  if ( is_numeric($_REQUEST["Ort"]) && $_REQUEST["Ort"] >= 0 ) {
    echo '<a href="'.get_url($id,'DelV='.$_REQUEST["Ort"]).'">Element '.$DerOrt.' löschen, Untereintr&auml;ge aufr&uuml;cken</a><br />';
    echo '<a href="'.get_url($id,'DelX='.$_REQUEST["Ort"]).'">Element '.$DerOrt.' löschen, Untereintr&auml;ge entfernen</a><br />';
    echo '<a href="'.get_url($id,'Copy='.$_REQUEST["Ort"]).'">Element '.$DerOrt.' verschieben</a><br />';
  }
  */
  if ( ! isset($_REQUEST["Ort"]) ) $_REQUEST["Ort"] = -1;
  $qu = sql_query("SELECT id, Bezeichnung FROM ".TABLE_ARTIKEL.
    " WHERE F_Art_id = ".$DieArt);
  if ( sql_num_rows($qu) > 0 ) {
    echo '<form action="'.get_url($id).'" method="post">';
    echo '<input type="hidden" name="Art" value="'.$DieArt.'"></input>';
    echo '<select name="Ort">';
    while ( $row = sql_fetch_array($qu) )
    {
      echo '<option value="'.$row["id"].'" ';
      if ( $_REQUEST["Ort"] == $row["id"] ) echo 'selected="selected"';
      echo '>'.$row["Bezeichnung"].'</option>';
    }
    echo "</select>";
    echo '<input type="submit" value="'.$Artikelarten[$DieArt].' ausw�hlen" />';
    echo '</form>';
  }
  sql_free_result($qu);
  /*
  echo '<form action="'.get_url($id).'" method="post">';
  echo '<label for="Kategorie">Neue Hauptkategorie</label>';
  echo '<input id="Kategorie" type="text" name="NeuArt" maxlen="20"></input>';
  echo '<input type="Submit" value="erstellen">';
  echo '</form>';
  */
}
else
{
  // Image Input
  create_editor_input('Image','image','image_input');
  echo '<br /><br />';
  // Text Input
  create_editor_input('Body','body','area_input');
  echo "<br /><br />";
}


?>