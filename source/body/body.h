#ifndef BODY
#define BODY

#include "../utils/vector2.h"
#include <ncurses.h>

typedef struct {
    chtype symbol;
    Vector2 position;
    Vector2 speed;
    Vector2 acceleration;
    struct Body *closests;
} Body;

typedef struct {
    int rows;
    int cols;
    int count;
    Body *bodys;
} Grid;

Vector2 normalize_direction();

void normalize(Vector2 *v);
Vector2 *direction(const Vector2 *a, const Vector2 *b);

float distance(int ya, int xa, int yb, int xb);
float bodys_distance(const Body *a, const Body *b);
float vector_distance(const Vector2 *a, const Vector2 *b);

#endif
