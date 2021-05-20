/*#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void) {
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_ALERT "Adios, mundo cruel...\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

static struct cdev cdev;
dev_t major;
static int count = 1;
static struct class* mi_class;

ssize_t lectura(struct file* filp, char __user* data, size_t s, loff_t* off){

	//char res = NULL;

	return NULL;
}

ssize_t escritura(struct file* filp, const char __user* data, size_t s, loff_t* off){
	return s;
}

/*ssize_t abrir(struct inode* inodo, struct file* fl){
	return 0;
}

ssize_t liberar(struct inode* inodo, struct file* fl){
	return 0;
}*/

static struct file_operations fops = {
.owner = THIS_MODULE,
.read = lectura,
.write = escritura
/*.open = abrir,
.release = liberar,*/
};


static int __init hello_init(void) {
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");

	//static struct file_operations* fops;
	cdev_init(&cdev, &fops);

	int error = alloc_chrdev_region(&major, 0, count, "nulo");

	cdev_add(&cdev, major, count);

	mi_class = class_create(THIS_MODULE, "nulo");
	device_create(mi_class, NULL, major, NULL, "nulo");
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_ALERT "Adios, mundo cruel...\n");

	
	device_destroy(mi_class, major);
	class_destroy(mi_class);
	unregister_chrdev_region(major, count);
	cdev_del(&cdev);
}



module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
