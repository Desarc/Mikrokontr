#ifndef SCREEN_H
#define SCREEN_H

void open_driver(void);
void close_driver(void);
void read_image_data(char image_path[]);
void generate_random_image(void);
void write_to_screen(void);


#endif
