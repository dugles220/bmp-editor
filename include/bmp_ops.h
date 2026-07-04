#ifndef BMP_OPS_H
#define BMP_OPS_H

#include "bmp_io.h"
#include "draw.h"
#include "pixels.h"

#include <string.h>

BMP *bmp_copy(const BMP *);
BMP *bmp_rgb_filter(const BMP *, char, unsigned char);
BMP *bmp_rotate(const BMP *, size_t, size_t, size_t, size_t, size_t);
void draw_square(Pixels *, size_t, size_t, size_t, size_t, int, int, const RGB *, const RGB *);

void bmp_mirror(Pixels *p, size_t lu_x, size_t lu_y, size_t side_size, unsigned char axis);
void bmp_diag_mirror(Pixels *p, size_t lu_x, size_t lu_y, size_t side_size);
void draw_square_rhombus(Pixels *p, int uv_x, int uv_y, size_t side_size, const RGB *color);

int check_rotate_coordinates(const Pixels *, size_t, size_t, size_t, size_t);

#endif