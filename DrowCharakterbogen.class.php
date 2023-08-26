<?php


/*
 * Created on 09.12.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
session_start();
require ('fpdf/fpdf.php');

include ('inc/functions.inc');
include ('inc/licence.key');
include ('inc/sessions.inc');
include ('inc/caching.inc');
include ('inc/config.inc');
include ('inc/misc/drow.inc');

class Charakterbogen extends FPDF
{
	function Header()
	{
		//Logo
		//$this->Image('Atlantisbanner.jpg', 30, 0, 0, 25);
		$this->Image('Malylenebanner.jpg', 20, 5, 180, 20);
		//Arial bold 15
		$this->SetFont('Arial', 'B', 14);
		//$this->setX(80);
		//$this->Write(10, 'Mal\'ylene-Charakterbogen');
		//Line break
		$this->Ln(20);
	}
	function trimString($string, $width)
	{
		if ($width == 0)
		$width = $this->w - $this->rMargin - $this->x;
		$wmax = ($width -2 * $this->cMargin);
		$teile = explode(' ', $string);
		$alles = array ();
		foreach ($teile as $teil)
		{
			$t = explode("\n", $teil);
			for ($i = 0; $i < Count($t) - 1; $i++)
			$alles[] = $t[$i] . "\n";
			// Letzten Eintrag ohne newline anh�ngen
			$alles[] = $t[$i];
		}
		$gesamt = '';
		$s = '';
		foreach ($alles as $teil)
		{
			if ($this->getStringWidth($s . ' ' . $teil) > $wmax || substr($teil, -1, 1) == "\n")
			{
				if ($s == '')
				{
					$gesamt .= $teil;
				} else
				{
					$gesamt .= $s;
					$s = $teil;
				}
				if ((substr($gesamt, -1, 1) != "\n"))
				$gesamt .= "\n";
			} else
			{
				if ($s != '')
				$s .= ' ';
				$s .= $teil;
			}
		}
		if ($s != '')
		{
			$gesamt .= $s;
		}
		return $gesamt;
	}
	//Page footer
	function Footer()
	{
		//Position at 1.5 cm from bottom
		$this->SetY(-15);
		//Arial italic 8
		$this->SetFont('Arial', '', 6);
		//Page number
		$this->Cell(0, 5, 'Drow-Con-SL, erstellt ' . date('d.m.Y H:i'));
		$this->Ln();
		$this->SetFont('Arial', 'I', 8);
		$this->Cell(0, 5, 'Seite ' . $this->PageNo() . '/{nb}', 0, 0, 'C');
	}
}

function erstelleSeite($charid, $pdf)
{
	// node zum Charakter feststellen wegen Bild
	$sql = 'SELECT node_id FROM drowcon_nodedata WHERE name=\'Charakter_id\' AND value=\''.$charid.'\'';
	$query = sql_query($sql);
	if ( $row = sql_fetch_row($query))
	{
		$id = $row[0];
	}
	else
	{
		throw new error("Keine Charakterseite f�r diesen Charakter gefunden!");
	}
	sql_free_result($query);
	$charakter = get_charakter($charid);
	$pdf->AddPage();
	$pdf->SetFont('Arial', 'B', 16);
	$pdf->Cell(100, 7, $charakter['Name']);
	$pdf->Cell(80, 7, $charakter['Hausname'],0,0,'R');
	$bbasedir = get_dir_name('files',$id).'/published/';
	if ( file_exists($bbasedir.'Spielerbild.jpg') ) {
		$pdf->Image($bbasedir.'Spielerbild.jpg', 180, 38, 20, 30, 'JPG');
	}
	// Rahmen f�r das Bild
	$pdf->Rect(180, 38, 20, 30);
	$pdf->SetFont('Arial', '', 10);
	$pdf->Ln();
	if ( $charakter['Charstatus'] != '') 
	{
		$pdf->Write(10, $charakter['Charstatus']);
		$pdf->Ln();
	}
	$pdf->Cell(25, 5, 'Realname:');
	$pdf->Cell(60, 5, $charakter['Spielername']);
	$pdf->Cell(35, 5, 'EP aus Contagen:');
	$pdf->Cell(40, 5, $charakter['Contage'].' ('.number_format($charakter['Contage']/30,1).' Spieltage)');
	$pdf->Ln();
	$pdf->Cell(25, 5, 'Position:');
	$pdf->Cell(60, 5, $charakter['Position']);
	$pdf->Cell(35, 5, 'EP gesamt:');
	$pdf->Cell(10, 5, bestimmeEP($charakter['Charakter_id']));
	$pdf->Ln();
	$pdf->Cell(25, 5, 'Klasse:');
	$pdf->Cell(60, 5, $charakter['Klasse']);
	$pdf->Cell(35, 5, 'EP frei:');
	$pdf->Cell(10, 5, $charakter['EP']);
	$pdf->Ln();
	$pdf->Cell(25, 5, 'Gunststufe:');
	$pdf->Cell(60, 5, $charakter['Gunststufe']);
	$pdf->Cell(35, 5, 'Gunstpunkte:');
	$s = $charakter['Gunstpunkte'];
	if ( $charakter['Gunstpunkte_Temp'] != 0)
	{
		$s .= ' (zzgl.'.$charakter['Gunstpunkte_Temp'].' tmp)';
	}
	$pdf->Cell(10, 5, $s);
	$pdf->Ln();
	$pdf->Cell(25, 5, 'Rasse:');
	$pdf->Cell(60, 5, $charakter['Rasse']);
	$pdf->Cell(35, 5, 'Gunstpunkte Status:');
	$Gunst = bestimmeCharGunst($charakter['Charakter_id']);
	$pdf->Cell(10, 5, abs($Gunst-$charakter['Gunstpunkte']-$charakter['Gunstpunkte_Temp']));
	$pdf->Ln();
	$pdf->Cell(25, 5, 'Adlig/Gemein:');
	if ( $charakter['Adlig'])
	{
		$pdf->Cell(60, 5, 'Adlig');
	}
	else
	{
		$pdf->Cell(60, 5, 'Gemein');
	}
	$pdf->Cell(35, 5, 'Gunstpunkte Gesamt:');
	$pdf->Cell(10, 5, $Gunst);
	$pdf->Ln();
	$sql = 'SELECT * FROM T_Eigenschaften INNER JOIN T_Charakter_Eigenschaften ON F_Eigenschaft_id=Eigenschaft_id '.
	'WHERE F_Charakter_id='.$charakter['Charakter_id'].' ORDER BY Art, Eigenschaftsname';
	$query = sql_query($sql);
	$summe = 0;
	$Art = '';
	$Nr = 0;
	$Anz = 0;
	$Punkte = 0;
	$Eigenschaft = '';
	while ( $row = sql_fetch_array($query))
	{
		if ( $Art != $row['Art'])
		{
			// pr�fen ob stapel 
			if ( $Eigenschaft != '' )
			{
				$pdf->Cell(10, 5, $Anz,1 );
				$pdf->Cell(70, 5, $Eigenschaft,1);
				$pdf->Cell(10, 5, $Punkte,1);
				$Nr++;
				if ( $Nr % 2 == 0)
				{
					$pdf->Ln();
				}
				$Eigenschaft = '';
			}	
			$Art = $row['Art'];
			if ( $Nr % 2 != 0)
			{
				$pdf->Ln();
			}
			$Nr = 0;
			$pdf->Ln();
			$pdf->SetFont('Arial', 'B', 10);
			$pdf->Cell(60,5, $Art);
			$pdf->Ln();
			$pdf->SetFont('Arial', '', 10);
		}
		if ( $Eigenschaft != $row['Eigenschaftsname'] && $row['Stapelbar'] == 1)
		{
			if ( $Eigenschaft != '' )
			{
				$pdf->Cell(10, 5, $Anz,1);
				$pdf->Cell(70, 5, $Eigenschaft,1);
				$pdf->Cell(10, 5, $Punkte,1);
				$Nr++;
				if ( $Nr % 2 == 0)
				{
					$pdf->Ln();
				}
			}
			$Eigenschaft = $row['Eigenschaftsname'];
			$Anz = 1;
			if ( $row['Erfahungspunkte'] != 0)
			{
				$Punkte = $row['Erfahrungspunkte'];
			}
			else
			{
				$Punkte = '';
			}
		}
		elseif ( $row['Stapelbar'] == 1 )
		{
			$Anz++;
			if ( $Punkte != '' ) $Punkte += $row['Erfahrungspunkte'];
		}
		else
		{
			// pr�fen Stapel 
			if ( $Eigenschaft != '' )
			{
				$pdf->Cell(10, 5, $Anz,1);
				$pdf->Cell(70, 5, $Eigenschaft,1);
				$pdf->Cell(10, 5, $Punkte,1);
				$Nr++;
				if ( $Nr % 2 == 0)
				{
					$pdf->Ln();
				}
				$Eigenschaft = '';
			}
			if ( $row['Erfahrungspunkte'] != 0 )
			{
				$pdf->Cell(10, 5, '',1);
			}
			else
			{
				$pdf->Cell(10, 5, date('m/y',strtotime($row['Erstellt'])),1);
			}
			$pdf->Cell(70, 5, $row['Eigenschaftsname'],1);
			$pdf->Cell(10, 5, $row['Erfahrungspunkte'],1);
			$Nr++;
			if ( $Nr % 2 == 0)
			{
				$pdf->Ln();
			}
		}
	}
	if ( $Eigenschaft != '' )
	{
		$pdf->Cell(10, 5, $Anz,1);
		$pdf->Cell(70, 5, $Eigenschaft,1);
		$pdf->Cell(10, 5, $Punkte,1);
		$Nr++;
		if ( $Nr % 2 == 0)
		{
			$pdf->Ln();
		}
	}
	sql_free_result($query);
	
	// Con-Teilnahmen
	$pdf->Ln();
	$pdf->Ln();
	$pdf->SetFont('Arial', 'B', 10);
	$pdf->Cell(60,5, 'Kampagnenteilnahmen');
	$pdf->SetFont('Arial', '', 10);
	$pdf->Ln();
	$daten = get_profile($charakter['F_Spieler_id'], false);
	foreach ( $daten as $key => $value)
	{
		if ( $value == $id )
		{
			$veranstaltung = substr($key, 4);
			if ( is_numeric($veranstaltung))
			{
				// Achtung: Flavour = 1 - > Standardsprache, ggf. �ndern wenn andere genutzt
				$node = get_nodedata($veranstaltung, 1);
				$pdf->Cell(80, 5, $node['title']);
				$pdf->Ln();
			}
		}
	}
	return $pdf;
}


if ((is_numeric($_REQUEST['Charakter_id']) || is_numeric($_REQUEST['Veranstaltung'])) 
&& session_is_registered('SL') || 
session_is_registered('Charakter_id') && session_is_registered('id') )
{
	$charid = array();
	// Datenbank �ffnen 
	if ( is_numeric($_REQUEST['Charakter_id']) && session_is_registered('SL'))
	{
		$charid[] = $_REQUEST['Charakter_id'];
	}
	elseif (  is_numeric($_REQUEST['Veranstaltung']) && session_is_registered('SL'))
	{
		// alle Charaktere einer Veranstaltung 
		$sql = 'SELECT n1.value FROM drowcon_userdata u1 
				INNER JOIN drowcon_nodedata n1 ON u1.value=n1.node_id 
				INNER JOIN T_Charaktere ON Charakter_id=n1.value
				WHERE u1.name=\'Char'.$_REQUEST['Veranstaltung'].'\' AND n1.name=\'Charakter_id\'
				ORDER BY T_Charaktere.Name';
		if (! $query = sql_query($sql) ) throw "Fehler: ".sql_error();
		while ( $row = sql_fetch_row($query))
		{
			$charid[] = $row[0];			
		}
		sql_free_result($query);
	}
	else
	{
		$charid[] = $_SESSION['Charakter_id'];
	}
	$pdf = new Charakterbogen();
	$pdf->AliasNbPages();
	$pdf->setLeftMargin(20);
	$pdf->setAuthor('C. Griep');
	$pdf->setTitle('Charakterbogen Drow-Con');
	$pdf->setCreator('DrowCon-SL');
	$pdf->setSubject('Charakterbogen Drow-Con ' .implode(',',$charid));
	foreach ($charid as $charakterid)
	{
		$pdf = erstelleSeite($charakterid, $pdf);
	}
	$pdf->Output();
} else
{
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/index.php');
} 
?>