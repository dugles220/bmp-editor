#ifndef BMP_OPS_H
#define BMP_OPS_H

#include "bmp_io.h"
#include "draw.h"
#include "pixels.h"

#include <string.h>

BMP *bmp_copy(const BMP *bmp_to_copy);
BMP *bmp_rgb_filter(const BMP *bmp, char rgb_comp_value, unsigned char rgb_comp_name);
BMP *bmp_rotate(const BMP *bmp, size_t left_up_x, size_t left_up_y, size_t right_down_x, size_t right_down_y, size_t rotates);
void draw_square(Pixels *pixels, size_t left_up_corner_x, size_t left_up_corner_y, size_t side_size, size_t thickness, int to_fill, int with_diag, const RGB *sides_color, const RGB *fill_color);

void bmp_mirror(Pixels *pixels, size_t left_up_x, size_t left_up_y, size_t side_size, unsigned char axis);
void bmp_diag_mirror(Pixels *pixels, size_t left_up_x, size_t left_up_y, size_t side_size);
void draw_square_rhombus(Pixels *pixels, int upper_vertex_x, int upper_vertex_y, size_t side_size, const RGB *color);

int check_rotate_coordinates(const Pixels *pixels, size_t left_up_x, size_t left_up_y, size_t right_down_x, size_t right_down_y);

#endif