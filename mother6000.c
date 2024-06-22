#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

void display_initial_loading_screen();
void display_matrix_cascade();
void display_prompt_screen();
void to_upper_case(char *str);

int main() {
    // Set locale to support special characters
    setlocale(LC_ALL, "");

    initscr();              // Initialize the window
    cbreak();               // Disable line buffering
    noecho();               // Turn off echo
    keypad(stdscr, TRUE);   // Enable function keys
    curs_set(0);            // Hide the cursor

    // Wait for the Return key to be pressed
    while (1) {
        int ch = getch();
        if (ch == '\n') {
            break;
        }
    }

    // Initial loading screen
    display_initial_loading_screen();
    sleep(3);

    // Display matrix cascade
    display_matrix_cascade();
    sleep(4);

    // Prompt screen
    display_prompt_screen();

    endwin(); // End curses mode
    return 0;
}

void display_initial_loading_screen() {
    clear();

    mvprintw(9, (COLS - strlen("CRONUSLINK")) / 2, "CRONUSLINK");
    mvprintw(13, (COLS - strlen("2328")) / 2, "2328");
    mvprintw(15, (COLS - strlen("AN SM-LINK PRODUCT")) / 2, "AN SM-LINK PRODUCT");
    mvprintw(16, (COLS - strlen("SM DATA SYSTEMS")) / 2, "SM DATA SYSTEMS");

    refresh();
}

void display_matrix_cascade() {
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
            int char_y = (positions[i] + (time(NULL) - start_time) * speeds[i]) % rows;
            char ch = rand() % (126 - 33 + 1) + 33; // Random character between '!' and '~'
            mvaddch(char_y, i, ch);
        }
        refresh();
        usleep(50000); // Sleep for 50 milliseconds
    }
}


void display_prompt_screen() {
    char *directives[] = {
        "X34J: FOR SCIENCE OFFICER EYES ONLY",
    };
    char *execute_orders[] = {
        "101: EXECUTING......\nORDER 101 EXECUTED SUCCESSFULLY",
    };
    char *crew_info[] = {
        "35: ACCESSING CREW INFO....\n| CREW MEMBER NAME: ALICE |\n| AGE: 26 |\n| POSITION: SHIP ENGINEER |\n| COMPANY: WEINLAND-YUTANI |\n| DUTIES TO PERFORM: MAINTAIN AND |\n| REPAIR SHIP AS NEEDED. EXECUTE |\n| FUNCTIONS TO RETAIN SHIP |\n| INTEGRITY. |\n| BONUS: 12% |"
    };

    char response[100];
    while (1) {
        clear();
        mvprintw(0, 0, "MOTHER 6000 INTERFACE\nWHAT IS YOUR DIRECTIVE:\n?");
        refresh();

        echo();
        curs_set(1);
        mvgetnstr(2, 1, response, sizeof(response) - 1);
        curs_set(0);
        noecho();

        to_upper_case(response);

        clear();

        if (strncmp(response, "WHAT IS DIRECTIVE ", 18) == 0) {
            char *directive = response + 18;
            if (strcmp(directive, "X34J") == 0) {
                mvprintw(0, 0, "DIRECTIVE %s: %s", directive, directives[0]);
            } else {
                mvprintw(0, 0, "NO MATCHING DIRECTIVE FOUND IN SYSTEM");
            }
        } else if (strncmp(response, "EXECUTE ORDER ", 14) == 0) {
            char *order = response + 14;
            if (strcmp(order, "101") == 0) {
                mvprintw(0, 0, "EXECUTE ORDER %s: %s", order, execute_orders[0]);
            } else {
                mvprintw(0, 0, "UNABLE TO EXECUTE ORDER %s", order);
            }
        } else if (strncmp(response, "ACCESS INFO ", 12) == 0) {
            char *info = response + 12;
            if (strcmp(info, "35") == 0) {
                mvprintw(0, 0, "%s", crew_info[0]);
            } else {
                mvprintw(0, 0, "NO CREW MEMBER INFO FOUND");
            }
        } else if (strcmp(response, "HELP") == 0) {
            mvprintw(0, 0, "AVAILABLE COMMANDS:\nWHAT IS DIRECTIVE XXXX\nEXECUTE ORDER XXX\nACCESS INFO XXX\nSYSTEM DIAGNOSTIC\nHELP");
        } else if (strcmp(response, "SYSTEM DIAGNOSTIC") == 0) {
            mvprintw(0, 0, "AIRLOCK SYSTEM: ONLINE\nSECURITY SYSTEM: ONLINE\nOXYGEN STATUS: FULL\nENGINE STATUS: STANDBY\nCOMMUNICATIONS: FUNCTIONAL\nLIFE SUPPORT: OPERATIONAL\nHULL INTEGRITY: STABLE");
        } else {
            mvprintw(0, 0, "UNKNOWN COMMAND");
        }
        refresh();
        getch();
    }
}

void to_upper_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}

