/**
 * @file bmp_structs.h
 * @brief Набор структур для хранения информации об изображении формата BMP
 */

#ifndef BMP_STRUCTS_H
#define BMP_STRUCTS_H

#include <stdint.h>

#pragma pack(push, 1)

/**
 * @brief Структура заголовка BitmapFileHeader формата BMP
 * 
 */
typedef struct {
    uint16_t signature;
    uint32_t filesize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelArrOffset;

} BitmapFileHeader;

/**
 * @brief Структура заголовка BitmapInfoHeader формата BMP
 * 
 */
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

/**
 * @brief Структура, хранящая информацию об одном пикселе 
 * 
 * Хранит значения каждой из RGB компоненты цвета.
 * 
 */
typedef struct {
    unsigned char b; ///< Компонента blue 
    unsigned char g; ///< Компонента green
    unsigned char r; ///< Компонента red

} RGB;
/**
 * @brief Структура, представляющая из себя полотно пикселей
 * 
 * Хранит высоту, ширину и двумерный массив структур RGB.
 * 
 */
typedef struct {
    RGB **pix_arr; ///< Двумерный массив структур RGB (пикселей)

    uint32_t H; ///< Высота полотна
    uint32_t W; ///< Ширина полотна

} Pixels;
/**
 * @brief Главная структура, представляющая изображение формата BMP
 * 
 * Хранит полотно пикселей и структуры заголовков формата BMP.
 * 
 */
typedef struct {
    Pixels *pixels; ///< Полотно пикселей
    BitmapFileHeader *file_header; ///< Заголовок File BMP 
    BitmapInfoHeader *info_header; ///< Заголовок Info BMP
} BMP;

#endif