#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>

#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/i2c.h>
#include <linux/delay.h>

#include <asm/uaccess.h>
#include <linux/string.h>

#define HIH6120_OK 0x00
#define HIH6120_STALE 0x01
#define HIH6120_COMM 0x02
#define HIH6120_TIMEOUT 0xFF

#define I2C_MAX 100

static char module_name[] = "HIH6120";
static char i2c_dev = 1;

static dev_t mydev;
static struct class *c1;
static char buffer[64];

struct cdev my_cdev;
struct i2c_adapter *i2c_adap;

char ret_type = 0;

char buf[5];
static struct i2c_msg rd_msg = {
		.addr = 0x27,
		.flags = I2C_M_RD,
		.len = 4,
		.buf = buf,
};

static struct i2c_msg meas_rq = {
		.addr = 0x27,
		.flags = 0,
		.len = 0,
		.buf = buf,
};

char get_hum_temp(char *humid, char *temp)
{
	char state;
	int humidity;
	char humid_int;
	char humid_dec;
	int temperature;
	char temp_int;
	char temp_dec;
	char timeout = 0;

	i2c_transfer(i2c_adap,&meas_rq,1);
	do {
		i2c_transfer(i2c_adap,&rd_msg,1);
		state = (buf[0] >> 6) & 0x03 ;
		timeout++;
	} while ( (state != 0) && (timeout < I2C_MAX) );
	if (timeout >= I2C_MAX) {
		printk(KERN_INFO "Sensor timed out after %d tries \n",timeout);
		state = HIH6120_TIMEOUT;
	}
	printk(KERN_INFO "State: %d\n",state);

	humidity = (buf[0] << 8) | buf[1];
	humidity = (humidity * 10000) / 16382;
	humid_int = humidity / 100;
	humid_dec = humidity % 100;
	sprintf(temp,"%d.%d",humid_int,humid_dec);
	printk(KERN_INFO "humidity: %s %%\n",temp);

	temperature = (buf[2] << 6) | (buf[3] >> 2);
	temperature = (temperature * 1650) / 16382;
	temp_int = temperature / 10 - 40;
	temp_dec = temperature % 10;
	sprintf(humid,"%d.%d",temp_int,temp_dec);
	printk(KERN_INFO "temperature: %s C\n",temp);

	return state;
}

char temperature[7];
char humidity[7];
char ret_val[7];

ssize_t hih6120_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int not_copied;
	int str_length;
	char state;

    if (*f_pos == 0) { // If this is the start of the string, determine a new temperature
    	state = get_hum_temp(humidity,temperature);
    	if ( state != HIH6120_OK){
    		return -state;
    	}
    	sprintf(ret_val,"%s",temperature);
    }

    if (ret_val[*f_pos] == '\0') {
    	// End of string
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

ssize_t hih6120_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
//	char message[len+1];
//	sprintf(message, "%s", buffer);
//	printk(KERN_INFO "Received %s.\n",message);
//	switch(buffer[0]){
//		case '0':
//			ret_type = 0;
//			break;
//		case '1':
//			ret_type = 1;
//			break;
//		default:
//			ret_type = 0;
//	};
	return len;
}

struct file_operations my_fops = {
		.owner = THIS_MODULE,
		.read = hih6120_read,
		.write = hih6120_write,
};

static int __init hih6120_in(void)
{
	printk(KERN_INFO "module %s being loaded.\n",module_name);
	// Get i2c device
	i2c_adap = i2c_get_adapter(i2c_dev);
	// Allocate Major/Minor driver nr in kernel space
	alloc_chrdev_region(&mydev, 0, 1, module_name);
	printk(KERN_INFO "Major:minor: %s\n", format_dev_t(buffer, mydev));

	// Register as a character driver
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;
	cdev_add(&my_cdev, mydev, 1);

	// Create user space interface
	c1 = class_create(THIS_MODULE,module_name);
	device_create(c1,NULL,mydev,NULL,module_name);

	return 0;
}

static void __exit hih6120_out(void)
{
	printk(KERN_INFO "module %s being unloaded.\n",module_name);

	cdev_del(&my_cdev);
	unregister_chrdev_region(mydev, 1);

	device_destroy(c1,mydev);
	class_destroy(c1);
}



module_init(hih6120_in)
module_exit(hih6120_out)


MODULE_AUTHOR("Allex Veldman");
MODULE_LICENSE("GPL");
