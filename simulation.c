#include "simulation.h"

#include <stdlib.h>

#include "body.h"
#include "vector2.h"

const float CHAR_WIDTH = 3.0f;
const float STEP = 0.005f;
// const float STEP = 0.0005f;
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
            // if (d > 50)
            //     continue;
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

void summon_galaxy(Grid *grid, Vector2 position) {
    for (int x = position.x - GALAXY_RADIUS; x < position.x + GALAXY_RADIUS; x++) {
        for (int y = position.y - GALAXY_RADIUS; y < position.y + GALAXY_RADIUS; y++) {
        }
    }
}
