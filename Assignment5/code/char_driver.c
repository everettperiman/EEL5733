#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */

#define MYDEV_NAME "/dev/mycdrv"

static char *ramdisk;
#define ramdisk_size (size_t) (16*PAGE_SIZE) //16*PAGE_SIZE

// Added to make the initial code work
static dev_t *device_pointer;
static struct ASP_mycdrv *mycdrv_devices;
static int my_major = 500;
static struct cdev *my_cdev;

//NUM_DEVICES defaults to 3 unless specified during insmod
static int NUM_DEVICES = 3;
module_param(NUM_DEVICES, int, S_IRUGO);

#define CDRV_IOC_MAGIC 'Z'
#define ASP_CLEAR_BUF _IO(CDRV_IOC_MAGIC, 1)

struct ASP_mycdrv {
	struct cdev dev;
	struct semaphore sem;
	int devNo;
	int size;
	int cur;
	char *ramdisk;
	// any other field you may want to add
};

// Added to make the device node automated
static struct class *cdev_class;

static int mycdrv_open(struct inode *inode, struct file *file)
{
    struct ASP_mycdrv *device_data = container_of(inode->i_cdev, struct ASP_mycdrv, dev);
    file->private_data = device_data;
	
	pr_info(" OPENING device: %s%d:\n\n", MYDEV_NAME, device_data->devNo);
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
	struct ASP_mycdrv *dev = file->private_data;
	// char temp[lbuf+1];
	int nbytes;
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	if ((lbuf + *ppos) > dev->size) {
		pr_info("trying to read past end of device,"
			"aborting because this is just a stub!\n");
		return 0;
	}
	nbytes = lbuf - copy_to_user(buf, dev->ramdisk + *ppos, lbuf);
	*ppos += nbytes;
	up(&dev->sem);
	pr_info("\n READING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;
}

static ssize_t
mycdrv_write(struct file *file, const char __user * buf, size_t lbuf, loff_t * ppos)
{
	struct ASP_mycdrv *dev = file->private_data;
	int nbytes;
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	if ((lbuf + *ppos) > dev->size) {
		return 0;
	}
	nbytes = lbuf - copy_from_user(dev->ramdisk + *ppos, buf, lbuf);
	*ppos += nbytes;
	up(&dev->sem);
	pr_info("\n WRITING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	return nbytes;
}

static ssize_t
mycdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct ASP_mycdrv *dev = file->private_data;
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	char *new_ramdisk = kzalloc(ramdisk_size, GFP_KERNEL);
	char *old_ramdisk = dev->ramdisk;
	switch(cmd)
	{
		case ASP_CLEAR_BUF:
			dev->ramdisk = new_ramdisk;
			file->f_pos = 0;
			dev->size = ramdisk_size;
			kfree(old_ramdisk);
			up(&dev->sem);
			return 0;
		default:
			up(&dev->sem);
			return -EINVAL;
	}
}

static loff_t 
my_llseek(struct file *file, loff_t offset, int whence)
{
	pr_info("\nLseek Start");
    struct ASP_mycdrv *dev = file->private_data;
    loff_t newpos;
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
    switch (whence) {
        case 0:
            newpos = offset;
            break;
        case 1:
            newpos = file->f_pos + offset;
            break;
        case 2:
            newpos = strlen(dev->ramdisk) + offset;
            break;
        default:
            return -EINVAL;
    }

    if (newpos < 0)
        return -EINVAL;

	if (newpos >= dev->size)
	{
		int new_pages = ((newpos - dev->size) / PAGE_SIZE) + 1;
		// Allocate to the new size newpos
		dev->ramdisk = krealloc(dev->ramdisk, ((new_pages*PAGE_SIZE) + dev->size), NULL);
		// Write zeros from the end of the previous block of memory to this current one
		memset(dev->ramdisk + dev->size + 1, 0, ((new_pages*PAGE_SIZE) + dev->size)) - 1;
		// Update the device size to reflect this update
		dev->size = ((new_pages*PAGE_SIZE) + dev->size); 
		pr_info("\n Updated device size %d", dev->size);
	}
    file->f_pos = newpos;
	up(&dev->sem);
	pr_info("\nLseek New Position %d", file->f_pos);
    return newpos;
}


static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.read = mycdrv_read,
	.write = mycdrv_write,
	.open = mycdrv_open,
	.release = mycdrv_release,
	.unlocked_ioctl  = mycdrv_ioctl,
	.llseek = my_llseek,
};

static int __init my_init(void)
{
	int i = 0;
	device_pointer = kmalloc(NUM_DEVICES * sizeof(dev_t), GFP_KERNEL);
	mycdrv_devices = kmalloc(NUM_DEVICES * sizeof(struct ASP_mycdrv), GFP_KERNEL);
	memset(mycdrv_devices, 0, NUM_DEVICES * sizeof(struct ASP_mycdrv));
	// Create all of the devices
	register_chrdev_region(my_major, NUM_DEVICES, MYDEV_NAME);
	while(i<NUM_DEVICES)
	{
		device_pointer[i] = MKDEV(my_major, i);

		// Initialize the private data
		mycdrv_devices[i].ramdisk = kzalloc(ramdisk_size, GFP_KERNEL);
		mycdrv_devices[i].devNo = MKDEV(my_major, i);
		mycdrv_devices[i].size = ramdisk_size;
		sema_init(&mycdrv_devices[i].sem,1);
		pr_info("\nEverett succeeded in registering character device mycdrv%d %d\n", MAJOR(mycdrv_devices[i].devNo), MINOR(mycdrv_devices[i].devNo));
		cdev_init(&mycdrv_devices[i].dev, &mycdrv_fops);
		cdev_add(&mycdrv_devices[i].dev, mycdrv_devices[i].devNo, 1);
		i++;
	}

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
	// Free all devics and data
	int i = 0;
	while(i<NUM_DEVICES)
	{
		kfree(mycdrv_devices[i].ramdisk);
		device_destroy(cdev_class, device_pointer[i]);
		cdev_del(&mycdrv_devices[i].dev);
		pr_info("\nEverett unregistered the device mycdrv%d\n", MINOR(i));
		i++;
	}
	class_destroy(cdev_class);
	unregister_chrdev_region(device_pointer[0], NUM_DEVICES);
	kfree(mycdrv_devices);
	kfree(device_pointer);
	pr_info("\nEverett unregistered the devices\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Everett Periman");
MODULE_LICENSE("GPL v2");
