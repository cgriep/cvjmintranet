#!/bin/sh
# Verwende den Bildschirm des Servers (User Poddel) zur Ausgabe sofern vorhanden
# (kann nur bei Fehlern passieren)
# /bin/cp /var/lib/gdm/:0.Xauth /srv/www/cgi-bin
export XAUTHORITY=/srv/www/cgi-bin/:0.Xauth 
# laufenden X-Server stoppen
#rm /tmp/.X0-lock
#/usr/bin/X11/xinit -- -auth /home/poddel/.Xauthority &
/usr/bin/php5 /srv/www/cgi-bin/testoopdf.php $1

