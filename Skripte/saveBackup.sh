#! /bin/bash
date

LOGFILE="/root/backup/databackup-$(date +"%F").sql.zip"
# Anzahl Tage für Backupdateien
STORE_DAYS=10
STORE_MONTH=120

# backup from database
rm /root/backup/databackup-*.sql.zip -f
mysqldump -ucvjm -p"dbpassword" Verwaltung > /root/backup/verwaltung.sql
zip --encrypt -P 'cryptpassword' -r "$LOGFILE" /root/backup/verwaltung.sql

date
# files sichern
LOGFILE="/root/backup/fbackup-$(date +"%F").tar"
rm /root/backup/fbackup-*.zip -f
rm /root/backup/fbackup*.tar -f
tar -c -f $LOGFILE  /var/www/httpdocs/*
zip --encrypt -P 'cryptpassword' "$LOGFILE.zip" "$LOGFILE"

date
# files transferieren

ssh -p23 -i /root/.ssh/id_storage  servere ls fbackup-*.zip > /root/.run/backupfilelist.txt
ssh -p23 -i /root/.ssh/id_storage  server "ls databackup-*.zip" > /root/.run/backupfilelist2.txt
echo "Uploading"
for datei in /root/backup/*.zip
do
  bdatei="$(basename $datei)"
  scp -i /root/.ssh/id_storage $datei  server:$bdatei
done
date
# delete old files
STORE_DATE=$(date -d "now -${STORE_DAYS} days" '+%Y%m%d')
STORE_MAX=$(date -d "now -${STORE_MONTH} days" '+%Y%m%d')
echo "checking ${STORE_DATE}"
while read -r LINE; do
        datum=${LINE:8:10}
        datum=${datum//-/}
        tag=${datum:6:2}
        echo "filedate $datum"
        if [[ ${STORE_DATE} -ge $datum && "$tag" != "01" && "$datum" != *\/ ]]; then
            ssh -p23 -i /root/.ssh/id_storage  server "rm $LINE" &
            echo "remove filedata from $datum"
        fi
        if [[ "$datum" != *\/ && ${STORE_MAX} -gt ${datum} ]]; then
            echo "remove monthly backup from $datum"
            ssh -p23 -i /root/.ssh/id_storage  server "rm $LINE" &
        fi
done < /root/.run/backupfilelist.txt

# delete old files databases
while read -r LINE; do
        datum=${LINE:11:10}
        datum=${datum//-/}
        tag=${datum:6:2}
        if [[ ${STORE_DATE} -ge $datum && "$tag" != "01" && "$datum" != *\/ ]]; then
            ssh -p23 -i /root/.ssh/id_storage  server "rm $LINE" &
            echo "remove dbdata from $datum"
        fi
         if [[ "$datum" != *\/ && ${STORE_MAX} -gt ${datum} ]]; then
            echo "remove monthly db backup from $datum"
            ssh -p23 -i /root/.ssh/id_storage  server "rm $LINE" &
        fi
done < /root/.run/backupfilelist2.txt

date

cat  /root/.run/backupftp.txt | mail -s Backup -r 'cron@absender' recipient
