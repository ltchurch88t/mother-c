#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>  // Include necessary header
#include "alien-console.h"

char splash_contents[1024];

int max_line_width(char *splash) {
    int max_line_width = 0;
    int line_width = 0;
    for (; *splash != '\0'; splash++) {
        if (*splash == '\n') {
            if (line_width > max_line_width)
                max_line_width = line_width;
            line_width = 0;
        } else {
            line_width++;
        }
    }
    return max_line_width;
}

int splash_load(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    fread(splash_contents, sizeof(char), sizeof(splash_contents) - 1, fp);
    splash_contents[sizeof(splash_contents) - 1] = '\0';
    fclose(fp);
    return 0;
}

int splash_compute_layout(const struct splash_params *params, struct splash_layout *layout) {
    getmaxyx(stdscr, layout->maxy, layout->maxx);
    layout->splash_lines = 12; // number of lines in splash screen
    layout->splash_width = max_line_width(splash_contents);
    layout->top = 1;
    layout->bottom = 1;
    layout->splash_start_x = (layout->maxx - layout->splash_width) / 2;
    layout->tagline_start_x = layout->splash_start_x + layout->splash_width - strlen(params->tagline);
    layout->copyright_start_x = (layout->maxx - strlen(params->copyright)) / 2;
    return 0;
}

void splash_sleep(int maxx) {
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 25000000};
    nanosleep(&ts, NULL);
}

void splash_display(const struct splash_params *params, const struct splash_layout *layout) {
    clear();

    // Center the splash text vertically
    int y = (layout->maxy - layout->splash_lines) / 2;
    char *splash_ptr = splash_contents;
    int line_length;
    while (*splash_ptr != '\0') {
        line_length = strchr(splash_ptr, '\n') - splash_ptr;
        mvaddnstr(y++, layout->splash_start_x, splash_ptr, line_length);
        splash_ptr += line_length + 1;
    }

    // Display tagline
    mvaddstr(y++, layout->tagline_start_x, params->tagline);

    // Display copyright line
    y = layout->maxy - 2; // One line above the bottom
    mvaddstr(y++, layout->copyright_start_x, params->copyright);

    // Display the loading bar at the bottom
    y = layout->maxy - 1;
    move(y, 0);
    refresh();
    for (int x = 0; x < layout->maxx; x++) {
        splash_sleep(layout->maxx);
        addch(' ' | A_REVERSE);
        refresh();
    }
    splash_sleep(layout->maxx); // for good measure
}

pid_t play_startup_sound(const struct splash_params *params) {
    pid_t pid = fork();
    if (pid == 0) {
        // child
        char *cmd[] = {
            (char *)params->audio_player,  // Cast to char *
            "/var/local/console.wav",
            NULL
        };
        int nul = open("/dev/null", O_WRONLY);
        dup2(nul, STDERR_FILENO);
        dup2(nul, STDOUT_FILENO);
        if (execvp(cmd[0], cmd) == -1) {
            exit(5);
        }
        exit(0);
    }
    return pid;
}

int splash(const struct splash_params *params) {  // Changed return type to int
    struct splash_layout layout;
    pid_t sound;

    if (splash_load(params->file) < 0) {
        return -1;
    }

    if (splash_compute_layout(params, &layout) < 0) {
        return -1;
    }

    sound = play_startup_sound(params);
    splash_display(params, &layout);
    if (sound > 0) {
        int status;
        waitpid(sound, &status, 0);
    }
    return 0;
}

