# yoMommaDriver
A linux module that tells yoMomma jokes. I used Derek Molloy's ebbchar driver as base found here
http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
Many thanks to Derek Molloy on his tutorials. They were a great start for me to start developing
Linux drivers.

Install and explore
- make
- make modules_install or insmod yoMomma.ko
- sudo modprobe yoMomma
- sudo chmod 755 /dev/yoMomma

Tell a joke
 cat /dev/yoMomma

