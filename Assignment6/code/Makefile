
obj-m := assignment6.o

KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules
	
app: 
	gcc -o testapp testapp.c -pthread

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *~ *.mod
