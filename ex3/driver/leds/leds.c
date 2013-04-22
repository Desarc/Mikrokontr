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

void set_leds(void);

dev_t dev;
int first_minor = 0, count = 1;
const char name[] = "leds";
struct cdev *leds_cdev;

const int SET_ALL_LEDS = 0xffffff00;
volatile long led_status = 0;


volatile avr32_pio_t *piob = &AVR32_PIOB;

/* fops-struct */

static struct file_operations leds_fops = {
  .owner = THIS_MODULE,
  .read = leds_read,
  .write = leds_write,
  .open = leds_open,
  .release = leds_release
};

void set_leds(void) {
	//printk("Writing %i to LED driver.\n", led_status);
	piob->codr = SET_ALL_LEDS;
	piob->sodr = led_status;

}

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */

static int __init leds_init (void) {

	/* allocating minor and major numbers */
	int alloc_success = -1;
  	alloc_success = alloc_chrdev_region(&dev, first_minor, count, name);
	printk("alloc success? %i \n", alloc_success);

  	/* be om tilgang til I/O-porter */
	int available = check_region(AVR32_PIOB_ADDRESS, AVR32_PIOB_IRQ);
	printk("region available? %i\n", available);
	request_region(AVR32_PIOB_ADDRESS, AVR32_PIOB_IRQ, name);

	/* initialisere PIO-maskinvaren (som i øving 2) */
	
	piob->per = SET_ALL_LEDS;
	piob->oer = SET_ALL_LEDS;
	piob->codr = SET_ALL_LEDS;
	
	/* registrere device i systemet (må gjøres når alt annet er initialisert) */
	int cdev_success;
	leds_cdev = cdev_alloc();
	leds_cdev->ops = &leds_fops;
	cdev_init(leds_cdev, &leds_fops);
	leds_cdev->owner = THIS_MODULE;
	cdev_success = cdev_add(leds_cdev, dev, count);
	printk("cdev_add success? %i \n", cdev_success);

	printk(KERN_ALERT "LED module enabled.\n");

  	return 0;
}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit leds_exit (void) {

	cdev_del(leds_cdev);

	release_region(AVR32_PIOB_ADDRESS, AVR32_PIOB_IRQ);

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
	long num;
	buff++;
	num = *buff;
	led_status = num << 32;
	buff++;
	num = *buff;
	led_status = num << 16;
	buff++;
	num = *buff;
	led_status += num << 8;
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

