#ifndef LEDS_CONTROL_H
#define LEDS_CONTROL_H

#define LED7 0x400000 //not working
#define LED6 0x0100
#define LED5 0x0080
#define LED4 0x0040
#define LED3 0x0020
#define LED2 0x0004
#define LED1 0x0002
#define LED0 0x0001

void increment_leds(void);
void decrement_leds(void);
int get_led_status(void);
void reset_leds(void);
static void set_led_on(int led);
static void set_led_off(int led);
static void write_to_led_driver(void);
void open_led_driver(void);
void close_led_driver(void);


#endif

