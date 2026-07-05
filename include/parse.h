/**
 * @file parse.h
 * @brief Модуль парсера аргументов комнадной строки (CLI)
 */
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

/**
 * @brief Структура, отражающая конфигурацию работы программы
 * 
 * Конфигурация собирается при обработке аргументов командной строки,
 * которые вводит пользователь.
 * 
 */
typedef struct {
    char *input_file; ///< Путь к обрабатываемому изображению BMP
    char *output_file; ///< Название файла, в который будет записан результат
    enum to_display info; ///< Флаг вывода информации об изображении
    enum to_display help; ///< Флаг вывода справки работы с программой
    enum actions action; ///< Действие, производимое над изображением
    int lu_x, lu_y; ///< Координаты левого верхнего угла области
    int rd_x, rd_y; ///< Координаты правого нижнего угла области
    int uv_x, uv_y; ///< Координаты верхней вершины ромба
    int side_size; ///< Длина стороны
    int thickness; ///< Толщина линий
    RGB color; ///< Цвет рисования
    enum to_fill fill; ///< Флаг использования заливки
    RGB fill_color; ///< Цвет заливки
    enum color component_name; ///< Имя компоненты, которую необходимо изменить
    int component_value; ///< Значение компоненты, которое необходимо выставить
    int angle; ///< Угол поворота области изображения (90, 180 или 270)
    unsigned char axis; ///< Ось отражения (x или y)

} Config;

void print_manual();
void execute_command(Config *config);
int check_config(Config *config);
void clamp_coordinates(Config *config, const BMP *bmp);
void handle_comp_name(Config *config, const char *component_name);
void handle_io_files(Config *config, int optind, int argc, char **argv);
void make_action(Config *config, BMP *bmp, BMP **copy);

#endif