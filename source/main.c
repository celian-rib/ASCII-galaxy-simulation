#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "simulation.h"

#define RESET "\033[0m"
#define CYAN "\033[36m"
#define YELLOW  "\033[33m"
#define RED "\033[31m"

// Number of step that have been simulated so far
int simulated_steps = 1;
double last_draw_time = 0;
double simulation_time = 0;

// 1 = Ncurse will not be used (Usefull for printf debugging)
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

void draw_help() {
    printf(RED"\nUnkown argument...\nusage : %s./galaxy%s or %s./galaxy <arg>\n\n"RESET, CYAN, RESET, CYAN);
    printf("%20s : Summon two galaxies with no specific mass at the center.\n", YELLOW"~none~"RESET);
    printf("%20s : Summon two galaxies that have blackhole at the center.\n", YELLOW"blackhole"RESET);
    printf("\n");
}

void draw_grid(const Grid *grid) {
    for (int i = 0; i < grid->count; i++)
        mvaddch(grid->bodys[i].position.y, grid->bodys[i].position.x * CHAR_WIDTH,
                grid->bodys[i].symbol);
}

int main(int argc, char *argv[]) {

    // Setup ncurses
    initscr();
    curs_set(FALSE);

    // Setup grid
    Grid grid = {
        .bodys = NULL,
        .count = 0,
        .rows = 20,
        .cols = 20,
    };
    getmaxyx(stdscr, grid.rows, grid.cols);

    if (argc <= 1) {
        Vector2 pos1 = {
            .x = (grid.cols / (2 * CHAR_WIDTH)) / 2,
            .y = grid.rows / 2,
        };
        Vector2 vel1 = {-5, 3};
        Vector2 pos2 = {
            .x = ((grid.cols / (2 * CHAR_WIDTH)) / 2) * 3,
            .y = grid.rows / 2,
        };
        Vector2 vel2 = {5, -3};
        summon_galaxy(&grid, &pos1, &vel1);
        summon_galaxy(&grid, &pos2, &vel2);
    } else if (!strcmp(argv[1], "blackhole")) {
        endwin();
        printf("Not implemented\n");
        return 0;
    } else {
        endwin();
        draw_help();
        return 127;
    }

    if (DISABLE_CURSES) {
    }
    if (!DISABLE_CURSES) {
        draw_grid(&grid);
        refresh();
    } else {
        endwin();
    }

    // Main loop
    while (1) {
        clock_t begin = clock();

        if (!DISABLE_CURSES)
            clear();

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

    // Disable ncurse
    endwin();
    free(grid.bodys);
    return 0;
}
