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
void extend(FillStack *);
void push(FillStack *, int, int);
Point pop(FillStack *);
int is_empty(FillStack *);
void free_stack(FillStack *);

#endif