#include "../include/parse.h"

#include <getopt.h>
#include <stdlib.h>

enum {
    OPT_SQUARED_LINES = 256,
    OPT_RGBFILTER,
    OPT_ROTATE,
    OPT_MIRROR,
    OPT_DIAG_MIRROR,
    OPT_SQUARE_RHOMBUS,
    OPT_LEFT_UP,
    OPT_RIGHT_DOWN,
    OPT_ANGLE,
    OPT_SIDE_SIZE,
    OPT_THICKNESS,
    OPT_COLOR,
    OPT_FILL,
    OPT_FILL_COLOR,
    OPT_COMP_NAME,
    OPT_COMP_VALUE,
    OPT_AXIS,
    OPT_UPPER_VERTEX
};

int main(int argc, char **argv)
{
    int opt;
    int option;
    Config config = {0};
    config.input_file = NULL;
    config.output_file = "out.bmp";

    struct option long_options[] = {

        {"help", no_argument, 0, 'h'},
        {"output", required_argument, 0, 'o'},
        {"info", no_argument, 0, 'i'},
        {"input", required_argument, 0, 'I'},

        {"squared_lines", no_argument, 0, OPT_SQUARED_LINES},
        {"rgbfilter", no_argument, 0, OPT_RGBFILTER},
        {"rotate", no_argument, 0, OPT_ROTATE},
        {"mirror", no_argument, 0, OPT_MIRROR},
        {"diag_mirror", no_argument, 0, OPT_DIAG_MIRROR},
        {"square_rhombus", no_argument, 0, OPT_SQUARE_RHOMBUS},

        {"left_up", required_argument, 0, OPT_LEFT_UP},
        {"right_down", required_argument, 0, OPT_RIGHT_DOWN},
        {"angle", required_argument, 0, OPT_ANGLE},
        {"side_size", required_argument, 0, OPT_SIDE_SIZE},
        {"thickness", required_argument, 0, OPT_THICKNESS},
        {"color", required_argument, 0, OPT_COLOR},
        {"fill", no_argument, 0, OPT_FILL},
        {"fill_color", required_argument, 0, OPT_FILL_COLOR},
        {"component_name", required_argument, 0, OPT_COMP_NAME},
        {"component_value", required_argument, 0, OPT_COMP_VALUE},
        {"axis", required_argument, 0, OPT_AXIS},
        {"upper_vertex", required_argument, 0, OPT_UPPER_VERTEX},

        {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "ho:iI:", long_options, &option)) != -1) {

        switch (opt) {

            case 'h':
                config.help = display;
                break;
            case 'o':
                config.output_file = optarg;
                break;
            case 'i':
                config.info = display;
                break;
            case 'I':
                config.input_file = optarg;
                break;

            case OPT_SQUARED_LINES:
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = squared_lines;
                break;
            case OPT_RGBFILTER:
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = rgbfilter;
                break;
            case OPT_ROTATE:
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = rotate;
                break;
            case OPT_MIRROR:
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = mirror;
                break;
            case OPT_DIAG_MIRROR:
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = diag_mirror;
                break;
            case OPT_SQUARE_RHOMBUS:
                if(config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = square_rhombus;
                break;
            case OPT_UPPER_VERTEX:
                if (sscanf(optarg, "%d.%d", &config.uv_x, &config.uv_y) != 2) {
                    fprintf(stderr,
                            "main: invalid argument for --upper_vertex: %s; x.y "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_LEFT_UP:
                if (sscanf(optarg, "%d.%d", &config.lu_x, &config.lu_y) != 2) {
                    fprintf(stderr,
                            "main: invalid argument for --left_up: %s; x.y "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_RIGHT_DOWN:
                if (sscanf(optarg, "%d.%d", &config.rd_x, &config.rd_y) != 2) {
                    fprintf(stderr,
                            "main: invalid argument for --right_down: %s; x.y "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_ANGLE:
                if (sscanf(optarg, "%d", &config.angle) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --angle: %s; integer "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_SIDE_SIZE:
                if (sscanf(optarg, "%d", &config.side_size) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --side_size: %s; "
                            "integer required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_THICKNESS:
                if (sscanf(optarg, "%d", &config.thickness) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --thickness: %s; "
                            "integer required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_COLOR:
                if (sscanf(optarg, "%hhu.%hhu.%hhu", &config.color.r, &config.color.g, &config.color.b) !=
                    3) {
                    fprintf(stderr,
                            "main: invalid argument for --color: %s; "
                            "rrr.ggg.bbb required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_FILL:
                config.fill = fill;
                break;
            case OPT_FILL_COLOR:
                if (sscanf(optarg, "%hhu.%hhu.%hhu", &config.fill_color.r, &config.fill_color.g,
                           &config.fill_color.b) != 3) {
                    fprintf(stderr,
                            "main: invalid argument for --fill_color: %s; "
                            "rrr.ggg.bbb required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_COMP_NAME:
                handle_comp_name(&config, optarg);
                break;
            case OPT_COMP_VALUE:
                if (sscanf(optarg, "%d", &config.component_value) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --component_value: %s; "
                            "integer required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case OPT_AXIS:
                if (sscanf(optarg, "%c", &config.axis) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --axis: %s; "
                            "x or y required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case '?':
                exit(ERR_INVALID_ARG);
        }
    }

    if (config.info && optind < argc - 1) {
        fprintf(stderr, "main: Argument '%s' ignored.\n", argv[optind]);
        exit(ERR_INVALID_ARG);
    }

    handle_io_files(&config, optind, argc, argv);

    execute_command(&config);

    return 0;
}