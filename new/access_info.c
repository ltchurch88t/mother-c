#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "alien-console.h"

struct AccessInfo {
    const char *name;
    const char *filename;
};

struct AccessInfo access_infos[] = {
    {"Info 1: Crew Member 35", "etc/info/info-35.txt"},
    {"Info 2: Engineering Report", "etc/info/engineering-report.txt"},
    {"Info 3: Security Log", "etc/info/security-log.txt"}
};

const int num_access_infos = sizeof(access_infos) / sizeof(access_infos[0]);

void display_access_info_content(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return;
    }

    char file_contents[4096];
    size_t read_size = fread(file_contents, sizeof(char), sizeof(file_contents) - 1, fp);
    if (read_size == 0 && ferror(fp)) {
        perror("fread");
        fclose(fp);
        return;
    }
    file_contents[read_size] = '\0';
    fclose(fp);

    int max_lines, max_cols;
    getmaxyx(stdscr, max_lines, max_cols);
    int start_line = 0;

    while (1) {
        clear();
        display_screen_border();
        mvprintw(1, 1, "ACCESS INFO:");

        char *line_start = file_contents;
        for (int i = 0; i < start_line; i++) {
            line_start = strchr(line_start, '\n');
            if (line_start) {
                line_start++;
            } else {
                line_start = file_contents + strlen(file_contents);
                break;
            }
        }

        char *line_end = line_start;
        for (int i = 0; i < max_lines - 4; i++) {
            line_end = strchr(line_end, '\n');
            if (line_end) {
                line_end++;
            } else {
                line_end = file_contents + strlen(file_contents);
                break;
            }
        }

        mvprintw(3, 1, "%.*s", (int)(line_end - line_start), line_start);

        mvprintw(max_lines - 2, 1, "Press UP/DOWN to scroll, RETURN to exit");
        refresh();

        int ch = getch();
        if (ch == '\n') {
            break;
        } else if (ch == KEY_UP && start_line > 0) {
            start_line--;
        } else if (ch == KEY_DOWN && line_end < file_contents + strlen(file_contents)) {
            start_line++;
        }
    }
}

void display_access_info(void) {
    int selected = 0;

    while (1) {
        clear();
        display_screen_border();
        mvprintw(1, 1, "ACCESS INFO:");

        for (int i = 0; i < num_access_infos; i++) {
            if (i == selected) {
                attron(A_REVERSE);
                mvprintw(3 + i, 1, "%s", access_infos[i].name);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, 1, "%s", access_infos[i].name);
            }
        }

        mvprintw(22, 1, "Use UP/DOWN to navigate, RETURN to select, Q to exit");
        refresh();

        int ch = getch();
        if (ch == '\n') {
            display_access_info_content(access_infos[selected].filename);
        } else if (ch == KEY_UP && selected > 0) {
            selected--;
        } else if (ch == KEY_DOWN && selected < num_access_infos - 1) {
            selected++;
        } else if (ch == 'q' || ch == 'Q') {
            break;
        }
    }
}

