#ifndef SCREEN_H
#define SCREEN_H

#define WALL '#'
#define BLANK ' '
#define TARGET '.'
#define PLAYER '@'
#define BOX '$'
#define PLAYER_ON_TARGET '+'
#define BOX_ON_TARGET '*'

void open_screen_driver(void);
void close_screen_driver(void);
void load_sokoban_images(void);
void clear_screen(void);
static void read_image_data(char image_path[], char *pixel_ptr, int height, int width);
static void generate_random_image(char *pixel_ptr);
void display_tile(char image, int tilePosX, int tilePosY, int dim);
static void write_to_screen(char *pixel_ptr, int posX, int posY, int height, int width);


#endif
