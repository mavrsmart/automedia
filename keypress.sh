#!/bin/bash

key="$1"

mplayer -af extrastereo=0,volume=-10 /automedia/songs/key.mp3 &

if [ $key == "VOLUME+" ] ; then
    /automedia/volplus.sh 
fi

if [ $key == "VOLUME-" ] ; then
    /automedia/volminus.sh 
fi


if [ $key == "TIMESHIFT" ] ; then
    killall disp
    killall cubard.sh
fi

if [ $key == "FULLSCREEN" ] ; then
    /automedia/poweroff.sh &
fi

if [ $key == "RECORD" ] ; then
    killall poweron.sh
    killall mplayer.sh
    killall mplayer
    /automedia/poweron.sh &
fi

if [ $key == "UP" ] ; then
    /automedia/next.sh &
fi

if [ $key == "DOWN" ] ; then
    /automedia/prev.sh &
fi

if [ $key == "RECALL" ] ; then
    /automedia/poweroff.sh &
fi

if [ $key == "SOURCE" ] ; then
    echo "" > /tmp/lirc/source &
fi

if [ $key == "K0" ] ; then
    echo "" > /tmp/lirc/0 &
fi

if [ $key == "K1" ] ; then
    echo "" > /tmp/lirc/1 &
fi


if [ $key == "K2" ] ; then
    echo "" > /tmp/lirc/2 &
fi


if [ $key == "K3" ] ; then
    echo "" > /tmp/lirc/3 &
fi

if [ $key == "K4" ] ; then
    echo "" > /tmp/lirc/4 &
fi

if [ $key == "K5" ] ; then
    echo "" > /tmp/lirc/5 &
fi

if [ $key == "K6" ] ; then
    echo "" > /tmp/lirc/6 &
fi

if [ $key == "K7" ] ; then
    echo "" > /tmp/lirc/7 &
fi

if [ $key == "K8" ] ; then
    echo "" > /tmp/lirc/8 &
fi

if [ $key == "K9" ] ; then
    echo "" > /tmp/lirc/9 &
fi


if [ $key == "MUTE" ] ; then
    echo "" > /tmp/lirc/mute &
fi

if [ $key == "POWER" ] ; then
    echo "" > /tmp/lirc/power &
fi

