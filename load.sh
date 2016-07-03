sudo make
if [ $? == 0 ]; then
	sudo rmmod HIH6120-stub
	sudo cp HIH6120-stub.rules /etc/udev/
	if [ ! -f /etc/udev/rules.d/010_HIH6120-stub.rules ] ; then
		sudo ln -s /etc/udev/HIH6120-stub.rules /etc/udev/rules.d/010_HIH6120-stub.rules
	fi
	sudo insmod HIH6120-stub.ko
fi
