#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/random.h>

static struct cdev dev;

static struct class* mi_class;
dev_t  major;
unsigned int minor = 0;
unsigned int count = 1;
char* DEVICE_NAME = "";
int* numeroUsuario ;
ssize_t read(struct file *filp, char __user *data, size_t s, loff_t *off){
	if(!numeroUsuario){
		-EPERM;
	}
	int* random  = kmalloc(12, GFP_KERNEL);
	char * res = kmalloc(16, GFP_KERNEL);
	get_random_bytes(random, 15);
	*random = *random % *numeroUsuario;


	snprintf(res, 15, "%u\n", *random);
	copy_to_user(data, res, sizeof(res));
	
	kfree(random);
	kfree(res);
	return s;

}

ssize_t write(struct file *filp, char __user *data, size_t s, loff_t *off){

	if(numeroUsuario){
		kfree(numeroUsuario);
	}

	char* __data = kmalloc(s+1, GFP_KERNEL);
	copy_from_user(__data, data, s);
	__data[s] = '\0';

	if(0 != kstrtoint(__data, 10, numeroUsuario)){
		return -EPERM;
	}
	kfree(__data);

	return s;
}


struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = read,
	.write = write,

};


static int __init random_init(void) {
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
	//inicializo el cdev
	cdev_init(&dev, &fops);
	//hago cosas
	alloc_chrdev_region(&major, 0, count, "azar");
	cdev_add(&dev, major, count);
	//creo los nodos del file system o algo asi
	mi_class = class_create(THIS_MODULE, "azar");
	device_create(mi_class, NULL, major, NULL, "azar");



	return 0;
}

static void __exit random_exit(void) {
	
	printk(KERN_ALERT "Adios, mundo cruel...\n");
	//destruyo todo
	
	if(!numeroUsuario){
		kfree(numeroUsuario);
	}

	device_destroy(mi_class, major);
	class_destroy(mi_class);

	unregister_chrdev_region(major, count);
	cdev_del(&dev);
	
}



module_init(random_init);
module_exit(random_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
