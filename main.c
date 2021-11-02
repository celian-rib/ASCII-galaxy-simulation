#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const float CHAR_WIDTH = 3.0f;
const float STEP = 0.005f;
const chtype SYMBOLS[3] = {'X', '@', 'O'};
const int GALAXY_RADIUS = 20;

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    chtype symbol;
    Vector2 position;
    Vector2 speed;
    struct Body *closests;
} Body;

typedef struct {
    int rows;
    int cols;
    int count;
    Body *bodys;
} Grid;

float distance(int ya, int xa, int yb, int xb) {
    return (float)sqrt(pow(xa - xb, 2) + pow(ya - yb, 2));
}

float bodys_distance(const Body *a, const Body *b) {
    return (float)distance(a->position.y, a->position.x, b->position.y, b->position.x);
}

float sqrt_distance(int ya, int xa, int yb, int xb) {
    return (float)(pow(xa - xb, 2) + pow(ya - yb, 2));
}

float sqrt_bodys_distance(const Body *a, const Body *b) {
    return (float)sqrt_distance(a->position.y, a->position.x, b->position.y, b->position.x);
}

Vector2 normalize_direction(const Body *b, const Body *a) {
    Vector2 dir = {a->position.x - b->position.x,
                   a->position.y - b->position.y};
    float magnitude = sqrt(dir.x * dir.x + dir.y * dir.y);
    Vector2 n = {
        .x = dir.x / magnitude,
        .y = dir.y / magnitude};
    return n;
}

void draw_header(const Grid *grid, int gen, double simulation_time, double last_draw_time) {
    char str[grid->cols];
    sprintf(str, "Generation : %d", gen);
    mvaddstr(0, 5, str);
    sprintf(str, "Simulation time : %fms", simulation_time * 1000);
    mvaddstr(0, grid->cols / 3, str);
    sprintf(str, "Body count : %d", grid->count);
    mvaddstr(0, 2 * (grid->cols / 3), str);
    sprintf(str, "Draw time : %fms", last_draw_time * 1000);
    mvaddstr(0, grid->cols - 30, str);
}

void draw_grid(const Grid *grid) {
    for (int i = 0; i < grid->count; i++)
        mvaddch(grid->bodys[i].position.y, grid->bodys[i].position.x * CHAR_WIDTH, grid->bodys[i].symbol);
}

void simulate(const Grid *grid) {
    for (int i = 0; i < grid->count; i++) {
        Body *body = &(grid->bodys[i]);
        Vector2 acceleration = {0, 0};
        for (int j = 0; j < grid->count; j++) {
            if (i == j) continue;
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
        .position = {.x = grid->cols / (2 * CHAR_WIDTH), .y = grid->rows / 2}};

    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            float d = distance(i, j, grid->rows / 2, grid->cols / (2 * CHAR_WIDTH));
            if (d > GALAXY_RADIUS)
                continue;
            // if (rand() % 2)
            //     continue;
            grid->bodys = realloc(grid->bodys, (grid->count + 1) * sizeof(Body));

            Body b = {
                .position = {
                    .x = j,
                    .y = i},
                .symbol = SYMBOLS[rand() % 3]};
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

int main() {
    initscr();
    curs_set(FALSE);

    Grid grid = {
        .bodys = NULL,
        .count = 0,
        .rows = 20,
        .cols = 20,
    };
    getmaxyx(stdscr, grid.rows, grid.cols);

    init_grid_bodys(&grid);
    // grid.bodys = malloc(2 * sizeof(Body));
    // grid.bodys[0] = b1;
    // grid.bodys[1] = b2;

    // grid.bodys = realloc(grid.bodys, 2 * sizeof(Body));
    // grid.bodys[0] = b1;
    // grid.bodys[1] = b2;

    int gen = 1;
    draw_grid(&grid);
    refresh();
    double last_draw_time = 0;
    while (1) {
        clear();
        clock_t begin = clock();
        simulate(&grid);
        clock_t end = clock();
        double simulation_time = (double)(end - begin) / CLOCKS_PER_SEC;

        begin = clock();
        draw_grid(&grid);
        end = clock();
        last_draw_time = (double)(end - begin) / CLOCKS_PER_SEC;

        draw_header(&grid, gen++, simulation_time, last_draw_time);
        refresh();
    }

    endwin();
    return 0;
}
