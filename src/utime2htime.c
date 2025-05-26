#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// ANSI Color Codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

// Colors
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bright colors
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_CYAN    "\033[96m"

void print_header() {
    printf("\n");
    printf(BRIGHT_CYAN "┌──────────────────────────────────────────┐\n" RESET);
    printf(BRIGHT_CYAN "│" BRIGHT_YELLOW "     Unix Timestamp to Human Time       " BRIGHT_CYAN "│\n" RESET);
    printf(BRIGHT_CYAN "│" DIM "           by AnmiTaliDev               " BRIGHT_CYAN "│\n" RESET);
    printf(BRIGHT_CYAN "└──────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

void print_usage(const char *program_name) {
    print_header();
    
    printf(BOLD BLUE "USAGE:\n" RESET);
    printf("  %s " YELLOW "<unix_timestamp>" RESET " " DIM "[--utc]\n\n" RESET, program_name);
    
    printf(BOLD BLUE "EXAMPLES:\n" RESET);
    printf("  %s " CYAN "1716556245\n" RESET, program_name);
    printf("  %s " CYAN "1716556245" RESET " " MAGENTA "--utc\n" RESET, program_name);
    printf("  %s " CYAN "0" RESET "          " DIM "# Unix epoch\n" RESET, program_name);
    printf("  %s " CYAN "2147483647" RESET "   " DIM "# Max 32-bit timestamp\n\n" RESET, program_name);
    
    printf(BOLD BLUE "OPTIONS:\n" RESET);
    printf("  " MAGENTA "--utc" RESET "     Show time in UTC instead of local time\n");
    printf("  " MAGENTA "-h, --help" RESET " Show this help message\n\n");
    
    printf(BOLD BLUE "OUTPUT FORMAT:\n" RESET);
    printf("  " YELLOW "YYYY-MM-DD HH:MM:SS" RESET " " DIM "[timezone]\n\n" RESET);
}

void print_error(const char *message) {
    printf(BRIGHT_RED "Error: " RESET "%s\n\n", message);
}

void print_result(time_t unix_time, const char *formatted_time, int is_utc) {
    printf("\n");
    printf(BRIGHT_GREEN "┌─ Conversion Result ─────────────────────┐\n" RESET);
    printf(BRIGHT_GREEN "│\n" RESET);
    printf(BRIGHT_GREEN "│ " RESET BOLD "Unix Time:  " RESET BRIGHT_YELLOW "%ld\n" RESET, unix_time);
    printf(BRIGHT_GREEN "│ " RESET BOLD "Human Time: " RESET CYAN "%s", formatted_time);
    if (is_utc) {
        printf(" " MAGENTA "UTC" RESET);
    }
    printf("\n" RESET);
    
    // Show additional info
    if (!is_utc) {
        struct tm *utc_tm = gmtime(&unix_time);
        if (utc_tm != NULL) {
            char utc_str[32];
            strftime(utc_str, sizeof(utc_str), "%Y-%m-%d %H:%M:%S", utc_tm);
            printf(BRIGHT_GREEN "│ " RESET BOLD "UTC:        " RESET MAGENTA "%s UTC\n" RESET, utc_str);
        }
    }
    
    // Show day of week
    struct tm *tm_info = is_utc ? gmtime(&unix_time) : localtime(&unix_time);
    if (tm_info != NULL) {
        char day_str[32];
        strftime(day_str, sizeof(day_str), "%A", tm_info);
        printf(BRIGHT_GREEN "│ " RESET BOLD "Day:        " RESET WHITE "%s\n" RESET, day_str);
    }
    
    printf(BRIGHT_GREEN "│\n" RESET);
    printf(BRIGHT_GREEN "└─────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Check for help flags
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    
    // Parse Unix timestamp
    char *endptr;
    errno = 0;
    long unix_timestamp = strtol(argv[1], &endptr, 10);
    
    if (errno != 0 || *endptr != '\0' || endptr == argv[1]) {
        print_error("Invalid Unix timestamp. Must be a valid integer.");
        printf(DIM "Example: 1716556245\n\n" RESET);
        return 1;
    }
    
    // Check for valid timestamp range
    if (unix_timestamp < 0) {
        print_error("Unix timestamp cannot be negative.");
        return 1;
    }
    
    // Check for --utc flag
    int use_utc = 0;
    if (argc == 3) {
        if (strcmp(argv[2], "--utc") == 0) {
            use_utc = 1;
        } else {
            print_error("Unknown option. Use --utc for UTC time.");
            printf(DIM "Run with -h for help.\n\n" RESET);
            return 1;
        }
    }
    
    time_t timestamp = (time_t)unix_timestamp;
    struct tm *tm_info;
    
    if (use_utc) {
        tm_info = gmtime(&timestamp);
    } else {
        tm_info = localtime(&timestamp);
    }
    
    if (tm_info == NULL) {
        print_error("Failed to convert Unix timestamp. Value may be out of range.");
        return 1;
    }
    
    // Format: YYYY-MM-DD HH:MM:SS
    char formatted_time[32];
    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", tm_info);
    
    print_result(timestamp, formatted_time, use_utc);
    return 0;
}