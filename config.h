#ifndef CONFIG_H
#define CONFIG_H

// limit definitions
#define MAX_SYSTEMS 10
#define MAX_INFO 10
#define MAX_DIRECTIVES 20
#define MAX_ORDERS 20
#define MAX_JOURNALS 20
#define MAX_LINE_LENGTH 256

// Typedefs
typedef struct {
    char system[MAX_LINE_LENGTH];
    char status[MAX_LINE_LENGTH];
} SystemStatus;

typedef struct {
    char name[MAX_LINE_LENGTH];
    char filename[MAX_LINE_LENGTH];
} AccessInfo;

typedef struct {
    char name[MAX_LINE_LENGTH];
    char details[MAX_LINE_LENGTH];
} Directive;

typedef struct {
    char name[MAX_LINE_LENGTH];
    char helper_text[MAX_LINE_LENGTH];
} Order;

typedef struct {
    char title[MAX_LINE_LENGTH];
    char file[MAX_LINE_LENGTH];
} JournalEntry;


// Config Data
extern AccessInfo access_info[MAX_INFO];
extern SystemStatus systems[MAX_SYSTEMS];
extern Directive directives[MAX_DIRECTIVES];
extern Order orders[MAX_ORDERS];
extern JournalEntry journals[MAX_JOURNALS];

// Base Data
extern char *interface_name;
extern char *prompt_display_text;
extern char *prompt_input_char;
extern char *splash_file;
extern char *splash_tagline;
extern char *splash_copyright;
extern char *splash_audio_player;
extern char *unrecognized_command;


// Size of structs for dynamic addtion in code
extern const int num_systems;
extern const int num_access_infos;
extern const int num_directives;
extern const int num_orders;
extern const int num_journals;

#endif // CONFIG_H

