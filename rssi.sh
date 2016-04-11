#!/bin/bash

  mk /tmp/rssi
    while true
    do
	/automedia/rssi.py signal > /tmp/rssi
	sleep 1
    done
