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

ssize_t lectura(struct file* filp, char __user* data, size_t s, loff_t* off){
	unsigned int* num_random = kmalloc(sizeof(unsigned int), GFP_KERNEL);

	get_random_bytes(num_random, sizeof(unsigned int));
	*num_random = *num_random % maximo;

	int max_digit_count = 10;

	char* resultado = kmalloc(max_digit_count + 1, GFP_KERNEL);

	int error = snprintf(resultado, max_digit_count + 2, "%u\n", *num_random); 
	if(error < 0 || error > max_digit_count + 1){
		return -EPERM;
	}
	

	*data = copy_to_user(data, resultado, max_digit_count + 1);


	kfree(num_random);
	kfree(resultado);
	return 0;
}

ssize_t escritura(struct file* filp, const char __user* data, size_t s, loff_t* off){
	char* datos = kmalloc(s+1, GFP_KERNEL);
	copy_from_user(datos, data, s);
	datos[s] = '\0';
	if(kstrtoint(datos, 0, &maximo) != 0){
		kfree(datos);
		return -EPERM;
	}

	kfree(datos);

	return 0;
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

	int error = alloc_chrdev_region(&major, 0, count, "azar");

	cdev_add(&cdev, major, count);

	mi_class = class_create(THIS_MODULE, "azar");
	device_create(mi_class, NULL, major, NULL, "azar");


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