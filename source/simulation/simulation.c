#include "simulation.h"

#include <math.h>
#include <stdlib.h>

#include "body.h"
#include "vector2.h"

const chtype SYMBOLS[3] = {'X', '#', 'O'};
const float CHAR_WIDTH = 3.0f;

void simulate(const float STEP, const Grid *grid) {
    for (int i = 0; i < grid->count; i++) {
        Body *body = &(grid->bodys[i]);
        Vector2 acceleration = {0, 0};
        for (int j = 0; j < grid->count; j++) {
            if (i == j)
                continue;

            Body *other = &(grid->bodys[j]);
            float d = bodys_distance(body, other);

            Vector2 normalized_dir = normalize_direction(body, other);
            acceleration.x += (normalized_dir.x / (d + 0.1f));
            acceleration.y += (normalized_dir.y / (d + 0.1f));
        }
        body->speed.x += acceleration.x * STEP;
        body->speed.y += acceleration.y * STEP;
        body->position.x += body->speed.x * STEP;
        body->position.y += body->speed.y * STEP;
    }
}

void print_vector(const Vector2 *v, char *name) {
    printf("%s (%f, %f)\n", name, v->x, v->y);
}

void summon_galaxy(const Simulation *simulation, Grid *grid, const Vector2 *position, const Vector2 *initial_velocity) {
    for (int r = 1; r < simulation->RING_COUNT + 1; r++) {

        float distance_to_center = r * simulation->RING_SPACING;
        int body_count_on_ring = r * simulation->RING_BODY_MULTIPLIER;

        float angle_between_bodies = 2 * M_PI / body_count_on_ring;
        float body_speed = sqrt(1 / distance_to_center);

        chtype symbol = SYMBOLS[(rand() % 4) - 1];

        for (int i = 0; i < body_count_on_ring; i++) {
            float angle = i * angle_between_bodies;
            Body new_body = {
                // Set position around the current ring
                .position = {
                    .x = position->x + (distance_to_center * cos(angle)), 
                    .y = position->y + (distance_to_center * sin(angle)),
                },
                // Set position perpendicular to the ring normal
                .speed = {
                    .x = initial_velocity->x + body_speed * -sin(angle) * 30,
                    .y = initial_velocity->y + body_speed * cos(angle) * 30,
                },
                .symbol = symbol,
            };
            grid->bodys = realloc(grid->bodys, (grid->count + 1) * sizeof(Body));
            grid->bodys[grid->count++] = new_body;
        }
    }
}
