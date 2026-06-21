#include "../include/pixels.h"

RGB **save_pixels(const Pixels *pixels, size_t lu_x, size_t lu_y, size_t rd_x, size_t rd_y)
{
    RGB **pixels_arr = pixels->pix_arr;

    size_t area_h = rd_y - lu_y;
    size_t area_w = rd_x - lu_x;

    RGB **new_pixels = allocate_pixels(area_h, area_w);
    if (new_pixels == NULL) {
        fprintf(stderr, "save_pixels: allocate_pixels returned NULL\n");
        return NULL;
    }

    for (size_t y = 0; y < area_h; y++) {
        for (size_t x = 0; x < area_w; x++) {
            size_t src_y = convert_index(pixels, lu_y + y);
            size_t src_x = lu_x + x;

            new_pixels[y][x] = pixels_arr[src_y][src_x];
        }
    }

    return new_pixels;
}

RGB **allocate_pixels(size_t h, size_t w)
{
    RGB **pixels = calloc(h, sizeof(RGB *));
    if (pixels == NULL) {
        perror("allocate_pixels: new_pixels allocate memory failed");
        return NULL;
    }

    RGB *lines = malloc(h * w * sizeof(RGB));
    if (lines == NULL) {
        perror("allocate_pixels: new_lines allocate memory failed");
        free(pixels);
        return NULL;
    }

    for (size_t i = 0; i < h; i++) {
        pixels[i] = lines + w * i;
    }

    return pixels;
}

void write_pixels(Pixels *image, int lu_x, int lu_y, RGB **pixels, size_t h, size_t w)
{
    size_t image_W = image->W;
    size_t image_H = image->H;

    RGB **image_pixels = image->pix_arr;

    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            int out_x = lu_x + (int)x;
            int out_y = lu_y + (int)y;

            size_t final_y = convert_index(image, (size_t)out_y);

            if (out_x >= 0 && out_x < (int)image_W && out_y >= 0 && out_y < (int)image_H) {
                image_pixels[final_y][out_x] = pixels[y][x];
            }
        }
    }
}

RGB **rotate_pixels(const RGB **pixels, size_t h, size_t w)
{
    RGB **new_pixels = allocate_pixels(w, h);
    if (new_pixels == NULL) {
        fprintf(stderr, "rotate_pixels: allocate_pixels returned NULL\n");
        return NULL;
    }

    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            size_t src_y = y;
            size_t src_x = x;

            size_t dst_y = (w - 1) - x;
            size_t dst_x = y;

            new_pixels[dst_y][dst_x] = pixels[src_y][src_x];
        }
    }

    return new_pixels;
}

void free_pixels_arr(RGB **pixels)
{
    if (pixels != NULL) {
        if (pixels[0] != NULL) {
            free(pixels[0]);
        }
        free(pixels);
    }
}

void repaint_pixel(RGB *pixel, const RGB *color)
{
    pixel->b = color->b;
    pixel->g = color->g;
    pixel->r = color->r;
}

size_t convert_index(const Pixels *pixels, size_t y)
{
    return (pixels->H - 1) - y;
}