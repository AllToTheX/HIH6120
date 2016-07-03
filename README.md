# HIH6120
kernel driver for HIH6120 on raspberry pi

For now only includes the stub.
Stub starts at value 20 when loaded and randomly adds or substracts 1 when read.

## Next steps are all executed on the raspberry pi

Clone repo:
* <code>git clone https://github.com/AllToTheX/HIH6120.git</code>

Install Curses and rpi-source:

* <code>sudo apt-get install libncurses5-dev</code>
* <code>sudo wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update</code>
* <code>sudo rpi-source</code>

To build:
* <code>sudo make</code>

To load:
* <code>sudo insmod HIH6120-stub.ko</code>

To check:
* <code>sudo cat /dev/HIH6120-stub</code>

To make driver readable for everyone:
* <code>sudo cp HIH6120-stub.rules /etc/udev/</code>
* <code>sudo ln -s /etc/udev/HIH6120-stub.rules /etc/udev/rules.d/010_HIH6120-stub.rules</code>

To do all that automatically:
* <code>./load_HIH6120-stub.sh</code>
