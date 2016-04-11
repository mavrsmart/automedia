#!/bin/bash

#nums=`cat /automedia/oldstation.db`
nums=1
numindb=`cat /automedia/radiolist.m3u | wc -l`

echo "nutm=$nums"

let "nums=$nums"
if [ "$nums" = 0 ];
 then
    let "nums=$numindb-1"
 fi

echo "numsv=$nums"
#echo $nums>/sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram
echo $nums > /automedia/oldstation.db

killall mplayer.sh
killall mplayer
