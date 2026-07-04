#ifndef BMP_IO_H
#define BMP_IO_H

#include "bmp_structs.h"
#include "pixels.h"

size_t get_padding(const BMP *bmp);
int bmp_is_format_correct(const BMP *bmp);
BMP *bmp_create();
BMP *bmp_read(const char *filename);
int bmp_write(const BMP *bmp , const char *filename);

void bmp_print_info(const BMP *bmp);
void pixels_free(Pixels *pixels);
void bmp_free(BMP *bmp);

#endif