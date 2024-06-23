#include <ncurses.h>
#include <string.h>
#include "alien-console.h"

struct Directive {
    const char *name;
    const char *details;
};

struct Directive directives[] = {
    {"Directive X34J", "This directive is for science officer eyes only. Enter code for further details."},
    {"Directive Y12K", "This directive outlines the emergency evacuation procedures for the entire crew. Follow the steps precisely to ensure safety."},
    {"Directive Z45M", "This directive details the maintenance routines for critical ship systems. Ensure compliance to avoid malfunctions."},
};

const int num_directives = sizeof(directives) / sizeof(directives[0]);

void display_override_details(void) {
    const char *override_details = "This is the additional information for Directive X34J.";
    int max_lines, max_cols;
    getmaxyx(stdscr, max_lines, max_cols);

    clear();
    display_screen_border();
    mvprintw(1, 1, "OVERRIDE DETAILS:");
    mvprintw(3, 1, "%s", override_details);
    mvprintw(max_lines - 2, 1, "Press RETURN to go back");
    refresh();
    
    getch(); // Wait for the user to press a key
}

void prompt_for_code(void) {
    const char *correct_code = "SCIENCE123";
    char input_code[256];

    clear();
    display_screen_border();
    mvprintw(1, 1, "Enter Science Officer Code:");
    echo();
    curs_set(1);
    mvgetnstr(3, 1, input_code, sizeof(input_code) - 1);
    curs_set(0);
    noecho();

    if (strcmp(input_code, correct_code) == 0) {
        display_override_details();
    } else {
        mvprintw(5, 1, "Incorrect code. Press RETURN to go back.");
        refresh();
        getch();
    }
}

void display_directive_details(const char *directive_name, const char *directive_details) {
    int max_lines, max_cols;
    getmaxyx(stdscr, max_lines, max_cols);

    clear();
    display_screen_border();
    mvprintw(1, 1, "DIRECTIVE DETAILS:");
    mvprintw(3, 1, "%s", directive_details);

    if (strcmp(directive_name, "Directive X34J") == 0) {
        mvprintw(max_lines - 3, 1, "Press 'O' to override with a code.");
    }

    mvprintw(max_lines - 2, 1, "Press RETURN to go back");
    refresh();

    int ch = getch();
    if (ch == 'o' || ch == 'O') {
        prompt_for_code();
    }
}

void display_directives(void) {
    int selected = 0;

    while (1) {
        clear();
        display_screen_border();
        mvprintw(1, 1, "DIRECTIVES:");

        for (int i = 0; i < num_directives; i++) {
            if (i == selected) {
                attron(A_REVERSE);
                mvprintw(3 + i, 1, "%s", directives[i].name);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, 1, "%s", directives[i].name);
            }
        }

        mvprintw(22, 1, "Use UP/DOWN to navigate, RETURN to select, Q to exit");
        refresh();

        int ch = getch();
        if (ch == '\n') {
            display_directive_details(directives[selected].name, directives[selected].details);
        } else if (ch == KEY_UP && selected > 0) {
            selected--;
        } else if (ch == KEY_DOWN && selected < num_directives - 1) {
            selected++;
        } else if (ch == 'q' || ch == 'Q') {
            break;
        }
    }
}

