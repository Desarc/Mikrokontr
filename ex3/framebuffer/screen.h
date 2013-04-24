#ifndef SCREEN_H
#define SCREEN_H

#define WIN 0
#define SPLASH 1
#define END 3

#define MAX_IMAGE_SIZE 76800
#define MAX_BUFFER_SIZE (MAX_IMAGE_SIZE*3)
#define MAX_HEIGHT 240
#define MAX_WIDTH 320
#define TILE_SIZE_8 64
#define TILE_SIZE_16 256
#define TILE_SIZE_32 1024
#define TILE_SIZE TILE_SIZE_16
#define TILE_DIM_8 8
#define TILE_DIM_16 16
#define TILE_DIM_32 32
#define TILE_DIM TILE_DIM_16

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
