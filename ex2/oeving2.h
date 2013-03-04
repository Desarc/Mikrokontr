/*****************************************************************************
 * 
 * Øving 2 UCSysDes
 *
 * Headerfil
 *
 *****************************************************************************/

#ifndef OEVING_2_H /* vanlig måte å unngå headerrekursjon på */
#define OEVING_2_H

#include <avr32/ap7000.h> /* inkludere Atmel-headerfil */
#include <sys/interrupts.h>
#include <stdlib.h>
#include <math.h>

/* legg andre "includes" her, sammen med "defines" og prototyper */
#define ABDAC_INT_LEVEL 0
#define BUTTONS_INT_LEVEL 0

const int SET_ALL = 0xff;
const int BUTTON7 = 0xffff3f7f;
const int BUTTON6 = 0xffff3fbf;
const int BUTTON5 = 0xffff3fdf;
const int BUTTON4 = 0xffff3fef;
const int BUTTON3 = 0xffff3ff7;
const int BUTTON2 = 0xffff3ffb;
const int BUTTON1 = 0xffff3ffd;
const int BUTTON0 = 0xffff3ffe;

const int LED7 = 0x80;
const int LED6 = 0x40;
const int LED5 = 0x20;
const int LED4 = 0x10;
const int LED3 = 0x08;
const int LED2 = 0x04;
const int LED1 = 0x02;
const int LED0 = 0x01;

const int TEST = 0;

const int SAMPLE_RATE = 4000;
const int SAMPLE_SIZE = 8;
const int channels = 4;

volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *sm = &AVR32_PM;
volatile avr32_abdac_t *dac = &AVR32_ABDAC;


/* prototyper */
int main (int argc, char *argv[]);
static void initIntc(void);
static void initButtons(void);
static void initLeds(void);
static void initAudio(void);
static void initHardware (void);

static void button_isr(void);
static void abdac_isr(void);
static void debounce(void);
static void playSound(int code);
static void miniWait(void);

#endif
