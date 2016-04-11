#!/bin/bash

nums=`cat /automedia/oldvol.db`

echo "nutm=$nums"

let "nums=5000+$nums"
if [ "$nums" -ge 65535 ];
 then
    nums=65534
 fi
  amixer set Master "$nums"
echo $nums>/automedia/oldvol.db

