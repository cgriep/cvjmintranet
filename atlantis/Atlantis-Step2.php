<?php
/**
 * Vor- und Nachteile festlegen
 * 2006 Christoph Griep
 */
include ('atlantis.db.php');
include ('character.class.php');
session_start();
if (!session_is_registered('Charakter'))
{
	// Fehler, kein Charakter
	header('Location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/Atlantis-Step0.php');
} else
{
	$Fehler = '';
	if (session_is_registered('Fehler') )
	{
		$Fehler = $_SESSION['Fehler'];
		session_unregister('Fehler');
	}
	$Charakter = unserialize($_SESSION['Charakter']);
	$Anhaengsel = '';
	if (isset($_REQUEST['U']))
	  $Anhaengsel = '?U=1';
	if (isset ($_REQUEST['Vorteile']) && is_numeric($_REQUEST['Vorteile']))
	{		
		$_SESSION['Fehler'] = $Charakter->fertigkeitHinzu($_REQUEST['Vorteile']);
		$_SESSION['Charakter'] = serialize($Charakter);
		header('Location: ' . $_SERVER['PHP_SELF'].$Anhaengsel);
	}
	elseif (isset ($_REQUEST['Entfernen']) && is_numeric($_REQUEST['Entfernen']))
	{
		$_SESSION['Fehler'] = $Charakter->fertigkeitEntfernen($_REQUEST['Entfernen']);
		$_SESSION['Charakter'] = serialize($Charakter);
		header('Location: ' . $_SERVER['PHP_SELF'] . $Anhaengsel);
	}
	else
	{
		$fertigkeit_output = array ();
		$fertigkeit_values = array ();
		$vfertigkeit_output = array ();
		$vfertigkeit_values = array ();
		$nfertigkeit_output = array ();
		$nfertigkeit_values = array ();
		$vnfertigkeit_output = array ();
		$vnfertigkeit_values = array ();
				
		foreach ($Charakter->Fertigkeiten as $key => $row)
		{
				if ($row['Art'] == Charakter::VORTEIL)
				{
					$vfertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
					$vfertigkeit_values[] = $row['Fertigkeit_id'];
				} elseif ( $row['Art']==Charakter::NACHTEIL)
				{
					$vnfertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
					$vnfertigkeit_values[] = $row['Fertigkeit_id'];
				}
		}
		$v = $Charakter->bestimmeMoeglicheVorteile(Charakter::VORTEIL);
		foreach ($v as $key => $row)
			{
				$fertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
				$fertigkeit_values[] = $row['Fertigkeit_id'];
			}
		$v = $Charakter->bestimmeMoeglicheVorteile(Charakter::NACHTEIL);
		foreach ($v as $key => $row)
			{
				$nfertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
				$nfertigkeit_values[] = $row['Fertigkeit_id'];
			}

		require 'Smarty/libs/Smarty.class.php';
		$smarty = new Smarty;
		$smarty->assign('Character', $Charakter->alsFeld(false));
		$smarty->assign('PageTitle', 'Vor- und Nachteile');
		$smarty->assign('mitAjax', true);
		$smarty->assign('fertigkeit_values', $fertigkeit_values);
		$smarty->assign('fertigkeit_output', $fertigkeit_output);
		$smarty->assign('fertigkeit_selected', '');
		$smarty->assign('vfertigkeit_values', $vfertigkeit_values);
		$smarty->assign('vfertigkeit_output', $vfertigkeit_output);
		$smarty->assign('vfertigkeit_selected', '');
		$smarty->assign('nfertigkeit_values', $nfertigkeit_values);
		$smarty->assign('nfertigkeit_output', $nfertigkeit_output);
		$smarty->assign('nfertigkeit_selected', '');
		$smarty->assign('vnfertigkeit_values', $vnfertigkeit_values);
		$smarty->assign('vnfertigkeit_output', $vnfertigkeit_output);
		$smarty->assign('vnfertigkeit_selected', '');

		$smarty->assign('Vorteilpunkte', $Charakter->getVorteilspunkte(Charakter::VORTEIL));
		$smarty->assign('Nachteilpunkte', $Charakter->getVorteilspunkte(Charakter::NACHTEIL));
		$smarty->assign('Verwendbarepunkte', $Charakter->Punkte);
		$smarty->assign('FehlerHinweis', $Fehler);

		// Übernatürliche Vor- und Nachteile
		if ($Charakter->isUebernatuerlich())
		{
			$smarty->assign('Uebernatuerlich', true);
			// Uebernatuerliche Wesen
			$fertigkeit_values = array ();
			$fertigkeit_output = array ();
			$vfertigkeit_values = array ();
			$vfertigkeit_output = array ();
			$nfertigkeit_values = array ();
			$nfertigkeit_output = array ();
			$vnfertigkeit_values = array ();
			$vnfertigkeit_output = array ();
			$Fehler = '';
			foreach ($Charakter->Fertigkeiten as $key => $row)
			{
				if ($row['Art'] == Charakter::VORTEILUEBERNATUERLICH)
				{
					$vfertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
					$vfertigkeit_values[] = $row['Fertigkeit_id'];
				} elseif ( $row['Art']==Charakter::NACHTEILUEBERNATUERLICH)
				{
					$vnfertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
					$vnfertigkeit_values[] = $row['Fertigkeit_id'];
				}
			}
			$v = $Charakter->bestimmeMoeglicheVorteile(Charakter::VORTEILUEBERNATUERLICH);
			foreach ($v as $key => $row)
			{
				$fertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
				$fertigkeit_values[] = $row['Fertigkeit_id'];
			}
			$v = $Charakter->bestimmeMoeglicheVorteile(Charakter::NACHTEILUEBERNATUERLICH);
			foreach ($v as $key => $row)
			{
				$nfertigkeit_output[] = $row['Fertigkeit'] . ' (' . $row['Kosten1'] . ')';
				$nfertigkeit_values[] = $row['Fertigkeit_id'];
			}
			$smarty->assign('MaxUePunkte', $Charakter->getMaximalVorteilsPunkte(Charakter::VORTEILUEBERNATUERLICH));
			$smarty->assign('UeVorteilpunkte', $Charakter->getVorteilspunkte(Charakter::VORTEILUEBERNATUERLICH));
			$smarty->assign('UeNachteilpunkte', $Charakter->getVorteilspunkte(Charakter::NACHTEILUEBERNATUERLICH));
			$smarty->assign('MaximalPunkte', -($Charakter->getVorteilspunkte(Charakter::VORTEILUEBERNATUERLICH) + 
			   $Charakter->getVorteilspunkte(Charakter::NACHTEILUEBERNATUERLICH)));
			$smarty->assign('UeFehlerHinweis', $Fehler);
			$smarty->assign('ufertigkeit_values', $fertigkeit_values);
			$smarty->assign('ufertigkeit_output', $fertigkeit_output);
			$smarty->assign('ufertigkeit_selected', '');
			$smarty->assign('vufertigkeit_values', $vfertigkeit_values);
			$smarty->assign('vufertigkeit_output', $vfertigkeit_output);
			$smarty->assign('vufertigkeit_selected', '');

			$smarty->assign('nufertigkeit_values', $nfertigkeit_values);
			$smarty->assign('nufertigkeit_output', $nfertigkeit_output);
			$smarty->assign('nufertigkeit_selected', '');
			$smarty->assign('vnufertigkeit_values', $vnfertigkeit_values);
			$smarty->assign('vnufertigkeit_output', $vnfertigkeit_output);
			$smarty->assign('vnufertigkeit_selected', '');

		} // if Formular übernatürliche Vor- und Nachteile

		$smarty->display('Atlantis-Step2.tpl');
	}
}
?>
