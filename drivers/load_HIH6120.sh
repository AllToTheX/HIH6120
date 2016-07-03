sudo make
if [ $? == 0 ]; then
	sudo rmmod HIH6120
	sudo cp HIH6120.rules /etc/udev/
	if [ ! -f /etc/udev/rules.d/010_HIH6120.rules ] ; then
		sudo ln -s /etc/udev/HIH6120.rules /etc/udev/rules.d/010_HIH6120.rules
	fi
	sudo insmod HIH6120.ko
fi
