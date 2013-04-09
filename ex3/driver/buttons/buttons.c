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

static int __init buttons_init(void);
static void __exit buttons_exit(void);
static int buttons_open (struct inode *inode, struct file *filp);
static int buttons_release (struct inode *inode, struct file *filp);
static ssize_t buttons_read (struct file *filp, char __user *buff,
                     size_t count, loff_t *offp);
static ssize_t buttons_write (struct file *filp, const char __user *buff,
                      size_t count, loff_t *offp);


dev_t first_major;
int first_minor = 0, count = 1;
const char name[] = "buttons";
struct resource;

/* fops-struct */

static struct file_operations driver_fops = {
  .owner = THIS_MODULE,
  .read = buttons_read,
  .write = buttons_write,
  .open = buttons_open,
  .release = buttons_release
};

/*****************************************************************************/
/* init-funksjon (kalles når modul lastes) */

static int __init buttons_init (void) {

	/* allocating minor and major numbers */
  	alloc_chrdev_region(&first_major, first_minor, count, name);
	
	printk(KERN_ALERT "Hello world!\n");

  	/* be om tilgang til I/O-porter */
	//resource = *request_region();
  
  /* initialisere PIO-maskinvaren (som i øving 2) */
 
  /* registrere device i systemet (må gjøres når alt annet er initialisert) */

  return 0;
}

/*****************************************************************************/
/* exit-funksjon (kalles når modul fjernes fra systemet) */

static void __exit buttons_exit (void) {

	printk(KERN_ALERT "Goodbye, cruel world!\n");

	/* releasing minor and major numbers */
	unregister_chrdev_region(first_major, count);

	//release_region();
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

