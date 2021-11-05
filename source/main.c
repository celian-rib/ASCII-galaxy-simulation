#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "simulation.h"

#define RESET "\033[0m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define RED "\033[31m"

// Number of step that have been simulated so far
int simulated_steps = 1;
double last_draw_time = 0;
double simulation_time = 0;

// 1 = The simulation will not be displayed
// (Usefull for printf debugging)
const int DISABLE_CURSES = 0;

/**
 * @brief Draw to the screen header with the current simulation infos.
 *
 * @param grid grid related to the current simulation.
 */
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

/**
 * @brief Draw command help
 */
void draw_help() {
    printf(RED "\nUnkown argument...\nusage : %s./galaxy <arg>\n\n" RESET, CYAN);
    printf("%20s : Summon a single galaxy.\n", YELLOW "single" RESET);
    printf("%20s : Summon two galaxies that will collide.\n", YELLOW "collision" RESET);
    printf("\n");
}

/**
 * @brief Draw the actual state of the provided grid
 * @param grid grid to draw
 */
void draw_grid(const Grid *grid) {
    for (int i = 0; i < grid->count; i++)
        mvaddch(grid->bodys[i].position.y, grid->bodys[i].position.x * CHAR_WIDTH, 'X');
}

/**
 * @brief Handle the simulation step by step, also focus on collecting stats and refreshing the
 * screen
 *
 * @param sim related simulation
 */
void main_loop(const Simulation *sim) {
    if (!DISABLE_CURSES) {
        draw_grid(sim->grid);
        refresh();
    } else {
        endwin();
    }
    while (1) {
        clock_t begin = clock();

        if (!DISABLE_CURSES)
            clear();

        simulate(sim);
        simulated_steps++;

        clock_t end = clock();
        simulation_time = (double)(end - begin) / CLOCKS_PER_SEC;
        begin = clock();

        if (!DISABLE_CURSES) {
            draw_grid(sim->grid);
            draw_header(sim->grid);
            refresh();
        }

        end = clock();
        last_draw_time = (double)(end - begin) / CLOCKS_PER_SEC;
    }
}

/**
 * @brief Start a single galaxy simulation
 * @param grid grid to use
 */
void single_galaxy_simulation(Grid *grid) {
    Simulation sim = {
        .grid = grid,
        .STEP = 0.05f,
        .GALAXY_BODY_COUNT = 800,
        .GALAXY_RADIUS = 15,
    };
    Vector2 pos = {
        .x = grid->cols / (2 * CHAR_WIDTH),
        .y = grid->rows / 2,
    };
    summon_galaxy(&sim, &pos);
    main_loop(&sim);
}

/**
 * @brief Start a colliding galaxies simulation
 * @param grid grid to use
 */
void collision_galaxy_simulation(Grid *grid) {
    Simulation sim = {
        .grid = grid,
        .STEP = 0.05f,
        .GALAXY_BODY_COUNT = 400,
        .GALAXY_RADIUS = 15,
    };
    Vector2 pos1 = {
        .x = (grid->cols / (2 * CHAR_WIDTH)) / 2,
        .y = grid->rows / 2,
    };
    Vector2 pos2 = {
        .x = ((grid->cols / (2 * CHAR_WIDTH)) / 2) * 3,
        .y = grid->rows / 2,
    };
    summon_galaxy(&sim, &pos1);
    summon_galaxy(&sim, &pos2);
    main_loop(&sim);
}

int main(int argc, char *argv[]) {

    // Setup ncurses
    initscr();
    curs_set(FALSE);

    // Setup grid
    Grid grid = {
        .bodys = NULL,
        .count = 0,
        .rows = 60,
        .cols = 60,
    };

    getmaxyx(stdscr, grid.rows, grid.cols);

    if (DISABLE_CURSES)
        endwin();

    if (argc <= 1) {
        endwin();
        draw_help();
        return 127;
    } else if (!strcmp(argv[1], "single")) {
        single_galaxy_simulation(&grid);
    } else if (!strcmp(argv[1], "collision")) {
        collision_galaxy_simulation(&grid);
    } else {
        endwin();
        draw_help();
        return 127;
    }

    // Disable ncurse
    endwin();
    free(grid.bodys);
    return 0;
}
