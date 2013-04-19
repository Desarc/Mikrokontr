#include <linux/init.h>
#include <linux/module.h>
#include <linux/fb.h>
//#include <sys/mman.h> //for mmap


MODULE_LICENSE("Dual BSD/GPL");

//Framebuffer memlocation: "102ba000-1034ffff: Framebuffer" viser i minicom ved oppstart.


static int framebuffer_init(void);
static void framebuffer_exit(void);

static int framebuffer_init(void)
{
	printk(KERN_ALERT "Hello world!\nFrame buffer test1\n");


	//Mitt forsøk på å få liv i skjermen//




	//Map framebuffer til minne
	int mapaddr = 0; 			//adresse til den nye mappingen.
	int addr = 0x102ba000; 			//Hentet fra minicom
	int length = 0x1034ffff - addr; 	//lengde på adresseområdet

	//mapaddr = mmap(NULL, length, 


	printk(KERN_ALERT "Length: %d\n", length);







	return 0;
}

static void framebuffer_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world!\n");
}

module_init(framebuffer_init);
module_exit(framebuffer_exit);



