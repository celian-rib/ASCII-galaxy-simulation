#include "simulation.h"

#include <math.h>
#include <stdlib.h>

#include "body.h"
#include "vector2.h"

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
        Body *body = &(sim->grid->bodys[i]);
        Vector2 acceleration = {0, 0};

        if (!DISABLE_GRAVITY)
            for (int j = 0; j < sim->grid->count; j++) {
                if (i == j)
                    continue;

                Body *other = &(sim->grid->bodys[j]);
                float d = bodys_distance(body, other);

                if (d == 0) // just ignore overlapping bodies
                    continue;

                Vector2 normalized_dir = normalize_direction(&(body->position), &(other->position));
                acceleration.x += (normalized_dir.x / (d + 0.1f));
                acceleration.y += (normalized_dir.y / (d + 0.1f));
            }

        body->speed.x += acceleration.x * sim->STEP;
        body->speed.y += acceleration.y * sim->STEP;
        body->position.x += body->speed.x * sim->STEP;
        body->position.y += body->speed.y * sim->STEP;
    }
}


void summon_galaxy(Simulation *sim, const Vector2 *position, const Vector2 *initial_velocity) {
    for (int i = 0; i < 800; i++) {
        // Angle
        float theta = ((rand() % 1000) / 1000.0f) * 2 * M_PI;
        // Normalized distance to center
        float d = (rand() % 1000) / 1000.0f;
        Vector2 body_pos = {d * cos(theta), d * sin(theta)};

        if(body_pos.x == 0 && body_pos.y == 0) // Prevent 0, 0 spawning -> nan vectors
            continue;
        
        int position_taken = 0;
        for (int j = 0; j < sim->grid->count; j++) {
            Vector2 b = sim->grid->bodys[j].position;
            if (b.x == body_pos.x && b.y == body_pos.y)
                position_taken = 1;
        }
        if(position_taken)
            continue;

        int radius = 15;
        body_pos.x *= radius;
        body_pos.y *= radius;
        body_pos.x += position->x;
        body_pos.y += position->y;

        float orbital_speed = sqrt(1 / (d * radius)) * 40;

        Vector2 dir_to_center = normalize_direction(&body_pos, position);
        print_vector(&dir_to_center, "dir");

        //Rotate 90 degree dir_to_center to get perpendicular direction
        Vector2 start_direction = {
            .x = -dir_to_center.y,
            .y = dir_to_center.x,
        };

        Body new_body = {
            // Set position around the current ring
            .position = body_pos,
            .speed = {
                .x = start_direction.x * orbital_speed,
                .y = start_direction.y * orbital_speed,
            },
        };
        // printf("%f\n", theta);
        print_vector(&(new_body.speed), "speed");
        sim->grid->bodys = realloc(sim->grid->bodys, (sim->grid->count + 1) * sizeof(Body));
        sim->grid->bodys[sim->grid->count++] = new_body;
    }
    

    /////////////////////////////////////////////////////////////////////////////////////////////////
    // for (int r = 1; r < sim->RING_COUNT + 1; r++) {

    //     float distance_to_center = r * sim->RING_SPACING;
    //     int body_count_on_ring = r * sim->RING_BODY_MULTIPLIER;

    //     float angle_between_bodies = 2 * M_PI / body_count_on_ring;
    //     float body_speed = sqrt(1 / distance_to_center);

    //     chtype symbol = SYMBOLS[(rand() % 4) - 1];

    //     for (int i = 0; i < body_count_on_ring; i++) {
    //         float angle = i * angle_between_bodies;
    //         Body new_body = {
    //             // Set position around the current ring
    //             .position = {
    //                 .x = position->x + (distance_to_center * cos(angle)),
    //                 .y = position->y + (distance_to_center * sin(angle)),
    //             },
    //             // Set position perpendicular to the ring normal
    //             .speed = {
    //                 .x = initial_velocity->x + body_speed * -sin(angle) * 30,
    //                 .y = initial_velocity->y + body_speed * cos(angle) * 30,
    //             },
    //             .symbol = symbol,
    //         };
    //         sim->grid->bodys = realloc(sim->grid->bodys, (sim->grid->count + 1) * sizeof(Body));
    //         sim->grid->bodys[sim->grid->count++] = new_body;
    //     }
    // }
}
