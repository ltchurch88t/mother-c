#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Include for sleep
#include <time.h>
#include "alien-console.h"
#include "config.h"

#define ESC_KEY 27

int display_prompt_screen(void);


int nanosleep(const struct timespec *req, struct timespec *rem);

void display_splash_and_matrix(void) {
    struct splash_params params = {
        .file = splash_file,
        .tagline = splash_tagline,
        .copyright = splash_copyright,
        .audio_player = splash_audio_player
    };

    struct timespec ts;
    ts.tv_sec = 0;           // 0 seconds
    ts.tv_nsec = 100000000L;   // 5000 microseconds (5 milliseconds) in nanoseconds

    timeout(100); // Set timeout for getch to 100 ms
    int ch;

    for (int i = 0; i < 30; i++) { // Approximate 3 seconds with 100ms intervals
        ch = getch();
        if (ch == ESC_KEY) {
            timeout(-1); // Disable timeout
            return;
        }
        if (i == 0) splash(&params); // Display splash screen only once
        nanosleep(&ts, NULL); // Sleep for 100 ms
    }

    for (int i = 0; i < 40; i++) { // Approximate 4 seconds with 100ms intervals
        ch = getch();
        if (ch == ESC_KEY) {
            timeout(-1); // Disable timeout
            return;
        }
        if (i == 0) display_matrix_cascade(); // Display matrix cascade only once
        nanosleep(&ts, NULL); // Sleep for 100 ms
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

    mvprintw(1, line_start, interface_name);
    for (int i = 1; i < cols - 1; i++) {
        mvprintw(2, i, "-");
    }
    mvprintw(3, line_start, prompt_display_text);
    mvprintw(4, line_start, prompt_input_char);
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
        mvprintw(0, 0, unrecognized_command);
    }
    refresh();
    getch();
    return 1; // Continue looping
}

