#ifndef BMP_IO_H
#define BMP_IO_H

#include "bmp_structs.h"
#include "pixels.h"
#include <stdio.h>
#include <stdlib.h>

size_t get_padding(const BMP *);
int bmp_is_format_correct(const BMP *);
BMP *bmp_create();
BMP *bmp_read(const char[]);
int bmp_write(const BMP *, const char[]);

void bmp_print_info(const BMP *);
void pixels_free(Pixels *);
void bmp_free(BMP *);

#endif