#!/bin/bash

#setserial /dev/ttyUSB0 baud_base 38400
#stty 9600 -F /dev/ttyUSB0 

I=0;

do_log() {

 while true
 do
  IFS=';' read -a arr
  if [ ! -z "$arr" ]; then
   
   echo "${arr[@]}";
#   echo "${arr[1]}";

#    IFS=';' read -a array <<< "$REPLY"
    if [ $I -ne "0" ]; then
	let I=$(( $I - 1 ));
    fi

    echo ${arr[0]} > /tmp/ard1/temp
    echo ${arr[1]} > /tmp/ard1/temp_max
    echo ${arr[2]} > /tmp/ard1/temp_min
    echo ${arr[3]} > /tmp/ard1/pwm_klap
    echo ${arr[4]} > /tmp/ard1/k1
    echo ${arr[5]} > /tmp/ard1/k2
    echo ${arr[6]} > /tmp/ard1/k3
    echo ${arr[7]} > /tmp/ard1/k4
    echo ${arr[8]} > /tmp/ard1/k5
    echo ${arr[9]} > /tmp/ard1/k6
    echo ${arr[10]} > /tmp/ard1/k7
    echo ${arr[11]} > /tmp/ard1/k8
    echo ${arr[11]} > /tmp/ard1/k9
    echo ${arr[11]} > /tmp/ard1/k10
    echo ${arr[11]} > /tmp/ard1/k11
    echo ${arr[11]} > /tmp/ard1/k12
    
    if [ $I -eq "0" ]; then
    if [ ${arr[5]} == "1" ]; then
	echo "Воспроизведение        " > /tmp/action
	iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
	killall poweron.sh
	killall mplayer.sh
	killall mplayer
	/automedia/poweron.sh &
	I=10;
    fi
    if [ ${arr[6]} == "1" ]; then
	echo "Стоп                       " > /tmp/action
	iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
	/automedia/poweroff.sh &
	I=10;
    fi
    if [ ${arr[4]} == "1" ]; then
	echo "Следующий канал           " > /tmp/action
	iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
	/automedia/next.sh &
	I=5;
    fi
    if [ ${arr[7]} == "1" ]; then
	echo "Предыдущий канал         " > /tmp/action
	iconv -f utf8 -t cp866 /tmp/action -o /tmp/action.cp866
	/automedia/prev.sh &
	I=5;
    fi
    fi

   fi
 done
}


mkdir /tmp/ard1


stty -F /dev/ttyARD raw ispeed 9600 ospeed 9600 -echo
#stty 19200 -F /dev/ttyARD
#setserial /dev/ttyARD baud_base 19200

while true; do

 cat /dev/ttyARD | do_log

done
