#!/bin/bash

nums=`cat /automedia/oldstation.db`
#nums=`head -n 1 /sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram`
numindb=`cat /automedia/radiolist.m3u | wc -l`

echo "nutm=$nums"

let "nums=1+$nums"
if [ "$nums" -ge "$numindb" ];
 then
    nums=1
 fi
echo "numsv=$nums"
#echo $nums>/sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram
echo $nums > /automedia/oldstation.db

killall mplayer.sh
killall mplayer
