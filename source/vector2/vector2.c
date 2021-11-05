#include "vector2.h"
#include "body.h"
#include <math.h>

Vector2 normalize_direction(const Vector2 *a, const Vector2 *b) {
    Vector2 dir = {b->x - a->x, b->y - a->y};
    float magnitude = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (magnitude == 0)
        return (Vector2){0, 0};
    return (Vector2){.x = dir.x / magnitude, .y = dir.y / magnitude};
}

float distance(int ya, int xa, int yb, int xb) {
    return (float)sqrt(pow(xa - xb, 2) + pow(ya - yb, 2));
}

float vector_distance(const Vector2 *a, const Vector2 *b) {
    return (float)distance(a->y, a->x, b->y, b->x);
}
