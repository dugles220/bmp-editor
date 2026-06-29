#include "../include/parse.h"
#include "../include/bmp_io.h"
#include "../include/bmp_ops.h"

void print_manual()
{
    printf("Usage: ./bmp-editor [ACTION] [OPTIONS] input_file\n\n");

    printf("Actions:\n");
    printf("  -S, --squared_lines    draw a square with diagonals.\n");
    printf("  -F, --rgbfilter        apply RGB component filter.\n");
    printf("  -R, --rotate           rotate a specific area of the image.\n");
    printf("  -i, --info             display BMP file information.\n");
    printf("  -h, --help             display this help message.\n\n");

    printf("Square (-S):\n");
    printf("  -u, --left_up x.y      top-left corner coordinates.\n");
    printf("  -s, --side_size N      size of the square side (N > 0).\n");
    printf("  -t, --thickness N      line thickness (N > 0).\n");
    printf("  -c, --color r.g.b      line color (0-255).\n");
    printf("  -f, --fill             enable filling.\n");
    printf("  -C, --fill_color r.g.b filling color (0-255).\n\n");

    printf("RGB Filter (-F):\n");
    printf("  -n, --component_name C target component (red, green, blue).\n");
    printf("  -v, --component_value N new value for component (0-255).\n\n");

    printf("Rotate Options (-R):\n");
    printf("  -u, --left_up x.y      top-left corner of the area.\n");
    printf("  -d, --right_down x.y   bottom-right corner of the area.\n");
    printf("  -a, --angle N          angle (90, 180, 270).\n\n");

    printf("General Options:\n");
    printf("  -o, --output FILE      output file name (default: out.bmp).\n");
}

int check_config(Config *config)
{
    if (config->action == squared_lines) {
        if (config->side_size <= 0) {
            fprintf(stderr, "check_config: invalid side_size value: %d.\n", config->side_size);
            return ERR_INVALID_ARG;
        }
        if (config->thickness <= 0) {
            fprintf(stderr, "check_config: invalid thickness value: %d.\n", config->thickness);
            return ERR_INVALID_ARG;
        }

        return squared_lines;
    }

    if (config->action == rgbfilter) {
        return rgbfilter;
    }

    if (config->action == rotate) {
        int angle = config->angle;

        if (angle != 90 && angle != 180 && angle != 270) {
            fprintf(stderr,
                    "check_config: invalid angle value: %d; 90, 180 or 270 "
                    "expected.\n",
                    angle);
            return ERR_INVALID_ARG;
        }

        if (!(config->lu_x < config->rd_x) || !(config->lu_y < config->rd_y)) {
            fprintf(stderr, "check_config: zero width or height part.\n");
            return ERR_INVALID_ARG;
        }

        if (config->lu_x > config->rd_x || config->lu_y > config->rd_y) {
            fprintf(stderr, "check_config: left-up coords must be smaller than "
                            "right-down coords.\n");
            return ERR_INVALID_ARG;
        }

        return rotate;
    }

    if (config->action == mirror) {
        if (config->axis != 'x' && config->axis != 'y') {
            fprintf(stderr, "check_config: invalid axis argument: %c; x or y required.\n", config->axis);
            return ERR_INVALID_ARG;
        }

        size_t dy = config->rd_y - config->lu_y;
        size_t dx = config->rd_x - config->lu_x;

        if (dy != dx) {
            fprintf(stderr, "check_config: square area required.\n");
            size_t side_size;
            if (dy < dx)
                side_size = dy;
            else
                side_size = dx;
            config->rd_x = config->lu_x + side_size;
            config->rd_y = config->lu_y + side_size;
        }

        return mirror;
    }

    if (config->action == diag_mirror) {
        size_t dy = config->rd_y - config->lu_y;
        size_t dx = config->rd_x - config->lu_x;

        if (dy != dx) {
            fprintf(stderr, "check_config: square area required.\n");

            size_t side_size;

            if (dy > dx)
                side_size = dy;
            else
                side_size = dx;

            config->rd_x = config->lu_x + side_size;
            config->rd_y = config->lu_y + side_size;
        }

        return diag_mirror;
    }

    return no_action;
}

