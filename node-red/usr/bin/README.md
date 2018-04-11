The digispark-send binary called from the script is send program from 

https://github.com/digistump/DigisparkExamplePrograms.git

You need to precompile it on target node-red server (or crosscompile with proper target and arch)

The digispark-leds.sh scirpt takes one argument and is called from node-red server.


Example use:

/usr/bin/digispark-leds.sh led:clear

;clears all leds.

/usr/bin/digispark-leds.sh led:01:00ff00:none

;sets LED no 01 to red color and no blinking


/usr/bin/digispark-leds.sh led:02:ff0000:blink

;sets LED no 02 to green color with blinking

/usr/bin/digispark-leds.sh brightness:100

;sets all leds brightness to 100 in hex values (The Neopixel library takes range from 00 to ff)

