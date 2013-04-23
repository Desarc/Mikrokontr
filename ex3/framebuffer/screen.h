#ifndef SCREEN_H
#define SCREEN_H

#define WIN 0
#define SPLASH 1

void open_screen_driver(void);
void close_screen_driver(void);
void load_sokoban_images(void);
void clear_screen(void);
static void read_image_data(char image_path[], char *pixel_ptr, int height, int width);
static void generate_random_image(char *pixel_ptr);
void display_image(int image);
void display_tile(char image, int tilePosX, int tilePosY, int dim);
static void write_to_screen(char *pixel_ptr, int posX, int posY, int height, int width);

#endif
