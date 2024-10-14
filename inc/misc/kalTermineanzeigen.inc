<?php

/**
 * Zeigt die aktiven Termine des Tages des eingeloggten Benutzers an
 * wird auf der Startseite verwendet, um einen Überblick über die anstehenden Dinge zu
 * erhalten.
 * 
 */

load_lang('Kalender');
include(INC_PATH.'misc/Termin.inc');
include_once(INC_PATH.'cvjm/Event.class.php');
global $design;
// Damit die Termine korrekt angezeigt werden
$document['AlleAnzeigen'] = 1;

function sucheTermine($Tag, $Monat, $Jahr)
{
	$Datum = mktime(0,0,0,$Monat,$Tag,$Jahr);
	// Termine suchen
	$search = "(name='Datum' AND ";
	$search .= "value BETWEEN '" . $Datum."'";
	$search .= " AND '" .mktime(23,59,59,$Monat,$Tag,$Jahr)."')";
	$search .= " OR (name='Enddatum' AND ";
	$search .= "value BETWEEN '" . $Datum."'";
	$search .= " AND '" .mktime(23,59,59,$Monat,$Tag,$Jahr)."')";
	unset($Termine);
	$Termine = search_nodes($search);
	if ( ! is_array($Termine))
	{
		$Termine = array();
	}
	$search = "name='Datum' AND ";
	$search .= "value < '" . $Datum."'";
	$fTermine = search_nodes($search, 500);
	if ( ! is_array($fTermine))
	{
		$fTermine = array();
	}
	foreach ( $fTermine as $key => $value)
	{
		if ( $value['Enddatum'] > mktime(23,59,59,$Monat,$Tag,$Jahr) )
		{
			$Termine[$key] = $value;
		}
	}
	unset($fTermine);
	$search = "name='Enddatum' AND ";
	$search .= "value > '".mktime(23,59,59,$Monat,$Tag,$Jahr)."'";
	$fTermine = search_nodes($search, 500);
	if ( ! is_array($fTermine))
	{
		$fTermine = array();
	}
	foreach ( $fTermine as $key => $value)
	{
		if ( $value['Datum'] < $Datum )
		{
			$Termine[$key] = $value;			
		}
	}

	unset($fTermine);
	$search = "name='Turnus' AND ";
	$search .= "value <> ''";
	$fTermine = search_nodes($search, 500);
	if ( ! is_array($fTermine))
	{
		$fTermine = array();
	}
	foreach ( $fTermine as $key => $value) 
	{
		if (($value['TurnusEnde'] == 0 ||
		$value['TurnusEnde'] >= $Datum) &&
		$value['Datum'] <= $Datum )
		switch ($value['Turnus']) {
			case 'd':
				$Termine[$key] = $value;
				$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
				date('i',$value['Datum']), date('s',$value['Datum']),
				$Monat, $Tag, $Jahr);
				$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
				date('i',$value['Enddatum']), date('s',$value['Enddatum']),
				$Monat, $Tag, $Jahr);
				break;
				break;
			case 'w':
				if ( date('w', $value['Datum']) == date('w',$Datum) ) {
					$Termine[$key] = $value;
					$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
					date('i',$value['Datum']), date('s',$value['Datum']),
					$Monat, $Tag, $Jahr);
					$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
					date('i',$value['Enddatum']), date('s',$value['Enddatum']),
					$Monat, $Tag, $Jahr);
				}
				break;
			case 'v':
				$unter = ((mktime(date('H',$value['Datum']), date('i',$value['Datum']),0,
				$Monat,$Tag,$Jahr)-$value['Datum'])/86400);
				if ( ($unter % 14) == 0 ) {
					if ( date('w', $value['Datum']) == date('w',$Datum) ) {
						$Termine[$key] = $value;
						$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
						date('i',$value['Datum']), date('s',$value['Datum']),
						$Monat, $Tag, $Jahr);
						$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
						date('i',$value['Enddatum']), date('s',$value['Enddatum']),
						$Monat, $Tag, $Jahr);
					}
				}
				break;
			case 'm':
				if ( date('d',$value['Datum']) == $Tag ) {
					$Termine[$key] = $value;
					$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
					date('i',$value['Datum']), date('s',$value['Datum']),
					$Monat, $Tag, $Jahr);
					$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
					date('i',$value['Enddatum']), date('s',$value['Enddatum']),
					$Monat, date('d',$value['Enddatum']), $Jahr);
				}
				break;
			case 'j':
				if ( date('m',$value['Datum']) == $Monat && date('d',$value['Datum']) == $Tag) {
					$Termine[$key] = $value;
					$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
					date('i',$value['Datum']), date('s',$value['Datum']),
					$Monat, $Tag, $Jahr);
					$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
					date('i',$value['Enddatum']), date('s',$value['Enddatum']),
					$Monat, date('d',$value['Enddatum']), $Jahr);
				}
				break;
			default:
		}
	}
	unset($fTermine);		
	if ( Count($Termine) == 0 )
	{
		unset($Termine);
	}
	else
	{
		// wir wollen nur die Zeit der Termine haben!
		foreach ( $Termine as $key => $value )
		{
			$Termine[$key]['Datum'] = strtotime(date('H:i', $value['Datum']));
		}
		$Termine = sortiereTermine($Termine);
	}
	// Neue Terminstruktur - über Events 
	$fTermine = Event::searchEvents($Datum);
	foreach ( $fTermine as $termin )
	{
		$nTermin = array();
		$nTermin['Datum'] = $termin->Datum;
		$nTermin['Enddatum'] = strtotime('+'.$termin->Dauer.'min',$termin->Datum);
		$nTermin['author_id'] = $termin->Autor;
		$nTermin['Turnus'] = $termin->Turnus;
		$nTermin['title'] = $termin->Titel;
		$Termine['EV'.$termin->Event_id] =  $nTermin;
	}	
	return $Termine;
}

