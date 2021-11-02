#include "simulation.h"

#include <stdlib.h>

#include "body.h"
#include "vector2.h"

const float CHAR_WIDTH = 3.0f;
// const float STEP = 0.005f;
const float STEP = 0.005f;
const int GALAXY_RADIUS = 21;
const chtype SYMBOLS[3] = {'X', '@', 'O'};

void simulate(const Grid *grid) {
    for (int i = 0; i < grid->count; i++) {
        Body *body = &(grid->bodys[i]);
        Vector2 acceleration = {0, 0};
        for (int j = 0; j < grid->count; j++) {
            if (i == j)
                continue;
            Body *other = &(grid->bodys[j]);
            float d = bodys_distance(body, other);
            if (d > 50)
                continue;
            Vector2 normalized_dir = normalize_direction(body, other);
            acceleration.x += (normalized_dir.x / (d + 10)) * 100;
            acceleration.y += (normalized_dir.y / (d + 10)) * 100;
        }
        body->speed.x += acceleration.x * STEP;
        body->speed.y += acceleration.y * STEP;
        body->position.x += body->speed.x * STEP;
        body->position.y += body->speed.y * STEP;
    }
}

void init_grid_bodys(Grid *grid) {
    Body center = {
        .position = {
            .x = grid->cols / (2 * CHAR_WIDTH),
            .y = grid->rows / 2,
        },
    };

    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            float d = distance(i, j, grid->rows / 2, grid->cols / (2 * CHAR_WIDTH));
            if (d > GALAXY_RADIUS)
                continue;
            // if (rand() % 2)
            //     continue;
            grid->bodys = realloc(grid->bodys, (grid->count + 1) * sizeof(Body));

            Body b = {.position = {.x = j, .y = i}, .symbol = SYMBOLS[rand() % 3]};
            Vector2 normalized_dir = normalize_direction(&b, &center);
            float distance = bodys_distance(&b, &center);
            b.speed.x = (-normalized_dir.y / ((GALAXY_RADIUS - distance) + 5)) * 800;
            b.speed.y = (normalized_dir.x / ((GALAXY_RADIUS - distance) + 5)) * 800;
            // b.speed.x = 0;
            b.speed.y -= 3;
            grid->bodys[grid->count++] = b;
        }
    }
}

void print_vector(const Vector2 *v, char *name) {
    printf("%s (%f, %f)\n", name, v->x, v->y);
}

void summon_galaxy(Grid *grid, const Vector2 *position) {
    for (int x = position->x - GALAXY_RADIUS; x < position->x + GALAXY_RADIUS; x++) {
        for (int y = position->y - GALAXY_RADIUS; y < position->y + GALAXY_RADIUS; y++) {
            float d = distance(y, x, position->y, position->x);

            if (d > GALAXY_RADIUS)
                continue;

            Body new_body = {
                .position = {x, y},
                .symbol = SYMBOLS[rand() % 3],
            };

            Vector2 dir_to_center = *direction(&new_body.position, position);
            normalize(&dir_to_center);

            //Rotate 90 degree dir_to_center to get perpendicular direction
            Vector2 start_direction = {
                .x = -dir_to_center.y,
                .y = dir_to_center.x,
            };

            print_vector(position, "center position");
            print_vector(&new_body.position, "b position");
            print_vector(&dir_to_center, "b -> center dir");
            print_vector(&start_direction, "start direction");

            new_body.speed.x = start_direction.x / (GALAXY_RADIUS - d) * 600;
            new_body.speed.y = start_direction.y / (GALAXY_RADIUS - d) * 600;

            grid->bodys = realloc(grid->bodys, (grid->count + 1) * sizeof(Body));
            grid->bodys[grid->count++] = new_body;
        }
    }
}
