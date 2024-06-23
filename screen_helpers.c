#include <ncurses.h>
#include <time.h>
#include "alien-console.h"


int nanosleep(const struct timespec *req, struct timespec *rem);

void display_screen_border(void) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols); // Get the current screen size

    for (int i = 0; i < cols; i++) {
        mvprintw(0, i, "-");
        mvprintw(rows - 1, i, "-");
    }

    for (int j = 0; j < rows; j++) {
        if ((j == 0) || (j == rows - 1)) {
            mvprintw(j, 0, "+");
            mvprintw(j, cols - 1, "+");
        } else {
            mvprintw(j, 0, "|");
            mvprintw(j, cols - 1, "|");
        }
    }
}


void display_help_screen(void) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int tab = 5;
    display_screen_border();
    mvprintw(1, 1, "AVAILABLE COMMANDS:");
    for (int i = 1; i < cols - 1; i++) {
        mvprintw(2, i, "-");
    }
    mvprintw(3, 1, "DIRECTIVES");
    mvprintw(4, tab, "- Lookup command to access directives");
    mvprintw(5, 1, "EXECUTE ORDER");
    mvprintw(6, tab, "- Command to perform terminal order");
    mvprintw(7, 1, "ACCESS INFO");
    mvprintw(8, tab, "- Access Data stored in mainframe");
    mvprintw(9, 1, "JOURNAL");
    mvprintw(10, tab, "- Access entries in memory");
    mvprintw(11, 1, "SYSTEM DIAGNOSTIC");
    mvprintw(12, tab, "- Display ship systems and current status");
    mvprintw(13, 1, "HELP");
    mvprintw(14, tab, "- List all terminal commands");
    mvprintw(rows - 2, 1, "Press RETURN for terminal...");
}

void display_system_diagnostics(void) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    const char *systems[] = {
        "Airlock System",
        "Security System",
        "Oxygen Status",
        "Engine Status",
        "Communication Systems",
        "Life Support Systems",
        "Hull Integrity"
    };
    const char *statuses[] = {
        "ONLINE",
        "ONLINE",
        "FULL",
        "STANDBY",
        "OFFLINE",
        "OPERATIONAL",
        "STABLE"
    };
    const int num_systems = sizeof(systems) / sizeof(systems[0]);

    struct timespec ts;
    ts.tv_sec = 0;           // 0 seconds
    ts.tv_nsec = 20000000L;   // 5000 microseconds (5 milliseconds) in nanoseconds

    display_screen_border();
    mvprintw(1, 1, "SYSTEM DIAGNOSTICS:");
    for (int i = 1; i < cols - 1; i++) {
        mvprintw(2, i, "-");
    }

    for (int i = 0; i < num_systems; i++) {
        int y = 3 + i;
        mvprintw(y, 1, "Checking %s", systems[i]);
        refresh();
        
        for (int percent = 0; percent <= 100; percent++) {
            mvprintw(y, 15 + strlen(systems[i]), "%3d%%", percent);
            refresh();
            nanosleep(&ts, NULL); // 20000 microseconds = 20 milliseconds, total of 2 seconds for 100 steps
        }

        // Clear the line to remove any leftover characters
        move(y, 0);
        clrtoeol();
        
        mvprintw(y, 1, "%s: %s", systems[i], statuses[i]);
        refresh();
    }
    
    mvprintw(rows - 2, 1, "Press RETURN for terminal...");
}

