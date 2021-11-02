#ifndef SIMULATION
#define SIMULATION

#include "body.h"

extern const float CHAR_WIDTH;
extern const float STEP;
extern const int GALAXY_RADIUS;
extern const chtype SYMBOLS[3];

void simulate(const Grid *grid);

void init_grid_bodys(Grid *grid);

#endif
