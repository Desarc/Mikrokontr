#!/bin/sh
destination="129.241.103.241"

/usr/bin/make ARCH=avr32 CROSS_COMPILE=avr32-linux- -C ./buttons/
/usr/bin/make ARCH=avr32 CROSS_COMPILE=avr32-linux- -C ./leds/

scp ./buttons/buttons.ko ./leds/leds.ko ./drivers_load.sh root@$destination:/root
