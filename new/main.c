#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Include for sleep
#include "alien-console.h"

#define ESC_KEY 27

int display_prompt_screen(void);

void display_splash_and_matrix(void) {
    struct splash_params params = {
        .file = "etc/splash/splash-alt.txt",
        .tagline = "AN SMLINK PRODUCT",
        .copyright = "(c) 2328 SMDATA SYSTEMS",
        .audio_player = "aplay"
    };

    timeout(100); // Set timeout for getch to 100 ms
    int ch;

    for (int i = 0; i < 30; i++) { // Approximate 3 seconds with 100ms intervals
        ch = getch();
        if (ch == ESC_KEY) {
            timeout(-1); // Disable timeout
            return;
        }
        if (i == 0) splash(&params); // Display splash screen only once
        usleep(100000); // Sleep for 100 ms
    }

    for (int i = 0; i < 40; i++) { // Approximate 4 seconds with 100ms intervals
        ch = getch();
        if (ch == ESC_KEY) {
            timeout(-1); // Disable timeout
            return;
        }
        if (i == 0) display_matrix_cascade(); // Display matrix cascade only once
        usleep(100000); // Sleep for 100 ms
    }

    timeout(-1); // Disable timeout
}

int main(void) {
    // Set locale to support special characters
    setlocale(LC_ALL, "");

    initscr();              // Initialize the window
    cbreak();               // Disable line buffering
    noecho();               // Turn off echo
    keypad(stdscr, TRUE);   // Enable function keys
    curs_set(0);            // Hide the cursor

    mvprintw(0, 0, "Press RETURN to start...");
    refresh();

    while (1) {
        int ch = getch();
        if (ch == '\n') {
            break;
        }
    }

    clear();
    refresh();

    display_splash_and_matrix();

    // Prompt screen
    while (1) {
        if (display_prompt_screen() == 0) {
            break;
        }
    }

    endwin(); // End curses mode
    return 0;
}

int display_prompt_screen(void) {
    int cols, rows;
    getmaxyx(stdscr, rows, cols); // Get the current screen size
    clear();
    display_screen_border();
    char response[256];
    int line_start = 1;

    mvprintw(1, line_start, "MOTHER 6000 INTERFACE");
    for (int i = 1; i < cols - 1; i++) {
        mvprintw(2, i, "-");
    }
    mvprintw(3, line_start, "WHAT IS YOUR DIRECTIVE: [Type \"help\" for command list]");
    mvprintw(4, line_start, "? ");
    refresh();

    echo();
    curs_set(1);
    mvgetnstr(4, line_start + 1, response, sizeof(response) - 1); 
    curs_set(0);
    noecho();

    to_upper_case(response);

    clear();

    if (strncmp(response, "DIRECTIVES", 10) == 0) {
        display_directives();
    } else if (strncmp(response, "EXECUTE ORDER", 13) == 0) {
        display_execute_order();
    } else if (strncmp(response, "ACCESS INFO", 11) == 0) {
        display_access_info();
    } else if (strcmp(response, "HELP") == 0) {
        display_help_screen();
    } else if (strcmp(response, "SYSTEM DIAGNOSTIC") == 0) {
        display_system_diagnostics();
    } else if (strcmp(response, "JOURNAL") == 0) {
        display_journal();
    } else if (strcmp(response, "EXIT") == 0) {
        return 0; // Exit command
    } else {
        mvprintw(0, 0, "UNKNOWN COMMAND");
    }
    refresh();
    getch();
    return 1; // Continue looping
}

