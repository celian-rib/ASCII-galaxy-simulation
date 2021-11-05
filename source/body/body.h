#ifndef BODY
#define BODY

#include "vector2.h"
#include <ncurses.h>

typedef struct {
    Vector2 position;
    Vector2 speed;
    Vector2 acceleration;
    struct Body *closests;
} Body;

Vector2 normalize_direction(const Vector2 *b, const Vector2 *a);

void normalize(Vector2 *v);
Vector2 *direction(const Vector2 *a, const Vector2 *b);

float distance(int ya, int xa, int yb, int xb);
float bodys_distance(const Body *a, const Body *b);
float vector_distance(const Vector2 *a, const Vector2 *b);

#endif
