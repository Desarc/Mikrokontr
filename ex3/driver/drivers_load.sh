#!/bin/sh
module1="buttons"
module2="leds"
mode="664"

rmmod $module2
rmmod $module1

rm -f /dev/${module2}
rm -f /dev/${module1}

insmod ./$module1.ko $* || exit 1
insmod ./$module2.ko $* || exit 1

major1=$(awk '$2=="'$module1'" {print $1}' /proc/devices)
echo $major1

major2=$(awk '$2=="'$module2'" {print $1}' /proc/devices)
echo $major2

mknod /dev/${module1} c ${major1} 0
mknod /dev/${module2} c ${major2} 0
