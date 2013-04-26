#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "ap7000.h"

#define PORT_RANGE 24
#define PORT_OFFSET 8

/* prototyper */

static int __init leds_init(void);
static void __exit leds_exit(void);
static int leds_open (struct inode *inode, struct file *filp);
static int leds_release (struct inode *inode, struct file *filp);
static ssize_t leds_read (struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t leds_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp);

void set_leds(void);

dev_t dev;
int first_minor = 0, count = 1;
const char name[] = "leds";
struct cdev *leds_cdev;

const long SET_ALL_LEDS = 0xffffff00;
volatile char led_status[4]; 

int status;
long vector;

volatile avr32_pio_t *piob = &AVR32_PIOB;

/* fops-struct */

static struct file_operations leds_fops = {
  .owner = THIS_MODULE,
  .read = leds_read,
  .write = leds_write,
  .open = leds_open,
  .release = leds_release
};

/* activate the LEDs indicated by the led_status number by writing to input data register */
void set_leds(void) {
	piob->codr = SET_ALL_LEDS;
	led_status[3] = 0x0; 
	vector = (led_status[0] << 24)+(led_status[1] << 16)+(led_status[2] << 8)+led_status[3];  
	piob->sodr = vector;
}

static int __init leds_init (void) {

	/* allocating minor and major numbers */
  	status = alloc_chrdev_region(&dev, first_minor, count, name);
	printk("alloc success? %i \n", status);

  	/* request access to ports */
	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PER+PORT_OFFSET, PORT_RANGE, name);
	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_OER+PORT_OFFSET, PORT_RANGE, name);
	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_CODR+PORT_OFFSET, PORT_RANGE, name);
	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_SODR+PORT_OFFSET, PORT_RANGE, name);

	/* initialize LEDs */
	piob->per = SET_ALL_LEDS;
	piob->oer = SET_ALL_LEDS;
	piob->codr = SET_ALL_LEDS;
	
	/* register device in the system */
	leds_cdev = cdev_alloc();
	leds_cdev->ops = &leds_fops;
	cdev_init(leds_cdev, &leds_fops);
	leds_cdev->owner = THIS_MODULE;
	status = cdev_add(leds_cdev, dev, count);
	printk("cdev_add success? %i \n", status);

	printk(KERN_ALERT "LED module enabled.\n");

  	return 0;
}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit leds_exit (void) {

	/* unregister device and release ports */
	cdev_del(leds_cdev);
	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PER+PORT_OFFSET, PORT_RANGE);
	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_OER+PORT_OFFSET, PORT_RANGE);
	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_CODR+PORT_OFFSET, PORT_RANGE);
	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_SODR+PORT_OFFSET, PORT_RANGE);

	/* releasing minor and major numbers */
	unregister_chrdev_region(dev, count);

	printk(KERN_ALERT "LED module disabled.\n");
}

/*****************************************************************************/
/* fops-funksjoner */

static int leds_open (struct inode *inode, struct file *filp) {

  	return 0;
}


/*---------------------------------------------------------------------------*/

static int leds_release (struct inode *inode, struct file *filp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t leds_read (struct file *filp, char __user *buff,
              size_t count, loff_t *offp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t leds_write (struct file *filp, const char __user *buff,
               size_t count, loff_t *offp) {
	
	/* get data from user space and write to LEDs */
	copy_from_user(&led_status, buff, count);
	set_leds();

	return 0;
}

/*****************************************************************************/
/* modul-beskrivelsesmakroer */

module_init (leds_init);  /* angir hva som er init-funksjon */
module_exit (leds_exit);  /* angir hva som er exit-funksjon */

MODULE_LICENSE ("GPL");     /* programlisens for modulen */
MODULE_DESCRIPTION ("support for leds");    /* tekstlig beskrivelse */
MODULE_VERSION ("");        /* versjonsnummer */
MODULE_AUTHOR ("");         /* forfatter(e) */

