# HIH6120
kernel character driver for HIH6120 on raspberry pi

Loading this module creates /dev/HIH6120 in user-space.

Reading from /dev/HIH6120 returns the measured temperature in format {x.1}

Writing a '1' to /dev/HIH6120 changes the output to measured humidity in format {x.2}

## Stub
When loading the stub /dev/HIH6120-stub is created

The stub responds in the same way as the regular module, but increments or decrements the integer part of the returned value at random when read and won't access the I2C bus.

## Compiling the module
Note: The following steps are all executed from the Raspberry Pi

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
