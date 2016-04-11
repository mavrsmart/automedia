#!/bin/bash

nums=`cat /automedia/oldvol.db`

echo "nutm=$nums"
amixer set Master "$nums"

