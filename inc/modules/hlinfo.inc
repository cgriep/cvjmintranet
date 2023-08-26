<?php
	/*
		Created:	2003-04-06
		Last modified:	2003-04-10
		Author:		Jonas Lindau (jol@tjohoo.se)

		Version:	0.2
		Description:	AWF module that shows status of halflife gameserver.
	*/

	include_once(BASE_PATH.INC_PATH.'misc/jHLNet.inc');
	
	$filename = INC_PATH.DATA_PATH.'hl_'.$module['id'].".txt";
	$lines = file($filename);
	
	$obj = new jHLNet();
	$obj->serverIP = $lines[0];
	$obj->serverPort = $lines[2];
	$obj->rconPasswd = $lines[3];
	$obj->connect();
	$serverVar = $obj->getServerInfo();
	$serverPlayers = $obj->getServerPlayers();
	//if($obj->isAlive()){
	if(strlen(trim($serverVar["mapName"]))!=0) {
		echo "<b>Status:</b> <font color='green'>Online</font><br>";
		echo "<b>IP:</b> ".$lines[1].":".$obj->serverPort."<br>";
		echo "<b>Players:</b> ".$serverVar["activePlayers"]."/".$serverVar["maxPlayers"]."<br>";
		echo "<b>Map:</b> ".$serverVar["mapName"]."<br>";
		if(strcmp(trim($lines[4]), "1")==0 && $serverVar["activePlayers"] > 0) {
			echo "<hr>";
			echo "<table width='100%' border='0' cellpadding=0 cellspacing=0>";
			echo "<tr>";
			echo "<td width='80%'><b>Name</b></td>";
			echo "<td align='right' width='20%'><b>Frag</b></td>";
			echo "</tr>";
			
			for($p=0;$p<$serverVar["activePlayers"];$p++) {
				echo "<tr>";
				echo "<td>".$serverPlayers[$p]["name"]."&nbsp;&nbsp;</td>";
				echo "<td align='right'>".$serverPlayers[$p]["frag"]."</td>";
				echo "</tr>";
			}
			echo "</table>";
		}
	}else{
		echo "<b>Status:</b> <font color='red'>Offline</font><br>";
	}
	$obj->close();
?>
