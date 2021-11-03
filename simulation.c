#include "simulation.h"

#include <math.h>
#include <stdlib.h>

#include "body.h"
#include "vector2.h"

const chtype SYMBOLS[3] = {'X', '#', 'O'};
const float CHAR_WIDTH = 3.0f;

const float STEP = 0.05f;

const int RING_COUNT = 5;
const int RING_SPACING = 2;
const int RING_BODY_MULTIPLIER = 40;

void simulate(const Grid *grid) {
    for (int i = 0; i < grid->count; i++) {
        Body *body = &(grid->bodys[i]);
        Vector2 acceleration = {0, 0};
        for (int j = 0; j < grid->count; j++) {
            if (i == j)
                continue;
            Body *other = &(grid->bodys[j]);
            float d = bodys_distance(body, other);
            
            // Vector2 dir = *direction(&(body->position), &(other->position));
            // normalize(&dir);
            // body->acceleration.x += 1 / ((d * d) + 1) * (dir.x / d);
            // body->acceleration.y += 1 / ((d * d) + 1) * (dir.y / d);

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

void summon_galaxy(Grid *grid, const Vector2 *position, const Vector2 *initial_velocity) {
    for (int r = 1; r < RING_COUNT + 1; r++) {

        float distance_to_center = r * RING_SPACING;
        int body_count_on_ring = r * RING_BODY_MULTIPLIER;

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
                    .x = initial_velocity->x + body_speed * -sin(angle) * 40,
                    .y = initial_velocity->y + body_speed * cos(angle) * 40,
                },
                .symbol = symbol,
            };

            grid->bodys = realloc(grid->bodys, (grid->count + 1) * sizeof(Body));
            grid->bodys[grid->count++] = new_body;
        }
    }

    //         // TO DO DENSITY EN FCT DE LA DISTANCE
    //         // TO DO SPEED EN FCT DE LA DISTANCE

    // Body new_body = {
    //     .position = {x, y},
    //     .symbol = SYMBOLS[rand() % 3],
    // };

    // Vector2 dir_to_center = *direction(&new_body.position, position);
    // normalize(&dir_to_center);

    // // Rotate 90 degree dir_to_center to get perpendicular direction
    // Vector2 start_direction = {
    //     .x = -dir_to_center.y,
    //     .y = dir_to_center.x,
    // };

    // print_vector(position, "center position");
    // print_vector(&new_body.position, "b position");
    // print_vector(&dir_to_center, "b -> center dir");
    // print_vector(&start_direction, "start direction");

    // new_body.speed.x = start_direction.x / ((GALAXY_RADIUS - d) + 1) * 400;
    // new_body.speed.y = start_direction.y / ((GALAXY_RADIUS - d) + 1) * 400;
    // new_body.speed.x += initial_velocity->x;
    // new_body.speed.y += initial_velocity->y;
}
