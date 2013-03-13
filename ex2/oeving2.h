#ifndef OEVING_2_H
#define OEVING_2_H

#include <avr32/ap7000.h>
#include <sys/interrupts.h>
#include <stdlib.h>
#include <math.h>

#define ABDAC_INT_LEVEL 0
#define BUTTONS_INT_LEVEL 0

/* Various */
volatile int LED_VECTOR = 0x0;
const int SET_ALL = 0xff;

/* Pointers */
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pm_t *pm = &AVR32_PM;
volatile avr32_abdac_t *dac = &AVR32_ABDAC;

/* Button masks */
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

/* Prototypes */
int main (int argc, char *argv[]);
static void initIntc(void);
static void initButtons(void);
static void initLeds(void);
static void initAudio(void);
static void initHardware (void);
static void button_isr(void);
static void abdac_isr(void);
static void debounce(void);
static void pregenerateTones(void);
static void generateSine(float f);
static void generateTriangle(float f);
static void init_sound(void);
static void set_tone(float tone);
static void set_sample_size(float tone);
static void set_tone_length(float tone, float length);

#endif
