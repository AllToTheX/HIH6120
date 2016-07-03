sudo make
if [ $? == 0 ]; then
sudo rmmod HIH6120-stub
sudo insmod HIH6120-stub.ko
fi
