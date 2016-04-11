#!/bin/bash

stty 9600 -F /dev/ttyUSB0 raw ispeed 9600 ospeed 9600 -echo

echo -en "stk,$1\n" > /dev/ttyUSB0