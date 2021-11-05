#include "simulation.h"

#include <math.h>
#include <stdlib.h>

#include "body.h"
#include "vector2.h"

// Space used horizontaly for one char (This make the terminal aspect ratio close to 1:1)
const float CHAR_WIDTH = 3.0f;

const int DISABLE_SIMULATION = 0;
const int DISABLE_GRAVITY = 0;

void print_vector(const Vector2 *v, char *name) {
    printf("%s (%f, %f)\n", name, v->x, v->y);
}

void simulate(const Simulation *sim) {
    if (DISABLE_SIMULATION)
        return;
    for (int i = 0; i < sim->grid->count; i++) {
        Body *body = &sim->grid->bodys[i];
        Vector2 acceleration = {0, 0};

        if (!DISABLE_GRAVITY)
            for (int j = 0; j < sim->grid->count; j++) {
                if (i == j)
                    continue;

                Body *other = &sim->grid->bodys[j];
                float d = vector_distance(&body->position, &other->position);

                if (d == 0) // just ignore overlapping bodies
                    continue;

                Vector2 normalized_dir = normalize_direction(&body->position, &other->position);
                acceleration.x += (normalized_dir.x / (d + 1));
                acceleration.y += (normalized_dir.y / (d + 1));
            }

        body->speed.x += acceleration.x * sim->STEP;
        body->speed.y += acceleration.y * sim->STEP;
        body->position.x += body->speed.x * sim->STEP;
        body->position.y += body->speed.y * sim->STEP;
    }
}

void summon_galaxy(Simulation *sim, const Vector2 *position) {
    for (int i = 0; i < sim->GALAXY_BODY_COUNT; i++) {
        // Get a random point in the galaxy
        // Random angle
        float theta = ((rand() % 1000) / 1000.0f) * 2 * M_PI;
        // Normalized distance to center
        float d = (rand() % 1000) / 1000.0f;
        Vector2 body_pos = {d * cos(theta), d * sin(theta)};

        if (body_pos.x == 0 && body_pos.y == 0) // Prevent 0, 0 spawning -> nan vectors
            continue;

        int position_taken = 0;
        for (int j = 0; j < sim->grid->count; j++) {
            Vector2 b = sim->grid->bodys[j].position;
            if (b.x == body_pos.x && b.y == body_pos.y)
                position_taken = 1;
        }
        if (position_taken)
            continue;

        body_pos.x *= sim->GALAXY_RADIUS;
        body_pos.y *= sim->GALAXY_RADIUS;
        body_pos.x += position->x;
        body_pos.y += position->y;

        float orbital_speed
            = sqrt(1 / (d * sim->GALAXY_RADIUS)) * 40; // I dont known where this 40 come from yay

        Vector2 dir_to_center = normalize_direction(&body_pos, position);

        // Rotate 90 degree the vector pointing from the body to the galaxy center (dir_to_center)
        // This make a perpendicular direction (Initial direction of the body)
        Vector2 start_direction = {
            .x = -dir_to_center.y,
            .y = dir_to_center.x,
        };

        Body new_body = {
            .position = body_pos,
            .speed = {
                .x = start_direction.x * orbital_speed,
                .y = start_direction.y * orbital_speed,
            },
        };

        // Add the new created body to the list of all bodies
        sim->grid->bodys = realloc(sim->grid->bodys, (sim->grid->count + 1) * sizeof(Body));
        sim->grid->bodys[sim->grid->count++] = new_body;
    }
}
