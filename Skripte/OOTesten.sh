#!/bin/bash
SERVICE='soffice'
 
if ps ax | grep -v grep | grep $SERVICE > /dev/null
then
    echo "$SERVICE service running, everything is fine"
else
    echo "$SERVICE is not running, restarted"
/opt/openoffice.org3/program/soffice -headless -accept="socket,host=127.0.0.1,port=8100;urp;" -nofirststartwizard &

    echo "$SERVICE is not running! Restarted service" | mail -s "$SERVICE down" cgriep@web.de 
fi
