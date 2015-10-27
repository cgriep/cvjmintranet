#! /bin/bash
cd /root/.run
# Belegungsübersicht als Datei erzeugen
wget --no-check-certificate -o /root/.run/SaveBelegung.log -O /root/.run/SaveBelegung.html "https://v.cvjm-feriendorf.de/index.php?id=2569&docinput[user_id]=cgriep@web.de&docinput[passwd2]="
# Datei kopieren 
cp /srv/www/vhosts/cvjm-feriendorf.de/subdomains/v/httpsdocs/files/belegung.html /srv/www/vhosts/cvjm-feriendorf.de/httpdocs/files/belegung.html

