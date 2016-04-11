#!/bin/bash

sleep 10

while true
do

  sleep 15

  count=$(ping -c 8 8.8.8.8 | grep 'received' | awk -F',' '{ print $2 }' | awk '{ print $1 }')
  echo "Received (8):" $count
  if [ "$count" != "" ]; then

  if [ $count -lt 5 ]; then
    # ie 70% failed
        echo "Poor network. Restart"
        killall wvdial
	killall mplayer
	killall mplayer.sh
	killall pingtest.sh
	/automedia/pingtest.sh &
  fi
else
	echo "Not network. Restart"
        killall wvdial
	killall mplayer
	killall mplayer.sh
	killall pingtest.sh
	/automedia/pingtest.sh &

fi



done