#ifndef BUTTONS_CONTROL_H
#define BUTTONS_CONTROL_H

#define BUTTON7 1919
#define BUTTON6 1983
#define BUTTON5 2015
#define BUTTON4 2031
#define BUTTON3 2039
#define BUTTON2 2043
#define BUTTON1 2045
#define BUTTON0 2046
#define NONE 2047

//int main (int argc, char *argv[]);

int read_button_status(void);
void open_buttons_driver(void);
void close_buttons_driver(void);
void debounce(void);


#endif

