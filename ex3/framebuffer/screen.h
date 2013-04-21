#ifndef SCREEN_H
#define SCREEN_H

void open_screen_driver(void);
void close_screen_driver(void);
void load_sokoban_images(void);
static void read_image_data(char image_path[], int *image_ptr, int height, int width);
static void generate_random_image(void);
static write_to_screen(int *image_ptr, int posX, int posY, int height, int width);


#endif
