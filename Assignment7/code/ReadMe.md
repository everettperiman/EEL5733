To compile the driver you must use Ubunutu desktop 22.04 or any linux environment using the 5.19.0-40-generic kernel
To compile the driver run make all
To install the driver run insmod usbkd.ko
A device must be bound to the driver to take effect
To remove the driver run rmmod usbkd.ko
To clean the installation directory run make clean