#ifndef MODERN_C_H
#define MODERN_C_H

/*
 * C99/C11/C17 features you will use in katas, each next to the C89 you'll still meet in older code.
 * Exercised by tests/test_modern_c.c. C23 previews are in comments (gcc 12 only half-supports C23).
 */

#include <stdbool.h> /* C99: bool/true/false. C89: #define BOOL int / TRUE 1. C23: built-in keywords */
#include <stddef.h>
#include <stdint.h> /* C99: exact-width ints. C89: guess with short/long per platform */

/* --- structs: designated initializers, compound literals, anonymous members --------- */

typedef struct {
    int x;
    int y;
} Point;

/* C11 anonymous union inside a struct: shape.circle.radius is written shape.radius */
typedef enum { SHAPE_CIRCLE, SHAPE_RECT } ShapeKind;

typedef struct {
    ShapeKind kind;
    Point origin;
    union {
        int radius;             /* SHAPE_CIRCLE */
        struct { int w, h; };   /* SHAPE_RECT (anonymous struct, also C11) */
    };
} Shape;

/* A struct is a value: passed and returned by copy, like a C# struct or record struct */
Point point_add(Point a, Point b);
int shape_area_approx(Shape shape);

/* --- static inline instead of function-like macros ------------------------------------ */

/*
 * C89 had only macros for "free" small helpers:
 *     #define SQUARE(x) ((x) * (x))     SQUARE(i++) increments twice!
 * C99 inline functions are type-checked and evaluate their argument once.
 */
static inline int square(int x)
{
    return x * x;
}

/* --- _Generic (C11): one name, picked by argument type at compile time ------------------ */

const char *describe_int(int value);
const char *describe_double(double value);
const char *describe_string(const char *value);

/* C89: you'd call describe_int/describe_double yourself. C++ would overload. */
#define describe(value)             \
    _Generic((value),               \
        int: describe_int,          \
        double: describe_double,    \
        char *: describe_string,    \
        const char *: describe_string)(value)

/* --- _Static_assert (C11): checked by the compiler, costs nothing at run time -------- */

/* C89 trick: typedef char check_int_is_32_bits[(sizeof(int) == 4) ? 1 : -1]; */
_Static_assert(sizeof(int32_t) == 4, "int32_t must be exactly 4 bytes");
/* C23: static_assert(sizeof(int32_t) == 4);   (keyword, message optional) */

/* --- flexible array member (C99): struct + variable-length tail in one malloc --------- */

typedef struct {
    size_t length;
    int items[]; /* C89 "struct hack": int items[1]; and over-allocate. Undefined behaviour! */
} IntList;

IntList *int_list_of(size_t length, const int *values);
int int_list_sum(const IntList *list);
void int_list_free(IntList *list);

/* --- restrict (C99): promise to the compiler that these pointers don't overlap ---------- */

void add_arrays(size_t n, int *restrict out, const int *restrict a, const int *restrict b);

/* --- C23 preview, for when gcc 14+ makes -std=c23 the norm (not compiled here) ----------
 *     bool ok = true;                   no <stdbool.h>
 *     int *p = nullptr;                 typed null instead of NULL / 0
 *     constexpr int SIZE = 3;           real compile-time constant instead of #define / enum
 *     typeof(x) copy = x;               standard typeof
 *     [[nodiscard]] int parse(...);     attributes: warn if the result is ignored
 *     int mask = 0b1010'0101;           binary literals + digit separators
 */

#endif