$Datum = time();
echo '<table width="100%">';
echo '<tr valign="top">';
for ( $i=0; $i< 3; $i++)
{
	$Monat = date("n", $Datum);
	$Tag = date("d", $Datum);
	$Jahr = date("Y", $Datum);
	echo '<td width="33%">';
	echo '<div align="center">';
	switch ( date('w',mktime(0,0,0,$Monat,$Tag,$Jahr)) )
	{
		case 0: echo 'Sonntag'; break;
		case 1: echo 'Montag'; break;
		case 2: echo 'Dienstag'; break;
		case 3: echo 'Mittwoch'; break;
		case 4:echo 'Donnerstag'; break;
		case 5:echo 'Freitag'; break;
		case 6: echo 'Samstag'; break;
	}
	echo " $Tag. ".$mth[$Monat].'</div>';

	$Termine = sucheTermine($Tag, $Monat, $Jahr);

	$switch = true;
	$Anz = 0;
	foreach ( $Termine as $key => $value)
	{
		if ( $switch )
		{
		echo '<div class="helleZeile">';
		}
		else
		{
		echo '<div class="dunkleZeile">';
		}
		$switch = ! $switch;
		echo date('H:i',$value['Datum']).' ';
		echo '<a href="';
		echo get_url($value['id']).'" title="'.get_user_nickname($value['author_id']).': ';
		if ( is_numeric($value['Enddatum']) )
		{
			echo 'bis '.date('d.m.Y H:i',$value['Enddatum']);
		}
		if ( strtotime("$Jahr-$Monat-$Tag 23:59") < $value['Enddatum'] )
		{
			echo ' (ganztägig!)';
		}
		switch ( $value['Turnus'] )
		{
			case 'd': echo ' täglich';
			break;
			case 'w': echo ' wöchentlich';
			break;
			case 'm': echo ' monatlich';
			break;
			case 'j': echo ' jährlich';
		}
		echo '">'.transform($value['title'],'clean').'</a>';
		echo '</div>';
	}
	if ( !is_array($Termine) ) { $Termine = array(); } 
	echo '<div align="center"><strong>'.count($Termine).' Termine</strong></div>';
	echo '</td>';
	$Datum = strtotime('+1 day', $Datum);
}
echo '</tr></table>';

?>