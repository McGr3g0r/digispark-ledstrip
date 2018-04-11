Connect WS2812B data to DigiSpark pin P0.

You can set led colors separately and group as well.

Message protocol format, use hex values:

 ll | bbrrgg | aa
 
 ll -led number from 0 to NUMPIXELS-1

 If ll is ff - then set color for all leds.
 If ll is fe - then rr is brightness from 0 to ff passed to the Nexopixel library.
 rr,gg,bb - hex values of the color to be set
 aa - led attribute, currently bit 0 (set 0x01) means blink
 
 