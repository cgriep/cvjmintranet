<?php
	/*
		Class:		jHLNet
		Created:	2003-04-06
		Last modified:	2003-04-09
		Author:		Jonas Lindau (jol@tjohoo.se)

		Version:	0.2
		Description:	This class makes it easier to communicate with Halflife game servers
				Used in hlinfo.inc and hlrcon.inc (under developement)
	*/

	class jHLNet {
		var $serverIP;
		var $serverPort;
		var $rconPasswd;
		
		var $fp;
		
		function connect() {
			$this->fp = fsockopen("udp://".$this->serverIP, $this->serverPort, $errNo, $errStr, 10);
			socket_set_timeout($this->fp, 2);
		}
		function close() {
			fclose($this->fp);
		}
		function sendRcon($command) {
			$buffer = $this->send("challenge rcon");
			if(strlen($buffer)>15) {
				$buffer = substr($buffer, 15,strpos($buffer,"\x00")-16);
				$buffer = $this->send("rcon $buffer \"$this->rconPasswd\" $command");

				return substr($buffer,1);
			}
			
		}
		function send($command) {
			socket_set_timeout($this->fp,10);
			fputs($this->fp, "\xff\xff\xff\xff$command\x00");
			$buffer = fread($this->fp, 4);
			$status = socket_get_status($this->fp);
			return fread($this->fp,$status["unread_bytes"]);
		}
		function getServerInfo() {
			$response = $this->send("info");
			$tmp = substr($response,strlen($response)-3);
			$response = substr($response,0,strlen($response)-3);
			if(strlen($response)!=0) {
				$response = explode("\x00", substr($response,1));
				if(strlen($response) == 5) {
					$serverVar["netAddress"] 	= $response[0];
					$serverVar["serverName"] 	= $response[1];
					$serverVar["mapName"] 		= $response[2];
					$serverVar["gameDesc"] 		= $response[4];
					$serverVar["activePlayers"] 	= ord(substr($tmp,0,1));
					$serverVar["maxPlayers"] 	= ord(substr($tmp,1,1));
					$serverVar["protocolVersion"] 	= ord(substr($tmp,2,1));
				}
			}
			return $serverVar;
		}
		function getServerPlayers() {
			$status = $this->sendRcon("status");
			$line = explode("\n", $status);

			$nr = 0;
			for($i=7;$i<=sizeof($line);$i++) {
				if(substr($line[$i],0,1)=="#") {
					//Name
					$s = strpos($line[$i],"\"") + 1;
					$e = strrpos($line[$i],"\"");
					$players[$nr]["name"] = substr($line[$i],$s,$e-$s);
					$line[$i] = trim(substr($line[$i],$e + 1));

					//Id
					$e = strpos($line[$i]," ");
					$players[$nr]["id"] = substr($line[$i],0,$e);
					$line[$i] = trim(substr($line[$i],$e));

					//wonid
					$e = strpos($line[$i]," ");
					$players[$nr]["wonid"] = substr($line[$i],0,$e);
					$line[$i] = trim(substr($line[$i],$e));
					
					//frag
					$e = strpos($line[$i]," ");
					$players[$nr]["frag"] = substr($line[$i],0,$e);
					$line[$i] = trim(substr($line[$i],$e));

					//time
					$e = strpos($line[$i]," ");
					$players[$nr]["time"] = substr($line[$i],0,$e);
					$line[$i] = trim(substr($line[$i],$e));

					//ping
					$e = strpos($line[$i]," ");
					$players[$nr]["ping"] = substr($line[$i],0,$e);
					$line[$i] = trim(substr($line[$i],$e));

					//loss
					$e = strpos($line[$i]," ");
					$players[$nr]["loss"] = substr($line[$i],0,$e);
					$line[$i] = trim(substr($line[$i],$e));

					//address
					$e = strpos($line[$i]," ");
					$players[$nr]["address"] = substr($line[$i],0,$e);

					$nr += 1;
				}
			}
		return $players;
		}
		function isAlive() {
			$response = $this->send("ping");
			if(substr($response,0,1) == "j") {
				return true;
			}else{
				return false;
			}
		}
	}
?>
