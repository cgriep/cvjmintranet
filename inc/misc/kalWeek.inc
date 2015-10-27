<table WIDTH="100%" BORDER="0" CELLSPACING="<?echo$cs?>"
	CELLPADDING="<?echo$cp?>">
	<tr>
		<td CLASS="monthyear" <?if($mtc) {echo ' BGCOLOR="#'.$mtc.'"';}?>>
		<div ALIGN="center"><?echo $mn;?></div>
		</td>
	</tr>
</table>

<table align="center" WIDTH="100%" BORDER="0" CELLSPACING="<?echo$cs?>"
	CELLPADDING="<?echo$cp?>">

	<tr ALIGN="center" HEIGHT="<?echo$fsd+$cs+$cp?>" CLASS="daynames"
	<?if($dtc)
	echo ' BGCOLOR="#'.$dtc.'"';
	else
	echo ' BGCOLOR="'.$design['table_header'].'"'; ?>>
	<?
	// Wochenanfang feststellen
	$sd = date ('w', mktime(0,1,0,$Monat,$Tag-$document['Erstertag'],$Jahr));
	$sd = ($AnzTage-1) / 2;
	$Anfang = strtotime("-$sd days", mktime(0,0,0,$Monat,$Tag,$Jahr));
	$endday = -$sd+$AnzTage;
	$Ende = strtotime("+$endday days", mktime(23,59,59,$Monat,$Tag,$Jahr));
	// Termine suchen
	$search = "(name='Datum' AND ";
	$search .= "value BETWEEN '" . $Anfang ."'";
	$search .= " AND '".$Ende."')";
	$search .= " OR (name='Enddatum' AND ";
	$search .= "value BETWEEN '".$Anfang."'";
	$search .= " AND '".$Ende."')";
	unset($Termine);
	if ( $document['AlleAnzeigen'] == '1' )
	$Termine = search_nodes($search,1000);
	else
	$Termine = search_children ($search,1000);
	$search = "name='Datum' AND ";
	$search .= "value < '".$Anfang."'";
	if ( $document['AlleAnzeigen'] == '1' )
	$fTermine = search_nodes($search,1000);
	else
	$fTermine = search_children ($search,1000);
	while ( list($key, $value) = each($fTermine) )
	{
		if ( $value['Enddatum'] > $Ende )
		$Termine[$key] = $value;
	}
	unset($fTermine);
	$search = "name='Enddatum' AND ";
	$search .= "value > '" .$Ende."'";
	if ( $document['AlleAnzeigen'] == "1" )
	$fTermine = search_nodes($search,1000);
	else
	$fTermine = search_children ($search,1000);
	while ( list($key, $value) = each($fTermine) )
	{
		if ( $value['Datum'] < $Anfang )
		$Termine[$key] = $value;
	}
	unset($fTermine);
	$search = "name='Turnus' AND ";
	$search .= "value <> ''";
	if ( $document['AlleAnzeigen'] == "1" )
	$fTermine = search_nodes($search,1000);
	else
	$fTermine = search_children ($search,1000);
	foreach ( $fTermine as $key => $value)
	{
		switch ($value['Turnus']) {
			case 'd':
				$ewota = $Anfang;
				while ( $ewota < $Ende ) {
					if ( ($value['TurnusEnde'] == 0 || $value['TurnusEnde'] >= mktime(0,0,0,date('m',$ewota), date('d', $ewota), $Jahr))
					&& ($value['Datum'] <= mktime(0,0,0,date('m',$ewota), date('d', $ewota), $Jahr))
					) {
						$Termine[$key."&$ewota"] = $value;
						$Termine[$key."&$ewota"]['Datum'] = mktime(date('H',$value['Datum']),
						date('i',$value['Datum']), date('s',$value['Datum']),
						date('m',$ewota),date('d',$ewota),date('Y',$ewota));
						$Termine[$key."&$ewota"]['Enddatum'] = mktime(date('H',$value['Enddatum']),
						date('i',$value['Enddatum']), date('s',$value['Enddatum']),
						date('m',$ewota),date('d',$ewota),date('Y',$ewota));
					}
					$ewota = strtotime('+1 day', $ewota);
				}
				break;
			case 'w':
				$ewota = $Anfang;
				while ( $ewota < $Ende ) {
					if ( date('w', $ewota) == date('w', $value['Datum']) &&
					($value['TurnusEnde'] == 0 ||
					$value['TurnusEnde'] >= mktime(0,0,0,date('m',$ewota), date('d', $ewota), $Jahr))
					&& $value['Datum'] <= mktime(0,0,0,date('m',$ewota),date('d',$ewota), $Jahr)
					) {
						$Termine[$key.'&'.date('w', $ewota)] = $value;
						$Termine[$key.'&'.date('w', $ewota)]['Datum'] = mktime(date('H',$value['Datum']),
						date('i',$value['Datum']), date('s',$value['Datum']),
						date('m',$ewota),date('d',$ewota),date('Y',$ewota));
						$Termine[$key.'&'.date('w', $ewota)]['Enddatum'] = mktime(date('H',$value['Enddatum']),
						date('i',$value['Enddatum']), date('s',$value['Enddatum']),
						date('m',$ewota),date('d',$ewota),date('Y',$ewota));
					}
					$ewota = strtotime('+1 day', $ewota);
				}
				break;
			case 'v':
				$ewota = $Anfang;
				while ( $ewota < $Ende ) {
					if ( date('w', $ewota) == date('w', $value['Datum']) &&
					($value['TurnusEnde'] == 0 || $value['TurnusEnde'] >= mktime(0,0,0,date('m',$ewota),
					date('d', $ewota), $Jahr))) {
						$unter = ((mktime(date('H',$value['Datum']), date('i',$value['Datum']),0,
						date('m',$ewota),date('d',$ewota),date('Y',$ewota))-$value['Datum'])/86400);
						if ( ($unter %14) == 0 ) {
							$Termine[$key.'&'.date('w', $ewota)] = $value;
							$Termine[$key.'&'.date('w', $ewota)]['Datum'] = mktime(date('H',$value['Datum']),
							date('i',$value['Datum']), date('s',$value['Datum']),
							date('m',$ewota),date('d',$ewota),date('Y',$ewota));
							$Termine[$key.'&'.date('w', $ewota)]['Enddatum'] = mktime(date('H',$value['Enddatum']),
							date('i',$value['Enddatum']), date('s',$value['Enddatum']),
							date('m',$ewota),date('d',$ewota),date('Y',$ewota));
						}
					}
					$ewota = strtotime('+1 day', $ewota);
				}
				break;
			case 'm':
				if ( date('d',$value['Datum']) >= $Tag && date('d',$value['Datum']) <= $Tag + $endday &&
				$value['Datum'] <= mktime(0,0,0,$Monat, date('d',$value['Datum']), $Jahr) &&
				($value['TurnusEnde'] == 0 ||
				$value['TurnusEnde'] >= mktime(0,0,0,$Monat, date('d',$value['Datum']),$Jahr)) ) {
					$Termine[$key] = $value;
					$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
					date('i',$value['Datum']), date('s',$value['Datum']),
					$Monat, date('d',$value['Datum']), $Jahr);
					$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
					date('i',$value['Enddatum']), date('s',$value['Enddatum']),
					$Monat, date('d',$value['Enddatum']), $Jahr);
				}
				break;
			case 'j':
				$monatstamp1 = mktime(0,0,0,date('m',$value['Datum']),date('d',$value['Datum']),
				$Jahr);
				$monatstamp2 = mktime(0,0,0,date('m',$value['Datum']),date('d',$value['Datum']),
				$Jahr+1);
				if ( $monatstamp1 >= $Anfang && ($monatstamp1 <= $Ende || $monatstamp2 <= $Ende) &&
				$value['Datum'] <= mktime(0,0,0,date('m',$value['Datum']),date('d',$value['Datum']), $Jahr) &&
				($value['TurnusEnde'] == 0 ||
				$value['TurnusEnde'] >= mktime(0,0,0,date('m',$value['Datum']), date('d',$value['Datum']),$Jahr)) ) {
					$Termine[$key] = $value;
					if ( $monatstamp2 <= $Ende ) {
						$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
						date('i',$value['Datum']), date('s',$value['Datum']),
						$Monat, date('d',$value['Datum']), $Jahr+1);
						$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
						date('i',$value['Enddatum']), date('s',$value['Enddatum']),
						$Monat, date('d',$value['Enddatum']), $Jahr+1);
					}
					else {
						$Termine[$key]['Datum'] = mktime(date('H',$value['Datum']),
						date('i',$value['Datum']), date('s',$value['Datum']),
						$Monat, date('d',$value['Datum']), $Jahr);
						$Termine[$key]['Enddatum'] = mktime(date('H',$value['Enddatum']),
						date('i',$value['Enddatum']), date('s',$value['Enddatum']),
						$Monat, date('d',$value['Enddatum']), $Jahr);
					}
				}
				break;
			default:
		}
	}
	unset($fTermine);
	if ( ! $Termine )
	unset($Termine);
	else
	$Termine = sortiereTermine($Termine);
	echo '<td>Zeit</td>';
	// Tages&uuml;berschriften anzeigen
	for ($I=0;$I<$AnzTage;$I++) {
		//$dayprint=$document['Erstertag']+$I;
		$dayprint=date('w',$Anfang)+$I;
		if ($dayprint>6) $dayprint=$dayprint-7;
		echo ' <td width="'.(100/($AnzTage+1)).'%"';
		//$anz = (-$sd)+$I;
		$datum = strtotime('+'.$I.' days', $Anfang);//mktime(0,1,0,$Monat,$Tag,$Jahr));
		$Wochenende = date('w', $datum);
		if ( $Wochenende == 0 || $Wochenende == 6)
		echo ' bgcolor="#'.$document['WEBackgroundcolor'].'"';
		echo '>'.$day[$dayprint].'<br />';
		// Datum des Tages anzeigen
		echo date('d',$datum);
		echo '</td>';
	}
	echo '</tr>';
	$zeit = strtotime('8:00');
	$diff = $document['Abstand'];
	if ( isset($_REQUEST['Diff']) && is_numeric($_REQUEST['Diff']) )
	$diff = $_REQUEST['Abstand'];
	if ( ! is_numeric($diff)) $diff = 30;
	while ( $zeit < strtotime('23:59') ) {
		echo '<tr><td align="center" bgcolor="#'.$document['Backgroundcolor'].'">';
		echo "&nbsp;<br />".date('H:i',$zeit) . '<br />&nbsp;</td>';
		for ($I=0;$I<$AnzTage;$I++) {
			$wann = -$sd+$I;
			$wann = strtotime($wann.' days', mktime(date('H',$zeit),date('i',$zeit),0,
			$Monat,$Tag,$Jahr));
			$Wochenende = date('w', $wann);
			echo '<td valign="top" bgcolor="#';
			if ( $Wochenende == 0 || $Wochenende == 6)
			echo $document['WEBackgroundcolor'];
			else
			echo $document['Backgroundcolor'];
			echo '">';
			//if ( $profile[$document["Terminrecht"]]=="1") {
			echo '<a href="'.get_url($id,array('Zeit'=>$wann, 'Termin'=>1));
			echo '"><img src="img/newtermin.gif" border="0"></a><br /> ';
			//}
			$Anz = 0;
			$nextzeit = strtotime("+$diff minutes", $wann);
			foreach ($Termine as $key => $value)  {
				if ( ($value['Datum'] >= $wann && $value['Datum'] < $nextzeit) ||
				( $value['Datum'] <= $wann && $value['Enddatum'] >= $wann )
				) {
					$nextd = -$sd+$I+1;
					if ( $value['Ok'.$I] != strtotime($nextd.' days', mktime(0,0,0,$Monat,$Tag,$Jahr)) )
					{
						if ( $Anz != 0 ) { echo "<br />";  }
						echo '<span ';
						if (InGruppe($value['Gruppe']))
						{
							echo 'style="background-color:'.$document['Gruppencolor'].'"';
						}
						else
						{
							if (InPerson($value['Person']) )
							{
								echo 'style="background-color:'.$document['Personcolor'].'"';
							}
						}
						echo '>';
						$Anz++;
						echo '<a href="'.get_url($value['id']).
						'" title="'.get_user_nickname($value['author_id']).': ';
						if ( is_numeric($value['Enddatum']) )
						{
							echo 'bis '.date('d.m.Y H:i',$value['Enddatum']);
						}
						if ( strtotime("+$nextd days", mktime(0,0,0,$Monat,$Tag,$Jahr)) < $value['Enddatum'] )
						{
							echo ' (ganztägig!)';
							$Termine[$key]['Ok'.$I] = strtotime($nextd.' days', mktime(0,0,0,$Monat,$Tag,$Jahr));
						}
						echo '">'.transform($value['title'],'clean').'</a>';
						echo '</span>';
					}
				}
			}
			echo '&nbsp;</td>';
		}
		$zeit = strtotime("+$diff minutes", $zeit);
		echo '</tr>';
		$anz++;
	}

	?>

</table>
	<?php
	echo '<b>'.count($Termine).' Termine</b><br />';
	?>