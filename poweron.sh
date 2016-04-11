#!/bin/bash

#/etc/inetstart.sh &

#/automedia/rssi.sh &

service pulseaudio restart &

rm /tmp/mplayeroff
echo "" > /tmp/mplayeron
echo "" > /tmp/mplayeron_ping

while true
do
I=0
cat /automedia/radiolist.m3u | while read LINE;
  do
   let "I+=1"
   NO=`cat /automedia/oldstation.db`
#    NO=`head -n 1 /sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram`
   #echo $NO
     if [ "$NO" = "$I" ]
     then
      killall mplayer.sh
      killall mplayer
      /automedia/mplayer.sh $LINE 256 50
      killall mplayer
     fi
  done
done
