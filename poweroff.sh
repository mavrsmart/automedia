#!/bin/sh

rm /tmp/mplayeron
rm /tmp/mplayeron_ping
echo "" > /tmp/mplayeroff


killall rssi.sh
killall poweron.sh
killall mplayer.sh
killall mplayer
#killall disp1602
#/automedia/disp1602 &
#killall inetstart.sh
#poff internet &