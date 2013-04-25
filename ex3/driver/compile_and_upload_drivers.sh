#!/bin/sh

# Where to upload it to?
destination="129.241.103.185"

# Make the drivers
/usr/bin/make ARCH=avr32 CROSS_COMPILE=avr32-linux- -C ./buttons/
/usr/bin/make ARCH=avr32 CROSS_COMPILE=avr32-linux- -C ./leds/

# Upload drivers and the driver loader
scp ./buttons/buttons.ko ./leds/leds.ko ./drivers_load.sh root@$destination:/root
