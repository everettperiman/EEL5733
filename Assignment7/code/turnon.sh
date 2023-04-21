make clean 
make all
insmod usbkbd.ko
echo -n "2-1:1.0" > /sys/bus/usb/drivers/usbhid/unbind
echo -n "2-1:1.0" > /sys/bus/usb/drivers/usbkbd/bind