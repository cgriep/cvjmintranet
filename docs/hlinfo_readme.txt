Datafile:
This module needs a datafile placed in /inc/data to work.
The data filename must be: hl_<module_id>.txt
The file should contain the following lines (a total of 5 lines)

Halflife server private IP
Halflife server public IP
Halflife server port
RconPassword
ShowPlayers

The module connects to the gameserver through the private IP
The public IP is the address shown in the module (theese two may be the same)
Server port is what you expect, the port number the game is running on, normally 27015
Rcon password is not used at the moment
Showplayers may be 0 or 1 depending on if you want to show a playerlist or not

The reason there are two IP addresses are simply because our server is behind a NAT proxy,
and our webserver needs to communicate with the gameserver through the private IP.