/*****************************************************************************
 *
 * Øving 3 uCSysDes, driverkoden
 *
 *****************************************************************************/

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

/* prototyper */

static int __init leds_init(void);
static void __exit leds_exit(void);
static int leds_open (struct inode *inode, struct file *filp);
static int leds_release (struct inode *inode, struct file *filp);
static ssize_t leds_read (struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t leds_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp);

void set_leds();
void set_led_on(int led);
void set_led_off(int led);

dev_t dev;
int first_minor = 0, count = 1;
const char name[] = "leds";
const int SET_ALL = 0xffffffff;
struct cdev *leds_cdev;
volatile int led_status = 0;
const int led0 = 0x100;
const int led1 = 0x200;
const int led2 = 0x400;
const int led5 = 0x8000;


volatile avr32_pio_t *piob = &AVR32_PIOB;

/* fops-struct */

static struct file_operations leds_fops = {
  .owner = THIS_MODULE,
  .read = leds_read,
  .write = leds_write,
  .open = leds_open,
  .release = leds_release
};

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */

static int __init leds_init (void) {

	/* allocating minor and major numbers */
	int alloc_success = -1;
  	alloc_success = alloc_chrdev_region(&dev, first_minor, count, name);
	printk("alloc success? %i \n", alloc_success);

  	/* be om tilgang til I/O-porter */

	request_region(AVR32_PIOB_ADDRESS, AVR32_PIOB_IRQ, name);
	//request_region(AVR32_PIOB_ADDRESS, 102400, name);

	/* initialisere PIO-maskinvaren (som i øving 2) */

	
	/* I have no idea what I'm doing from this point */
	
	piob->per = SET_ALL;
	piob->oer = SET_ALL;

	//led_status = kmalloc(1, GFP_KERNEL);

	//memset(led_status, 0, 1);
	
	
	/*outw(SET_ALL, leds_per);
	outw(SET_ALL, leds_oer);
	outw(SET_ALL, leds_codr);
	outw(0x01, leds_sodr);*/

	/* registrere device i systemet (må gjøres når alt annet er initialisert) */
	//int cdev_success = -1;
	//leds_cdev = cdev_alloc();
	//leds_cdev->ops = &leds_fops;
	//cdev_init(leds_cdev, &leds_fops);
	//leds_cdev->owner = THIS_MODULE;
	//cdev_success = cdev_add(leds_cdev, dev, count);
	//cdev_success = register_chrdev(dev, name, &leds_fops);
	//printk("cdev_add success? %i \n", cdev_success);
	set_led_on(led1);
	set_led_off(led1);
	set_led_on(led2);
	//outw(SET_ALL, piob->codr);
	//outw(0x01, piob->sodr);

	printk(KERN_ALERT "LED module enabled.\n");

  	return 0;
}

void set_led_on(int led) {
	led_status += led;
	set_leds();
}

void set_led_off(int led) {
	led_status -= led;
	set_leds();
}

void set_leds() {
	piob->codr = SET_ALL;
	piob->sodr = led_status;

}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit leds_exit (void) {

	//cdev_del(leds_cdev);
	release_region(AVR32_PIOB_ADDRESS, AVR32_PIOB_IRQ);
	//release_region(AVR32_PIOB_ADDRESS, 102400);

	/* releasing minor and major numbers */
	unregister_chrdev_region(dev, count);
	

	printk(KERN_ALERT "LED module disabled.\n");
}

/*****************************************************************************/
/* fops-funksjoner */

static int leds_open (struct inode *inode, struct file *filp) {
	
	//filp->private_data = leds_cdev;


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

