Atmel (Microchip) bit banging io routines and cli
based on https://github.com/edrosten/eeprom-hacks.git
and https://deathandthepenguinblog.wordpress.com/2018/01/03/hacking-the-neutrogena-visible-light-therapy-system-to-get-99-lives/


Simple serial port command line interface to the routines bundled together to make it easier to have a single
Arduino image and control things through a use serial ports (or the arduino serial monitor)

Pinout/connections
 Arduino | 8 pin  | usage
 mini    | clip   |
 pro pin | Pin    |
---------+--------+-----------
  gnd    |   5    | Ground
  gnd    |   6    | org = x8
  vcc    |   8    | 3.3v Power
   4     |   1    | CS
   5     |   2    | Clock
   6     |   3    | MoSi
   7     |   4    | MiSo

Cli Reference / Commands
------------------------
dump (start) (end) - dump an area of the eeprom
write (address) (data) -- write an eeprom location
all - dump all te eeprom contents
reset - write the data to reset to a count of 30
? print help


