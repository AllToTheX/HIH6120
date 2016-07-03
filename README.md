# HIH6120
kernel driver for HIH6120 on raspberry pi

For now only includes the stub.
Stub starts at value 20 when loaded and randomly adds or substracts 1 when read.

## Next steps are all executed on the raspberry pi

Install Curses and rpi-source:

* sudo apt-get install libncurses5-dev
* sudo wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update
* sudo rpi-source

To build:
* sudo make

To load:
* sudo insmod HIH6120-stub.ko

To check:
* sudo cat /dev/HIH6120-stub

To make driver readable for everyone:
* sudo cp HIH6120-stub.rules /etc/udev/
* sudo ln -s /etc/udev/HIH6120-stub.rules /etc/udev/rules.d/010_HIH6120-stub.rules

To do all that automatically:
./load_HIH6120-stub.sh
