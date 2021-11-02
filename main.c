#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "simulation.h"

int simulated_steps = 1;
double last_draw_time = 0;
double simulation_time = 0;

const int DISABLE_CURSES = 0;

void draw_header(const Grid *grid) {
    char str[grid->cols];
    sprintf(str, "Simulated steps: %d", simulated_steps);
    mvaddstr(0, 5, str);
    sprintf(str, "Simulation time: %fms", simulation_time * 1000);
    mvaddstr(0, grid->cols / 3, str);
    sprintf(str, "Body count : %d", grid->count);
    mvaddstr(0, 2 * (grid->cols / 3), str);
    sprintf(str, "Draw time: %fms", last_draw_time * 1000);
    mvaddstr(0, grid->cols - 30, str);
}

void draw_grid(const Grid *grid) {
    for (int i = 0; i < grid->count; i++)
        mvaddch(grid->bodys[i].position.y, grid->bodys[i].position.x * CHAR_WIDTH,
                grid->bodys[i].symbol);
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

    if (DISABLE_CURSES)
        endwin();

    // init_grid_bodys(&grid);
    Vector2 center = {
        .x = grid.cols / (2 * CHAR_WIDTH),
        .y = grid.rows / 2,
    };
    summon_galaxy(&grid, &center);

    if (!DISABLE_CURSES) {
        draw_grid(&grid);
        refresh();
    }
    while (1) {
        if (!DISABLE_CURSES)
            clear();
        clock_t begin = clock();
        simulate(&grid);
        simulated_steps++;
        clock_t end = clock();
        simulation_time = (double)(end - begin) / CLOCKS_PER_SEC;

        begin = clock();
        if (!DISABLE_CURSES) {
            draw_grid(&grid);
            draw_header(&grid);
            refresh();
        }
        end = clock();
        last_draw_time = (double)(end - begin) / CLOCKS_PER_SEC;
    }

    endwin();
    return 0;
}
