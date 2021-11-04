#ifndef SIMULATION
#define SIMULATION

#include "../body/body.h"
#include "../utils/vector2.h"

extern const float CHAR_WIDTH;
extern const float STEP;
extern const int GALAXY_RADIUS;
extern const chtype SYMBOLS[3];

void simulate(const Grid *grid);

void summon_galaxy(Grid *grid, const Vector2 *position, const Vector2 *initial_velocity);

#endif