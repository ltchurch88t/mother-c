#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "alien-console.h"
#include "config.h"

void display_journal_entry(const char *filename) {
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
    int wrap_width = 50;  // Fixed wrap width for narrower display
    int start_line = 0;

    while (1) {
        clear();
        display_screen_border();
        mvprintw(1, 1, "JOURNAL ENTRY:");

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
        int line_count = 0;
        while (line_end && line_count < max_lines - 4) {
            char *next_newline = strchr(line_end, '\n');
            if (next_newline && (next_newline - line_end) <= wrap_width) {
                line_end = next_newline + 1;
            } else {
                line_end += wrap_width;
                if (line_end > file_contents + strlen(file_contents)) {
                    line_end = file_contents + strlen(file_contents);
                } else {
                    while (*line_end != ' ' && *line_end != '\n' && line_end > line_start) {
                        line_end--;
                    }
                    if (*line_end == ' ') {
                        line_end++;
                    }
                }
            }
            mvprintw(3 + line_count, 1, "%.*s", (int)(line_end - line_start), line_start);
            line_start = line_end;
            line_count++;
        }

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

void display_journal(void) {
    int selected = 0;

    while (1) {
        clear();
        display_screen_border();
        mvprintw(1, 1, "JOURNAL ENTRIES:");

        for (int i = 0; i < num_journals; i++) {
            if (i == selected) {
                attron(A_REVERSE);
                mvprintw(3 + i, 1, "%s", journals[i].title);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, 1, "%s", journals[i].title);
            }
        }

        mvprintw(22, 1, "Press number key (1-3) to select, qq to exit");
        refresh();

        int ch = getch();
        if (ch >= '1' && ch <= '3') {
            int index = ch - '1';
            if (index < num_journals) {
                display_journal_entry(journals[index].file);
            }
        } else if (ch == 'q' || ch == 'Q') {
            break;
        }
    }
}

