#!/bin/bash

attr=00

a1=`echo $1 | awk '{split ($0,a ,":"); print a[1]};'`
a2=`echo $1 | awk '{split ($0,a ,":"); print a[2]};'`
a3=`echo $1 | awk '{split ($0,a ,":"); print a[3]};'`
a4=`echo $1 | awk '{split ($0,a ,":"); print a[4]};'`



if [ "$a4" == "blink" ]; then
    attr="01"
fi
if [ "$a4" == "none" ]; then
    attr="00"
fi

if [ "$a1" == "led" -a "$a2" == "clear" ]; then
    output="ff00000000"
fi

if [ "$a1" == "brightness" ]; then
    output="fe${a2}000000"
fi


if [ "$a1" == "led" -a "$a2" != "clear" ]; then
    output="${a2}${a3}${attr}"
fi

# LOGING
# echo "`date`: input [$a1][$a2][$a3][$a4]"  >> /var/log/digispark-leds.log
# echo "`date`: output:$output" >> /var/log/digispark-leds.log

/usr/bin/digispark-send $output --no-new-line
