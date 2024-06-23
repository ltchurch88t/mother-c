#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "alien-console.h"

void display_matrix_cascade(void) {
    clear();
    srand(time(NULL));
    int columns = COLS;
    int rows = LINES;
    int speeds[columns];
    int positions[columns];

    // Initialize the speeds and positions
    for (int i = 0; i < columns; i++) {
        speeds[i] = rand() % 3 + 1; // Random speed between 1 and 3
        positions[i] = rand() % rows;
    }

    // Define the duration of the cascade effect
    int duration = 3; // Duration in seconds
    time_t start_time = time(NULL);

    while (difftime(time(NULL), start_time) < duration) {
        clear();
        for (int i = 0; i < columns; i++) {
            for (int j = 0; j < speeds[i]; j++) {
                mvprintw(positions[i], i, "%c", rand() % 94 + 33);
                positions[i] = (positions[i] + 1) % rows;
            }
        }
        refresh();
        usleep(50000);
    }
}

