#! /bin/bash
cd /root/.run
# Belegungsübersicht als Datei erzeugen
wget --no-check-certificate -o /root/.run/SaveBelegung.log -O /root/.run/SaveBelegung.html "https://cvjm-intranet.de/index.php?id=2569&docinput[user_id]=***&docinput[passwd2]=***"
# Datei kopieren
# cp /var/www/vhosts/cvjm-intranet.de/httpdocs/files/belegung.html /var/www/vhosts/cvjm-feriendorf.de/httpdocs/files/belegung.html

