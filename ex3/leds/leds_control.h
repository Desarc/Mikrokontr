#ifndef LEDS_CONTROL_H
#define LEDS_CONTROL_H

// define the values of the leds for the driver
#define LED7 0x80
#define LED6 0x40
#define LED5 0x20
#define LED4 0x10
#define LED3 0x80
#define LED2 0x04
#define LED1 0x02
#define LED0 0x01
#define ALL_LEDS 0xff

void increment_leds(void);
void decrement_leds(void);
void blink_leds(void);
int get_led_status(void);
void reset_leds(void);
static void set_led_on(int led);
static void set_led_off(int led);
static void write_to_led_driver(void);
void open_led_driver(void);
void close_led_driver(void);

#endif
