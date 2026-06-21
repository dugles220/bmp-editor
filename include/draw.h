#ifndef DRAW_H
#define DRAW_H

#include "bmp_structs.h"
#include "pixels.h"
#include "stack.h"

int check_coordinates(const Pixels *, int, int);

void draw_line(Pixels *, int, int, int, int, int, const RGB *);
void scanline_fill(Pixels *, size_t, size_t, const RGB *, const RGB *);

int are_colors_equal(const RGB *, const RGB *);
void check_line(FillStack *, Pixels *, size_t, size_t, int, const RGB *, const RGB *);
int sign(int);

#endif