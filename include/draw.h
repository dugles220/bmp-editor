#ifndef DRAW_H
#define DRAW_H

#include "bmp_structs.h"
#include "pixels.h"
#include "stack.h"

int check_coordinates(const Pixels *, int x, int y);

void draw_line(Pixels *pixels, int start_x, int start_y, int end_x, int end_y, int thickness, const RGB *color);
void scanline_fill(Pixels *pixels , size_t x, size_t y, const RGB *fill_color, const RGB *bound_color);

int are_colors_equal(const RGB *color1, const RGB *color2);
void check_line(FillStack *stack, Pixels *pixels, size_t x_start, size_t x_end, int y, const RGB *bound_color, const RGB *fill_color);
int sign(int num);

#endif