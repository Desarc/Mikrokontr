#ifndef LEDS_CONTROL_H
#define LEDS_CONTROL_H

#define LED7 0x080000 //not working
#define LED6 0x000100
#define LED5 0x000080
#define LED4 0x000040
#define LED3 0x000020
#define LED2 0x000004
#define LED1 0x000002
#define LED0 0x000001

//int main (int argc, char *argv[]);
void increment_leds(void);
void decrement_leds(void);
void reset_leds(void);
static void set_led_on(int led);
static void set_led_off(int led);
static void write_to_led_driver(void);
void open_led_driver(void);
void close_led_driver(void);


#endif

