#ifndef INT_STACK_H
#define INT_STACK_H

#include <stdbool.h>
#include <stddef.h>

/*
 * Opaque type: callers only see a pointer, the struct's fields live in int_stack.c.
 * It's C's version of private fields + a constructor/destructor (like a class with #private
 * fields in TypeScript), and changing the fields never forces callers to recompile.
 */
typedef struct IntStack IntStack;

/* Ownership: whoever calls _create owns the stack and must call _destroy. Returns NULL if out of memory. */
IntStack *int_stack_create(void);
void int_stack_destroy(IntStack *stack);

/* Returns false when out of memory; the stack is unchanged */
bool int_stack_push(IntStack *stack, int value);

/*
 * No exceptions in C: success is the return value, the result goes through an "out parameter".
 * Returns false (and leaves *out untouched) when the stack is empty.
 */
bool int_stack_pop(IntStack *stack, int *out);

/* `const IntStack *`: promises not to modify the stack, so it can be called on read-only data */
size_t int_stack_size(const IntStack *stack);
bool int_stack_peek(const IntStack *stack, int *out);

#endif
