#include "../include/draw.h"

int sign(int x)
{
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;
    return 0;
}

void draw_line(Pixels *pixels, int xstart, int ystart, int xend, int yend, int thickness, const RGB *color)
{
    int x, y, dx, dy, x_to, y_to, pdx, pdy, es, el, err;

    dx = xend - xstart;
    dy = yend - ystart;

    x_to = sign(dx);
    y_to = sign(dy);

    dx = abs(dx);
    dy = abs(dy);

    if (dx > dy) {
        pdx = x_to;
        pdy = 0;

        es = dy;
        el = dx;
    } else {
        pdx = 0;
        pdy = y_to;
        es = dx;
        el = dy;
    }

    x = xstart;
    y = ystart;
    err = el / 2;

    draw_pixel(pixels, x, y, thickness, color);

    for (int t = 0; t < el; t++) {
        err -= es;

        if (err < 0) {
            err += el;
            x += x_to;
            y += y_to;
        } else {
            x += pdx;
            y += pdy;
        }
        draw_pixel(pixels, x, y, thickness, color);
    }
}

void draw_pixel(Pixels *pixels, int x, int y, int thickness, const RGB *color)
{
    int y_cord, x_cord;
    RGB *pixel;
    RGB **pix_arr = pixels->pix_arr;

    int radius = thickness / 2;

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            if (i * i + j * j <= radius * radius) {
                y_cord = convert_index(pixels, y + j);
                x_cord = x + i;
                if (check_coordinates(pixels, x_cord, y_cord)) {
                    pixel = &pix_arr[y_cord][x_cord];
                    repaint_pixel(pixel, color);
                }
            }
        }
    }
}

int check_coordinates(const Pixels *pixels, int x, int y)
{
    size_t height = pixels->H;
    size_t width = pixels->W;

    if (x >= (int)width || x < 0 || y >= (int)height || y < 0)
        return 0;

    return 1;
}

int are_colors_equal(const RGB *c1, const RGB *c2)
{
    if (c1->b != c2->b || c1->g != c2->g || c1->r != c2->r)
        return 0;

    return 1;
}

void scanline_fill(Pixels *pixels, size_t x, size_t y, const RGB *fill_color, const RGB *bound_color)
{
    RGB **pix_arr = pixels->pix_arr;

    size_t W = pixels->W;
    size_t H = pixels->H;

    if (x > W - 1)
        x = W - 1;
    if (x < 0)
        x = 0;
    if (y > H - 1)
        y = H - 1;
    if (y < 0)
        y = 0;

    FillStack *s = create_stack();
    push(s, (int)x, (int)y);

    while (!is_empty(s)) {
        Point p = pop(s);

        int cur_x = p.x;
        int cur_y = p.y;

        int converted_y = convert_index(pixels, cur_y);

        int left_x = cur_x;
        while (left_x >= 0 && !are_colors_equal(&pix_arr[converted_y][left_x], bound_color) &&
               !are_colors_equal(&pix_arr[converted_y][left_x], fill_color)) {
            repaint_pixel(&pix_arr[converted_y][left_x], fill_color);
            left_x--;
        }

        int right_x = cur_x + 1;
        while (right_x < W && !are_colors_equal(&pix_arr[converted_y][right_x], bound_color) &&
               !are_colors_equal(&pix_arr[converted_y][right_x], fill_color)) {
            repaint_pixel(&pix_arr[converted_y][right_x], fill_color);
            right_x++;
        }

        check_line(s, pixels, left_x + 1, right_x - 1, cur_y + 1, bound_color, fill_color);
        check_line(s, pixels, left_x + 1, right_x - 1, cur_y - 1, bound_color, fill_color);
    }

    free_stack(s);
}

void check_line(FillStack *s, Pixels *pixels, size_t x_start, size_t x_end, int y, const RGB *bound_color,
                const RGB *fill_color)
{
    if (y < 0 || y >= pixels->H)
        return;

    RGB **pix_arr = pixels->pix_arr;
    int is_added = 0;
    int converted_y = convert_index(pixels, y);

    for (size_t x = x_start; x < x_end; x++) {
        RGB *curr_pixel = &pix_arr[converted_y][x];

        if (!are_colors_equal(curr_pixel, bound_color) && !are_colors_equal(curr_pixel, fill_color)) {
            if (!is_added) {
                push(s, x, y);
                is_added = 1;
            }
        } else {
            is_added = 0;
        }
    }
}