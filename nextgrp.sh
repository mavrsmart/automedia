#!/bin/bash

#/etc/inetstart.sh &

# NO=`head -n 1 /sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram`
 NO=`cat /automedia/oldstation.db`

I=0
ESOLD=0
NUMS=1
cat /automedia/radiolist.grp | while read LINE;
  do
     let "I+=1"
     NEW=$LINE

     if [ "$NO" = "$I" ]
     then
          ESOLD=1
          OLD=$NEW
     fi
     
     if [ "$ESOLD" -eq 1 ] 
     then
     if [ "$OLD" != "$NEW" ]
     then
     let NUMS=$I
     echo $I>/tmp/channel
     echo $OLD
     echo $NEW
#       exit
       break
     fi
    fi
  done


numindb=`cat /automedia/radiolist.m3u | wc -l`

 NUMS=`head -n 1 /tmp/channel`


echo "nutm=$NUMS"

if [ "$NUMS" -ge "$numindb" ];
 then
    NUMS=1
 fi
#echo $NUMS>/sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram
echo $NUMS> /automedia/oldstation.db

killall mplayer.sh
killall mplayer
