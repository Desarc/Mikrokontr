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


#define PORT_RANGE 8
#define PORT_OFFSET 0

/* prototyper */

static int __init buttons_init(void);
static void __exit buttons_exit(void);
static int buttons_open (struct inode *inode, struct file *filp);
static int buttons_release (struct inode *inode, struct file *filp);
static ssize_t buttons_read (struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t buttons_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp);

void button_isr(void);

dev_t dev;
int first_minor = 0, count = 1;
const char name[] = "buttons";
struct cdev *buttons_cdev;
int status;

const int SET_ALL_BUTTONS = 0xff;
volatile char button_status[2];

volatile avr32_pio_t *piob = &AVR32_PIOB;

/* fops-struct */

static struct file_operations buttons_fops = {
  .owner = THIS_MODULE,
  .read = buttons_read,
  .write = buttons_write,
  .open = buttons_open,
  .release = buttons_release
};

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */

void read_button_status(void) {
	piob->isr;
	int status = piob->pdsr;
	button_status[0] = status >> 8;
	button_status[1] = status;
}

static int __init buttons_init (void) {

	/* allocating minor and major numbers */
	
  	status = alloc_chrdev_region(&dev, first_minor, count, name);
	printk("alloc success? %i\n", status);
	
	
  	/* be om tilgang til I/O-porter */
	
	//status = check_region(AVR32_PIOB_ADDRESS+PORT_OFFSET, PORT_RANGE);
	//printk("region available? %i\n", status);
  	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PER, PORT_RANGE, name);
	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PUER, PORT_RANGE, name);
	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_IER, PORT_RANGE, name);
	request_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PDSR, PORT_RANGE, name);
  	
	/* initialisere PIO-maskinvaren (som i øving 2) */

	piob->per = SET_ALL_BUTTONS;
	piob->puer = SET_ALL_BUTTONS;
	piob->ier = SET_ALL_BUTTONS;
  	//register_interrupt(button_isr, AVR32_PIOC_IRQ/32, AVR32_PIOC_IRQ % 32, BUTTONS_INT_LEVEL);
	piob->isr;	
	//init_interrupts();
 
  	/* registrere device i systemet (må gjøres når alt annet er initialisert) */

	
	buttons_cdev = cdev_alloc();
	buttons_cdev->ops = &buttons_fops;
	cdev_init(buttons_cdev, &buttons_fops);
	buttons_cdev->owner = THIS_MODULE;
	status = cdev_add(buttons_cdev, dev, count);
	printk("cdev_add success? %i \n", status);

	printk(KERN_ALERT "Button module enabled.\n");

  return 0;
}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit buttons_exit (void) {

	cdev_del(buttons_cdev);

	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PER, PORT_RANGE);
	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PUER, PORT_RANGE);
	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_IER, PORT_RANGE);
	release_region(AVR32_PIOB_ADDRESS+AVR32_PIO_PDSR, PORT_RANGE);

	/* releasing minor and major numbers */
	unregister_chrdev_region(dev, count);

	printk(KERN_ALERT "Buttons module disabled.\n");
}

/*****************************************************************************/
/* fops-funksjoner */

static int buttons_open (struct inode *inode, struct file *filp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static int buttons_release (struct inode *inode, struct file *filp) {
  return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t buttons_read (struct file *filp, char __user *buff,
              size_t count, loff_t *offp) {

	read_button_status();
	copy_to_user(buff, &button_status, count);
	return 0;
}

/*---------------------------------------------------------------------------*/

static ssize_t buttons_write (struct file *filp, const char __user *buff,
               size_t count, loff_t *offp) {
  return 0;
}

/*****************************************************************************/
/* modul-beskrivelsesmakroer */

module_init (buttons_init);  /* angir hva som er init-funksjon */
module_exit (buttons_exit);  /* angir hva som er exit-funksjon */

MODULE_LICENSE ("GPL");     /* programlisens for modulen */
MODULE_DESCRIPTION ("support for buttons");    /* tekstlig beskrivelse */
MODULE_VERSION ("");        /* versjonsnummer */
MODULE_AUTHOR ("");         /* forfatter(e) */

