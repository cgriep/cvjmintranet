<?php


/*
 * Created on 09.12.2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
session_start();
require ('fpdf/fpdf.php');
include ('atlantis.db.php');
include ('character.class.php');

class Charakterbogen extends FPDF
{
	function Header()
	{
		//Logo
		//$this->Image('Atlantisbanner.jpg', 30, 0, 0, 25);
		$this->Image('atlantislogo001.jpg', 20, 0);
		//Arial bold 15
		$this->SetFont('Arial', 'B', 14);
		$this->setX(80);
		$this->Write(10, 'Atlantis-Charakterbogen');
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
			// Letzten Eintrag ohne newline anhängen
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
		$this->Cell(0, 5, 'Atlantis-SL, erstellt ' . date('d.m.Y H:i'));
		$this->Ln();
		$this->SetFont('Arial', 'I', 8);
		$this->Cell(0, 5, 'Seite ' . $this->PageNo() . '/{nb}', 0, 0, 'C');
	}
}

if (session_is_registered('Charakter_id'))
{
	$char = new Charakter();
	if ($char->loadFromDatabase($_SESSION['Charakter_id']))
	{
		$pdf = new Charakterbogen();
		$pdf->AliasNbPages();
		$pdf->setLeftMargin(20);
		$pdf->setAuthor('C. Griep');
		$pdf->setTitle('Charakterbogen ' . $char->Charaktername);
		$pdf->setCreator('Atlantis-SL');
		$pdf->setSubject('Charakterbogen ' . $char->Charaktername . ' (' .
		$char->Charakter_id . ')');
		$pdf->AddPage();
		$pdf->SetFont('Arial', 'B', 16);
		$pdf->Write(5, $char->Charaktername);
		if ($char->Bild != '')
		{
			// temporäre Datei anlegen
			$Dateiname = tempnam('', 'char' . $_SESSION['Charakter_id']);
			$file = fopen($Dateiname, 'w');
			fwrite($file, $char->Bild);
			fclose($file);
			$pdf->Image($Dateiname, 150, 30, 50, 60, 'JPG');
			unlink($Dateiname);
		}
		// Rahmen für das Bild
		$pdf->Rect(150, 30, 50, 60);
		$pdf->SetFont('Arial', '', 10);
		$pdf->Ln();
		$pdf->Cell(25, 5, 'Rasse:');
		$pdf->Cell(30, 5, $char->getRasse());
		if ($char->isMagisch())
		{
			$pdf->Cell(30, 5, 'Klasse:');
			$pdf->Cell(30, 5, $char->getKlasse());
			$s = $char->getSpezialisierung();
			if ($s != '')
			{
				$pdf->Cell(30, 5, 'Ausrichtung:');
				$pdf->Cell(0, 5, $s);
			}
		}
		$pdf->Ln();		
		$pdf->Cell(25, 5, 'Contage:');
		$pdf->Cell(30, 5, $char->Contage);
		$pdf->Cell(30, 5, 'Atlantis-Contage:');
		$pdf->Cell(0, 5, $char->Atlantiscontage);
		$pdf->Ln();
		$pdf->Cell(25, 5, 'Freie Punkte:');
		$punkte = $char->Punkte;
		if ( $char->isUebernatuerlich())
		  $punkte .= ' (ÜV:'.$char->getAktivierungspunkte().')';
		$pdf->Cell(30, 5, $punkte);
		if ($char->isMagisch())
		{
			$pdf->Cell(30, 5, 'Magiepunkte:');
			$pdf->Cell(0, 5, $char->getMagiepunkte());
		}
		$pdf->Ln();
		// Spruchlisten
		if (Count($char->Spruchlisten) > 0)
		{
			$pdf->SetFont('Arial', 'B', 10);
			$pdf->Cell(0, 5, 'Spruchlisten');
			$pdf->Ln();
			$pdf->SetFont('Arial', '', 10);
			$da = false;
			foreach ($char->Spruchlisten as $sl)
			{
				if ($da)
					$pdf->Write(5, ', ');
				$da = true;
				$pdf->Write(5, $sl['Spezialisierung']);
			}
		}
		$pdf->Ln();
		$s = '';
		$pdf->SetFont('Arial', 'B', 10);
		$pdf->Cell(0, 4, 'Spezialisierungen und Ränge');
		$pdf->Ln();
		$pdf->SetFont('Arial', '', 6);
		$anz = 0;
		foreach ($char->Spezialisierungen as $klasseid => $spez)
		{
			foreach ($spez as $spezid => $wert)
			{
				$max = 0;
				foreach ($char->RangErlaubt[$spezid] as $key => $value)
				{
					if ($value)
					{
						$max = $key;
					}
				}
				if ($max > 0)
				{
					$s = holeSpezialisierungsNamen($spezid) . ' - ';
					switch ($max)
					{
						case 1 :
							$s .= 'Wanderer';
							break;
						case 2 :
							$s .= 'Adept';
							break;
						case 3 :
							$s .= 'Meister';
							break;
						case 4 :
							$s .= 'Großmeister';
							break;
					}
					$pdf->Cell(40, 3, $s);
					$anz++;
					if ($anz % 3 == 0)
						$pdf->Ln();
				}
			}
		}
		//$pdf->MultiCell(0,3,$char->Historie);
		// Tabellen mit Vor- und Nachteilen
		$pdf->setXY(20, 95);
		$pdf->SetFillColor(255, 0, 0);
		$pdf->SetTextColor(255);
		$pdf->SetDrawColor(128, 0, 0);
		$pdf->SetLineWidth(.3);

		$spaltenbreiten = array (
			70,
			5,
			105
		);
		if (Count($char->Fertigkeiten) > 0)
		{
			$pdf->SetFont('Arial', '', 10);
			$pdf->Cell($spaltenbreiten[0], 5, 'Vor-/Nachteil', 1, 0, 'C', 1);
			$pdf->Cell($spaltenbreiten[1], 5, 'St', 1, 0, 'C', 1);
			$pdf->Cell($spaltenbreiten[2], 5, 'Beschreibung', 1, 0, 'C', 1);
			$pdf->Ln();
			$fill = false;
			$pdf->SetFont('Arial', '', 10);
			$pdf->SetFillColor(224, 235, 255);
			$pdf->SetTextColor(0);
			foreach ($char->Fertigkeiten as $key => $value)
			{
				if ($value['Art'] == Charakter::VORTEIL || $value['Art'] == Charakter::NACHTEIL)
				{
					//Höhe berechnen
					$pdf->SetFont('Arial', '', 6);
					$nb = $pdf->trimString($value['Beschreibung'], $spaltenbreiten[2]);
					$zeilen = Count(explode("\n", $nb));
					if ($zeilen % 2 == 1)
						$nb .= "\n ";
					$zeilen = ceil($zeilen / 2);
					$pdf->SetFont('Arial', '', 8);
					$pdf->Cell($spaltenbreiten[0], $zeilen * 5, $value['Fertigkeit'], 'LR', 0, 'L', $fill);
					$pdf->SetFont('Arial', '', 6);
					$pdf->Cell($spaltenbreiten[1], $zeilen * 5, $value['Stufe'], 'LR', 0, 'L', $fill);
					$pdf->MultiCell($spaltenbreiten[2], 2.5, $nb, 'LR', 'L', $fill);
					$fill = !$fill;
				}
			}
			$pdf->Cell(180, 0, '', 'T');
			$pdf->Ln();
			// Übernatürliche Vor- und Nachteile
			foreach ($char->Fertigkeiten as $key => $value)
			{
				if ($value['Art'] == Charakter::VORTEILUEBERNATUERLICH 
				|| $value['Art'] == Charakter::NACHTEILUEBERNATUERLICH)
				{
					$pdf->SetFont('Arial', '', 6);
					$nb = $pdf->trimString($value['Beschreibung'], $spaltenbreiten[2]);
					$zeilen = Count(explode("\n", $nb));
					if ($zeilen % 2 == 1)
						$nb .= "\n ";
					$zeilen = ceil($zeilen / 2);
					if ($value['Stufe'] > 0)
					{
						$pdf->SetFont('Arial', '', 8);
						$pdf->Cell($spaltenbreiten[0], $zeilen * 5, $value['Fertigkeit'], 'LR', 0, 'L', $fill);
					} else
					{
						$pdf->SetFont('Arial', 'I', 8);
						$pdf->Cell($spaltenbreiten[0], $zeilen * 5, $value['Fertigkeit'] . ' (n/a, '.
                          $value['Kosten1'].')', 'LR', 0, 'L', $fill);
					}
					$pdf->SetFont('Arial', '', 6);
					$pdf->Cell($spaltenbreiten[1], $zeilen * 5, $value['Stufe'], 'LR', 0, 'L', $fill);
					$pdf->MultiCell($spaltenbreiten[2], 2.5, $nb, 'LR', 'L', $fill);
					$fill = !$fill;
				}
			}
			$pdf->Cell(180, 0, '', 'T');
			$pdf->Ln(8);
		}
		$pdf->SetFont('Arial', 'B', 10);
		$pdf->SetFillColor(255, 0, 0);
		$pdf->SetTextColor(255);
		$pdf->SetDrawColor(128, 0, 0);
		$pdf->SetLineWidth(.3);
		$pdf->Cell(70, 5, 'Fertigkeit', 1, 0, 'C', 1);
		$pdf->Cell(5, 5, 'S', 1, 0, 'C', 1);
		$pdf->Cell(30, 5, 'Spezialisierung', 1, 0, 'C', 1);
		$pdf->Cell(20, 5, 'Rang', 1, 0, 'C', 1);
		$pdf->Cell(5, 5, 'K', 1, 0, 'C', 1);
		$pdf->Cell(50, 5, 'Beschreibung', 1, 0, 'C', 1);
		$pdf->Ln();
		//Color and font restoration
		$pdf->SetFillColor(224, 235, 255);
		$pdf->SetTextColor(0);
		
		$fill = false;
		foreach ($char->Fertigkeiten as $key => $value)
		{
			if ($value['Art'] == Charakter::FERTIGKEIT )
			{
				$pdf->SetFont('Arial', '', 8);
				$pdf->Cell(70, 4, $value['Fertigkeit'], 'LR', 0, 'L', $fill);
				$pdf->SetFont('Arial', '', 6);
				$pdf->Cell(5, 4, $value['Stufe'], 'LR', 0, 'L', $fill);
				$pdf->Cell(30, 4, $value['Spezialisierung'], 'LR', 0, 'L', $fill);
				$pdf->Cell(20, 4, $value['Rang'], 'LR', 0, 'L', $fill);
				$pdf->Cell(5, 4, $value['Kosten'], 'LR', 0, 'C', $fill);
				$pdf->SetFont('Arial', '', 4);
				$pdf->Cell(50, 4, $value['Beschreibung'], 'LR', 0, 'C', $fill);
				$pdf->Ln();
				$fill = !$fill;
			}
		}
		$pdf->Cell(180, 0, '', 'T');
		$anz =0;
		foreach ($char->Fertigkeiten as $key => $value)
		{
			if ($value['Art'] == Charakter::MAGIE )
			{
				$pdf->Cell(80, 4, $value['Fertigkeit'], 'LR', 0, 'L', $fill);
				$pdf->Cell(10, 4, $value['Stufe'], 'LR', 0, 'L', $fill);
				$pdf->Cell(40, 4, $value['Spezialisierung'], 'LR', 0, 'L', $fill);
				$pdf->Cell(30, 4, $value['Rang'], 'LR', 0, 'L', $fill);
				$pdf->Cell(20, 4, $value['Kosten'], 'LR', 0, 'C', $fill);
				$pdf->Ln();
				$fill = !$fill;
				$anz++;
			}
		}
		if ( $anz > 0 )
		{
		$pdf->Cell(180, 0, '', 'T');
		}
		
		$pdf->Output();
	} else
		header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Auswahl.php');
} else
{
	// Fehler - zurück zur Charakterauswahl
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Auswahl.php');
}
?>