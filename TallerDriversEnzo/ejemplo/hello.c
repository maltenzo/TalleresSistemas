#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

static struct cdev cdev;

static struct class* mi_class;
dev_t  major;
unsigned int minor = 0;
unsigned int count = 1;
char* DEVICE_NAME = "nulo";

ssize_t read(struct file *filp, char __user *data, size_t s, loff_t *off){
	return NULL;
}

ssize_t write(struct file *filp, char __user *data, size_t s, loff_t *off){
	return NULL;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = read,
	.write = write,

};


static int __init hello_init(void) {
	//inicializo el cdev
	cdev_init(&cdev, &fops);
	//hago cosas
	alloc_chrdev_region(&major, minor, count, DEVICE_NAME);
	cdev_add(&cdev, major, count);
	//creo los nodos del file system o algo asi
	mi_class = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(mi_class, NULL, major, NULL, DEVICE_NAME);
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");


	return 0;
}

static void __exit hello_exit(void) {
	unregister_chrdev_region(major, count);
	cdev_del(&cdev);

	//destruyo los nodos
	device_destroy(mi_class, major);
	class_destroy(mi_class);
	printk(KERN_ALERT "Adios, mundo cruel...\n");
}



module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
