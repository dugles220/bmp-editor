#include "../include/stack.h"

FillStack *create_stack()
{
    FillStack *s = malloc(sizeof(FillStack));
    if (!s) {
        perror("create_stack: stack memory allocate failed");
        return NULL;
    }

    s->data = malloc(INITIAL_CAP * sizeof(Point));
    if (!s->data) {
        perror("create_stack: stack data memory allocate failed");
        free(s);
        return NULL;
    }

    s->count = 0;
    s->capacity = INITIAL_CAP;

    return s;
}

void extend(FillStack *s)
{
    s->capacity *= 2;
    s->data = realloc(s->data, s->capacity * sizeof(Point));
}

void push(FillStack *s, int x, int y)
{
    if (s == NULL) {
        fprintf(stderr, "push: s is a NULL pointer\n");
        return;
    }

    if (s->count >= s->capacity) {
        extend(s);
    }

    s->data[s->count].x = x;
    s->data[s->count].y = y;
    s->count++;
}

Point pop(FillStack *s)
{
    return s->data[--s->count];
}

int is_empty(FillStack *s)
{
    return s->count == 0;
}

void free_stack(FillStack *s)
{
    if (s != NULL) {
        free(s->data);
        free(s);
    }
}