#ifndef BMP_STRUCTS_H
#define BMP_STRUCTS_H

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t signature;
    uint32_t filesize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelArrOffset;

} BitmapFileHeader;

typedef struct {
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t xPixelsPerMeter;
    uint32_t yPixelsPerMeter;
    uint32_t colorsInColorTable;
    uint32_t importantColorCount;

} BitmapInfoHeader;

#pragma pack(pop)

typedef struct {
    unsigned char b;
    unsigned char g;
    unsigned char r;

} RGB;

typedef struct {
    RGB **pix_arr;

    uint32_t H;
    uint32_t W;

} Pixels;

typedef struct {
    Pixels *pixels;
    BitmapFileHeader *file_header;
    BitmapInfoHeader *info_header;
} BMP;

#endif