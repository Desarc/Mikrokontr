#ifndef LIGHTS_H
#define LIGHTS_H

#include <avr32/ap7000.h>
#include <sys/interrupts.h>

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

void interruptRoutine(void);

#endif
