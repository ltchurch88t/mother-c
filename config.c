#include "config.h"


// Base data
char *interface_name = "Mother 6000 Terminal Interface";
char *prompt_display_text = "What is your directive: [Type \"help\" to display all commands]";
char *prompt_input_char = "? ";
char *splash_file = "etc/splash/splash-alt.txt";
char *splash_tagline = "AN SMLINK PRODUCT";
char *splash_copyright = "(c) 2328 SMDATA SYSTEMS";
char *splash_audio_player = "aplay";
char *unrecognized_command = "Command not recognized...";
 

// Config Data
AccessInfo access_info[MAX_INFO] = {
    {"Info 1: Crew Member 35", "etc/info/info-35.txt"},
    {"Info 2: Engineering Report", "etc/info/engineering-report.txt"},
    {"Info 3: Security Log", "etc/info/security-log.txt"}
};


SystemStatus systems[MAX_SYSTEMS] = {
    {"Airlock System", "ONLINE"},
    {"Security System", "ONLINE"},
    {"Oxygen Status", "FULL"},
    {"Engine Status", "STANDBY"},
    {"Communication System", "OFFLINE"},
    {"Life Support Systems", "OPERATIONAL"},
    {"Hull Integrity", "STABLE"}
};

Directive directives[MAX_DIRECTIVES] = {
    {"Directive X34J", "This directive is for science officer eyes only. Enter code for further details."},
    {"Directive Y12K", "This directive outlines the emergency evacuation procedures for the entire crew. Follow the steps precisely to ensure safety."},
    {"Directive Z45M", "This directive details the maintenance routines for critical ship systems. Ensure compliance to avoid malfunctions."},
    {"Directive AS88", "This is a new directive to test functionality."}
};

Order orders[MAX_ORDERS] = {
    {"1. Shutdown Engines", "This order will shut down all ship engines, effectively stopping all propulsion."},
    {"2. Open Airlock", "This order will open the ship's airlock, which can be dangerous if not used properly."},
    {"3. Close Airlock", "This order will close the ship's airlock to ensure the ship is sealed."},
    {"4. Emergency Self Destruct", "This order will initiate the self-destruct sequence, destroying the ship."},
    {"5. New Order", "This is to test adding orders in."}
};


JournalEntry journals[MAX_JOURNALS] = {
    {"1. Entry 1: First Contact", "etc/entries/journal1.txt"},
    {"2. Entry 2: Alien Encounter", "etc/entries/journal2.txt"},
    {"3. Entry 3: System Malfunction", "etc/entries/journal3.txt"}
};


// Size of structs for dynamic addtion in code
const int num_systems = sizeof systems / sizeof systems[0];
const int num_access_infos = sizeof access_info / sizeof access_info[0];
const int num_directives = sizeof(directives) / sizeof(directives[0]);
const int num_orders = sizeof(orders) / sizeof(orders[0]);
const int num_journals = sizeof(journals) / sizeof(journals[0]);


