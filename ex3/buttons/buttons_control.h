#ifndef BUTTONS_CONTROL_H
#define BUTTONS_CONTROL_H

#define BUTTON7 127
#define BUTTON6 191
#define BUTTON5 223
#define BUTTON4 239
#define BUTTON3 247
#define BUTTON2 251
#define BUTTON1 253
#define BUTTON0 254
#define NONE 255

//int main (int argc, char *argv[]);

int read_button_status(void);
void open_buttons_driver(void);
void close_buttons_driver(void);
void debounce(void);


#endif

