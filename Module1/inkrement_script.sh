#!/bin/bash

# Файл, куда будем записывать результат
LOGFILE="$HOME/log_file.txt"

if [ ! -f "$LOGFILE" ]; then
    COUNT=1
else
COUNT=$(tail -n 1 "$LOGFILE" |awk '{print $(NF-1)}' )
COUNT=$((COUNT + 1))
fi


echo "$(date '+%Y-%m-%d %H:%M:%S') I run $COUNT time" >> "$LOGFILE"


if [ "$COUNT" -gt 10 ]; then
    crontab -l | grep -v "$HOME/log_script.sh" | crontab -
fi
