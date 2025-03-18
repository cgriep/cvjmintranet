#! /bin/bash
cd /home/cvjm
sudo -u cvjm /usr/bin/soffice --headless --accept="socket,host=127.0.0.1,port=8100;urp;" --nofirststartwizard &

echo "OpenOffice-Daemon gestartet"



