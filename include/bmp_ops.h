/**
 * @file bmp_ops.h
 * @brief Модуль для изменения изображений, записанных в структуру BMP
 */

#ifndef BMP_OPS_H
#define BMP_OPS_H

#include "bmp_io.h"
#include "draw.h"
#include "pixels.h"

#include <string.h>

/**
 * @brief Копирует существующую структуру BMP
 * 
 * @param bmp_to_copy Структура BMP, которую необходимо скопировать
 * @return BMP* Скопированная структура BMP при успешном копировании
 * @return NULL Если при копировании произошла ошибка
 */
BMP *bmp_copy(const BMP *bmp_to_copy);
/**
 * @brief Изменяет одну из RGB компонент на заданное значение у всего изображения
 * 
 * @param bmp Изображение, на которое необходимо наложить фильтр 
 * @param rgb_comp_value Значение компоненты, которое необходимо задать
 * @param rgb_comp_name Символ, обозначающий компоненту
 * @return BMP* Копия изображения, на которую наложен фильтр
 * @return NULL Если при наложении произошла ошибка
 */
BMP *bmp_rgb_filter(const BMP *bmp, char rgb_comp_value, unsigned char rgb_comp_name);
/**
 * @brief Поворачивает заданную область изображения на 90, 180 или 270 градусов.
 * 
 * Поворот совершается относительно центра области против часовой стрелки.
 * Если область не квадратная и после поворота её часть выходит за рамки
 * изображения, то эта часть обрезается.
 * 
 * @param bmp Изображение, над которым производится поворот
 * @param left_up_x Координата X верхнего левого угла области
 * @param left_up_y Координата Y верхнего левого угла области
 * @param right_down_x Координата X нижнего права угла области
 * @param right_down_y Координата Y нижнего права угла области
 * @param rotates Количество поворотов на 90 градусов
 * @return BMP* Копия изображения с повёрнутой областью
 * @return NULL Если при выполнении возникла ошибка
 */
BMP *bmp_rotate(const BMP *bmp, size_t left_up_x, size_t left_up_y, size_t right_down_x, size_t right_down_y, size_t rotates);
/**
 * @brief Рисует квадрат с заданной стороной и толщиной линий.
 * 
 * Опционально можно выбрать рисование диагоналей квадрата и заливку квадрата заданным цветом.
 * Если квадрат выходит за рамки изображения, то рисуется только доступная часть.
 * Изменяет подаваемое полотно, не создавая копию.
 * 
 * @param pixels Полотно пикселей, на котором необходимо нарисовать квадрат
 * @param left_up_corner_x Координата X верхнего левого угла квадрата
 * @param left_up_corner_y Координата Y верхнего левого угла квадрата
 * @param side_size Длина стороны квадрата
 * @param thickness Толщина сторон квадрата (и диагоналей, если они отрисовываются)
 * @param to_fill Флаг заливки квадрата (0 - нет, 1 - да)
 * @param with_diag Флаг отрисовки диагоналей (0 - нет, 1 - да)
 * @param sides_color Цвет сторон квадрата (и диагоналей, если они отрисовываются)
 * @param fill_color Цвет заливки квадрата (если to_fill задано как 1, иначе подаётся NULL)
 */
void draw_square(Pixels *pixels, size_t left_up_corner_x, size_t left_up_corner_y, size_t side_size, size_t thickness, int to_fill, int with_diag, const RGB *sides_color, const RGB *fill_color);
/**
 * @brief Отражает квадратную область изображения по оси X или Y
 * 
 * Изменяет подаваемое полотно, не создавая копию.
 *
 * @param pixels Полотно пикселей, область которого необходимо отразить
 * @param left_up_x Координата X верхнего левого угла области
 * @param left_up_y Координата Y верхнего левого угла области
 * @param side_size Длина стороны области
 * @param axis Ось, по которой будет отражена область (x или y)
 */
void bmp_mirror(Pixels *pixels, size_t left_up_x, size_t left_up_y, size_t side_size, unsigned char axis);
/**
 * @brief Отражает квадратную область изображения по диагонали
 * 
 * Изменяет подаваемое полотно, не создавая копию.
 * 
 * @param pixels Полотно пикселей, область которого необходимо отразить
 * @param left_up_x Координата X верхнего левого угла области
 * @param left_up_y Координата Y верхнего левого угла области
 * @param side_size Длина стороны области
 */
void bmp_diag_mirror(Pixels *pixels, size_t left_up_x, size_t left_up_y, size_t side_size);
/**
 * @brief Рисует равносторонний залитый ромб с заданной длиной стороны
 * 
 * Изменяет подаваемое полотно, не создавая копию.
 * 
 * @param pixels Полотно пикселей, на котором необходимо нарисовать ромб
 * @param upper_vertex_x Координата X верхней вершины ромба
 * @param upper_vertex_y Координата Y верхней вершины ромба
 * @param side_size Длина стороны ромба
 * @param color Цвет ромба
 */
void draw_square_rhombus(Pixels *pixels, int upper_vertex_x, int upper_vertex_y, size_t side_size, const RGB *color);
/**
 * @brief Проверяет корректность координат, подаваемых в функцию bmp_rotate
 * 
 * @param pixels Полотно пикселей, область которого необходимо повернуть
 * @param left_up_x Координата X верхнего левого угла области
 * @param left_up_y Координата Y верхнего левого угла области
 * @param right_down_x Координата X нижнего права угла области
 * @param right_down_y Координата Y нижнего права угла области
 * @return 1 Если координаты корректны
 * @return 0 Если координаты левого верхнего угла больше, чем координаты правого нижнего
 * или если размер области больше размеров изображения
 */
int check_rotate_coordinates(const Pixels *pixels, size_t left_up_x, size_t left_up_y, size_t right_down_x, size_t right_down_y);

#endif