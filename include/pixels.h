#ifndef PIXELS_H
#define PIXELS_H

#include "bmp_structs.h"
#include <stdio.h>
#include <stdlib.h>

RGB **save_pixels(const Pixels *pixels, size_t left_up_x, size_t left_up_y, size_t right_down_x, size_t right_down_y);
RGB **allocate_pixels(size_t height, size_t width);
void write_pixels(Pixels *image, int left_up_x, int left_up_y, RGB **pixels, size_t height, size_t width);
RGB **rotate_pixels(const RGB **pixels, size_t height, size_t width);
void free_pixels_arr(RGB **pixels);
void repaint_pixel(RGB *pixel, const RGB *color);
void draw_pixel(Pixels *pixels, int x, int y, int thickness, const RGB *color);

#endif