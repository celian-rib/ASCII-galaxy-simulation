#ifndef SIMULATION
#define SIMULATION

#include "body.h"
#include "vector2.h"

extern const float CHAR_WIDTH;

typedef struct {
    int rows;
    int cols;
    int count;
    Body *bodys;
} Grid;

typedef struct {
    Grid *grid;
    const float STEP;
    const int RING_COUNT;
    const int RING_SPACING;
    const int RING_BODY_MULTIPLIER;
} Simulation;

void simulate(const Simulation *sim);

void summon_galaxy(Simulation *simulation, const Vector2 *position, const Vector2 *initial_velocity);

#endif