void clamp_coordinates(Config *config, const BMP *bmp)
{
    size_t W = bmp->pixels->W;
    size_t H = bmp->pixels->H;

    if (config->lu_x < 0) {
        config->lu_x = 0;
        fprintf(stderr, "check_config: lu_x coordinate is out of range, a "
                        "clamp was used.\n");
    }
    if (config->lu_y < 0) {
        config->lu_y = 0;
        fprintf(stderr, "check_config: lu_y coordinate is out of range, a "
                        "clamp was used.\n");
    }
    if (config->rd_x >= W) {
        config->rd_x = W;
        fprintf(stderr, "check_config: rd_x coordinate is out of range, a "
                        "clamp was used.\n");
    }
    if (config->rd_y >= H) {
        config->rd_y = H;
        fprintf(stderr, "check_config: rd_y coordinate is out of range, a "
                        "clamp was used.\n");
    }
}

void execute_command(Config *config)
{
    if (config->help) {
        print_manual();
        if (config->action == 0 && config->info == 0)
            return;
    }

    BMP *bmp = bmp_read(config->input_file);
    if (bmp == NULL)
        exit(ERR_INVALID_FILE);

    clamp_coordinates(config, bmp);
    config->action = check_config(config);

    if (config->action == ERR_INVALID_ARG) {
        bmp_free(bmp);
        exit(ERR_INVALID_ARG);
    }

    if (config->info) {
        bmp_print_info(bmp);

        if (config->action == no_action) {
            bmp_free(bmp);
            return;
        }
    }

    BMP *copy = bmp;

    make_action(config, bmp, &copy);

    if (copy != NULL) {
        bmp_write(copy, config->output_file);
    } else if (config->action != 0) {
        fprintf(stderr, "execute_command: action %d failed.\n", config->action);
        bmp_free(bmp);
        exit(ERR_ACTION_FAILED);
    }

    if (copy != bmp)
        bmp_free(copy);
    bmp_free(bmp);
}

void handle_io_files(Config *config, int optind, int argc, char **argv)
{
    if (optind < argc) {
        config->input_file = argv[argc - 1];
    } else if (config->help == 0 && config->input_file == NULL) {
        fprintf(stderr, "main: input file name required.\n");
        exit(ERR_INVALID_FILE);
    }

    if (config->help == 0 && strcmp(config->input_file, config->output_file) == 0) {
        fprintf(stderr, "main: input and output files are the same.\n");
        exit(ERR_SAME_FILES);
    }
}

void handle_comp_name(Config *config, const char *line)
{
    if (strcmp(line, RED) == 0)
        config->component_name = 'r';
    else if (strcmp(line, GREEN) == 0)
        config->component_name = 'g';
    else if (strcmp(line, BLUE) == 0)
        config->component_name = 'b';
    else {
        fprintf(stderr,
                "main: invalid argument for --component_name: %s; "
                "'red', 'green' or 'blue' required.\n",
                line);
        exit(ERR_INVALID_ARG);
    }
}

void make_action(Config *config, BMP *bmp, BMP **copy)
{
    switch (config->action) {
        case squared_lines:
            draw_square(bmp->pixels, config->lu_x, config->lu_y, config->side_size, config->thickness,
                        config->fill, 1, &config->color, &config->fill_color);

            break;
        case rgbfilter:
            *copy = bmp_rgb_filter(bmp, config->component_name, config->component_value);
            break;
        case rotate:

            if (config->angle == 90)
                config->angle = 1;
            else if (config->angle == 180)
                config->angle = 2;
            else if (config->angle == 270)
                config->angle = 3;

            *copy = bmp_rotate(bmp, config->lu_x, config->lu_y, config->rd_x, config->rd_y, config->angle);
            break;
        case mirror:
            size_t side_size = config->rd_y - config->lu_y;
            bmp_mirror(bmp->pixels, config->lu_x, config->lu_y, side_size, config->axis);
            break;
        case diag_mirror:
            side_size = config->rd_y - config->lu_y;
            bmp_diag_mirror(bmp->pixels, config->lu_x, config->lu_y, side_size);
            break;
        default:
            fprintf(stderr, "execute_command: no action for %d.\n", config->action);
    }
}