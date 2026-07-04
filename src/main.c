#include "../include/parse.h"

#include <getopt.h>
#include <stdlib.h>

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

        {"squared_lines", no_argument, 0, 'S'},
        {"rgbfilter", no_argument, 0, 'F'},
        {"rotate", no_argument, 0, 'R'},
        {"mirror", no_argument, 0, 'M'},
        {"diag_mirror", no_argument, 0, 'D'},
        {"square_rhombus", no_argument, 0, 'r'},

        {"left_up", required_argument, 0, 'u'},
        {"right_down", required_argument, 0, 'd'},
        {"angle", required_argument, 0, 'a'},
        {"side_size", required_argument, 0, 's'},
        {"thickness", required_argument, 0, 't'},
        {"color", required_argument, 0, 'c'},
        {"fill", no_argument, 0, 'f'},
        {"fill_color", required_argument, 0, 'C'},
        {"component_name", required_argument, 0, 'n'},
        {"component_value", required_argument, 0, 'v'},
        {"axis", required_argument, 0, 'A'},
        {"upper_vertex", required_argument, 0, 'U'},

        {0, 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "ho:iI:SFRMDru:d:a:s:t:c:fU:C:n:v:", long_options, &option)) != -1) {
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
            case 'S':
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = squared_lines;
                break;
            case 'F':
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = rgbfilter;
                break;
            case 'R':
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = rotate;
                break;
            case 'M':
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = mirror;
                break;
            case 'D':
                if (config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = diag_mirror;
                break;
            case 'r':
                if(config.action != no_action) {
                    fprintf(stderr, "main: mulitple action selected\n");
                    exit(ERR_MULTIPLE_ACTIONS);
                }
                config.action = square_rhombus;
                break;
            case 'U':
                if (sscanf(optarg, "%d.%d", &config.uv_x, &config.uv_y) != 2) {
                    fprintf(stderr,
                            "main: invalid argument for --upper_vertex: %s; x.y "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 'u':
                if (sscanf(optarg, "%d.%d", &config.lu_x, &config.lu_y) != 2) {
                    fprintf(stderr,
                            "main: invalid argument for --left_up: %s; x.y "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 'd':
                if (sscanf(optarg, "%d.%d", &config.rd_x, &config.rd_y) != 2) {
                    fprintf(stderr,
                            "main: invalid argument for --right_down: %s; x.y "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 'a':
                if (sscanf(optarg, "%d", &config.angle) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --angle: %s; integer "
                            "required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 's':
                if (sscanf(optarg, "%d", &config.side_size) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --side_size: %s; "
                            "integer required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 't':
                if (sscanf(optarg, "%d", &config.thickness) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --thickness: %s; "
                            "integer required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 'c':
                if (sscanf(optarg, "%hhu.%hhu.%hhu", &config.color.r, &config.color.g, &config.color.b) !=
                    3) {
                    fprintf(stderr,
                            "main: invalid argument for --color: %s; "
                            "rrr.ggg.bbb required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 'f':
                config.fill = fill;
                break;
            case 'C':
                if (sscanf(optarg, "%hhu.%hhu.%hhu", &config.fill_color.r, &config.fill_color.g,
                           &config.fill_color.b) != 3) {
                    fprintf(stderr,
                            "main: invalid argument for --fill_color: %s; "
                            "rrr.ggg.bbb required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 'n':
                handle_comp_name(&config, optarg);
                break;
            case 'v':
                if (sscanf(optarg, "%d", &config.component_value) != 1) {
                    fprintf(stderr,
                            "main: invalid argument for --component_value: %s; "
                            "integer required.\n",
                            optarg);
                    exit(ERR_INVALID_ARG);
                }
                break;
            case 'A':
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