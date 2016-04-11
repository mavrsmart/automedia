#!/bin/bash

do_log() {

 while true
 do
  IFS=';' read -a arr
  if [ ! -z "$arr" ]; then
   
   echo "${arr[0]}";
#   echo "${arr[1]}";

#    IFS=';' read -a array <<< "$REPLY"
     

    if [ "${arr[0]}" == "1" ]; then
        echo "Воспроизведение        " > /tmp/action
        iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
       killall poweron.sh
       killall mplayer.sh
       killall mplayer
       /automedia/poweron.sh &

    fi
    if [ "${arr[1]}" == "1" ]; then
        echo "Стоп               " > /tmp/action
        iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
      /automedia/poweroff.sh &
    fi
    if [ "${arr[2]}" == "1" ]; then
        echo "Предыдущий канал      " > /tmp/action
        iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
      /automedia/prev.sh &
    fi
    if [ "${arr[3]}" == "1" ]; then
        echo "Следующий канал        " > /tmp/action
        iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
      /automedia/next.sh &
    fi
    if [ "${arr[4]}" == "1" ]; then
      echo "" > /tmp/lirc/source &
    fi

   fi
 done
}



stty -F /dev/ttyDISP raw ispeed 115200 ospeed 115200 -echo
#stty 19200 -F /dev/ttyARD
#setserial /dev/ttyARD baud_base 19200

while true; do

 cat /dev/ttyDISP | do_log

done