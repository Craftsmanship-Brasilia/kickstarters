#include "int_stack.h"

#include <stdlib.h>

enum { INITIAL_CAPACITY = 4 };

struct IntStack {
    int *items;      /* heap array, grown with realloc */
    size_t size;     /* items in use */
    size_t capacity; /* items allocated */
};

IntStack *int_stack_create(void)
{
    IntStack *stack = malloc(sizeof *stack); /* sizeof *stack: stays right if the type changes */
    if (stack == NULL) {
        return NULL;
    }
    *stack = (IntStack){0}; /* zero every field: NULL items, size 0, capacity 0 */
    return stack;
}

void int_stack_destroy(IntStack *stack)
{
    if (stack == NULL) {
        return;
    }
    free(stack->items); /* free what the struct owns first... */
    free(stack);        /* ...then the struct itself */
}

static bool grow(IntStack *stack)
{
    size_t capacity = stack->capacity == 0 ? INITIAL_CAPACITY : stack->capacity * 2;
    /* Never `stack->items = realloc(stack->items, ...)`: on failure it returns NULL and the old block leaks */
    int *items = realloc(stack->items, capacity * sizeof *items);
    if (items == NULL) {
        return false;
    }
    stack->items = items;
    stack->capacity = capacity;
    return true;
}

bool int_stack_push(IntStack *stack, int value)
{
    if (stack->size == stack->capacity && !grow(stack)) {
        return false;
    }
    stack->items[stack->size++] = value;
    return true;
}

bool int_stack_pop(IntStack *stack, int *out)
{
    if (!int_stack_peek(stack, out)) {
        return false;
    }
    stack->size--;
    return true;
}

bool int_stack_peek(const IntStack *stack, int *out)
{
    if (stack->size == 0) {
        return false;
    }
    *out = stack->items[stack->size - 1];
    return true;
}

size_t int_stack_size(const IntStack *stack)
{
    return stack->size;
}
