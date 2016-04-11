#!/bin/bash

stty -F /dev/ttyUSB0 raw ispeed 9600 ospeed 9600 -echo

echo -en "stmk,$1\n" > /dev/ttyUSB0