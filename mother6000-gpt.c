#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <locale.h>
#include <ctype.h>
#include "alien-console.h"

// Definitions from splash.c and other files
struct splash_layout {
    int maxx, maxy; // from ncurses
    int splash_lines;
    int splash_width;
    int top; // amount of blank space at the top
    int bottom; // amount of blank space below splash
    int splash_start_x; // where to start printing splash
    int tagline_start_x; // where to start printing tagline
    int copyright_start_x; // where to start printing copyright
};

char splash_contents[1024];

static int max_line_width(char *splash) {
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

static int splash_compute_layout(const struct splash_params *params, struct splash_layout *layout) {
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

static void splash_sleep(int maxx) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 25000000;
    nanosleep(&ts, NULL);
}

static void splash_display(const struct splash_params *params, const struct splash_layout *layout) {
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

static pid_t play_startup_sound(const struct splash_params *params) {
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

static int splash_load(const char *file) {
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

int splash(const struct splash_params *params) {
    struct splash_layout layout;
    pid_t sound;

    if (splash_load(params->file) < 0) {
        mark_error();
        return -1;
    }

    if (splash_compute_layout(params, &layout) < 0) {
        mark_error();
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

// Define _mark_error function
void _mark_error(const char *file, const char *func, int line) {
    fprintf(stderr, "Error in file %s, function %s, line %d\n", file, func, line);
}

// Existing functions and definitions
void display_matrix_cascade();
int display_prompt_screen();
void to_upper_case(char *str);
void display_help_screen();
void display_system_diagnostics();
void display_access_info(char *info);
void display_execute_order(int order);
void display_journal();

int main() {
    // Set locale to support special characters
    setlocale(LC_ALL, "");

    initscr();              // Initialize the window
    cbreak();               // Disable line buffering
    noecho();               // Turn off echo
    keypad(stdscr, TRUE);   // Enable function keys
    curs_set(0);            // Hide the cursor

    struct splash_params params = {
        .file = "splash-alt.txt",
        .tagline = "AN SMLINK PRODUCT",
        .copyright = "(c) 2328 SMDATA SYSTEMS",
        .audio_player = "aplay"
    };

    // Wait for the Return key to be pressed
    while (1) {
        int ch = getch();
        if (ch == '\n') {
            break;
        }
    }

    // Call Splash Screen
   splash(&params);
   sleep(3);

    // Display matrix cascade
   display_matrix_cascade();
   sleep(4);

    // Prompt screen
    while (1) {
        if (display_prompt_screen() == 0) {
            break;
        }
    }

    endwin(); // End curses mode
    return 0;
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
            for (int j = 0; j < speeds[i]; j++) {
                mvprintw(positions[i], i, "%c", rand() % 94 + 33);
                positions[i] = (positions[i] + 1) % rows;
            }
        }
        refresh();
        usleep(50000);
    }
}

void display_screen_border() {
    int columns = 79;
    int rows = 23;
    
    for (int i = 0; i < columns; i++) {
        mvprintw(0, i, "-");
        mvprintw(rows, i, "-");
    }

    for (int j = 0; j <= rows; j++) {
        if ((j == 0) || (j == rows)) {
            mvprintw(j, 0, "+");
            mvprintw(j, columns, "+"); 
        } else {
            mvprintw(j, 0, "|");
           mvprintw(j, columns, "|");
        }
    }

}

int display_prompt_screen() {
    int columns = 80;
    clear();
    display_screen_border();
    char response[256];
    char *directives[] = {
        "DIRECTIVE X34J FOR SCIENCE OFFICER EYES ONLY"
    };
    char *execute_orders[] = {
        "EMERGENCY SELF DESTRUCT"
    };
    char *crew_info[] = {
        "CREW MEMBER 35: SCIENCE OFFICER, NAME: ALONSO, AGE: 34"
    };
    int line_start = 1;

    mvprintw(1, line_start, "MOTHER 6000 INTERFACE");
    for (int i = 1; i < columns -1; i++) {
        mvprintw(2, i, "-");
    }
    mvprintw(3, line_start, "WHAT IS YOUR DIRECTIVE:");
    mvprintw(4, line_start, "? ");
    refresh();

    echo();
    curs_set(1);
    mvgetnstr(4, line_start + 1, response, sizeof(response) - 1); curs_set(0);
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
          display_help_screen();
    } else if (strcmp(response, "SYSTEM DIAGNOSTIC") == 0) {
          display_system_diagnostics(); 
    } else if (strcmp(response, "EXIT") == 0) {
        return 0; // Exit command
    } else {
        mvprintw(0, 0, "UNKNOWN COMMAND");
    }
    refresh();
    getch();
    return 1; // Continue looping
}


void display_help_screen() {
    int columns = 80;
    int tab = 5;
    display_screen_border();
    mvprintw(1, 1, "AVAILABLE COMMANDS:");
    for (int i = 1; i < columns - 1; i++) {
        mvprintw(2, i, "-");
    }
    mvprintw(3, 1, "WHAT IS DIRECTIVE XXXXXX");
    mvprintw(4, tab, "- Lookup command to access directives");
    mvprintw(5, 1, "EXECUTE ORDER XXX");
    mvprintw(6, tab, "- Command to perform terminal order");
    mvprintw(7, 1, "ACCESS INFO XXX");
    mvprintw(8, tab, "- Access Data stored in mainframe");
    mvprintw(9, 1, "SYSTEM DIAGNOSTIC");
    mvprintw(10, tab, " - Display ship systems and current status");
    mvprintw(11, 1, "HELP");
    mvprintw(12, tab, "- List all terminal commands");
    mvprintw(22, 1, "Press RETURN for terminal...");
}

void display_system_diagnostics() {
    int columns = 80;
    const char *systems[] = {
        "Airlock System",
        "Security System",
        "Oxygen Status",
        "Engine Status",
        "COMMUNICATIONS",
        "Life Support Systems",
        "Hull Integrity"
    };
    const char *statuses[] = {
        "ONLINE",
        "ONLINE",
        "Full",
        "STANDBY",
        "OFFLINE",
        "OPERATIONAL",
        "STABLE"
    };
    const int num_systems = sizeof(systems) / sizeof(systems[0]);

    display_screen_border();
    mvprintw(1, 1, "SYSTEM DIAGNOSTICS:");
    for (int i = 1; i < columns - 1; i++) {
        mvprintw(2, i, "-");
    }

    for (int i = 0; i < num_systems; i++) {
        int y = 3 + i;
        mvprintw(y, 1, "Checking %s", systems[i]);
        refresh();
        
        for (int percent = 0; percent <= 100; percent++) {
            mvprintw(y, 15 + strlen(systems[i]), "%3d%%", percent);
            refresh();
            usleep(20000); // 20000 microseconds = 20 milliseconds, total of 2 seconds for 100 steps
        }

        // Clear the line to remove any leftover characters
        move(y, 0);
        clrtoeol();
        
        mvprintw(y, 1, "%s: %s", systems[i], statuses[i]);
        refresh();
    }
    
    mvprintw(22, 1, "Press RETURN for terminal...");
}

// void display_system_diagnostics() {
    // int columns = 80;
    // display_screen_border();
    // mvprintw(1, 1, "SYSTEM DIAGNOSTICS:");
    // for (int i = 1; i < columns - 1; i++) {
        // mvprintw(2, i, "-");
    // }
    // mvprintw(3, 1, "Airlock System: ONLINE");
    // mvprintw(4, 1, "Security System: ONLINE");
    // mvprintw(5, 1, "Oxygen Status: Full");
    // mvprintw(6, 1, "Engine Status: STANDBY");
    // mvprintw(7, 1, "COMMUNICATIONS: OFFLINE");
    // mvprintw(8, 1, "Life Support Systems: OPERATIONAL");
    // mvprintw(9, 1, "Hull Integrity: STABLE");
    // mvprintw(22, 1, "Press RETURN for terminal...");
// }


void display_access_info(char* info) {
// display access info
}


void display_execute_order(int order) {
// display order execution
}


void display_journal() {
// display user journal
}

void to_upper_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}
