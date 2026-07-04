#ifndef PIXELS_H
#define PIXELS_H

#include "bmp_structs.h"
#include <stdio.h>
#include <stdlib.h>

RGB **save_pixels(const Pixels *, size_t, size_t, size_t, size_t);
RGB **allocate_pixels(size_t, size_t);
void write_pixels(Pixels *, int, int, RGB **, size_t, size_t);
RGB **rotate_pixels(const RGB **, size_t, size_t);
void free_pixels_arr(RGB **);
void repaint_pixel(RGB *, const RGB *);
void draw_pixel(Pixels *, int, int, int, const RGB *);

#endif