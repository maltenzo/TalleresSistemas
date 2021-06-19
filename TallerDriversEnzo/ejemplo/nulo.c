#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

static struct cdev dev;

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


static int __init nulo_init(void) {
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
	//inicializo el cdev
	cdev_init(&dev, &fops);
	//hago cosas
	if (!alloc_chrdev_region(&major, 0, count, "nulo")){
		return 1 ;
	}
	cdev_add(&dev, major, count);
	//creo los nodos del file system o algo asi
	mi_class = class_create(THIS_MODULE, "nulo");
	device_create(mi_class, NULL, major, NULL, "nulo");



	return 0;
}

static void __exit nulo_exit(void) {
	
	printk(KERN_ALERT "Adios, mundo cruel...\n");
	//destruyo todo
	
	device_destroy(mi_class, major);
	class_destroy(mi_class);

	unregister_chrdev_region(major, count);
	cdev_del(&dev);
	
}



module_init(nulo_init);
module_exit(nulo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
