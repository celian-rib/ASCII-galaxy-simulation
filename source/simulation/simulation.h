#ifndef SIMULATION
#define SIMULATION

#include "body.h"
#include "vector2.h"

extern const float CHAR_WIDTH;
extern const float STEP;
extern const chtype SYMBOLS[3];

typedef struct {
    const float STEP;
    const int RING_COUNT;
    const int RING_SPACING;
    const int RING_BODY_MULTIPLIER;
} Simulation;

void simulate(const float STEP, const Grid *grid);

void summon_galaxy(const Simulation *simulation, Grid *grid, const Vector2 *position, const Vector2 *initial_velocity);

#endif
