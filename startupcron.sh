#!/bin/bash

#service dnsmasq restart &
#service hostapd restart &

while true
do

sleep 2

if ps ax | grep ard.sh | grep -vq grep
then
  echo ""
else
  /automedia/arduino1/ard.sh &
fi

if ps ax | grep cubard.sh | grep -vq grep
then
  echo ""
else
  /automedia/cubard/cubard.sh &
fi


done