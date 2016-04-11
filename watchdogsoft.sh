#!/bin/bash


while true
do

if ps ax | grep disp | grep -vq grep
then
  echo ""
else
  /automedia/disp &
fi

if ps ax | grep mikas | grep -vq grep
then
  echo ""
else
  /automedia/mikas/mikas &
fi


  sleep 1
done