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

#define A4 440.00
#define B4 493.88
#define C5 523.25
#define D5 587.33
#define E5 659.26
#define F5 698.46
#define G5 783.99

const float A8 = 4*A4;
const float B8 = 4*B4;
const float C9 = 4*C5;
const float D9 = 4*D5;
const float E9 = 4*E5;
const float F9 = 4*F5;
const float G9 = 4*G5;
//const float A9 = 5*A4;

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

const int SOUND0 = 0;
const int SOUND1 = 1;
const int SOUND2 = 2;
const int SOUND3 = 3;
const int SOUND4 = 4;
const int SOUND5 = 5;
const int SOUND6 = 6;
const int SOUND7 = 7;

volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;
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

#endif
