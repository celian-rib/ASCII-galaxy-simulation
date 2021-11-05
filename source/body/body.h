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

#endif
