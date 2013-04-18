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

void init_switches(void);
char btn_read(void);

dev_t dev_no, dev;
static int Major;
struct cdev *leds_cdev;
volatile avr32_pio_t *piob = &AVR32_PIOB;

/* fops-struct */
static struct file_operations leds_fops = {
	.owner = THIS_MODULE,
	.read = leds_read,
	.write = leds_write,
	.open = leds_open,
	.release = leds_release
};

/* init-funksjon (kalles når modul lastes) */
static int __init leds_init (void) {
	int ret;

	/* allocating minor and major numbers */
	ret = alloc_chrdev_region(&dev_no,0,1,"leds");
	if (ret < 0) {
		printk(KERN_INFO "Major num fail");
		return ret;
	}

  	/* be om tilgang til I/O-porter */
	request_region(AVR32_PIOB_ADDRESS, AVR32_PIOB_IRQ, "leds");
  
	/* initialisere PIO-maskinvaren (som i øving 2) */
	init_switches();

	/* registrere device i systemet (må gjøres når alt annet er initialisert) */
	Major = MAJOR(dev_no);
	dev = MKDEV(Major,0);
	printk("The major number is %d\n", Major);

	leds_cdev = cdev_alloc();
	leds_cdev->ops = &leds_fops;
	leds_cdev->owner = THIS_MODULE;
	ret = cdev_add(leds_cdev,dev,1);
	if (ret < 0) {
		printk(KERN_INFO "Unable to add cdev");
		return ret;
	}
	return 0;
}

/* exit-funksjon (kalles når modul fjernes fra systemet) */
static void __exit leds_exit (void) {
	/* releasing minor and major numbers */
	cdev_del(leds_cdev);
	release_region(AVR32_PIOB_ADDRESS, AVR32_PIOB_IRQ);
	unregister_chrdev_region(dev_no, 1);
	printk(KERN_ALERT "LED module disabled.\n");
}

/*
void init_leds() {
	piob->per = 0xffffffff;
	piob->oer = 0xffffffff;
	piob->codr = 0xffffffff;
}
*/

void init_switches(void) {
	piob->per = 0xffffffff;
	piob->puer = 0xffffffff;
	piob->ier = 0xffffffff;
}

char btn_read(void) {
	char sw = piob->pdsr;
	volatile int i;
	for (i = 0; i < 1000; ++i) 
	;	
	sw &= piob->pdsr;
	return sw;
}

/* fops-funksjoner */
static int leds_open (struct inode *inode, struct file *filp) {
	printk(KERN_INFO "Inside open \n");
  	return 0;
}

static int leds_release (struct inode *inode, struct file *filp) {
	printk(KERN_INFO "Inside release \n");
	return 0;
}

static ssize_t leds_read (struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	volatile int j;
	for (j = 0; j < count; ++j) {
		*buff++ = btn_read();
	}
	return count;
}

static ssize_t leds_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	return 0;
}

/* modul-beskrivelsesmakroer */
module_init (leds_init);  /* angir hva som er init-funksjon */
module_exit (leds_exit);  /* angir hva som er exit-funksjon */

MODULE_LICENSE ("GPL");     /* programlisens for modulen */
MODULE_DESCRIPTION ("support for leds");    /* tekstlig beskrivelse */
MODULE_VERSION ("");        /* versjonsnummer */
MODULE_AUTHOR ("");         /* forfatter(e) */
