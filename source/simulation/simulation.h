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
    const int GALAXY_BODY_COUNT;
    const int GALAXY_RADIUS;
} Simulation;

/**
 * @brief Simulate one step with the given simulation object
 * 
 * @param sim simulation object to work with
 */
void simulate(const Simulation *sim);

/**
 * @brief Create a new galaxy into the given simulation object
 * 
 * @param simulation simulation in which the galaxy will be created
 * @param position position of the galaxy relative to the screen
 */
void summon_galaxy(Simulation *simulation, const Vector2 *position);

#endif
