# HIH6120
kernel driver for HIH6120 on raspberry pi

For now only includes the stub.

Install Curses and rpi-source:

* sudo apt-get install libncurses5-dev
* sudo wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update
* sudo rpi-source

To build:
* sudo make

To load:
sudo insmod HIH6120-stub.ko

To check:
sudo cat /dev/HIH6120-stub
