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
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/random.h>

static struct cdev cdev;
dev_t major;
static int count = 1;
unsigned int maximo = 0;
static struct class* mi_class;

static ssize_t lectura(struct file* filp, char __user* data, size_t size, loff_t* off){
	unsigned int num_random;
	get_random_bytes(&num_random, sizeof(unsigned int));
	num_random = num_random % maximo;

	char* buffer_res = kmalloc(size, GFP_KERNEL);

	int effective_size = snprintf(buffer_res, size, "%u\n", num_random); 
	if(effective_size < 0 || effective_size > size){
		return -EPERM;
	}
	
	copy_to_user(data, buffer_res, size);

	kfree(buffer_res);
	return  effective_size;
}

static ssize_t escritura(struct file* filp, const char __user* data, size_t size, loff_t* off){
	char* datos = kmalloc(size+1, GFP_KERNEL);
	copy_from_user(datos, data, size);
	datos[size] = '\0';
	if(kstrtoint(datos, 0, &maximo) != 0 || maximo < 0){
		kfree(datos);
		return -EPERM;
	}

	kfree(datos);

	return size;
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
.write = escritura,
/*.open = abrir,
.release = liberar,*/
};


static int __init azar_init(void) {
	printk(KERN_ALERT "Hola, Sistemas Operativos!\n");

	//static struct file_operations* fops;
	cdev_init(&cdev, &fops);

	int error = alloc_chrdev_region(&major, 0, count, "azar");

	cdev_add(&cdev, major, count);

	azar_class = class_create(THIS_MODULE, "azar");
	device_create(azar_class, NULL, major, NULL, "azar");


	return 0;
}

static void __exit azar_exit(void) {
	printk(KERN_ALERT "Adios, mundo cruel...\n");

	
	device_destroy(azar_class, major);
	class_destroy(azar_class);
	unregister_chrdev_region(major, count);
	cdev_del(&cdev);
}





module_init(azar_init);
module_exit(azar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Generador de numeros al azar");