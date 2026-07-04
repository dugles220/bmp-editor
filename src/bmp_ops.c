#include "../include/bmp_ops.h"
#include "enums.h"

#include <math.h>

enum spectrum { RED, GREEN, BLUE };

BMP *bmp_copy(const BMP *bmp_to_copy)
{
    if (bmp_to_copy == NULL) {
        fprintf(stderr, "bmp_copy: argument is a NULL pointer\n");
        return NULL;
    }

    BMP *new_bmp = bmp_create();

    if (new_bmp == NULL) {
        fprintf(stderr, "bmp_copy: bmp_create returned NULL\n");
        return NULL;
    }

    memcpy(new_bmp->file_header, bmp_to_copy->file_header, sizeof(BitmapFileHeader));
    memcpy(new_bmp->info_header, bmp_to_copy->info_header, sizeof(BitmapInfoHeader));

    Pixels *new_pixels = malloc(sizeof(Pixels));
    new_bmp->pixels = new_pixels;

    if (!new_pixels) {
        perror("bmp_copy: new Pixels struct malloc failed");
        bmp_free(new_bmp);
        return NULL;
    }

    new_pixels->H = bmp_to_copy->info_header->height;
    new_pixels->W = bmp_to_copy->info_header->width;

    size_t H = new_pixels->H;
    size_t W = new_pixels->W;

    RGB **arr = allocate_pixels(H, W);

    if (arr == NULL) {
        fprintf(stderr, "bmp_copy: allocate_pixels returned NULL.\n");
        bmp_free(new_bmp);
        return NULL;
    }

    new_pixels->pix_arr = arr;

    for (size_t i = 0; i < H; i++) {
        memcpy(arr[i], bmp_to_copy->pixels->pix_arr[i], W * sizeof(RGB));
    }

    return new_bmp;
}

BMP *bmp_rgb_filter(const BMP *bmp, char component, unsigned char value)
{
    if (bmp == NULL) {
        fprintf(stderr, "bmp_rgb_filter: bmp is a NULL pointer\n");
        return NULL;
    }

    enum spectrum color;

    switch (component) {
        case red:
            color = RED;
            break;
        case green:
            color = GREEN;
            break;
        case blue:
            color = BLUE;
            break;
        default:
            fprintf(stderr, "bmp_rgb_filter: invalid component argument: %c\n", component);
            return NULL;
    }

    BMP *new_bmp = bmp_copy(bmp);

    if (new_bmp == NULL) {
        fprintf(stderr, "bmp_rgb_filter: bmp_copy returned NULL\n");
        return NULL;
    }

    RGB **pixels = new_bmp->pixels->pix_arr;

    size_t H = new_bmp->pixels->H;
    size_t W = new_bmp->pixels->W;

    for (size_t i = 0; i < H; i++) {
        for (size_t j = 0; j < W; j++) {
            if (color == RED)
                pixels[i][j].r = value;
            if (color == GREEN)
                pixels[i][j].g = value;
            if (color == BLUE)
                pixels[i][j].b = value;
        }
    }

    return new_bmp;
}

BMP *bmp_rotate(const BMP *bmp, size_t lu_x, size_t lu_y, size_t rd_x, size_t rd_y, size_t rotates)
{
    if (bmp == NULL) {
        fprintf(stderr, "bmp_rotate: bmp is a NULL pointer\n");
        return NULL;
    }

    if (!check_rotate_coordinates(bmp->pixels, lu_x, lu_y, rd_x, rd_y)) {
        fprintf(stderr, "bmp_rotate: invalid coordinates\n");
        return NULL;
    }

    BMP *cpd_bmp = bmp_copy(bmp);

    if (cpd_bmp == NULL) {
        fprintf(stderr, "bmp_rotate: bmp_copy returned NULL\n");
        return NULL;
    }

    size_t area_h = rd_y - lu_y;
    size_t area_w = rd_x - lu_x;

    RGB **rotated_pixels = save_pixels(cpd_bmp->pixels, lu_x, lu_y, rd_x, rd_y);
    if (rotated_pixels == NULL) {
        fprintf(stderr, "bmp_rotate: save_pixels returned NULL\n");
        bmp_free(cpd_bmp);
        return NULL;
    }

    RGB **new_rotated_pixels;

    for (size_t i = 0; i < rotates; i++) {
        new_rotated_pixels = rotate_pixels((const RGB **)rotated_pixels, area_h, area_w);
        if (new_rotated_pixels == NULL) {
            fprintf(stderr, "bmp_rotate: rotate_pixels returned NULL\n");
            bmp_free(cpd_bmp);
            return NULL;
        }

        free_pixels_arr(rotated_pixels);
        rotated_pixels = new_rotated_pixels;

        size_t temp = area_h;
        area_h = area_w;
        area_w = temp;
    }

    int new_lu_x = lu_x;
    int new_lu_y = lu_y;

    if (rotates % 2 != 0) {
        double old_w = (double)(rd_x - lu_x);
        int center_y2 = (int)(lu_y + rd_y);

        new_lu_x = lu_x + (int)floor((old_w - area_w) / 2.0 + 0.5);
        new_lu_y = (center_y2 - (int)area_h) / 2;
    }

    write_pixels(cpd_bmp->pixels, new_lu_x, new_lu_y, rotated_pixels, area_h, area_w);

    free_pixels_arr(rotated_pixels);

    return cpd_bmp;
}

