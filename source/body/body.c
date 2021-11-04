#include "body.h"
#include "vector2.h"
#include <math.h>

Vector2 normalize_direction(const Body *b, const Body *a) {
    Vector2 dir = {a->position.x - b->position.x, a->position.y - b->position.y};
    float magnitude = sqrt(dir.x * dir.x + dir.y * dir.y);
    Vector2 n = {.x = dir.x / magnitude, .y = dir.y / magnitude};
    return n;
}

void normalize(Vector2 *v) {
    float magnitude = sqrt((v->x * v->x) + (v->y * v->y));
    v->x /= magnitude;
    v->y /= magnitude;
}

Vector2 *direction(const Vector2 *a, const Vector2 *b) {
    Vector2 dir = {
        .x = a->x - b->x,
        .y = a->y - b->y,
    };
    return &dir;
}

float distance(int ya, int xa, int yb, int xb) {
    return (float)sqrt(pow(xa - xb, 2) + pow(ya - yb, 2));
}

float bodys_distance(const Body *a, const Body *b) {
    return (float)distance(a->position.y, a->position.x, b->position.y, b->position.x);
}

float vector_distance(const Vector2 *a, const Vector2 *b) {
    return (float)distance(a->y, a->x, b->y, b->x);
}

/*
{ BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 0, BreakBeforeBraces: Custom, BraceWrapping: {
AfterEnum : false, AfterStruct : false, AfterUnion : false }}
*/
