#ifndef FILL_STACK
#define FILL_STACK

#define INITIAL_CAP 128

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;

} Point;

typedef struct {
    Point *data;
    size_t count;
    size_t capacity;

} FillStack;

FillStack *create_stack();
void extend(FillStack *stack);
void push(FillStack *stack, int x, int y);
Point pop(FillStack *stack);
int is_empty(FillStack *stack);
void free_stack(FillStack *stack);

#endif