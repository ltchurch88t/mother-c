#include <ncurses.h>
#include <time.h>
#include "alien-console.h"
#include "config.h"

int nanosleep(const struct timespec *req, struct timespec *rem);

void display_order_progress(const char *order_name) {
    int max_lines, max_cols;
    getmaxyx(stdscr, max_lines, max_cols);

    struct timespec ts;
    ts.tv_sec = 0;           // 0 seconds
    ts.tv_nsec = 20000000L;   // 5000 microseconds (5 milliseconds) in nanoseconds
    nanosleep(&ts, NULL);

    for (int i = 0; i <= 100; i++) {
        clear();
        mvprintw(max_lines / 2, (max_cols - strlen(order_name)) / 2, "%s", order_name);
        mvprintw(max_lines / 2 + 1, (max_cols - 20) / 2, "Progress: %3d%%", i);
        refresh();
        nanosleep(&ts, NULL); // 20 milliseconds, total of 2 seconds for 100 steps
    }

    clear();
    mvprintw(max_lines / 2, (max_cols - 20) / 2, "Order executed successfully!");
    refresh();
    nanosleep(&ts, NULL); // Display success message for 2 seconds
}

void display_execute_order(void) {
    int selected = 0;

    while (1) {
        clear();
        display_screen_border();
        mvprintw(1, 1, "EXECUTE ORDER:");

        for (int i = 0; i < num_orders; i++) {
            if (i == selected) {
                attron(A_REVERSE);
                mvprintw(3 + i, 1, "%s", orders[i].name);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, 1, "%s", orders[i].name);
            }
        }

        mvprintw(15, 1, "HELPER TEXT:");
        mvprintw(16, 1, "%s", orders[selected].helper_text);

        mvprintw(22, 1, "Use UP/DOWN to navigate, RETURN to select, qq to exit");
        refresh();

        int ch = getch();
        if (ch == '\n') {
            display_order_progress(orders[selected].name);
        } else if (ch == KEY_UP && selected > 0) {
            selected--;
        } else if (ch == KEY_DOWN && selected < num_orders - 1) {
            selected++;
        } else if (ch == 'q' || ch == 'Q') {
            break;
        }
    }
}

