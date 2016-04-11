#!/bin/bash

while true; do
result=$(ping -c 2 8.8.8.8 | tail -2 | head -1 | awk '{print $4}')

if [ "$result" != "" ]
then
if [ $result -eq 0 ]
 then
#echo "0"
echo "0" > /tmp/pingtest
echo "" > /tmp/lostinet
#break
else
#echo "1"
 if test -f /tmp/lostinet; then  
    rm /tmp/lostinet
echo "1" > /tmp/pingtest
 if test -f /tmp/mplayeron_ping; then  
	    /automedia/poweroff.sh 
	    /automedia/poweron.sh &
 fi
fi
fi

else
echo "0" > /tmp/pingtest
echo "" > /tmp/lostinet
fi

sleep 3
done

exit 0