int check_rotate_coordinates(const Pixels *pixels, size_t lu_x, size_t lu_y, size_t rd_x, size_t rd_y)
{
    if (lu_x > rd_x || lu_y > rd_y) {
        fprintf(stderr, "check_rotate_coordinates: wrong angles coordinates\n");
        return 0;
    }

    size_t H = pixels->H;
    size_t W = pixels->W;

    if (lu_x > W || rd_x > W || lu_y > H || rd_y > H) {
        fprintf(stderr, "check_rotate_coordinates: coordinates are out of range\n");
        return 0;
    }

    return 1;
}

void draw_square(Pixels *pixels, size_t lu_x, size_t lu_y, size_t side_size, size_t thickness, int to_fill,
                 int diag, const RGB *color, const RGB *fill_color)
{
    size_t rd_x = lu_x + side_size;
    size_t rd_y = lu_y + side_size;

    draw_line(pixels, lu_x, lu_y, rd_x, lu_y, thickness, color); // верх
    draw_line(pixels, rd_x, lu_y, rd_x, rd_y, thickness, color); // право
    draw_line(pixels, rd_x, rd_y, lu_x, rd_y, thickness, color); // низ
    draw_line(pixels, lu_x, rd_y, lu_x, lu_y, thickness, color); // лево

    if (to_fill == 1) {
        size_t center_x = lu_x + ((rd_x - lu_x) / 2);
        size_t center_y = lu_y + ((rd_y - lu_y) / 2);

        scanline_fill(pixels, center_x, center_y, fill_color, color);
    }

    if (diag == 1) {
        draw_line(pixels, lu_x, lu_y, rd_x, rd_y, thickness, color);
        draw_line(pixels, lu_x, rd_y, rd_x, lu_y, thickness, color);
    }
}

void bmp_mirror(Pixels *p, size_t lu_x, size_t lu_y, size_t side_size, unsigned char axis)
{
    int mode = 0;

    RGB **pixels = p->pix_arr;

    RGB **mirrored = allocate_pixels(side_size, side_size);

    if (axis == 'x') {
        for (size_t y = 0; y < side_size; y++) {
            memcpy(mirrored[side_size - y - 1], &pixels[convert_index(p, y + lu_y)][lu_x],
                   side_size * sizeof(RGB));
        }

        mode = 1;
    } else if (axis == 'y') {
        for (size_t y = 0; y < side_size; y++) {
            for (size_t x = 0; x < side_size; x++) {
                repaint_pixel(&mirrored[y][x], &pixels[convert_index(p, y + lu_y)][lu_x + side_size - x - 1]);
            }
        }

        mode = 1;
    }

    if (mode == 1)
        write_pixels(p, lu_x, lu_y, mirrored, side_size, side_size);

    free_pixels_arr(mirrored);
}

void bmp_diag_mirror(Pixels *p, size_t lu_x, size_t lu_y, size_t side_size)
{
    RGB **pixels = p->pix_arr;

    RGB **mirrored = allocate_pixels(side_size, side_size);

    for (size_t y = 0; y < side_size; y++) {
        for (size_t x = 0; x < side_size; x++) {
            repaint_pixel(&mirrored[x][y], &pixels[convert_index(p, y + lu_y)][x + lu_x]);
        }
    }

    write_pixels(p, lu_x, lu_y, mirrored, side_size, side_size);

    free_pixels_arr(mirrored);
}

void draw_square_rhombus(Pixels *p, int uv_x, int uv_y, size_t side_size, const RGB *color)
{
    int square_half_side = sqrt(side_size * side_size / 2);

    draw_line(p, uv_x, uv_y, uv_x + square_half_side, uv_y + square_half_side, 1, color);
    draw_line(p, uv_x, uv_y, uv_x - square_half_side, uv_y + square_half_side, 1, color);
    draw_line(p, uv_x, uv_y + square_half_side * 2, uv_x + square_half_side, uv_y + square_half_side, 1, color);
    draw_line(p, uv_x, uv_y + square_half_side * 2, uv_x - square_half_side, uv_y + square_half_side, 1, color);

    scanline_fill(p, uv_x, uv_y + square_half_side, color, color);
}