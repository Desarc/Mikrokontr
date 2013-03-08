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

#define ABDAC_INT_LEVEL 0
#define BUTTONS_INT_LEVEL 0

/* frequency levels for various tones */
#define A4 440.00
#define B4 493.88
#define C5 523.25
#define D5 587.33
#define E5 659.26
#define F5 698.46
#define G5 783.99
#define A5 880.00
#define B5 987.77
#define C6 1046.50
#define D6 1174.66
#define E6 1318.51
#define F6 1396.91
#define G6 1567.98
#define A6 1760.00
#define B6 1975.53
#define C7 2093.00
#define D7 2349.32
#define E7 2637.02
#define F7 2793.83
#define G7 3135.96
#define A7 3520.00


/* tone lengths */
#define Q 50
#define H 100
#define F 200

const int SET_ALL = 0xff;

/* button masks */
const int BUTTON7 = 0xffff3f7f;
const int BUTTON6 = 0xffff3fbf;
const int BUTTON5 = 0xffff3fdf;
const int BUTTON4 = 0xffff3fef;
const int BUTTON3 = 0xffff3ff7;
const int BUTTON2 = 0xffff3ffb;
const int BUTTON1 = 0xffff3ffd;
const int BUTTON0 = 0xffff3ffe;

/* LED vector values */
const int LED7 = 0x80;
const int LED6 = 0x40;
const int LED5 = 0x20;
const int LED4 = 0x10;
const int LED3 = 0x08;
const int LED2 = 0x04;
const int LED1 = 0x02;
const int LED0 = 0x01;

/* sound identifiers */
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


/* prototypes */
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
static void generate_tone(float f);
static void init_sound(void);
static void set_tone(float tone);

#endif
