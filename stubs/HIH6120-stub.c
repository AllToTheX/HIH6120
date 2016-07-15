#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/random.h>

#include <asm/uaccess.h>
#include <linux/string.h>

#define HIH6120_OK 0x00
#define HIH6120_STALE 0x01
#define HIH6120_COMM 0x02
#define HIH6120_TIMEOUT 0xFF
#define HIH6120_HUMID 0x00
#define HIH6120_TEMP 0x01

char ret_type = HIH6120_TEMP;

static char module_name[] = "HIH6120-stub";

static dev_t mydev;
static struct class *c1;
static char buffer[64];

struct cdev my_cdev;

void get_new_temp(char *temperature, char *humidity)
{
	char tmp_nr;
	get_random_bytes(&tmp_nr,sizeof(tmp_nr));
	printk(KERN_INFO "Random nr: %d", tmp_nr);
	if (tmp_nr > 128){
		*temperature+=1;
		*humidity+=1;
	}else{
		*temperature-=1;
		*humidity-=1;
	}
}


char temp_int = 20;
char temp_dec = 5;
char humid_int = 42;
char humid_dec = 23;
char ret_val[20];

ssize_t my_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int not_copied;
	int str_length;

    if (*f_pos == 0) { // If this is the start of the string, determine a new temperature
    	get_new_temp(&temp_int,&humid_int);
    	switch(ret_type){
			case HIH6120_HUMID:
				sprintf(ret_val,"%d.%d",humid_int,humid_dec);
				break;
			case HIH6120_TEMP:
			default:
				sprintf(ret_val,"%d.%d",temp_int,temp_dec);
		}
    	printk(KERN_INFO "New value to return: %s\n", ret_val);
    }


    if (ret_val[*f_pos] == '\0') {
        printk(KERN_INFO "End of string, returning zero.\n");
        *f_pos = 0;
        return 0;
    }

    str_length = strlen(&ret_val[*f_pos]);
    not_copied = copy_to_user(buf, &ret_val[*f_pos], str_length);
    if (not_copied == 0)
    {
    	*f_pos += str_length;
    }else{
    	*f_pos += (str_length - not_copied);
    	printk(KERN_INFO "Could not copy %d bytes.\n",not_copied);
    }
    return str_length - not_copied;  // returned a  character
}

ssize_t my_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
	char message[len+1];
	int i;
	for ( i=0; i < len; i++ ){
		if ( buffer[*offset+i] == '\n'){
			break;
		}
		message[i] = buffer[*offset+i];
	}
	message[i] = '\0';

	printk(KERN_INFO "Received %s.\n",message);
	if( strlen(message) == 1)
	{
		switch(message[0]){
				case '1':
					ret_type = HIH6120_HUMID;
					break;
				case '0':
				default:
					ret_type = HIH6120_TEMP;
			};
	}
	return len;
}

struct file_operations my_fops = {
		.owner = THIS_MODULE,
		.read = my_read,
		.write = my_write,
};

static int __init chardrv_in(void)
{
	printk(KERN_INFO "module %s being loaded.\n",module_name);

	// Allocate Major/Minor driver nr in kernel space
	alloc_chrdev_region(&mydev, 0, 1, module_name);
	printk(KERN_INFO "%s\n", format_dev_t(buffer, mydev));

	// Register as a character driver
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;
	cdev_add(&my_cdev, mydev, 1);

	// Create user space interface
	c1 = class_create(THIS_MODULE,module_name);
	device_create(c1,NULL,mydev,NULL,module_name);

	return 0;
}

static void __exit chardrv_out(void)
{
	printk(KERN_INFO "module %s being unloaded.\n",module_name);

	cdev_del(&my_cdev);
	unregister_chrdev_region(mydev, 1);

	device_destroy(c1,mydev);
	class_destroy(c1);
}



module_init(chardrv_in)
module_exit(chardrv_out)


MODULE_AUTHOR("Allex Veldman");
MODULE_LICENSE("GPL");
