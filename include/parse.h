#ifndef PARSE_H
#define PARSE_H

#include "bmp_structs.h"
#include "enums.h"

#include <stdio.h>
#include <string.h>

#define ERR_INVALID_ARG 40
#define ERR_INVALID_FILE 41
#define ERR_MULTIPLE_ACTIONS 42
#define ERR_COORDINATES 43
#define ERR_SAME_FILES 44
#define ERR_ACTION_FAILED 45

typedef struct {
    char *input_file;
    char *output_file;
    enum to_display info;
    enum to_display help;
    enum actions action;

    int lu_x, lu_y;
    int rd_x, rd_y;
    int uv_x, uv_y;
    int side_size;
    int thickness;
    RGB color;
    enum to_fill fill;
    RGB fill_color;
    int upsize;

    enum color component_name;
    int component_value;

    int angle;

    unsigned char axis;

} Config;

void print_manual();
void execute_command(Config *);

int check_config(Config *);
void clamp_coordinates(Config *, const BMP *);
void handle_comp_name(Config *, const char *);
void handle_io_files(Config *config, int optind, int argc, char **argv);
void make_action(Config *config, BMP *bmp, BMP **copy);

#endif