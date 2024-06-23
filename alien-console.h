/**
 * Definitions for alien-console.
 * Copyright (c) 2017 Stephen Brennan. Released under the Revised BSD License.
 */
#ifndef ALIEN_CONSOLE_H
#define ALIEN_CONSOLE_H

#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <sys/types.h>

/*
 * UTILITIES
 */
#define nelem(x) (sizeof(x) / sizeof(x[0]))
int count_lines(char *);

/*
 * SPLASH SCREEN
 */
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

struct splash_params {
    const char *file;
    const char *tagline;
    const char *copyright;
    const char *audio_player;
};

/*
 * CONFIGURATION
 */
#define SYSTEM_CONFIG "/etc/alien-console/alien-console.conf"
#define DEFAULT_CONFIG "/usr/share/alien-console/alien-console.conf"
struct pt_entry {
    char *folder;
    char *title;
    FILE *content;
};

struct pt_params {
    struct splash_params splash;
    struct pt_entry entries[4];
    int num_entries;
};

int parse_config(const char *filename, struct pt_params *params);
void cleanup_config(struct pt_params *params);

/*
 * SPLASH SCREEN
 */
int splash(const struct splash_params *params);
int splash_load(const char *file);
int splash_compute_layout(const struct splash_params *params, struct splash_layout *layout);
void splash_display(const struct splash_params *params, const struct splash_layout *layout);
pid_t play_startup_sound(const struct splash_params *params);
void splash_sleep(int maxx);

/*
 * PERSONAL TERMINAL
 */
int personal_terminal(struct pt_params *params);

/*
 * ERROR HANDLING (see error.c for an overview)
 */
enum error {
    /* UPDATES TO THIS TABLE MUST UPDATE error_values TABLE IN error.c */
    ESYS         = -1,
    NO_ERROR     = 0,
    ENARROW      = 1,
    ESHORT       = 2,
    EBIGFILE     = 3,
    EBIGTEXT     = 4,
    ECONFREAD    = 5,
    ECONFPARSE   = 6,
    ECONFSET     = 7,
    EMEM         = 8,
    E2MANY       = 9,
    EBADFILE     = 10,
};

const char *error_string(void);
int get_error(void);
#define error_is_set() (get_error() != NO_ERROR)
void clear_error(void);
void report_error(FILE *f);

void _mark_error(const char *file, const char *func, int line);
#define mark_error() _mark_error(__FILE__, __func__, __LINE__)

void _set_error(int error, const char *file, const char *func, int line);
#define set_error(error) _set_error(error, __FILE__, __func__, __LINE__)

/*
 * MATRIX CASCADE
 */
void display_matrix_cascade(void);

/*
 * DIRECTIVES
 */
void display_directive_details(const char *directive_name, const char *directive_details);
void display_directives(void);

/*
 * EXECUTE ORDER
 */
void display_execute_order(void);
void display_order_progress(const char *order_name);

/*
 * JOURNAL
 */
void display_journal(void);
void display_journal_entry(const char *filename);

/*
 * ACCESS INFO
 */
void display_access_info(void);

/*
 * SCREEN HELPERS
 */
void display_screen_border(void);
void display_help_screen(void);
void display_system_diagnostics(void);

/*
 * GENERAL HELPERS
 */
void to_upper_case(char *str);

#endif /* ALIEN_CONSOLE_H */

