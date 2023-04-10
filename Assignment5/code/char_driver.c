#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */

#define MYDEV_NAME "/dev/mycdrv"

static char *ramdisk;
#define ramdisk_size (size_t) (16*PAGE_SIZE)

// Added to make the initial code work
static dev_t *device_pointer;
static struct ASP_mycdrv *mycdrv_devices;
static int my_major = 500;
static struct cdev *my_cdev;

//NUM_DEVICES defaults to 3 unless specified during insmod
static int NUM_DEVICES = 10;
module_param(NUM_DEVICES, int, S_IRUGO);

#define CDRV_IOC_MAGIC 'Z'
#define ASP_CLEAR_BUF _IO(CDRV_IOC_MAGIC, 1)

struct ASP_mycdrv {
	struct cdev dev;
	char *ramdisk;
	struct semaphore sem;
	int devNo;
	// any other field you may want to add
};

// Added to make the device node automated
static struct class *cdev_class;

static int mycdrv_open(struct inode *inode, struct file *file)
{
	pr_info(" OPENING device: %s:\n\n", MYDEV_NAME);
	return 0;
}

static int mycdrv_release(struct inode *inode, struct file *file)
{
	pr_info(" CLOSING device: %s:\n\n", MYDEV_NAME);
	return 0;
}

static ssize_t
mycdrv_read(struct file *file, char __user * buf, size_t lbuf, loff_t * ppos)
{
	int nbytes;
	if ((lbuf + *ppos) > ramdisk_size) {
		pr_info("trying to read past end of device,"
			"aborting because this is just a stub!\n");
		return 0;
	}
	nbytes = lbuf - copy_to_user(buf, ramdisk + *ppos, lbuf);
	*ppos += nbytes;
	pr_info("\n READING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;
}

static ssize_t
mycdrv_write(struct file *file, const char __user * buf, size_t lbuf,
	     loff_t * ppos)
{
	int nbytes;
	if ((lbuf + *ppos) > ramdisk_size) {
		pr_info("trying to read past end of device,"
			"aborting because this is just a stub!\n");
		return 0;
	}
	nbytes = lbuf - copy_from_user(ramdisk + *ppos, buf, lbuf);
	*ppos += nbytes;
	pr_info("\n WRITING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;
}

static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.read = mycdrv_read,
	.write = mycdrv_write,
	.open = mycdrv_open,
	.release = mycdrv_release,
};

static int __init my_init(void)
{
	int i = 0;
	device_pointer = kmalloc(NUM_DEVICES * sizeof(dev_t), GFP_KERNEL);
	mycdrv_devices = kmalloc(NUM_DEVICES * sizeof(struct ASP_mycdrv), GFP_KERNEL);
	// Create all of the devices
	while(i<NUM_DEVICES)
	{
		device_pointer[i] = MKDEV(my_major, i);
		mycdrv_devices[i].ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
		mycdrv_devices[i].devNo = i;
		i++;
	}
	register_chrdev_region(device_pointer[0], NUM_DEVICES, MYDEV_NAME);
	my_cdev = cdev_alloc();
	cdev_init(my_cdev, &mycdrv_fops);
	cdev_add(my_cdev, device_pointer[0], NUM_DEVICES);

	// Register the class and devices
	cdev_class = class_create(THIS_MODULE, "my_class");
	i = 0;
	while(i<NUM_DEVICES)
	{
		device_create(cdev_class, NULL, device_pointer[i], NULL, "mycdrv%d", MINOR(i));
		pr_info("\nEverett succeeded in registering character device mycdrv%d\n", MINOR(i));
		i++;
	}
	pr_info("\nEverett succeeded in registering the character devices");
	return 0;
}

static void __exit my_exit(void)
{
	int i = 0;
	while(i<NUM_DEVICES)
	{
		device_destroy(cdev_class, device_pointer[i]);
		pr_info("\nEverett unregistered the device mycdrv%d\n", MINOR(i));
		i++;
	}
	class_destroy(cdev_class);
	cdev_del(my_cdev);
	unregister_chrdev_region(device_pointer[0], NUM_DEVICES);
	pr_info("\nEverett unregistered the devices\n");
	//i = 0;
	//while(i<NUM_DEVICES)
	//{
		kfree(ramdisk);
		//i++;
	//}
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Everett Periman");
MODULE_LICENSE("GPL v2");
