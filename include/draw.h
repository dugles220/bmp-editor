/**
 * @file draw.h
 * @brief Модуль функций рисования на полотне пикселей
 */

#ifndef DRAW_H
#define DRAW_H

#include "bmp_structs.h"
#include "pixels.h"
#include "stack.h"

/**
 * @brief Проверяет, лежит ли точка в пределах изображения 
 *
 * @param pixels Полотно пикселей 
 * @param x Координата X точки
 * @param y Координата Y точки
 * @return 1 Если точка лежит в пределах изображения
 * @return 0 Если точка лежит вне пределов или одна из координат меньше нуля 
 */
int check_coordinates(const Pixels *pixels, int x, int y);
/**
 * @brief Рисует линию по алгоритму Брезенхема
 * 
 * Использует круглую кисть.
 * Если часть линии находится вне изображения, отрисуется только доступная часть.
 * 
 * @param pixels Полотно пикселей, на котором необходимо нарисовать линию
 * @param start_x Координита X начала линии
 * @param start_y Координата Y начала линии
 * @param end_x Координата X конца линии
 * @param end_y Координата Y конца линии
 * @param thickness Толщина линии
 * @param color Цвет линии
 */
void draw_line(Pixels *pixels, int start_x, int start_y, int end_x, int end_y, int thickness, const RGB *color);
/**
 * @brief Заливает область, ограниченную заданным цветом, используя алгоритм Scanline Fill
 *
 * Закрашивает заданным цветом область, которая ограничена замкнутой фигурой какого-либо цвета,
 * который указывается в аргументах. 
 *  
 * @param pixels Полотно пикселей
 * @param x Координата X точки из области, которую нужно залить
 * @param y Координата Y точки из области, которую нужно залить
 * @param fill_color Цвет заливки
 * @param bound_color Цвет, ограничивающий заливку
 */
void scanline_fill(Pixels *pixels , size_t x, size_t y, const RGB *fill_color, const RGB *bound_color);
/**
 * @brief Проверяет, одинаковы ли 2 цвета
 * 
 * @param color1 Первый цвет
 * @param color2 Второй цвет
 * @return 0 Если цвета не одинаковы 
 * @return 1 Если цвета одинаковы 
 */
int are_colors_equal(const RGB *color1, const RGB *color2);
/**
 * @brief Вспомогательная функция для scanline_fill
 */
void check_line(FillStack *stack, Pixels *pixels, size_t x_start, size_t x_end, int y, const RGB *bound_color, const RGB *fill_color);
/**
 * @brief Определяет знак числа 
 */
int sign(int num);

#endif