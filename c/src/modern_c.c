#include "modern_c.h"

#include <stdlib.h>
#include <string.h>

Point point_add(Point a, Point b)
{
    /* Compound literal (C99): an unnamed struct value built in place.
       C89: Point sum; sum.x = a.x + b.x; sum.y = a.y + b.y; return sum; */
    return (Point){.x = a.x + b.x, .y = a.y + b.y};
}

int shape_area_approx(Shape shape)
{
    switch (shape.kind) {
    case SHAPE_CIRCLE:
        return 3 * square(shape.radius); /* anonymous union: no .u.radius */
    case SHAPE_RECT:
        return shape.w * shape.h;
    }
    return 0; /* unreachable for valid kinds; -Wall warns if a case is missing above */
}

const char *describe_int(int value)
{
    (void)value;
    return "int";
}

const char *describe_double(double value)
{
    (void)value;
    return "double";
}

const char *describe_string(const char *value)
{
    (void)value;
    return "string";
}

IntList *int_list_of(size_t length, const int *values)
{
    /* One allocation for header + items: sizeof(IntList) doesn't include the flexible member */
    IntList *list = malloc(sizeof *list + length * sizeof list->items[0]);
    if (list == NULL) {
        return NULL;
    }
    list->length = length;
    memcpy(list->items, values, length * sizeof list->items[0]);
    return list;
}

int int_list_sum(const IntList *list)
{
    int sum = 0;
    for (size_t i = 0; i < list->length; i++) { /* C99: declare the loop variable in the for */
        sum += list->items[i];
    }
    return sum;
}

void int_list_free(IntList *list)
{
    free(list); /* free(NULL) is a no-op, no need to check */
}

void add_arrays(size_t n, int *restrict out, const int *restrict a, const int *restrict b)
{
    for (size_t i = 0; i < n; i++) {
        out[i] = a[i] + b[i];
    }
}
