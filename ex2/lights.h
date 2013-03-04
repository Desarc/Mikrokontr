#ifndef LIGHTS_H
#define LIGHTS_H

#include <avr32/ap7000.h>
#include <sys/interrupts.h>

#define ABDAC_INT_LEVEL 0
#define BUTTONS_INT_LEVEL 0

volatile avr32_pio_t *pioc = &AVR32_PIOC;
volatile avr32_pio_t *piob = &AVR32_PIOB;
volatile avr32_pio_t *intc = &AVR32_INTC;

int main (int argc, char *argv[]);
static void initLeds(void);
static void initButtons(void);
static void initInterrupts(void);
static void goLeft(void);
static void goRight(void);
static void debounce(void);
static void clearLeds(void);

static void interruptRoutine(void);

#endif
