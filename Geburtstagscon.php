<html>

<head>
<title>Geburtstagscon Poddel</title>

</head>

<body>
<?php 
if ( isset($_REQUEST['Zauberer']))
{

 $s = 'Eintrag zu Poddels Geburtstagscon am '.date("d.m.Y H:i")."\n";
 foreach ( $_REQUEST as $eintrag => $wert)
 {
	$s .= $eintrag . ' = ' .$wert . "\n";

 }
	mail('sascharheindorf@yahoo.de', 'Formular zu Poddels Geburtstagscon', $s);
	$s = 'Eintrag zu Poddels Geburtstagscon am '.date("d.m.Y H:i")."\n";
 $s .= 'Name: '.$_REQUEST['Teilnehmername']."\n";
 $s .= 'Bin dabei: '.$_REQUEST['BinDabei']."\n";
 $s .= 'E-Mail:'.$_REQUEST['EMail'];
    mail('poddel@podlinski.de', 'Anmeldung Geburtstagscon', $s);
	?>
	Ihre Daten wurden gespeichert.<br />
	 <br />Vielen Dank. 
	<?php
}
else
{
?>

<h1>Fragebogen zu Poddels Geburtstagscon &quot;...und ne Buddel voll Rum&quot;</h1>
<form action="Geburtstagscon.php" method="post">
<p>Name:  <textarea name="Teilnehmername" rows="1"></textarea>
, e-Mail: <textarea name="EMail" rows="1" ></textarea>
ich komme: <select name="BinDabei">
<option>Ja</option>
<option>Nein</option>
</select></p>

<p>Ich spiele lieber böse Rollen <select name="Boese Rollen">
<option >Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungBoeseRollen" /></p>

<p>Ich spiele offen und ehrlich <select name="Ehrliche Rollen">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungEhrlicheRollen" /></p>

<p>Ich spiele gerne verführerische Rollen <select name="Verfuererische Rollen">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungVerfuehrerischeRollen" /></p>
  
<p>Ich bin gerne Zauberer <select name="Zauberer">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungZauberer" /></p>

<p>Ich bin gerne Krieger  <select name="Krieger">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungKrieger" /></p>

<p>Ich möchte eine "wichtige" Rolle <select name="Wichtige Rolle">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungWichtigeRolle" /></p>

<p>Ich möchte gern eine eng mit Anderen verwobene Rolle  <select name="Verwobene Rollen">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungVerwobeneRollen" /></p>

<p>Ich möchte gerne mit anderen als Gruppe zusammen spielen (Bitte Namen angeben) <select name="Gruppe">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungGruppe" /></p>

<p>Ich trete gern selbstbewusst auf <select name="Selbstbewusst">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungSelbstbewusst" /></p>

<p>Ich spiele gerne mystische Rollen (Zauberer, Geister, magische Wesen...)<select name="Mystische Rolle">
<option>Ja</option>
<option selected="selected">egal</option>
<option>Nein</option>
</select> Anmerkung: <input type="text" name="AnmerkungMystischeRolle" /></p>

<p>Ich singe gern <select name="Singen">
<option>Ja</option>
<option>Nein</option>
</select></p>
<p>Ich tanze gern <select name="Tanzen">
<option>Ja</option>
<option>Nein</option>
</select></p>
</p>
<p>Ich musiziere gern <select name="Musizieren">
<option>Ja</option>
<option>Nein</option>
</select></p>
</p>
<p>Ich stelle gern Rituale dar <select name="Rituale">
<option>Ja</option>
<option>Nein</option>
</select></p>
</p>

<p>Anmerkungen (Vielleicht hast du ja eine ungefähre Vorstellung davon, welche Rolle du pielen möchtest, dann kannst du sie hier kurz umreißen):
<br /><textarea name="Anmerkungen" cols="50" rows="10">
</textarea></p>

<input type="submit" value="Absenden" />
</form>
<?php } ?>
</body>

</html>
