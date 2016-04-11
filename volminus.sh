#!/bin/bash

nums=`cat /automedia/oldvol.db`

echo "nutm=$nums"

let "nums=$nums-2500"
if [ "$nums" -le 1 ];
 then
    nums=1
 fi
  amixer set Master "$nums"
echo $nums>/automedia/oldvol.db

